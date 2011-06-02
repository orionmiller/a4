/* Authors: Geoffrey Lawson, Orion Miller
 * Class: CPE 453
 * Assignment 4
 *
 * Tokenizing Library
 *   strtok sucks
 */

#include "toklib.h"

char ** tokstr(char *str, char *delim) /*used to ret (char *) */
{
  uint32_t str_pos = 0;
  /*uint32_t str_len = 0;
  uint32_t delim_pos = 0;
  uint32_t delim_len = 0;*/
  uint32_t tok_num = 1;
  uint32_t tok_start = 0;
  uint32_t tok_end = 0;
  uint32_t tok_size = 0;
  uint32_t tok_pos = 0; 
  char ** token = NULL;
  int32_t state = DELIM_STATE, old_state = DELIM_STATE;
  
  /* CHECK ARGUMENTS / INIT VARS */
  if (str == NULL || delim == NULL)
    return NULL;
  
  /* FIND NUM OF TOKENS */
  tok_num += numOfTokens(str, delim);
  
  /* TOKENIZE */
  
  FATALCALL((token=(char **)calloc(tok_num*sizeof(char **),
				   tok_num*sizeof(char **)))==NULL,"calloc");
  token[tok_num] = NULL;

  for (; str[str_pos] != '\0'; str_pos++, old_state = state)
    {
      state = matchDelim(str[str_pos], delim);

      if (state == TOKEN_STATE && old_state == DELIM_STATE)
	{
	  tok_start = str_pos;
	}
      else if (state == DELIM_STATE && old_state == TOKEN_STATE)
	{
	  tok_end = str_pos;
	  tok_size = 1 + tok_end - tok_start;
	  FATALCALL((token[tok_pos]=(char *)calloc(tok_size, 
						   tok_size))==NULL,"calloc");
	  strncpy(token[tok_pos], str+tok_start, (tok_size-1));
	  tok_pos++;
	}
      else
	{
	  /*do nothing*/
	}
    }
  if (old_state == TOKEN_STATE)
    {
      tok_end = str_pos;
      tok_size = 1 + tok_end - tok_start;
      FATALCALL((token[tok_pos]=(char *)calloc(tok_size, 
					       tok_size))==NULL,"calloc");
      strncpy(token[tok_pos], str+tok_start, (tok_size-1));
      tok_pos++;
    }

  return token;
}


int numOfTokens(char *str, char *delim)
{
  uint32_t str_pos = 0;
  int32_t state = DELIM_STATE, old_state = DELIM_STATE;
  int32_t tok_num = 0;

  if (delim == NULL)
    return NO_DELIM;

  for (; str[str_pos] != '\0'; str_pos++)
    {
      state = matchDelim(str[str_pos], delim);
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
  uint8_t pos;
  if (delim == NULL)
    return NO_DELIM;

  for (pos = 0; delim[pos] != '\0'; pos++)
    {
      if (c == delim[pos])
	return 1;
    }
  return 0;
}
