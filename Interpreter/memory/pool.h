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

#ifndef POOL_H_INCLUDED
#define POOL_H_INCLUDED

/* -------------------------------------------------------------------------- */
//#define MEMCHECK
#ifdef __GXX_EXPERIMENTAL_CXX0X__
   #define __STD_CXX0X__
#endif
/* -------------------------------------------------------------------------- */
#ifdef MEMCHECK
   #include <typeinfo>
#endif
/* -------------------------------------------------------------------------- */
#include "common.h"
  using std::vector;
#include "options.h"
/* -------------------------------------------------------------------------- */
  class Pool {
     public:
        Pool () : allocated(0), maxallocated(0) {};
        /// ---------------------------------------------------------------- ///
#ifdef __STD_CXX0X__ /* Just to remember */
        template <typename T, typename ...Args> T *alloc (const Args...args)
         {
           allocated += sizeof(T);
           if (allocated > maxallocated) maxallocated = allocated;
           return new T(args...);
         }
#else
        /// ---------------------------------------------------------------- ///
        /// Memory allocation function for program
        /// ---------------------------------------------------------------- ///
        template <typename T> T *alloc ()
        {
           allocated += sizeof(T);
           if (allocated > maxallocated) maxallocated = allocated;

#ifdef MEMCHECK
           if (!user_opt.is_trace_silent()) {
              std::cout << "\nAllocating: " << typeid(T).name()
                        << " : " << sizeof(T) << " bytes." << endl;
              fflush(stdout);
           }
           else {

              if (!trace_file.is_open())
                 trace_file.open("tracing.log");

              trace_file << "\nAllocating: " << typeid(T).name()
                           << " : " << sizeof(T) << " bytes." << endl;
           }
#endif
           return new T;
        }
        /// ---------------------------------------------------------------- ///
        template <typename T,typename A> T *alloc (const A &a)
        {
           allocated += sizeof(T);
           if (allocated > maxallocated) maxallocated = allocated;

#ifdef MEMCHECK
           if (!user_opt.is_trace_silent()) {
              std::cout << "\nAllocating: " << typeid(T).name()
                        << " : " << sizeof(T) << " bytes." << endl;
              fflush(stdout);
           }
           else {

              if (!trace_file.is_open())
                 trace_file.open("tracing.log");

              trace_file << "\nAllocating: " << typeid(T).name()
                           << " : " << sizeof(T) << " bytes." << endl;
           }
#endif
           return new T(a);
        }
        /// ---------------------------------------------------------------- ///
        template <typename T,typename A,typename B>
        T *alloc (const A &a,const B &b)
        {
           allocated += sizeof(T);
           if (allocated > maxallocated) maxallocated = allocated;

#ifdef MEMCHECK
           if (!user_opt.is_trace_silent()) {
              std::cout << "\nAllocating: " << typeid(T).name()
                        << " : " << sizeof(T) << " bytes." << endl;
              fflush(stdout);
           }
           else {

              if (!trace_file.is_open())
                 trace_file.open("tracing.log");

              trace_file << "\nAllocating: " << typeid(T).name()
                           << " : " << sizeof(T) << " bytes." << endl;
           }
#endif
           return new T(a,b);
        }
        /// ---------------------------------------------------------------- ///
        template <typename T,typename A,typename B,typename C>
        T *alloc (const A &a,const B &b,const C &c)
        {
           allocated += sizeof(T);
           if (allocated > maxallocated) maxallocated = allocated;

#ifdef MEMCHECK
           if (!user_opt.is_trace_silent()) {
              std::cout << "\nAllocating: " << typeid(T).name()
                        << " : " << sizeof(T) << " bytes." << endl;
              fflush(stdout);
           }
           else {

              if (!trace_file.is_open())
                 trace_file.open("tracing.log");

              trace_file << "\nAllocating: " << typeid(T).name()
                           << " : " << sizeof(T) << " bytes." << endl;
           }
#endif
           return new T(a,b,c);
        }
        /// ---------------------------------------------------------------- ///
        template <typename T,typename A,typename B,typename C,typename D>
        T *alloc (const A &a,const B &b,const C &c,const D &d)
        {
           allocated += sizeof(T);
           if (allocated > maxallocated) maxallocated = allocated;

#ifdef MEMCHECK
           if (!user_opt.is_trace_silent()) {
              std::cout << "\nAllocating: " << typeid(T).name()
                        << " : " << sizeof(T) << " bytes." << endl;
              fflush(stdout);
           }
           else {

              if (!trace_file.is_open())
                 trace_file.open("tracing.log");

              trace_file << "\nAllocating: " << typeid(T).name()
                           << " : " << sizeof(T) << " bytes." << endl;
           }
#endif
           return new T(a,b,c,d);
        }
#endif
        /// ---------------------------------------------------------------- ///
        template <typename T> void free (T *object)
        {
           if (object != NULL) {
              allocated -= sizeof(T);

#ifdef MEMCHECK
              if (!user_opt.is_trace_silent()) {
                 std::cout << "\nDeallocating: " << typeid(T).name()
                           << " : " << sizeof(T) << " bytes." << endl;
                 fflush(stdout);
              }
              else {

                 if (!trace_file.is_open())
                    trace_file.open("tracing.log");

                 trace_file << "\nDeallocating: " << typeid(T).name()
                              << " : " << sizeof(T) << " bytes." << endl;
              }
#endif
              delete object;
              object = NULL;
              return;
           }
#ifdef MEMCHECK
           if (!user_opt.is_trace_silent()) {
              std::cout << "\nDouble free! "<< endl;
              fflush(stdout);
           }
           else {
              if (!trace_file.is_open())
                 trace_file.open("tracing.log");
              trace_file << "\nDouble free! "<< endl;
           }

#endif
        }
        /// ---------------------------------------------------------------- ///
        void init();
        /// ---------------------------------------------------------------- ///
        void freall();
        /// ---------------------------------------------------------------- ///
        void freeStmtList (stmt_List_t *list);
        /// ---------------------------------------------------------------- ///
        var_Arr_t *GetCurVars  () const {return current_vars;};
        arr_Arr_t *GetCurArrs  () const {return current_arrs;};
        lab_Arr_t *GetCurLbls  () const {return current_lbls;};
        sub_Arr_t *GetCurFuncs () const {return current_funcs;};

        val_Arr_t *GetCurValsHeap  () const {
           return vals_multi_stack->at(running_func_index)->at(cur_stack_depth);
        }

        val_Arr_t *GetCurArrsValsHeap  (_long_t arr_index) const {
           return arrs_vals_multi_stack->
                     at(running_func_index)->at(arr_index)->at(cur_stack_depth);
        }

        var_Arr_t *GetSharedVars () const {return shared_vars;};
        arr_Arr_t *GetSharedArrs () const {return shared_arrs;};

        val_Arr_t   *getSharedVarsVals () const {return shared_vars_values;};

        val_Arr_t   *getSharedArrsValsAt (_long_t index) const {
           return shared_arrs_values->at(index);
        };

        val_Stack_t   *getSharedArrsVals () const {
           return shared_arrs_values;
        };

        arr_Arr_t *GetGlobArrs  () const {return arrays_stack->front();};
        sub_Arr_t *GetGlobFuncs  () const {return functions_stack->front();};

        /// ---------------------------------------------------------------- ///
        var_Arr_t *GetPrevVars  () const {
           var_Stack_t::iterator i = find (vars_stack->begin(),
                                           vars_stack->end(),
                                           current_vars        );
           assert ( i != vars_stack->end() );
           return vars_stack->at( distance(vars_stack->begin(), i) - 1 );
        };
        /// ---------------------------------------------------------------- ///
        arr_Arr_t *GetPrevArrs  () const {
           arr_Stack_t::iterator i = find (arrays_stack->begin(),
                                           arrays_stack->end(),
                                           current_arrs        );
           assert ( i != arrays_stack->end() );
           return arrays_stack->at( distance(arrays_stack->begin(), i) - 1 );
        };
        /// ---------------------------------------------------------------- ///
        lab_Arr_t *GetPrevLbls  () const {
           lab_Stack_t::iterator i = find (labels_stack->begin(),
                                           labels_stack->end(),
                                           current_lbls        );
           assert ( i != labels_stack->end() );
           return labels_stack->at( distance(labels_stack->begin(), i) - 1 );
        };
        /// ---------------------------------------------------------------- ///
        sub_Arr_t *GetPrevFuncs () const {
           sub_Stack_t::iterator i = find (functions_stack->begin(),
                                           functions_stack->end(),
                                           current_funcs        );
           assert ( i != functions_stack->end() );
           return functions_stack->at(distance(functions_stack->begin(),i) - 1);
        };
        /// ---------------------------------------------------------------- ///
        void SetCurVars  (var_Arr_t *vars) {current_vars  = vars;};
        void SetCurArrs  (arr_Arr_t *arrs) {current_arrs  = arrs;};
        void SetCurLbls  (lab_Arr_t *lbls) {current_lbls  = lbls;};
        void SetCurFuncs (sub_Arr_t *fncs) {current_funcs = fncs;};

        void setRunningFunc(_size_t index) {
           running_func_index = index;
        };

        void setCurFuncStackDepth(_size_t depth) {
           cur_stack_depth = depth;
        };

        //void    incrStackDepth () {++cur_stack_depth;};
        //void    decrStackDepth () {--cur_stack_depth;};
        //_size_t getStackDepth  () const {return cur_stack_depth;};

        void pushToVarsStack (var_Arr_t *vars) {vars_stack->push_back(vars);};
        void pushToArrsStack (arr_Arr_t *arrs) {arrays_stack->push_back(arrs);};
        void pushToLblsStack (lab_Arr_t *lbls) {labels_stack->push_back(lbls);};

        /// This function adding values stack for certain array defined in
        /// function statements list (or global statements list).
        /// @param func_index is index of function in function list
        //void pushToArrsValsMStack(val_Stack_t *array_vals, size_t func_index) {
        //   arrs_vals_multi_stack->at(func_index)->push_back(array_vals);
        //};

        void pushFirstToArrsValsMStack(val_Arr_t *array_vals, size_t func_index)
        {
           arrs_vals_multi_stack->at(func_index)->
                                  at(cur_stack_depth)->push_back(array_vals);
        };

        //void pushToValsMStack(val_Stack_t *vals) {
        //   vals_multi_stack->push_back(vals);
        //};

        void pushFirstToValsMStack(val_Arr_t *vals, size_t func_index)
        {
           if (vals_multi_stack->at(func_index)->empty())
              vals_multi_stack->at(func_index)->push_back(vals);
        };

        val_MStack_t *getValsStack()    const {return vals_multi_stack;};
        val_VMMS_t   *getArrValsStack() const {return arrs_vals_multi_stack;};
        arr_Stack_t  *GetArrStack()     const {return arrays_stack;};
        lab_Stack_t  *GetLabStack()     const {return labels_stack;};

        void AddSharedVar (Variable *var) {
           shared_vars->push_back(var);
        };
        void AddSharedArr (Array *arr) {
           shared_arrs->push_back(arr);
        };

        _bool_t hasSharedVars() {return has_shared_vars;};
        _bool_t hasSharedArrs() {return has_shared_arrs;};

        /// ---------------------------------------------------------------- ///
        enum IS_CONFLICTING_DECLARATION_ARGS {//This is only a try,
                                              //possibly to be removed
           SEARCH_BY_DEFAULT          = 0x000000,
           SEARCH_IN_SHARED_VARS      = 0x000001,
           SEARCH_IN_SHARED_ARRS      = 0x000020,
           SEARCH_IN_SHARED_LBLS      = 0x000300,
           SEARCH_IN_GLOBAL_FNCS      = 0x004000,
           SEARCH_IN_ALL_DATA         = 0x050000,
           SEARCH_SHARED_DATA_ONLY    = 0x600000,

           DONT_SEARCH_IN_VARS        = 0x00000F,
           DONT_SEARCH_IN_ARRS        = 0x0000F0,
           DONT_SEARCH_IN_LBLS        = 0x000F00,
           DONT_SEARCH_IN_FNCS        = 0x00F000
        };

        typedef unsigned int args_t;

        _bool_t isConflictingDeclaration (string declared_name,
                                               args_t args = SEARCH_BY_DEFAULT);
        /// ---------------------------------------------------------------- ///
     private:
        var_Stack_t   *vars_stack;
        val_VMMS_t    *arrs_vals_multi_stack;
        val_MStack_t  *vals_multi_stack;
        arr_Stack_t   *arrays_stack;
        lab_Stack_t   *labels_stack;
        sub_Stack_t   *functions_stack;

        var_Arr_t   *current_vars;
        arr_Arr_t   *current_arrs;
        lab_Arr_t   *current_lbls;
        sub_Arr_t   *current_funcs;

        _size_t      running_func_index;
        _size_t      cur_stack_depth;

        var_Arr_t   *shared_vars;
        val_Arr_t   *shared_vars_values;
        arr_Arr_t   *shared_arrs;
        val_Stack_t *shared_arrs_values;

        _bool_t      has_shared_vars;
        _bool_t      has_shared_arrs;

        _size_t allocated;
        _size_t maxallocated;

#ifndef DISABLE_TRACING
        /// ---------------------------------------------------------------- ///
        void memprint();
        /// ---------------------------------------------------------------- ///
#endif
  };
/* -------------------------------------------------------------------------- */
  extern Pool pool;
/* -------------------------------------------------------------------------- */
#endif // POOL_H_INCLUDED
