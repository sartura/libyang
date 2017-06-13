/**
 * @file Internal.cpp
 * @author Mislav Novakovic <mislav.novakovic@sartura.hr>
 * @brief Internal C++ helper class
 *
 * @copyright
 * Copyright 2016 Deutsche Telekom AG.
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


#ifndef INTERNAL_H
#define INTERNAL_H

#define S_String    std::string
#define S_Deleter   std::shared_ptr<Deleter>
#define S_Context   std::shared_ptr<Context>
#define S_Data_Node std::shared_ptr<Data_Node>
#define S_Module    std::shared_ptr<Module>
#define S_Modules   std::shared_ptr<Modules>

extern "C" {
#include <libyang/libyang.h>
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
