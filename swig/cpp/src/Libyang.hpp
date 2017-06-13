#ifndef LIBYANG_H
#define LIBYANG_H

#include <iostream>
#include <memory>
#include <exception>

#include "Internal.hpp"
#include "Tree_Data.hpp"
#include "Tree_Schema.hpp"

extern "C" {
#include <libyang/libyang.h>
}

using namespace std;

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
	Context(const char *search_dir = NULL);
	Context(const char *search_dir, const char *path, LYD_FORMAT format);
	Context(const char *search_dir, LYD_FORMAT format, const char *data);
	~Context();
	void set_searchdir(const char *search_dir);
	void unset_searchdirs();
	const char *get_searchdir();
	void set_allimplemented();
	void unset_allimplemented();
	S_Tree_Data info();
	S_Module get_module(const char *name, const char *revision);
	S_Module get_module_older(S_Module module);
	S_Module load_module(const char *name, const char *revision);
	S_Module get_module_by_ns(const char *ns, const char *revision);
	void clean();

private:
	struct ly_ctx *_ctx;
	S_Deleter _deleter;
};

#endif
