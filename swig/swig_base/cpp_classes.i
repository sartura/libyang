%module cpp_classes

%include <std_except.i>
%catches(std::runtime_error, std::exception, std::string);

%include <typemaps.i>
%include <stdint.i>
%include <std_string.i>

%include <std_shared_ptr.i>
%ignore throw_exception;

%shared_ptr(Context);
%shared_ptr(Error);

%{
/* Includes the header in the wrapper code */
#include "Internal.h"
#include "Libyang.h"
#include "Tree_Data.h"
%}

%include "Internal.h"
%include "Libyang.h"
%include "Tree_Data.h"
