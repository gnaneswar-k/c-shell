#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"
#include "windows.h"
#include "../echo_code/echo.h"
#include "../pwd_code/pwd.h"
#include "../cd_code/cd.h"

// Lengths.
#define MAX_BUFFER 1024               // Maximum length of input characters read at once.
#define MAX_CMD 10                    // Maximum length of the name of a command.
#define MAX_FILE MAX_PATH_LENGTH + 20 // Maximum length of helper files.

// Files.
#define CMD_HIST "\\cmd_hist.txt"
#define CMD_HIST_OLD "\\old_cmd_hist.txt"
#define CD_HIST "\\cd_hist.txt"
#define CD_HIST_NEW "\\new_cd_hist.txt"
#define INPUT_FILE "\\input.txt"

char *createFilePath(char *home, char *name)
{
  char *pointer = (char *)malloc(MAX_FILE * sizeof(char));
  assert(pointer != NULL);
  strcpy(pointer, home);
  strcat(pointer, name);

  return pointer;
};

void readInputAndWriteToFile(char *input_array, FILE *f_inp)
{
  // Loop until either input ends or \n is encountered.
  while (fgets(input_array, MAX_BUFFER, stdin) != NULL)
  {
    fputs(input_array, f_inp);
    if (strchr(input_array, '\n'))
      break;
  };
};

void handle_echo(FILE *f_inp, char *input_array)
{
  // Initialise the state.
  echoValues *echoData = initEchoVal();
  long echo_counter = 0;
  int curr;

  // Loop until echo command is not finished.
  while (echoData->not_finished)
  {
    // Loop until input is finished or echo command finishes.
    while (echoData->not_finished)
    {
      fseek(f_inp, ftell(f_inp), SEEK_SET);

      // Read character.
      curr = fgetc(f_inp);
      // Exit if end of file.
      if (curr == EOF)
        break;

      // Parse the character. Put in a loop to execute until command exits normally.
      while (parse_char((char)curr, echoData))
        ;

      // Write the necessary character into the file if it is not a quote.
      if (echoData->curr != '"')
      {
        // Position file pointer to overwrite the extra characters.
        if (echoData->curr == '\n') // '\n' being read as two characters \r\n in Windows.
          fseek(f_inp, ftell(f_inp) - 2L - echo_counter, SEEK_SET);
        else // Other characters.
          fseek(f_inp, ftell(f_inp) - 1L - echo_counter, SEEK_SET);

        // Write character into the file.
        fputc(echoData->curr, f_inp);

        // Adjust position back according to original input.
        fseek(f_inp, echo_counter, SEEK_CUR);
      }
      // Otherwise skip reading.
      else
        echo_counter++;
    };

    // If input is completed yet echo command is not finished
    // i.e when quotes are started but not finished and \n was encountered.
    if (echoData->not_finished)
    {
      // Prompt for additional input and read.
      printf("> ");

      long pos = ftell(f_inp); // Variable to store the location from where reading needs to be done.

      // Read new input and reset pointer to where previously the commant was stopped.
      readInputAndWriteToFile(input_array, f_inp);
      fseek(f_inp, pos, SEEK_SET);
    }
  };

  // Set pointer to the last location of the final output and insert '\0'.
  // This is for final printing.
  fseek(f_inp, -1L - echo_counter, SEEK_CUR);
  fputc('\0', f_inp);

  // Set file pointer to after echo command text.
  // This is 5L as "echo " has 5 characters.
  fseek(f_inp, 5L, SEEK_SET);

  // Print the required output.
  curr = fgetc(f_inp);
  while (curr != '\0')
  {
    if (curr != '\r') // Ignore carriage return.
      printf("%c", curr);
    curr = fgetc(f_inp);
  };

  // Free up allocated data.
  while (freeEchoVal(echoData))
    ;

  return;
};

int updateHistory(char *curr_cmd, char *prev_cmd, char *cmd_hist, char *old_cmd_hist)
{
  if (strcmp(curr_cmd, prev_cmd) != 0)
  {
    // Renaming previous history file.
    rename(cmd_hist, old_cmd_hist);
    // Opening using append+ mode to allow for file creation in case file doesn't exist.
    FILE *f_p_hist = fopen(old_cmd_hist, "a+");
    if (f_p_hist == NULL)
    {
      perror("Internal Error");
      return -1;
    }
    // Opening the new history file to be written into.
    FILE *f_n_hist = fopen(cmd_hist, "w");
    if (f_n_hist == NULL)
    {
      perror("Internal Error");
      return -1;
    }

    // Handling the counter.
    short hist_count = 0;
    fseek(f_p_hist, 0L, SEEK_SET);
    if (!feof(f_p_hist))
      fscanf(f_p_hist, "%hd", &hist_count);
    hist_count++;

    // Handling printing.
    char cmd[MAX_CMD + 1] = "";
    if (hist_count <= 20)
      // Print new number of commands stored in history.
      fprintf(f_n_hist, "%02hd\n", hist_count);
    else
    {
      // Print new number of commands stored in history.
      fprintf(f_n_hist, "20\n");
      // Ignore the first command.
      fscanf(f_p_hist, "%s\n", cmd);
    }

    // Print remaining commands.
    while (!feof(f_p_hist))
    {
      fscanf(f_p_hist, "%s\n", cmd);
      fprintf(f_n_hist, "%s\n", cmd);
    };
    // Print the new command if the last printed command does not match the current command.
    if (strcmp(cmd, curr_cmd) != 0)
      fprintf(f_n_hist, "%s\n", curr_cmd);
    // Else decrease the
    else
    {
      fseek(f_n_hist, 0L, SEEK_SET);
      fprintf(f_n_hist, "%02hd", hist_count - 1);
    }

    // Handle opened files.
    fclose(f_p_hist);
    remove(old_cmd_hist);
    fclose(f_n_hist);
  };

  return 0;
};

int handle_history(char *cmd_hist)
{
  // Open file containing command history.
  FILE *f_hist = fopen(cmd_hist, "a+");
  if (f_hist == NULL)
  {
    perror("Internal Error");
    return -1;
  }
  fseek(f_hist, 0L, SEEK_SET);

  // Initialise variables and read the number of commands in history.
  char cmd[MAX_CMD + 1] = "";
  short count = 1;
  fscanf(f_hist, "%hd", &count);

  // Skip reading commands until 10 or less commands are left to be read.
  while (count > 10)
  {
    fscanf(f_hist, "%s\n", cmd);
    count--;
  };
  // Print the remaining commands.
  while (count)
  {
    fscanf(f_hist, "%s\n", cmd);
    printf("%s\n", cmd);
    count--;
  };

  // Close opened files.
  fclose(f_hist);

  return 0;
};

int cdHistoryUpdate(char *curr_dir, int cd_ret, char *cd_hist, char *cd_hist_new)
{
  // Open the necessary files.
  FILE *cd_old = fopen(cd_hist, "r");
  if (cd_old == NULL)
  {
    perror("Internal Error");
    return -1;
  }
  FILE *cd_new = fopen(cd_hist_new, "w");
  if (cd_new == NULL)
  {
    perror("Internal Error");
    return -1;
  }

  char path_read[MAX_PATH_LENGTH + 1] = ""; // Array to store paths.
  int counter = 0;                          // Counter to store number of directories in history.

  // Count number of directories in history.
  while (fgets(path_read, MAX_PATH_LENGTH, cd_old) != NULL)
  {
    fscanf(cd_old, "%s", path_read);
    counter++;
  }
  // Reset file pointer to beginning.
  fseek(cd_old, 0L, SEEK_SET);

  // When visiting cd history.
  if (cd_ret == 4)
  {
    // Store history except last directory.
    for (int i = 0; i < counter - 1; i++)
    {
      fscanf(cd_old, "%s ", path_read);
      fprintf(cd_new, "%s\n", path_read);
    }
  }
  // When not visiting cd history.
  else
  {
    // Store all the history.
    for (int i = 0; i < counter; i++)
    {
      fscanf(cd_old, "%s ", path_read);
      fprintf(cd_new, "%s\n", path_read);
    }
    // Store the directory before change.
    fprintf(cd_new, "%s\n", curr_dir);
  }

  fclose(cd_old);
  fclose(cd_new);
  remove(cd_hist);
  rename(cd_hist_new, cd_hist);

  return 0;
};

int main()
{
  char input_array[MAX_BUFFER + 1] = ""; // Array to store the input uptil MAX_BUFFER.
  char prev_cmd[MAX_CMD + 1] = "";       // Array to store the previous command name.
  char curr_cmd[MAX_CMD + 1] = "";       // Array to store the current command name.

  long home_len = GetCurrentDirectory(0, NULL);         // Length of home directory.
  char *home = (char *)malloc(home_len * sizeof(char)); // Array to store home directory.
  GetCurrentDirectory(home_len, home);                  // Store the current directory as home.

  // String to store the prefix of the directory to the helper files.
  char *helper_prefix = (char *)malloc(strlen(getenv("USERPROFILE")) + sizeof("\\Documents"));
  strcpy(helper_prefix, getenv("USERPROFILE"));
  strcat(helper_prefix, "\\Documents");
  // char *helper_prefix = home;

  // Name of helper files.
  char *cmd_hist_txt = createFilePath(helper_prefix, CMD_HIST);         // History of commands.
  char *cmd_hist_old_txt = createFilePath(helper_prefix, CMD_HIST_OLD); // Old command history file.
  char *cd_hist_txt = createFilePath(helper_prefix, CD_HIST);           // History of changes in working directory.
  char *cd_hist_new_txt = createFilePath(helper_prefix, CD_HIST_NEW);   // New changes in working directory history file.
  char *input_txt = createFilePath(helper_prefix, INPUT_FILE);          // File for storing user input.

  // File to store history of changes in current directory.
  FILE *f_cd_hist = fopen(cd_hist_txt, "w");
  if (f_cd_hist == NULL)
  {
    perror("Internal Error");
    return -1;
  }
  // Close after creating.
  fclose(f_cd_hist);

  while (1)
  {
    // Fetching the current directory for displaying.
    long curr_len = GetCurrentDirectory(0, NULL);             // Length of current directory.
    char *curr_dir = (char *)malloc(curr_len * sizeof(char)); // Array to store current directory.
    GetCurrentDirectory(curr_len, curr_dir);                  // Store the current directory.

    // Printing the username, systemname and current directory.
    printf("<%s@%s:~", getenv("USERNAME"), getenv("COMPUTERNAME"));
    // Printing if current directory is not the home directory.
    if (curr_dir[home_len - 1] != '\0')
      printf("%s", &curr_dir[home_len - 1]);
    printf("> ");

    // File to write input to for parsing later on.
    FILE *f_inp = fopen(input_txt, "w+");
    if (f_inp == NULL)
    {
      perror("Internal Error");
      return -1;
    }
    // Reading the input.
    readInputAndWriteToFile(input_array, f_inp);
    // Reset pointer to beginning.
    fseek(f_inp, 0L, SEEK_SET);
    // Reading the command.
    fscanf(f_inp, "%s ", curr_cmd);
    // In case of empty command.
    if (ftell(f_inp) <= 2L)
      strcpy(curr_cmd, "");

    // For empty command.
    if (strcmp(curr_cmd, "") == 0)
      ;
    // For echo command.
    else if (strcmp(curr_cmd, "echo") == 0)
    {
      updateHistory(curr_cmd, prev_cmd, cmd_hist_txt, cmd_hist_old_txt);
      handle_echo(f_inp, input_array);
      strcpy(prev_cmd, curr_cmd);
      printf("\n");
    }
    // For pwd command.
    else if (strcmp(curr_cmd, "pwd") == 0)
    {
      updateHistory(curr_cmd, prev_cmd, cmd_hist_txt, cmd_hist_old_txt);
      pwd();
      strcpy(prev_cmd, curr_cmd);
    }
    // For cd command.
    else if (strcmp(curr_cmd, "cd") == 0)
    {
      updateHistory(curr_cmd, prev_cmd, cmd_hist_txt, cmd_hist_old_txt);

      f_cd_hist = fopen(cd_hist_txt, "r");
      // If file wasn't opened.
      if (f_cd_hist == NULL)
      {
        perror("Internal Error");
        return -1;
      }

      // Return value after handling cd.
      int cd_ret = cd(f_inp, f_cd_hist, home);
      fclose(f_cd_hist);

      // Updating cd history.
      if (cd_ret == 0 || cd_ret == 4)
        cdHistoryUpdate(curr_dir, cd_ret, cd_hist_txt, cd_hist_new_txt);

      strcpy(prev_cmd, curr_cmd);
    }
    // For history command.
    else if (strcmp(curr_cmd, "history") == 0)
    {
      updateHistory(curr_cmd, prev_cmd, cmd_hist_txt, cmd_hist_old_txt);
      handle_history(cmd_hist_txt);
      strcpy(prev_cmd, curr_cmd);
    }
    // For exit command.
    else if (strcmp(curr_cmd, "exit") == 0)
    {
      updateHistory(curr_cmd, prev_cmd, cmd_hist_txt, cmd_hist_old_txt);
      // Close opened files.
      fclose(f_inp);
      // Remove temporary files.
      remove(input_txt);
      remove(cd_hist_txt);
      // Free up allocated memory.
      free(curr_dir);
      free(home);
      free(helper_prefix);
      free(cmd_hist_txt);
      free(cmd_hist_old_txt);
      free(cd_hist_txt);
      free(cd_hist_new_txt);
      free(input_txt);

      // Exit the shell.
      return 0;
    }
    // For invalid command.
    else
      fprintf(stderr, "Error: Invalid command.\n");

    // Close the file to read input into.
    fclose(f_inp);
    remove(input_txt);
    // Freeing memory allocated for storing the current directory.
    free(curr_dir);
  };

  return 1;
};
