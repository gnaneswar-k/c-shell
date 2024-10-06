#ifndef __CD_HEADER__
#define __CD_HEADER__

#define MAX_PATH_LENGTH 1024

/**
 * Function to handle processing of given path.
 * @param path Strring containing the input path.
 * @param cd_hist File containing history of past destinations.
 * @param home String containing the path of home folder.
 * @returns 0 if successful.
 * @returns -1 if wrong number of arguments.
 * @returns 1 if system error.
 * @returns 2 if user error.
 * @returns 3 if command error.
 * @returns 4 if history.
 */
int handle_cd(char *path, char *home, FILE *cd_hist);

/**
 * Function to execute change directory command.
 * @param f_inp File containing the input parameters for cd.
 * @param cd_hist File containing history of past destinations.
 * @param home String containing the path of home folder.
 * @returns 0 if successful.
 * @returns -1 if wrong number of arguments.
 * @returns 1 if system error.
 * @returns 2 if user error.
 * @returns 3 if command error.
 * @returns 4 if history.
 */
int cd(FILE *f_inp, FILE *cd_hist, char *home);

#endif
