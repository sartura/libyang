#include <iostream>
#include <memory>
#include <stdexcept>

#include "Libyang.h"

extern "C" {
#include "libyang/libyang.h"
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
	if (NULL != node) {
		throw runtime_error("can not create new context");
	}
	return S_Tree_Data(new Tree_Data(node, _deleter));
}
void Context::clean() {
	return ly_ctx_clean(_ctx, NULL);
}
