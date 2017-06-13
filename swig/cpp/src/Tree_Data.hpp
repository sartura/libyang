/**
 * @file Tree_Data.cpp
 * @author Mislav Novakovic <mislav.novakovic@sartura.hr>
 * @brief Class implementation for libyang C header tree_data.h.
 *
 * @copyright
 * Copyright 2017 Deutsche Telekom AG.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

class Data_Node
{
public:
	Data_Node(struct lyd_node *node, S_Deleter deleter = NULL);
	~Data_Node();
	uint8_t validity() {return _node->validity;};
	uint8_t dflt() {return _node->dflt;};
	uint8_t when_status() {return _node->when_status;};
	S_Data_Node next() {_node->next ? S_Data_Node(new Data_Node(_node->next, _deleter)) : NULL;}
	S_Data_Node prev() {_node->prev ? S_Data_Node(new Data_Node(_node->prev, _deleter)) : NULL;}
	S_Data_Node parent() {_node->parent ? S_Data_Node(new Data_Node(_node->parent, _deleter)) : NULL;}
	S_Data_Node child() {_node->child ? S_Data_Node(new Data_Node(_node->child, _deleter)) : NULL;}
private:
	struct lyd_node *_node;
	S_Deleter _deleter;
};

#endif
