#include <iostream>
#include <memory>
#include <stdexcept>

#include "Internal.h"
#include "Libyang.h"
#include "Tree_Data.h"
#include "Tree_Schema.h"

extern "C" {
#include "libyang/libyang.h"
#include "libyang/tree_data.h"
#include "libyang/tree_schema.h"
}

using namespace std;

Context::Context(const char *search_dir) {
	_ctx = ly_ctx_new(search_dir);
	if (NULL == _ctx) {
		goto cleanup;
	}
    _deleter = S_Deleter(new Deleter(_ctx));
	return;
cleanup:
	throw runtime_error("can not create new context");
	return;
}
Context::Context(const char *search_dir, const char *path, LYD_FORMAT format) {
	_ctx = ly_ctx_new_ylpath(search_dir, path, format);
	if (NULL == _ctx) {
		goto cleanup;
	}
    _deleter = S_Deleter(new Deleter(_ctx));
	return;
cleanup:
	throw runtime_error("can not create new context");
	return;
}
Context::Context(const char *search_dir, LYD_FORMAT format, const char *data) {
	_ctx = ly_ctx_new_ylmem(search_dir, data, format);
	if (NULL == _ctx) {
		goto cleanup;
	}
    _deleter = S_Deleter(new Deleter(_ctx));
	return;
cleanup:
	throw runtime_error("can not create new context");
	return;
}
Context::~Context() {}
void Context::set_searchdir(const char *search_dir) {
	return ly_ctx_set_searchdir(_ctx, search_dir);
}
void Context::unset_searchdirs() {
	return ly_ctx_unset_searchdirs(_ctx);
}
const char *Context::get_searchdir() {
	return ly_ctx_get_searchdir(_ctx);
}
void Context::set_allimplemented() {
	return ly_ctx_set_allimplemented(_ctx);
}
void Context::unset_allimplemented() {
	return ly_ctx_unset_allimplemented(_ctx);
}
S_Tree_Data Context::info() {
	struct lyd_node *node = ly_ctx_info(_ctx);
	return node ? S_Tree_Data(new Tree_Data(node, _deleter)) : NULL;
}
S_Module Context::get_module(const char *name, const char *revision) {
	const struct lys_module *module = ly_ctx_get_module(_ctx, name, revision);
	return module ? S_Module(new Module((lys_module *) module, _deleter)) : NULL;
}
S_Module Context::get_module_older(S_Module module) {
	const struct lys_module *new_module = ly_ctx_get_module_older(_ctx, module->_module);
	return new_module ? S_Module(new Module((lys_module *) new_module, _deleter)) : NULL;
}
S_Module Context::load_module(const char *name, const char *revision) {
	const struct lys_module *module = ly_ctx_load_module(_ctx, name, revision);
	return module ? S_Module(new Module((lys_module *) module, _deleter)) : NULL;
}
S_Module Context::get_module_by_ns(const char *ns, const char *revision) {
	const struct lys_module *module = ly_ctx_get_module_by_ns(_ctx, ns, revision);
	return module ? S_Module(new Module((lys_module *) module, _deleter)) : NULL;
}
void Context::clean() {
	return ly_ctx_clean(_ctx, NULL);
}
