#ifndef INTERNAL_H
#define INTERNAL_H

#define S_String  std::string
#define S_Deleter std::shared_ptr<Deleter>
#define S_Context std::shared_ptr<Context>

extern "C" {
#include "libyang/libyang.h"
}

/* TODO
class Deleter
{
public:
    Deleter(ly_ctx *ctx);
    ~Deleter();

private:
    ly_ctx *ctx;
};
*/

#endif
