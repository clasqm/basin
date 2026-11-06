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

/* -------------------------------------------------------------------------- */
#include <cstdlib>
         // using exit()
#include "error_handler.h"
#include "pool.h"
/* -------------------------------------------------------------------------- */
  using std::cerr;
  using std::endl;
  using std::string;
/* -------------------------------------------------------------------------- */
extern void stmt_PrintTree (stmt_List_t *list);
extern stmt_List_t *stmt_list;
extern string parser_CurFile;
extern count  parser_CurLine;
/* -------------------------------------------------------------------------- */
   void error (errors error_type)
   {
       cerr << endl
            << "file: " << ::parser_CurFile
            << " line: " << ::parser_CurLine
            << " error: "<< error_strings[errors(error_type)]
            << endl;
#ifndef DISABLE_TRACING
     if (user_opt.is_tracing_tree())
        stmt_PrintTree (stmt_list);
#endif // DISABLE_TRACING
       pool.freall();
       exit(errors(error_type));
   }
/* -------------------------------------------------------------------------- */
   void error (errors error_type,string s)
   {
       cerr << endl
            <<  "file: " << ::parser_CurFile
            << " line: " << ::parser_CurLine
            << " error: "<< error_strings[errors(error_type)]
            << s
            << endl;
#ifndef DISABLE_TRACING
     if (user_opt.is_tracing_tree())
        stmt_PrintTree (stmt_list);
#endif // DISABLE_TRACING
       pool.freall();
       exit(errors(error_type));
   }
/* -------------------------------------------------------------------------- */
   void error (errors error_type,_char_t c)
   {
       cerr << endl
            <<  "file: " << ::parser_CurFile
            << " line: " << ::parser_CurLine
            << " error: "<< error_strings[errors(error_type)]
            << c
            << endl;
#ifndef DISABLE_TRACING
     if (user_opt.is_tracing_tree())
        stmt_PrintTree (stmt_list);
#endif // DISABLE_TRACING
       pool.freall();
       exit(errors(error_type));
   }
/* -------------------------------------------------------------------------- */
   void error (parser_TokenType token)
   {
       cerr << std::endl
            << "file: "  << ::parser_CurFile
            << " line: " << ::parser_CurLine
            << " error: "<< parser_TokenTypeNames[parser_TokenType(token)]
            << " expected."
            << endl;
#ifndef DISABLE_TRACING
     if (user_opt.is_tracing_tree())
        stmt_PrintTree (stmt_list);
#endif // DISABLE_TRACING
       pool.freall();
       exit(EXIT_FAILURE);
   }
/* -------------------------------------------------------------------------- */
   void runtime_error(errors error_type,string cur_file,line_count_t cur_line)
   {
       cerr << std::endl
            << "file: "  << cur_file
            << " line: " << cur_line
            << " error: "<< error_strings[errors(error_type)]
            << endl;
       pool.freall();
       exit(errors(error_type));
   }
/* -------------------------------------------------------------------------- */
   void runtime_error(errors error_type,string s,string cur_file,
                                                          line_count_t cur_line)
   {
       cerr << std::endl
            <<  "file: " << cur_file
            << " line: " << cur_line
            << " error: "<< error_strings[errors(error_type)]
            << s
            << endl;
       pool.freall();
       exit(errors(error_type));
   }
/* -------------------------------------------------------------------------- */
   void runtime_error (errors error_type,_char_t c,string cur_file,
                                                          line_count_t cur_line)
   {
       cerr << std::endl
            <<  "file: " << cur_file
            << " line: " << cur_line
            << " error: "<< error_strings[errors(error_type)]
            << c
            << endl;
       pool.freall();
       exit(errors(error_type));
   }
/* -------------------------------------------------------------------------- */
   void runtime_error (parser_TokenType token,string cur_file,
                                                          line_count_t cur_line)
   {
       cerr << std::endl
            << "file: "  << cur_file
            << " line: " << cur_line
            << " error: "<< parser_TokenTypeNames[parser_TokenType(token)]
            << " expected."
            << endl;
       pool.freall();
       exit(EXIT_FAILURE);
   }
/* -------------------------------------------------------------------------- */
