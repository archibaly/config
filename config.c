/* 
 * config.c
 *
 * Copyright (c) 2012 "config" Niels Vanden Eynde 
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.*
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "debug.h"
#include "config.h"

#define DQUOTE		'"'

#define END_LINE(c)	(c == '\n' || c == '\0')

static config_opt_t *config_table = NULL;

static char delim = '=';
static char comment = '#';

static config_opt_t *new_config_opt(const char *name, const char *value)
{
	config_opt_t *opt;

	if (!(opt = malloc(sizeof(config_opt_t))))
		exit(EXIT_FAILURE);

	if (!(opt->name = strdup(name)))
		exit(EXIT_FAILURE);

	if (!(opt->value = strdup(value)))
		exit(EXIT_FAILURE);

	return opt;
}

static config_opt_t *config_add_opt(const char *name, const char *value)
{
	config_opt_t *opt;

	HASH_FIND_STR(config_table, name, opt);
	if (opt == NULL) {
		opt = new_config_opt(name, value);
		HASH_ADD_STR(config_table, name, opt);
	}

	return opt;
}

static config_opt_t *config_get_opt(const char *name)
{
	config_opt_t *opt;

	HASH_FIND_STR(config_table, name, opt);

	return opt;
}

char *config_get_value(const char *name)
{
	char *value;
	config_opt_t *opt;

	opt = config_get_opt(name);

	if (opt)
		value = opt->value;
	else
		value = NULL;

	return value;
}

void config_set_value(const char *name, const char *value)
{
	config_opt_t *opt;
	opt = config_get_opt(name);
	if (opt) {
		free(opt->value);
		if (!(opt->value = strdup(value)))
			exit(EXIT_FAILURE);
	} else {
		config_add_opt(name, value);
	}
}

void config_set_delim(char d)
{
	delim = d;
}

void config_print_opt(const char *name)
{
	config_opt_t *opt;

	opt = config_get_opt(name);

	if (opt == NULL) {
		fprintf(stdout, "NULL => NULL\n");
		return;
	}

	fprintf(stdout, "name => %s\n", opt->name);
	fprintf(stdout, "value => %s\n", opt->value);
}

static int parse_line(char *string)
{
	char value[512], name[512], c;
	int have_name, have_quote;
	int i = 0;

	have_name = have_quote = 0;

	while ((c = *string++) != '\0') {
		if (c == DQUOTE) {
			if (!have_name) {
				debug("unexpected '%c'", DQUOTE);
				return -1;
			}
			if (have_quote && !END_LINE(*string)) {
				debug("unexpected '%c' after '%c'", *string, DQUOTE);
				return -1;
			}
			have_quote = !have_quote;
		} else if (c == ' ') {
			/* ignore spaces outside of quotes. */
			if (have_quote)
				value[i++] = c;
		} else if (c == delim) {
			if (have_name) {
				debug("unexpected '%c'", delim);
				return -1;
			}

			have_name = 1;
			name[i] = '\0';
			i = 0;
		} else if (c == '\n') {
			break;
		} else {
			if (have_name)
				value[i++] = c;
			else
				name[i++] = c;
		}
	}

	value[i] = '\0';

	if (!have_name) {
		debug("do not have name");
		return -1;
	}
	if (have_quote) {
		debug("need another quote");
		return -1;
	}

	config_add_opt(name, value);

	return 0;
}

/*
 * load config file
 * @return: 0 - success;
 *         -1 - failed
 */
int config_load(const char *filename)
{
	FILE *fp;
	char line[1024];

	if ((fp = fopen(filename, "r")) == NULL)
		return -1;

	while (fgets(line, sizeof(line), fp) != NULL) {
		/* ignore lines that start with a comment character */
		if (*line == comment)
			continue;

		if (parse_line(line) < 0) {
			fclose(fp);
			return -1;
		}
	}

	fclose(fp);
	return 0;
}

static int has_space(const char *str)
{
	int i;
	for (i = 0; str[i] != '\0'; i++) {
		if (isspace(str[i]))
			return 1;
	}
	return 0;
}

int config_save(const char *filename)
{
	FILE *fp;
	char line[1024];
	config_opt_t *opt, *tmp;

	if ((fp = fopen(filename, "w")) == NULL)
		return -1;

	HASH_ITER(hh, config_table, opt, tmp) {
		if (has_space(opt->value))
			sprintf(line, "%s %c \"%s\"\n", opt->name, delim, opt->value);
		else
			sprintf(line, "%s %c %s\n", opt->name, delim, opt->value);
		fputs(line, fp);
	}
	fclose(fp);
	return 0;
}

static void config_free_opt(config_opt_t *opt)
{
	free(opt->value);
	free(opt->name);
	free(opt);
}

void config_free(void)
{
	config_opt_t *opt, *tmp;
	HASH_ITER(hh, config_table, opt, tmp) {
		config_free_opt(opt);
	}
}
