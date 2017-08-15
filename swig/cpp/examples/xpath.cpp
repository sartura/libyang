/**
 * @file xpath.cpp
 * @author Mislav Novakovic <mislav.novakovic@sartura.hr>
 * @brief Example of the libyang C++ bindings
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

#include <Libyang.hpp>
#include <Tree_Data.hpp>
#include <Tree_Schema.hpp>

using namespace std;

int main() {

	S_Context ctx = NULL;
    try {
		ctx = S_Context(new Context("/etc/sysrepo/yang"));
    } catch( const std::exception& e ) {
		cout << e.what() << endl;
		return -1;
	}

	auto module = ctx->load_module("turing-machine", NULL);
	if (NULL == module) {
		printf("module not loaded\n");
		return -1;
	}

	S_Data_Node node = NULL;
	try {
		node = ctx->parse_path("/etc/sysrepo/data/turing-machine.startup", LYD_XML, LYD_OPT_CONFIG);
    } catch( const std::exception& e ) {
		cout << e.what() << endl;
		return -1;
	}

	auto node_set = node->find_path("/turing-machine:turing-machine/transition-function/delta[label='left summand']/*");\
	if (NULL == node_set) {
		printf("could not find data for xpath\n");
		return -1;
	}

	std::vector<S_Data_Node>::iterator data_set;
	auto list = std::shared_ptr<std::vector<S_Data_Node>>(node_set->data());
	for(data_set = list->begin() ; data_set != list->end() ; ++data_set) {
		cout << "name: " << (*data_set)->schema()->name() << " type: " << (*data_set)->schema()->nodetype() << " path: " << (*data_set)->path() << endl;
	}

	return 0;
}
