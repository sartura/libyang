/**
 * @file Xml.cpp
 * @author Mislav Novakovic <mislav.novakovic@sartura.hr>
 * @brief Implementation of header Xml.hpp
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

#include "Internal.hpp"
#include "Xml.hpp"

extern "C" {
#include <libyang/libyang.h>
#include <libyang/xml.h>
}

using namespace std;

Xml_Ns::Xml_Ns(struct lyxml_ns *ns, S_Deleter deleter) {
	_ns = ns;
	_deleter = deleter;
}
Xml_Ns::~Xml_Ns() {}
S_Xml_Ns Xml_Ns::next() {return _ns->next ? S_Xml_Ns(new Xml_Ns(_ns->next, _deleter)) : NULL;}

Xml_Attr::Xml_Attr(struct lyxml_attr *attr, S_Deleter deleter) {
	_attr = attr;
	_deleter = deleter;
}
Xml_Attr::~Xml_Attr() {}
S_Xml_Attr Xml_Attr::next() {return _attr->next ? S_Xml_Attr(new Xml_Attr(_attr->next, _deleter)) : NULL;}
S_Xml_Ns Xml_Attr::ns() {return _attr->ns ? S_Xml_Ns(new Xml_Ns((struct lyxml_ns *)_attr->ns, _deleter)) : NULL;}

Xml_Elem::Xml_Elem(S_Context context, struct lyxml_elem *elem, S_Deleter deleter) {
	_context = context;
	_elem = elem;
	_deleter = deleter;
}
Xml_Elem::~Xml_Elem() {}
S_Xml_Elem Xml_Elem::parent() {return _elem->parent ? S_Xml_Elem(new Xml_Elem(_context, _elem->parent, _deleter)) : NULL;}
S_Xml_Attr Xml_Elem::attr() {return _elem->attr ? S_Xml_Attr(new Xml_Attr(_elem->attr, _deleter)) : NULL;}
S_Xml_Elem Xml_Elem::child() {return _elem->child ? S_Xml_Elem(new Xml_Elem(_context, _elem->child, _deleter)) : NULL;}
S_Xml_Elem Xml_Elem::next() {return _elem->next ? S_Xml_Elem(new Xml_Elem(_context, _elem->next, _deleter)) : NULL;}
S_Xml_Elem Xml_Elem::prev() {return _elem->prev ? S_Xml_Elem(new Xml_Elem(_context, _elem->prev, _deleter)) : NULL;}
S_Xml_Ns Xml_Elem::ns() {return _elem->ns ? S_Xml_Ns(new Xml_Ns((struct lyxml_ns *)_elem->ns, _deleter)) : NULL;}
const char *Xml_Elem::get_attr(const char *name, const char *ns) {
	return lyxml_get_attr(_elem, name, ns);
}
S_Xml_Ns Xml_Elem::get_ns(const char *prefix) {
	const struct lyxml_ns *ns = lyxml_get_ns(_elem, prefix);
	return _elem->ns ? S_Xml_Ns(new Xml_Ns((struct lyxml_ns *)ns, _deleter)) : NULL;
}
S_String Xml_Elem::print_mem(int options) {
	char *data = NULL;

	lyxml_print_mem(&data, (const struct lyxml_elem *) _elem, options);
	if (NULL == data) {
		return NULL;
	}

    S_String s_data = data;
    free(data);
    return s_data;
}

std::vector<S_Xml_Elem> *Xml_Elem::tree_for() {
	auto s_vector = new vector<S_Xml_Elem>;

	if (NULL == s_vector) {
		return NULL;
	}

	struct lyxml_elem *elem = NULL;
	LY_TREE_FOR(_elem, elem) {
		s_vector->push_back(S_Xml_Elem(new Xml_Elem(_context, elem, _deleter)));
	}

	return s_vector;
}
std::vector<S_Xml_Elem> *Xml_Elem::tree_dfs() {
	auto s_vector = new vector<S_Xml_Elem>;

	if (NULL == s_vector) {
		return NULL;
	}

	struct lyxml_elem *elem = NULL, *next = NULL;
	LY_TREE_DFS_BEGIN(_elem, next, elem) {
		s_vector->push_back(S_Xml_Elem(new Xml_Elem(_context, elem, _deleter)));
		LY_TREE_DFS_END(_elem, next, elem)
	}

		return s_vector;
			}
