/* Authors: Geoffrey Lawson, Orion Miller
 * Class: CPE 453
 * Assignment 4
 *
 * Tokenizing Library
 *   A library to avoid strtok
 */

#ifndef TOKLIB_H
#define TOKELIB_H

#include <string.h>

#define FREE_TOK_SUCCESS 0
#define FREE_TOK_FAILURE -1

#define TOKEN_STATE 0
#define DELIM_STATE 1


/* Takes string and tokenizes it by using the given delimiter string.
 * 
 * PARAMATER:
 *   str - the string that will be tokenized
 *   delim - the delimiter on the string
 *
 * RETURN:
 *   SUCCESS: pointer to an array of the passed in string.
 *   FAILURE: NULL pointer
 *
 * POSTCONDITION:
 *   The parameter size should be updated with the size used to allocate
 *   the tokenized string array.
 *   
 * WARNING:
 *   It is assumed that the strings passed in are NULL terminated. The returned 
 *   pointer should be freed (you can use freeTok() for that) else it will
 *   cause memory leaks.
 */
int tokstr(char *str, char *delim);


/* Takes string and finds the number of tokens within that string.
 * 
 * PARAMATER:
 *   str - the string that will be tokenized
 *   delim - the delimiter on the string
 *
 * RETURN:
 *   number of tokens within the string
 *   
 * WARNING:
 *   It is assumed that the strings passed in are NULL terminated.
 *   
 */
int numOfTokens(char *str, char *delim);

int freeTok(char *tok)

int matchDelim(char c, char *delim, int delim_len);

#endif
