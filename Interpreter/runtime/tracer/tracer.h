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

#ifndef TRACER_H_INCLUDED
#define TRACER_H_INCLUDED
/* -------------------------------------------------------------------------- */
#include <iostream>
   using std::cout;
   using std::endl;
   using std::cerr;
#include <string>
   using std::string;
#include "types.h"
#include "value.h"
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_TRACING
  class Tracer
  {
     public:
        // Runtime tracing
        void WriteVarPrint(string name,Value *val,string cur_file,
                                                                size3 cur_line);
        void WriteArrPrint(string name,size3 index,Value *val,
                                                string cur_file,size3 cur_line);
        void WriteVarPrint(string name,Value const &val,string cur_file,
                                                                size3 cur_line);
        void WriteArrPrint(string name,size3 index,Value const &val,
                                                string cur_file,size3 cur_line);
        void WriteTreeTracing(StatementKind kind,string cur_file,
                                                                size3 cur_line);
        void WriteTreeTracing(StatementKind skind,ProcKind pkind,
                                                string cur_file,size3 cur_line);
  };
/* -------------------------------------------------------------------------- */
  extern Tracer tracer;
/* -------------------------------------------------------------------------- */
#endif // DISABLE_TRACING
/* -------------------------------------------------------------------------- */
#endif // TRACER_H_INCLUDED
