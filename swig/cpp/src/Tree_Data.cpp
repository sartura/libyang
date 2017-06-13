#include <iostream>
#include <memory>
#include <stdexcept>

#include "Libyang.hpp"
#include "Tree_Data.hpp"

extern "C" {
#include <libyang/libyang.h>
#include <libyang/tree_data.h>
}

using namespace std;

Tree_Data::Tree_Data(struct lyd_node *node, S_Deleter deleter) {
	_node = node;
	_deleter = deleter;
};
Tree_Data::~Tree_Data() {};
