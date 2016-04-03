/* 
 * hash.h - Header file for hash.c.
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

#ifndef _HASH_H_
#define _HASH_H_

#include <string.h>

typedef struct hash_node {
    /* key to find a node. */
    char *key;
    /* the data we need. */
    void *data;
    /* next hash in the table. */
    struct hash_node *next;
} hash_node_t;

typedef struct {
    /* number of allocated nodes. */
    size_t size;
    /* number of nodes in the hash table. */
    size_t count;
    /* the hash nodes in the hash table. */
    hash_node_t **nodes;
} hash_table_t;

hash_table_t *new_hash_table(size_t);
hash_node_t *insert_hash_node(const char *, void *, hash_table_t *);
void free_hash_node(hash_node_t *);
int remove_hash_node(const char *, hash_table_t *);
void free_hash_table(hash_table_t *);
hash_node_t *find_hash_node(const char *, hash_table_t *);
void resize_hash_table(const size_t, hash_table_t **);

#endif /* _HASH_H_ */
