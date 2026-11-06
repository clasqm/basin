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

#include <cmath>
#include "common.h"

#include "expression.h"
#include "array.h"
#include "options.h"
#include "lib.h"
#include "pool.h"
#include "error_handler.h"
/* -------------------------------------------------------------------------- */
  Expression::Expression(ExprKind ek,string name,expr_List_t *ind)
  {
     cur_line = parser_CurLine;
     cur_file = parser_CurFile;
     assert(ek == EK_ARR_EL);
     kind                 = EK_ARR_EL;
     data.arr_el.arr_name = pool.alloc<std::string>(name);
     data.arr_el.indexes  = ind;
  }
/* -------------------------------------------------------------------------- */
  Expression::Expression (ExprKind ek,Variable *var)
  {
     cur_line = parser_CurLine;
     cur_file = parser_CurFile;
     assert (ek == EK_VAR);
     kind         = ek;
     data.var.var = var;
  }
/* -------------------------------------------------------------------------- */
  Expression::Expression (ExprKind ek,std::string *str,Types t)
  {
     cur_line = parser_CurLine;
     cur_file = parser_CurFile;
     assert (ek == EK_CONST);
     kind              = ek;

     /// FIXME Move this to run-time
     data.constant.val = pool.alloc<Value>(*str,t);
  }
/* -------------------------------------------------------------------------- */
  Expression::Expression (ExprKind ek,expr_Node_t *operand, OpKind oper)
  {
     cur_line = parser_CurLine;
     cur_file = parser_CurFile;
     assert(ek == EK_UNOP);
     kind              = EK_UNOP;
     data.unop.kind    = oper;
     data.unop.operand = operand;
  }
/* -------------------------------------------------------------------------- */
  Expression::Expression (ExprKind ek,expr_Node_t *operand1,
                                      expr_Node_t *operand2, OpKind oper)
  {
     cur_line = parser_CurLine;
     cur_file = parser_CurFile;
     assert(ek == EK_BINOP);
     kind                = EK_BINOP;
     data.binop.kind     = oper;
     data.binop.operand1 = operand1;
     data.binop.operand2 = operand2;
  }
/* -------------------------------------------------------------------------- */
  Expression::Expression (ExprKind ek,ProcKind pk,expr_List_t *parameters)
  {
     cur_line = parser_CurLine;
     cur_file = parser_CurFile;
     assert(ek == EK_PROC);
     kind                      = EK_PROC;
     data.procedure.params     = parameters;
     data.procedure.kind       = pk;
  }
/* -------------------------------------------------------------------------- */
  Value Expression::GetVarValue (var_Arr_t *list) const
  {
     string str = data.var.var->GetName();
     count i = findVarName (str,list);
     Value wrong;
     if (i < 0)
     {  // Variable not found in local list.
        //Trying to search in shared variables
        i = findVarName (str, pool.GetSharedVars());
        list = pool.GetSharedVars();
        assert (i >= 0);
     }
     if (user_opt.check("--noinit-errors"))  {// Initialization control option
        if (!list->at(i)->IsInitialized()) {
           runtime_error(NON_INIT_VAR,str,cur_file,cur_line);
        }
        else {
           return *(list->at(i)->GetValue());
        }
     }
     else {
        if (!list->at(i)->IsInitialized()) {
           Types t = list->at(i)->GetType();
           if (list->at(i)->isValueNULL()) {
              Value *zero   = pool.alloc<Value>("0",t);
              list->at(i)->SetValue(zero);
              return *zero;
           }
           Value zero("0",t);
           list->at(i)->SetValue(zero);
           return zero;
        }
        else {
           return *(list->at(i)->GetValue());
        }
     }
     assert(0);
     return wrong;
  }
/* -------------------------------------------------------------------------- */
  Value Expression::CalcUnOp (OpKind k,Value operand) const
  {
     Value result;
     switch (k)
     {
        case OP_MINUS:  result = -operand;
                        return result;
        case OP_NOT:    result = ~operand;
                        return result;
        default:        assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
  Value Expression::CalcBinOp (OpKind k,Value operand1,Value operand2 ) const
  {
     switch (k)
     {
        case OP_PLUS:           return operand1 + operand2;
        case OP_MINUS:          return operand1 - operand2;
        case OP_MULTIPLY:       return operand1 * operand2;
        case OP_DIVIDE:         return operand1 / operand2;
        case OP_XOR:            return operand1 ^ operand2;
        case OP_OR:             return operand1 | operand2;
        case OP_AND:            return operand1 & operand2;
        case OP_REL_LESS:       return operand1 < operand2;
        case OP_REL_LESS_EQUAL: return operand1 <= operand2;
        case OP_REL_GREAT:      return operand1 > operand2;
        case OP_REL_GREAT_EQUAL:return operand1 >= operand2;
        case OP_REL_OR:         return operand1 || operand2;
        case OP_REL_AND:        return operand1 && operand2;
        case OP_REL_EQUAL:      return operand1 == operand2;
        case OP_REL_NOT_EQUAL:  return operand1 != operand2;
        default:assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
  void Expression::memFree ()
  {
     switch (kind)
     {
        case EK_ARR_EL:pool.free(data.arr_el.arr_name);
                       for (size3 i = data.arr_el.indexes->size()-1;i >= 0; --i)
                       {
                          data.arr_el.indexes->at(i)->memFree();
                          pool.free(data.arr_el.indexes->at(i));
                       }
                       pool.free(data.arr_el.indexes);
        break;
        case EK_CONST: pool.free(data.constant.val);
        break;
        case EK_VAR:   // Variables memory will be freed in memory manager.
        break;
        case EK_UNOP:  data.unop.operand->memFree();
                       pool.free(data.unop.operand);
        break;
        case EK_BINOP: data.binop.operand1->memFree();
                       pool.free(data.binop.operand1);
                       data.binop.operand2->memFree();
                       pool.free(data.binop.operand2);
        break;
        case EK_PROC:  for (size3 i = data.procedure.params->size()-1;
                                                                    i >= 0; --i)
                       {
                          data.procedure.params->at(i)->memFree();
                          pool.free(data.procedure.params->at(i));
                       }
                       pool.free(data.procedure.params);
        break;
        default: assert (0);
     }
  }
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_TRACING
  void Expression::Printer() const
  {
     if (!user_opt.is_trace_silent())
     {
        cout << ExprKindNames[ExprKind(kind)]               << ' ';
        switch (kind)
        {
           case EK_CONST:
              cout << '(';
              data.constant.val->Printer();
              cout << ')';
           break;
           case EK_VAR:
              cout << '(';
              data.var.var->Printer();
              cout << ')';
           break;
           case EK_ARR_EL:
              cout << '(' << *data.arr_el.arr_name << '[';
              expr_ListPrint (data.arr_el.indexes);
              cout << ']' << ')';
           break;
           case EK_UNOP:
              cout << OpKindNames[OpKind(data.unop.kind)]  << ' '
                                                           << '(';
              data.unop.operand->Printer();
              cout                                         << ')';
           break;
           case EK_BINOP:
              cout << OpKindNames[OpKind(data.binop.kind)] << ' '
                                                           << '(';
              data.binop.operand1->Printer();
              cout                                         << ')'
                                                           << '(';
              data.binop.operand2->Printer();
              cout                                         << ')';
           break;
           case EK_PROC:
              cout << '(' << ProcKindNames[ProcKind(data.procedure.kind)]
                   << '(';
                   expr_ListPrint (data.procedure.params);
              cout << ')' << ')';
           break;
           default:assert(0);
        }
        return;
     }
     trace_file << ExprKindNames[ExprKind(kind)]               << ' ';
     switch (kind)
     {
        case EK_CONST:
           trace_file << '(';
           data.constant.val->Printer();
           trace_file << ')';
        break;
        case EK_VAR:
           trace_file << '(';
           data.var.var->Printer();
           trace_file << ')';
        break;
        case EK_ARR_EL:
           trace_file << '(' << *data.arr_el.arr_name << '[';
           expr_ListPrint (data.arr_el.indexes);
           trace_file << ']' << ')';
        break;
        case EK_UNOP:
           trace_file << OpKindNames[OpKind(data.unop.kind)]  << ' '
                                                              << '(';
           data.unop.operand->Printer();
           trace_file                                         << ')';
        break;
        case EK_BINOP:
           trace_file << '('
                      << OpKindNames[OpKind(data.binop.kind)] << ' '
                                                              << '(';
           data.binop.operand1->Printer();
           trace_file                                         << ')'
                                                              << '(';
           data.binop.operand2->Printer();
           trace_file                                         << ')'
                                                              << ')';
        break;
        case EK_PROC:
           trace_file << '(' << ProcKindNames[ProcKind(data.procedure.kind)]
                      << '(';
                      expr_ListPrint (data.procedure.params);
           trace_file << ')' << ')';
        break;
        default: assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
  void expr_ListPrint (expr_List_t *list)
  {
     size3 size = list->size();
     if (!user_opt.is_trace_silent())
     {
        if (!size) {
           std::cout << "empty";
           return;
        }
        for (size3 i = 0; i < size; ++i)
        {
           list->at(i)->Printer();
           if (i != size-1)
             std::cout << ',';
        }
        return;
     }
     if (!size) {
        trace_file << "empty";
        return;
     }
     for (size3 i = 0; i < size; ++i)
     {
        list->at(i)->Printer();
        if (i != size-1)
          trace_file << ',';
     }
  }
/* -------------------------------------------------------------------------- */
#endif // DISABLE_TRACING
/* -------------------------------------------------------------------------- */
