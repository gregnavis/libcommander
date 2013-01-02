/* The internals. */

int commander_group_handler(int *, char ***, void *);
int commander_command_handler(int *, char ***, void *);
int commander_flag_handler(int *, char ***, void *);

struct commander_option {
	char short_name;
	char *long_name;
	int (*handler)(int *, char ***, void *);
	void *data;
};

struct commander_command {
	int (*handler)(int, char **);
	struct commander_option **options;
};

struct commander {
	int (*handler)(int *, char ***, void *);
	void *data;
};

/* The public API. */

#define commander_group(...) \
	&((struct commander) { \
		commander_group_handler, \
		&((void *[]) { __VA_ARGS__, NULL }) \
	})

#define commander_command0(handler) \
	&((struct commander) { \
		commander_command_handler, \
		&((struct commander_command) { \
			handler, \
			(struct commander_option *[]) { NULL } \
		}) \
	})

#define commander_command(handler, ...) \
	&((struct commander) { \
		commander_command_handler, \
		&((struct commander_command) { \
			handler, \
			(struct commander_option *[]) { __VA_ARGS__, NULL } \
		}) \
	})

#define commander_flag(short_name, long_name, flag_ptr) \
	&((struct commander_option) { \
		short_name, \
		long_name, \
		commander_flag_handler, \
		flag_ptr \
	})

int commander_handle(int, char **, struct commander *);
