/**
 * @file context.cpp
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
		ctx = S_Context(new Context("/etc/sysrepo2/yang"));
    } catch( const std::exception& e ) {
		cout << e.what() << endl;
		auto err = Error();
		cout << "err: " << err.err() << endl;
		cout << "vecode: " << err.vecode() << endl;
		cout << "errmsg: " << err.errmsg() << endl;
		cout << "errpath: " << err.errpath() << endl;
		cout << "errapptag: " << err.errapptag() << endl;
	}

	try {
		ctx = S_Context(new Context("/etc/sysrepo/yang"));
    } catch( const std::exception& e ) {
		cout << e.what() << endl;
	}

	auto folders = std::shared_ptr<std::vector<S_String>>(ctx->get_searchdirs());
	std::vector<S_String>::iterator elem;
	for(elem = folders->begin() ; elem != folders->end() ; ++elem) {
		cout << (*elem) << endl;
	}
	cout << endl;

	auto module = ctx->get_module("ietf-interfaces", NULL, 0);
	if (NULL != module) {
		cout << module->name() << endl;
	} else {
		module = ctx->load_module("ietf-interfaces", NULL);
		if (NULL != module) {
			cout << module->name() << endl;
		}
	}

	auto modules = std::shared_ptr<std::vector<S_Module>>(ctx->get_module_iter());
	std::vector<S_Module>::iterator mod;
	for(mod = modules->begin() ; mod != modules->end() ; ++mod) {
		cout << "module " << (*mod)->name() << " prefix " << (*mod)->prefix() << " type " << (*mod)->type() << endl;
	}

	return 0;
}
