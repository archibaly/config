#include <stdio.h>

#include "config.h"

int main(int argc, char *argv[])
{
    if (config_load("simple.conf") < 0)
		return -1;

	config_print_opt("name");
	config_print_opt("age");
	config_print_opt("sex");
	config_print_opt("test");

	printf("name = %s\n", config_get_value("name"));
	printf("age = %s\n", config_get_value("age"));

	config_set_value("name", "jacky");
	config_set_value("age", "27");
	config_set_value("sex", "male");

	config_save("simple.conf");

    config_free();

    return 0;
}
