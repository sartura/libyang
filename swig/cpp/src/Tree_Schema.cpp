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
S_Revision Module::rev() NEW(_module, rev, Revision);
std::vector<S_Deviation> *Module::deviation() NEW_LIST(_module, deviation, deviation_size, Deviation);
Submodule::Submodule(struct lys_submodule *submodule, S_Deleter deleter) {
	_submodule = submodule;
	_deleter = deleter;
};
Submodule::~Submodule() {};
S_Revision Submodule::rev() NEW(_submodule, rev, Revision);
std::vector<S_Deviation> *Submodule::deviation() NEW_LIST(_submodule, deviation, deviation_size, Deviation);

Ext_Instance::Ext_Instance(lys_ext_instance *ext_instance, S_Deleter deleter) {
	_ext_instance = ext_instance;
	_deleter = deleter;
};
Ext_Instance::~Ext_Instance() {};
std::vector<S_Ext_Instance> *Ext_Instance::ext() NEW_P_LIST(_ext_instance, ext, ext_size, Ext_Instance);

Revision::Revision(lys_revision *revision, S_Deleter deleter) {
	_revision = revision;
	_deleter = deleter;
};
Revision::~Revision() {};

Schema_Node::Schema_Node(struct lys_node *node, S_Deleter deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node::~Schema_Node() {};
S_Module Schema_Node::module() NEW(_node, module, Module);
S_Schema_Node Schema_Node::parent() NEW(_node, parent, Schema_Node);
S_Schema_Node Schema_Node::child() NEW(_node, child, Schema_Node);
S_Schema_Node Schema_Node::next() NEW(_node, next, Schema_Node);
S_Schema_Node Schema_Node::prev() NEW(_node, prev, Schema_Node); 
S_Set Schema_Node::find_xpath(const char *path) {
	struct ly_set *set = lys_find_path(_node->module, _node, path);
	if (NULL == set) {
		return NULL;
	}

	return S_Set(new Set(set, _deleter));
}
S_Set Schema_Node::xpath_atomize(int options) {
	struct ly_set *set = lys_node_xpath_atomize(_node, options);
	if (NULL == set) {
		return NULL;
	}

	return S_Set(new Set(set, _deleter));
}
std::vector<S_Schema_Node> *Schema_Node::tree_for() {
	auto s_vector = new vector<S_Schema_Node>;

	if (NULL == s_vector) {
		return NULL;
	}

	struct lys_node *elem = NULL;
	LY_TREE_FOR(_node, elem) {
		s_vector->push_back(S_Schema_Node(new Schema_Node(elem, _deleter)));
	}

	return s_vector;
}
std::vector<S_Schema_Node> *Schema_Node::tree_dfs() {
	auto s_vector = new vector<S_Schema_Node>;

	if (NULL == s_vector) {
		return NULL;
	}

	struct lys_node *elem = NULL, *next = NULL;
	LY_TREE_DFS_BEGIN(_node, next, elem) {
		s_vector->push_back(S_Schema_Node(new Schema_Node(elem, _deleter)));
		LY_TREE_DFS_END(_node, next, elem)
	}

	return s_vector;
}

Schema_Node_Container::~Schema_Node_Container() {};
S_When Schema_Node_Container::when() NEW_CASTED(lys_node_container, _node, when, When);

Schema_Node_Choice::~Schema_Node_Choice() {};
S_When Schema_Node_Choice::when() NEW_CASTED(lys_node_choice, _node, when, When);

Schema_Node_Leaf::~Schema_Node_Leaf() {};
S_Set Schema_Node_Leaf::backlinks() NEW_CASTED(lys_node_leaf, _node, backlinks, Set);
S_When Schema_Node_Leaf::when() NEW_CASTED(lys_node_leaf, _node, when, When);

Schema_Node_Leaflist::~Schema_Node_Leaflist() {};
S_Set Schema_Node_Leaflist::backlinks() NEW_CASTED(lys_node_leaflist, _node, backlinks, Set);
S_When Schema_Node_Leaflist::when() NEW_CASTED(lys_node_leaflist, _node, when, When);
std::vector<S_String> *Schema_Node_Leaflist::dflt() {
	struct lys_node_leaflist *node = (struct lys_node_leaflist *)_node;
	NEW_STRING_LIST(node, dflt, dflt_size);
}
std::vector<S_Restr> *Schema_Node_Leaflist::must() NEW_LIST_CASTED(lys_node_leaflist, _node, must, must_size, Restr);

Schema_Node_List::~Schema_Node_List() {};
S_When Schema_Node_List::when() NEW_CASTED(lys_node_list, _node, when, When);
std::vector<S_Restr> *Schema_Node_List::must() NEW_LIST_CASTED(lys_node_list, _node, must, must_size, Restr);
std::vector<S_Tpdf> *Schema_Node_List::tpdf() NEW_LIST_CASTED(lys_node_list, _node, tpdf, tpdf_size, Tpdf);
//std::vector<Schema_Node_Leaf> *Schema_Node_List::keys() NEW_P_LIST_CASTED(lys_node_list, _node, keys, keys_size, Schema_Node_Leaf);
std::vector<S_Unique> *Schema_Node_List::unique() NEW_LIST_CASTED(lys_node_list, _node, unique, unique_size, Unique);

Schema_Node_Anydata::~Schema_Node_Anydata() {};
S_When Schema_Node_Anydata::when() NEW_CASTED(lys_node_anydata, _node, when, When);
std::vector<S_Restr> *Schema_Node_Anydata::must() NEW_LIST_CASTED(lys_node_anydata, _node, must, must_size, Restr);

Schema_Node_Uses::~Schema_Node_Uses() {};
S_When Schema_Node_Uses::when() NEW_CASTED(lys_node_uses, _node, when, When);
std::vector<S_Refine> *Schema_Node_Uses::refine() NEW_LIST_CASTED(lys_node_uses, _node, refine, refine_size, Refine);

Schema_Node_Grp::~Schema_Node_Grp() {};
std::vector<S_Tpdf> *Schema_Node_Grp::tpdf() NEW_LIST_CASTED(lys_node_grp, _node, tpdf, tpdf_size, Tpdf);

Schema_Node_Case::~Schema_Node_Case() {};
S_When Schema_Node_Case::when() NEW_CASTED(lys_node_case, _node, when, When);

Schema_Node_Inout::~Schema_Node_Inout() {};
std::vector<S_Tpdf> *Schema_Node_Inout::tpdf() NEW_LIST_CASTED(lys_node_inout, _node, tpdf, tpdf_size, Tpdf);
std::vector<S_Restr> *Schema_Node_Inout::must() NEW_LIST_CASTED(lys_node_inout, _node, must, must_size, Restr);

Schema_Node_Notif::~Schema_Node_Notif() {};
std::vector<S_Tpdf> *Schema_Node_Notif::tpdf() NEW_LIST_CASTED(lys_node_notif, _node, tpdf, tpdf_size, Tpdf);
std::vector<S_Restr> *Schema_Node_Notif::must() NEW_LIST_CASTED(lys_node_notif, _node, must, must_size, Restr);

Schema_Node_Rpc_Action::~Schema_Node_Rpc_Action() {};
std::vector<S_Tpdf> *Schema_Node_Rpc_Action::tpdf() NEW_LIST_CASTED(lys_node_rpc_action, _node, tpdf, tpdf_size, Tpdf);

Schema_Node_Augment::~Schema_Node_Augment() {};
S_When Schema_Node_Augment::when() NEW_CASTED(lys_node_augment, _node, when, When);

When::When(struct lys_when *when, S_Deleter deleter) {
	_when = when;
	_deleter = deleter;
}
When::~When() {};
std::vector<S_Ext_Instance> *When::ext() NEW_P_LIST(_when, ext, ext_size, Ext_Instance);

Substmt::Substmt(struct lyext_substmt *substmt, S_Deleter deleter) {
	_substmt = substmt;
	_deleter = deleter;
}
Substmt::~Substmt() {};

Ext::Ext(struct lys_ext *ext, S_Deleter deleter) {
	_ext = ext;
	_deleter = deleter;
}
Ext::~Ext() {};
std::vector<S_Ext_Instance> *Ext::ext() NEW_P_LIST(_ext, ext, ext_size, Ext_Instance);
S_Module Ext::module() NEW(_ext, module, Module);

Refine_Mod_List::Refine_Mod_List(struct lys_refine_mod_list *list, S_Deleter deleter) {
	_list = list;
	_deleter = _deleter;
}
Refine_Mod_List::~Refine_Mod_List() {};

Refine::Refine(struct lys_refine *refine, S_Deleter deleter) {
	_refine = refine;
	_deleter = _deleter;
}
Refine::~Refine() {};
std::vector<S_Ext_Instance> *Refine::ext() NEW_P_LIST(_refine, ext, ext_size, Ext_Instance);
S_Module Refine::module() NEW(_refine, module, Module);

Deviate::Deviate(struct lys_deviate *deviate, S_Deleter deleter) {
	_deviate = deviate;
	_deleter = _deleter;
}
Deviate::~Deviate() {};
std::vector<S_Ext_Instance> *Deviate::ext() NEW_P_LIST(_deviate, ext, ext_size, Ext_Instance);

Deviation::Deviation(struct lys_deviation *deviation, S_Deleter deleter) {
	_deviation = deviation;
	_deleter = _deleter;
}
Deviation::~Deviation() {};
S_Schema_Node Deviation::orig_node() NEW(_deviation, orig_node, Schema_Node);
std::vector<S_Deviate> *Deviation::deviate() NEW_LIST(_deviation, deviate, deviate_size, Deviate);
std::vector<S_Ext_Instance> *Deviation::ext() NEW_P_LIST(_deviation, ext, ext_size, Ext_Instance);

Import::Import(struct lys_import *import, S_Deleter deleter) {
	_import = import;
	_deleter = deleter;
}
Import::~Import() {};

Include::Include(struct lys_include *include, S_Deleter deleter) {
	_include = include;
	_deleter = deleter;
}
Include::~Include() {};

Tpdf::Tpdf(struct lys_tpdf *tpdf, S_Deleter deleter) {
	_tpdf = tpdf;
	_deleter = deleter;
}
Tpdf::~Tpdf() {};

Unique::Unique(struct lys_unique *unique, S_Deleter deleter) {
	_unique = unique;
	_deleter = deleter;
}
Unique::~Unique() {};

Feature::Feature(struct lys_feature *feature, S_Deleter deleter) {
	_feature = feature;
	_deleter = deleter;
}
Feature::~Feature() {};

Restr::Restr(struct lys_restr *restr, S_Deleter deleter) {
	_restr = restr;
	_deleter = deleter;
}
Restr::~Restr() {};

Ident::Ident(struct lys_ident *ident, S_Deleter deleter) {
	_ident = ident;
	_deleter = deleter;
}
Ident::~Ident() {};
std::vector<S_Ident> *Ident::base() NEW_P_LIST(_ident, base, base_size, Ident);
