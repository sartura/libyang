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

Attr::Attr(struct lyd_attr *attr, S_Deleter deleter) {
	_attr = attr;
	_deleter = deleter;
};
Attr::~Attr() {};
S_Data_Node Attr::parent() {return _attr->parent ? S_Data_Node(new Data_Node(_attr->parent, _deleter)) : NULL;};
S_Attr Attr::next() {return _attr->next ? S_Attr(new Attr(_attr->next, _deleter)) : NULL;};
