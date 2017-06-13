#ifndef TREE_SCHEMA_H
#define TREE_SCHEMA_H

#include <iostream>
#include <memory>
#include <exception>

#include "Internal.h"
#include "Libyang.h"

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

#endif
