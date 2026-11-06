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

#include "variable.h"
#include "options.h"
#include "pool.h"
/* -------------------------------------------------------------------------- */
  static void ClearValue (Variable *var);
/* -------------------------------------------------------------------------- */
  Variable::Variable (const string &name):
     val (NULL),
     var_name (name),
     var_type (VOID),
     is_shared (false),
     is_initialized (false) {
  }
/* -------------------------------------------------------------------------- */
  Variable::Variable (const string &name,Types type):
     val (NULL),
     var_name (name),
     var_type (type),
     is_shared (false),
     is_initialized (false) {
  }
/* -------------------------------------------------------------------------- */
  Variable::Variable (const string &name,Value *value):
     val (value),
     var_name (name),
     var_type (value->GetType()),
     is_shared (false),
     is_initialized (false) {
  }
/* -------------------------------------------------------------------------- */
  Value &Variable::operator = (Value& next_val)
  {
     *val = next_val;
     if (!is_initialized) is_initialized = true;
     return *val;
  }
/* -------------------------------------------------------------------------- */
  Value &Variable::operator = (Variable& next_val)
  {
     *val = *next_val.GetValue();
     if (!is_initialized) is_initialized = true;
     return *val;
  }
/* -------------------------------------------------------------------------- */
  void Variable::Printer ()
  {
     if (!user_opt.is_trace_silent())
     {
        cout << TypesNames[Types(var_type)] << ' ' << var_name << ' ' << '{';
        if (val != NULL)
           val->Printer();
        else cout << "NULL";
        cout << '}';
        return;
     }
     trace_file << TypesNames[Types(var_type)] << ' ' << var_name << ' ' << '{';
     if (val != NULL)
        val->Printer();
     else trace_file << "NULL";
     trace_file << '}';;
  }
/* -------------------------------------------------------------------------- */
  void ClearVariables (var_Arr_t *vars)
  {
     if (vars != NULL && vars->size())
        for_each(vars->begin(),vars->end(),ClearValue);
  }
/* -------------------------------------------------------------------------- */
  static void ClearValue (Variable *var)
  {
     switch (var->GetType())
     {
        case VOID:
        case INT:   var->SetValue_ival(0); break;
        case LINT:  var->SetValue_lival(0);break;
        case FLOAT: var->SetValue_fval(0); break;
        case DOUBLE:var->SetValue_dval(0); break;
        case STRING:var->SetValue_sval("");break;
        default: assert(0);
     }
     var->SetInitialization(false);
  }
/* -------------------------------------------------------------------------- */
  void var_FreeArray (var_Arr_t *array)
  {
     if (array == NULL || array->empty()) return;
     _size_t size = array->size();
     for (_size_t i = 0; i < size; ++i) {
        if (array->at(i) != NULL)
           pool.free(array->at(i));
     }
  }
/* -------------------------------------------------------------------------- */
