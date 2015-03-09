// Copyright (c) 2014-2015 Josh Blum
// SPDX-License-Identifier: BSL-1.0

#include <SoapySDR/Logger.h>
#include <cstdio>
#include <cstdlib>

/***********************************************************************
 * Compatibility for vasprintf under MSVC
 **********************************************************************/
#ifdef _MSC_VER
int vasprintf(char **strp, const char *fmt, va_list ap)
{
    int r = _vscprintf(fmt, ap);
    if (r < 0) return r;
    *strp = (char *)malloc(r+1);
    return vsprintf_s(*strp, r+1, fmt, ap);
}
#endif

/***********************************************************************
 * ANSI terminal colors for default logger
 **********************************************************************/
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BOLD    "\x1b[1m"

/***********************************************************************
 * Default log message handler implementation
 **********************************************************************/
void defaultLogHandler(const SoapySDRLogLevel logLevel, const char *message)
{
    switch (logLevel)
    {
    case SOAPY_SDR_FATAL:    fprintf(stderr, ANSI_COLOR_BOLD ANSI_COLOR_RED "[FATAL] %s" ANSI_COLOR_RESET "\n", message); break;
    case SOAPY_SDR_CRITICAL: fprintf(stderr, ANSI_COLOR_BOLD ANSI_COLOR_RED "[CRITICAL] %s" ANSI_COLOR_RESET "\n", message); break;
    case SOAPY_SDR_ERROR:    fprintf(stderr, ANSI_COLOR_BOLD ANSI_COLOR_RED "[ERROR] %s" ANSI_COLOR_RESET "\n", message); break;
    case SOAPY_SDR_WARNING:  fprintf(stderr, ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "[WARNING] %s" ANSI_COLOR_RESET "\n", message); break;
    case SOAPY_SDR_NOTICE:   fprintf(stdout, ANSI_COLOR_GREEN "[NOTICE] %s" ANSI_COLOR_RESET "\n", message); break;
    case SOAPY_SDR_INFO:     fprintf(stdout, "[INFO] %s\n", message); break;
    case SOAPY_SDR_DEBUG:    fprintf(stdout, "[DEBUG] %s\n", message); break;
    case SOAPY_SDR_TRACE:    fprintf(stdout, "[TRACE] %s\n", message); break;
    case SOAPY_SDR_SSI:      fputs(message, stderr); fflush(stderr); break;
    }
}

static SoapySDRLogHandler registeredLogHandler = &defaultLogHandler;

extern "C" {

void SoapySDR_log(const SoapySDRLogLevel logLevel, const char *message)
{
    return registeredLogHandler(logLevel, message);
}

void SoapySDR_vlogf(const SoapySDRLogLevel logLevel, const char *format, va_list argList)
{
    char *message;
    vasprintf(&message, format, argList);
    SoapySDR_log(logLevel, message);
    free(message);
}

void SoapySDR_registerLogHandler(const SoapySDRLogHandler handler)
{
    registeredLogHandler = handler;
}

}
