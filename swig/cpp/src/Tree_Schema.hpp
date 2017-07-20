/**
 * @file Tree_Schema.hpp
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

/* defined */
class Module;
class Subodule;
class Ext_Instance;
class Revision;
class Schema_Node;
class Schema_Node_Container;
class Schema_Node_Choice;
class Schema_Node_Leaf;
class Schema_Node_Leaflist;
class Schema_Node_List;
class Schema_Node_Anydata;
class Schema_Node_Uses;
class Schema_Node_Grp;
class Schema_Node_Case;
class Schema_Node_Inout;
class Schema_Node_Notif;
class Schema_Node_Action;
class Schema_Node_Augment;
class When;
class Substmt;
class Ext;

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
	S_Revision rev();

	friend class Context;
	friend class Data_Node;

private:
	struct lys_module *_module;
	S_Deleter _deleter;
};

class Submodule
{
public:
	Submodule(struct lys_submodule *submodule, S_Deleter deleter);
	~Submodule();
	S_Context ctx() {return _submodule->ctx ? S_Context(new Context(_submodule->ctx, _deleter)) : NULL;};
	const char *name() {return _submodule->name;};
	const char *prefix() {return _submodule->prefix;};
	const char *dsc() {return _submodule->dsc;};
	const char *ref() {return _submodule->ref;};
	const char *org() {return _submodule->org;};
	const char *contact() {return _submodule->contact;};
	const char *filepath() {return _submodule->filepath;};
	uint8_t type() {return _submodule->type;};
	uint8_t version() {return _submodule->version;};
	uint8_t deviated() {return _submodule->deviated;};
	uint8_t disabled() {return _submodule->disabled;};
	uint8_t implemented() {return _submodule->implemented;};
	uint8_t rev_size() {return _submodule->rev_size;};
	uint8_t imp_size() {return _submodule->imp_size;};
	uint8_t inc_size() {return _submodule->inc_size;};
	uint8_t ident_size() {return _submodule->ident_size;};
	uint8_t tpdf_size() {return _submodule->tpdf_size;};
	uint8_t features_size() {return _submodule->features_size;};
	uint8_t augment_size() {return _submodule->augment_size;};
	uint8_t devaiation_size() {return _submodule->deviation_size;};
	uint8_t extensions_size() {return _submodule->extensions_size;};
	uint8_t ext_size() {return _submodule->ext_size;};
	S_Revision rev();

	S_Module belongsto() {return _submodule->belongsto ? S_Module(new Module(_submodule->belongsto, _deleter)) : NULL;};

private:
	struct lys_submodule *_submodule;
	S_Deleter _deleter;
};

class Ext_Instance
{
public:
	Ext_Instance(lys_ext_instance *ext_instance, S_Deleter deleter);
	~Ext_Instance();
	//TODO void *parent();
	const char *arg_value() {return _ext_instance->arg_value;};
	uint16_t flags() {return _ext_instance->flags;};
	uint8_t ext_size() {return _ext_instance->ext_size;};
	uint8_t insubstmt_index() {return _ext_instance->insubstmt_index;};
	uint8_t insubstmt() {return _ext_instance->insubstmt;};
	uint8_t parent_type() {return _ext_instance->parent_type;};
	uint8_t ext_type() {return _ext_instance->ext_type;};
	std::vector<S_Ext_Instance> *ext();
	void *priv() {return _ext_instance->priv;};
	S_Module module() {return _ext_instance->module ? S_Module(new Module(_ext_instance->module, _deleter)) : NULL;};
	LYS_NODE nodetype() {return _ext_instance->nodetype;};
private:
	struct lys_ext_instance *_ext_instance;
	S_Deleter _deleter;
};

class Revision
{
public:
	Revision(lys_revision *revision, S_Deleter deleter);
	~Revision();
	char *date() {return &_revisinon->date[0];};
	uint8_t ext_size() {return _revisinon->ext_size;};
	const char *dsc() {return _revisinon->dsc;};
	const char *ref() {return _revisinon->ref;};

private:
	struct lys_revision *_revisinon;
	S_Deleter _deleter;
};

class Schema_Node
{
public:
	Schema_Node(lys_node *node, S_Deleter deleter);
	~Schema_Node();
	const char *name() {return _node->name;};
	const char *dsc() {return _node->dsc;};
	const char *ref() {return _node->ref;};
	uint16_t flags() {return _node->flags;};
	uint8_t ext_size() {return _node->ext_size;};
	uint8_t iffeature_size() {return _node->iffeature_size;};
	// struct lys_ext_instance **ext;
	// struct lys_iffeature *iffeature;
	S_Module module();
	LYS_NODE nodetype() {return _node->nodetype;};
	S_Schema_Node parent();
	S_Schema_Node child();
	S_Schema_Node next();
	S_Schema_Node prev();
	S_Set find_xpath(const char *expr, int options);
	//struct ly_set *lys_xpath_atomize(const struct lys_node *ctx_node, enum lyxp_node_type ctx_node_type,
    //                             const char *expr, int options);
	S_Set xpath_atomize(int options);
	// void *priv;

	/* emulate TREE macro's */
	std::vector<S_Schema_Node> *tree_for();
	std::vector<S_Schema_Node> *tree_dfs();

	/* SWIG can not access private variables so it needs public getters */
	struct lys_node *swig_node() {return _node;};
	S_Deleter swig_deleter() {return _deleter;};

	friend Set;
	friend Data_Node;

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Container : public Schema_Node
{
public:
	Schema_Node_Container(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_Container();
	S_When when();
    //struct lys_restr *must;          /**< array of must constraints */
    //struct lys_tpdf *tpdf;           /**< array of typedefs */
    const char *presence() {return ((struct lys_node_container *) _node)->presence;};

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Choice : public Schema_Node
{
public:
	Schema_Node_Choice(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_Choice();
	S_When when();
    //struct lys_node *dflt;           /**< default case of the choice (optional) */

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Leaf : public Schema_Node
{
public:
	Schema_Node_Leaf(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_Leaf();
	S_Set backlinks();
	S_When when();
    //struct lys_restr *must;          /**< array of must constraints */
    //struct lys_type type;            /**< YANG data type definition of the leaf (mandatory) */
    const char *units() {return ((struct lys_node_leaf *)_node)->units;};
    const char *dflt() {return ((struct lys_node_leaf *)_node)->dflt;};

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Leaflist : public Schema_Node
{
public:
	Schema_Node_Leaflist(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_Leaflist();
	S_When when();
	S_Set backlinks();
    //struct lys_restr *must;          /**< array of must constraints */
    //struct lys_type type;            /**< YANG data type definition of the leaf (mandatory) */
    const char *units() {return ((struct lys_node_leaflist *)_node)->units;};
    //TODO vector of string dflt();
    uint32_t min() {return ((struct lys_node_leaflist *)_node)->min;};
    uint32_t max() {return ((struct lys_node_leaflist *)_node)->max;};

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_List : public Schema_Node
{
public:
	Schema_Node_List(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_List();
	S_When when();
    //struct lys_restr *must;          /**< array of must constraints */
    //struct lys_tpdf *tpdf;           /**< array of typedefs */
    //struct lys_node_leaf **keys;     /**< array of pointers to the key nodes */
    //struct lys_unique *unique;       /**< array of unique statement structures */
    uint32_t min() {return ((struct lys_node_list *)_node)->min;};
    uint32_t max() {return ((struct lys_node_list *)_node)->max;};
    const char *keys_str() {return ((struct lys_node_list *)_node)->keys_str;};

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Anydata : public Schema_Node
{
public:
	Schema_Node_Anydata(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_Anydata();
	S_When when();
	//struct lys_restr *must;          /**< array of must constraints */

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Uses : public Schema_Node
{
public:
	Schema_Node_Uses(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_Uses();
	S_When when();
    //struct lys_refine *refine;       /**< array of refine changes to the referred grouping */
    //struct lys_node_augment *augment;/**< array of local augments to the referred grouping */
    //struct lys_node_grp *grp;        /**< referred grouping definition (mandatory) */

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Grp : public Schema_Node
{
public:
	Schema_Node_Grp(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_Grp();
    //struct lys_tpdf *tpdf;           /**< array of typedefs */

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Case : public Schema_Node
{
public:
	Schema_Node_Case(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_Case();
	S_When when();

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Inout : public Schema_Node
{
public:
	Schema_Node_Inout(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_Inout();
    //struct lys_tpdf *tpdf;           /**< array of typedefs */
    //struct lys_restr *must;          /**< array of must constraints */

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Notif : public Schema_Node
{
public:
	Schema_Node_Notif(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_Notif();
    //struct lys_tpdf *tpdf;           /**< array of typedefs */
    //struct lys_restr *must;          /**< array of must constraints */

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Action : public Schema_Node
{
public:
	Schema_Node_Action(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_Action();
    //struct lys_tpdf *tpdf;           /**< array of typedefs */

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Augment : public Schema_Node
{
public:
	Schema_Node_Augment(struct lys_node *node, S_Deleter deleter = NULL);
	~Schema_Node_Augment();
	S_When when();
    //struct lys_node *target;         /**< pointer to the target node */

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class When
{
public:
	When(struct lys_when *when, S_Deleter deleter = NULL);
	~When();
	const char *cond() {return _when->cond;};
	const char *dsc() {return _when->dsc;};
	const char *ref() {return _when->ref;};
	std::vector<S_Ext_Instance> *ext();
	uint8_t ext_size() {return _when->ext_size;};

private:
	struct lys_when *_when;
	S_Deleter _deleter;
};

class Substmt
{
public:
	Substmt(struct lyext_substmt *substmt, S_Deleter deleter);
	~Substmt();
	LY_STMT stmt() {return _substmt->stmt;};
	size_t offest() {return _substmt->offset;};
	LY_STMT_CARD cardinality() {return _substmt->cardinality;};
private:
	struct lyext_substmt *_substmt;
	S_Deleter _deleter;
};

class Ext
{
public:
	Ext(struct lys_ext *ext, S_Deleter deleter);
	~Ext();
	const char *name() {return _ext->name;};
	const char *dsc() {return _ext->dsc;};
	const char *ref() {return _ext->ref;};
	uint16_t flags() {return _ext->flags;};
	uint8_t ext_size() {return _ext->ext_size;};
	std::vector<S_Ext_Instance> *ext();
	const char *argument() {return _ext->argument;};
	S_Module module();
	//struct lyext_plugin *plugin;
private:
	struct lys_ext *_ext;
	S_Deleter _deleter;
};
#endif
