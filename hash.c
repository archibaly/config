/* 
 * hash.c - Hashing.
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
 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hash.h"

static hash_node_t *new_hash_node(const char *, void *);
static unsigned int hash(const char *);

static unsigned int hash(const char *string)
{
    unsigned int hashval = 0;

    while (*string)
		hashval = ((hashval << 5) + hashval) + *string++;

    return hashval;
}

/* initialize the hash table. */
hash_table_t *new_hash_table(size_t size)
{
    hash_table_t *table;

	if ((table = malloc(sizeof(hash_table_t))) == NULL)
		exit(EXIT_FAILURE);

    if ((table->nodes = calloc(size, sizeof(hash_node_t *))) == NULL)
		exit(EXIT_FAILURE);
    table->size = size;
    table->count = 0;

    return table;
}

static hash_node_t *new_hash_node(const char *key, void *data)
{
    hash_node_t *node;

    if ((node = malloc(sizeof(hash_node_t))) == NULL)
		exit(EXIT_FAILURE);

    node->key = strdup(key);
    node->data = data;
    node->next = NULL;

    return node;
}

void resize_hash_table(const size_t size, hash_table_t ** table)
{
    hash_table_t *new_table, *old_table;
    hash_node_t *node, *next;
    size_t i;

    new_table = new_hash_table(size);
    old_table = *table;

    for (i = 0; i < old_table->size; i++)
		for (node = old_table->nodes[i]; node != NULL; node = next) {
			next = node->next;
			insert_hash_node(node->key, node->data, new_table);
			remove_hash_node(node->key, old_table);
		}

	free(old_table->nodes);
    free(old_table);

    *table = new_table;
}

hash_node_t *insert_hash_node(const char *key, void *data, hash_table_t * table)
{
    hash_node_t *node;
    unsigned int hashkey;

    hashkey = hash(key) % table->size;

    node = table->nodes[hashkey];

    while (node && node->next) {
		if (strcmp(node->key, key) == 0) {
			free(node->data);
			node->data = strdup(data);
			return node;
		}
		node = node->next;
    }

    node = new_hash_node(key, data);
    node->next = table->nodes[hashkey];

    table->count++;
    table->nodes[hashkey] = node;

    return node;
}

/* free a single node, node->data children must free
   free before calling this function. */
void free_hash_node(hash_node_t * node)
{
    free(node->key);
    free(node->data);
    free(node);
}

/* remove an item from the hashtable specified by 'key'. If no item
   is found, return 0. Otherwise return -1. */
int remove_hash_node(const char *key, hash_table_t * table)
{
    hash_node_t *node, *prev = NULL;
    unsigned int hashkey;

    hashkey = hash(key) % table->size;
    node = table->nodes[hashkey];

    while (node) {
		if (strcmp(node->key, key) != 0) {
			prev = node;
			node = node->next;
			continue;
		}

		if (prev)
			prev->next = node->next;
		else
			table->nodes[hashkey] = node->next;

		free_hash_node(node);
		return -1;
    }
    return 0;
}

/* free the entire hash table. */
void free_hash_table(hash_table_t * table)
{
    size_t i;
    hash_node_t *node, *prev;

    for (i = 0; i < table->size; i++) {
		node = table->nodes[i];
		while (node) {
			prev = node;
			node = node->next;
			free_hash_node(prev);
		}
    }

    free(table->nodes);
    free(table);
}

/* find a node in the hash table identified by KEY. */
hash_node_t *find_hash_node(const char *key, hash_table_t * table)
{
    unsigned int hashkey;
    hash_node_t *node;

    hashkey = hash(key) % table->size;

    node = table->nodes[hashkey];

    while (node) {
		if (strcmp(node->key, key) == 0)
			return node;
		else
			node = node->next;
    }
    return NULL;
}
