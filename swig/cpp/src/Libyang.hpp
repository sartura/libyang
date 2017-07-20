/**
 * @file Libyang.hpp
 * @author Mislav Novakovic <mislav.novakovic@sartura.hr>
 * @brief Class implementation for libyang C header libyang.h.
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

#ifndef LIBYANG_H
#define LIBYANG_H

#include <iostream>
#include <memory>
#include <exception>
#include <vector>

#include "Internal.hpp"

extern "C" {
#include <libyang/libyang.h>
}

using namespace std;

/* defined */
class Context;
class Error;

/* used */
class Context;
class Module;
class Data_Node;
class Xml_Elem;

class Error
{
/* add custom deleter for Context class */
public:
	Error() {
		_err = *ly_errno_location();
		_vecode = *ly_vecode_location();
		_errmsg = ly_errmsg();
		_errpath = ly_errpath();
		_errapptag = ly_errapptag();
	};
	~Error() {};
	LY_ERR err() throw() {return _err;};
	LY_VECODE vecode() throw() {return _vecode;};
	const char *errmsg() const throw() {return _errmsg;};
	const char *errpath() const throw() {return _errpath;};
	const char *errapptag() const throw() {return _errapptag;};
private:
	LY_ERR _err;
	LY_VECODE _vecode;
	const char *_errmsg;
	const char *_errpath;
	const char *_errapptag;
};

class Context
{
public:
	Context(ly_ctx *ctx, S_Deleter deleter);
	Context(const char *search_dir = NULL);
	Context(const char *search_dir, const char *path, LYD_FORMAT format);
	Context(const char *search_dir, LYD_FORMAT format, const char *data);
	~Context();
	void set_searchdir(const char *search_dir) {return ly_ctx_set_searchdir(_ctx, search_dir);};
	void unset_searchdirs(int idx) {return ly_ctx_unset_searchdirs(_ctx, idx);};
	vector<string> *get_searchdirs();
	void set_allimplemented() {return ly_ctx_set_allimplemented(_ctx);};
	void unset_allimplemented() {return ly_ctx_unset_allimplemented(_ctx);};
	S_Data_Node info();
	vector<S_Module> *get_module_iter();
	vector<S_Module> *get_disabled_module_iter();
	S_Module get_module(const char *name, const char *revision);
	S_Module get_module_older(S_Module module);
	S_Module load_module(const char *name, const char *revision);
	S_Module get_module_by_ns(const char *ns, const char *revision);
	void clean();

	/* functions */
	S_Data_Node parse_mem(const char *data, LYD_FORMAT format, int options);
	S_Data_Node parse_fd(int fd, LYD_FORMAT format, int options);
	S_Data_Node parse_path(const char *path, LYD_FORMAT format, int options);
	S_Data_Node parse_xml(S_Xml_Elem elem, int options);

	friend Data_Node;

private:
	struct ly_ctx *_ctx;
	S_Deleter _deleter;
};

#endif
