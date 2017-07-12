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
Data_Node::Data_Node(S_Data_Node parent, S_Module module, const char *name) {
	lyd_node *node = NULL;

	if (NULL == module) {
		throw std::invalid_argument("Module can not be empty");
	}

	node = lyd_new(parent->_node, module->_module, name);
	if (NULL == node) {
		throw std::invalid_argument("libyang could not create new data node, invalid argument");
	}

	_node = node;
	_deleter = NULL;
};
Data_Node::Data_Node(S_Data_Node parent, S_Module module, const char *name, const char *val_str) {
	lyd_node *node = NULL;

	if (NULL == module) {
		throw std::invalid_argument("Module can not be empty");
	}

	node = lyd_new_leaf(parent->_node, module->_module, name, val_str);
	if (NULL == node) {
		throw std::invalid_argument("libyang could not create new data node, invalid argument");
	}

	_node = node;
	_deleter = NULL;
};
Data_Node::Data_Node(S_Data_Node parent, S_Module module, const char *name, const char *value, LYD_ANYDATA_VALUETYPE value_type) {
	lyd_node *node = NULL;

	if (NULL == module) {
		throw std::invalid_argument("Module can not be empty");
	}

	node = lyd_new_anydata(parent->_node, module->_module, name, (void *) value, value_type);
	if (NULL == node) {
		throw std::invalid_argument("libyang could not create new data node, invalid argument");
	}

	_node = node;
	_deleter = NULL;
};
Data_Node::Data_Node(S_Data_Node parent, S_Module module, const char *name, S_Data_Node value, LYD_ANYDATA_VALUETYPE value_type) {
	lyd_node *node = NULL;

	if (NULL == module) {
		throw std::invalid_argument("Module can not be empty");
	}

	node = lyd_new_anydata(parent->_node, module->_module, name, (void *) value->_node, value_type);
	if (NULL == node) {
		throw std::invalid_argument("libyang could not create new data node, invalid argument");
	}

	_node = node;
	_deleter = NULL;
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
S_Data_Node Data_Node::dup(int recursive) {
	struct lyd_node *node = NULL;

	node = lyd_dup(_node, recursive);

	return node ? S_Data_Node(new Data_Node(node, _deleter)) : NULL;
}
S_Data_Node Data_Node::dup_to_ctx(int recursive, S_Context context) {
	struct lyd_node *node = NULL;

	node = lyd_dup_to_ctx(_node, recursive, context->_ctx);

	return node ? S_Data_Node(new Data_Node(node, _deleter)) : NULL;
}
int Data_Node::merge(S_Data_Node source, int options) {
	return lyd_merge(_node, source->_node, options);
}
int Data_Node::merge_to_ctx(S_Data_Node source, int options, S_Context context) {
	return lyd_merge_to_ctx(&_node, source->_node, options, context->_ctx);
}
int Data_Node::insert(S_Data_Node node) {
	return lyd_insert(_node, node->_node);
}
int Data_Node::insert_sibling(S_Data_Node node) {
	return lyd_insert_sibling(&_node, node->_node);
}
int Data_Node::insert_before(S_Data_Node node) {
	return lyd_insert_before(_node, node->_node);
}
int Data_Node::insert_after(S_Data_Node node) {
	return lyd_insert_after(_node, node->_node);
}
int Data_Node::schema_sort(int recursive) {
	return lyd_schema_sort(_node, recursive);
}
S_Data_Node Data_Node::first_sibling() {
	struct lyd_node *node = NULL;

	node = lyd_first_sibling(_node);

	return node ? S_Data_Node(new Data_Node(node, _deleter)) : NULL;
}
int Data_Node::validate(int options, S_Context var_arg) {
	return lyd_validate(&_node, options, (void *) var_arg->_ctx);
}
int Data_Node::validate(int options, S_Data_Node var_arg) {
	return lyd_validate(&_node, options, (void *) var_arg->_node);
}

Data_Node_Leaf_List::Data_Node_Leaf_List(struct lyd_node *node, S_Deleter deleter) : Data_Node(node, deleter) {};
Data_Node_Leaf_List::~Data_Node_Leaf_List() {};

Data_Node_Anydata::Data_Node_Anydata(struct lyd_node *node, S_Deleter deleter) : Data_Node(node, deleter) {};
Data_Node_Anydata::~Data_Node_Anydata() {};

Attr::Attr(struct lyd_attr *attr, S_Deleter deleter) {
	_attr = attr;
	_deleter = deleter;
};
Attr::~Attr() {};
S_Data_Node Attr::parent() {return _attr->parent ? S_Data_Node(new Data_Node(_attr->parent, _deleter)) : NULL;};
S_Attr Attr::next() {return _attr->next ? S_Attr(new Attr(_attr->next, _deleter)) : NULL;};
