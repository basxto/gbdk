/** @file stdio.h
    Basic file/console input output functions.
 */
#include <types.h>

#if STRICT_ANSI
void putchar(int c);
#else
/** Put the character 'c' to stdout. */
void putchar(char c);
#endif

/** Print the string and arguments given by format to stdout.
    Currently supported: \%c (character), \%u (unsigned int), 
    \%d (signed int), \%x (unsigned int as hex), and \%s (string).
    Does not return the number of characters printed.
 */
void printf(const char *format, ...);

/** Print the string and arguments given by format to a buffer.
    Currently supported: \%c (character), \%u (unsigned int), 
    \%d (signed int), \%x (unsigned int as hex), and \%s (string).
    Does not return the number of characters printed.

    @param str		The buffer to print into.
    @param format	The format string as per printf.
 */
void sprintf(char *str, const char *format, ...);
