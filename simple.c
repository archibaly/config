#include <stdio.h>

#include "config.h"

int main(int argc, char *argv[])
{
    config_opt_t *opt;
    int i;

    if (argc != 2)
		return 1;

    config_init();
    config_load("simple.conf");

    opt = config_get_opt(argv[1]);

    if (opt == NULL) {
		puts("not found");
		return 1;
    }

    if (opt->is_array)
		for (i = 0; i < opt->size; i++)
			printf("%d. %s\n", i + 1, opt->values[i]);
    else
		printf("%s\n", opt->value);

    config_free();

    return 0;
}
