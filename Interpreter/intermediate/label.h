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

#ifndef LABEL_H_INCLUDED
#define LABEL_H_INCLUDED

#include "common.h"
  using std::string;
  using std::vector;
/* -------------------------------------------------------------------------- */
  typedef struct Label {
     string name;
     stmt_Node_t *label;
     _bool_t linked;

     void         SetName    (string str)      {name = str;};
     void         SetLabel (stmt_Node_t *adr)  {label = adr;};
     void         SetLinked  (_bool_t b)       {linked = b;};

     string       GetName    () const {return name;};
     stmt_Node_t *GetAddress ()       {return label;};
     _bool_t      IsLinked   () const {return linked;};
  } label_Named_t;
/* -------------------------------------------------------------------------- */
  void label_FreeArray (lab_Arr_t *array);
/* -------------------------------------------------------------------------- */
  label_Named_t *label_NewNamedLabel (const string str,stmt_Node_t *address,
                                                             lab_Arr_t *labels);
/* -------------------------------------------------------------------------- */

#endif // LABEL_H_INCLUDED
