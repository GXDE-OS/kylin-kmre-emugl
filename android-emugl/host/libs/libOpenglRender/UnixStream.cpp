/*
* Copyright (C) 2011 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "UnixStream.h"

#include "emugl/common/sockets.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/syslog.h>

#include <string>
#include <pwd.h>

/* Not all systems define PATH_MAX, those who don't generally don't
 * have a limit on the maximum path size, so use a value that is
 * large enough for our very limited needs.
 */
#ifndef PATH_MAX
#define PATH_MAX   128
#endif

#define BUF_SIZE 4096

namespace emugl {
    
static std::string getUserName() {
    std::string userName;

    char user[1024] = {0};
    if (getlogin()) {
        sprintf(user, "%s", getlogin());
    } else {
        struct passwd  pwd;
        struct passwd* result = 0;
        char buf[1024];

        memset(&buf, 0, sizeof(buf));
        uint32_t uid = getuid();
        (void)getpwuid_r(uid, &pwd, buf, 1024, &result);
        if (!result) {
            syslog(LOG_DEBUG, "getpwnam_r error,uid = %d",uid);
            fprintf(stderr, "getpwnam_r error,uid = %d\n",uid);
        } 
        if (!(pwd.pw_name)) {
            fprintf(stderr, "Failed to get user name from uid.\n");
            syslog(LOG_ERR, "utils: Failed to get user name from uid.");
            return userName;
        }

        char* _user = pwd.pw_name;
        sprintf(user, "%s", _user);

        struct passwd  pwd1;
        struct passwd* result1 = 0;
        char buf1[1024];

        memset(&buf1, 0, sizeof(buf1));
        
        (void)getpwnam_r(_user, &pwd1, buf1, 1024, &result1);
        if (!result1) {
            syslog(LOG_DEBUG, "getpwnam_r error,userName = %s",_user);
            fprintf(stderr, "getpwnam_r error,userName = %s\n",_user);
        }
        if (pwd1.pw_uid != getuid()) {
            fprintf(stderr, "User name doesn't match uid.\n");
            syslog(LOG_ERR, "utils: User name doesn't match uid.");
            return userName;
        }
    }

    userName = std::string(user);
    return userName;
}

std::string makeContainerName(const std::string& userName, int uid) {
    std::string containerName;

    if (userName.length() == 0) {
        return containerName;
    }

    if (uid < 0) {
        return containerName;
    }

    containerName = "kmre-" + std::to_string(uid) + "-" + userName;

    return containerName;
}

static std::string convertUserNameToPath(const std::string& userName)
{
    char buffer[BUF_SIZE] = {0};
    std::string path = userName;
    unsigned int i = 0;
    const char* str = nullptr;

    str = userName.c_str();
    if (str && strstr(str, "\\")) {
        snprintf(buffer, sizeof(buffer), "%s", str);
        for (i = 0; i < sizeof(buffer); ++i) {
            if ('\0' == buffer[i]) {
                break;
            }

            if ('\\' == buffer[i]) {
                buffer[i] = '_';
            }
        }

        path = buffer;
    }

    return path;
}

std::string makeContainerName() {
    std::string userName;

    userName = convertUserNameToPath(getUserName());

    return makeContainerName(userName, getuid());
}



UnixStream::UnixStream(size_t bufSize) :
    ChannelStream(bufSize),
    bound_socket_path(NULL) {
}

UnixStream::UnixStream(int sock, size_t bufSize) :
    ChannelStream(sock, bufSize),
    bound_socket_path(NULL) {
}

UnixStream::~UnixStream() {
    if (bound_socket_path != NULL) {
      int ret = 0;
      do {
          ret = unlink(bound_socket_path);
      } while (ret < 0 && errno == EINTR);
      if(ret != 0) {
          ERR("Failed to unlink UNIX socket at \"%s\"\n", bound_socket_path);
          perror("UNIX socket could not be unlinked");
      }
      free(bound_socket_path);
    }
}

/* Initialize a sockaddr_un with the appropriate values corresponding
 * to a given 'virtual port'. Returns 0 on success, -1 on error.
 */
static int
make_unix_path(char *path, size_t  pathlen, int port_number) {
    int ret = 0;
    std::string containerName;
    std::string containerPath;

    containerName = makeContainerName();
    if (containerName.length() == 0) {
        return -1;
    }

    containerPath = "/var/lib/kmre/" + containerName;

    // Now, initialize it properly
    snprintf(path, pathlen, "%s/sockets/qemu_pipe", containerPath.c_str());

    // If the emulator is killed, it can leave the socket file behind.
    // Since the filename has PID in it, we can be sure that this socket
    // is not supposed to be here and delete it, to prevent EADDRINUSE
    // later in bind()
    if (::access(path, F_OK) == 0) {
        ret = ::remove(path);
        if (ret < 0) {
            ERR("Failed to remove stale socket file at %s: %s\n", path, strerror(errno));
        } else {
            DBG("Stale socket file at %s was removed.\n", path);
        }
    }

    return 0;
}


int UnixStream::listen(char addrstr[MAX_ADDRSTR_LEN]) {
    if (make_unix_path(addrstr, MAX_ADDRSTR_LEN, getpid()) < 0) {
        return -1;
    }

    m_sock = emugl::socketLocalServer(addrstr, SOCK_STREAM);

    if (!valid())
        return int(ERR_INVALID_SOCKET);

    bound_socket_path = strdup(addrstr);
    if(bound_socket_path != NULL)
        syslog(LOG_DEBUG,"bound_socket_path = %s",bound_socket_path);

    if(bound_socket_path == NULL) {
        ERR("WARNING: UNIX socket at \"%s\" should be manually removed \n",
            addrstr);
        return -1;
    }

    return 0;
}

ChannelStream * UnixStream::accept() {
    int clientSock = -1;

    while (true) {
        struct sockaddr_un addr;
        socklen_t len = sizeof(addr);
        clientSock = ::accept(m_sock, (sockaddr *)&addr, &len);
        // DBG("UnixStream::accept  @ %d \n", clientSock);

        if (clientSock < 0 && errno == EINTR) {
            continue;
        }
        break;
    }

    UnixStream *clientStream = NULL;

    if (clientSock >= 0) {
        clientStream =  new UnixStream(clientSock, m_bufsize);
    }
    return clientStream;
}

int UnixStream::connect(const char* addr) {
    m_sock = emugl::socketLocalClient(addr, SOCK_STREAM);
    // DBG("UnixStream::connect @ %d \n", m_sock);
    if (!valid()) return -1;

    return 0;
}

}  // namespace emugl
