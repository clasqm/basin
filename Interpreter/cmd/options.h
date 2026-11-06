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
#ifndef OPTIONS_H_INCLUDED
#define OPTIONS_H_INCLUDED

#include <cstring>
#include "common.h"
   using std::ofstream;
   using std::map;
   using std::cout;
   using std::endl;
   using std::cerr;
   using std::string;
/* -------------------------------------------------------------------------- */
  extern map <string,_bool_t> cmd_options;
/* -------------------------------------------------------------------------- */
  class Options { // Class will be used to control user options in code.
     private: size3 array_base;
#ifndef DISABLE_TRACING
        _bool_t trace_actions;
        _bool_t trace_tree;
        _bool_t trace_silent;
        _bool_t trace_tokens;
        _bool_t debug_functions;
#endif // DISABLE_TRACING
     public:
        Options();
        ~Options();
        size3 GetBase () const {return array_base;};
        void SetBase(size3 n){array_base = n;};
        _bool_t check(const string str) {return cmd_options[str];};
#ifndef DISABLE_TRACING
        void Init();
        _bool_t GetTraceAct ()    const {return trace_actions;};
        _bool_t GetTraceTokens()  const {return trace_tokens;};
        _bool_t is_trace_silent() const {return trace_silent;};
        _bool_t is_tracing_tree() const {return trace_tree;};
        _bool_t IsFuncDebug ()    const {return debug_functions;}
        void SetTraceAct(_bool_t trace){trace_actions = trace;};
        void SetTraceTree(_bool_t trace){trace_tree = trace;};
        void WriteVarPrint(string name,Value *val,string cur_file,
                                                          size3 cur_line);
        void WriteArrPrint(string name,size3 index,Value *val,
                                          string cur_file,size3 cur_line);
        void WriteTreeTracing(StatementKind kind,string cur_file,
                                                          size3 cur_line);
        void WriteTreeTracing(StatementKind skind,ProcKind pkind,
                                                         string cur_file,
                                                          size3 cur_line);
#endif // DISABLE_TRACING
  };
/* -------------------------------------------------------------------------- */
  extern Options user_opt;
  void cmd_ParamsDefaultInit();
  int  cmd_ParamsCheck(char *const filename,const _int_t argc,
                                            const _char_t *const argv[]);
#ifndef DISABLE_TRACING
  extern ofstream trace_file;
#endif // DISABLE_TRACING
/* -------------------------------------------------------------------------- */
#endif // OPTIONS_H_INCLUDED
