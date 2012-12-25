libcommander - command line parsing for everyone!
=================================================

Example usage
-------------

Let's skip all the talking and look at the code.

```c
#include <commander.h>

/* The defaults */
int flag_verbose = 0;
int flag_keep_alive = 0;
const char *proxy = NULL;
int delay = 0;

int command_handler(int argc, const char **argv)
{
    /* Handle the command here. */
}

struct commander main_command = commander_command(command_handler,
    {
        /* supported flags and options */
        commander_flag('v', "verbose", &flag_verbose),
        commander_flag('k', "keep-alive", &flag_keep_alive),
        commander_option_string('p', "proxy", &proxy)
        commander_option_int('d', "delay", &delay)
    },
)

int main(int argc, char *argv[])
{
    return commander_handle(&argc, &argv, &main_command);
}
```

It supports two boolean flags (`-v` and `--verbose`, `-k` and `--keep-alive`)
and two options (`-p` and `--proxy`, `-d` and `--delay`). The flags are not set
by default, there's no proxy and no delay. The function `commander_handle`
does all the command line processing (including setting the flags and
converting string to integers!) and then passes control to the
`command_handler`. It receives remaining command line arguments in `argv`
and their number in `argc`. The `arg` argument is the last field of the
`main_command` structure (`NULL` in this case). The value returned from the
`command_handler` is the value returned from the handler.

But there's more! Look at this

```c
struct commander help_command = commander_command(help_handler, NULL);

/* Some more commands like download_command etc.. */

struct commander main_command = commander_group(
    { "help", help_command },
    { "download", download_command }
    /* and so on ... */
)
```

The definition above introduces many commands. Among them are `help` and
`download`. The program using the above definition can be called like

    program download --an-option a-value --a-flag some-argument

There's even more! Commands can be organized into trees!

```c
struct commander help_command = commander_command(help, NULL);
struct commander generate_controller_command = commander_command(
    generate_controller,
    NULL
);
struct commander generate_model_command = commander_command(
    generate_model,
    NULL
);

struct commander main_command = commander_group({
    { "help", help_command },
    { "generate", commander_group({
        { "controller", generate_controller_command },
        { "model", generate_model_command }
    })}
})
```

The definition above introduces a top level command `help` and a group of
commands called `generate`. It has two subcommands: `controller` and `model`.
