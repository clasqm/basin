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

#ifndef SUBROUTINE_H_INCLUDED
#define SUBROUTINE_H_INCLUDED

/* -------------------------------------------------------------------------- */
#include "common.h"
/* -------------------------------------------------------------------------- */
#include "label.h"
#include "lib.h"
#include "array.h"
#include "variable.h"
#include "statement.h"
#include "pool.h"
#include "error_handler.h"
/* -------------------------------------------------------------------------- */
  class Subroutine {

     public:
        Subroutine (_size_t const index):
           index_in_list(index),exits_count(0),runtime_calls_count(0)
        {
           returns_stack               = pool.alloc<stmt_List_t>();
           local_list                  = pool.alloc<stmt_List_t>();

           cur_vals_stack              = pool.alloc<val_Stack_t>();
           val_Arr_t *first_run_values = pool.alloc<val_Arr_t>();

           cur_vals_stack->push_back(first_run_values);
           pool.getValsStack()->push_back(cur_vals_stack);

           arrs_vals_stack = pool.alloc<val_MStack_t>();
           pool.getArrValsStack()->push_back(arrs_vals_stack);
        };
        ~Subroutine() {
        };

       _bool_t is_funtion  () const {return is_function;};
       _bool_t isRecursive () const {return is_recursive;};

       void setName ( const string *const s )   {name = *s;};
       void setVars ( var_Arr_t *const vars )   {variables = vars;};
       void setArrs ( arr_Arr_t *const arrs )   {arrays = arrs;};
       void setLabs ( lab_Arr_t *const labs )   {labels = labs;};
       void setAddr ( stmt_Node_t *const addr ) {address = addr;};
       void setExit ( stmt_Node_t *const exit ) {exit_addr = exit;};
       void setRecursive ( const _bool_t val )  {is_recursive = val;};
       /// ---------------------------------------------------------------- ///
       ///Used in syntax_stmtCALL
       // This function adding label after CALL statement, that being used in
       // runtime part as return place after calling a function
       void addExitFromCall(stmt_Node_t *const exit){
          returns_stack->push_back(exit);
          if (returns_stack->size() > 1)
            ++exits_count;
       };
       /// ---------------------------------------------------------------- ///
       ///Used in interp_stmtCALL
       // Function linking exit_addr to label in code,that is return address
       // from function for current call.
       void linkToNextExit () {
          assert (!returns_stack->empty());

          if (returns_stack->at(exits_count) == exit_addr->GetJumpAddress())
            return;

          exit_addr->LinkToLabel(returns_stack->at(exits_count));
       };
       /// ---------------------------------------------------------------- ///
       ///Used in interp_stmtCALL
       void createMemPagesForData () {
          assert(!cur_vals_stack->empty());

          bool must_increment_runtime_calls_count(false);
          if (!cur_vals_stack->at(runtime_calls_count)->empty()) // First call
             must_increment_runtime_calls_count = true;

          /// First reserve memory page for variables
          val_Arr_t *page     = pool.alloc<val_Arr_t>();
          // Really need this step? May be in future to add try-catch block?
          page->reserve (variables->size());
          /// FIXME not sure that all variables has types at this point
          _size_t j(0);
          for (var_Arr_t::iterator i = variables->begin();
                                   i < variables->end();   ++i, ++j) {
             if (variables->at(j)->isValueNULL()) {
                Value *val = pool.alloc<Value>(variables->at(j)->GetType());
                page->push_back(val);
             }
             else {
                Value *val = pool.alloc<Value>(
                                       variables->at(j)->GetValue()->GetType());
                page->push_back(val);
             }
          }
          setElementsToZero(page);
          cur_vals_stack->push_back(page);
          /// And then for arrays (if any)
          if (!arrays->empty())
          {
             assert(!arrs_vals_stack->empty());
             size_t stack_size = arrays->size();
             size_t array_size(0);
             for (size_t i = 0; i < stack_size; ++i)
             {  // Each element in arrs_vals_stack is collection of previous
                // runtime values for certain array. When adding next values for
                // this array, we need to reserve size of the memory window,
                // which is depends of array size.
                val_Arr_t* arr_vals = pool.alloc<val_Arr_t>();
                array_size = arrays->at(i)->getSize();
                // Really need this step?
                arr_vals->reserve(array_size);
                for (arr_Arr_t::iterator it = arrays->begin();
                                         it < arrays->end();   ++it) {
                   Value *val = pool.alloc<Value>(/* FIXME,(*it)->GetType()
                                                                        ??? */);
                   arr_vals->push_back(val);
                }
                setElementsToZero(arr_vals);
                arrs_vals_stack->at(i)->push_back(arr_vals);
             }
          }
          //FIXME may be this operation need to be in separate function in some
          //FIXME special place,for example,to use it from interp.cpp.
          if (must_increment_runtime_calls_count)
             ++runtime_calls_count;
       };
       /// ---------------------------------------------------------------- ///
       ///Used in interp_stmtCALL
       // This function is needed for saving all previous values of function
       // stack,when running functions recursively
       void setMemPagesForData () {
          /// First set memory page for variables
          val_Arr_t *cur_vals = cur_vals_stack->at(runtime_calls_count);
          assert(cur_vals->size() == variables->size());

               var_Arr_t::iterator var_iter = variables->begin();
          for (val_Arr_t::iterator val_iter = cur_vals->begin();
                                   val_iter < cur_vals->end();
                                                                  ++var_iter,
                                                                  ++val_iter )
          {
             //FIXME Прорядок создания переменных = порядку создания значений?
             //assert ( (*var_iter)->GetType() == (*val_iter)->GetType() );
             (*var_iter)->SetValue(*val_iter);
          }
          /// Then for arrays (if any)
          if (!arrays->empty()) {
             size_t stack_size = arrs_vals_stack->size();
             assert (arrays->size() == stack_size);
             for (size_t i = 0; i < stack_size; ++i)
             {
                val_Arr_t *curr_arr_vals = arrs_vals_stack->at(i)->
                                                        at(runtime_calls_count);
                // Elements are already set to zero in moment of use of this
                // function. We will not changing any signs of initialisation
                // because when calling function recursively it is not needed.
                arrays->at(i)->SetElements(curr_arr_vals);
             }
          }
       };
       /// ---------------------------------------------------------------- ///
       void destroyMemPagesForData () {
          /// Destroying variables memory window
          val_Arr_t *cur_vals = cur_vals_stack->at(runtime_calls_count);

          for (val_Arr_t::iterator it = cur_vals->begin();
                                   it < cur_vals->end();   ++it)
             pool.free(*it);

          pool.free (cur_vals);
          cur_vals_stack->pop_back();// FIXME Possibly double free
          /// Destroying arrays memory window (if any)
          if (!arrays->empty()) {
             size_t stack_size = arrs_vals_stack->size();
             for (size_t i = 0; i < stack_size; ++i)
             {
                val_Arr_t *cur_arr_vals = arrs_vals_stack->at(i)->
                                                        at(runtime_calls_count);

                for (val_Arr_t::iterator it = cur_arr_vals->begin();
                                         it < cur_arr_vals->end();   ++it)
                   pool.free(*it);

                pool.free (cur_arr_vals);
                arrs_vals_stack->at(i)->pop_back(); //FIXME Possibly double free
             }
          }
          --runtime_calls_count;
       }
       /// ---------------------------------------------------------------- ///

       std::string  GetName()  const {return name;};
       _size_t      getIndex() const {return index_in_list;};

       stmt_Node_t *getAddr() const {return address;};
       stmt_Node_t *getExit() const {return exit_addr->GetJumpAddress();};

       //val_Stack_t *get
       var_Arr_t   *getVars() const {return variables;};
       arr_Arr_t   *getArrs() const {return arrays;};
       lab_Arr_t   *getLabs() const {return labels;};

       stmt_List_t *getStmtList() const {return local_list;};
       _long_t      getRuntimeCallsCount() const {return runtime_calls_count;};

       void         freeOwnListAndData()  {
          pool.free(returns_stack);
          pool.freeStmtList(local_list);
          local_list->erase(local_list->begin(),local_list->end());
          pool.free(local_list);
       };

       void resVarValsWindow(const size_t &size) const {
          if (!runtime_calls_count)
             cur_vals_stack->at(runtime_calls_count)->resize(size);
       };

     private:
        _bool_t      is_function; // Function have returnable value
        std::string  name;
        stmt_Node_t *address;
        _size_t     index_in_list;

        stmt_Node_t *exit_addr;
        stmt_List_t *returns_stack;
        size3        exits_count;

        _bool_t      is_recursive;
        size3        runtime_calls_count;

        // Run-time values stack for Vaiables in subroutine list (exlude shared)
        val_Stack_t     *cur_vals_stack; //FIXME!Decide where it will be created
        val_MStack_t    *arrs_vals_stack;//FIXME!Decide where it will be created
        var_Arr_t       *variables;
        arr_Arr_t       *arrays;
        lab_Arr_t       *labels;
        var_Arr_t       *call_parameters;

        stmt_List_t *local_list;
        /// ---------------------------------------------------------------- ///
        void setValueToZero    (Value *val) {
           switch (val->GetType())
           {
              case INT:   val->Set_ival(0);  break;
              case LINT:  val->Set_lival(0); break;
              case FLOAT: val->Set_fval(0);  break;
              case DOUBLE:val->Set_dval(0);  break;
              case STRING:val->Set_sval(""); break;


              case VOID:
                 /* Do nothing.Type will be
                    set in assign operation
                  */                         break;

              default: error(UNKNOWN_TYPE);
           }
        };
        /// ---------------------------------------------------------------- ///
        void setElementsToZero (val_Arr_t *elements) {
           for (val_Arr_t::iterator i = elements->begin();
                                    i < elements->end();   ++i)
              setValueToZero(*i);
        };
        /// ---------------------------------------------------------------- ///
        void setElementsToZero (val_Stack_t *elements) {
           for (val_Stack_t::iterator i = elements->begin();
                                      i < elements->end();   ++i)
              for (val_Arr_t::iterator j = (*i)->begin(); j < (*i)->end(); ++j)
                 setValueToZero(*j);
        };
  };
/* -------------------------------------------------------------------------- */
  class Function : public Subroutine {
     private:
        Types     returnable_type;
        union {
           struct {
              Variable *var;
           } var;
           struct {
              Array    *arr;
           } array;
           struct {
              Value    *val;
           } value;
        } returnable;
  };
/* -------------------------------------------------------------------------- */
#endif // SUBROUTINE_H_INCLUDED
