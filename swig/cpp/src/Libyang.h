#ifndef LIBYANG_H
#define LIBYANG_H

#include <iostream>

#include "Internal.h"

extern "C" {
#include "libyang/libyang.h"
}

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
	void clean();

private:
	struct ly_ctx *_ctx;
};

#endif
