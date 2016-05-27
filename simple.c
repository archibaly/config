#include <stdio.h>
#include "config.h"

int main(int argc, char *argv[])
{
    config_load("simple.conf");

	config_print_opt("name");
	config_print_opt("age");
	config_print_opt("sex");

	printf("name = %s\n", config_get_value("name"));
	printf("age = %s\n", config_get_value("age"));

	config_set_value("name", "jacky liu");
	config_set_value("age", "25");
	config_set_value("sex", "male");

	config_save("simple.conf");

    config_free();

    return 0;
}
