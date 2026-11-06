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

#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <string>
  using std::string;
#include <vector>
  using std::vector;
/* -------------------------------------------------------------------------- */
   enum parser_TokenType
   {
#define  PARSER_DEF_TOKEN_KIND(p1,p2) p1,
#include "defines/parser1.def"
#undef   PARSER_DEF_TOKEN_KIND
#define  PARSER_DEF_TOKEN_KIND(p1,p2) p1
#include "defines/parser2.def"
#undef   PARSER_DEF_TOKEN_KIND
   };
/* -------------------------------------------------------------------------- */
   // For debug printing only
   const std::string parser_TokenTypeNames[] =
   {
#define  PARSER_DEF_TOKEN_KIND(p1,p2) p2,
#include "defines/parser1.def"
#undef   PARSER_DEF_TOKEN_KIND
#define  PARSER_DEF_TOKEN_KIND(p1,p2) p2
#include "defines/parser2.def"
#undef   PARSER_DEF_TOKEN_KIND
   };
/* -------------------------------------------------------------------------- */
   // Contain all types of
   // constants,variables
   // and functions.
   enum Types {                          // Тип константы,переменной,или функции
#define  TYPE_KIND_DEF(p1,p2) p1,
#include "defines/types1.def"
#undef   TYPE_KIND_DEF
#define  TYPE_KIND_DEF(p1,p2) p1
#include "defines/types2.def"
#undef   TYPE_KIND_DEF
   };
/* -------------------------------------------------------------------------- */
   // For debug printing only
   const std::string TypesNames   [] = //Тип константы,переменной,или функции
   {
#define  TYPE_KIND_DEF(p1,p2) p2,
#include "defines/types1.def"
#undef   TYPE_KIND_DEF
#define  TYPE_KIND_DEF(p1,p2) p2
#include "defines/types2.def"
#undef   TYPE_KIND_DEF
   };
/* -------------------------------------------------------------------------- */
  enum StatementKind
  {
#define  STMT_KIND_DEF(p1,p2,p3) p1,
#include "defines/statement1.def"
#undef   STMT_KIND_DEF
#define  STMT_KIND_DEF(p1,p2,p3) p1
#include "defines/statement2.def"
#undef   STMT_KIND_DEF
  };
/* -------------------------------------------------------------------------- */
  const std::string StmtKindNames [] =
  {
#define  STMT_KIND_DEF(p1,p2,p3) p3,
#include "defines/statement1.def"
#undef   STMT_KIND_DEF
#define  STMT_KIND_DEF(p1,p2,p3) p3
#include "defines/statement2.def"
#undef   STMT_KIND_DEF
  };
/* -------------------------------------------------------------------------- */
  enum ProcKind
  {
#define EXPR_PROC_DEF(p1,p2,p3) p1,
#include "defines/expression1.def"
#undef EXPR_PROC_DEF
#define EXPR_PROC_DEF(p1,p2,p3) p1
#include "defines/expression2.def"
#undef EXPR_PROC_DEF
  };
/* -------------------------------------------------------------------------- */
  const std::string ProcKindNames [] =
  {
#define EXPR_PROC_DEF(p1,p2,p3) p3,
#include "defines/expression1.def"
#undef EXPR_PROC_DEF
#define EXPR_PROC_DEF(p1,p2,p3) p3
#include "defines/expression2.def"
#undef EXPR_PROC_DEF
  };
/* -------------------------------------------------------------------------- */
   // Common errors
   enum errors {
#define  ERRORS_DEF(p1,p2) p1,
#include "defines/errors1.def"
#undef   ERRORS_DEF
#define  ERRORS_DEF(p1,p2) p1
#include "defines/errors2.def"
#undef   ERRORS_DEF
   };
/* -------------------------------------------------------------------------- */
   // Common errors for printing
   const std::string error_strings [] = {
#define  ERRORS_DEF(p1,p2) p2,
#include "defines/errors1.def"
#undef   ERRORS_DEF
#define  ERRORS_DEF(p1,p2) p2
#include "defines/errors2.def"
#undef   ERRORS_DEF
   };
/* -------------------------------------------------------------------------- */
  // Operation type (binary,unary)
  enum OpKind
  {
      OP_PLUS,
      OP_MINUS,
      OP_MULTIPLY,
      OP_DIVIDE,
      OP_NOT,
      OP_OR,
      OP_XOR,
      OP_AND,
      OP_REL_LESS,
      OP_REL_LESS_EQUAL,
      OP_REL_GREAT,
      OP_REL_GREAT_EQUAL,
      OP_REL_OR,
      OP_REL_EQUAL,
      OP_REL_NOT_EQUAL,
      OP_REL_AND
  };
/* -------------------------------------------------------------------------- */
  const std::string OpKindNames [] =
  {
      "+  ",// "+"
      "-  ",// "-"
      "*  ",// "*"
      "/  ",// "/"
      "NOT",// "~"
      "OR ",// "|"
      "XOR",// "^"
      "AND",// "&"
      "<  ",// "<"
      "<= ",// "<="
      ">  ",// ">"
      ">= ",// ">="
      "=  ",// "="
      "<> ",// "<>"
      "|| ",// "||" // FIXME
      "&& " // "&&" // FIXME

  };
/* -------------------------------------------------------------------------- */
  // Тип узла дерева
  enum ExprKind
  {
      EK_CONST, // tree element is a constant
      EK_VAR,   // tree element is a variable
      EK_ARR_EL,// tree element is an array element
      EK_UNOP,  // tree element is an unary operation
      EK_BINOP, // tree element is a binary operation
      EK_PROC   // tree element is a procedure call
  };
/* -------------------------------------------------------------------------- */
  const std::string ExprKindNames [] =
  {
      "EK_CONST", // tree element is a constant
      "EK_VAR",   // tree element is a variable
      "EK_ARR_EL",// tree element is an array element
      "EK_UNOP",  // tree element is an unary operation
      "EK_BINOP", // tree element is a binary operation
      "EK_PROC"   // tree element is a procedure call
  };
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
   typedef unsigned int _uint32_t;
   /* ---------------------------- */
   typedef _char_t  size1;
   typedef _int_t   size2;
   typedef _long_t  size3;
   typedef _ulong_t size4;
   typedef size4  stmtId;
   typedef vector <_bool_t>         bool_Arr_t;
   typedef vector <string>          str_Arr_t;
   typedef vector <size3>           sz3_Arr_t;
   typedef vector <size4>           sz4_Arr_t;
   typedef vector <sz3_Arr_t *>     sz3_MDim_t;
   typedef vector <sz4_Arr_t *>     sz4_MDim_t;

   // Type will be used for some loops
   typedef _int_t count;

   // Type for counting symbols in program
   typedef size4 s_count;
   typedef size4 line_count_t;
/* -------------------------------------------------------------------------- */
  typedef vector <class Variable*> var_Arr_t;
  typedef vector <var_Arr_t *>     var_Stack_t;
  // Single dimension array of Values
  typedef vector <class Value *> val_Arr_t;
  // Multidimensional array of Values
  typedef vector <val_Arr_t   *> val_Stack_t;
  typedef vector <val_Stack_t *> val_MStack_t;
  // Arrays values multi stack (used in pool)
  typedef vector <val_MStack_t *> val_VMMS_t;
/* -------------------------------------------------------------------------- */
  typedef vector <class Array *> arr_Arr_t;
  typedef vector <arr_Arr_t *>   arr_Stack_t;
/* -------------------------------------------------------------------------- */
  typedef class Expression expr_Node_t;
  typedef vector <expr_Node_t *> expr_List_t;
/* -------------------------------------------------------------------------- */
  typedef class Statement stmt_Node_t;
  typedef vector <stmt_Node_t *> stmt_List_t;
/* -------------------------------------------------------------------------- */
  typedef class Label label_Named_t;
  typedef vector <label_Named_t *> lab_Arr_t;
  typedef vector <lab_Arr_t *>     lab_Stack_t;
/* -------------------------------------------------------------------------- */
  typedef vector <class Subroutine *> sub_Arr_t;
  typedef vector <sub_Arr_t *>  sub_Stack_t;
/* -------------------------------------------------------------------------- */
#define scast static_cast
/* -------------------------------------------------------------------------- */
#endif // TYPES_H_INCLUDED
