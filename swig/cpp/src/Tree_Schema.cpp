/**
 * @file Tree_Schema.cpp
 * @author Mislav Novakovic <mislav.novakovic@sartura.hr>
 * @brief Implementation of header Tree_Schema.hpp.
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

#include "Internal.hpp"
#include "Libyang.hpp"
#include "Tree_Schema.hpp"

extern "C" {
#include <libyang/libyang.h>
#include <libyang/tree_schema.h>
}

using namespace std;

Module::Module(struct lys_module *module, S_Deleter deleter) {
	_module = module;
	_deleter = deleter;
};
Module::~Module() {};
S_Revision Module::rev() {return _module->rev ? S_Revision(new Revision(_module->rev, _deleter)) : NULL;};

Submodule::Submodule(struct lys_submodule *submodule, S_Deleter deleter) {
	_submodule = submodule;
	_deleter = deleter;
};
Submodule::~Submodule() {};
S_Revision Submodule::rev() {return _submodule->rev ? S_Revision(new Revision(_submodule->rev, _deleter)) : NULL;};

Ext_Instance::Ext_Instance(lys_ext_instance *ext_instance, S_Deleter deleter) {
	_ext_instance = ext_instance;
	_deleter = deleter;
};
Ext_Instance::~Ext_Instance() {};
std::vector<S_Ext_Instance> *Ext_Instance::ext() {
	auto s_vector = new vector<S_Ext_Instance>;

	if (NULL == s_vector) {
		return NULL;
	}

	for(uint8_t i = 0; i < _ext_instance->ext_size; i++) {
		s_vector->push_back(S_Ext_Instance(new Ext_Instance(_ext_instance->ext[i], _deleter)));
	}

	return s_vector;
}

Revision::Revision(lys_revision *revision, S_Deleter deleter) {
	_revisinon = revision;
	_deleter = deleter;
};
Revision::~Revision() {};

Schema_Node::Schema_Node(struct lys_node *node, S_Deleter deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node::~Schema_Node() {};
