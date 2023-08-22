# Program Documentation: Saldo Calculation

## Table of Contents
1. [Introduction](https://github.com/asakura42/saldo/edit/master/README.md#1-introduction)
2. [Usage]
3. [Command Line Options]
4. [Configuration File]
5. [Implementation Details]
6. [Examples]

## 1. Introduction
The Saldo Calculation program is designed to calculate the current saldo for a given month based on the expenses recorded in a configuration file. It provides a simple and convenient way to manage and track your expenses throughout the month. The program takes into account the user's income, fixed expenses, and daily expenses to calculate the saldo for each day of the month.

## 2. Usage
The program can be executed from the command line with various options and arguments. It supports the following general usage format:

```
saldo [options] [arguments]
```

## 3. Command Line Options
The program supports the following command line options:

- `--help`, `-h`: Displays the help text, providing information about the program and its usage.
- `--config`, `-cfg` [path]: Specifies an alternative configuration file path instead of the default `~/.local/share/saldo_config.txt`.
- `--today`, `-t`: Shows only the saldo for today and tomorrow.
- `--edit-config`, `-c`: Opens the configuration file in a text editor (neovim, vim, or nano) for manual editing.
- `--edit`, `-e` [day] [expense]: Edits the expenses for the specified day in the configuration file.
- `--add`, `-a` [day] [expense]: Adds the specified expense to the existing expenses for the specified day in the configuration file.
- `--new-config`, `--rebuild-config` [income] [expenses] (optional): Rebuilds the configuration file, optionally specifying the income and fixed expenses for the new month.

## 4. Configuration File
The program uses a configuration file to store the user's income, fixed expenses, and daily expenses for each day of the month. The default configuration file path is `~/.local/share/saldo_config.txt`, but an alternative path can be specified using the `--config` option.

The configuration file has the following format:

```
income [amount]
fixed_expenses [amount]

[day] [expense]
[day] [expense]
...

# Comments can be added starting with a '#' symbol
```

The `income` field represents the user's monthly income, and the `fixed_expenses` field represents the total fixed expenses for the month. Each line following these fields represents the expenses for a specific day of the month. The `[day]` field denotes the day of the month, and the `[expense]` field represents the expenses for that day.

## 5. Implementation Details
The program is implemented in C and uses several standard library functions for file handling, string manipulation, and time-related operations. It follows a modular approach with different functions for handling specific tasks.

The main function performs the following steps:

1. Parses the command line arguments to determine the desired functionality.
2. Reads the current month and the number of days in the month.
3. Checks for the `--help` option and displays the help text if detected.
4. Handles the `--new-config` or `--rebuild-config` options to rebuild the configuration file for a new month.
5. Handles the `--edit-config` option to open the configuration file in a text editor for manual editing.
6. Handles the `--edit` option to modify the expenses for a specific day in the configuration file.
7. Handles the `--add` option to add expenses for a specific day in the configuration file.
8. Checks if the configuration file exists and generates it if not.
9. Calculates and prints the saldo for each day of the month based on the configuration file.
10. Optionally, displays only the saldo for today and tomorrow if the `--today` option is specified.

## 6. Examples
Here are some examples of how to use the Saldo Calculation program:

- Calculate the saldo for the current month:
```
saldo
```

- Calculate the saldo for today and tomorrow only:
```
saldo --today
```

- Calculate the saldo using a custom configuration file:
```
saldo --config /path/to/config.txt
```

- Edit the configuration file using the neovim editor:
```
saldo --edit-config
```

- Edit the expenses for a specific day in the configuration file:
```
saldo --edit 15 50.75
```

- Add expenses for a specific day in the configuration file:
```
saldo --add 20 30.50
```

- Rebuild the configuration file for the new month:
```
saldo --new-config 2000 500
```
