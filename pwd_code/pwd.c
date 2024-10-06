#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#include "./pwd.h"

int pwd()
{
  // Determine size of path and allocate memory accordingly.
  unsigned long path_size = GetCurrentDirectory(0, NULL); // Length of the path.
  char *path = (char *)malloc(path_size * sizeof(char));  // Array to store the path.
  GetCurrentDirectory(path_size, path);                   // Store the path.

  // Print the path.
  printf("%s\n", path);

  // Free allocated memory.
  free(path);

  return 0;
};
