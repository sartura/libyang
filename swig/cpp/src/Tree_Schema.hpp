/**
 * @file Tree_Schema.cpp
 * @author Mislav Novakovic <mislav.novakovic@sartura.hr>
 * @brief Class implementation for libyang C header tree_schema.h
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

#ifndef TREE_SCHEMA_H
#define TREE_SCHEMA_H

#include <iostream>
#include <memory>
#include <exception>

#include "Internal.hpp"
#include "Libyang.hpp"

extern "C" {
#include <libyang/libyang.h>
#include <libyang/tree_schema.h>
}

using namespace std;

class Module
{
public:
	Module(struct lys_module *module, S_Deleter deleter);
	~Module();
	const char *name() {return _module->name;};
	const char *prefix() {return _module->prefix;};
	const char *dsc() {return _module->dsc;};
	const char *ref() {return _module->ref;};
	const char *org() {return _module->org;};
	const char *contact() {return _module->contact;};
	const char *filepath() {return _module->filepath;};
	uint8_t type() {return _module->type;};
	uint8_t version() {return _module->version;};
	uint8_t deviated() {return _module->deviated;};
	uint8_t disabled() {return _module->disabled;};
	uint8_t rev_size() {return _module->rev_size;};
	uint8_t imp_size() {return _module->imp_size;};
	uint8_t inc_size() {return _module->inc_size;};
	uint8_t ident_size() {return _module->ident_size;};
	uint8_t tpdf_size() {return _module->tpdf_size;};
	uint8_t features_size() {return _module->features_size;};
	uint8_t augment_size() {return _module->augment_size;};
	uint8_t devaiation_size() {return _module->deviation_size;};
	uint8_t extensions_size() {return _module->extensions_size;};
	uint8_t ext_size() {return _module->ext_size;};
	const char *ns() {return _module->ns;};

	friend class Context;

private:
	struct lys_module *_module;
	S_Deleter _deleter;
};
/*
class Schema_Node
{
public:
	Schema_Node(lys_node *node, S_Deleter deleter);
	~Schema_Node();
	const char *name() {return _node->name;};
private:
	struct lys_node *_node;
	S_Deleter _deleter;
};
*/
#endif
