
#include "debug.h"
#include "str.h"

#if defined EN_OS_WIN32 //&& NV_CC_MSVC
#   define WIN32_LEAN_AND_MEAN
#   define VC_EXTRALEAN
#   include <windows.h>
#   include <direct.h>
#if defined(_MSC_VER) 
#       include <crtdbg.h>
#       if _MSC_VER < 1300
#           define DECLSPEC_DEPRECATED
#           include <dbghelp.h> // must be XP version of file
#       else
#           include <dbghelp.h>
#       endif
#   endif
#   pragma comment(lib,"dbghelp.lib")
#endif


namespace base
{
	namespace
	{
		static bool s_interactive = true;
		static MessageHandler * s_message_handler = NULL;
		static AssertHandler * s_assert_handler = NULL;

		static inline int backtraceWithSymbols(CONTEXT * ctx, void * trace[], int maxcount, int skip = 0) {

			// Init the stack frame for this function
			STACKFRAME64 stackFrame = { 0 };

#if EN_CPU_X86_64
			DWORD dwMachineType = IMAGE_FILE_MACHINE_AMD64;
			stackFrame.AddrPC.Offset = ctx->Rip;
			stackFrame.AddrFrame.Offset = ctx->Rbp;
			stackFrame.AddrStack.Offset = ctx->Rsp;
#elif EN_CPU_X86
			DWORD dwMachineType = IMAGE_FILE_MACHINE_I386;
			stackFrame.AddrPC.Offset = ctx->Eip;
			stackFrame.AddrFrame.Offset = ctx->Ebp;
			stackFrame.AddrStack.Offset = ctx->Esp;
#else
#error "Platform not supported!"
#endif
			stackFrame.AddrPC.Mode = AddrModeFlat;
			stackFrame.AddrFrame.Mode = AddrModeFlat;
			stackFrame.AddrStack.Mode = AddrModeFlat;

			// Walk up the stack
			const HANDLE hThread = GetCurrentThread();
			const HANDLE hProcess = GetCurrentProcess();
			int i;
			for (i = 0; i < maxcount; i++)
			{
				// walking once first makes us skip self
				if (!StackWalk64(dwMachineType, hProcess, hThread, &stackFrame, ctx, NULL, &SymFunctionTableAccess64, &SymGetModuleBase64, NULL)) {
					break;
				}

				/*if (stackFrame.AddrPC.Offset == stackFrame.AddrReturn.Offset || stackFrame.AddrPC.Offset == 0) {
				break;
				}*/

				if (i >= skip) {
					trace[i - skip] = (PVOID)stackFrame.AddrPC.Offset;
				}
			}

			return i - skip;
		}

#pragma warning(push)
#pragma warning(disable:4748)
		static  int backtrace(void * trace[], int maxcount) {
			CONTEXT ctx = { 0 };
#if EN_CPU_X86 && !EN_CPU_X86_64
			ctx.ContextFlags = CONTEXT_CONTROL;
			_asm {
				call x
				x : pop eax
					mov ctx.Eip, eax
					mov ctx.Ebp, ebp
					mov ctx.Esp, esp
			}
#else
			RtlCaptureContext(&ctx); // Not implemented correctly in x86.
#endif

			return backtraceWithSymbols(&ctx, trace, maxcount, 1);
		}
#pragma warning(pop)

		struct Win32AssertHandler : public AssertHandler
		{
			// Flush the message queue. This is necessary for the message box to show up.
			static void flushMessageQueue()
			{
				MSG msg;
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					//if( msg.message == WM_QUIT ) break;
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			// Assert handler method.
			virtual int assertion(const char * exp, const char * file, int line, const char * func, const char * msg, va_list arg)
			{
				int ret = KA_ABORT_EXIT;

				StringBuilder error_string;
				error_string.format("*** Assertion failed: %s\n    On file: %s\n    On line: %d\n", exp, file, line);
				if (func != NULL) {
					error_string.appendFormat("    On function: %s\n", func);
				}
				if (msg != NULL) {
					error_string.append("    Message: ");
					va_list tmp;
					va_copy(tmp, arg);
					error_string.appendFormatList(msg, tmp);
					va_end(tmp);
					error_string.append("\n");
				}

				// Print stack trace:
				debug::dumpInfo();

				if (debug::isDebuggerPresent()) {
					return KA_ABORT_DEBUG;
				}

				if (s_interactive) {
					flushMessageQueue();
					int action = MessageBoxA(NULL, error_string.str(), "Assertion failed", MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_TOPMOST);
					switch (action) {
					case IDRETRY:
						ret = KA_ABORT_DEBUG;
						break;
					case IDIGNORE:
						ret = KA_ABORT_IGNORE;
						break;
					case IDABORT:
					default:
						ret = KA_ABORT_EXIT;
						break;
					}
				}

				if (ret == KA_ABORT_EXIT) {
					exit(EXIT_FAILURE + 1);
				}

				return ret;
			}
		};
	}

	void debug::dumpInfo()
	{
		if (true)
		{
			void * trace[64];
			int size = backtrace(trace, 64);

			enDebug("\nDumping stacktrace:\n");

			Array<const char *> lines;
			writeStackTrace(trace, size, 1, lines);

			for (uint i = 0; i < lines.count(); i++) {
				enDebug("%s", lines[i]);
				delete lines[i];
			}
		}
	}

	void debug::dumpCallstack(MessageHandler *messageHandler, int callstackLevelsToSkip /*= 0*/)
	{

	}

	void debug::setMessageHandler(MessageHandler * messageHandler)
	{

	}

	void debug::resetMessageHandler()
	{

	}

	void debug::setAssertHandler(AssertHandler * assertHanlder)
	{

	}

	void debug::resetAssertHandler()
	{

	}

	void debug::enableSigHandler(bool interactive)
	{

	}

	void debug::disableSigHandler()
	{

	}

	bool debug::isDebuggerPresent()
	{
		return true;
	}

	bool debug::attachToDebugger()
	{
		return true;
	}

	void debug::terminate(int code)
	{

	}

}

using namespace base;
int kaAbort(const char *exp, const char *file, int line, const char * func /*= NULL*/, const char * msg /*= NULL*/, ...)
{
#if defined EN_OS_WIN32
	static Win32AssertHandler s_default_assert_handler;
#else
#endif

	va_list arg;
	va_start(arg, msg);
	AssertHandler * handler = s_assert_handler != NULL ? s_assert_handler : &s_default_assert_handler;
	int result = handler->assertion(exp, file, line, func, msg, arg);

	va_end(arg);

	return result;
}

