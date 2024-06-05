// Generated Code - DO NOT EDIT !!
// generated by 'emugen'

#ifndef GUARD_foo_encoder_context_t
#define GUARD_foo_encoder_context_t

#include "IOStream.h"
#include "ChecksumCalculator.h"
#include "foo_client_context.h"


#include "fooUtils.h"
#include "fooBase.h"

struct foo_encoder_context_t : public foo_client_context_t {

	IOStream *m_stream;
	ChecksumCalculator *m_checksumCalculator;

	foo_encoder_context_t(IOStream *stream, ChecksumCalculator *checksumCalculator);
	virtual uint64_t lockAndWriteDma(void*, uint32_t) { return 0; }
};

#endif  // GUARD_foo_encoder_context_t
