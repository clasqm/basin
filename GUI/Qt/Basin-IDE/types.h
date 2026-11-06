/*
   Basin-IDE,frontend to Basin - QBASIC Interpreter.
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

#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED
#include <iostream>
/* -------------------------------------------------------------------------- */
   // Basic types
   typedef char           _char_t;
   typedef int             _int_t;
   typedef unsigned       _uint_t;
   typedef long           _long_t;
   typedef unsigned long _ulong_t;
   typedef float         _float_t;
   typedef double       _double_t;
   typedef bool           _bool_t;
   typedef size_t         _size_t;
   /* ---------------------------- */
   typedef _char_t  size1;
   typedef _int_t   size2;
   typedef _long_t  size3;
   typedef _ulong_t size4;

   // Type will be used for some loops
   typedef _int_t count;

   // Type for counting symbols in program
   typedef size4 s_count;
   typedef size4 line_count_t;
/* -------------------------------------------------------------------------- */
#endif // TYPES_H_INCLUDED
