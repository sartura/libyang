#include <iostream>
#include <memory>
#include <stdexcept>

#include "Internal.h"
#include "Libyang.h"
#include "Tree_Schema.h"

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
