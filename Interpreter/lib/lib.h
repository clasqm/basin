/*
   basin ,simple QBASIC interpreter.
   Copyright (C) 2009-2011  #pragma, Evg

   This program is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation,  either  version  3  of the License, or(at your option) any later
   version.

   This  program  is  distributed  in  the  hope  that  it  will  be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
   details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <string>
   using std::string;
#include <sstream>
   using std::stringstream;
#include <iostream>

#include "types.h"
#include "options.h"
/* -------------------------------------------------------------------------- */
  template <typename T> string ntos (T number)
  {
     stringstream buffer;
     string str;
     buffer << number;
     buffer >> str;
     return str;
  }
/* -------------------------------------------------------------------------- */
  // Find string (name of variable) in container of variables.
  /// @return On success,return position of variable in vector,
  /// on failure return -1
  template <typename T> size3 findVarName (const std::string &str, T *array)
  {
     for (typename T::iterator i = array->begin(); i < array->end(); ++i)
        if ((*i)->GetName()  == str)
           return distance(array->begin(),i);
     return -1;
  }
/* -------------------------------------------------------------------------- */
  template <typename T1, typename T2> size3 findVar (const T1 *var, T2 *array)
  {
     for (typename T2::iterator i = array->begin(); i < array->end(); ++i)
        if ((*i)  == var)
           return distance(array->begin(),i);
     return -1;
  }
/* -------------------------------------------------------------------------- */
  template <typename T> void arrayPrinter (T *array)
  {
     size3 size = array->size();
     if (!user_opt.is_trace_silent())
     {
        for (size3 i = 0; i < size; ++i)
        {
           std::cout << array->at(i);
           if (i != size-1)
              std::cout << ',';
        }
        return;
     }
     for (size3 i = 0; i < size; ++i)
     {
        trace_file << array->at(i);
        if (i != size-1)
           trace_file << ',';
     }
  }
/* -------------------------------------------------------------------------- */
#endif // LIB_H_INCLUDED
