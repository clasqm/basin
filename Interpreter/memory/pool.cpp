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

#include "pool.h"
#include "lib.h"
#include "subroutine.h"
/* -------------------------------------------------------------------------- */
  Pool pool;
/* -------------------------------------------------------------------------- */
  void Pool::init() {
     ///TODO Allocate stacks only when needed.For example,if interpreted
     ///     program have no arrays,what for to allocate memory?
     vars_stack      = Pool::alloc<var_Stack_t>();
     arrays_stack    = Pool::alloc<arr_Stack_t>();
     labels_stack    = Pool::alloc<lab_Stack_t>();
     functions_stack = Pool::alloc<sub_Stack_t>();

     arrs_vals_multi_stack = Pool::alloc<val_VMMS_t>();
     val_MStack_t *arrs_vals_stack = Pool::alloc<val_MStack_t>();
     val_Stack_t *global_arr_vals = Pool::alloc<val_Stack_t>();
     arrs_vals_stack->push_back(global_arr_vals);

     arrs_vals_multi_stack->push_back(arrs_vals_stack);

     vals_multi_stack               = Pool::alloc<val_MStack_t>();
     val_Stack_t *global_vals_stack = Pool::alloc<val_Stack_t>();
     val_Arr_t *global_vals         = Pool::alloc<val_Arr_t>();

     global_vals_stack->push_back(global_vals);
     vals_multi_stack->push_back(global_vals_stack);

     // Creating global variables
     var_Arr_t *global_vars   = Pool::alloc<var_Arr_t>();
     vars_stack->push_back(global_vars);
     // Creating global arrays
     arr_Arr_t *global_arrays = Pool::alloc<arr_Arr_t>();
     arrays_stack->push_back(global_arrays);
     // Creating global labels
     lab_Arr_t *global_labels = Pool::alloc<lab_Arr_t>();
     labels_stack->push_back(global_labels);
     // Creating functions
     sub_Arr_t *global_functions = Pool::alloc<sub_Arr_t>();
     functions_stack->push_back(global_functions);

     current_vars  =   vars_stack->front();
     current_arrs  = arrays_stack->front();
     current_lbls  = labels_stack->front();
     current_funcs = functions_stack->front();

     shared_vars   = Pool::alloc<var_Arr_t>();
     shared_arrs   = Pool::alloc<arr_Arr_t>();

     shared_vars_values   = Pool::alloc<val_Arr_t>();
     shared_arrs_values   = Pool::alloc<val_Stack_t>();

     has_shared_vars = false;
     has_shared_arrs = false;
  }
/* -------------------------------------------------------------------------- */
  _bool_t Pool::isConflictingDeclaration ( string declared_name, args_t args )
  {
     // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
     /// What's wrong in this algorithm? TODO
     /*
     for (args_t i = SEARCH_BY_DEFAULT; i <= DONT_SEARCH_IN_FNCS; ++i)
     {
        switch (args & i)
        {
           case SEARCH_BY_DEFAULT:{

              if (
                    ( ( !(args & DONT_SEARCH_IN_FNCS) )             &&
                      (findVarName (declared_name, current_funcs) >= 0) )
                                                                           ||
                    ( ( !(args & DONT_SEARCH_IN_VARS) )             &&
                      (findVarName (declared_name, current_vars) >= 0)  )
                                                                           ||
                    ( ( !(args & DONT_SEARCH_IN_ARRS) )             &&
                      (findVarName (declared_name, current_arrs) >= 0)  )
                                                                           ||
                    ( ( !(args & DONT_SEARCH_IN_LBLS) )             &&
                      (findVarName (declared_name, current_lbls) >= 0)  )
                 )
                 return true;
              return false;
           }
           case SEARCH_IN_ALL_DATA:{

              if (
                    ( ( !(args & DONT_SEARCH_IN_FNCS) )             &&
                      (findVarName (declared_name, current_funcs) >= 0) )
                                                                           ||
                    ( ( !(args & DONT_SEARCH_IN_VARS) )             &&
                      (findVarName (declared_name, current_vars) >= 0)  )
                                                                           ||
                    ( ( !(args & DONT_SEARCH_IN_ARRS) )             &&
                      (findVarName (declared_name, current_arrs) >= 0)  )
                                                                           ||
                    ( ( !(args & DONT_SEARCH_IN_LBLS) )             &&
                      (findVarName (declared_name, current_lbls) >= 0)  )
                 )
                 return true;

              if (
                    (  ( !(args & DONT_SEARCH_IN_FNCS) )  &&
                          (args & SEARCH_IN_GLOBAL_FNCS)  &&
                          (findVarName(declared_name,functions_stack->front()) >=0))

                                               ||

                    (  ( !(args & DONT_SEARCH_IN_VARS) )  &&
                          (args & SEARCH_IN_SHARED_VARS)  &&
                          has_shared_vars                 &&
                          (findVarName (declared_name,shared_vars) >= 0)  )

                                               ||

                    (  ( !(args & DONT_SEARCH_IN_ARRS) )  &&
                          (args & SEARCH_IN_SHARED_ARRS)  &&
                          has_shared_arrs                 &&
                          (findVarName (declared_name,shared_arrs) >= 0)  )
                 )
                 return true;
              // Conflicting = no
              return false;
           }
           case SEARCH_SHARED_DATA_ONLY:{

              if (
                    (  ( !(args & DONT_SEARCH_IN_FNCS) )  &&
                          (args & SEARCH_IN_GLOBAL_FNCS)  &&
                          (findVarName(declared_name,functions_stack->front()) >=0))

                                               ||

                    (  ( !(args & DONT_SEARCH_IN_VARS) )  &&
                          (args & SEARCH_IN_SHARED_VARS)  &&
                          has_shared_vars                 &&
                          (findVarName (declared_name,shared_vars) >= 0)  )

                                               ||

                    (  ( !(args & DONT_SEARCH_IN_ARRS) )  &&
                          (args & SEARCH_IN_SHARED_ARRS)  &&
                          has_shared_arrs                 &&
                          (findVarName (declared_name,shared_arrs) >= 0)  )
                 )
                 return true;
              // Conflicting = no
              return false;
           }
           default: ???continue???;assert(0);
        }
     }
     */
     if (args == SEARCH_BY_DEFAULT) {

        if (
              ( ( !(args & DONT_SEARCH_IN_FNCS) )             &&
                (findVarName (declared_name, current_funcs) >= 0) )
                                                                     ||
              ( ( !(args & DONT_SEARCH_IN_VARS) )             &&
                (findVarName (declared_name, current_vars) >= 0)  )
                                                                     ||
              ( ( !(args & DONT_SEARCH_IN_ARRS) )             &&
                (findVarName (declared_name, current_arrs) >= 0)  )
                                                                     ||
              ( ( !(args & DONT_SEARCH_IN_LBLS) )             &&
                (findVarName (declared_name, current_lbls) >= 0)  )
           )
           return true;
        return false;
     }
     // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
     if (args & SEARCH_IN_ALL_DATA) {

        if (
              ( ( !(args & DONT_SEARCH_IN_FNCS) )             &&
                (findVarName (declared_name, current_funcs) >= 0) )
                                                                     ||
              ( ( !(args & DONT_SEARCH_IN_VARS) )             &&
                (findVarName (declared_name, current_vars) >= 0)  )
                                                                     ||
              ( ( !(args & DONT_SEARCH_IN_ARRS) )             &&
                (findVarName (declared_name, current_arrs) >= 0)  )
                                                                     ||
              ( ( !(args & DONT_SEARCH_IN_LBLS) )             &&
                (findVarName (declared_name, current_lbls) >= 0)  )
           )
           return true;

        if (
              (  ( !(args & DONT_SEARCH_IN_FNCS) )  &&
                    (findVarName(declared_name,functions_stack->front()) >=0))

                                         ||

              (  ( !(args & DONT_SEARCH_IN_VARS) )  &&
                    has_shared_vars                 &&
                    (findVarName (declared_name,shared_vars) >= 0)  )

                                         ||

              (  ( !(args & DONT_SEARCH_IN_ARRS) )  &&
                    has_shared_arrs                 &&
                    (findVarName (declared_name,shared_arrs) >= 0)  )
           )
           return true;
        // Conflicting = no
        return false;
     }
     // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
     if (args & SEARCH_SHARED_DATA_ONLY) {

        if (
              (  ( !(args & DONT_SEARCH_IN_FNCS) )  &&
                    (findVarName(declared_name,functions_stack->front()) >=0))

                                         ||

              (  ( !(args & DONT_SEARCH_IN_VARS) )  &&
                    (args & SEARCH_IN_SHARED_VARS)  &&
                    has_shared_vars                 &&
                    (findVarName (declared_name,shared_vars) >= 0)  )

                                         ||

              (  ( !(args & DONT_SEARCH_IN_ARRS) )  &&
                    has_shared_arrs                 &&
                    (findVarName (declared_name,shared_arrs) >= 0)  )
           )
           return true;
        // Conflicting = no
        return false;
     }
     // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
     // We shouldn't be at this point
     assert (0);
     return false;
  }
/* -------------------------------------------------------------------------- */
  void Pool::freall() {

     // Free Values
     for (val_MStack_t::iterator it = vals_multi_stack->begin();
                                 it < vals_multi_stack->end();  ++it) {
        for_each((*it)->begin(), (*it)->end(),val_FreeArray);
        for (val_Stack_t::iterator it2 = (*it)->begin();
                                   it2 < (*it)->end();  ++it2) {
           Pool::free(*it2);
        }
        (*it)->erase((*it)->begin(),(*it)->end());
        Pool::free(*it);
     }
     vals_multi_stack->erase(vals_multi_stack->begin(),vals_multi_stack->end());
     Pool::free(vals_multi_stack);

     // Free Array stacks data
     for (val_VMMS_t::iterator it = arrs_vals_multi_stack->begin();
                               it < arrs_vals_multi_stack->end();  ++it) {

        for (val_MStack_t::iterator it2 = (*it)->begin();
                                    it2 < (*it)->end();  ++it2) {

           for_each((*it2)->begin(), (*it2)->end(),val_FreeArray);
           for (val_Stack_t::iterator it3 = (*it2)->begin();
                                       it3 < (*it2)->end();  ++it3) {
              Pool::free(*it3);
           }
           (*it2)->erase((*it2)->begin(),(*it2)->end());
           Pool::free(*it2);
        }
        (*it)->erase((*it)->begin(),(*it)->end());
        Pool::free(*it);
     }
     (arrs_vals_multi_stack)->erase( (arrs_vals_multi_stack)->begin(),
                                     (arrs_vals_multi_stack)->end()   );
     Pool::free(arrs_vals_multi_stack);

     // Free variables
     for (var_Stack_t::iterator it = vars_stack->begin();
                                it < vars_stack->end();  ++it) {
        var_FreeArray(*it);
        Pool::free(*it);
     }
     vars_stack->erase(vars_stack->begin(),vars_stack->end());
     Pool::free(vars_stack);

     // Free arrays
     for_each(arrays_stack->begin(),arrays_stack->end(),arr_FreeArray);
     for (arr_Stack_t::iterator it = arrays_stack->begin();
                                it < arrays_stack->end();  ++it)
        Pool::free(*it);
     arrays_stack->erase(arrays_stack->begin(),arrays_stack->end());
     Pool::free(arrays_stack);

     // Free labels
     for_each(labels_stack->begin(),labels_stack->end(),label_FreeArray);
     for (lab_Stack_t::iterator it = labels_stack->begin();
                                it < labels_stack->end();  ++it)
        Pool::free(*it);
     labels_stack->erase(labels_stack->begin(),labels_stack->end());
     Pool::free(labels_stack);

     // Free subroutines
     for (sub_Stack_t::iterator it = functions_stack->begin();
                                it < functions_stack->end();  ++it) {
        for (sub_Arr_t::iterator it2 = (*it)->begin();
                                 it2 < (*it)->end();  ++it2) {
           (*it2)->freeOwnListAndData();
           Pool::free(*it2);
        }
        (*it)->erase((*it)->begin(),(*it)->end());
        Pool::free(*it);
     }
     functions_stack->erase(functions_stack->begin(),functions_stack->end());
     Pool::free(functions_stack);

     val_FreeArray(shared_vars_values);
     Pool::free(shared_vars_values);

     var_FreeArray(shared_vars);
     shared_vars->erase(shared_vars->begin(),shared_vars->end());
     Pool::free(shared_vars);

     for_each( shared_arrs_values->begin(),
                                    shared_arrs_values->end(),  val_FreeArray );

     for (val_Stack_t::iterator it = shared_arrs_values->begin();
                                it < shared_arrs_values->end();  ++it) {
        Pool::free(*it);
     }

     Pool::free(shared_arrs_values);

     arr_FreeArray(shared_arrs);
     shared_arrs->erase(shared_arrs->begin(),shared_arrs->end());
     Pool::free(shared_arrs);

     Pool::freeStmtList(stmt_list);
     Pool::free(stmt_list);

#ifndef DISABLE_TRACING
     Pool::memprint();
#endif // DISABLE_TRACING
  }
/* -------------------------------------------------------------------------- */
  void Pool::freeStmtList(stmt_List_t * list) {

     if (list == NULL || list->empty()) return;

     for (stmt_List_t::iterator it = list->begin();
                                it < list->end();   ++it) {
        (*it)->memFree();
        Pool::free((*it));
     }
     list->erase(list->begin(),list->end());
  }
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_TRACING
  void Pool::memprint()
  {
     if (!user_opt.is_trace_silent())
     {
        cout << "\nCurrently allocated space in bytes: "<< allocated
             <<  " (" << (static_cast<_double_t>(allocated)) / 1024
             << " Kbytes)" << endl
             << "Maximum allocated space in bytes: "    << maxallocated
             <<  " (" << (static_cast<_double_t>(maxallocated)) / 1024
             << " Kbytes)" << endl;
        // Just to remember
        if (allocated != 0)
           cout << endl << "ATTENTION! PROGRAM HAVE A MEMORY LEAK!" << endl;
        return;
     }
     trace_file << "\nCurrently allocated space in bytes: "<< allocated
                <<  " (" << (static_cast<_double_t>(allocated)) / 1024
                << " Kbytes)" << endl
                << "Maximum allocated space in bytes: "    << maxallocated
                <<  " (" << (static_cast<_double_t>(maxallocated)) / 1024
                << " Kbytes)" << endl;
     // Just to remember
     if (allocated != 0)
        cout << endl << "ATTENTION! PROGRAM HAVE A MEMORY LEAK!" << endl;
  }
#endif // DISABLE_TRACING
/* -------------------------------------------------------------------------- */
