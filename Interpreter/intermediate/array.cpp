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

#include <algorithm>
  // using copy()
#include "common.h"

#include "array.h"
#include "error_handler.h"
#include "options.h"
#include "pool.h"
/* -------------------------------------------------------------------------- */
  static void Clear (Value *val);
  static void InitClear (_bool_t b);
  static void ArrClearElements (Array *array);
  extern void expr_ListPrint (expr_List_t *list);
/* -------------------------------------------------------------------------- */
  Array::Array (string str,Types elements_type, locality_t loc):
     name(str),
     type(elements_type),
     zeroed (false),
     locality (loc),
     max_index(0)
  {
     elements = pool.alloc<val_Arr_t>();
     if (locality == LOCAL) {
        // To locate which function being builded now, we try to locate distance
        // from beginning of arrays list to current arrays,that set in pool.
        size_t index_in_list = distance(
                                        find( pool.GetArrStack()->begin(),
                                              pool.GetArrStack()->end(),
                                              pool.GetCurArrs()           ),
                                        pool.GetArrStack()->begin()           );
        pool.pushFirstToArrsValsMStack(elements, index_in_list);
     }
     else {
        pool.getSharedArrsVals ()->push_back(elements);
     }
  }
/* -------------------------------------------------------------------------- */
  void Array::SetStartIndexes(const sz3_Arr_t * const array)
  {
     start_index.resize(array->size());
     copy(array->begin(),array->end(),start_index.begin());
  }
/* -------------------------------------------------------------------------- */
  void Array::SetDimensions(sz4_Arr_t *dims)
  {
     size3 size = dims->size();
     assert (size >= 0);
     for (size3 i = 0; i < size; ++i)
        dimensions.push_back(dims->at(i));
  }
/* -------------------------------------------------------------------------- */
  void Array::SetElement  (_size_t index,Value *val)
  {
     assert (elements->size() != 0);
     if (val->GetType() != type)
        error(CANNOT_CONVERT_TYPE);
     if (elements->at(index) == NULL) {
        (elements->at(index)) = val;
        init_at_i.at(index) = true;
        return;
     }
     *(elements->at(index)) = *val;
     init_at_i.at(index) = true;
  }
/* -------------------------------------------------------------------------- */
  void Array::SetElement  (_size_t index,Value const &val)
  {
     assert (elements->size() != 0);
     if (val.GetType() != type)
        error(CANNOT_CONVERT_TYPE);
     assert(elements->at(index) != NULL);
     *(elements->at(index)) = val;
     init_at_i.at(index) = true;
  }
/* -------------------------------------------------------------------------- */
  void Array::pushNULLs() {
     for (_size_t i = 0; i < max_index; ++i) {
        Value *val = NULL;
        elements->push_back(val);
        init_at_i.push_back(false);
     }
  }
/* -------------------------------------------------------------------------- */
  void Array::ArrZeroInit ()
  {
     for (_size_t i = 0; i < max_index; ++i)
     {
        switch (type)
        {
           case INT:
           case LINT:
           case FLOAT:
           case DOUBLE:{Value *val = pool.alloc<Value>("0",type);
                        elements->push_back(val);
           break;}
           case STRING:{Value *val = pool.alloc<Value>("",STRING);
                        elements->push_back(val);
           break;}
           default: assert(0);
        }
        init_at_i.push_back(false);
     }
     //FIXME! Store elements in pool
  }
/* -------------------------------------------------------------------------- */
  void Array::Printer(expr_List_t *indexes)
  {
     if (!user_opt.is_trace_silent())
     {
        cout << TypesNames[Types(type)] << ' ' << name << ' ' << '[';
        expr_ListPrint (indexes);
        cout << ']';
        return;
     }
     trace_file << TypesNames[Types(type)] << ' ' << name << ' ' << '[';
     expr_ListPrint (indexes);
     trace_file << ']';
  }
/* -------------------------------------------------------------------------- */
   static void Clear(Value *val)
   {
      switch (val->GetType())
      {
           case INT:   val->Set_ival(0); break;
           case LINT:  val->Set_lival(0);break;
           case FLOAT: val->Set_fval(0); break;
           case DOUBLE:val->Set_dval(0); break;
           case STRING:val->Set_sval("");break;
           default: assert(0);
      }
   }
/* -------------------------------------------------------------------------- */
   static void InitClear(_bool_t b)
   {
      b = false;
   }
/* -------------------------------------------------------------------------- */
  void Array::ArrClearElements ()
  {
     for_each(elements->begin(),elements->end(),Clear);
     for_each(init_at_i.begin(),init_at_i.end(),InitClear);
  }
/* -------------------------------------------------------------------------- */
  static void ArrClearElements (Array *array)
  {
     array->ArrClearElements ();// Тут по-моему я совсем переборщил..
  }
/* -------------------------------------------------------------------------- */
  void ClearAllArraysElements (arr_Arr_t *arrays)
  {
     if (arrays != NULL && arrays->size())
        for_each(arrays->begin(),arrays->end(),ArrClearElements);
  }
/* -------------------------------------------------------------------------- */
void arr_FreeArray (arr_Arr_t *array)
{
   if(array == NULL || array->empty()) return;

   _size_t size = array->size();

   for (_size_t i = 0; i < size; ++i) {
      pool.free(array->at(i));
   }
}
/* -------------------------------------------------------------------------- */
