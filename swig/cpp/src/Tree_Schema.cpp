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
