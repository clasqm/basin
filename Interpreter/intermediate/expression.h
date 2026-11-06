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

#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED

/* -------------------------------------------------------------------------- */
#include "common.h"
  using std::string;

#include "value.h"
#include "variable.h"
/* -------------------------------------------------------------------------- */
extern std::string parser_CurFile;
extern count       parser_CurLine;
/* -------------------------------------------------------------------------- */
  typedef class Expression
  {
     private:
        ExprKind kind;
        line_count_t cur_line;// Line in source
        string cur_file;      // File where statement located
#ifndef DISABLE_TRACING
        _bool_t trace_enabled;
#endif // DISABLE_TRACING
        union
        {
           struct {
              string      *arr_name;// Array name
              expr_List_t *indexes; // List of indexes
           } arr_el;
           struct {
              Value *val;           // Consant value
           } constant;
           struct {
              Variable *var;        // Variable
           } var;
           struct {                 // Unary operation
              OpKind kind;
              expr_Node_t *operand;
           } unop;
           struct {                 // Binary operation
              OpKind kind;
              expr_Node_t *operand1;
              expr_Node_t *operand2;
           } binop;
           struct {                 // Built-in procedure
              ProcKind kind;
              expr_List_t *params;
           } procedure;
        } data;
     public:
        Expression() : cur_line(parser_CurLine),cur_file(parser_CurFile) {};

        Expression (ExprKind ek,string name,expr_List_t *ind);

        Expression (ExprKind ek,expr_Node_t *operand, OpKind oper);

        Expression (ExprKind ek,std::string *str,Types t);

        Expression (ExprKind ek,Variable *var);

        Expression (ExprKind ek,expr_Node_t *operand1,
                                             expr_Node_t *operand2,OpKind oper);

        Expression (ExprKind ek,ProcKind k,expr_List_t *parameters);

        ~Expression (){};
        expr_Node_t *NewConstant    (string *str,Types t);
        expr_Node_t *NewArrElement  (string name,expr_List_t *ind);
        expr_Node_t *NewVariable    (string *str,Types t);
        expr_Node_t *NewUnOp        (expr_Node_t *operand, OpKind oper);
        expr_Node_t *NewBinOp       (expr_Node_t *operand1,
                                            expr_Node_t *operand2, OpKind oper);
        expr_Node_t *NewProcedure   (ProcKind kind,expr_List_t *parameters);

        Value        GetVarValue    (var_Arr_t *list) const;
        ExprKind     GetKind        () const {return kind;};
        ProcKind     GetProcKind    () const {return data.procedure.kind;};
        Value        GetValue       () const {return *data.constant.val;};
        string      *GetArrElemName () const {return data.arr_el.arr_name;};
        expr_List_t *GetArrayIndexes() const {return data.arr_el.indexes;};
        Variable    *GetVar         () const {return data.var.var;};
        OpKind       GetBinOpKind   () const {return data.binop.kind;};
        OpKind       GetUnOpKind    () const {return data.unop.kind;};
        expr_Node_t *GetBinOp1      () const {return data.binop.operand1;};
        expr_Node_t *GetBinOp2      () const {return data.binop.operand2;};
        expr_Node_t *GetUnOperand   () const {return data.unop.operand;};
        expr_List_t *GetParameters  () const {return data.procedure.params;};
        line_count_t GetCurLine     () const {return cur_line;};
        string       GetCurFile     () const {return cur_file;};

        Value        CalcUnOp  (OpKind k,Value operand) const;
        Value        CalcBinOp (OpKind k,Value operand1,Value operand2 ) const;
#ifndef DISABLE_TRACING
        _bool_t      IsTraced       () const {return trace_enabled;};
        void         Printer        () const;
#endif // DISABLE_TRACING
        void         memFree        ();
  } expr_Node_t;
/* -------------------------------------------------------------------------- */
  void expr_ListPrint (expr_List_t *list);
/* -------------------------------------------------------------------------- */
#endif // EXPRESSION_H_INCLUDED
