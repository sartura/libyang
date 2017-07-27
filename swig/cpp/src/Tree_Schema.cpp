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
std::vector<S_Deviation> *Module::deviation() {
	auto s_vector = new vector<S_Deviation>;
	if (NULL == s_vector) {
		return NULL;
	}

	for(uint8_t i = 0; i < _module->deviation_size; i++) {
		s_vector->push_back(S_Deviation(new Deviation(&_module->deviation[i], _deleter)));
	}

	return s_vector;
}
Submodule::Submodule(struct lys_submodule *submodule, S_Deleter deleter) {
	_submodule = submodule;
	_deleter = deleter;
};
Submodule::~Submodule() {};
S_Revision Submodule::rev() {return _submodule->rev ? S_Revision(new Revision(_submodule->rev, _deleter)) : NULL;};
std::vector<S_Deviation> *Submodule::deviation() {
	auto s_vector = new vector<S_Deviation>;
	if (NULL == s_vector) {
		return NULL;
	}

	for(uint8_t i = 0; i < _submodule->deviation_size; i++) {
		s_vector->push_back(S_Deviation(new Deviation(&_submodule->deviation[i], _deleter)));
	}

	return s_vector;
}

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
	_revision = revision;
	_deleter = deleter;
};
Revision::~Revision() {};

Schema_Node::Schema_Node(struct lys_node *node, S_Deleter deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node::~Schema_Node() {};
S_Module Schema_Node::module() {return _node->module ? S_Module(new Module(_node->module, _deleter)) : NULL;};
S_Schema_Node Schema_Node::parent() {return _node->parent ? S_Schema_Node(new Schema_Node(_node->parent, _deleter)) : NULL;};
S_Schema_Node Schema_Node::child() {return _node->child ? S_Schema_Node(new Schema_Node(_node->child, _deleter)) : NULL;};
S_Schema_Node Schema_Node::next() {return _node->next ? S_Schema_Node(new Schema_Node(_node->next, _deleter)) : NULL;};
S_Schema_Node Schema_Node::prev() {return _node->prev ? S_Schema_Node(new Schema_Node(_node->prev, _deleter)) : NULL;};
S_Set Schema_Node::find_xpath(const char *expr, int options) {
	struct ly_set *set = lys_find_xpath(_node, expr, options);
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

Schema_Node_Container::Schema_Node_Container(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_Container::~Schema_Node_Container() {};
S_When Schema_Node_Container::when() {
	lys_node_container *node = (struct lys_node_container *)_node;
	return node->when ? S_When(new When(node->when, _deleter)) : NULL;
};

Schema_Node_Choice::Schema_Node_Choice(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_Choice::~Schema_Node_Choice() {};
S_When Schema_Node_Choice::when() {
	lys_node_choice *node = (struct lys_node_choice *)_node;
	return node->when ? S_When(new When(node->when, _deleter)) : NULL;
};

Schema_Node_Leaf::Schema_Node_Leaf(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_Leaf::~Schema_Node_Leaf() {};
S_Set Schema_Node_Leaf::backlinks() {
	struct ly_set *set = ((struct lys_node_leaf *)_node)->backlinks;
	return set ? S_Set(new Set(set, _deleter)) : NULL;
}
S_When Schema_Node_Leaf::when() {
	lys_node_leaf *node = (struct lys_node_leaf *)_node;
	return node->when ? S_When(new When(node->when, _deleter)) : NULL;
};

Schema_Node_Leaflist::Schema_Node_Leaflist(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_Leaflist::~Schema_Node_Leaflist() {};
S_Set Schema_Node_Leaflist::backlinks() {
	struct ly_set *set = ((struct lys_node_leaflist *)_node)->backlinks;
	return set ? S_Set(new Set(set, _deleter)) : NULL;
}
S_When Schema_Node_Leaflist::when() {
	lys_node_leaflist *node = (struct lys_node_leaflist *)_node;
	return node->when ? S_When(new When(node->when, _deleter)) : NULL;
};

Schema_Node_List::Schema_Node_List(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_List::~Schema_Node_List() {};
S_When Schema_Node_List::when() {
	lys_node_list *node = (struct lys_node_list *)_node;
	return node->when ? S_When(new When(node->when, _deleter)) : NULL;
};

Schema_Node_Anydata::Schema_Node_Anydata(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_Anydata::~Schema_Node_Anydata() {};
S_When Schema_Node_Anydata::when() {
	lys_node_anydata *node = (struct lys_node_anydata *)_node;
	return node->when ? S_When(new When(node->when, _deleter)) : NULL;
};

Schema_Node_Uses::Schema_Node_Uses(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_Uses::~Schema_Node_Uses() {};
S_When Schema_Node_Uses::when() {
	lys_node_uses *node = (struct lys_node_uses *)_node;
	return node->when ? S_When(new When(node->when, _deleter)) : NULL;
};
std::vector<S_Refine> *Schema_Node_Uses::refine() {
	lys_node_uses *node = (struct lys_node_uses *)_node;
	auto s_vector = new vector<S_Refine>;

	if (NULL == s_vector) {
		return NULL;
	}

	for(uint8_t i = 0; i < node->refine_size; i++) {
		s_vector->push_back(S_Refine(new Refine(&node->refine[i], _deleter)));
	}

	return s_vector;
}
Schema_Node_Grp::Schema_Node_Grp(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_Grp::~Schema_Node_Grp() {};

Schema_Node_Case::Schema_Node_Case(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_Case::~Schema_Node_Case() {};
S_When Schema_Node_Case::when() {
	lys_node_case *node = (struct lys_node_case *)_node;
	return node->when ? S_When(new When(node->when, _deleter)) : NULL;
};

Schema_Node_Inout::Schema_Node_Inout(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_Inout::~Schema_Node_Inout() {};

Schema_Node_Notif::Schema_Node_Notif(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_Notif::~Schema_Node_Notif() {};

Schema_Node_Action::Schema_Node_Action(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_Action::~Schema_Node_Action() {};

Schema_Node_Augment::Schema_Node_Augment(struct lys_node *node, S_Deleter deleter) : Schema_Node(node, deleter) {
	_node = node;
	_deleter = deleter;
};
Schema_Node_Augment::~Schema_Node_Augment() {};
S_When Schema_Node_Augment::when() {
	lys_node_augment *node = (struct lys_node_augment *)_node;
	return node->when ? S_When(new When(node->when, _deleter)) : NULL;
};

When::When(struct lys_when *when, S_Deleter deleter) {
	_when = when;
	_deleter = deleter;
}
When::~When() {};
std::vector<S_Ext_Instance> *When::ext() {
	auto s_vector = new vector<S_Ext_Instance>;

	if (NULL == s_vector) {
		return NULL;
	}

	for(uint8_t i = 0; i < _when->ext_size; i++) {
		s_vector->push_back(S_Ext_Instance(new Ext_Instance(_when->ext[i], _deleter)));
	}

	return s_vector;
}

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
std::vector<S_Ext_Instance> *Ext::ext() {
	auto s_vector = new vector<S_Ext_Instance>;

	if (NULL == s_vector) {
		return NULL;
	}

	for(uint8_t i = 0; i < _ext->ext_size; i++) {
		s_vector->push_back(S_Ext_Instance(new Ext_Instance(_ext->ext[i], _deleter)));
	}

	return s_vector;
}
S_Module Ext::module() {return _ext->module ? S_Module(new Module(_ext->module, _deleter)) : NULL;};

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
std::vector<S_Ext_Instance> *Refine::ext() {
	auto s_vector = new vector<S_Ext_Instance>;

	if (NULL == s_vector) {
		return NULL;
	}

	for(uint8_t i = 0; i < _refine->ext_size; i++) {
		s_vector->push_back(S_Ext_Instance(new Ext_Instance(_refine->ext[i], _deleter)));
	}

	return s_vector;
}
S_Module Refine::module() {return _refine->module ? S_Module(new Module(_refine->module, _deleter)) : NULL;};
vector<string> *Refine::dflt() {
	auto s_vector = new vector<string>;
	if (NULL == s_vector) {
		return NULL;
	}

	for(uint8_t i = 0; i < _refine->dflt_size; i++) {
		s_vector->push_back(std::string(_refine->dflt[i]));
	}

	return s_vector;
}

Deviate::Deviate(struct lys_deviate *deviate, S_Deleter deleter) {
	_deviate = deviate;
	_deleter = _deleter;
}
Deviate::~Deviate() {};
vector<string> *Deviate::dflt() {
	auto s_vector = new vector<string>;
	if (NULL == s_vector) {
		return NULL;
	}

	for(uint8_t i = 0; i < _deviate->dflt_size; i++) {
		s_vector->push_back(std::string(_deviate->dflt[i]));
	}

	return s_vector;
}
std::vector<S_Ext_Instance> *Deviate::ext() {
	auto s_vector = new vector<S_Ext_Instance>;

	if (NULL == s_vector) {
		return NULL;
	}

	for(uint8_t i = 0; i < _deviate->ext_size; i++) {
		s_vector->push_back(S_Ext_Instance(new Ext_Instance(_deviate->ext[i], _deleter)));
	}

	return s_vector;
}

Deviation::Deviation(struct lys_deviation *deviation, S_Deleter deleter) {
	_deviation = deviation;
	_deleter = _deleter;
}
Deviation::~Deviation() {};
S_Schema_Node Deviation::orig_node() {return _deviation->orig_node ? S_Schema_Node(new Schema_Node(_deviation->orig_node, _deleter)) : NULL;};
std::vector<S_Deviate> *Deviation::deviate() {
	auto s_vector = new vector<S_Deviate>;
	if (NULL == s_vector) {
		return NULL;
	}

	for(uint8_t i = 0; i < _deviation->deviate_size; i++) {
		s_vector->push_back(S_Deviate(new Deviate(&_deviation->deviate[i], _deleter)));
	}

	return s_vector;
}
std::vector<S_Ext_Instance> *Deviation::ext() {
	auto s_vector = new vector<S_Ext_Instance>;

	if (NULL == s_vector) {
		return NULL;
	}

	for(uint8_t i = 0; i < _deviation->ext_size; i++) {
		s_vector->push_back(S_Ext_Instance(new Ext_Instance(_deviation->ext[i], _deleter)));
	}

	return s_vector;
}
