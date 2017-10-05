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
#include "../../../src/libyang.h"
#include "../../../src/tree_schema.h"
}

using namespace std;

/* defined */
class Module;
class Submodule;
class Ext_Instance;
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
class Substmt;
class Ext;
class Refine_Mod_List;
class Refine;
class Deviate;
class Deviation;
class Import;
class Include;
class Revision;
class Tpdf;
class Unique;
class Feature;
class Restr;
class When;
class Ident;

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
	std::vector<S_Deviation> *deviation();

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
	S_Context ctx() NEW(_submodule, ctx, Context);
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
	std::vector<S_Deviation> *deviation();

	S_Module belongsto() NEW(_submodule, belongsto, Module);

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
	S_Module module() NEW(_ext_instance, module, Module);
	LYS_NODE nodetype() {return _ext_instance->nodetype;};
private:
	struct lys_ext_instance *_ext_instance;
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
	virtual S_Schema_Node parent();
	virtual S_Schema_Node child();
	virtual S_Schema_Node next();
	virtual S_Schema_Node prev();
	S_Set find_xpath(const char *path);
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
	friend Context;

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Container : public Schema_Node
{
public:
	using Schema_Node::Schema_Node;
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
	using Schema_Node::Schema_Node;
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
	using Schema_Node::Schema_Node;
	~Schema_Node_Leaf();
	S_Set backlinks();
	S_When when();
	//struct lys_type type;            /**< YANG data type definition of the leaf (mandatory) */
	const char *units() {return ((struct lys_node_leaf *)_node)->units;};
	const char *dflt() {return ((struct lys_node_leaf *)_node)->dflt;};
	S_Schema_Node child() {return NULL;};

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Leaflist : public Schema_Node
{
public:
	using Schema_Node::Schema_Node;
	~Schema_Node_Leaflist();
	uint8_t dflt_size() {return ((struct lys_node_leaflist *)_node)->dflt_size;};
	uint8_t must_size() {return ((struct lys_node_leaflist *)_node)->must_size;};
	S_When when();
	S_Set backlinks();
	std::vector<S_Restr> *must();
	//struct lys_type type;            /**< YANG data type definition of the leaf (mandatory) */
	const char *units() {return ((struct lys_node_leaflist *)_node)->units;};
	vector<S_String> *dflt();
	uint32_t min() {return ((struct lys_node_leaflist *)_node)->min;};
	uint32_t max() {return ((struct lys_node_leaflist *)_node)->max;};
	S_Schema_Node child() {return NULL;};

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_List : public Schema_Node
{
public:
	using Schema_Node::Schema_Node;
	~Schema_Node_List();
	uint8_t must_size() {return ((struct lys_node_list *)_node)->must_size;};
	uint8_t tpdf_size() {return ((struct lys_node_list *)_node)->tpdf_size;};
	uint8_t keys_size() {return ((struct lys_node_list *)_node)->keys_size;};
	uint8_t unique_size() {return ((struct lys_node_list *)_node)->unique_size;};
	S_When when();
	std::vector<S_Restr> *must();
	std::vector<S_Tpdf> *tpdf();
	//std::vector<Schema_Node_Leaf> *keys();
	std::vector<S_Unique> *unique();
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
	using Schema_Node::Schema_Node;
	~Schema_Node_Anydata();
	uint8_t must_size() {return ((struct lys_node_list *)_node)->must_size;};
	S_When when();
	std::vector<S_Restr> *must();

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Uses : public Schema_Node
{
public:
	using Schema_Node::Schema_Node;
	~Schema_Node_Uses();
	uint8_t augment_size() {return ((struct lys_node_uses *)_node)->augment_size;};
	S_When when();
	std::vector<S_Refine> *refine();
	//std::vector<S_Schema_Node_Augment> *augment();
	//S_Schema_Node_Grp *grp() NEW_CASTED(lys_node_uses, _node, grp, Schema_Node_Grp);

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Grp : public Schema_Node
{
public:
	using Schema_Node::Schema_Node;
	~Schema_Node_Grp();
	uint8_t tpdf_size() {return ((struct lys_node_grp *)_node)->tpdf_size;};
	std::vector<S_Tpdf> *tpdf();

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Case : public Schema_Node
{
public:
	using Schema_Node::Schema_Node;
	~Schema_Node_Case();
	S_When when();

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Inout : public Schema_Node
{
public:
	using Schema_Node::Schema_Node;
	~Schema_Node_Inout();
	uint8_t tpdf_size() {return ((struct lys_node_inout *)_node)->tpdf_size;};
	uint8_t must_size() {return ((struct lys_node_inout *)_node)->must_size;};
	std::vector<S_Tpdf> *tpdf();
	std::vector<S_Restr> *must();

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Notif : public Schema_Node
{
public:
	using Schema_Node::Schema_Node;
	~Schema_Node_Notif();
	uint8_t tpdf_size() {return ((struct lys_node_notif *)_node)->tpdf_size;};
	uint8_t must_size() {return ((struct lys_node_notif *)_node)->must_size;};
	std::vector<S_Tpdf> *tpdf();
	std::vector<S_Restr> *must();

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Rpc_Action : public Schema_Node
{
public:
	using Schema_Node::Schema_Node;
	~Schema_Node_Rpc_Action();
	uint8_t tpdf_size() {return ((struct lys_node_rpc_action *)_node)->tpdf_size;};
	std::vector<S_Tpdf> *tpdf();

private:
	struct lys_node *_node;
	S_Deleter _deleter;
};

class Schema_Node_Augment : public Schema_Node
{
public:
	using Schema_Node::Schema_Node;
	~Schema_Node_Augment();
	S_When when();
	S_Schema_Node target() NEW_CASTED(lys_node_augment, _node, target, Schema_Node);

private:
	struct lys_node *_node;
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

class Refine_Mod_List
{
public:
	Refine_Mod_List(struct lys_refine_mod_list *list, S_Deleter deleter);
	~Refine_Mod_List();
	uint32_t min() {return _list->min;};
	uint32_t max() {return _list->max;};

private:
	struct lys_refine_mod_list *_list;
	S_Deleter _deleter;
};

class Refine
{
public:
	Refine(struct lys_refine *refine, S_Deleter deleter);
	~Refine();
	const char *target_name() {return _refine->target_name;};
	const char *dsc() {return _refine->dsc;};
	const char *ref() {return _refine->ref;};
	uint16_t flags() {return _refine->flags;};
	uint8_t ext_size() {return _refine->ext_size;};
	uint8_t iffeature_size() {return _refine->iffeature_size;};
	uint16_t target_type() {return _refine->target_type;};
	uint8_t must_size() {return _refine->must_size;};
	uint8_t dflt_size() {return _refine->dflt_size;};
	std::vector<S_Ext_Instance> *ext();
	//struct lys_iffeature *iffeature; /**< array of if-feature expressions */
	S_Module module();
	//struct lys_restr *must;          /**< array of additional must restrictions to be added to the target */
	vector<S_String> *dflt() NEW_STRING_LIST(_refine, dflt, dflt_size);
	//union lys_refine_mod mod;        /**< mutually exclusive target modifications according to the possible target_type */

private:
	struct lys_refine *_refine;
	S_Deleter _deleter;
};

class Deviate
{
public:
	Deviate(struct lys_deviate *deviate, S_Deleter deleter);
	~Deviate();
	LYS_DEVIATE_TYPE mod() {return _deviate->mod;};
	uint8_t flags() {return _deviate->flags;};
	uint8_t dflt_size() {return _deviate->dflt_size;};
	uint8_t ext_size() {return _deviate->ext_size;};
	uint8_t min_set() {return _deviate->min_set;};
	uint8_t max_set() {return _deviate->max_set;};
	uint8_t must_size() {return _deviate->must_size;};
	uint8_t unique_size() {return _deviate->unique_size;};
	uint32_t min() {return _deviate->min;};
	uint32_t max() {return _deviate->max;};
    //struct lys_restr *must;          /**< Properties: must - array of must constraints */
	//struct lys_unique *unique;       /**< Properties: unique - array of unique statement structures */
	//struct lys_type *type;           /**< Properties: type - pointer to type in target, type cannot be deleted or added */
	const char *units() {return _deviate->units;};
	vector<S_String> *dflt() NEW_STRING_LIST(_deviate, dflt, dflt_size);
	std::vector<S_Ext_Instance> *ext();

private:
	struct lys_deviate *_deviate;
	S_Deleter _deleter;
};

class Deviation
{
public:
	Deviation(struct lys_deviation *deviation, S_Deleter deleter);
	~Deviation();
	const char *target_name() {return _deviation->target_name;};
	const char *dsc() {return _deviation->dsc;};
	const char *ref() {return _deviation->ref;};
	S_Schema_Node orig_node();
	uint8_t deviate_size() {return _deviation->deviate_size;};
	uint8_t ext_size() {return _deviation->ext_size;};
	std::vector<S_Deviate> *deviate();
	std::vector<S_Ext_Instance> *ext();

private:
	struct lys_deviation *_deviation;
	S_Deleter _deleter;
};

class Import
{
public:
	Import(struct lys_import *import, S_Deleter deleter);
	~Import();
	S_Module module() NEW(_import, module, Module);
	const char *prefix() {return _import->prefix;};
	char *rev() {return &_import->rev[0];};
	uint8_t ext_size() {return _import->ext_size;};
	std::vector<S_Ext_Instance> *ext() NEW_P_LIST(_import, ext, ext_size, Ext_Instance);
	const char *dsc() {return _import->dsc;};
	const char *ref() {return _import->ref;};

private:
	struct lys_import *_import;
	S_Deleter _deleter;
};

class Include
{
public:
	Include(struct lys_include *include, S_Deleter deleter);
	~Include();
	S_Submodule submodule() NEW(_include, submodule, Submodule);
	char *rev() {return &_include->rev[0];};
	uint8_t ext_size() {return _include->ext_size;};
	std::vector<S_Ext_Instance> *ext() NEW_P_LIST(_include, ext, ext_size, Ext_Instance);
	const char *dsc() {return _include->dsc;};
	const char *ref() {return _include->ref;};

private:
	struct lys_include *_include;
	S_Deleter _deleter;
};

class Revision
{
public:
	Revision(lys_revision *revision, S_Deleter deleter);
	~Revision();
	char *date() {return &_revision->date[0];};
	uint8_t ext_size() {return _revision->ext_size;};
	const char *dsc() {return _revision->dsc;};
	const char *ref() {return _revision->ref;};

private:
	struct lys_revision *_revision;
	S_Deleter _deleter;
};

class Tpdf
{
public:
	Tpdf(struct lys_tpdf *tpdf, S_Deleter deleter);
	~Tpdf();
	const char *name() {return _tpdf->name;};
	const char *dsc() {return _tpdf->dsc;};
	const char *ref() {return _tpdf->ref;};
	uint16_t flags() {return _tpdf->flags;};
	uint8_t ext_size() {return _tpdf->ext_size;};
	uint8_t padding_iffsize() {return _tpdf->padding_iffsize;};
	uint8_t has_union_leafref() {return _tpdf->has_union_leafref;};
	std::vector<S_Ext_Instance> *ext() NEW_P_LIST(_tpdf, ext, ext_size, Ext_Instance);
	const char *units() {return _tpdf->units;};
	S_Module module() NEW(_tpdf, module, Module);
	//struct lys_type type;            /**< base type from which the typedef is derived (mandatory). In case of a special
	const char *dflt() {return _tpdf->dflt;};

private:
	struct lys_tpdf *_tpdf;
	S_Deleter _deleter;
};

class Unique
{
public:
	Unique(struct lys_unique *unique, S_Deleter deleter);
	~Unique();
	std::vector<S_String> *expr() NEW_STRING_LIST(_unique, expr, expr_size);
	uint8_t expr_size() {return _unique->expr_size;};
	uint8_t trg_type() {return _unique->trg_type;};

private:
	struct lys_unique *_unique;
	S_Deleter _deleter;
};

class Feature
{
public:
	Feature(struct lys_feature *feature, S_Deleter);
	~Feature();
	const char *name() {return _feature->name;};
	const char *dsc() {return _feature->dsc;};
	const char *ref() {return _feature->ref;};
	uint16_t flags() {return _feature->flags;};
	uint8_t ext_size() {return _feature->ext_size;};
	uint8_t iffeature_size() {return _feature->iffeature_size;};
	std::vector<S_Ext_Instance> *ext() NEW_P_LIST(_feature, ext, ext_size, Ext_Instance);
	//struct lys_iffeature *iffeature; /**< array of if-feature expressions */
	S_Module module() NEW(_feature, module, Module);
	S_Set depfeatures() NEW(_feature, depfeatures, Set);

private:
	struct lys_feature *_feature;
	S_Deleter _deleter;
};

class Restr
{
public:
	Restr(struct lys_restr *restr, S_Deleter deleter);
	~Restr();
	const char *expr() {return _restr->expr;};
	const char *dsc() {return _restr->dsc;};
	const char *ref() {return _restr->ref;};
	const char *eapptag() {return _restr->eapptag;};
	const char *emsg() {return _restr->emsg;};
	std::vector<S_Ext_Instance> *ext() NEW_P_LIST(_restr, ext, ext_size, Ext_Instance);
	uint8_t ext_size() {return _restr->ext_size;};

private:
	struct lys_restr *_restr;
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

class Ident
{
public:
	Ident(struct lys_ident *ident, S_Deleter deleter);
	~Ident();
	const char *name() {return _ident->name;};
	const char *dsc() {return _ident->dsc;};
	const char *ref() {return _ident->ref;};
	uint16_t flags() {return _ident->flags;};
	uint8_t ext_size() {return _ident->ext_size;};
	uint8_t iffeature_size() {return _ident->iffeature_size;};
	uint8_t base_size() {return _ident->base_size;};
	std::vector<S_Ext_Instance> *ext() NEW_P_LIST(_ident, ext, ext_size, Ext_Instance);
    //struct lys_iffeature *iffeature; /**< array of if-feature expressions */
	S_Module module() NEW(_ident, module, Module);
	std::vector<S_Ident> *base();
	S_Set der() NEW(_ident, der, Set);

private:
	struct lys_ident *_ident;
	S_Deleter _deleter;
};

#endif
