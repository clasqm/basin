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

#include "label.h"
#include "lib.h"
#include "pool.h"
/* -------------------------------------------------------------------------- */
  void label_FreeArray (lab_Arr_t *array)
  {
     if(!array->empty())
     {
        size3 size = array->size();
        for (size3 i = 0; i < size; ++i)
        {
           pool.free(array->at(i));
        }
     }
  }
/* -------------------------------------------------------------------------- */
  label_Named_t *label_NewNamedLabel (const string str,stmt_Node_t *address,
                                                              lab_Arr_t *labels)
  {
     label_Named_t *label = pool.alloc<label_Named_t>();
     label->SetName(str);
     label->SetLabel(address);
     label->SetLinked(false);
     labels->push_back(label);
     return label;
  }
/* -------------------------------------------------------------------------- */
