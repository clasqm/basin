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

#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include <vector>
   using std::vector;
#include <string>
   using std::string;
#include "common.h"
/* -------------------------------------------------------------------------- */
  Value func_Calculate (const expr_Node_t *expr,val_Arr_t *params);
  Value func_Calculate (const stmt_Node_t *stmt,val_Arr_t *params,
                                                             var_Arr_t *vars,
                                                             arr_Arr_t *arrays);
/* -------------------------------------------------------------------------- */
#endif // FUNCTION_H_INCLUDED
