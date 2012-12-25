#include <stdio.h>

#include "commander.h"

int flag_verbose = 0;
int flag_help = 0;

const char *main_message =
	"verbose = %d\n"
	"help = %d\n"
	"argc = %d\n";

int main_handler(int argc, char *argv[])
{
	printf(main_message, flag_verbose, flag_help, argc);

	for (int i = 0; i < argc; i++) {
		printf("argv[%d] = \"%s\"\n", i, argv[i]);
	}

	return 0;
}

struct commander *main_command = commander_command(
	main_handler,
	commander_flag('v', "verbose", &flag_verbose),
	commander_flag('h', "help", &flag_help)
);

int main(int argc, char *argv[])
{
	int code = commander_handle(argc, argv, main_command);
	printf("code = %d\n", code);
	return code;
}
