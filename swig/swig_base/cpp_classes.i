%module cpp_classes

%include <std_except.i>
%catches(std::runtime_error, std::exception, std::string);

%include <typemaps.i>
%include <stdint.i>
%include <std_string.i>

%include <std_shared_ptr.i>
%ignore throw_exception;

%shared_ptr(Context);

%{
/* Includes the header in the wrapper code */
#include "Libyang.h"
%}

%include "Libyang.h"
