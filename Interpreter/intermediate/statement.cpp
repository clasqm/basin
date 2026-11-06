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

#include "statement.h"
#include "value.h"
#include "variable.h"
#include "expression.h"
#include "subroutine.h"
#include "parser.h"
#include "array.h"
#include "options.h"
#include "tracer.h"
#include "lib.h"
#include "pool.h"
/* -------------------------------------------------------------------------- */
  stmt_Node_t *last_stmt(NULL);
  //Statement list of the program
  stmt_List_t *stmt_list = pool.alloc<stmt_List_t>();
/* -------------------------------------------------------------------------- */
  Statement::Statement ()
  {
     cur_line = parser_CurLine;
     cur_file = parser_CurFile;
#ifndef DISABLE_TRACING
     trace.trace_act  = user_opt.GetTraceAct();
     trace.trace_tree = user_opt.is_tracing_tree();
#endif // DISABLE_TRACING
     ///FIXME!!!
     ///FIXME!!!
     ///FIXME!!!
     // FIXME!!! There is a bug with function's standalone lists. Next in global
     // list is not always next statement,when calling a function!
     if (last_stmt != NULL)
     {
        last_stmt->next = this;
        last_stmt = this;
     }
     else last_stmt = this;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::Begin (stmt_List_t *list)
  {
     kind = SK_BEGIN;
     list->push_back(this);
     return this;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::End (stmt_List_t *list)
  {
     kind = SK_PROGRAM_END;
     list->push_back(this);
     return this;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::NewStmtLET (Variable *left,expr_Node_t *right,
                                                              stmt_List_t *list)
  {
     kind                   = SK_LET;
     data.let.el_type       = stmt::let::vartype::VAR;
     data.let.data.var.var  = left;
     data.let.expr          = right;
     list->push_back(this);
     return this;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::NewStmtLET (Array *left,expr_List_t *arr_indexes,
                                           expr_Node_t *right,stmt_List_t *list)
  {
     kind                         = SK_LET;
     data.let.el_type             = stmt::let::vartype::ARRAY;
     data.let.data.arr_el.array   = left;
     data.let.data.arr_el.indexes = arr_indexes;
     data.let.expr                = right;
     list->push_back(this);
     return this;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::NewStmtCALL (ProcKind k,expr_List_t *expr_list,
                                       stmt_List_t *list,
                                       size3 index,string *name)
  {
     kind                            = SK_CALL;
     data.function.kind              = k;
     data.function.params            = expr_list;
     data.function.name              = name;
     if (k == PK_USER)
        last_stmt->next = pool.GetCurFuncs()->at(index)->getAddr();
     list->push_back(this);
     return this;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::NewStmtRETURN (ProcKind k,expr_List_t *expr_list,
                                         stmt_List_t *list,string *name,
                                         _bool_t is_recursive)
  {
     kind                            = SK_RETURN;
     data.function.kind              = k;
     data.function.params            = expr_list;
     data.function.name              = name;
     data.function.is_recursive_call = is_recursive;
     list->push_back(this);
     return this;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::NewStmtINPUT (string *hint,expr_List_t *var_list,
                                       _bool_t *have_dotcomma,stmt_List_t *list)
  {
     kind                           = SK_INPUT;
     data.input.hint                = hint;
     data.input.variables           = var_list;
     data.input.print_question_mark = have_dotcomma;
     list->push_back(this);
     return this;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::NewCBranch (expr_Node_t *expr,stmt_List_t *list)
  {
     kind = SK_CBRANCH;
     data.cbranch.relation_expr = expr;
     list->push_back(this);
     return this;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::LinkCBRLabels(stmt_Node_t *go_on_true,
                                        stmt_Node_t *go_on_false)
  {
     data.cbranch.on_true  = go_on_true;
     data.cbranch.on_false = go_on_false;
     return this;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::NewLabel (stmt_List_t *list)
  {
     kind = SK_LABEL;
     list->push_back(this);
     data.label.address = NULL;
     return this;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::NewEmptyLabel (stmt_List_t *list)
  {
     kind = SK_EMPTY_LABEL;
     list->push_back(this);
     return this;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::LinkToLabel (stmt_Node_t *addr)
  {
     data.label.address = addr;
     return this;
  }
/* -------------------------------------------------------------------------- */
  expr_Node_t *Statement::GetStmtExpr () const
  {
     switch (kind)
     {
        case SK_LET:     return data.let.expr;
        case SK_CBRANCH: return data.cbranch.relation_expr;
        default: assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::GetCBrLabel (_bool_t choice) const
  {
     assert(kind == SK_CBRANCH);
     switch (choice)
     {
        case true:  return data.cbranch.on_true;
        case false: return data.cbranch.on_false;
        default: assert (0);
     }
     return data.cbranch.on_true;
  }
/* -------------------------------------------------------------------------- */
  _bool_t Statement::isReturnFromRecursion () const
  {
     if (data.function.is_recursive_call) return true;
     return false;
  }
/* -------------------------------------------------------------------------- */
  stmt_Node_t *Statement::GetJumpAddress() const
  {
     switch (kind)
     {
        case SK_LABEL:return data.label.address;
        default: assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
  expr_List_t *Statement::GetParameters () const
  {
     switch (kind)
     {
        case SK_LET:   return data.let.data.arr_el.indexes;
        case SK_INPUT: return data.input.variables;
        case SK_CALL:  return data.function.params;
        default: assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
  Array *Statement::GetArrayPtr () const
  {
     switch (kind)
     {
        case SK_LET: return data.let.data.arr_el.array;
        default: assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
  Variable *Statement::GetStmtVar () const
  {
     switch (kind)
     {
        case SK_LET: assert(data.let.el_type == stmt::let::vartype::VAR);
                     return data.let.data.var.var;
        break;
        default: assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
  stmt::let::vartype::Type Statement::GetLetMode () const
  {
     assert(kind == SK_LET);
     return data.let.el_type;
  }
/* -------------------------------------------------------------------------- */
  string Statement::GetHint () const
  {
     switch (kind)
     {
        case SK_INPUT: return *(data.input.hint);
        default: assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
  _bool_t Statement::PrintQuestionMark () const
  {
     assert(kind == SK_INPUT);
     return *(data.input.print_question_mark);
  }
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_TRACING
  void Statement::PrintAddress () const
  {
     if (!user_opt.is_trace_silent())
     {
        cout <<  this  << ' ';
        return;
     }
     trace_file <<  this  << ' ';
  }
/* -------------------------------------------------------------------------- */
  void Statement::PrintThis () const
  {
     if (!user_opt.is_trace_silent())
     {
        std::cout << this << ' ';
        return;
     }
     trace_file <<  this  << ' ';
  }
#endif // DISABLE_TRACING
/* -------------------------------------------------------------------------- */
// FIXME This is runtime part of the program and not supposed to be here.
  void Statement::SetStmtVar (Value *val,_size_t index)
  {
     assert (kind == SK_LET);
     switch (data.let.el_type)
     {
        case stmt::let::vartype::VAR:
#ifndef DISABLE_TRACING
           if (trace.trace_act)
              tracer.WriteVarPrint (data.let.data.var.var->GetName(),val,
                                      cur_file,cur_line                       );
#endif // DISABLE_TRACING
           data.let.data.var.var->SetValue(val);
        break;
        case stmt::let::vartype::ARRAY:
#ifndef DISABLE_TRACING
           if (trace.trace_act)
              tracer.WriteArrPrint (data.let.data.arr_el.array->GetName(),
                                                   index,val,cur_file,cur_line);
#endif // DISABLE_TRACING
           if (data.let.data.arr_el.array->GetElements()->empty())
           {// FIXME move this to somewhere ; And add elements to pool!
              data.let.data.arr_el.array->ArrZeroInit();
              data.let.data.arr_el.array->SetZeroed(true);
           }
           data.let.data.arr_el.array->SetElement(index,val);
        break;
        default: assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
// FIXME This is runtime part of the program and not supposed to be here.
  void Statement::SetStmtVar (Value const &val,_size_t index)
  {
     assert (kind == SK_LET);
     switch (data.let.el_type)
     {
        case stmt::let::vartype::VAR:
#ifndef DISABLE_TRACING
           if (trace.trace_act)
              tracer.WriteVarPrint (data.let.data.var.var->GetName(),val,
                                      cur_file,cur_line                       );
#endif // DISABLE_TRACING
           data.let.data.var.var->SetValue(val);
        break;
        case stmt::let::vartype::ARRAY:
#ifndef DISABLE_TRACING
           if (trace.trace_act)
              tracer.WriteArrPrint (data.let.data.arr_el.array->GetName(),
                                                   index,val,cur_file,cur_line);
#endif // DISABLE_TRACING
           if (data.let.data.arr_el.array->GetElements()->empty())
           {// FIXME move this to somewhere
              data.let.data.arr_el.array->ArrZeroInit();
              data.let.data.arr_el.array->SetZeroed(true);
           }
           data.let.data.arr_el.array->SetElement(index,val);
        break;
        default: assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
  void Statement::memFree()
  {
     switch (kind)
     {
        case SK_LET:  data.let.expr->memFree();
                      pool.free(data.let.expr);
                      // Variables table will be free in memory manager.
                      if (data.let.el_type == stmt::let::vartype::ARRAY)
                      {
                         for (size3 i = data.let.data.arr_el.indexes->size()-1;
                                                                    i >= 0; --i)
                         {
                            data.let.data.arr_el.indexes->at(i)->memFree();
                            pool.free(data.let.data.arr_el.indexes->at(i));
                         }
                         pool.free(data.let.data.arr_el.indexes);
                      }
        break;
        case SK_CALL: if (data.function.name != NULL)
                         pool.free(data.function.name);
                      if (!data.function.params->empty())
                      {
                         for (count i = data.function.params->size()-1;
                                                                    i >= 0; --i)
                         {
                            data.function.params->at(i)->memFree();
                            pool.free(data.function.params->at(i));
                         }
                         pool.free(data.function.params);
                         break;
                      }
                      pool.free(data.function.params);
        break;
        case SK_RETURN:
        break;
        case SK_CBRANCH:data.cbranch.relation_expr->memFree();
                        pool.free(data.cbranch.relation_expr);
        break;
        case SK_INPUT:pool.free(data.input.hint);
                      pool.free(data.input.print_question_mark);
                      if (!data.input.variables->empty())
                      {
                         for (size3 i = data.input.variables->size()-1;
                                                                    i >= 0; --i)
                         {
                            data.input.variables->at(i)->memFree();
                            pool.free(data.input.variables->at(i));
                         }
                         pool.free(data.input.variables);
                         break;
                      }
                      pool.free(data.input.variables);
        break;
        case SK_LABEL:       // This memory will be free in stmt_FreeStmtList
        case SK_EMPTY_LABEL: // function.All these statements have no internal
        case SK_PROGRAM_END: // data,but only pointers to statements.
        case SK_BEGIN:
        break;
        default: assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_TRACING
  void Statement::Printer()
  {
     if (!user_opt.is_trace_silent())
     {
        cout << "File: "              << cur_file  << ' '
             << "Line: "              << cur_line  << ' '
             << "Address: "           << this      << ' '
             << "Kind: "
             << StmtKindNames[StatementKind(kind)] << ' ';
        switch (kind)
        {
           case SK_LET:
              cout << '(';
              if (data.let.el_type == stmt::let::vartype::VAR)
                 data.let.data.var.var->Printer();
              else
                 data.let.data.arr_el.array->Printer(data.let.data.arr_el.indexes);
              cout << ')' << ' ' << '=' << ' ';
              data.let.expr->Printer();
           break;
           case SK_CALL:
              cout << ProcKindNames[ProcKind(data.function.kind)] << ' ' << '(';
              expr_ListPrint (data.function.params);
              cout << ')';
           break;
           case SK_RETURN:
              if (ProcKind(data.function.kind) == PK_USER) {
                  cout << "return from: "<< *data.function.name
                             << ' ' << '(';
                  expr_ListPrint (data.function.params);
                  cout << ')';;
                  //break;
              }
           break;
           case SK_LABEL:
              cout << "Jump to: ";
              data.label.address->PrintAddress();
              cout << ' ';
           break;
           case SK_CBRANCH:
              cout << "Condition: "
                   << '(';
              data.cbranch.relation_expr->Printer();
              cout << ')' << " on true goto: ";
              data.cbranch.on_true->PrintThis();
              cout << " on false goto: ";
              data.cbranch.on_false->PrintThis();
           break;
           case SK_EMPTY_LABEL: break;
           case SK_INPUT:
              cout << *data.input.hint << ' ';
              if (*data.input.print_question_mark)
                 cout << '?' << ' ';
              cout << '(';
              expr_ListPrint (data.input.variables);
              cout << ')';
           break;
           case SK_BEGIN:
           case SK_PROGRAM_END: break;
           default: assert (0);
        }
        cout << endl;
        return;
     }
     trace_file << "File: "              << cur_file  << ' '
                << "Line: "              << cur_line  << ' '
                << "Address: "           << this      << ' '
                << "Kind: "
                << StmtKindNames[StatementKind(kind)] << ' ';
     switch (kind)
     {
        case SK_LET:
           trace_file << '(';
           if (data.let.el_type == stmt::let::vartype::VAR)
              data.let.data.var.var->Printer();
           else
              data.let.data.arr_el.array->Printer(data.let.data.arr_el.indexes);
           trace_file << ')' << ' ' << '=' << ' ';
           data.let.expr->Printer();
        break;
        case SK_CALL:
           if (ProcKind(data.function.kind) == PK_USER) {
               trace_file << *data.function.name
                          << ' ' << '(';
               expr_ListPrint (data.function.params);
               trace_file << ')';;
               break;
           }
           trace_file << ProcKindNames[ProcKind(data.function.kind)]
                      << ' ' << '(';
           expr_ListPrint (data.function.params);
           trace_file << ')';
        break;
        case SK_RETURN:
           if (ProcKind(data.function.kind) == PK_USER) {
               trace_file << "return from: "<< *data.function.name
                          << ' ' << '(';
               expr_ListPrint (data.function.params);
               trace_file << ')';;
               //break;
           }
        break;
        case SK_LABEL:
           trace_file << "Jump to: ";
           data.label.address->PrintAddress();
           trace_file << ' ';
        break;
        case SK_CBRANCH:
           trace_file << "Condition: " << '(';
           data.cbranch.relation_expr->Printer();
           trace_file << ')' << " on true goto: ";
           data.cbranch.on_true->PrintThis();
           trace_file << " on false goto: ";
           data.cbranch.on_false->PrintThis();
        break;
        case SK_EMPTY_LABEL: break;
        case SK_INPUT:
           trace_file << *data.input.hint << ' ';
           if (*data.input.print_question_mark)
              cout << '?' << ' ';
           trace_file << '(';
           expr_ListPrint (data.input.variables);
           trace_file << ')';
        break;
        case SK_BEGIN:
        case SK_PROGRAM_END: break;
        default: assert (0);
     }
     trace_file << endl;
  }
/* -------------------------------------------------------------------------- */
// FIXME need to move all printing non-members to
// debug/printer.cpp (or tracer.cpp)
  void stmt_PrintTree (stmt_List_t *list)
  {
     if (!list->empty())
     {
        size3 size = list->size();
        for (size3 i  = 0; i < size; ++i)
           list->at(i)->Printer();
     }
     if (!user_opt.is_trace_silent())
     {
        cout << endl << " *** END OF PROGRAM TREE *** " << endl << endl;
        return;
     }
     trace_file << endl << " *** END OF PROGRAM TREE *** " << endl << endl;
  }
/* -------------------------------------------------------------------------- */
#endif // DISABLE_TRACING
/* -------------------------------------------------------------------------- */
