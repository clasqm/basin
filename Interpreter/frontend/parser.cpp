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

#include <limits>
   using std::numeric_limits;
#include "parser.h"
#include "options.h"
#include "error_handler.h"
/* -------------------------------------------------------------------------- */
  /** @var Variables parser_CurFile, parser_CurLine contain
   *  information about place of current token in source
   *  code for debug purposes,parser_CurToken is the current
   *  token in line.parser_CurTokenStr contains value of
   *  token,if available. */
   parser_TokenType                parser_CurToken;
   parser_TokenType                parser_NextToken;

   string                          parser_CurTokenStr;
   string                          parser_CurFile;

   count                           parser_CurLine;
   s_count                         parser_CurSymbol;
  /** @var parser_ResWords contain reserved words
   *  for parser.Initialization is in
   *  parser_Init (std::string file_name).parser_ResType
   *  contain reserved types names as "int","float" etc.
   *  parser_ResSign is for signs like '*', '/' etc.*/
   map <const  string,_int_t>          parser_ResType;
   map <const _char_t,_int_t>          parser_ResTSpc;
   map <const  string,_int_t>          parser_ResWord;
   map <const _char_t,_int_t>          parser_ResSign;

/* -------------------------------------------------------------------------- */
#ifndef DISABLE_TRACING
  /** @brief Printer for debugging purposes
   *  of current token state. */
   void parser_Printer (const std::string str);
   void parser_Printer ();
#endif // DISABLE_TRACING
/* -------------------------------------------------------------------------- */
   void parser_Init (const _char_t * file_name)
   {   // Reserved words - Keywords   //////////////////////////////////////////
       parser_ResWord[ "LET"  ] = parser_TokenType(TOKEN_KW_LET);
       parser_ResWord["PRINT" ] = parser_TokenType(TOKEN_KW_PRINT);
       parser_ResWord["INPUT" ] = parser_TokenType(TOKEN_KW_INPUT);
       parser_ResWord[ "DIM"  ] = parser_TokenType(TOKEN_KW_DIM);
       parser_ResWord["SHARED"] = parser_TokenType(TOKEN_KW_SHARED);
       parser_ResWord[  "AS"  ] = parser_TokenType(TOKEN_KW_AS);
       parser_ResWord[  "TO"  ] = parser_TokenType(TOKEN_KW_TO);
       parser_ResWord["OPTION"] = parser_TokenType(TOKEN_KW_OPTION);
       parser_ResWord[ "BASE" ] = parser_TokenType(TOKEN_KW_BASE);
       parser_ResWord[  "IF"  ] = parser_TokenType(TOKEN_KW_IF);
       parser_ResWord[ "THEN" ] = parser_TokenType(TOKEN_KW_THEN);
       parser_ResWord[ "ELSE" ] = parser_TokenType(TOKEN_KW_ELSE);
       parser_ResWord["ELSEIF"] = parser_TokenType(TOKEN_KW_ELSEIF);
       parser_ResWord["WHILE" ] = parser_TokenType(TOKEN_KW_WHILE);
       parser_ResWord[ "WEND" ] = parser_TokenType(TOKEN_KW_WEND);
       parser_ResWord[ "FOR"  ] = parser_TokenType(TOKEN_KW_FOR);
       parser_ResWord[ "STEP" ] = parser_TokenType(TOKEN_KW_STEP);
       parser_ResWord[ "NEXT" ] = parser_TokenType(TOKEN_KW_NEXT);
       parser_ResWord[  "DO"  ] = parser_TokenType(TOKEN_KW_DO);
       parser_ResWord[ "UNTIL"] = parser_TokenType(TOKEN_KW_UNTIL);
       parser_ResWord[ "SUB"  ] = parser_TokenType(TOKEN_KW_SUB);
       parser_ResWord[ "CALL" ] = parser_TokenType(TOKEN_KW_CALL);
       parser_ResWord["BEGIN" ] = parser_TokenType(TOKEN_KW_BEGIN);
       parser_ResWord[ "END"  ] = parser_TokenType(TOKEN_KW_END);
       parser_ResWord[ "LOOP" ] = parser_TokenType(TOKEN_KW_LOOP);
       parser_ResWord["RETURN"] = parser_TokenType(TOKEN_KW_RETURN);
       parser_ResWord[ "GOTO" ] = parser_TokenType(TOKEN_KW_GOTO);
#ifndef DISABLE_TRACING
       parser_ResWord["TRACE" ] = parser_TokenType(TOKEN_KW_TRACE);
#endif // DISABLE_TRACING
#ifndef DISABLE_SDL_GRAPHICS
       parser_ResWord["SCREEN"] = parser_TokenType(TOKEN_KW_SCREEN);
       parser_ResWord["PRESET"] = parser_TokenType(TOKEN_KW_PRESET);
       parser_ResWord[ "LINE" ] = parser_TokenType(TOKEN_KW_LINE);
       parser_ResWord["CIRCLE"] = parser_TokenType(TOKEN_KW_CIRCLE);
#endif // DISABLE_SDL_GRAPHICS
       parser_ResWord[ "CLS"  ] = parser_TokenType(TOKEN_KW_CLS);
       parser_ResWord[ "BEEP" ] = parser_TokenType(TOKEN_KW_BEEP);
       parser_ResWord["CLEAR" ] = parser_TokenType(TOKEN_KW_CLEAR);
       parser_ResWord[ "SIN"  ] = parser_TokenType(TOKEN_KW_SIN);
       parser_ResWord[ "COS"  ] = parser_TokenType(TOKEN_KW_COS);
       parser_ResWord[ "TAN"  ] = parser_TokenType(TOKEN_KW_TAN);
       parser_ResWord[ "ATN"  ] = parser_TokenType(TOKEN_KW_ATN);
       parser_ResWord[ "VAL"  ] = parser_TokenType(TOKEN_KW_VAL);
       parser_ResWord[ "STR$" ] = parser_TokenType(TOKEN_KW_STR);
       parser_ResWord[ "LEN"  ] = parser_TokenType(TOKEN_KW_LEN);
       parser_ResWord[ "SQR"  ] = parser_TokenType(TOKEN_KW_SQR);
       parser_ResWord[ "ABS"  ] = parser_TokenType(TOKEN_KW_ABS);
       parser_ResWord["INSTR" ] = parser_TokenType(TOKEN_KW_INSTR);
       parser_ResWord[ "LEFT$"] = parser_TokenType(TOKEN_KW_LEFT);
       parser_ResWord["RIGHT$"] = parser_TokenType(TOKEN_KW_RIGHT);
       parser_ResWord[ "MID$" ] = parser_TokenType(TOKEN_KW_MID);
       parser_ResWord[ "INT"  ] = parser_TokenType(TOKEN_KW_INT);

       // Reserved words - Typenames  //////////////////////////////////////////
       parser_ResType["INTEGER"] = parser_TokenType(TOKEN_TYPE_INT);
       parser_ResType[ "LONG"  ] = parser_TokenType(TOKEN_TYPE_LINT);
       parser_ResType[ "SINGLE"] = parser_TokenType(TOKEN_TYPE_FLOAT);
       parser_ResType[ "DOUBLE"] = parser_TokenType(TOKEN_TYPE_DOUBLE);
       parser_ResType[ "STRING"] = parser_TokenType(TOKEN_TYPE_STRING);
       parser_ResType[ "VOID"  ] = parser_TokenType(TOKEN_TYPE_VOID);

       parser_ResTSpc[  '%'   ] = Types(INT);
       parser_ResTSpc[  '&'   ] = Types(LINT);
       parser_ResTSpc[  '!'   ] = Types(FLOAT);
       parser_ResTSpc[  '#'   ] = Types(DOUBLE);
       parser_ResTSpc[  '$'   ] = Types(STRING);

       // Reserved signs - Delimeters //////////////////////////////////////////
       parser_ResSign[  '='   ] = parser_TokenType(TOKEN_DELIM_EQUAL);
       parser_ResSign[  '+'   ] = parser_TokenType(TOKEN_DELIM_PLUS);
       parser_ResSign[  '-'   ] = parser_TokenType(TOKEN_DELIM_MINUS);
       parser_ResSign[  '*'   ] = parser_TokenType(TOKEN_DELIM_MULTIPLY);
       parser_ResSign[  '/'   ] = parser_TokenType(TOKEN_DELIM_DIVIDE);

       // Reserved signs - Binary operators ////////////////////////////////////
       parser_ResSign[  '&'   ] = parser_TokenType(TOKEN_BITWISE_AND);
       parser_ResSign[  '|'   ] = parser_TokenType(TOKEN_BITWISE_OR);
       parser_ResSign[  '^'   ] = parser_TokenType(TOKEN_BITWISE_XOR);
       parser_ResSign[  '~'   ] = parser_TokenType(TOKEN_BITWISE_NOT);

       // Reserved signs - Logical operators ///////////////////////////////////
       parser_ResSign[  '>'   ] = parser_TokenType(TOKEN_BOOL_GREATER);
       parser_ResSign[  '<'   ] = parser_TokenType(TOKEN_BOOL_LESS);

       // Reserved signs - Brackets ////////////////////////////////////////////
       parser_ResSign[  '('   ] = parser_TokenType(TOKEN_LEFT_PARENTH);
       parser_ResSign[  ')'   ] = parser_TokenType(TOKEN_RIGHT_PARENTH);
       parser_ResSign[  '{'   ] = parser_TokenType(TOKEN_LEFT_CBR);
       parser_ResSign[  '}'   ] = parser_TokenType(TOKEN_RIGHT_CBR);


       // Reserved signs - Punctuation marks ///////////////////////////////////
       parser_ResSign[  ','   ] = parser_TokenType(TOKEN_COMMA);
       parser_ResSign[  ':'   ] = parser_TokenType(TOKEN_COLON);
       parser_ResSign[  ';'   ] = parser_TokenType(TOKEN_DOTCOMMA);
       parser_ResSign[  '\n'  ] = parser_TokenType(TOKEN_EOL);
       parser_ResSign[  '\r'  ] = parser_TokenType(TOKEN_EOL);
       parser_ResSign[  ' '   ] = parser_TokenType(TOKEN_NULL);

       ::parser_CurToken = TOKEN_NULL;
       ::parser_CurTokenStr = "";
       ::parser_CurLine = 1;
       ::parser_CurFile = file_name;
       ::parser_CurSymbol = 1;
   }
/* -------------------------------------------------------------------------- */
   void parser_GetBack(s_count how_much_times)
   {
       for (s_count i = 0; i < how_much_times; ++i)
          source.unget();
   }
/* -------------------------------------------------------------------------- */
   parser_TokenType parser_GetToken ()
   {
       _char_t c = 0;
       ::parser_CurTokenStr = "";
       while (!source.eof())
       {
           source.get(c);
           ++::parser_CurSymbol;
           // Identificator or
           // reserved word
           if (isalpha(c))
           {
               while (!source.eof()&&isalnum(c))
               {
                  if (isalpha(c)) c = toupper(c);
                  ::parser_CurTokenStr.push_back(c);
                  source.get(c);
                  ++::parser_CurSymbol;
               }
               source.unget();
               --::parser_CurSymbol;

               map<const _char_t,_int_t>::iterator i = parser_ResTSpc.find(c);

               if (i != parser_ResTSpc.end())
               {
                  ::parser_CurTokenStr.push_back(c);
                  // For this case: STR$
                  map<const string,_int_t>::iterator it = parser_ResWord.find
                                                         (::parser_CurTokenStr);
                  if (it!=parser_ResWord.end())
                  {
                      ::parser_CurToken = parser_TokenType(parser_ResWord
                                                        [::parser_CurTokenStr]);
#ifndef DISABLE_TRACING
                     if (user_opt.GetTraceTokens())
                        user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
                      return ::parser_CurToken;
                  }

                  ::parser_CurToken = TOKEN_IDENT;
#ifndef DISABLE_TRACING
                   if (user_opt.GetTraceTokens())
                      user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
                   return ::parser_CurToken;
               }
               i = parser_ResSign.find(c);

               if (i != parser_ResSign.end())
                   ::parser_NextToken = parser_TokenType(parser_ResSign[c]);

               else error(UNEXPECTED_SYMBOL,c);

               map<const string,_int_t>::iterator ii = parser_ResWord.find
                                                         (::parser_CurTokenStr);
               if (ii!=parser_ResWord.end())
                   ::parser_CurToken = parser_TokenType(parser_ResWord
                                                        [::parser_CurTokenStr]);
               else
               {
                   map<const string,_int_t>::iterator iii = parser_ResType.find
                                                         (::parser_CurTokenStr);
                   if (iii!=parser_ResType.end())
                       ::parser_CurToken = parser_TokenType(parser_ResType
                                                        [::parser_CurTokenStr]);
                   else
                       ::parser_CurToken = TOKEN_IDENT;
               }
               if (::parser_NextToken == TOKEN_EOL)
               {
#ifndef DISABLE_TRACING
                   if (user_opt.GetTraceTokens())
                      user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
                   return ::parser_CurToken;
               }
#ifndef DISABLE_TRACING
               if (user_opt.GetTraceTokens())
                  user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
               return ::parser_CurToken;
           }
           else
           // Number
           if (isdigit(c))
           {
               while (!source.eof()&&isdigit(c))
               {
                    ::parser_CurTokenStr.push_back(c);
                    source.get(c);
                    ++::parser_CurSymbol;
               }
               source.unget();
               --::parser_CurSymbol;
               // Float
               if (c == '.')
               {
                    ::parser_CurTokenStr.push_back(c);
                    source.get(c);          // FIXME
                    source.get(c);          // Twice because of previous unget()
                    ++::parser_CurSymbol;

                    if (isdigit(c))
                        while (!source.eof()&&isdigit(c))
                        {
                             ::parser_CurTokenStr.push_back(c);
                             source.get(c);
                             ++::parser_CurSymbol;
                        }
                    else error(WRONG_FLOAT_CONSTANT);

                    source.unget();
                    --::parser_CurSymbol;

                    map<const _char_t,_int_t>::iterator i
                                                       = parser_ResSign.find(c);

                    if (i!=parser_ResSign.end())
                       ::parser_NextToken = parser_TokenType(parser_ResSign[c]);
                    else error(UNEXPECTED_SYMBOL,c);

                    ::parser_CurToken = TOKEN_CONST_FLOAT;
#ifndef DISABLE_TRACING
                     if (user_opt.GetTraceTokens())
                        user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
                    return ::parser_CurToken;
               }
               // Integer
               map<const _char_t,_int_t>::iterator i = parser_ResSign.find(c);

               if (i!=parser_ResSign.end())
                    ::parser_NextToken = parser_TokenType(parser_ResSign[c]);
               else error(UNEXPECTED_SYMBOL,c);

               ::parser_CurToken = TOKEN_CONST_INT;

               if (::parser_NextToken == TOKEN_EOL)
               {
#ifndef DISABLE_TRACING
                     if (user_opt.GetTraceTokens())
                        user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
                    return ::parser_CurToken;
               }
#ifndef DISABLE_TRACING
               if (user_opt.GetTraceTokens())
                  user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
               return ::parser_CurToken;
           }
           else
           // !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
           if (ispunct(c))
           {
               map<const _char_t,_int_t>::iterator i = parser_ResSign.find(c);

               if (i!=parser_ResSign.end())
               {
                   ::parser_CurToken = parser_TokenType(parser_ResSign[c]);
#ifndef DISABLE_TRACING
                   if (user_opt.GetTraceTokens())
                      user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
                   switch (::parser_CurToken)
                   {
                      case TOKEN_BOOL_GREATER: {
                         s_count pos = source.tellg();
                         _char_t ch;
                         source.get(ch);
                         if (ch == '=')
                            return ::parser_CurToken = TOKEN_BOOL_GREAT_EQUAL;
                         source.seekg(pos);
                      break;}
                      case TOKEN_BOOL_LESS:    {
                         s_count pos = source.tellg();
                         _char_t ch;
                         source.get(ch);
                         switch (ch)
                         {
                            case '=':
                               return ::parser_CurToken = TOKEN_BOOL_LESS_EQUAL;
                            case '>':
                               return ::parser_CurToken = TOKEN_BOOL_NOT_EQUAL;
                            default:
                               source.seekg(pos);
                         }
                      break;}
                      default: {}// Pass trough
                   }
                   return ::parser_CurToken;
               }
               else
               if (c == '"')
               {
                   source.get(c);
                   ++::parser_CurSymbol;

                   while (c != '"')
                   {
                       if (c == '\n')
                       {
                           error(QUOT_MARK_EXPECTED);
                           break;
                       }
                       ::parser_CurTokenStr.push_back(c);
                       source.get(c);
                       ++::parser_CurSymbol;
                   }
                   if (c == '\n')
                   {
                       error(QUOT_MARK_EXPECTED);
                       break;
                   }
                   ::parser_CurToken = TOKEN_CONST_STRING;
#ifndef DISABLE_TRACING
                   if (user_opt.GetTraceTokens())
                      user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
                   return ::parser_CurToken;
               }
               // Comment
               if (c == '\'')
               {
                   while (!source.eof()&&c!='\n')
                   {
                       source.get(c);
                       ++::parser_CurSymbol;
                   }

                   if (c == '\n')
                   {
                       ::parser_CurToken = TOKEN_EOL;
                       ::parser_NextToken = TOKEN_NULL;
                       ++::parser_CurLine;
#ifndef DISABLE_TRACING
                       if (user_opt.GetTraceTokens())
                          user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
                       return ::parser_CurToken;
                   }
                   else
                   {
                       ::parser_CurToken = TOKEN_EOF;
                       ::parser_NextToken = TOKEN_NULL;
#ifndef DISABLE_TRACING
                       if (user_opt.GetTraceTokens())
                          user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
                       return ::parser_CurToken;

                   }
#ifndef DISABLE_TRACING
                   if (user_opt.GetTraceTokens())
                      user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
                   // When on commented line
                   // we don't want to return
                   // from function
                   continue;
               }
               // ...
           }
           else
           // ' ' and '\n'
           if (isspace(c))
           {
               if (c == ' ') continue;
               else
               if (c == '\n')
               {
                   ::parser_CurToken = TOKEN_EOL;
                   ::parser_NextToken = TOKEN_NULL;
                   ++::parser_CurLine;
                   return ::parser_CurToken;
               }
               if (c == '\r')
               {
                  ::parser_CurToken = TOKEN_EOL;
                  ::parser_NextToken = TOKEN_NULL;
                  ++::parser_CurLine;
                  return ::parser_CurToken;
               }
#ifndef DISABLE_TRACING
               if (user_opt.GetTraceTokens())
                  user_opt.IsFuncDebug() ?
                                    parser_Printer ("parser_GetToken ()"):
                                    parser_Printer ();
#endif // DISABLE_TRACING
               continue;
           }
           else
           if (c == '\0') continue;

           else error(UNEXPECTED_SYMBOL,c);
       }
       ::parser_CurToken = TOKEN_EOF;
       ::parser_NextToken = TOKEN_NULL;
#ifndef DISABLE_TRACING
       if (user_opt.GetTraceTokens())
          user_opt.IsFuncDebug() ? parser_Printer ("parser_GetToken ()"):
                                   parser_Printer ();
#endif // DISABLE_TRACING
       return ::parser_CurToken;
   }
/* -------------------------------------------------------------------------- */
   parser_TokenType parser_CheckNumInStr (string str)
   {
       _size_t found = str.find('.');
       if (found == string::npos)
       {  // Not a number with floating point
          size3 tmp = atol(str.c_str());
          if (tmp <= numeric_limits<_int_t>::max() &&
              tmp >= numeric_limits<_int_t>::min()   )
             return TOKEN_CONST_INT;
          return TOKEN_CONST_LINT;
       }
       _float_t tmp = atof(str.c_str());
       if (tmp <= numeric_limits<_float_t>::max() &&
           tmp >= numeric_limits<_float_t>::min()   )
          return TOKEN_CONST_FLOAT;
       return TOKEN_CONST_DOUBLE;
   }
/* -------------------------------------------------------------------------- */
  void parser_Expected(parser_TokenType token_expected,_bool_t take_token)
  {
     switch (take_token)
     {
        case true:  if (parser_GetToken() != token_expected)
                       error(token_expected);
        break;
        case false: if (parser_CurToken != token_expected)
                       error(token_expected);
        break;
        default: assert (0);
     }
  }
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_TRACING
   void parser_Printer (const std::string str)
   {
      if (user_opt.is_trace_silent())
      {
         trace_file << "File: "        << ::parser_CurFile               << ' '
                    << "Line: "        << ::parser_CurLine               << ' '
                    << "Now in: "      << str                            << ' '
                    << "CurToken: "
                    << parser_TokenTypeNames
                              [parser_TokenType(::parser_CurToken)]      << '\t'
                    << "NextToken: "
                    << parser_TokenTypeNames
                              [parser_TokenType(::parser_NextToken)]     << '\t'
                    << "TokenStr: "    << ::parser_CurTokenStr           << ' '
                    << endl;
         return;
      }
      cout << "File: "        <<                   ::parser_CurFile      << ' '
           << "Line: "        <<                   ::parser_CurLine      << ' '
           << "Now in: "      <<                   str                   << ' '
           << "CurToken: "    << parser_TokenTypeNames
                                  [parser_TokenType(::parser_CurToken)]  << '\t'
           << "NextToken: "   << parser_TokenTypeNames
                                  [parser_TokenType(::parser_NextToken)] << '\t'
           << "TokenStr: "    <<                   ::parser_CurTokenStr  << ' '
           << endl;
   }
/* -------------------------------------------------------------------------- */
   void parser_Printer ()
   {
      if (user_opt.is_trace_silent())
      {
         trace_file << "File: "        <<    ::parser_CurFile     << ' '
                    << "Line: "        <<    ::parser_CurLine     << ' '
                    << "CurToken: "
                    << parser_TokenTypeNames
                              [parser_TokenType(::parser_CurToken)]      << '\t'
                    << "NextToken: "
                    << parser_TokenTypeNames
                              [parser_TokenType(::parser_NextToken)]     << '\t'
                    << "TokenStr: "    <<    ::parser_CurTokenStr << ' '
                    << endl;
         return;
      }
      cout << "File: "        <<                   ::parser_CurFile     << ' '
           << "Line: "        <<                   ::parser_CurLine     << ' '
           << "CurToken: "    << parser_TokenTypeNames
                                  [parser_TokenType(::parser_CurToken)]  << '\t'
           << "NextToken: "   << parser_TokenTypeNames
                                  [parser_TokenType(::parser_NextToken)] << '\t'
           << "TokenStr: "    <<                   ::parser_CurTokenStr << ' '
           << endl;
   }
#endif // DISABLE_TRACING
/* -------------------------------------------------------------------------- */
