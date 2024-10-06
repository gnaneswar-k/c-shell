#include "stdio.h"
#include "stdlib.h"
#include "windows.h"
#include "string.h"
#include "stdbool.h"
#include "./cd.h"

int handle_cd(char *path, char *home, FILE *cd_hist)
{
  // Return value of the function.
  int ret_val = 0;

  // For ., i.e. current directory.
  if (strcmp(path, ".") == 0)
    return ret_val;
  // For ~ and case when no args are given, i.e. home directory.
  else if ((strcmp(path, "~") == 0) || (strcmp(path, "") == 0))
  {
    // If system error condition.
    if (SetCurrentDirectory(home) == 0)
    {
      perror("System Error");
      ret_val = 1;
    }

    return ret_val;
  }
  // For -, i.e. previous directory.
  else if (strcmp(path, "-") == 0)
  {
    // Destination path.
    char dest[(2 * MAX_PATH_LENGTH) + 1] = "";
    // Read the most recently visited directory.
    while (!feof(cd_hist))
      fscanf(cd_hist, "%s ", dest);

    // If no history is left.
    if (strcmp(dest, "") == 0)
    {
      fprintf(stderr, "Error: No location history left to navigate to.\n");
      ret_val = 2;
    }
    // Test if location can be navigated to.
    else
    {
      ret_val = 4;

      // If system error condition.
      if (SetCurrentDirectory(dest) == 0)
      {
        perror("System Error");
        ret_val = 1;
      }
      // Else print the destination being navigated to.
      else
        printf("%s\n", dest);
    }

    return ret_val;
  }
  // For .., i.e. one directory up.
  else if (strcmp(path, "..") == 0)
  {
    // Length of current directory.
    long curr_dir_len = GetCurrentDirectory(0, NULL);
    // Array to store current directory.
    char *curr_dir = (char *)malloc(curr_dir_len * sizeof(char));
    // Store the current directory.
    GetCurrentDirectory(curr_dir_len, curr_dir);

    // If current directory is home, then don't do anything.
    if (strcmp(curr_dir, home) != 0)
    {
      // If system error condition.
      if (SetCurrentDirectory("..") == 0)
      {
        perror("System Error");
        ret_val = 1;
      }
    }
    else
      fprintf(stderr, "Error: Cannot navigate above home directory.\n");

    // Free up allocated memory.
    free(curr_dir);

    return ret_val;
  }
  // When a name is given.
  else
  {
    // If error condition.
    if (SetCurrentDirectory(path) == 0)
    {
      fprintf(stderr, "Error\n");
      ret_val = 2;
    }

    return ret_val;
  }

  // Unknown error, as code should not reach this part.
  perror("Internal Error");
  return 3;
};

int cd(FILE *f_inp, FILE *cd_hist, char *home)
{
  char path[MAX_PATH_LENGTH + 1]; // Path to change to.

  // When no arguments are passed, i.e. go to home directory.
  if (feof(f_inp))
    strcpy(path, "~");
  // Else parse the arguments.
  else
  {
    // Read the path to change to.
    fscanf(f_inp, "%s ", path);

    // Handle error when there are more than one arguments.
    int argCheck = feof(f_inp);
    if (argCheck == 0)
    {
      fprintf(stderr, "Error: Invalid number of arguments. Expected 1 or lesser number of arguments.\n");
      return -1;
    }
  }

  int ret_val = 0;                      // Return value of the function.
  char *sub_path = strtok(path, "/\\"); // Array containing the path to traverse to when given multiple values separated by '/' and/or '\'.

  // While ret_val is valid.
  // We're not checking for 4 (ret_val for history) even though it is valid as '-' should be the only argument.
  // If other arguments are present, then the command is automatically invalid.
  // If there is(are) character(s) immediately following '-', then the code checks for that whole string and returns invalid.
  while (ret_val == 0)
  {
    ret_val = handle_cd(sub_path, home, cd_hist);
    sub_path = strtok(NULL, "/\\");
    if (sub_path == NULL)
      break;
  };

  return ret_val;
};
