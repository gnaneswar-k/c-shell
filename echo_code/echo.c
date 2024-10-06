#include "stdlib.h"
#include "assert.h"
#include "./echo.h"

echoValues *initEchoVal()
{
  echoValues *init = (echoValues *)malloc(sizeof(echoValues));
  assert(init != NULL);
  init->curr = '\0';
  init->prev = '\0';
  init->no_quotes = true;
  init->not_finished = true;

  return init;
};

int freeEchoVal(echoValues *echoData)
{
  free(echoData);
  return 0;
};

void handle_space_cases(char curr, echoValues *previous)
{
  // When there's a quote, which means the character has to be print as is.
  if (!previous->no_quotes)
  {
    previous->curr = curr;
    previous->prev = curr;
    return;
  }
  // When quotes aren't there and there was no space character previously.
  else if (previous->no_quotes && (previous->prev != ' '))
  {
    previous->curr = ' ';
    previous->prev = ' ';
    return;
  }
  // When quotes aren't there and there was a space character previously.
  else
  {
    previous->curr = '"';
    previous->prev = ' ';
    return;
  }
};

void handle_newline(char curr, echoValues *previous)
{
  previous->curr = '\n';
  previous->prev = '\n';

  // When a quote wasn't started, newline means end of input.
  if (previous->no_quotes)
    previous->not_finished = false;
  // If a quote was started, then newline is considered as an input character.
  else
    previous->not_finished = true;

  return;
};

int parse_char(char curr, echoValues *previous)
{
  // Line end case.
  if (curr == '\n')
  {
    handle_newline(curr, previous);
    return 0;
  }
  // If a quote is read, then toggle the flag.
  else if (curr == '"')
  {
    previous->no_quotes = !previous->no_quotes;
    previous->prev = curr;
    previous->curr = '"';
    return 0;
  }
  // Case for space and tab charcters.
  else if (curr == '\t' || curr == ' ')
  {
    handle_space_cases(curr, previous);
    return 0;
  }
  // Default case.
  else
  {
    previous->curr = curr;
    previous->prev = curr;
    return 0;
  }

  return 1;
};
