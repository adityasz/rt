// Copyright (C) 2023 Aditya Singh

#ifndef PRINT_H
#define PRINT_H

#include "log_levels.h"

#define INFO    "[INFO]: "
#define DBG     "[DEBUG]: "

#define ESC "\033"
#define CSI "["
#define PREV_LINE "F"
#define BACKSPACE "D"

#if defined(DEBUG)
#define LOGLEVEL LOGLEVEL_DEBUG
#endif

#if LOGLEVEL > LOGLEVEL_DEFAULT
#include <fmt/format.h>
namespace fmtlib = fmt;

/**
 * @brief Print a message to stderr.
 *
 * @param fmt format string
 * @param ... format arguments
 */
#define pr_error(fmt, ...)                                  \
        fmtlib::print(stderr, fmt, ##__VA_ARGS__)
#endif

#if LOGLEVEL >= LOGLEVEL_DEBUG
/**
 * @brief Print a message to stdout.
 *
 * This macro expands to a print to stdout with LOGLEVEL_INFO loglevel.
 *
 * @param fmt format string
 * @param ... format arguments
 */
#define printrt(fmt, ...)                                     \
	fmtlib::print(fmt, ##__VA_ARGS__)

/**
 * @brief Print an info-level message.
 *
 * This macro expands to a print to stderr with LOGLEVEL_INFO loglevel.
 *
 * @param fmt format string
 * @param ... format arguments
 */
#define pr_info(fmt, ...)                                   \
        fmtlib::print(stderr, INFO fmt, ##__VA_ARGS__)

/**
 * @brief Print a debug-level message.
 *
 * This macro expands to a print to stderr with LOGLEVEL_DEBUG loglevel.
 *
 * @param fmt format string
 * @param ... format arguments
 */
#define pr_debug(fmt, ...)                                  \
        fmtlib::print(stderr, DBG fmt, ##__VA_ARGS__)
#elif LOGLEVEL >= LOGLEVEL_INFO
/**
 * @brief Print a message to stdout.
 *
 * This macro expands to a print to stdout with LOGLEVEL_INFO loglevel.
 * @param fmt format string
 * @param ... format arguments
 */
#define printrt(fmt, ...)                                     \
	fmtlib::print(stdout, fmt, ##__VA_ARGS__)

/**
 * @brief Print an info-level message.
 *
 * This macro expands to a print to stderr with LOGLEVEL_INFO loglevel.
 *
 * @param fmt format string
 * @param ... format arguments
 */
#define pr_info(fmt, ...)                                   \
        fmtlib::print(stderr, INFO fmt, ##__VA_ARGS__)



#define pr_debug(...)    do {} while(0)

#else
#define printrt(...)       do {} while(0)
#define pr_info(...)     do {} while(0)
#define pr_debug(...)    do {} while(0)
#define pr_error(...)    do {} while(0)
#endif

#endif // PRINT_H
