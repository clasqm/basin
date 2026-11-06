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

#include "common.h"
  using std::cout;

#include "parser.h"
#include "label.h"
#include "value.h"
#include "array.h"
#include "expression.h"
#include "statement.h"
#include "subroutine.h"
#include "options.h"
#include "pool.h"
#include "tracer.h"
#include "syntax.h"
#include "lib.h"
#include "error_handler.h"
/* -------------------------------------------------------------------------- */
// TODO Remove all parameters from functions below.
//      All data should be found in pool.
/* -------------------------------------------------------------------------- */
 /** Sometimes need to read values,and not to make representation for them,but
  *  write them directly.Example may be DIM(-10 TO 20).Here minus is token
  *  itself,and we coud just use one functon only once to get needed
  *  value -10. */
  static Value syntax_runtimePrimary();
/* -------------------------------------------------------------------------- */
  static expr_Node_t *syntax_Primary (var_Arr_t *vars,
                                           arr_Arr_t *arrays,lab_Arr_t *labels);
  static expr_Node_t *syntax_Term (var_Arr_t *vars,
                                           arr_Arr_t *arrays,lab_Arr_t *labels);
  static expr_Node_t *syntax_Expr (var_Arr_t *vars,
                                           arr_Arr_t *arrays,lab_Arr_t *labels);
  static expr_Node_t *syntax_BitExpr (var_Arr_t *vars,
                                           arr_Arr_t *arrays,lab_Arr_t *labels);
  static expr_Node_t *syntax_RelExpr (var_Arr_t *vars,
                                           arr_Arr_t *arrays,lab_Arr_t *labels);
  static expr_Node_t *syntax_IdentRValue (var_Arr_t *vars,
                                          arr_Arr_t *arrays,lab_Arr_t *labels);
  static expr_Node_t *syntax_IdentRValueInput(var_Arr_t *vars,
                                           arr_Arr_t *arrays,lab_Arr_t *labels);
  static expr_Node_t *syntax_Procedure (var_Arr_t *vars,
                                           arr_Arr_t *arrays,lab_Arr_t *labels);
/* -------------------------------------------------------------------------- */
  static stmt_Node_t *syntax_stmtLET   (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels);
  static         void syntax_stmtPRINT (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels);
  static stmt_Node_t *syntax_stmtIF    (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels);
  static stmt_Node_t *syntax_stmtWHILE (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels);
  static stmt_Node_t *syntax_stmtFOR   (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels);
  static stmt_Node_t *syntax_stmtDO    (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels);
  static stmt_Node_t *syntax_CBranch   (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels);
  static         void syntax_Label     (stmt_List_t *list,lab_Arr_t *labels);
  static         void syntax_stmtGOTO  (stmt_List_t *list,lab_Arr_t *labels);
  static         void syntax_stmtDIM   (var_Arr_t *vars,arr_Arr_t *arrays);
  static         void syntax_stmtSUB   (stmt_List_t *list);
  static         void syntax_stmtCALL  (stmt_List_t *list);
#ifndef DISABLE_SDL_GRAPHICS
  static         void syntax_stmtSCREEN(stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels);
  static         void syntax_stmtPRESET(stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels);
  static         void syntax_stmtLINE  (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels);
  static         void syntax_stmtCIRCLE(stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels);
#endif // DISABLE_SDL_GRAPHICS
  static         void syntax_stmtCLS   (stmt_List_t *list);
  static         void syntax_stmtEND   (stmt_List_t *list);
  static         void syntax_stmtBEEP  (stmt_List_t *list);
  static         void syntax_stmtCLEAR (stmt_List_t *list);
 /** Function  will  check  desired block of statements for syntax errors and
  *  write these statements to vector sent as parameter.
  *  @param stmt_List_t *list - vector  to  write  statements,could be inside
  *         IF THEN ELSE,WHILE loop,FOR loop  and  any  statement or function
  *         that have local block.
  *  @param parser_TokenType end_token -  Token for syntax check of the block
  *         to be ended.                                                    */
  static void syntax_BlockCheck(stmt_List_t *list,parser_TokenType end_token,
                                var_Arr_t *vars,arr_Arr_t *arrays,
                                                             lab_Arr_t *labels);
  static void syntax_BlockCheck(stmt_List_t *list,parser_TokenType end_token1,
                                                  parser_TokenType end_token2,
                                                  var_Arr_t *vars,
                                                  arr_Arr_t *arrays,
                                                  lab_Arr_t *labels);
  static void syntax_BlockCheck(stmt_List_t *list,parser_TokenType end_token1,
                                                  parser_TokenType end_token2,
                                                  parser_TokenType end_token3,
                                                  var_Arr_t *vars,
                                                  arr_Arr_t *arrays,
                                                  lab_Arr_t *labels);
  static void syntax_LabelsMustBeValid ();
  static void syntax_userOPTION ();
/* -------------------------------------------------------------------------- */
  static Value syntax_runtimePrimary()
  {
     switch (parser_GetToken())
     {
        case TOKEN_CONST_INT: {
           Value int_x(::parser_CurTokenStr, INT);
           return int_x;
        }
        case TOKEN_CONST_FLOAT: {
           Value float_x(::parser_CurTokenStr, FLOAT);
           return float_x;
        }
        case TOKEN_DELIM_MINUS: {
           return -syntax_runtimePrimary();
        }
        default:error(WRONG_EXPRESSION);
     }
     // This is bad
     //это просто чтобе не получать предупр.компилятора
     Value null_x("0",INT);
     return null_x;
  }
/* -------------------------------------------------------------------------- */
  static expr_Node_t *syntax_Primary (var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     expr_Node_t *expr(NULL);
     switch (parser_GetToken())
     {
        case TOKEN_CONST_INT:   {
           expr = pool.alloc<expr_Node_t>(EK_CONST,&::parser_CurTokenStr,INT);
        break;}
        case TOKEN_CONST_FLOAT: {
           expr = pool.alloc<expr_Node_t>(EK_CONST,&::parser_CurTokenStr,FLOAT);
        break;}
        case TOKEN_CONST_STRING:{
           expr = pool.alloc<expr_Node_t>(EK_CONST,
                                                  &::parser_CurTokenStr,STRING);
        break;}
        case TOKEN_IDENT:       {
           expr = syntax_IdentRValue (vars,arrays,labels);
        break;}
        case TOKEN_DELIM_MINUS: {
           expr_Node_t *operand = syntax_Primary (vars,arrays,labels);
           expr = pool.alloc<expr_Node_t>(EK_UNOP,operand,OP_MINUS);
        break;}
        case TOKEN_BITWISE_NOT: {
           expr_Node_t *operand = syntax_BitExpr(vars,arrays,labels);
           expr = pool.alloc<expr_Node_t>(EK_UNOP,operand,OP_NOT);
        break;}
        case TOKEN_LEFT_PARENTH:{
           expr = syntax_RelExpr(vars,arrays,labels);
           if (::parser_CurToken != TOKEN_RIGHT_PARENTH)
                   error(RIGHTP_EXPECTED);
        break;}
        case TOKEN_KW_SIN:
        case TOKEN_KW_COS:
        case TOKEN_KW_TAN:
        case TOKEN_KW_ATN:
        case TOKEN_KW_VAL:
        case TOKEN_KW_STR:
        case TOKEN_KW_LEN:
        case TOKEN_KW_SQR:
        case TOKEN_KW_ABS:
        case TOKEN_KW_INSTR:
        case TOKEN_KW_LEFT:
        case TOKEN_KW_RIGHT:
        case TOKEN_KW_MID:
        case TOKEN_KW_INT:
           expr = syntax_Procedure (vars,arrays,labels);
        break;
        default:
             error(WRONG_EXPRESSION);
     }
     return expr;
  }
/* -------------------------------------------------------------------------- */
  static expr_Node_t *syntax_Term (var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     expr_Node_t *operand1;
     operand1 = syntax_Primary(vars,arrays,labels);
     while (true){
         switch (parser_GetToken())
         {
             case TOKEN_DELIM_MULTIPLY:{
                 expr_Node_t *operand2;
                 operand2 = syntax_Primary(vars,arrays,labels);
                 operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                                 operand1,operand2,OP_MULTIPLY);
             break;}
             case TOKEN_DELIM_DIVIDE:  {
                 expr_Node_t *operand2;
                 operand2 = syntax_Primary(vars,arrays,labels);
                 operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                                   operand1,operand2,OP_DIVIDE);
             break;}
             default:return operand1;
         }
     }
     return operand1;
  }
/* -------------------------------------------------------------------------- */
  static expr_Node_t *syntax_Expr (var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     expr_Node_t *operand1;
     operand1 =  syntax_Term (vars,arrays,labels);
     while (true){
         switch (::parser_CurToken)
         {
             case TOKEN_DELIM_PLUS: {
                 expr_Node_t *operand2;
                 operand2 = syntax_Term(vars,arrays,labels);
                 operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                                     operand1,operand2,OP_PLUS);
             break;}
             case TOKEN_DELIM_MINUS:{
                 expr_Node_t *operand2;
                 operand2 = syntax_Term(vars,arrays,labels);
                 operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                                    operand1,operand2,OP_MINUS);
             break;}
             default: return operand1;
         }
     }
     return operand1;
  }
/* -------------------------------------------------------------------------- */
  static expr_Node_t *syntax_BitExpr (var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     expr_Node_t *operand1;
     operand1 =  syntax_Expr (vars,arrays,labels);
     while (true){
         switch (::parser_CurToken)
         {
             case TOKEN_BITWISE_AND:{
                 expr_Node_t *operand2;
                 operand2 = syntax_Expr(vars,arrays,labels);
                 operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                                      operand1,operand2,OP_AND);
             break;}
             case TOKEN_BITWISE_XOR:{
                 expr_Node_t *operand2;
                 operand2 = syntax_Expr(vars,arrays,labels);
                 operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                                      operand1,operand2,OP_XOR);
             break;}
             case TOKEN_BITWISE_OR: {
                 expr_Node_t *operand2;
                 operand2 = syntax_Expr(vars,arrays,labels);
                 operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                                       operand1,operand2,OP_OR);
             break;}
             default: {
                 if (::parser_CurToken == TOKEN_RIGHT_PARENTH ||
                     ::parser_CurToken == TOKEN_BOOL_GREATER  ||
                     ::parser_CurToken == TOKEN_BOOL_LESS       )
                   return operand1;
                 return operand1;
             }
         }
     }
     return operand1;
  }
/* -------------------------------------------------------------------------- */
  static expr_Node_t *syntax_RelExpr (var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     expr_Node_t *operand1;
     operand1 = syntax_BitExpr (vars,arrays,labels);
     switch (::parser_CurToken)
     {
        case TOKEN_BOOL_GREATER: {
           expr_Node_t *operand2;
           operand2 = syntax_BitExpr(vars,arrays,labels);
           operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                                operand1,operand2,OP_REL_GREAT);
        break;}
        case TOKEN_BOOL_GREAT_EQUAL: {
           expr_Node_t *operand2;
           operand2 = syntax_BitExpr(vars,arrays,labels);
           operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                          operand1,operand2,OP_REL_GREAT_EQUAL);
        break;}
        case TOKEN_BOOL_LESS: {
           expr_Node_t *operand2;
           operand2 = syntax_BitExpr(vars,arrays,labels);
           operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                                 operand1,operand2,OP_REL_LESS);
        break;}
        case TOKEN_BOOL_LESS_EQUAL: {
           expr_Node_t *operand2;
           operand2 = syntax_BitExpr(vars,arrays,labels);
           operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                           operand1,operand2,OP_REL_LESS_EQUAL);
        break;}
        case TOKEN_DELIM_EQUAL: {
           expr_Node_t *operand2;
           operand2 = syntax_BitExpr(vars,arrays,labels);
           operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                                operand1,operand2,OP_REL_EQUAL);
        break;}
        case TOKEN_BOOL_NOT_EQUAL: {
           expr_Node_t *operand2;
           operand2 = syntax_BitExpr(vars,arrays,labels);
           operand1 = pool.alloc<expr_Node_t>(EK_BINOP,
                                            operand1,operand2,OP_REL_NOT_EQUAL);
        break;}
        default:return operand1;
     }
     return operand1;
  }
/* -------------------------------------------------------------------------- */
  static expr_Node_t *syntax_IdentRValue (var_Arr_t *vars,
                                          arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     size3 i = findVarName (::parser_CurTokenStr,vars);
     size3 j = findVarName (::parser_CurTokenStr,arrays);

     size3 k = findVarName (::parser_CurTokenStr, pool.GetSharedVars());
     size3 z = findVarName (::parser_CurTokenStr, pool.GetSharedArrs());

     if (i < 0 && j < 0 && k < 0 && z < 0)
     {
        if ( pool.isConflictingDeclaration ( ::parser_CurTokenStr,
                                             pool.SEARCH_SHARED_DATA_ONLY ) )
           error(CONFLICTING_DECLARATION,::parser_CurTokenStr);

        if (cmd_options["--noinit-errors"]) // Initialization control option
           error(UNDEFINED_SYMBOL,::parser_CurTokenStr);
        Types t;
        switch (::parser_CurTokenStr.at(::parser_CurTokenStr.size()-1))
        {
           case '%': t = INT;   break;
           case '&': t = LINT;  break;
           case '!': t = FLOAT; break;
           case '#': t = DOUBLE;break;
           case '$': t = STRING;break;
           default : t = INT;   break;
        }
        Variable *var = pool.alloc<Variable>(::parser_CurTokenStr);
        var->SetType(t);
        var->SetPage(vars);
        vars->push_back(var);
        return pool.alloc<expr_Node_t>(EK_VAR,var);
     }
     if (i >= 0)
        return pool.alloc<expr_Node_t>(EK_VAR,vars->at(i));
     if (k >= 0)
        return pool.alloc<expr_Node_t>(EK_VAR,pool.GetSharedVars()->at(k));
     if (z >= 0)
     {
        // Array of indexes of the element
        expr_List_t *ind = pool.alloc<expr_List_t>();
        parser_Expected(TOKEN_LEFT_PARENTH,true);
        s_count c = source.tellg();
        if (parser_GetToken() == TOKEN_COMMA) // This kind of error: array(,i)
           error(CONST_REQUIRED);
        source.seekg(c);
        while (::parser_CurToken != TOKEN_RIGHT_PARENTH)
        {
           expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
           ind->push_back(expr);
        }
        return pool.alloc<expr_Node_t>
                         (EK_ARR_EL,pool.GetSharedArrs()->at(z)->GetName(),ind);
     }
     // Array of indexes of the element
     expr_List_t *ind = pool.alloc<expr_List_t>();
     parser_Expected(TOKEN_LEFT_PARENTH,true);
     s_count c = source.tellg();
     if (parser_GetToken() == TOKEN_COMMA) // This kind of error: array(,i)
        error(CONST_REQUIRED);
     source.seekg(c);
     while (::parser_CurToken != TOKEN_RIGHT_PARENTH)
     {
        expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
        ind->push_back(expr);
     }
     return pool.alloc<expr_Node_t>(EK_ARR_EL,arrays->at(j)->GetName(),ind);
  }
/* -------------------------------------------------------------------------- */
  static expr_Node_t *syntax_IdentRValueInput (var_Arr_t *vars,
                                               arr_Arr_t *arrays,
                                               lab_Arr_t *labels)
  {
     s_count pos = source.tellg();
     string name = ::parser_CurTokenStr;
     size3 i = findVarName (name,vars);
     size3 j = findVarName (name,arrays);

     size3 k = findVarName (::parser_CurTokenStr, pool.GetSharedVars());
     size3 z = findVarName (::parser_CurTokenStr, pool.GetSharedArrs());

     if (i < 0 && j < 0 && k < 0 && z < 0)
     {
        if ( pool.isConflictingDeclaration (name,pool.SEARCH_SHARED_DATA_ONLY))
           error(CONFLICTING_DECLARATION,name);

        if (parser_GetToken() == TOKEN_LEFT_PARENTH)
           error(UNDEFINED_ARRAY,name);
        source.seekg(pos);
        Types t;
        switch (name.at(name.size()-1))
        {
           case '%': t = INT;   break;
           case '&': t = LINT;  break;
           case '!': t = FLOAT; break;
           case '#': t = DOUBLE;break;
           case '$': t = STRING;break;
           default : t = INT;   break;
        }
        Variable *var = pool.alloc<Variable>(name);
        var->SetType(t);
        var->SetPage(vars);
        vars->push_back(var);
        return pool.alloc<expr_Node_t>(EK_VAR,var);
     }
     if (i >= 0)
     {
        return pool.alloc<expr_Node_t>(EK_VAR,vars->at(i));
     }
     if (k >= 0)
        return pool.alloc<expr_Node_t>(EK_VAR,pool.GetSharedVars()->at(k));
     if (z >= 0)
     {
        // Array of indexes of the element
        expr_List_t *ind = pool.alloc<expr_List_t>();
        parser_Expected(TOKEN_LEFT_PARENTH,true);
        s_count c = source.tellg();
        if (parser_GetToken() == TOKEN_COMMA) // This kind of error: array(,i)
           error(CONST_REQUIRED);
        source.seekg(c);
        while (::parser_CurToken != TOKEN_RIGHT_PARENTH)
        {
           expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
           ind->push_back(expr);
        }
        return pool.alloc<expr_Node_t>(EK_ARR_EL,
                                    pool.GetSharedArrs()->at(z)->GetName(),ind);
     }
     // Array of indexes of the element
     expr_List_t *ind = pool.alloc<expr_List_t>();
     parser_Expected(TOKEN_LEFT_PARENTH,true);
     s_count c = source.tellg();
     if (parser_GetToken() == TOKEN_COMMA) // This kind of error: array(,i)
        error(CONST_REQUIRED);
     source.seekg(c);
     while (::parser_CurToken != TOKEN_RIGHT_PARENTH)
     {
        expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
        ind->push_back(expr);
     }
     return pool.alloc<expr_Node_t>(EK_ARR_EL,arrays->at(j)->GetName(),ind);
  }
/* -------------------------------------------------------------------------- */
  static expr_Node_t *syntax_Procedure (var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     ProcKind kind;
     switch (::parser_CurToken)
     {
        case TOKEN_KW_SIN:  kind = PK_SIN;  break;
        case TOKEN_KW_COS:  kind = PK_COS;  break;
        case TOKEN_KW_TAN:  kind = PK_TAN;  break;
        case TOKEN_KW_ATN:  kind = PK_ATN;  break;
        case TOKEN_KW_VAL:  kind = PK_VAL;  break;
        case TOKEN_KW_STR:  kind = PK_STR;  break;
        case TOKEN_KW_LEN:  kind = PK_LEN;  break;
        case TOKEN_KW_SQR:  kind = PK_SQR;  break;
        case TOKEN_KW_ABS:  kind = PK_ABS;  break;
        case TOKEN_KW_INSTR:kind = PK_INSTR;break;
        case TOKEN_KW_LEFT: kind = PK_LEFT; break;
        case TOKEN_KW_RIGHT:kind = PK_RIGHT;break;
        case TOKEN_KW_MID:  kind = PK_MID;  break;
        case TOKEN_KW_INT:  kind = PK_INT;  break;
        default:assert(0);
     }
     expr_List_t *params = pool.alloc<expr_List_t>();// Array of parameters
     parser_Expected(TOKEN_LEFT_PARENTH,true);
     s_count c = source.tellg();
     if (parser_GetToken() == TOKEN_COMMA) // This kind of error: func(,i)
        error(CONST_REQUIRED);
     source.seekg(c);
     while (::parser_CurToken != TOKEN_RIGHT_PARENTH)
     {
        expr_Node_t *parameter = syntax_RelExpr(vars,arrays,labels);
        params->push_back(parameter);
     }
     size_t size = params->size();
     if (size == 0                                                   ||
         ((kind == PK_INSTR || kind == PK_LEFT || kind == PK_RIGHT)
                                                       && size < 2 ) ||
          (kind == PK_MID && size < 3 )                                )
        error(TOO_FEW_PARAMETERS);

     if (((kind != PK_INSTR && kind != PK_LEFT &&
           kind != PK_RIGHT && kind != PK_MID && kind != PK_INT) && size > 1) ||
         ((kind == PK_INSTR ||kind == PK_MID)&& size > 3)   ||
         ((kind == PK_LEFT || kind == PK_RIGHT) && size > 2)           )
        error(TOO_MUCH_PARAMETERS);
     c = source.tellg();
     if (  parser_GetToken() != TOKEN_DOTCOMMA)
        source.seekg(c);
     return pool.alloc<expr_Node_t>(EK_PROC,kind,params);
  }
/* -------------------------------------------------------------------------- */
  static stmt_Node_t *syntax_stmtLET (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     if (::parser_CurToken == TOKEN_IDENT) ;
     else if (parser_GetToken() != TOKEN_IDENT)
        error(IDENTIFIER_EXPECTED);
     stmt_Node_t *stmt = pool.alloc<stmt_Node_t>();;
     std::string name = ::parser_CurTokenStr;
     size3 i = findVarName (name,vars);
     size3 j = findVarName (name,arrays);
     size3 k = findVarName (name, pool.GetSharedVars());
     size3 z = findVarName (name, pool.GetSharedArrs());
     /// ///////////////////////////////////////////////////////////////////////
     /// THIS IS SEEMS NOT WORKING!
     if ( pool.isConflictingDeclaration ( name, pool.SEARCH_SHARED_DATA_ONLY) )
        error(CONFLICTING_DECLARATION,name);
     /// ///////////////////////////////////////////////////////////////////////
     if (i < 0 && j < 0 && k < 0 && z < 0)
     {// This can't be array ((j<0)-Was not defined with DIM)
      // Creating Variable.
        if (parser_GetToken() != TOKEN_DELIM_EQUAL)
           error(DELIMETER_EXPECTED);
        expr_Node_t *right = syntax_RelExpr (vars,arrays,labels);
        Variable *var = pool.alloc<Variable>(name);
        var->SetPage(vars);
        vars->push_back(var);
        return stmt->NewStmtLET (vars->back(),right,list);
     }
     if (j >= 0)
     {// In case of array element (array already defined with DIM)
        expr_List_t *indexes = pool.alloc<expr_List_t>();
        switch (parser_GetToken())
        {  // Now following list of indexes,separated by commas,or single index
           case TOKEN_LEFT_PARENTH:{
              s_count c = source.tellg();          // Remember pointer position
              if (parser_GetToken() == TOKEN_COMMA)// This kind of error:
                 error(CONST_REQUIRED);            // array(,i)
              source.seekg(c);                     // Return pointer back
              while (::parser_CurToken != TOKEN_RIGHT_PARENTH)
              {
                 expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
                 indexes->push_back(expr);
                 // FIXME Add check for symbols between expressions
              }
           break;}
           default:error(LEFTP_EXPECTED);
        }
        parser_Expected(TOKEN_DELIM_EQUAL,true);
        expr_Node_t *right = syntax_RelExpr(vars,arrays,labels);
        return stmt->NewStmtLET (arrays->at(j),indexes,right,list);
     }
     if (z >= 0)
     {// In case of array element (array already defined with DIM)
        expr_List_t *indexes = pool.alloc<expr_List_t>();
        switch (parser_GetToken())
        {  // Now following list of indexes,separated by commas,or single index
           case TOKEN_LEFT_PARENTH:{
              s_count c = source.tellg();          // Remember pointer position
              if (parser_GetToken() == TOKEN_COMMA)// This kind of error:
                 error(CONST_REQUIRED);            // array(,i)
              source.seekg(c);                     // Return pointer back
              while (::parser_CurToken != TOKEN_RIGHT_PARENTH)
              {
                 expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
                 indexes->push_back(expr);
                 // FIXME Add check for symbols between expressions
              }
           break;}
           default:error(LEFTP_EXPECTED);
        }
        parser_Expected(TOKEN_DELIM_EQUAL,true);
        expr_Node_t *right = syntax_RelExpr(vars,arrays,labels);
        return stmt->NewStmtLET(pool.GetSharedArrs()->at(z),indexes,right,list);
     }
     if (k >= 0)
     {
        parser_Expected(TOKEN_DELIM_EQUAL,true);
        expr_Node_t *right = syntax_RelExpr (vars,arrays,labels);
        return stmt->NewStmtLET (pool.GetSharedVars()->at(k),right,list);
     }
     parser_Expected(TOKEN_DELIM_EQUAL,true);
     expr_Node_t *right = syntax_RelExpr (vars,arrays,labels);
     return stmt->NewStmtLET (vars->at(i),right,list);
  }
/* -------------------------------------------------------------------------- */
  static void syntax_stmtPRINT (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     // FIXME shared vars(?)
     s_count i = source.tellg();
     stmt_Node_t *stmt = pool.alloc<stmt_Node_t>();
     if (parser_GetToken() == TOKEN_EOL || ::parser_CurToken == TOKEN_COLON)
     {  // This is for empty PRINT statement.Interpreter will check vector.
        expr_List_t *print_expr_list = pool.alloc<expr_List_t>();
        stmt->NewStmtCALL (PK_PRINT,print_expr_list,list);
        return;
     }
     source.seekg(i);
     expr_List_t *print_expr_list = pool.alloc<expr_List_t>();
     expr_Node_t *expr;
     for ( ; ::parser_CurToken == TOKEN_COMMA ||
                       ::parser_CurToken != TOKEN_EOL     ; )
     {
        expr = syntax_RelExpr (vars,arrays,labels);
        print_expr_list->push_back(expr);
        if (::parser_CurToken == TOKEN_IDENT       ||
            ::parser_CurToken == TOKEN_CONST_STRING||
            ::parser_CurToken == TOKEN_DELIM_EQUAL   )
        // FIXME if you want to add labels with letter names,not just numbers
           error(UNEXPECTED_SYMBOL,::parser_CurTokenStr);
        if (::parser_CurToken == TOKEN_EOL       ||
            ::parser_CurToken == TOKEN_EOF       ||
            ::parser_CurToken == TOKEN_KW_ELSE   ||
            ::parser_CurToken == TOKEN_KW_WEND   ||
            ::parser_CurToken == TOKEN_CONST_INT ||
            ::parser_CurToken == TOKEN_COLON       )  break;
        if (::parser_CurToken == TOKEN_COMMA ||
            ::parser_CurToken == TOKEN_DOTCOMMA)      continue;
        parser_GetToken();
     }
     stmt->NewStmtCALL (PK_PRINT,print_expr_list,list);
  }
/* -------------------------------------------------------------------------- */
  static stmt_Node_t *syntax_CBranch (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     switch (::parser_CurToken)
     {
        case TOKEN_KW_IF:    return syntax_stmtIF    (list,vars,arrays,labels);
        case TOKEN_KW_WHILE: return syntax_stmtWHILE (list,vars,arrays,labels);
        case TOKEN_KW_FOR:   return syntax_stmtFOR   (list,vars,arrays,labels);
        case TOKEN_KW_DO:    return syntax_stmtDO    (list,vars,arrays,labels);
        default: assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
  static void syntax_stmtINPUT (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     s_count pos = source.tellg();
     stmt_Node_t *stmt     = pool.alloc<stmt_Node_t>();
     expr_List_t *var_list = pool.alloc<expr_List_t>();
     // This parameter tell us to print '?' or not as it's done in QBASIC.
     _bool_t *dot_comma = NULL;
     string *hint;
     if (parser_GetToken() == TOKEN_CONST_STRING)
     {
        hint = pool.alloc<std::string>(::parser_CurTokenStr);
        if (parser_GetToken() == TOKEN_COMMA)
           dot_comma = pool.alloc<_bool_t>(false);
        else
        if (::parser_CurToken == TOKEN_DOTCOMMA)
           dot_comma = pool.alloc<_bool_t>(true);
        else error(COMMA_EXPECTED);
     }
     else
     {
        source.seekg(pos);// Going back because previous parser_GetToken()
        dot_comma = pool.alloc<_bool_t>(false);
        hint = pool.alloc<std::string>("");
     }
     parser_Expected(TOKEN_IDENT,true);
     while (::parser_CurToken != TOKEN_EOL   ||
            ::parser_CurToken != TOKEN_COLON   )
     {  loop:; // In case of more then one input jumping to here from switch
        expr_Node_t *element = pool.alloc<expr_Node_t>();
        element = syntax_IdentRValueInput (vars,arrays,labels);
        var_list->push_back(element);
        switch (parser_GetToken())
        {
           case TOKEN_EOL:  goto exit;
           case TOKEN_COMMA:parser_GetToken();// Next line goto loop
           case TOKEN_IDENT:goto loop;
           default: error(WRONG_PARAMETERS_LIST);
        }
     }  exit:;
     stmt->NewStmtINPUT(hint,var_list,dot_comma,list);
  }
/* -------------------------------------------------------------------------- */
  static stmt_Node_t *syntax_stmtIF (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     elseif:;
     expr_Node_t *rel_expr = syntax_RelExpr (vars,arrays,labels);
     stmt_Node_t *cbr_stmt = pool.alloc<stmt_Node_t>()->NewCBranch(rel_expr,
                                                                          list);
     parser_Expected(TOKEN_KW_THEN,false);
     if (parser_GetToken() == TOKEN_EOL)
     {
        stmt_Node_t *lbl_on_true = pool.alloc<stmt_Node_t>()->NewEmptyLabel(
                                                                          list);
        syntax_BlockCheck(list,TOKEN_KW_ELSE,TOKEN_KW_ELSEIF,TOKEN_KW_END,
                                                            vars,arrays,labels);
        if (::parser_CurToken == TOKEN_KW_ELSE)
        {
           stmt_Node_t *lbl_to_finish = pool.alloc<stmt_Node_t>()->NewLabel(
                                                                          list);
           stmt_Node_t *lbl_on_false  = pool.alloc<stmt_Node_t>();
           lbl_on_false->NewEmptyLabel(list);
           parser_GetToken(); // To skip ELSE
           syntax_BlockCheck(list,TOKEN_KW_ELSEIF,TOKEN_KW_END,
                                                            vars,arrays,labels);
           if (::parser_CurToken == TOKEN_KW_ELSEIF)
           {
              stmt_Node_t *label_finish = pool.alloc<stmt_Node_t>();
              label_finish->NewEmptyLabel(list);
              lbl_to_finish->LinkToLabel (label_finish);
              cbr_stmt->LinkCBRLabels(lbl_on_true,lbl_on_false);
              goto elseif;
           }
           if (parser_GetToken() != TOKEN_KW_IF) // Looking forward for IF
           {// FIXME there is a possible bug.
            // ... THEN
            // ...
            // END
            // ...
            // END IF
            // Need to add error "Unreachable code" or similar.
              parser_Expected(TOKEN_KW_END,true);
              parser_Expected(TOKEN_KW_IF,true);
           }
           stmt_Node_t *label_finish = pool.alloc<stmt_Node_t>();
           label_finish->NewEmptyLabel(list);
           lbl_to_finish->LinkToLabel (label_finish);
           parser_GetToken();
           return cbr_stmt->LinkCBRLabels(lbl_on_true,lbl_on_false);
        }
        if (::parser_CurToken == TOKEN_KW_ELSEIF)
        {
           stmt_Node_t *lbl_to_finish = pool.alloc<stmt_Node_t>();
           lbl_to_finish->NewLabel(list);
           stmt_Node_t *lbl_on_false = pool.alloc<stmt_Node_t>();
           lbl_on_false->NewEmptyLabel(list);
           stmt_Node_t *label_finish = pool.alloc<stmt_Node_t>();
           label_finish->NewEmptyLabel(list);
           lbl_to_finish->LinkToLabel (label_finish);
           cbr_stmt->LinkCBRLabels(lbl_on_true,lbl_on_false);
           goto elseif;
        }
        // Token now is TOKEN_KW_END
        if (parser_GetToken() != TOKEN_KW_IF) // Looking forward for IF
        {
           parser_Expected(TOKEN_KW_END,true);
           parser_Expected(TOKEN_KW_IF,true);
        }
        stmt_Node_t *lbl_on_false = pool.alloc<stmt_Node_t>();
        lbl_on_false->NewEmptyLabel(list);
        parser_GetToken();
        return cbr_stmt->LinkCBRLabels(lbl_on_true,lbl_on_false);
     }
     else
     {
        stmt_Node_t *lbl_on_true = pool.alloc<stmt_Node_t>()->NewEmptyLabel(
                                                                          list);
        syntax_BlockCheck(list,TOKEN_KW_ELSE,TOKEN_KW_ELSEIF,TOKEN_EOL,
                                                            vars,arrays,labels);
        if (::parser_CurToken == TOKEN_KW_ELSE)
        {
           stmt_Node_t *lbl_to_finish = pool.alloc<stmt_Node_t>();
           lbl_to_finish->NewLabel(list);
           stmt_Node_t *lbl_on_false  = pool.alloc<stmt_Node_t>();
           lbl_on_false->NewEmptyLabel(list);
           parser_GetToken(); // To skip ELSE
           syntax_BlockCheck(list,TOKEN_KW_ELSEIF,TOKEN_EOL,vars,arrays,labels);
           if (::parser_CurToken == TOKEN_KW_ELSEIF)
           {
              stmt_Node_t *label_finish = pool.alloc<stmt_Node_t>();
              label_finish->NewEmptyLabel(list);
              lbl_to_finish->LinkToLabel (label_finish);
              cbr_stmt->LinkCBRLabels(lbl_on_true,lbl_on_false);
              goto elseif;
           }
           stmt_Node_t *label_finish = pool.alloc<stmt_Node_t>();
           label_finish->NewEmptyLabel(list);
           lbl_to_finish->LinkToLabel (label_finish);
           return cbr_stmt->LinkCBRLabels(lbl_on_true,lbl_on_false);
        }
        if (::parser_CurToken == TOKEN_KW_ELSEIF)
        {
           stmt_Node_t *lbl_to_finish = pool.alloc<stmt_Node_t>()->NewLabel(
                                                                          list);
           stmt_Node_t *lbl_on_false = pool.alloc<stmt_Node_t>();
           lbl_on_false->NewEmptyLabel(list);
           stmt_Node_t *label_finish = pool.alloc<stmt_Node_t>();
           label_finish->NewEmptyLabel(list);
           lbl_to_finish->LinkToLabel (label_finish);
           cbr_stmt->LinkCBRLabels(lbl_on_true,lbl_on_false);
           goto elseif;
        }
        // Token now is TOKEN_EOL
        stmt_Node_t *lbl_on_false = pool.alloc<stmt_Node_t>();
        lbl_on_false->NewEmptyLabel(list);
        return cbr_stmt->LinkCBRLabels(lbl_on_true,lbl_on_false);
     }
  }
/* -------------------------------------------------------------------------- */
  static stmt_Node_t *syntax_stmtWHILE (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     expr_Node_t *rel_expr = syntax_RelExpr (vars,arrays,labels);
     stmt_Node_t *cbr_stmt = pool.alloc<stmt_Node_t>();
     cbr_stmt->NewCBranch(rel_expr,list);
     stmt_Node_t *loop = pool.alloc<stmt_Node_t>()->NewEmptyLabel(list);
     syntax_BlockCheck(list,TOKEN_KW_WEND,vars,arrays,labels);
     stmt_Node_t *check = pool.alloc<stmt_Node_t>()->NewLabel(list);
     check->LinkToLabel (cbr_stmt);
     stmt_Node_t *lbl_on_true = pool.alloc<stmt_Node_t>()->NewLabel(list);
     lbl_on_true->LinkToLabel (loop);
     stmt_Node_t *lbl_on_false = pool.alloc<stmt_Node_t>();
     lbl_on_false->NewEmptyLabel(list);
     parser_GetToken();
     return cbr_stmt->LinkCBRLabels(lbl_on_true,lbl_on_false);
  }
/* -------------------------------------------------------------------------- */
  static stmt_Node_t *syntax_stmtFOR (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     parser_Expected(TOKEN_IDENT,true);
     stmt_Node_t *stmt = pool.alloc<stmt_Node_t>();
     std::string name = ::parser_CurTokenStr;// Counter name
     Types var_type;// Counter type
     // Attention.'i' will be reused again in function,below
     size3 i = findVarName (name,vars); // Will not accept array element
     parser_Expected(TOKEN_DELIM_EQUAL,true);  // as counter.Variable only.
     Variable *counter = NULL;
     if (i < 0)
     {  // Creating Variable.
        expr_Node_t *start = syntax_RelExpr(vars,arrays,labels);
        Variable *var = pool.alloc<Variable>(name);
        var->SetPage(vars);
        vars->push_back(var);
        counter = var;
        stmt->NewStmtLET (vars->back(),start,list);
     }
     else
     {  // Variable exist
        expr_Node_t *start = syntax_RelExpr(vars,arrays,labels);
        counter = vars->at(i);
        stmt->NewStmtLET (vars->at(i),start,list);
     }
     switch (name.at(name.size()-1)) // Detect the type of variable for later
     {                               // using.
        case '%': var_type = INT;   break;
        case '&': var_type = LINT;  break;
        case '!': var_type = FLOAT; break;
        case '#': var_type = DOUBLE;break;
        case '$': var_type = STRING;break;
        default : var_type = INT;   break;
     }
     parser_Expected(TOKEN_KW_TO,false);
     // Manually generate relation expression
     counter->SetType(var_type);
     expr_Node_t *operand1 = pool.alloc<expr_Node_t>(EK_VAR,counter);
     // The end value of the counter
     expr_Node_t *operand2 = syntax_RelExpr(vars,arrays,labels);
     expr_Node_t *rel_expr(NULL);  // Relation expression
     stmt_Node_t *cbr_stmt = pool.alloc<stmt_Node_t>();// Cbranch statement
     stmt_Node_t *loop     = pool.alloc<stmt_Node_t>();// loop label
     expr_Node_t *operation;
     /// Variant with default stepping.Default stepping is 1.Relation is "<="
     if (::parser_CurToken != TOKEN_KW_STEP)
     {
        // Creating conditional branch
        rel_expr = pool.alloc<expr_Node_t>(EK_BINOP,
                                           operand1,operand2,OP_REL_LESS_EQUAL);
        cbr_stmt->NewCBranch(rel_expr,list);
        loop->NewEmptyLabel(list);
        syntax_BlockCheck(list,TOKEN_KW_NEXT,vars,arrays,labels);
        parser_Expected(TOKEN_IDENT,true);
        if (::parser_CurTokenStr != name) // After NEXT must follow right
           error(EXPECTED_VARIABLE,name); // variable
        // Creating counter increment part with default stepping
        operand1 = pool.alloc<expr_Node_t>(EK_VAR,counter);
        string one_val("1");
        operand2 = pool.alloc<expr_Node_t>(EK_CONST,&one_val,var_type);
     }
     else
     {/// Variant,when step isn't default.Current token is STEP keyword.
        Value step = syntax_runtimePrimary();
        string step_string = ::parser_CurTokenStr;
        switch (step.GetType())
        {
           case INT:
              if (step.Get_ival() < 0)   // Relation is ">="
                 rel_expr = pool.alloc<expr_Node_t>(EK_BINOP,operand1,operand2,
                                                            OP_REL_GREAT_EQUAL);
              else                   // Relation is "<="
                 rel_expr = pool.alloc<expr_Node_t>(EK_BINOP,operand1,operand2,
                                                             OP_REL_LESS_EQUAL);
           break;
           case LINT:
              if (step.Get_lival() < 0)   // Relation is ">="
                 rel_expr = pool.alloc<expr_Node_t>(EK_BINOP,operand1,operand2,
                                                            OP_REL_GREAT_EQUAL);
              else                    // Relation is "<="
                 rel_expr = pool.alloc<expr_Node_t>(EK_BINOP,operand1,operand2,
                                                             OP_REL_LESS_EQUAL);
           break;
           case FLOAT:
              if (step.Get_fval() < 0)   // Relation is ">="
                 rel_expr = pool.alloc<expr_Node_t>(EK_BINOP,operand1,operand2,
                                                            OP_REL_GREAT_EQUAL);
              else                   // Relation is "<="
                 rel_expr = pool.alloc<expr_Node_t>(EK_BINOP,operand1,operand2,
                                                             OP_REL_LESS_EQUAL);
           break;
           case DOUBLE:
              if (step.Get_dval() < 0)   // Relation is ">="
                 rel_expr = pool.alloc<expr_Node_t>(EK_BINOP,operand1,operand2,
                                                            OP_REL_GREAT_EQUAL);
              else                   // Relation is "<="
                 rel_expr = pool.alloc<expr_Node_t>(EK_BINOP,operand1,operand2,
                                                             OP_REL_LESS_EQUAL);
           break;
           default: error(CONSTANT_EXPECTED);
        }
        cbr_stmt->NewCBranch(rel_expr,list);
        loop->NewEmptyLabel(list);
        parser_GetToken(); // To skip TOKEN_CONST_**
        syntax_BlockCheck(list,TOKEN_KW_NEXT,vars,arrays,labels);
        parser_Expected(TOKEN_IDENT,true);
        if (::parser_CurTokenStr != name) // After NEXT must follow right
           error(EXPECTED_VARIABLE,name); // variable
        // Creating counter increment part with user defined stepping
        operand1 = pool.alloc<expr_Node_t>(EK_VAR,counter);
        switch (step.GetType())
        {
           case INT:
              if (step.Get_ival() < 0)   // Relation is ">="
              {
                 expr_Node_t *operand;
                 operand  = pool.alloc<expr_Node_t>(EK_CONST,
                                                   &step_string,step.GetType());
                 operand2 = pool.alloc<expr_Node_t>(EK_UNOP,operand,OP_MINUS);
              }
              else                   // Relation is "<="
                 operand2  = pool.alloc<expr_Node_t>(EK_CONST,
                                                   &step_string,step.GetType());
           break;
           case LINT:
              if (step.Get_lival() < 0)  // Relation is ">="
              {
                 expr_Node_t *operand;
                 operand  = pool.alloc<expr_Node_t>(EK_CONST,
                                                   &step_string,step.GetType());
                 operand2 = pool.alloc<expr_Node_t>(EK_UNOP,operand,OP_MINUS);
              }
              else                   // Relation is "<="
                 operand2  = pool.alloc<expr_Node_t>(EK_CONST,
                                                   &step_string,step.GetType());
           break;
           case FLOAT:
              if (step.Get_fval() < 0)   // Relation is ">="
              {
                 expr_Node_t *operand;
                 operand  = pool.alloc<expr_Node_t>(EK_CONST,
                                                   &step_string,step.GetType());
                 operand2 = pool.alloc<expr_Node_t>(EK_UNOP,operand,OP_MINUS);
              }
              else                   // Relation is "<="
                 operand2  = pool.alloc<expr_Node_t>(EK_CONST,
                                                   &step_string,step.GetType());
           break;
           case DOUBLE:
              if (step.Get_dval() < 0)   // Relation is ">="
              {
                 expr_Node_t *operand;
                 operand  = pool.alloc<expr_Node_t>(EK_CONST,
                                                   &step_string,step.GetType());
                 operand2 = pool.alloc<expr_Node_t>(EK_UNOP,operand,OP_MINUS);
              }
              else                   // Relation is "<="
                 operand2  = pool.alloc<expr_Node_t>(EK_CONST,
                                                   &step_string,step.GetType());
           break;
           default: error(CONSTANT_EXPECTED);
        }
     }
     // If step is < 0 it have it's own unary minus
     operation = pool.alloc<expr_Node_t>(EK_BINOP,operand1,operand2,OP_PLUS);
     i = findVarName (name,vars);
     assert (i >= 0);// Paranoid?
     stmt_Node_t *increment = pool.alloc<stmt_Node_t>();
     increment->NewStmtLET (vars->at(i),operation,list);

     stmt_Node_t *check = pool.alloc<stmt_Node_t>()->NewLabel(list);
     check->LinkToLabel (cbr_stmt);

     stmt_Node_t *lbl_on_true = pool.alloc<stmt_Node_t>()->NewLabel(list);
     lbl_on_true->LinkToLabel (loop);

     stmt_Node_t *lbl_on_false = pool.alloc<stmt_Node_t>()->NewEmptyLabel(list);

     parser_GetToken();
     return cbr_stmt->LinkCBRLabels(lbl_on_true,lbl_on_false);
  }
/* -------------------------------------------------------------------------- */
  static stmt_Node_t *syntax_stmtDO (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     stmt_Node_t *loop = pool.alloc<stmt_Node_t>()->NewEmptyLabel(list);

     parser_GetToken(); // To skip DO keyword.

     syntax_BlockCheck(list,TOKEN_KW_UNTIL,TOKEN_KW_WHILE,vars,arrays,labels);

     expr_Node_t *rel_expr = syntax_RelExpr(vars,arrays,labels);

     stmt_Node_t *cbr_stmt = pool.alloc<stmt_Node_t>();
     cbr_stmt->NewCBranch(rel_expr,list);

     stmt_Node_t *lbl_on_true = pool.alloc<stmt_Node_t>()->NewLabel(list);
     lbl_on_true->LinkToLabel (loop);

     stmt_Node_t *lbl_on_false = pool.alloc<stmt_Node_t>()->NewEmptyLabel(list);

     parser_GetToken();
     return cbr_stmt->LinkCBRLabels(lbl_on_true,lbl_on_false);
  }
/* -------------------------------------------------------------------------- */
  static void syntax_stmtGOTO (stmt_List_t *list,lab_Arr_t *labels)
  {
     parser_Expected(TOKEN_CONST_INT,true);
     size3 i = findVarName (::parser_CurTokenStr,labels);
     if ( pool.isConflictingDeclaration (   ::parser_CurTokenStr,
                                            pool.SEARCH_IN_ALL_DATA
                                          | pool.DONT_SEARCH_IN_LBLS ) )
        error(CONFLICTING_DECLARATION,::parser_CurTokenStr);
     if (i < 0)
     {
        stmt_Node_t *label = pool.alloc<stmt_Node_t>()->NewLabel(list);
        label_Named_t *nlabel = label_NewNamedLabel(::parser_CurTokenStr,label,
                                                                        labels);
        nlabel->SetLinked(false);
        parser_GetToken();
        return;
     }
     stmt_Node_t *label = pool.alloc<stmt_Node_t>()->NewLabel(list);
     label->LinkToLabel(labels->at(i)->GetAddress());
     parser_GetToken();
  }
/* -------------------------------------------------------------------------- */
  static void syntax_Label(stmt_List_t *list,lab_Arr_t *labels)
  {
     size3 i = findVarName (::parser_CurTokenStr,labels);
     if ( pool.isConflictingDeclaration (   ::parser_CurTokenStr,
                                            pool.SEARCH_SHARED_DATA_ONLY
                                          | pool.DONT_SEARCH_IN_LBLS  ) )
        error(CONFLICTING_DECLARATION,::parser_CurTokenStr);
     if (i >= 0)
     {
        if (labels->at(i)->IsLinked())
           error(DUPLICATE_CASE_LABEL,::parser_CurTokenStr);
        stmt_Node_t *addr = pool.alloc<stmt_Node_t>()->NewEmptyLabel(list);
        labels->at(i)->GetAddress()->LinkToLabel(addr);
        labels->at(i)->SetLinked(true);
        parser_Expected(TOKEN_COLON,true);
        return;
     }
     stmt_Node_t *label = pool.alloc<stmt_Node_t>()->NewEmptyLabel(list);
     label_Named_t *nlabel = label_NewNamedLabel(::parser_CurTokenStr,label,
                                                                        labels);
     // Here is a bit confusing place.Label is not linked to any place,
     // but we using SetLinked() function to avoid conflict when label could be
     // found in program more than one time.
     nlabel->SetLinked(true);
     parser_Expected(TOKEN_COLON,true);
  }
/* -------------------------------------------------------------------------- */
  static void syntax_stmtDIM (var_Arr_t *vars,arr_Arr_t *arrays)
  {  // Checking first variable/array definition for SHARED keyword.
     /// In original QBASIC, when SHARED keyword used after DIM, all variables
     /// and arrays in this statement are shared.
     /// When you try this: DIM var1, SHARED var2 AS 'typename'
     /// QBASIC will generate error.So SHARED can be used once in statement,
     /// and it have to follow DIM keyword.
     _bool_t shared_var = false;
     size_t pos = source.tellg();
     if (parser_GetToken() == TOKEN_KW_SHARED)
        shared_var = true;
     else
        source.seekg(pos);
     parser_Expected(TOKEN_IDENT,true);
     while (::parser_CurToken != TOKEN_EOL || ::parser_CurToken == TOKEN_COMMA)
     {
        parser_Expected(TOKEN_IDENT,false);
        string name = ::parser_CurTokenStr;
        if ( pool.isConflictingDeclaration (name,pool.SEARCH_IN_ALL_DATA) )
           error(CONFLICTING_DECLARATION,name);

        // DIM SHARED var [AS type] case
        pos = source.tellg();
        if (parser_GetToken() != TOKEN_LEFT_PARENTH)
        {  /// Variable definition (begin)
           Variable *var = pool.alloc<Variable>(name);
           Types var_type(VOID);
           // AS 'typename' case
           size_t pos2 = source.tellg();
           if (::parser_CurToken != TOKEN_EOL  &&
               ::parser_CurToken != TOKEN_EOF  &&
               parser_GetToken() == TOKEN_KW_AS  )
           {
              switch (parser_GetToken())
              {
                 case TOKEN_TYPE_INT:    var_type = INT;   break;
                 case TOKEN_TYPE_LINT:   var_type = LINT;  break;
                 case TOKEN_TYPE_FLOAT:  var_type = FLOAT; break;
                 case TOKEN_TYPE_DOUBLE: var_type = DOUBLE;break;
                 case TOKEN_TYPE_STRING: var_type = STRING;break;
                 default: error(TYPENAME_EXPECTED);
              }
           }
           else
           {
              source.seekg(pos2);                     // Get back at same token
              if (::parser_CurToken != TOKEN_EOL  &&
                  ::parser_CurToken != TOKEN_EOF    )
                 parser_GetToken();
              switch (name.at(name.size()-1))
              {  // Find what type of array to create depends of array's name
                 case '%': var_type = INT;   break;
                 case '&': var_type = LINT;  break;
                 case '!': var_type = FLOAT; break;
                 case '#': var_type = DOUBLE;break;
                 case '$': var_type = STRING;break;
                 default : var_type = INT;   break;
              }
           }
           var->SetType(var_type);
           //FIXME! Value *zero = memalloc(zero,"0",var_type); /*
           //FIXME! var->SetValue(zero);
           if (shared_var)
           {
              var->SetShared(true);
              var->SetPage(pool.GetSharedVars());
              pool.AddSharedVar(var);
           }
           else
           {
              var->SetPage(vars);
              vars->push_back(var);
           }
        }  /// Variable definition (end)
        else
        {  /// Array definition (begin)
           source.seekg(pos);
           parser_Expected(TOKEN_LEFT_PARENTH,true);// Expected '('
           sz4_Arr_t *indexes       = pool.alloc<sz4_Arr_t>();
           sz3_Arr_t *start_indexes = pool.alloc<sz3_Arr_t>();
           while (::parser_CurToken != TOKEN_RIGHT_PARENTH)
           {
              Value val1 = syntax_runtimePrimary();
              // Checking type of index and converting it to long type
              switch (val1.GetType())
              {
                 case INT:  val1.SetType(LINT);
                            val1.Set_lival(val1.Get_ival());
                 break;
                 case LINT: break;
                 default:   error(WRONG_PARAMETER_TYPE);
              }
              // Check what is after number: ")" or "," or "TO"
              switch (parser_GetToken())
              {
                 case TOKEN_KW_TO: {// "[x] TO [y]" variant
                    Value val2 = syntax_runtimePrimary();
                    // Checking type of index and converting it to long type
                    switch (val2.GetType())
                    {
                       case INT:  val2.SetType(LINT);
                                  val2.Set_lival(val2.Get_ival());
                       break;
                       case LINT: break;
                       default:   error(WRONG_PARAMETER_TYPE);
                    }
                    // Set lower bound for index in array
                    start_indexes->push_back(val1.Get_lival());
                    // Set next dimension in array
                    indexes->push_back(val2.Get_lival()-val1.Get_lival()+1);
                    if (indexes->back() < 1)
                       error(UNKNOWN_SIZE);
                                      //Expected comma or ')'.In case of comma
                    parser_GetToken();//syntax_runtimePrimary will work fine or
                                      //catch an error.In case of ')' loop ended
                 break;}
                 case TOKEN_RIGHT_PARENTH:
                    // Set lower bound for index in array
                    start_indexes->push_back(user_opt.GetBase()); //Default base
                    // Set next dimension in array
                    indexes->push_back(val1.Get_lival());
                 break;
                 case TOKEN_COMMA:
                    // Set lower bound for index in array
                    start_indexes->push_back(user_opt.GetBase()); //Default base
                    // Set next dimension in array
                    indexes->push_back(val1.Get_lival());
                    // If after comma unexpected symbol,next loop will catch it.
                 break;
                 default: error(WRONG_PARAMETERS_LIST);
              }
           }
           if (indexes->empty())
              error(UNKNOWN_SIZE);
           Types arr_elem_type(VOID);
           size_t pos2 = source.tellg();
           if (parser_GetToken() == TOKEN_KW_AS)
           {
              switch (parser_GetToken())
              {
                 case TOKEN_TYPE_INT:    arr_elem_type = INT;   break;
                 case TOKEN_TYPE_LINT:   arr_elem_type = LINT;  break;
                 case TOKEN_TYPE_FLOAT:  arr_elem_type = FLOAT; break;
                 case TOKEN_TYPE_DOUBLE: arr_elem_type = DOUBLE;break;
                 case TOKEN_TYPE_STRING: arr_elem_type = STRING;break;
                 default: error(TYPENAME_EXPECTED);
              }
           }
           else
           {
              source.seekg(pos2);
              switch (name.at(name.size()-1))
              {  // Find what type of array to create depends of array's name
                 case '%': arr_elem_type = INT;   break;
                 case '&': arr_elem_type = LINT;  break;
                 case '!': arr_elem_type = FLOAT; break;
                 case '#': arr_elem_type = DOUBLE;break;
                 case '$': arr_elem_type = STRING;break;
                 default : arr_elem_type = INT;   break;
              }
           }
           Array *arr = NULL;
           if (shared_var)
           {
              arr = pool.alloc<Array>(name,arr_elem_type,Array::SHARED);
              arr->SetPage(pool.GetSharedArrs());
              pool.AddSharedArr(arr);
           }
           else {
              arr = pool.alloc<Array>(name,arr_elem_type,Array::LOCAL);
              arr->SetPage(arrays);
              arrays->push_back(arr);
           }
           size4 size = indexes->size();
           size4 elem_count = 1;
           size4 sizeMax = static_cast<size4>(arr->GetElements()->max_size());
           for (size4 i = 0;i < size; ++i)
           {// Checking how much elements array will contain
              elem_count *= indexes->at(i);
              if (elem_count == 0)
                 error(ZERO_SIZE_DIM);
              if (elem_count > sizeMax)
                 error(LARGE_VARIABLE,name);
           }
           // Creating array
           arr->SetMaxIndex(elem_count);
           arr->pushNULLs();            //Push elem_count NULLs to data section.
           arr->SetStartIndexes(start_indexes);
           arr->SetDimensions(indexes);

           pool.free(start_indexes);
           pool.free(indexes);

        } /// Array definition (end)
        if (::parser_CurToken == TOKEN_EOL ||
            ::parser_CurToken == TOKEN_EOF   ) break;

        if (parser_GetToken() == TOKEN_COMMA)
        {
           parser_GetToken();
           parser_Expected(TOKEN_IDENT,false);
        }

        if (::parser_CurToken == TOKEN_EOL ||
            ::parser_CurToken == TOKEN_EOF   ) break;
     }
  }
/* -------------------------------------------------------------------------- */
  static void syntax_stmtSUB (stmt_List_t *list)
  {
     parser_Expected (TOKEN_IDENT,true);
     std::string name = parser_CurTokenStr;
     if ( pool.isConflictingDeclaration (name,pool.SEARCH_IN_ALL_DATA) )
        error(CONFLICTING_DECLARATION,name);
     /// Subroutine creation
     // Local data for subroutine

     _size_t subroutine_self_index = pool.GetCurFuncs()->size();

     var_Arr_t *lcl_vars = pool.alloc<var_Arr_t>();
     arr_Arr_t *lcl_arrs = pool.alloc<arr_Arr_t>();
     lab_Arr_t *lcl_lbls = pool.alloc<lab_Arr_t>();

     // Label in main program to skip function body,it will be liked to label in
     // the function end
     stmt_Node_t *skip_body = ::last_stmt;// FIXME! "BAD STEP! FIX ME SOMEHOW!"

     // Born
     Subroutine *sub = pool.alloc<Subroutine>(subroutine_self_index);
     /// Need to do this now,because in case of recursion we have no function
     /// to refer to
     pool.GetCurFuncs()->push_back(sub);
     // Subroutine address
     stmt_Node_t *addr = pool.alloc<stmt_Node_t>();
     addr->NewEmptyLabel(sub->getStmtList());
     sub->setName(&parser_CurTokenStr);
     sub->setAddr(addr);

     // Checking for parameters
     if (parser_GetToken() == TOKEN_LEFT_PARENTH)
     {// Parameters list
        /// FIXME Add code here
     }

     while (::parser_CurToken != TOKEN_KW_SUB)
     {// Syntax checking inside function body
        syntax_BlockCheck(sub->getStmtList(), TOKEN_KW_END, TOKEN_KW_CALL,
                                                    lcl_vars,lcl_arrs,lcl_lbls);
        /// Is function calling itself?
        if (::parser_CurToken == TOKEN_KW_CALL) {
           _size_t pos = source.tellg();
           parser_Expected(TOKEN_IDENT,true);
           if (name == ::parser_CurTokenStr) // This is recursion
           {  // At this point we need to create CALL statement, and right after
              // it special RETURN statement,that will signal to interpreter to
              // set appropriate values for variables page,
              // and to work with same page.
              _bool_t recursion_state = true;
              sub->setRecursive(recursion_state);

              expr_List_t *parameters     = pool.alloc<expr_List_t>();
              stmt_Node_t *function_call  = pool.alloc<stmt_Node_t>();

              std::string *call_name = pool.alloc<std::string>
                                                           (parser_CurTokenStr);

              function_call->NewStmtCALL(PK_USER, parameters,
                                         sub->getStmtList(),
                                         pool.GetCurFuncs()->size() - 1,// Dirty
                                         call_name                           );

              stmt_Node_t *exit_from_call = pool.alloc<stmt_Node_t>();

              exit_from_call->NewStmtRETURN(PK_USER,parameters,
                                            sub->getStmtList(),
                                            call_name,recursion_state);

              sub->addExitFromCall(exit_from_call);
              parser_GetToken();
           }
           else source.seekg(pos);
        }
        if (parser_GetToken() == TOKEN_KW_SUB)
        {
           parser_GetToken();
           break;
        }
        parser_GetToken();
     }

     // This is label that pointing to next instruction after exiting from
     // function. Each call it will be changed.
     stmt_Node_t *exit_from = pool.alloc<stmt_Node_t>();
     exit_from->NewLabel(sub->getStmtList());

     // To skip function body (definition) in program
     stmt_Node_t *body_end  = pool.alloc<stmt_Node_t>()->NewEmptyLabel(list);
     skip_body->setNextStmt (body_end);

     sub->setVars(lcl_vars);
     sub->setArrs(lcl_arrs);
     sub->setLabs(lcl_lbls);
     sub->setExit(exit_from);

     pool.pushToVarsStack (lcl_vars);
     pool.pushToArrsStack (lcl_arrs);
     pool.pushToLblsStack (lcl_lbls);
  }
/* -------------------------------------------------------------------------- */
  static void syntax_stmtCALL (stmt_List_t *list)
  {
     parser_Expected (TOKEN_IDENT,true);
     std::string *name = pool.alloc<std::string>(parser_CurTokenStr);
     //FIXME!!! sub_Arr_t *curr_functions = pool.GetGlobFuncs();
     size3 index = findVarName (*name,pool.GetCurFuncs());
     if (index < 0) error(UNDEFINED_FUNCTION,*name);

     expr_List_t *parameters     = pool.alloc<expr_List_t>();
     stmt_Node_t *function_call  = pool.alloc<stmt_Node_t>();
     function_call->NewStmtCALL(PK_USER,parameters,list,index,name);

     stmt_Node_t *return_from_call = pool.alloc<stmt_Node_t>();
     return_from_call->NewStmtRETURN(PK_USER,parameters,list,name);

     pool.GetCurFuncs()->at(index)->addExitFromCall(return_from_call);
     parser_GetToken();
  }
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_SDL_GRAPHICS
  static void syntax_stmtSCREEN (stmt_List_t *list,var_Arr_t *vars,
                                            arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     expr_List_t *parameters = pool.alloc<expr_List_t>();
     while (::parser_CurToken != TOKEN_EOL)
     {
        expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
        parameters->push_back(expr);
     }
     size3 size = parameters->size();
     if (size == 0) error(TOO_FEW_PARAMETERS);
     if (size > 1) error(TOO_MUCH_PARAMETERS);
     stmt_Node_t *stmt = pool.alloc<stmt_Node_t>();
     stmt->NewStmtCALL(PK_SCREEN,parameters,list);
  }
/* -------------------------------------------------------------------------- */
  static void syntax_stmtPRESET (stmt_List_t *list,var_Arr_t *vars,
                                            arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     expr_List_t *parameters = pool.alloc<expr_List_t>();
     parser_Expected(TOKEN_LEFT_PARENTH,true);
     while (::parser_CurToken != TOKEN_RIGHT_PARENTH)
     {
        expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
        parameters->push_back(expr);
     }
     if (parser_GetToken() == TOKEN_COMMA)
     {
        expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
        parameters->push_back(expr);
        parser_GetToken();
     }
     size3 size = parameters->size();
     if (size < 2) error(TOO_FEW_PARAMETERS);
     if (size > 3) error(TOO_MUCH_PARAMETERS);
     stmt_Node_t *stmt = pool.alloc<stmt_Node_t>();
     stmt->NewStmtCALL(PK_PRESET,parameters,list);
  }
/* -------------------------------------------------------------------------- */
  static void syntax_stmtLINE (stmt_List_t *list,var_Arr_t *vars,
                                            arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     expr_List_t *parameters = pool.alloc<expr_List_t>();
     parser_Expected(TOKEN_LEFT_PARENTH,true);
     while (::parser_CurToken != TOKEN_RIGHT_PARENTH)
     {
        expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
        parameters->push_back(expr);
     }
     parser_Expected(TOKEN_DELIM_MINUS,true);
     parser_Expected(TOKEN_LEFT_PARENTH,true);
     while (::parser_CurToken != TOKEN_RIGHT_PARENTH)
     {
        expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
        parameters->push_back(expr);
     }
     while ( ::parser_CurToken != TOKEN_EOL )
     {
        parser_GetToken();
        parser_Expected(TOKEN_COMMA,false);
        comma_token:;                                             // comma_token
        s_count i = source.tellg();
        if ( (parser_GetToken() != TOKEN_IDENT) &&
             ::parser_CurToken != TOKEN_CONST_INT )
           error(WRONG_PARAMETERS_LIST);
        source.seekg(i);
        if (parser_GetToken() == TOKEN_IDENT)
        {
           if ( ::parser_CurTokenStr != "B" && ::parser_CurTokenStr != "BF" )
           {
              source.seekg(i);
              expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
              parameters->push_back(expr);
              if (::parser_CurToken == TOKEN_COMMA) goto comma_token;
              if (::parser_CurToken == TOKEN_EOL) break;
           }
           expr_Node_t *expr;
           expr = pool.alloc<expr_Node_t>(EK_CONST,
                                                  &::parser_CurTokenStr,STRING);
           parameters->push_back(expr);
           parser_Expected(TOKEN_EOL,true);
        }
        else
        if (::parser_CurToken == TOKEN_CONST_INT)
        {
           source.seekg(i);
           expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
           parameters->push_back(expr);
           if (::parser_CurToken == TOKEN_COMMA) goto comma_token;// Up
           if (::parser_CurToken == TOKEN_EOL) break;
        }
     }
     size3 size = parameters->size();
     if (size < 4) error(TOO_FEW_PARAMETERS);
     if (size > 6) error(TOO_MUCH_PARAMETERS);
     stmt_Node_t *stmt = pool.alloc<stmt_Node_t>();
     stmt->NewStmtCALL(PK_LINE,parameters,list);
  }
/* -------------------------------------------------------------------------- */
  static void syntax_stmtCIRCLE (stmt_List_t *list,var_Arr_t *vars,
                                        arr_Arr_t *arrays,lab_Arr_t *labels)
  {
     expr_List_t *parameters = pool.alloc<expr_List_t>();
     parser_Expected(TOKEN_LEFT_PARENTH,true);
     while (::parser_CurToken != TOKEN_RIGHT_PARENTH)
     {
        expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
        parameters->push_back(expr);
     }
     parser_GetToken();
     while (::parser_CurToken != TOKEN_EOL)
     {
        if (::parser_CurToken == TOKEN_COMMA)
        {
           expr_Node_t *expr = syntax_RelExpr(vars,arrays,labels);
           parameters->push_back(expr);
        }
     }
     size3 size = parameters->size();
     if (size < 3) error(TOO_FEW_PARAMETERS);
     if (size > 4) error(TOO_MUCH_PARAMETERS);
     stmt_Node_t *stmt = pool.alloc<stmt_Node_t>();
     stmt->NewStmtCALL(PK_CIRCLE,parameters,list);
  }
#endif // DISABLE_SDL_GRAPHICS
/* -------------------------------------------------------------------------- */
  static void syntax_stmtCLS (stmt_List_t *list)
  {
     expr_List_t *parameters = pool.alloc<expr_List_t>();
     stmt_Node_t *stmt = pool.alloc<stmt_Node_t>();
     stmt->NewStmtCALL(PK_CLS,parameters,list);
     parser_GetToken();
  }
/* -------------------------------------------------------------------------- */
  static void syntax_stmtEND(stmt_List_t *list)
  {
     expr_List_t *parameters = pool.alloc<expr_List_t>();
     stmt_Node_t *stmt = pool.alloc<stmt_Node_t>();
     stmt->NewStmtCALL(PK_END,parameters,list);
  }
/* -------------------------------------------------------------------------- */
  static void syntax_stmtBEEP(stmt_List_t *list)
  {
     expr_List_t *parameters = pool.alloc<expr_List_t>();
     stmt_Node_t *stmt = pool.alloc<stmt_Node_t>();
     stmt->NewStmtCALL(PK_BEEP,parameters,list);
     parser_GetToken();
  }
/* -------------------------------------------------------------------------- */
  static void syntax_stmtCLEAR(stmt_List_t *list)
  {
     expr_List_t *parameters = pool.alloc<expr_List_t>();
     stmt_Node_t *stmt = pool.alloc<stmt_Node_t>();
     stmt->NewStmtCALL(PK_CLEAR,parameters,list);
     parser_GetToken();
  }
/* -------------------------------------------------------------------------- */
  static void syntax_LabelsMustBeValid ()
  {
     for (vector <lab_Arr_t *>::iterator i = pool.GetLabStack()->begin();
                                             i < pool.GetLabStack()->end(); ++i)
     {
        if (!(*i)->empty())
        {
           for (vector <label_Named_t *>::iterator j = (*i)->begin();
                                                           j < (*i)->end(); ++j)
           {
              if (!(*j)->IsLinked())
                 error(UNLINKED_LABEL,(*j)->GetName());
           }
        }
     }
  }
/* -------------------------------------------------------------------------- */
  static void syntax_userOPTION ()
  {
     switch (parser_GetToken())
     {
        case TOKEN_KW_BASE: {
           // OPTION BASE <val> - changing the default base for array indexes
           Value val = syntax_runtimePrimary ();
           switch (val.GetType())
           {// Checking type of index and converting it to long type
              case INT:  val.SetType(LINT);
                         val.Set_lival(val.Get_ival());
              break;
              case LINT: break;
              default:   error(WRONG_PARAMETER_TYPE);
           }
           user_opt.SetBase(val.Get_lival());// Setting base value
           parser_GetToken();// To get to TOKEN_EOL or error.
           return;}
#ifndef DISABLE_TRACING
        case TOKEN_KW_TRACE:
           parser_GetToken();
           if (::parser_CurTokenStr == "ACTION")
           {
              // OPTION TRACE ACTION <val>
              Value val = syntax_runtimePrimary ();
              switch (val.GetType())
              {
                 case INT:  if (val.Get_ival() != 0)
                               user_opt.SetTraceAct(true);
                            else  user_opt.SetTraceAct(false);
                 break;
                 default:   error(WRONG_PARAMETER_TYPE);
              }
              parser_GetToken();// To get to TOKEN_EOL or error.
              return;
           }
           if (::parser_CurTokenStr == "TREE")
           {
              // OPTION TRACE TREE <val>
              Value val = syntax_runtimePrimary ();
              switch (val.GetType())
              {
                 case INT:  if (val.Get_ival() != 0)
                               user_opt.SetTraceTree(true);
                            else  user_opt.SetTraceTree(false);
                 break;
                 default:   error(WRONG_PARAMETER_TYPE);
              }
              parser_GetToken();// To get to TOKEN_EOL or error.
              return;
           }
#endif // DISABLE_TRACING
           error(WRONG_OPTION_STATEMENT);
        default: error(WRONG_OPTION_STATEMENT);
     }
  }
/* -------------------------------------------------------------------------- */
  static void syntax_BlockCheck(stmt_List_t *list,parser_TokenType end_token,
                                                              var_Arr_t *vars,
                                                              arr_Arr_t *arrays,
                                                              lab_Arr_t *lbls)
  {
     while (::parser_CurToken != end_token)
     {
        switch (::parser_CurToken)
        {
           case TOKEN_KW_DO:
           case TOKEN_KW_FOR:
           case TOKEN_KW_WHILE:
           case TOKEN_KW_IF:    syntax_CBranch   (list,vars,arrays,lbls); break;
           case TOKEN_IDENT:
           case TOKEN_KW_LET:   syntax_stmtLET   (list,vars,arrays,lbls); break;
           case TOKEN_KW_PRINT: syntax_stmtPRINT (list,vars,arrays,lbls); break;
           case TOKEN_KW_INPUT: syntax_stmtINPUT (list,vars,arrays,lbls); break;
           case TOKEN_KW_GOTO:  syntax_stmtGOTO  (list,            lbls); break;
           case TOKEN_KW_DIM:   syntax_stmtDIM   (     vars,arrays     ); break;
           case TOKEN_KW_CALL:  syntax_stmtCALL  (list                 ); break;
#ifndef DISABLE_SDL_GRAPHICS
           case TOKEN_KW_SCREEN:syntax_stmtSCREEN(list,vars,arrays,lbls); break;
           case TOKEN_KW_PRESET:syntax_stmtPRESET(list,vars,arrays,lbls); break;
           case TOKEN_KW_LINE:  syntax_stmtLINE  (list,vars,arrays,lbls); break;
           case TOKEN_KW_CIRCLE:syntax_stmtCIRCLE(list,vars,arrays,lbls); break;
#endif // DISABLE_SDL_GRAPHICS
           case TOKEN_KW_CLS:   syntax_stmtCLS   (list                 ); break;
           case TOKEN_KW_END:   syntax_stmtEND   (list                 ); break;
           case TOKEN_KW_BEEP:  syntax_stmtBEEP  (list                 ); break;
           case TOKEN_KW_CLEAR: syntax_stmtCLEAR (list                 ); break;
           case TOKEN_CONST_INT:syntax_Label     (list,            lbls); break;
           case TOKEN_KW_OPTION:syntax_userOPTION ();    break;
           case TOKEN_COLON:    parser_GetToken   ();    break;
           case TOKEN_EOL:      parser_GetToken   ();    break;
           case TOKEN_NULL:     parser_GetToken   ();    break;
           default: error (KEYWORD_EXPECTED);
        }
     }
  }
/* -------------------------------------------------------------------------- */
  static void syntax_BlockCheck(stmt_List_t *list,parser_TokenType end_token1,
                                                  parser_TokenType end_token2,
                                                  var_Arr_t *vars,
                                                  arr_Arr_t *arrays,
                                                  lab_Arr_t *lbls)
  {
     while (::parser_CurToken != end_token1 &&
            ::parser_CurToken != end_token2   )
     {
        switch (::parser_CurToken)
        {
           case TOKEN_KW_DO:
           case TOKEN_KW_FOR:
           case TOKEN_KW_WHILE:
           case TOKEN_KW_IF:    syntax_CBranch   (list,vars,arrays,lbls); break;
           case TOKEN_IDENT:
           case TOKEN_KW_LET:   syntax_stmtLET   (list,vars,arrays,lbls); break;
           case TOKEN_KW_PRINT: syntax_stmtPRINT (list,vars,arrays,lbls); break;
           case TOKEN_KW_INPUT: syntax_stmtINPUT (list,vars,arrays,lbls); break;
           case TOKEN_KW_GOTO:  syntax_stmtGOTO  (list,            lbls); break;
           case TOKEN_KW_DIM:   syntax_stmtDIM   (     vars,arrays     ); break;
           case TOKEN_KW_CALL:  syntax_stmtCALL  (list                 ); break;
#ifndef DISABLE_SDL_GRAPHICS
           case TOKEN_KW_SCREEN:syntax_stmtSCREEN(list,vars,arrays,lbls); break;
           case TOKEN_KW_PRESET:syntax_stmtPRESET(list,vars,arrays,lbls); break;
           case TOKEN_KW_LINE:  syntax_stmtLINE  (list,vars,arrays,lbls); break;
           case TOKEN_KW_CIRCLE:syntax_stmtCIRCLE(list,vars,arrays,lbls); break;
#endif // DISABLE_SDL_GRAPHICS
           case TOKEN_KW_CLS:   syntax_stmtCLS   (list                 ); break;
           case TOKEN_KW_END:   syntax_stmtEND   (list                 ); break;
           case TOKEN_KW_BEEP:  syntax_stmtBEEP  (list                 ); break;
           case TOKEN_KW_CLEAR: syntax_stmtCLEAR (list                 ); break;
           case TOKEN_CONST_INT:syntax_Label     (list,            lbls); break;
           case TOKEN_KW_OPTION:syntax_userOPTION ();    break;
           case TOKEN_COLON:    parser_GetToken   ();    break;
           case TOKEN_EOL:      parser_GetToken   ();    break;
           case TOKEN_NULL:     parser_GetToken   ();    break;
           default: error (KEYWORD_EXPECTED);
        }
     }
  }
/* -------------------------------------------------------------------------- */
  static void syntax_BlockCheck(stmt_List_t *list,parser_TokenType end_token1,
                                                  parser_TokenType end_token2,
                                                  parser_TokenType end_token3,
                                                  var_Arr_t *vars,
                                                  arr_Arr_t *arrays,
                                                  lab_Arr_t *lbls)
  {
     while (::parser_CurToken != end_token1 &&
            ::parser_CurToken != end_token2 &&
            ::parser_CurToken != end_token3   )
     {
        switch (::parser_CurToken)
        {
           case TOKEN_KW_DO:
           case TOKEN_KW_FOR:
           case TOKEN_KW_WHILE:
           case TOKEN_KW_IF:    syntax_CBranch   (list,vars,arrays,lbls); break;
           case TOKEN_IDENT:
           case TOKEN_KW_LET:   syntax_stmtLET   (list,vars,arrays,lbls); break;
           case TOKEN_KW_PRINT: syntax_stmtPRINT (list,vars,arrays,lbls); break;
           case TOKEN_KW_INPUT: syntax_stmtINPUT (list,vars,arrays,lbls); break;
           case TOKEN_KW_GOTO:  syntax_stmtGOTO  (list,            lbls); break;
           case TOKEN_KW_DIM:   syntax_stmtDIM   (     vars,arrays     ); break;
           case TOKEN_KW_CALL:  syntax_stmtCALL  (list                 ); break;
#ifndef DISABLE_SDL_GRAPHICS
           case TOKEN_KW_SCREEN:syntax_stmtSCREEN(list,vars,arrays,lbls); break;
           case TOKEN_KW_PRESET:syntax_stmtPRESET(list,vars,arrays,lbls); break;
           case TOKEN_KW_LINE:  syntax_stmtLINE  (list,vars,arrays,lbls); break;
           case TOKEN_KW_CIRCLE:syntax_stmtCIRCLE(list,vars,arrays,lbls); break;
#endif // DISABLE_SDL_GRAPHICS
           case TOKEN_KW_CLS:   syntax_stmtCLS   (list                 ); break;
           case TOKEN_KW_END:   syntax_stmtEND   (list                 ); break;
           case TOKEN_KW_BEEP:  syntax_stmtBEEP  (list                 ); break;
           case TOKEN_KW_CLEAR: syntax_stmtCLEAR (list                 ); break;
           case TOKEN_CONST_INT:syntax_Label     (list,            lbls); break;
           case TOKEN_KW_OPTION:syntax_userOPTION ();    break;
           case TOKEN_COLON:    parser_GetToken   ();    break;
           case TOKEN_EOL:      parser_GetToken   ();    break;
           case TOKEN_NULL:     parser_GetToken   ();    break;
           default: error (KEYWORD_EXPECTED);
        }
     }
  }
/* -------------------------------------------------------------------------- */
  void syntax_Check()
  {
     pool.alloc<stmt_Node_t>()->Begin (stmt_list);
     while (::parser_CurToken != TOKEN_EOF)
     {
        switch (::parser_CurToken)
        {
           case TOKEN_KW_DO:
           case TOKEN_KW_FOR:
           case TOKEN_KW_WHILE:
           case TOKEN_KW_IF:
              syntax_CBranch    (stmt_list,pool.GetCurVars(),
                                           pool.GetCurArrs(),
                                           pool.GetCurLbls()); break;
           case TOKEN_IDENT:
           case TOKEN_KW_LET:
              syntax_stmtLET    (stmt_list,pool.GetCurVars(),
                                           pool.GetCurArrs(),
                                           pool.GetCurLbls()); break;
           case TOKEN_KW_PRINT:
              syntax_stmtPRINT  (stmt_list,pool.GetCurVars(),
                                           pool.GetCurArrs(),
                                           pool.GetCurLbls()); break;
           case TOKEN_KW_INPUT:
              syntax_stmtINPUT  (stmt_list,pool.GetCurVars(),
                                           pool.GetCurArrs(),
                                           pool.GetCurLbls()); break;
           case TOKEN_KW_GOTO:
              syntax_stmtGOTO   (stmt_list,pool.GetCurLbls()); break;
           case TOKEN_KW_DIM:
              syntax_stmtDIM    (          pool.GetCurVars(),
                                           pool.GetCurArrs()); break;
           case TOKEN_KW_SUB:
              syntax_stmtSUB    (stmt_list                  ); break;
           case TOKEN_KW_CALL:
              syntax_stmtCALL   (stmt_list                  ); break;
#ifndef DISABLE_SDL_GRAPHICS
           case TOKEN_KW_SCREEN:
              syntax_stmtSCREEN (stmt_list,pool.GetCurVars(),
                                           pool.GetCurArrs(),
                                           pool.GetCurLbls()); break;
           case TOKEN_KW_PRESET:
              syntax_stmtPRESET (stmt_list,pool.GetCurVars(),
                                           pool.GetCurArrs(),
                                           pool.GetCurLbls()); break;
           case TOKEN_KW_LINE:
              syntax_stmtLINE   (stmt_list,pool.GetCurVars(),
                                           pool.GetCurArrs(),
                                           pool.GetCurLbls()); break;
           case TOKEN_KW_CIRCLE:
              syntax_stmtCIRCLE (stmt_list,pool.GetCurVars(),
                                           pool.GetCurArrs(),
                                           pool.GetCurLbls()); break;
#endif // DISABLE_SDL_GRAPHICS
           case TOKEN_KW_CLS:
              syntax_stmtCLS    (stmt_list                  ); break;
           case TOKEN_KW_END:
              syntax_stmtEND    (stmt_list                  );
                                parser_GetToken   ();          break;
           case TOKEN_KW_BEEP:
              syntax_stmtBEEP   (stmt_list                  ); break;
           case TOKEN_KW_CLEAR:
              syntax_stmtCLEAR  (stmt_list                  ); break;
           case TOKEN_CONST_INT:
              syntax_Label      (stmt_list,pool.GetCurLbls()); break;
           case TOKEN_KW_OPTION:
              syntax_userOPTION ();                            break;
           case TOKEN_COLON:    parser_GetToken   ();          break;
           case TOKEN_EOL:      parser_GetToken   ();          break;
           case TOKEN_NULL:     parser_GetToken   ();          break;
           default: error (KEYWORD_EXPECTED);
        }
     }
     syntax_LabelsMustBeValid ();
     pool.alloc<stmt_Node_t>()->End (stmt_list);
#ifndef DISABLE_TRACING
     if (user_opt.is_tracing_tree())
        stmt_PrintTree (stmt_list);
#endif // DISABLE_TRACING
  }
/* -------------------------------------------------------------------------- */
