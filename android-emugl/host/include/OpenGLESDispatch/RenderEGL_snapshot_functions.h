// Auto-generated with: scripts/gen-entries.py --mode=functions android-emugl/host/libs/libOpenGLESDispatch/render_egl_snapshot.entries --output=android-emugl/host/include/OpenGLESDispatch/RenderEGL_snapshot_functions.h
// DO NOT EDIT THIS FILE

#ifndef RENDER_EGL_SNAPSHOT_FUNCTIONS_H
#define RENDER_EGL_SNAPSHOT_FUNCTIONS_H

#define LIST_RENDER_EGL_SNAPSHOT_FUNCTIONS(X) \
  X(EGLConfig, eglLoadConfig, (EGLDisplay display, EGLStream stream)) \
  X(EGLContext, eglLoadContext, (EGLDisplay display, const EGLint * attrib_list, EGLStream stream)) \
  X(EGLBoolean, eglLoadAllImages, (EGLDisplay display, EGLStream stream, const void* textureLoader)) \
  X(EGLBoolean, eglSaveConfig, (EGLDisplay display, EGLConfig config, EGLStream stream)) \
  X(EGLBoolean, eglSaveContext, (EGLDisplay display, EGLContext context, EGLStream stream)) \
  X(EGLBoolean, eglSaveAllImages, (EGLDisplay display, EGLStream stream, const void* textureSaver)) \
  X(EGLBoolean, eglPreSaveContext, (EGLDisplay display, EGLContext contex, EGLStream stream)) \
  X(EGLBoolean, eglPostLoadAllImages, (EGLDisplay display, EGLStream stream)) \
  X(EGLBoolean, eglPostSaveContext, (EGLDisplay display, EGLConfig config, EGLStream stream)) \
  X(void, eglUseOsEglApi, (EGLBoolean enable)) \
  X(void, eglSetMaxGLESVersion, (EGLint glesVersion)) \
  X(void, eglFillUsages, (void* usages)) \

EGLAPI EGLConfig EGLAPIENTRY eglLoadConfig(EGLDisplay display, EGLStream stream);
EGLAPI EGLContext EGLAPIENTRY eglLoadContext(EGLDisplay display, const EGLint * attrib_list, EGLStream stream);
EGLAPI EGLBoolean EGLAPIENTRY eglLoadAllImages(EGLDisplay display, EGLStream stream, const void* textureLoader);
EGLAPI EGLBoolean EGLAPIENTRY eglSaveConfig(EGLDisplay display, EGLConfig config, EGLStream stream);
EGLAPI EGLBoolean EGLAPIENTRY eglSaveContext(EGLDisplay display, EGLContext context, EGLStream stream);
EGLAPI EGLBoolean EGLAPIENTRY eglSaveAllImages(EGLDisplay display, EGLStream stream, const void* textureSaver);
EGLAPI EGLBoolean EGLAPIENTRY eglPreSaveContext(EGLDisplay display, EGLContext contex, EGLStream stream);
EGLAPI EGLBoolean EGLAPIENTRY eglPostLoadAllImages(EGLDisplay display, EGLStream stream);
EGLAPI EGLBoolean EGLAPIENTRY eglPostSaveContext(EGLDisplay display, EGLConfig config, EGLStream stream);
EGLAPI void EGLAPIENTRY eglUseOsEglApi(EGLBoolean enable);
EGLAPI void EGLAPIENTRY eglSetMaxGLESVersion(EGLint glesVersion);
EGLAPI void EGLAPIENTRY eglFillUsages(void* usages);

#endif  // RENDER_EGL_SNAPSHOT_FUNCTIONS_H