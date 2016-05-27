/* 
 * config.h ~ Header file for config_read.c.
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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "uthash.h"

/* example: */
/* name = "jacky liu" */
/* age = 25 */

typedef struct {
	/* name of the option */
	char *name;
	/* value of the option, NULL if this opt is an array */
	char *value;
	/* make this structure hashable */
	ut_hash_handle hh;
} config_opt_t;

int config_load(const char *);
int config_save(const char *);
void config_free(void);
void config_set_delim(char d);
char *config_get_value(const char *);
void config_set_value(const char *, const char *);
void config_print_opt(const char *);

#endif /* _CONFIG_H_ */
