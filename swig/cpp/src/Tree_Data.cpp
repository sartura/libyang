/**
 * @file Tree_Data.cpp
 * @author Mislav Novakovic <mislav.novakovic@sartura.hr>
 * @brief Implementation of header Tree_Data.hpp.
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

#include <iostream>
#include <memory>
#include <stdexcept>

#include "Libyang.hpp"
#include "Tree_Data.hpp"
#include "Tree_Schema.hpp"

extern "C" {
#include <libyang/libyang.h>
#include <libyang/tree_data.h>
}

using namespace std;

Data_Node::Data_Node(struct lyd_node *node, S_Deleter deleter) {
	_node = node;
	_deleter = deleter;
};
Data_Node::~Data_Node() {};
S_Schema_Node Data_Node::schema() {return _node->schema ? S_Schema_Node(new Schema_Node(_node->schema, _deleter)) : NULL;};
S_Attr Data_Node::attr() {return _node->attr ? S_Attr(new Attr(_node->attr, _deleter)) : NULL;}
S_Data_Node Data_Node::next() {return _node->next ? S_Data_Node(new Data_Node(_node->next, _deleter)) : NULL;}
S_Data_Node Data_Node::prev() {return _node->prev ? S_Data_Node(new Data_Node(_node->prev, _deleter)) : NULL;}
S_Data_Node Data_Node::parent() {return _node->parent ? S_Data_Node(new Data_Node(_node->parent, _deleter)) : NULL;}
S_Data_Node Data_Node::child() {return _node->child ? S_Data_Node(new Data_Node(_node->child, _deleter)) : NULL;}
S_String Data_Node::path() {
	char *path = NULL;

	path = lyd_path(_node);
	if (NULL == path) {
		return NULL;
	}

    S_String s_path = path;
    free(path);
    return s_path;
}
S_String Data_Node::qualified_path() {
	char *qualified_path = NULL;

	qualified_path = lyd_qualified_path(_node);
	if (NULL == qualified_path) {
		return NULL;
	}

    S_String s_qualified_path = qualified_path;
    free(qualified_path);
    return s_qualified_path;
}

Attr::Attr(struct lyd_attr *attr, S_Deleter deleter) {
	_attr = attr;
	_deleter = deleter;
};
Attr::~Attr() {};
S_Data_Node Attr::parent() {return _attr->parent ? S_Data_Node(new Data_Node(_attr->parent, _deleter)) : NULL;};
S_Attr Attr::next() {return _attr->next ? S_Attr(new Attr(_attr->next, _deleter)) : NULL;};
