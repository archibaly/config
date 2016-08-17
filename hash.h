#ifndef _HASH_H_
#define _HASH_H_

#include "list.h"

#define HASH_KEY_TYPE_INT	1
#define HASH_KEY_TYPE_STR	2

#define hash_for_each_entry(pos, table) hlist_for_each_entry(pos, table, node)
#define hash_head hlist_head

struct hash_node {
	void *key;
	void *value;
	struct hlist_node node;
};

struct hash_head *hash_get_head(void);
int hash_init(int size, int key_type);
int hash_add(void *key, void *value);
int hash_find(const void *key, struct hash_node **node, size_t size);
void hash_free_node(struct hash_node *node);
void hash_free(void);

#endif /* _HASH_H_ */
