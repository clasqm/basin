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

#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <streambuf>

#include "common.h"
   using std::string;
   using std::map;
   using std::streambuf;
   using std::endl;
   using std::cout;

#include "main.h"
/* -------------------------------------------------------------------------- */
  /** @brief Parser initialization.
   *  @param file_name contains path
   *  to file with source code,wich parser is working with.
   *  @return In case of problem with lile opening return false,
   *  and true if file was opened */
   void parser_Init (const _char_t *file_name);

  /** @brief Procedure parser_GetToken get's the next token from
   *  input stream.
   *  @param As a result writing variables parser_CurToken and
   *  parser_LastTokenStr.
   *  @return Procedure parser_GetToken return value of
   *  parser_CurToken. */
   parser_TokenType parser_GetToken ();
   parser_TokenType parser_CheckNumInStr (string str);

   void parser_GetBack(s_count how_much_times);
  /** @param token_expected is expected token from parser.
   *  @param take_token is saying to use parser_GetToken().
   *  @return if @param do not match next token,program exit.*/
   void parser_Expected(parser_TokenType token_expected,_bool_t take_token);
  /** @var Variables parser_CurFile, parser_CurLine contain
   *  information about place of current token in source
   *  code for debug purposes,parser_CurToken is the current
   *  token in line.parser_CurTokenStr contains value of
   *  token,if available. */
   extern std::string      parser_CurFile;
   extern count            parser_CurLine;
   extern parser_TokenType parser_CurToken;
   extern std::string      parser_CurTokenStr;
   extern s_count          parser_CurSymbol;

   extern std::map <const std::string,s_count> parser_GotoCases;

  /** @brief End of parser's work */
   void parser_Finish ();
/* -------------------------------------------------------------------------- */

#endif // PARSER_H_INCLUDED
