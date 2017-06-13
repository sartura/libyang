#ifndef TREE_DATA_H
#define TREE_DATA_H

#include <iostream>
#include <memory>
#include <exception>

#include "Internal.hpp"

extern "C" {
#include <libyang/libyang.h>
#include <libyang/tree_data.h>
}

using namespace std;

class Tree_Data
{
public:
	Tree_Data(struct lyd_node *node, S_Deleter deleter = NULL);
	~Tree_Data();
	uint8_t validity() {return _node->validity;};
	uint8_t dflt() {return _node->dflt;};
	uint8_t when_status() {return _node->when_status;};
	S_Tree_Data next() {_node->next ? S_Tree_Data(new Tree_Data(_node->next, _deleter)) : NULL;}
	S_Tree_Data prev() {_node->prev ? S_Tree_Data(new Tree_Data(_node->prev, _deleter)) : NULL;}
	S_Tree_Data parent() {_node->parent ? S_Tree_Data(new Tree_Data(_node->parent, _deleter)) : NULL;}
	S_Tree_Data child() {_node->child ? S_Tree_Data(new Tree_Data(_node->child, _deleter)) : NULL;}
private:
	struct lyd_node *_node;
	S_Deleter _deleter;
};

#endif
