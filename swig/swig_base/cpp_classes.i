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

/* add %newobject 
 it tells python to free memory
 */
/* Xml.hpp */
%shared_ptr(Xml_Ns);
%shared_ptr(Xml_Attr);
%shared_ptr(Xml_Elem);

/* Libyang.hpp */
%shared_ptr(Context);
%shared_ptr(Set);

/* Tree_Data.hpp */
%shared_ptr(Data_Node);
%ignore Data_Node::swig_node;
%ignore Data_Node::swig_deleter;

%shared_ptr(Data_Node_Leaf_List);
%shared_ptr(Data_Node_Anydata);
%shared_ptr(Attr);

/* Tree_Schema.hpp */
%shared_ptr(Module);
%shared_ptr(Submodule);
%shared_ptr(Ext_Instance);
%shared_ptr(Revision);
%shared_ptr(Schema_Node);
%ignore Schema_Node::swig_node;
%ignore Schema_Node::swig_deleter;
%shared_ptr(Schema_Node_Container);
%shared_ptr(Schema_Node_Choice);
%shared_ptr(Schema_Node_Leaf);
%shared_ptr(Schema_Node_Leaflist);
%shared_ptr(Schema_Node_List);
%shared_ptr(Schema_Node_Anydata);
%shared_ptr(Schema_Node_Uses);
%shared_ptr(Schema_Node_Grp);
%shared_ptr(Schema_Node_Case);
%shared_ptr(Schema_Node_Inout);
%shared_ptr(Schema_Node_Notif);
%shared_ptr(Schema_Node_Action);
%shared_ptr(Schema_Node_Augment);
%shared_ptr(When);
%shared_ptr(Substmt);
%shared_ptr(Ext);

%shared_ptr(Error);

%{
/* Includes the header in the wrapper code */
#include "Xml.hpp"
#include "Internal.hpp"
#include "Libyang.hpp"
#include "Tree_Data.hpp"
#include "Tree_Schema.hpp"
#include <vector>
%}

%include "Xml.hpp"
%include "Internal.hpp"
%include "Libyang.hpp"
%include "Tree_Data.hpp"
%include "Tree_Schema.hpp"

%template(vectorString) std::vector<string>;
%template(vectorModules) std::vector<S_Module>;
%template(vectorData_Node) std::vector<S_Data_Node>;
%template(vectorSchema_Node) std::vector<S_Schema_Node>;
%template(vectorExt_Instance) std::vector<S_Ext_Instance>;
%template(vectorWhen) std::vector<S_When>;
%template(vectorXml_Elem) std::vector<S_Xml_Elem>;

