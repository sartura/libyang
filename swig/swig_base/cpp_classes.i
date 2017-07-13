%module cpp_classes

%include <std_except.i>
%catches(std::runtime_error, std::exception, std::string);

%include <cpointer.i>
%include <typemaps.i>
%include <stdint.i>
%include <std_string.i>
%include <std_vector.i>
%include <std_shared_ptr.i>

%ignore throw_exception;

/* Libyang.hpp */
%shared_ptr(Context);

/* Tree_Data.hpp */
%shared_ptr(Data_Node);
%shared_ptr(Data_Node_Leaf_List);
%shared_ptr(Data_Node_Anydata);
%shared_ptr(Attr);

/* Tree_Schema.hpp */
%shared_ptr(Module);
%shared_ptr(Submodule);
%shared_ptr(Ext_Instance);
%shared_ptr(Revision);
%shared_ptr(Schema_Node);
%shared_ptr(Schema_Node_Container);
%shared_ptr(Schema_Node_Choice);
%shared_ptr(Schema_Node_Leaf);
%shared_ptr(Schema_Node_Leaflist);
%shared_ptr(Schema_Node_List);
%shared_ptr(Substmt);
%shared_ptr(Ext);

%shared_ptr(Error);

%{
/* Includes the header in the wrapper code */
#include "Internal.hpp"
#include "Libyang.hpp"
#include "Tree_Data.hpp"
#include "Tree_Schema.hpp"
#include <vector>
%}

%include "Internal.hpp"
%include "Libyang.hpp"
%include "Tree_Data.hpp"
%include "Tree_Schema.hpp"

%template(vectorString) std::vector<string>;
%template(vectorModules) std::vector<S_Module>;
%template(vectorData_Node) std::vector<S_Data_Node>;
%template(vectorSchema_Node) std::vector<S_Schema_Node>;
%template(vectorExt_Instance) std::vector<S_Ext_Instance>;

