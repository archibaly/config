#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "hash.h"
#include "debug.h"

/* 2^31 + 2^29 - 2^25 + 2^22 - 2^19 - 2^16 + 1 */
#define GOLDEN_RATIO_PRIME_32 0x9e370001UL

static struct hash_head *hash_table;
static int hash_shift;
static int hash_key_type;

static inline uint32_t next_power_of_two(uint32_t x)
{
	return 1 << (32 - __builtin_clz(x - 1));
}

static uint32_t hash_int(uint32_t val, uint32_t bits)
{
	uint32_t hash = val * GOLDEN_RATIO_PRIME_32;

	/* high bits are more random, so use them */
	return hash >> (32 - bits);
}

static uint32_t hash_str(const char *val, uint32_t bits)
{
	uint32_t hash = 0;
	uint32_t seed = 131;	/* 31 131 1313 13131 131313 .. */

	while (*val)
		hash = hash * seed + (uint32_t)*val++;

	return hash % (1 << bits);
}

int hash_init(int size, int key_type)
{
	int i;

	size = next_power_of_two(size);
	hash_shift = log(size) / log(2);
	hash_key_type = key_type;

	if (!(hash_table = malloc(sizeof(struct hash_head) * size)))
		return -1;
	
	for (i = 0; i < size; i++)
		INIT_HLIST_HEAD(hash_table + i);

	return 0;
}

struct hash_head *hash_get_head(void)
{
	return hash_table;
}

static struct hash_node *new_hash_node(void *key, void *value)
{
	struct hash_node *node;

	if (!(node = malloc(sizeof(struct hash_node))))
		return NULL;

	node->key = key;
	node->value = value;
	hlist_node_init(&node->node);

	return node;
}

int hash_add(void *key, void *value)
{
	int offset;
	struct hash_node *node;

	if (hash_key_type == HASH_KEY_TYPE_INT)
		offset = hash_int(*(int *)key, hash_shift);
	else if (hash_key_type == HASH_KEY_TYPE_STR)
		offset = hash_str((char *)key, hash_shift);
	else
		return -1;

	if (!(node = new_hash_node(key, value)))
		return -1;

	debug("offset = %d", offset);
	hlist_add_head(&node->node, hash_table + offset);

	return 0;
}

static int hash_int_find(int key, struct hash_node **node, size_t size)
{
	int offset;
	size_t i = 0;
	struct hash_node *pos;

	if (!hash_table)
		return 0;

	offset = hash_int(key, hash_shift);

	hlist_for_each_entry(pos, hash_table + offset, node) {
		if (*(int *)pos->key == key) {
			if (i < size)
				node[i++] = pos;
			else
				break;
		}
	}

	return i;
}

static int hash_str_find(const char *key, struct hash_node **node, size_t size)
{
	int offset;
	size_t i = 0;
	struct hash_node *pos;

	if (!hash_table)
		return 0;

	offset = hash_str(key, hash_shift);

	hlist_for_each_entry(pos, hash_table + offset, node) {
		if (strcmp((char *)pos->key, key) == 0) {
			if (i < size)
				node[i++] = pos;
			else
				break;
		}
	}

	return i;
}

/*
 * @return: the number of found nodes
 */
int hash_find(const void *key, struct hash_node **node, size_t size)
{
	if (hash_key_type == HASH_KEY_TYPE_INT) {
		return hash_int_find(*(int *)key, node, size);
	} else if (hash_key_type == HASH_KEY_TYPE_STR) {
		return hash_str_find((char *)key, node, size);
	} else {
		return 0;
	}
}

void hash_free_node(struct hash_node *node)
{
	if (!node)
		return;
	if (!hlist_unhashed(&node->node)) {
		__hlist_del(&node->node);
	}
	free(node);
}

void hash_free(void)
{
	int i;
	int size;
	struct hash_node *pos;

	if (!hash_table)
		return;
	size = 1 << hash_shift;
	for (i = 0; i < size; i++) {
		hlist_for_each_entry(pos, hash_table + i, node) {
			hash_free_node(pos);
		}
	}
	free(hash_table);
	hash_shift = 0;
	hash_table = NULL;
}
