#include <stdio.h>

#include "commander.h"

int flag_verbose = 0;
int flag_help = 0;

const char *report_message =
	"command = %s\n"
	"verbose = %d\n"
	"help = %d\n"
	"argc = %d\n";

const char *help_message = "help\n";

int report(char *name, int argc, char *argv[])
{
	printf(report_message, name, flag_verbose, flag_help, argc);

	for (int i = 0; i < argc; i++) {
		printf("argv[%d] = \"%s\"\n", i, argv[i]);
	}

	return 0;
}

int main_handler(int argc, char *argv[])
{
	return report("main", argc, argv);
}

int help_handler(int argc, char *argv[])
{
	return report("help", argc, argv);
}

int one_handler(int argc, char *argv[])
{
	return report("one", argc, argv);
}

int two_handler(int argc, char *argv[])
{
	return report("two", argc, argv);
}

struct commander *main_command = commander_group(
	"main", commander_command(main_handler,
		commander_flag('v', "verbose", &flag_verbose),
		commander_flag('h', "help", &flag_help)
	),
	"help", commander_command0(help_handler),
	"subtree", commander_group(
		"one", commander_command0(one_handler),
		"two", commander_command0(two_handler)
	)
);

int main(int argc, char *argv[])
{
	int code = commander_handle(argc, argv, main_command);
	printf("code = %d\n", code);
	return code;
}
