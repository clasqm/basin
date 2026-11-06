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

#ifndef ERROR_HANDLER_H_INCLUDED
#define ERROR_HANDLER_H_INCLUDED

#include "common.h"
/* -------------------------------------------------------------------------- */
   void error(errors error_type);
   void error(errors error_type,string s);
   void error (errors error_type,_char_t c);
   // TODO Make new special massive of token
   // names for user interface only.
   void error (parser_TokenType token);

   void runtime_error(errors error_type,string cur_file,line_count_t cur_line);
   void runtime_error(errors error_type,string s,string cur_file,
                                                         line_count_t cur_line);
   void runtime_error (errors error_type,_char_t c,string cur_file,
                                                         line_count_t cur_line);
   // TODO Make new special massive of token
   // names for user interface only.
   void runtime_error (parser_TokenType token,string cur_file,
                                                         line_count_t cur_line);
/* -------------------------------------------------------------------------- */

#endif // ERROR_HANDLER_H_INCLUDED
