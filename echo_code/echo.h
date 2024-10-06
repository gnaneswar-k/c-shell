#ifndef __ECHO_HEADER__
#define __ECHO_HEADER__

#include "stdbool.h"

typedef struct echo echoValues;

struct echo
{
  char curr;         // Currently read character.
  char prev;         // Immediate predecessor character that was read.
  bool no_quotes;    // Boolean to check if a quote was started.
  bool not_finished; // Boolean to check if the line has ended.
};

// Function to initalise struct containing data required for echo command.
// Returns: Initialised echoValues struct.
echoValues *initEchoVal();

// Function to free memory allocated to echo command struct.
// Returns: 0 if successful, otherwise a non-zero integer.
int freeEchoVal(echoValues *echoData);

// Function to handle the space and tab characters.
// curr = Currently read character.
// prev = Immediate predecessor character that was read.
// no_quotes = Boolean to check if a quote was started.
void handle_space_cases(char curr, echoValues *previous);

// Function to handle newline character.
// curr = Currently read character.
// prev = Immediate predecessor character that was read.
// no_quotes = Boolean to check if a quote was started.
void handle_newline(char curr, echoValues *previous);

// Function to parse the character read.
// curr = Currently read character.
// previous = Struct pointer containing previously stored return value of echo command.
int parse_char(char curr, echoValues *previous);

#endif
