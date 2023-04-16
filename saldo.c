#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Function to print help text when --help option is detected
void print_help_text()
{
	printf("This program calculates the current saldo for a given month by "
		   "reading expenses from a configuration file.\n");
	printf("Usage: saldo [--help|-h] [--edit-config|-c] [--edit|-e [day] "
		   "[expense]] [--add|-a [day] [expense]] [--new-config|--rebuild-config "
		   "[income] [expenses]] [number]\n");
	printf("--help|-h: Displays the help text.\n");
	printf("--edit-config|-c: Edit config manually using neovim, vim or nano.\n");
	printf(
		"--edit|-e [day] [expense]: Edits the expenses for the specified day.\n");
	printf("--add|-a [day] [expense]: Adds the specified expense to the existing "
		   "expense for the specified day.\n");
	printf("--config|-cfg ... : Use another config.\n");
	printf("--new-config|--rebuild-config [income] [expenses] (optionally): "
		   "rebuilds config (for example, after starting next month). Be "
		   "careful!\n");
	printf("Without any flag you can write just a number like `saldo 1000' and "
		   "this will work like `saldo --add [today] 1000'\n");
	printf(
		"If the configuration file is not found, the program will generate one "
		"in ~/.local/share/saldo_config.txt.\nThe configuration file contains "
		"the income, fixed expenses and expenses for each day of the month.\nThe "
		"program will then calculate the saldo for each day based on the daily "
		"budget (income - fixed expenses / days in the month).\nThe daily budget "
		"and the saldo for each day will be displayed.\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	double income, fixedExpenses, totalBudget, dailyBudget, expenses, saldo;
	int month, day, daysInMonth;
	char configFilePath[256];
	// Check if --config or -cfg flag is passed
	if (argc > 1 &&
		(strcmp(argv[1], "--config") == 0 || strcmp(argv[1], "-cfg") == 0))
	{
		sprintf(configFilePath, "%s", argv[2]);
		// Shift argv two positions
		memmove(argv, argv + 2, (argc - 2) * sizeof(char *));
	}
	else
	{
		sprintf(configFilePath, "%s/.local/share/saldo_config.txt", getenv("HOME"));
	}
	FILE *configFile = fopen(configFilePath, "r+"); // Read and write

	// Get the current month
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	month = timeinfo->tm_mon + 1;
	int today = timeinfo->tm_mday;
	int yesterday = timeinfo->tm_mday - 1;

	// Get the number of days in the month
	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 ||
		month == 10 || month == 12)
		daysInMonth = 31;
	else if (month == 2)
		daysInMonth = 28;
	else
		daysInMonth = 30;

	// Print help text if --help option is detected
	if (argc > 1 &&
		(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
	{
		print_help_text();
	}

	if (argc > 3 && (strcmp(argv[1], "--new-config") == 0 ||
					 strcmp(argv[1], "--rebuild-config") == 0))
	{
		printf("Rebuilding config...\nDon't forget to edit it\n");
		FILE *configFile = fopen(configFilePath, "w");
		fprintf(configFile, "income %s\nfixed_expenses %s\n", argv[2], argv[3]);
		for (int i = 1; i <= daysInMonth; i++)
		{
			fprintf(configFile, "%d 0\n", i);
		}
		fclose(configFile);
		exit(0);
	}

	if (argc > 1 && (strcmp(argv[1], "--new-config") == 0 ||
					 strcmp(argv[1], "--rebuild-config") == 0))
	{
		printf("Rebuilding config...\nDon't forget to edit it\n");
		FILE *configFile = fopen(configFilePath, "w");
		fprintf(configFile, "income 0\nfixed_expenses 0\n");
		for (int i = 1; i <= daysInMonth; i++)
		{
			fprintf(configFile, "%d 0\n", i);
		}
		fclose(configFile);
		exit(0);
	}

	// Add option to open neovim to edit config
	if (argc > 1 &&
		(strcmp(argv[1], "--edit-config") == 0 || strcmp(argv[1], "-c") == 0))
	{
		// Check if neovim exists
		if (system("which nvim") == 0)
		{
			char command[256];
			sprintf(command, "nvim %s", configFilePath);
			system(command);
		}
		// Check if nano exists
		else if (system("which nano") == 0)
		{
			char command[256];
			sprintf(command, "vim %s", configFilePath);
			system(command);
		}
		// Check if vi exists
		else if (system("which vi") == 0)
		{
			char command[256];
			sprintf(command, "nano %s", configFilePath);
			system(command);
		}
		exit(0);
	}

	// If --edit option is detected, edit the config file
	if (argc > 3 &&
		(strcmp(argv[1], "--edit") == 0 || strcmp(argv[1], "-e") == 0))
	{
		int editDay = atoi(argv[2]);
		double editExpense = atof(argv[3]);

		// Create a new file
		char newConfigFilePath[256];
		sprintf(newConfigFilePath, "%s/.local/share/saldotmpconfig.txt",
				getenv("HOME"));
		FILE *newConfigFile = fopen(newConfigFilePath, "w");

		// Find the line
		char line[100];
		while (fgets(line, sizeof(line), configFile) != NULL)
		{
			if (atoi(line) == editDay)
			{
				fprintf(newConfigFile, "%d %.2lf\n", editDay, editExpense);
			}
			else
			{
				fprintf(newConfigFile, "%s", line);
			}
		}

		// Close the files
		fclose(configFile);
		fclose(newConfigFile);

		// Rename the new file
		rename(newConfigFilePath, configFilePath);

		exit(0);
	}

	// If --add option is detected, add expenses from user input to config file
	if (argc > 3 &&
		(strcmp(argv[1], "--add") == 0 || strcmp(argv[1], "-a") == 0))
	{
		int addDay = atoi(argv[2]);
		double addExpense = atof(argv[3]);

		// Create a new file
		char newConfigFilePath[256];
		sprintf(newConfigFilePath, "%s/.local/share/saldotmpconfig.txt",
				getenv("HOME"));
		FILE *newConfigFile = fopen(newConfigFilePath, "w");

		// Find the line
		char line[100];
		while (fgets(line, sizeof(line), configFile) != NULL)
		{
			if (atoi(line) == addDay)
			{
				double existingExpense;
				sscanf(line, "%d %lf", &addDay, &existingExpense);
				addExpense += existingExpense;
				fprintf(newConfigFile, "%d %.2lf\n", addDay, addExpense);
			}
			else
			{
				fprintf(newConfigFile, "%s", line);
			}
		}

		// Close the files
		fclose(configFile);
		fclose(newConfigFile);

		// Rename the new file
		rename(newConfigFilePath, configFilePath);

		exit(0);
	}

	if (argc == 2)
	{
		double expense = atof(argv[1]);

		// Create a new file
		char newConfigFilePath[256];
		sprintf(newConfigFilePath, "%s/.local/share/saldotmpconfig.txt",
				getenv("HOME"));
		FILE *newConfigFile = fopen(newConfigFilePath, "w");

		// Find the line
		char line[100];
		while (fgets(line, sizeof(line), configFile) != NULL)
		{
			if (atoi(line) == today)
			{
				double existingExpense;
				sscanf(line, "%d %lf", &today, &existingExpense);
				expense += existingExpense;
				fprintf(newConfigFile, "%d %.2lf\n", today, expense);
			}
			else
			{
				fprintf(newConfigFile, "%s", line);
			}
		}

		// Close the files
		fclose(configFile);
		fclose(newConfigFile);

		// Rename the new file
		rename(newConfigFilePath, configFilePath);
		exit(0);
	}

	// Check if the configuration file exists and generate it if not
	if (!configFile)
	{
		printf("No config file found. Generating one now...\nDon't forget to edit "
			   "it\n");
		FILE *configFile = fopen(configFilePath, "w");
		fprintf(configFile, "income 0\nfixed_expenses 0\n");
		for (int i = 1; i <= daysInMonth; i++)
		{
			fprintf(configFile, "%d 0\n", i);
		}
		fclose(configFile);
		exit(0);
	}

	// Read income and fixed expenses from the configuration file
	fscanf(configFile, "income %lf\nfixed_expenses %lf\n", &income,
		   &fixedExpenses);
	totalBudget = income - fixedExpenses;
	dailyBudget = totalBudget / daysInMonth;
	saldo = 0;

	// Calculate the saldo for each day and print the result
	for (day = 1; day <= daysInMonth; day++)
	{
		if (fscanf(configFile, "%d %lf\n", &day, &expenses) != EOF)
		{
			saldo += dailyBudget - expenses;
		}
		else
		{
			saldo += dailyBudget;
		}
		//		if (day == yesterday)
		//		{
		//			printf("On the %d you may spend %.2lf\033[0;31m\n", day, saldo);
		//		}
		if (day == today)
		{
			printf("On the %d you may spend %.2lf *\n", day, saldo);
			//			printf("\033[0m");
		}
		else
		{
			printf("On the %d you may spend %.2lf\n", day, saldo);
		}
	}
	// Display the daily budget
	printf("Your average daily budget for this month is %.2lf\n", dailyBudget);
	fclose(configFile);
	return 0;
}
