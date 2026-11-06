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
#include <cassert>
#include "options.h"
#include "tracer.h"
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_TRACING
/* -------------------------------------------------------------------------- */
  Tracer tracer;
/* -------------------------------------------------------------------------- */
/// Runtime tracing
/* -------------------------------------------------------------------------- */
  void Tracer::WriteVarPrint(string name,Value *val,string cur_file,
                                                                 size3 cur_line)
  {
     switch (user_opt.is_trace_silent())
     {
        case true:
              trace_file << cur_file << ' ' << cur_line << ' '
                         << "WRITE " << name << " = " << *val << endl;
        break;
        case false:
           cout << cur_file << ' ' << cur_line << ' '
                << "WRITE " << name << " = " << *val << endl;
        break;
        default: assert (0);
     }
  }
/* -------------------------------------------------------------------------- */
  void Tracer::WriteArrPrint(string name,size3 index,Value *val,
                                                 string cur_file,size3 cur_line)
  {
     switch (user_opt.is_trace_silent())
     {
        case true:
              trace_file << cur_file << ' ' << cur_line << ' '
                         << "WRITE " << name
                         << "[" << index << "]" << " = " << *val << endl;
        break;
        case false:
           cout << cur_file << ' ' << cur_line << ' '
                << "WRITE "  << name
                << "[" << index << "]" << " = " << *val << endl;
        break;
        default: assert (0);
     }
  }
/* -------------------------------------------------------------------------- */
  void Tracer::WriteVarPrint(string name,Value const &val,string cur_file,
                                                                 size3 cur_line)
  {
     switch (user_opt.is_trace_silent())
     {
        case true:
              trace_file << cur_file << ' ' << cur_line << ' '
                         << "WRITE " << name << " = " << val << endl;
        break;
        case false:
           cout << cur_file << ' ' << cur_line << ' '
                << "WRITE " << name << " = " << val << endl;
        break;
        default: assert (0);
     }
  }
/* -------------------------------------------------------------------------- */
  void Tracer::WriteArrPrint(string name,size3 index,Value const &val,
                                                 string cur_file,size3 cur_line)
  {
     switch (user_opt.is_trace_silent())
     {
        case true:
              trace_file << cur_file << ' ' << cur_line << ' '
                         << "WRITE " << name
                         << "[" << index << "]" << " = " << val << endl;
        break;
        case false:
           cout << cur_file << ' ' << cur_line << ' '
                << "WRITE "  << name
                << "[" << index << "]" << " = " << val << endl;
        break;
        default: assert (0);
     }
  }
/* -------------------------------------------------------------------------- */
  void Tracer::WriteTreeTracing(StatementKind kind,string cur_file,
                                                                 size3 cur_line)
  {
     switch (user_opt.is_trace_silent())
     {
        case true:
              trace_file << cur_file << ' ' << cur_line << ' '
                         << StmtKindNames[StatementKind(kind)] << endl;
        break;
        case false:
           cout << cur_file << ' ' << cur_line << ' '
                << StmtKindNames[StatementKind(kind)] << endl;
        break;
        default: assert (0);
     }
  }
/* -------------------------------------------------------------------------- */
  void
  Tracer::WriteTreeTracing(StatementKind skind,ProcKind pkind,string cur_file,
                                                                 size3 cur_line)
  {
     switch (user_opt.is_trace_silent())
     {
        case true:
              trace_file << cur_file << ' ' << cur_line << ' '
                         << StmtKindNames[StatementKind(skind)]
                         << " Calling: "
                         << ProcKindNames[ProcKind(pkind)] << endl;
        break;
        case false:
           cout << cur_file << ' ' << cur_line << ' '
                << StmtKindNames[StatementKind(skind)]
                << " Calling: "
                << ProcKindNames[ProcKind(pkind)] << endl;
        break;
        default: assert (0);
     }
  }
/* -------------------------------------------------------------------------- */
#endif // DISABLE_TRACING
