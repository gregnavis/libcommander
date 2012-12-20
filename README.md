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

int command_handler(int argc, const char **argv, void *arg)
{
    /* Handle the command here. */
}

struct commander_command the_command = {
    NULL, /* the command doesn't have a name */
    {
        /* supported flags and options */
        commander_flag('v', "verbose", &flag_verbose),
        commander_flag('k', "keep-alive", &flag_keep_alive),
        commander_option_string('p', "proxy", &proxy)
        commander_option_int('d', "delay", &delay)
    },
    command_handler,
    NULL /* no arg is passed to the handler */
};

int main(int argc, char *argv[])
{
    return commander_handle(&argc, &argv, &the_command);
}
```

It supports two boolean flags (`-v` and `--verbose`, `-k` and `--keep-alive`)
and two options (`-p` and `--proxy`, `-d` and `--delay`). The flags are not set
by default, there's no proxy and no delay. The function `commander_handle`
does all the command line processing (including setting the flags and
converting string to integers!) and then passes control to the
`command_handler`. It receives remaining command line arguments in `argv`
and their number in `argc`. The `arg` argument is the last field of the
`the_command` structure (`NULL` in this case). The value returned from the
`command_handler` is the value returned from the handler.
