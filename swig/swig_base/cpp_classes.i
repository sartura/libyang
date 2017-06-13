%module cpp_classes

%include <std_except.i>
%catches(std::runtime_error, std::exception, std::string);

%include <typemaps.i>
%include <stdint.i>
%include <std_string.i>

%include <std_shared_ptr.i>
%ignore throw_exception;

%shared_ptr(Context);
%shared_ptr(Data_Node);
%shared_ptr(Module);
%shared_ptr(Modules);
%shared_ptr(Error);

%{
/* Includes the header in the wrapper code */
#include "Internal.hpp"
#include "Libyang.hpp"
#include "Tree_Data.hpp"
#include "Tree_Schema.hpp"
%}

%include "Internal.hpp"
%include "Libyang.hpp"
%include "Tree_Data.hpp"
%include "Tree_Schema.hpp"
