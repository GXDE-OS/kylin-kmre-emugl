// Generated Code - DO NOT EDIT !!
// generated by 'emugen'

#ifndef GUARD_gles2_decoder_context_t
#define GUARD_gles2_decoder_context_t

#include "OpenglRender/IOStream.h"
#include "ChecksumCalculator.h"
#include "gles2_server_context.h"


#include "emugl/common/logging.h"

struct gles2_decoder_context_t : public gles2_server_context_t {

    int decode(void *buf, size_t bufsize, IOStream *stream, ChecksumCalculator* checksumCalc);

};

#endif  // GUARD_gles2_decoder_context_t