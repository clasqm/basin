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

#ifndef STATEMENT_H_INCLUDED
#define STATEMENT_H_INCLUDED

/* -------------------------------------------------------------------------- */
#include "common.h"
  using std::vector;

/* -------------------------------------------------------------------------- */
 /** Here is all program before running */
  extern  stmt_List_t *stmt_list;
 /** Last created statement */
  extern  stmt_Node_t *last_stmt;
/* -------------------------------------------------------------------------- */
  namespace stmt {
     namespace let {
        namespace vartype {
           enum Type {VAR,ARRAY};
        }
     }
  }
/* -------------------------------------------------------------------------- */
  typedef class Statement
  {
     public:/** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
        Statement ();
        ~Statement(){};
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        stmt_Node_t *NewStmtLET (Variable *left,expr_Node_t *right,
                                                             stmt_List_t *list);
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        stmt_Node_t *NewStmtLET (Array *left,expr_List_t *arr_indexes,
                                         expr_Node_t *right,stmt_List_t *list );
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        stmt_Node_t *NewStmtCALL  (ProcKind kind,expr_List_t *expr_list,
                                   stmt_List_t *list,
                                   size3 index = 0,string *name = NULL);
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        stmt_Node_t *NewStmtRETURN(ProcKind k,expr_List_t *expr_list,
                                   stmt_List_t *list,string *name,
                                   _bool_t is_recursive = false      );
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        stmt_Node_t *NewStmtINPUT (string *hint,expr_List_t *var_list,
                                      _bool_t *have_dotcomma,stmt_List_t *list);
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        stmt_Node_t *NewCBranch   (expr_Node_t *expr,stmt_List_t *list);
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        stmt_Node_t *LinkCBRLabels(stmt_Node_t *go_on_true,
                                                      stmt_Node_t *go_on_false);
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        stmt_Node_t *NewLabel        (stmt_List_t *list);
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        stmt_Node_t *NewEmptyLabel   (stmt_List_t *list);
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        stmt_Node_t *LinkToLabel (stmt_Node_t *addr);
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        stmt_Node_t *Begin (stmt_List_t *list);
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        stmt_Node_t *End (stmt_List_t *list);
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        expr_Node_t  *GetStmtExpr        () const;
        StatementKind GetKind            () const {return kind;};
        ProcKind      GetFuncKind        () const {return data.function.kind;};
        string       *GetFuncName        () const {return data.function.name;};
        stmt_Node_t  *GetNext            () const {return next;};
        stmt_Node_t  *GetJumpAddress     () const;
        Variable     *GetStmtVar         () const;
        Array        *GetArrayPtr        () const;
        line_count_t  GetCurLine         () const {return cur_line;};
        string        GetCurFile         () const {return cur_file;};
        stmt_Node_t  *GetCBrLabel        (_bool_t choice) const;
        _bool_t       isReturnFromRecursion () const;
        /// Expression list vector for statement
        expr_List_t  *GetParameters      () const;
        string        GetHint            () const;
        _bool_t       PrintQuestionMark  () const;
        str_Arr_t    *GetStrArr          () const;
        sz4_MDim_t   *GetMDimArr         () const;
        sz3_MDim_t   *GetDimStartIndexes () const;
        stmt::let::vartype::Type GetLetMode () const;
#ifndef DISABLE_TRACING
        _bool_t       is_tracing_tree    () const {return trace.trace_tree;};
        void Printer();
#endif // DISABLE_TRACING
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
        void        setNextStmt (stmt_Node_t *next_stmt){next = next_stmt;};
        void        SetStmtVar(Value *val,_size_t index = 0);
        void        SetStmtVar(Value const &val,_size_t index = 0);
        void        memFree   ();
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
     private:/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
        line_count_t cur_line;// Line in source
        string cur_file;      // File where statement located
        stmt_Node_t *next;    // Following statement
        StatementKind kind;   // Тип statement'а
#ifndef DISABLE_TRACING
        struct {
           _bool_t trace_act;
           _bool_t trace_tree;
        } trace;
#endif // DISABLE_TRACING
        union
        {  // Statements data
           struct { // Данные для LET
              stmt::let::vartype::Type el_type;//What is in union-var,or arr_el?
              union {                    ///   @lvalue {
                 struct {                /// lvalue is variable
                    Variable    *var;    // Left side is Variable
                 } var;
                 struct {                /// lvalue is array element
                    Array *array;
                    expr_List_t *indexes;// List of expressions to calculate
                 } arr_el;               // index
              } data;                    ///   @lvalue }
              expr_Node_t *expr;         /// { @rvalue }
           } let;
           struct { // Conditional jump (IF,WHILE)
              expr_Node_t *relation_expr;
              stmt_Node_t *on_true;
              stmt_Node_t *on_false;
           } cbranch;
           struct { // Label
              stmt_Node_t *address;
           } label;
           struct { // Data for INPUT
              string *hint;
              _bool_t *print_question_mark;
              expr_List_t *variables;
           } input;
           struct { // Data for CALL (SUB, FUNCTION) and RETURN
              ProcKind kind;
              expr_List_t *params;//FIXME!!!Add params stack for recursion calls
              string *name;
              _bool_t is_recursive_call;
           } function;
        } data;
#ifndef DISABLE_TRACING
        void          PrintThis          () const;
        void          PrintAddress       () const;
#endif // DISABLE_TRACING
  } stmt_Node_t;
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_TRACING
  void stmt_PrintTree (stmt_List_t *list);
#endif // DISABLE_TRACING
/* -------------------------------------------------------------------------- */
#endif // STATEMENT_H_INCLUDED
