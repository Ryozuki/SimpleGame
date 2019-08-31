#include <stdarg.h>
#include <stdio.h>
#include "color.h"
#include "logger.h"

static const char* get_level_str(int level) {
	switch(level) {
		case LOG_DEBUG:
			return "[" TCOLOR_BOLD TCOLOR_YELLOW "Debug" TCOLOR_RESET "] ";
		case LOG_ERROR:
			return "[" TCOLOR_BOLD TCOLOR_RED "Error" TCOLOR_RESET "] ";
		case LOG_WARNING:
			return "[" TCOLOR_BOLD TCOLOR_MAGENTA "Warning" TCOLOR_RESET "] ";
		case LOG_INFO:
			return "[" TCOLOR_BOLD TCOLOR_CYAN "Info" TCOLOR_RESET "] ";
	}
	return "";
}

void log_write(int level, const char* fmt, ...) {
#ifdef NDEBUG
	if(level == LOG_DEBUG)
		return;
#endif
	printf(get_level_str(level));

	va_list args;
	va_start(args, fmt);

	vprintf(fmt, args);

	va_end(args);
}

static void log_write_internal(int level, const char* fmt, va_list args) {
#ifdef NDEBUG
	if(level == LOG_DEBUG)
		return;
#endif
	printf(get_level_str(level));
	vprintf(fmt, args);
}

void log_info(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	log_write_internal(LOG_INFO, fmt, args);

	va_end(args);
}

void log_warning(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	log_write_internal(LOG_WARNING, fmt, args);

	va_end(args);
}

void log_debug(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	log_write_internal(LOG_DEBUG, fmt, args);

	va_end(args);
}

void log_error(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	log_write_internal(LOG_ERROR, fmt, args);

	va_end(args);
}
