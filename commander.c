#include <stdio.h>
#include <string.h>

#include "commander.h"

#define COMMANDER_OPTION_DELIMITER "--"

static int _commander_handle(int, char **, struct commander *);

static int is_option_delimiter(char *option)
{
	return !strcmp(option, COMMANDER_OPTION_DELIMITER);
}

static int is_short_option(char *option)
{
	return '-' == option[0]
		&& 2 == strlen(option)
		&& '-' != option[1];
}

static int is_long_option(char *option)
{
	return '-' == option[0]
		&& strlen(option) > 2
		&& '-' == option[1]
		&& '-' != option[2];
}

static char *get_short_name(struct commander_option *option, char *buffer)
{
	buffer[0] = option->short_name;
	buffer[1] = '\0';
	return buffer;
}

static char *get_long_name(struct commander_option *option, char *buffer)
{
	return option->long_name;
}

static int handle_option(int *argc_ptr,
	char ***argv_ptr,
	struct commander_option *supported_options[])
{
	char * (*name_function)(struct commander_option *, char *);
	struct commander_option **option;
	char *argument = **argv_ptr, *name;

	if (!*argc_ptr || !argument) {
		return 0;
	}

	if (is_option_delimiter(argument)) {
		(*argc_ptr)--;
		(*argv_ptr)++;
		return 0;
	}

	if (is_short_option(argument)) {
		name_function = get_short_name;
		name = argument + 1;
	} else if (is_long_option(argument)) {
		name_function = get_long_name;
		name = argument + 2;
	} else {
		return 0;
	}

	(*argc_ptr)--;
	(*argv_ptr)++;

	for (option = supported_options; *option; option++) {
		char buffer[2];
		char *supported_option_name = name_function(*option, buffer);

		if (!strcmp(supported_option_name, name)) {
			return (*option)->handler(argc_ptr, argv_ptr, (*option)->data);
		}
	}

	return -1;
}

static int handle_options(int *argc_ptr,
	char ***argv_ptr,
	struct commander_option *options[])
{
	int result;

	do {
		result = handle_option(argc_ptr, argv_ptr, options);
	} while (result > 0);

	return result;
}

int commander_group_handler(int *argc_ptr, char ***argv_ptr, void *data_ptr)
{
	char *command = **argv_ptr;
	void **element_ptr = data_ptr;
	int result;

	if (!*argc_ptr) {
		goto not_found;
	}

	while (*element_ptr) {
		char *handler_name = *element_ptr++;
		void *handler_data = *element_ptr++;

		if (strcmp(command, handler_name) == 0) {
			(*argc_ptr)--;
			(*argv_ptr)++;
			result = _commander_handle(*argc_ptr,
					*argv_ptr,
					handler_data);
			goto out;
		}
	}

not_found:
	result = -1;

out:
	return result;
}

int commander_command_handler(int *argc_ptr, char ***argv_ptr, void *data_ptr)
{
	struct commander_command *command_ptr = data_ptr;
	int result;

	result = handle_options(argc_ptr, argv_ptr, command_ptr->options);
	if (result < 0) {
		goto out;
	}

	result = command_ptr->handler(*argc_ptr, *argv_ptr);

out:
	return result;
}

int commander_flag_handler(int *argc_ptr, char ***argv_ptr, void *flag_ptr)
{
	(*(int *)flag_ptr) = 1;
	return 1;
}

int _commander_handle(int argc, char **argv, struct commander *commander_ptr)
{
	int argc_copy = argc, result = -1;
	char **argv_copy = argv;

	result = commander_ptr->handler(&argc_copy,
		&argv_copy,
		commander_ptr->data);

	return result;
}

int commander_handle(int argc, char **argv, struct commander *commander_ptr)
{
	return _commander_handle(argc - 1, argv + 1, commander_ptr);
}
