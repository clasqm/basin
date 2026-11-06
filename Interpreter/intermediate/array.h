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

#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

#include "common.h"
  using std::string;
  using std::vector;

#include "value.h"
/* -------------------------------------------------------------------------- */
  class Array {
     public:
        /* ------------------------------------------ */
        typedef enum {SHARED, LOCAL} locality_t;

        //Array ();
        Array (string str, Types elements_type, locality_t locality);
        //~Array(){};

        _bool_t    IsInitAt (_size_t i)   const {return init_at_i.at(i);};
        locality_t IsShared      ()       const {return locality;};
        _bool_t    isValueNULLat (_size_t const &index) const {
           return elements->at(index) == NULL;
        }

        string     GetName        ()         const {return name;};
        Types      GetType        ()         const {return type;};
        sz4_Arr_t  GetDimensions  ()         const {return dimensions;};
        val_Arr_t *GetElements    ()         const {return elements;};
        size3      GetDimension(size3 i)     const {return dimensions.at(i);};
        size3      GetDimensionsSize()       const {return dimensions.size();};
        Value      GetValueAt (size3 i)      const {return *(elements->at(i));};
        size3      GetStartIndexAt (size3 i) const {return start_index.at(i);};
        arr_Arr_t *GetPage       ()          const {return page;};
        size3      getSize()                 const {return max_index;};

        void SetName (string s){name = s;};
        void SetType (Types t) {type = t;};
        void SetInitAt (size3 i,_bool_t b){init_at_i.at(i) = b;};
        //void SetShared (_bool_t b) {shared = b;};
        void SetZeroed (_bool_t b) {zeroed = b;};
        void SetDimensions(sz4_Arr_t *dims);
        void SetElements (val_Arr_t *elems){elements = elems;};
        void SetElement  (_size_t index,Value *val);
        void SetElement  (_size_t index,Value const &val);
        void SetMaxIndex (size3 i) {max_index = i;};
        void SetStartIndexAt (size3 i,size3 index) {start_index.at(i) = index;};
        void SetStartIndexes(const sz3_Arr_t * const array);
        void SetPage (arr_Arr_t *p) {page = p;};
        //void ResElements (size3 quantity){elements->reserve(quantity);};
        //void FreeElements();
        void ArrZeroInit ();
        void pushNULLs   ();
        void ArrClearElements ();
        void Printer(expr_List_t *indexes);
        /* ------------------------------------------ */
     private:
        /* ------------------------------------------ */
        string      name;          // Name of array
        Types       type;          // Type of it's elements
        _bool_t     zeroed;        // Zeroed?(On runtime)
        locality_t  locality;      // SHARED when defined with SHARED keyword
        bool_Arr_t  init_at_i;     // Is element at i got value?
        _size_t     max_index;     // Quantity of elements in array
        sz3_Arr_t   start_index;   // Start index for each dimension
        sz4_Arr_t   dimensions;    // Array of dimensions
        val_Arr_t   *elements;     // Table of elements
        //val_Stack_t *own_stack;    // It's own stack,where stored *elements
        arr_Arr_t   *page;         // In which page array located.
  };
/* -------------------------------------------------------------------------- */
   extern void arr_FreeArray (arr_Arr_t *array);
   extern void ClearAllArraysElements (arr_Arr_t *arrays);
/* -------------------------------------------------------------------------- */

#endif // ARRAY_H_INCLUDED
