#pragma once

#ifndef __Debug_H_
#define __Debug_H_

#include "decl.h"
#include <stdarg.h> // va_list
#include "sys.h"

#if defined EN_OS_WIN32 || defined EN_OS_WIN64
#include "defsVcWin32.h"
#endif // EN_OS_WIN32

#undef assert

#define KA_ABORT_DEBUG      1
#define KA_ABORT_IGNORE     2
#define KA_ABORT_EXIT       3

#define kaNoAssert(exp) \
    NV_MULTI_LINE_MACRO_BEGIN \
    (void)sizeof(exp); \
    NV_MULTI_LINE_MACRO_END


#if defined EN_OS_WIN32 
#       define kaDebugBreak()       __debugbreak()
#   else
#       error "No kaDebugBreak()!"
#   endif

#define kaAssertMacro(exp) \
    NV_MULTI_LINE_MACRO_BEGIN \
    if (!(exp)) { \
        if (kaAbort(#exp, __FILE__, __LINE__, __FUNC__) == KA_ABORT_DEBUG) { \
            kaDebugBreak(); \
        } \
    } \
    NV_MULTI_LINE_MACRO_END

#define kaCheckMacro(exp) \
    (\
        (exp) ? true : ( \
            (kaAbort(#exp, __FILE__, __LINE__, __FUNC__) == KA_ABORT_DEBUG) ? (kaDebugBreak(), true) : ( false ) \
        ) \
    )

#define kaAssert(exp)    kaAssertMacro(exp)
#define kaCheck(exp)     kaAssertMacro(exp)

#if defined(_DEBUG)
#   define kaDebugAssert(exp)   kaAssertMacro(exp)
#   define kaDebugCheck(exp)    kaAssertMacro(exp)
#else // _DEBUG
#   define kaDebugAssert(exp)   kaNoAssert(exp)
#   define kaDebugCheck(exp)    kaNoAssert(exp)
#endif // _DEBUG


LIBENIGHT_EXPORT int kaAbort(const char *exp, const char *file, int line, const char * func = NULL, const char * msg = NULL, ...) __attribute__((format(printf, 5, 6)));
LIBENIGHT_EXPORT void KA_CDECL nvDebugPrint(const char *msg, ...) __attribute__((format(printf, 1, 2)));

namespace base
{
	struct MessageHandler {
		virtual void log(const char * str, va_list arg) = 0;
		virtual ~MessageHandler() {}
	};

	// Assert handler interface.
	struct AssertHandler {
		virtual int assertion(const char *exp, const char *file, int line, const char *func, const char *msg, va_list arg) = 0;
		virtual ~AssertHandler() {}
	};

	namespace debug
	{
		LIBENIGHT_EXPORT void dumpInfo();
		LIBENIGHT_EXPORT void dumpCallstack(MessageHandler *messageHandler, int callstackLevelsToSkip = 0);

		LIBENIGHT_EXPORT void setMessageHandler(MessageHandler * messageHandler);
		LIBENIGHT_EXPORT void resetMessageHandler();

		LIBENIGHT_EXPORT void setAssertHandler(AssertHandler * assertHanlder);
		LIBENIGHT_EXPORT void resetAssertHandler();

		LIBENIGHT_EXPORT void enableSigHandler(bool interactive);
		LIBENIGHT_EXPORT void disableSigHandler();

		LIBENIGHT_EXPORT bool isDebuggerPresent();
		LIBENIGHT_EXPORT bool attachToDebugger();

		LIBENIGHT_EXPORT void terminate(int code);
	}
}

#endif
