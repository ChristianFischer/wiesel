#include <wiesel.h>
#include <android/log.h>


using namespace wiesel;


// implementing the platform specific logging
int _logmsg_impl(LogLevel level, const char *tag, const char *message) {
	android_LogPriority priority = ANDROID_LOG_INFO;

	switch(level) {
		case LogLevel_Debug:	priority = ANDROID_LOG_DEBUG;	break;
		case LogLevel_Info:		priority = ANDROID_LOG_INFO;	break;
		case LogLevel_Warning:	priority = ANDROID_LOG_WARN;	break;
		case LogLevel_Error:	priority = ANDROID_LOG_ERROR;	break;
	}

	return __android_log_print(priority, tag, message);
}

