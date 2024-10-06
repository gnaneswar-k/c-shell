# C Shell

***Note: This shell code has been written to be run on a computer with the Microsoft Windows Operating System. Commands are likely to not run on other operating systems.***

## Table of contents

- [C Shell](#c-shell)
  - [Table of contents](#table-of-contents)
  - [Compilation instructions](#compilation-instructions)
  - [List of commands](#list-of-commands)
    - [`echo` command](#echo-command)
      - [Usage of `echo`](#usage-of-echo)
      - [Available features in `echo`](#available-features-in-echo)
      - [Unavailable features in `echo`](#unavailable-features-in-echo)
      - [Examples for `echo`](#examples-for-echo)
    - [`pwd` command](#pwd-command)
      - [Usage of `pwd`](#usage-of-pwd)
      - [Example for `pwd`](#example-for-pwd)
    - [`cd` command](#cd-command)
      - [Usage of `cd`](#usage-of-cd)
      - [Available features in `cd`](#available-features-in-cd)
      - [Unavailable features in `cd`](#unavailable-features-in-cd)
      - [Examples for `cd`](#examples-for-cd)
    - [`history` command](#history-command)
      - [Usage of `history`](#usage-of-history)
      - [Example for `history`](#example-for-history)
    - [`exit` command](#exit-command)
      - [Usage of `exit`](#usage-of-exit)
  - [Directory structure](#directory-structure)

## Compilation instructions

1. Open your preferred terminal which can detect the `gcc` command.
2. Navigate to the current `c-shell` folder.
3. Run the `make all` command.
4. Open the shell using `./shell.exe`.
    - Alternatively, you can also double click and run the executable file.
5. To clear the files, run `make clear`.
    - Note: A helper file named `cmd_hist.txt` is created in the user's `Documents` folder on the first run of the shell, which does not get automatically deleted when the `make clear` command is run.

## List of commands

1. [`echo`](#echo-command)
2. [`pwd`](#pwd-command)
3. [`cd`](#cd-command)
4. [`history`](#history-command)
5. [`exit`](#exit-command)

### `echo` command

Prints back the user input with formatting if necessary.

***Note:** Even though the shell is configured to run in Windows OS, the `echo` command is designed to simulate the functioning of `echo` in a Linux environment.*

#### Usage of `echo`

```shell
echo <prompt>
```

```shell
echo "<prompt_line_1>
> <prompt_line_2>
> ..."
```

#### Available features in `echo`

- Truncates extra white spaces.
- Handling of quotes, i.e. when a double quote is started, characters are read as is without any truncation/modification until double quotes are closed.

#### Unavailable features in `echo`

- If a user starts a double quote and enters a new line, then they cannot cancel the new line.
- Cannot parse escape sharacters such as `\b`, `\t`, `\r`, etc. if entered manually as text in the prompt.

#### Examples for `echo`

```shell
<USERNAME@WINDOWS:~> echo Hi!
Hi!
<USERNAME@WINDOWS:~> 
```

```shell
<USERNAME@WINDOWS:~> echo Hi! "My name is:
> FirstName   LastName."    Nice to meet you!
Hi! My name is:
FirstName   LastName. Nice to meet you!
<USERNAME@WINDOWS:~> 
```

### `pwd` command

Prints the current working directory of the shell.

#### Usage of `pwd`

```shell
pwd
```

#### Example for `pwd`

```shell
<USERNAME@WINDOWS:~> pwd
C:\Users\USERNAME
<USERNAME@WINDOWS:~> 
```

### `cd` command

Navigates to the specified directory.

#### Usage of `cd`

```shell
cd <prompt>
```

#### Available features in `cd`

- Parses special inputs such as `.`, `..`, `-`, `~` and empty input.
- Remembers the entire history of changes in directory in a shell session.
- Can handle nested directories.

#### Unavailable features in `cd`

- Cannot navigate to directories which have spaces in their name, regardless of the presence of double quotes.
- Cannot navigate above the home directory.
- If an error occurs in the middle of executing a nested file path, then the shell's working directory is set to the sub-path till which execution was successful.

#### Examples for `cd`

```shell
<USERNAME@WINDOWS:~> cd .
<USERNAME@WINDOWS:~> cd folder
<USERNAME@WINDOWS:~\folder> cd
<USERNAME@WINDOWS:~> cd -
<USERNAME@WINDOWS:~\folder> cd ~
<USERNAME@WINDOWS:~> cd folder/nested_folder_1/
<USERNAME@WINDOWS:~\folder\nested_folder_1> cd ../nested_folder_2
<USERNAME@WINDOWS:~\folder\nested_folder_2>
```

```shell
<USERNAME@WINDOWS:~> cd folder
<USERNAME@WINDOWS:~\folder> cd nested_folder_1
<USERNAME@WINDOWS:~\folder\nested_folder_1> cd ../nested_folder_2
<USERNAME@WINDOWS:~\folder\nested_folder_2> cd -
<USERNAME@WINDOWS:~\folder\nested_folder_1> cd -
<USERNAME@WINDOWS:~\folder> cd -
<USERNAME@WINDOWS:~>
```

### `history` command

Lists the history of commands previously run on the shell across sessions.

#### Usage of `history`

```shell
history
```

#### Example for `history`

```shell
<USERNAME@WINDOWS:~> echo Hi!
Hi!
<USERNAME@WINDOWS:~> pwd       
C:\Users\USERNAME
<USERNAME@WINDOWS:~> echo Greetings.
Greetings.
<USERNAME@WINDOWS:~> echo Hello! 
Hello!
<USERNAME@WINDOWS:~> history
echo
pwd
echo
history
<USERNAME@WINDOWS:~> exit
```

Upon running the shell again,

```shell
<USERNAME@WINDOWS:~> history
echo
pwd
echo
history
exit
history
<USERNAME@WINDOWS:~>
```

### `exit` command

Exits the shell.

#### Usage of `exit`

```shell
exit
```

## Directory structure

```text
c-shell
|_cd_code
  |_cd.c
  |_cd.h
|_echo_code
  |_echo.c
  _|echo.h
|_pwd_code
  |_pwd.c
  |_pwd.h
|_shell_code
  |_shell.c
.gitignore
makefile
README.md
```

The `*_code` folders, where `*` is a placeholder for the command name, contain the files specifying the declarations and definitions of the functions of the respective commands.

The `.gitignore` file is for specifying the files to ignore if a git repository is initialised in the folder.

The `makefile` file contains the commands for compilation and deletion of the shell.

The `README.md` file contains the compilation instructions, description of the commands and the file structure.
