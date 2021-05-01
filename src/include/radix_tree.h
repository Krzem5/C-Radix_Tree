#ifndef __RADIX_TREE_H__
#define __RADIX_TREE_H__ 1
#include <stdint.h>



typedef struct __RADIX_TREE_NODE{
	char* v;
	uint32_t vl;
	uint32_t id;
	struct __RADIX_TREE_NODE* nl;
	uint32_t nll;
} radix_tree_node_t;



typedef struct __RADIX_TREE{
	char* v;
	uint32_t vl;
	uint32_t id;
	radix_tree_node_t* nl;
	uint32_t nll;
	uint32_t _nid;
} radix_tree_t;



void radix_tree_init(radix_tree_t* t);



void radix_tree_insert(radix_tree_t* t,char* v);



uint8_t radix_tree_delete(radix_tree_t* t,char* v);



void radix_tree_print(radix_tree_t* t);



#endif
