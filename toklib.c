/* Authors: Geoffrey Lawson, Orion Miller
 * Class: CPE 453
 * Assignment 4
 *
 * Tokenizing Library
 *   strtok sucks
 */

#include "toklib.h"

int tokstr(char *str, char *delim) /*used to ret (char *) */
{
  unsigned int str_pos = 0;
  unsigned int str_len = 0;
  unsigned int delim_pos = 0;
  unsigned int delim_len = 0;
  unsigned int tok_num = 1;
  unsigned int tok_start = 0;
  unsigned int tok_end = 0;
  char * token = NULL;
  int state = DELIM_STATE, old_state = DELIM_STATE;

  /* CHECK ARGUMENTS / INIT VARS */

  if ((str_len=strlen(str)) == 0)
    return 0; /*NULL*/

  if ((delim_len=strlen(delim)) == 0)
    return 0;/*NULL*/

  /* FIND NUM OF TOKENS */
  tok_num += numOfTokens(str, delim, delim_len);

  /* TOKENIZE */

  FATALCALL((token=(char *)malloc(tok_num))==NULL,"malloc");
  token[token_num] = NULL;

  for (; str[str_pos] != '\0'; str_pos++)
    {
      state = matchDelim(str[str_pos], delim, delim_len);
      if (old_state == TOKEN_STATE && state == DELIM_STATE)
	{
	}
      old_state = state;
    }

  return NULL;
}

int numOfTokens(char *str, char *delim, int delim_len)
{
  unsigned int str_pos = 0;
  int state = DELIM_STATE, old_state = DELIM_STATE;
  int tok_num = 0;

  for (; str[str_pos] != '\0'; str_pos++)
    {
      state = matchDelim(str[str_pos], delim, delim_len);
      if (old_state == TOKEN_STATE && state == DELIM_STATE)
	{
	  tok_num++;
	}
      old_state = state;
    }
  if (old_state == TOKEN_STATE)
    {
      tok_num++;
    }

  return tok_num;
}

int matchDelim(char c, char *delim)
{
  char pos;
  for (pos = 0; delim[pos] != '\0'; pos++)
    {
      if (c == delim[pos])
	return 1;
    }
  return 0;
}


int freeTok(char *tok)
{
  return FREE_TOK_FAILURE;
}
