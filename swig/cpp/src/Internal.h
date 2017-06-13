#ifndef INTERNAL_H
#define INTERNAL_H

#define S_String    std::string
#define S_Deleter   std::shared_ptr<Deleter>
#define S_Context   std::shared_ptr<Context>
#define S_Tree_Data std::shared_ptr<Tree_Data>
#define S_Module    std::shared_ptr<Module>

extern "C" {
#include "libyang/libyang.h"
}

class Deleter
{
public:
    Deleter(ly_ctx *ctx);
    ~Deleter();

private:
    ly_ctx *_ctx;
};

#endif
