/**
 * @file Libyang.cpp
 * @author Mislav Novakovic <mislav.novakovic@sartura.hr>
 * @brief Implementation of header Libyang.hpp
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
#include <vector>

#include "Xml.hpp"
#include "Internal.hpp"
#include "Libyang.hpp"
#include "Tree_Data.hpp"
#include "Tree_Schema.hpp"

extern "C" {
#include <libyang/libyang.h>
#include <libyang/tree_data.h>
#include <libyang/tree_schema.h>
}

using namespace std;

Context::Context(ly_ctx *ctx, S_Deleter deleter) {
	_ctx = ctx;
	_deleter = deleter;
}
Context::Context(const char *search_dir) {
	_ctx = ly_ctx_new(search_dir);
	if (NULL == _ctx) {
		throw runtime_error("can not create new context");
	}
	_deleter = S_Deleter(new Deleter(_ctx));
}
Context::Context(const char *search_dir, const char *path, LYD_FORMAT format) {
	_ctx = ly_ctx_new_ylpath(search_dir, path, format);
	if (NULL == _ctx) {
		throw runtime_error("can not create new context");
	}
	_deleter = S_Deleter(new Deleter(_ctx));
}
Context::Context(const char *search_dir, LYD_FORMAT format, const char *data) {
	_ctx = ly_ctx_new_ylmem(search_dir, data, format);
	if (NULL == _ctx) {
		throw runtime_error("can not create new context");
	}
	_deleter = S_Deleter(new Deleter(_ctx));
}
Context::~Context() {}
S_Data_Node Context::info() {
	struct lyd_node *node = ly_ctx_info(_ctx);
	return node ? S_Data_Node(new Data_Node(node, _deleter)) : NULL;
}
S_Module Context::get_module(const char *name, const char *revision) {
	const struct lys_module *module = ly_ctx_get_module(_ctx, name, revision);
	return module ? S_Module(new Module((lys_module *) module, _deleter)) : NULL;
}
S_Module Context::get_module_older(S_Module module) {
	const struct lys_module *new_module = ly_ctx_get_module_older(_ctx, module->_module);
	return new_module ? S_Module(new Module((lys_module *) new_module, _deleter)) : NULL;
}
S_Module Context::load_module(const char *name, const char *revision) {
	const struct lys_module *module = ly_ctx_load_module(_ctx, name, revision);
	return module ? S_Module(new Module((lys_module *) module, _deleter)) : NULL;
}
S_Module Context::get_module_by_ns(const char *ns, const char *revision) {
	const struct lys_module *module = ly_ctx_get_module_by_ns(_ctx, ns, revision);
	return module ? S_Module(new Module((lys_module *) module, _deleter)) : NULL;
}
vector<S_Module> *Context::get_module_iter() {
	const struct lys_module *mod = NULL;
	uint32_t i = 0;

	auto s_vector = new vector<S_Module>;
	if (NULL == s_vector) {
		return NULL;
	}

    while ((mod = ly_ctx_get_module_iter(_ctx, &i))) {
		if (mod == NULL) {
			break;
		}
		s_vector->push_back(S_Module(new Module((lys_module *) mod, _deleter)));
	}

	return s_vector;
}
vector<S_Module> *Context::get_disabled_module_iter() {
	const struct lys_module *mod = NULL;
	uint32_t i = 0;

	auto s_vector = new vector<S_Module>;
	if (NULL == s_vector) {
		return NULL;
	}

    while ((mod = ly_ctx_get_disabled_module_iter(_ctx, &i))) {
		if (mod == NULL) {
			break;
		}
		s_vector->push_back(S_Module(new Module((lys_module *) mod, _deleter)));
	}

	return s_vector;
}
void Context::clean() {
	return ly_ctx_clean(_ctx, NULL);
}
vector<string> *Context::get_searchdirs() {
	const char * const *data = ly_ctx_get_searchdirs(_ctx);
	if (NULL == data) {
		return NULL;
	}

	//std::shared_ptr<std::vector<string> > s_vector( new std::vector<string>);
	auto s_vector = new vector<string>;
	if (NULL == s_vector) {
		return NULL;
	}

	int size = 0;
	while (true) {
		if (data[size] == NULL) {
			break;
		}
		s_vector->push_back(std::string(data[size]));
		size++;
	}

	return s_vector;
};
S_Data_Node Context::parse_mem(const char *data, LYD_FORMAT format, int options) {
	struct lyd_node *node = NULL;

	node = lyd_parse_mem(_ctx, data, format, options);
	if (NULL == node) {
		return NULL;
	}

	S_Deleter deleter = S_Deleter(new Deleter(node, _deleter));
	return S_Data_Node(new Data_Node(node, deleter));
}
S_Data_Node Context::parse_fd(int fd, LYD_FORMAT format, int options) {
	struct lyd_node *node = NULL;

	node = lyd_parse_fd(_ctx, fd, format, options);
	if (NULL == node) {
		return NULL;
	}

	S_Deleter deleter = S_Deleter(new Deleter(node, _deleter));
	return S_Data_Node(new Data_Node(node, deleter));
}
S_Data_Node Context::parse_path(const char *path, LYD_FORMAT format, int options) {
	struct lyd_node *node = NULL;

	node = lyd_parse_path(_ctx, path, format, options);
	if (NULL == node) {
		return NULL;
	}

	S_Deleter deleter = S_Deleter(new Deleter(node, _deleter));
	return S_Data_Node(new Data_Node(node, deleter));
}
S_Data_Node Context::parse_xml(S_Xml_Elem elem, int options) {
	struct lyd_node *node = NULL;

	node = lyd_parse_xml(_ctx, &elem->_elem, options);
	if (NULL == node) {
		return NULL;
	}

	S_Deleter deleter = S_Deleter(new Deleter(node, _deleter));
	return S_Data_Node(new Data_Node(node, deleter));
}
