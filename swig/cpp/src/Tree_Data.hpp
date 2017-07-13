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

/* defined */
class Data_Node;
class Data_Node_Leaf_List;
class Data_Node_Anydata;
class Attr;

/* used */
class Schema_Node;

class Data_Node
{
public:
	Data_Node(struct lyd_node *node, S_Deleter deleter = NULL);
	Data_Node(S_Data_Node parent, S_Module module, const char *name);
	Data_Node(S_Data_Node parent, S_Module module, const char *name, const char *val_str);
	Data_Node(S_Data_Node parent, S_Module module, const char *name, const char *value, LYD_ANYDATA_VALUETYPE value_type);
	Data_Node(S_Data_Node parent, S_Module module, const char *name, S_Data_Node value, LYD_ANYDATA_VALUETYPE value_type);
	//Data_Node(S_Data_Node parent, S_Module module, const char *name, lyxml_elem value, LYD_ANYDATA_VALUETYPE value_type);
	//struct lyd_node *lyd_new_output(struct lyd_node *parent, const struct lys_module *module, const char *name);
	~Data_Node();
	S_Schema_Node schema();
	uint8_t validity() {return _node->validity;};
	uint8_t dflt() {return _node->dflt;};
	uint8_t when_status() {return _node->when_status;};
	S_Attr attr();
	S_Data_Node next();
	S_Data_Node prev();
	S_Data_Node parent();
	S_Data_Node child();

	/* functions */
	S_String path();
	S_String qualified_path();
	S_Data_Node dup(int recursive);
	S_Data_Node dup_to_ctx(int recursive, S_Context context);
	int merge(S_Data_Node source, int options);
	int merge_to_ctx(S_Data_Node source, int options, S_Context context);
	int insert(S_Data_Node node);
	int insert_sibling(S_Data_Node node);
	int insert_before(S_Data_Node node);
	int insert_after(S_Data_Node node);
	int schema_sort(int recursive);
	//struct ly_set *lyd_find_xpath(const struct lyd_node *ctx_node, const char *expr);
	//struct ly_set *lyd_find_instance(const struct lyd_node *data, const struct lys_node *schema);
	S_Data_Node first_sibling();
	int validate(int options, S_Context var_arg);
	int validate(int options, S_Data_Node var_arg);
	//int lyd_wd_default(struct lyd_node_leaf_list *node);

	/* emulate TREE macro's */
	std::vector<S_Data_Node> *tree_for();
	std::vector<S_Data_Node> *tree_dfs();

private:
	struct lyd_node *_node;
	S_Deleter _deleter;
};

class Data_Node_Leaf_List : public Data_Node
{
public:
	Data_Node_Leaf_List(struct lyd_node *node, S_Deleter deleter = NULL);
	~Data_Node_Leaf_List();
    const char *value_str() {return ((struct lyd_node_leaf_list *) _node)->value_str;};
    //lyd_val value;                   /**< node's value representation, always corresponds to schema->type.base */
    uint16_t value_type() {return ((struct lyd_node_leaf_list *) _node)->value_type;};

	/* functions */
	int change_leaf(const char *val_str);
	int wd_default();
	//const struct lys_type *lyd_leaf_type(const struct lyd_node_leaf_list *leaf);

private:
	struct lyd_node *_node;
	S_Deleter _deleter;
};

class Data_Node_Anydata : public Data_Node
{
public:
	Data_Node_Anydata(struct lyd_node *node, S_Deleter deleter = NULL);
	~Data_Node_Anydata();
    LYD_ANYDATA_VALUETYPE value_type() {return ((struct lyd_node_anydata *) _node)->value_type;};
	//union value

private:
	struct lyd_node *_node;
	S_Deleter _deleter;
};

class Attr
{
public:
	Attr(struct lyd_attr *attr, S_Deleter deleter = NULL);
	~Attr();
	S_Data_Node parent();
	S_Attr next();
	//struct lys_ext_instance_complex *annotation
	const char *name() {return _attr->name;};
	const char *value_str() {return _attr->value_str;};
	//lyd_val value;
	uint16_t value_type() {return _attr->value_type;};
private:
	struct lyd_attr *_attr;
	S_Deleter _deleter;
};

#endif
