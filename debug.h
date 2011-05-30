/**
 * @file
 * Header for debugging options.  Not commented much, to accentuate what
 * is actually here.  For commentary, see the version in the Lab 1 solution.
 *
 * @author Kevin O'Gorman
 * @version Fall 2010
 * Last Modified: Fri Nov 26 05:32:07 PST 2010ime 2010
 */

#ifndef DEBUG_H
#define DEBUG_H

/* Don't ask why I need 3 macros.  Look at 
 * http://www.decompile.com/cpp/faq/file_and_line_error_string.htm
 */
#define _STRINGIFY_(x) #x
#define _TOSTRING_(x) _STRINGIFY_(x)
#define _LOCATOR_ __FILE__ ":" _TOSTRING_(__LINE__) "; "

#ifdef DEBUG
#undef NDEBUG /* turn on assertions, if using assert.h  */
/** @def PRINT0
 * Prints the given string to stderr.  */
#define PRINT0(f) fputs(_LOCATOR_ f, stderr)

/** @def PRINT1
 * Prints to stderr with a format string and a single additional parameter. */
#define PRINT1(f,x) fprintf(stderr, _LOCATOR_ f, x)

/** @def PRINT2
 * Prints to stderr with a format string and two additional parameters. */
#define PRINT2(f,x,y) fprintf(stderr, _LOCATOR_ f, x, y)

/** @def PRINT3
 * Prints to stderr with a format string and three additional parameters. */
#define PRINT3(f,x,y,z) fprintf(stderr, _LOCATOR_ f, x, y, z)

/** @def PRINT4
 * Prints to stderr with a format string and four additional parameters. */
#define PRINT4(f,x,y,z,w) fprintf(stderr, _LOCATOR_ f, x, y, z, w)

#else

#define NDEBUG /* turn off assertions */
/** @def PRINT0
 * Empty macro, disabled by DEBUG */
#define PRINT0(f)

/** @def PRINT1
 * Empty macro, disabled by DEBUG */
#define PRINT1(f,x)

/** @def PRINT2
 * Empty macro, disabled by DEBUG */
#define PRINT2(f,x,y)

/** @def PRINT3
 * Empty macro, disabled by DEBUG */
#define PRINT3(f,x,y,z)

/** @def PRINT4
 * Empty macro, disabled by DEBUG */
#define PRINT4(f,x,y,z,w)
#endif

/** @def FATAL1
 * Evaluate a condition, and if it is false, produce an error message and quit.
 * The second parameter must be a string giving information about the error.
 */
#define FATAL(condition, complaint) \
    if (condition) { \
        fputs(_LOCATOR_ complaint "\n", stderr); \
        exit(EXIT_FAILURE); \
    }

/** @def FATAL1
 * Evaluate a condition, and if it is false, produce an error message and quit.
 * The second parameter must be an output format string, and the third 
 * parameter is intended to satisfy the one %-format in that string.
 */
#define FATAL1(condition, complaint, arg1) \
    if (condition) { \
        fprintf(stderr, _LOCATOR_ complaint "\n", arg1); \
        exit(EXIT_FAILURE); \
    }

/** @def FATALCALL
 * Evaluates the condition, complains and quits if it represents 
 * function failure.
 * The condition should contain or otherwise involve the return value of a
 * library function or system call (not a function provided in the application
 * unless it's careful to change 'errno'.)  Errors involve the perror(3) 
 * function which relies on 'errno'.
 */
#define FATALCALL(condition, complaint) \
    if (condition) { \
        perror(_LOCATOR_ complaint); \
        exit(EXIT_FAILURE); \
    }

#endif
int asdafd;
