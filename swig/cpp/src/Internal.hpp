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

#define S_String                 std::string
#define S_Deleter                std::shared_ptr<Deleter>

/* Libyang.hpp */
#define S_Context                std::shared_ptr<Context>

/* Tree_Data.hpp */
#define S_Data_Node              std::shared_ptr<Data_Node>
#define S_Data_Node_Leaf_List    std::shared_ptr<Data_Node_Leaf_List>
#define S_Data_Node_Anydata      std::shared_ptr<Data_Node_Anydata>
#define S_Attr                   std::shared_ptr<Attr>

/* Tree_Schema.hpp */
#define S_Module                 std::shared_ptr<Module>
#define S_Submodule              std::shared_ptr<Submodule>
#define S_Ext_Instance           std::shared_ptr<Ext_Instance>
#define S_Revision               std::shared_ptr<Revision>
#define S_Schema_Node            std::shared_ptr<Schema_Node>
#define S_Schema_Node_Container  std::shared_ptr<Schema_Node_Container>
#define S_Schema_Node_Choice     std::shared_ptr<Schema_Node_Choice>
#define S_Schema_Node_Leaf       std::shared_ptr<Schema_Node_Leaf>
#define S_Schema_Node_Leaflist   std::shared_ptr<Schema_Node_Leaflist>
#define S_Schema_Node_List       std::shared_ptr<Schema_Node_List>
#define S_Schema_Node_Anydata    std::shared_ptr<Schema_Node_Anydata>
#define S_Schema_Node_Uses       std::shared_ptr<Schema_Node_Uses>
#define S_Schema_Node_Grp        std::shared_ptr<Schema_Node_Grp>
#define S_Schema_Node_Case       std::shared_ptr<Schema_Node_Case>
#define S_Schema_Node_Inout      std::shared_ptr<Schema_Node_Inout>
#define S_Schema_Node_Notif      std::shared_ptr<Schema_Node_Notif>
#define S_Schema_Node_Action     std::shared_ptr<Schema_Node_Action>
#define S_Schema_Node_Augment    std::shared_ptr<Schema_Node_Augment>
#define S_Substmt                std::shared_ptr<Substmt>
#define S_Ext                    std::shared_ptr<Ext>

extern "C" {
#include <libyang/libyang.h>
}

#define typeof(x) __typeof__(x)

class Deleter
{
public:
    Deleter(ly_ctx *ctx);
    ~Deleter();

private:
    ly_ctx *_ctx;
};

#endif
