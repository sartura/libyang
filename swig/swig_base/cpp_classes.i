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

/* Xml.hpp */
%shared_ptr(Xml_Ns);
%newobject Xml_Ns::next;

%shared_ptr(Xml_Attr);
%newobject Xml_Attr::next;
%newobject Xml_Attr::ns;

%shared_ptr(Xml_Elem);
%newobject Xml_Elem::parent;
%newobject Xml_Elem::attr;
%newobject Xml_Elem::child;
%newobject Xml_Elem::next;
%newobject Xml_Elem::prev;
%newobject Xml_Elem::ns;
%newobject Xml_Elem::get_ns;


/* Libyang.hpp */
%shared_ptr(Context);
%newobject Context::get_searchdirs;
%newobject Context::info;
%newobject Context::get_module_iter;
%newobject Context::get_disabled_module_iter;
%newobject Context::get_module;
%newobject Context::get_module_older;
%newobject Context::load_module;
%newobject Context::get_module_by_Ns;
%newobject Context::parse_mem;
%newobject Context::parse_fd;
%newobject Context::parse_path;
%newobject Context::parse_xml;

%shared_ptr(Set);
%newobject Set::data;
%newobject Set::schema;
%newobject Set::dup;


/* Tree_Data.hpp */
%shared_ptr(Data_Node);
%newobject Data_Node::schema;
%newobject Data_Node::attr;
%newobject Data_Node::next;
%newobject Data_Node::prev;
%newobject Data_Node::parent;
%newobject Data_Node::child;
%newobject Data_Node::path;
%newobject Data_Node::qualifed_path;
%newobject Data_Node::dup;
%newobject Data_Node::dup_to_ctx;
%newobject Data_Node::find_xpath;
%newobject Data_Node::find_instance;
%ignore Data_Node::swig_node;
%ignore Data_Node::swig_deleter;

%shared_ptr(Data_Node_Leaf_List);
%newobject Data_Node_Leaf_List::schema;
%newobject Data_Node_Leaf_List::attr;
%newobject Data_Node_Leaf_List::next;
%newobject Data_Node_Leaf_List::prev;
%newobject Data_Node_Leaf_List::parent;
%newobject Data_Node_Leaf_List::child;
%newobject Data_Node_Leaf_List::path;
%newobject Data_Node_Leaf_List::qualifed_path;
%newobject Data_Node_Leaf_List::dup;
%newobject Data_Node_Leaf_List::dup_to_ctx;
%newobject Data_Node_Leaf_List::find_xpath;
%newobject Data_Node_Leaf_List::find_instance;
%ignore Data_Node_Leaf_List::swig_node;
%ignore Data_Node_Leaf_List::swig_deleter;


%shared_ptr(Data_Node_Anydata);
%newobject Data_Node_Leaf_List::schema;
%newobject Data_Node_Leaf_List::attr;
%newobject Data_Node_Leaf_List::next;
%newobject Data_Node_Leaf_List::prev;
%newobject Data_Node_Leaf_List::parent;
%newobject Data_Node_Leaf_List::child;
%newobject Data_Node_Leaf_List::path;
%newobject Data_Node_Leaf_List::qualifed_path;
%newobject Data_Node_Leaf_List::dup;
%newobject Data_Node_Leaf_List::dup_to_ctx;
%newobject Data_Node_Leaf_List::find_xpath;
%newobject Data_Node_Leaf_List::find_instance;
%ignore Data_Node_Leaf_List::swig_node;
%ignore Data_Node_Leaf_List::swig_deleter;

%shared_ptr(Attr);
%newobject Attr::next;


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

