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

#include "value.h"
#include "error_handler.h"
#include "options.h"
#include "pool.h"
/* -------------------------------------------------------------------------- */
  Value::Value(const string str, Types ValType):
     value_type (ValType)
  {
     switch (value_type)
     {
        case INT:   value.ival  = atoi(str.c_str());break;
        case LINT:  value.lival = atol(str.c_str());break;
        case FLOAT: value.fval  = atof(str.c_str());break;
        case DOUBLE:value.dval  = atof(str.c_str());break;
        case STRING:       sval = str;              break;
        case LABEL:        sval = str;              break;
        default: error(UNKNOWN_TYPE);
     }
  }
/* -------------------------------------------------------------------------- */
  Value::Value(Types ValType):
     value_type (ValType)
  {
     switch (value_type)
     {
        case INT:   value.ival = 0; break;
        case LINT:  value.lival = 0;  break;
        case FLOAT: value.fval = 0;  break;
        case DOUBLE:value.dval = 0;  break;
        case STRING:sval = "";       break;
        case VOID:
           /* Do nothing.Type will be
              set in assign operation
            */                       break;
        default: error(UNKNOWN_TYPE);
     }
  }
/* -------------------------------------------------------------------------- */
  Value::Value(const string str, Types ValType,_int_t ival):
     value_type (ValType),
     sval (str)
  {
     value.ival = ival;
  }
/* -------------------------------------------------------------------------- */
   Value &Value::operator = (const Value& prev)
   {
       switch (prev.GetType ())
       {
          case INT:
             switch (value_type)
             {
                case INT:value.ival = prev.Get_ival();break;
                case LINT:
                   value.lival = static_cast<_long_t>(prev.Get_ival());break;
                case FLOAT:
                   value.fval = static_cast<_float_t>(prev.Get_ival());break;
                case DOUBLE:
                   value.dval = static_cast<_double_t>(prev.Get_ival());break;
                case VOID:
                   value_type =  INT;
                   value.ival = prev.Get_ival();break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default:assert(0);
             }
          break;
          case LINT:
             switch (value_type)
             {
                case INT:
                   value.ival = static_cast<_int_t>(prev.Get_lival());break;
                case LINT:
                   value.lival = prev.Get_lival();break;
                case FLOAT:
                   value.fval = static_cast<_float_t>(prev.Get_lival());break;
                case DOUBLE:
                   value.dval = static_cast<_double_t>(prev.Get_lival());break;
                case VOID:
                   value_type =  LINT;
                   value.lival = prev.Get_lival();break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default:assert(0);
             }
          break;
          case FLOAT:
             switch (value_type)
             {
                case INT:
                   value.ival = static_cast<_int_t>(prev.Get_fval());break;
                case LINT:
                   value.lival = static_cast<_long_t>(prev.Get_fval());break;
                case FLOAT:
                   value.fval = prev.Get_fval();break;
                case DOUBLE:
                   value.dval = static_cast<_double_t>(prev.Get_fval());break;
                case VOID:
                   value_type =  FLOAT;
                   value.fval = prev.Get_fval();break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default:assert(0);
             }
          break;
          case DOUBLE:
             switch (value_type)
             {
                case INT:
                   value.ival = static_cast<_int_t>(prev.Get_dval());break;
                case LINT:
                   value.lival = static_cast<_long_t>(prev.Get_dval());break;
                case FLOAT:
                   value.fval = static_cast<_float_t>(prev.Get_dval());break;
                case DOUBLE:
                   value.dval = prev.Get_dval();break;
                case VOID:
                   value_type =  DOUBLE;
                   value.dval = prev.Get_dval();break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default:assert(0);
             }
          break;
          case STRING:
             switch (value_type)
             {
                case INT:
                case LINT:
                case FLOAT:
                case DOUBLE:error(CANNOT_CONVERT_TYPE);
                case STRING:sval = prev.Get_sval();break;
                case VOID:
                   value_type =  STRING;
                   sval = prev.Get_sval();break;
                default:assert(0);
             }
          break;
          default:assert(0);
       }
      return *this;
   }
/* -------------------------------------------------------------------------- */
  Value &Value::operator = (const _int_t i)
  {
     assert(this->value_type == VOID);
     value_type = INT;
     value.ival = i;
     return *this;
  }
/* -------------------------------------------------------------------------- */
   Value Value::operator + (const Value& prev)
   {
      Value v;
      switch (prev.GetType ())
      {
         case INT:
            switch (value_type) {
               case INT:
               case VOID:
                  v.SetType(INT);
                  v.Set_ival(value.ival + prev.Get_ival()); break;
               case LINT:
                  v.SetType(LINT);
                  v.Set_lival(value.lival + prev.Get_ival()); break;
               case FLOAT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.fval + prev.Get_ival()); break;
               case DOUBLE:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval + prev.Get_ival()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case LINT:
            switch (value_type) {
               case INT:
                  v.SetType(LINT);
                  v.Set_lival(value.ival + prev.Get_lival());break;
               case LINT:
               case VOID:
                  v.SetType(LINT);
                  v.Set_lival(value.lival + prev.Get_lival());break;
               case FLOAT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.fval + prev.Get_lival());break;
               case DOUBLE:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval + prev.Get_lival());break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case FLOAT:
            switch (value_type) {
               case INT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.ival + prev.Get_fval());break;
               case LINT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.lival + prev.Get_fval());break;
               case FLOAT:
               case VOID:
                  v.SetType(FLOAT);
                  v.Set_fval(value.fval + prev.Get_fval());break;
               case DOUBLE:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval + prev.Get_fval());break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case DOUBLE:
            switch (value_type) {
               case INT:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.ival + prev.Get_dval()); break;
               case LINT:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.lival + prev.Get_dval()); break;
               case FLOAT:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.fval + prev.Get_dval()); break;
               case DOUBLE:
               case VOID:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval + prev.Get_dval()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case STRING:
            switch (value_type) {
               case INT: case LINT: case FLOAT: case DOUBLE:
                  error(CANNOT_CONVERT_TYPE); break;
               case STRING:
               case VOID:
                  v.SetType(STRING);
                  v.Set_sval(sval + prev.Get_sval());break;
               default:assert (0);
            }
         break;
         default:assert (0);
      }
      //cout << endl << "operator +: v= "<<v << endl ;
      return v;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator - (const Value& prev)
   {
      Value v;
      switch (prev.GetType ())
      {
         case INT:
            switch (value_type) {
               case INT:
               case VOID:
                  v.SetType(INT);
                  v.Set_ival(value.ival - prev.Get_ival()); break;
               case LINT:
                  v.SetType(LINT);
                  v.Set_lival(value.lival - prev.Get_ival()); break;
               case FLOAT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.fval - prev.Get_ival()); break;
               case DOUBLE:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval - prev.Get_ival()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case LINT:
            switch (value_type) {
               case INT:
                  v.SetType(LINT);
                  v.Set_lival(value.ival - prev.Get_lival()); break;
               case LINT:
               case VOID:
                  v.SetType(LINT);
                  v.Set_lival(value.lival - prev.Get_lival()); break;
               case FLOAT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.fval - prev.Get_lival()); break;
               case DOUBLE:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval - prev.Get_lival()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case FLOAT:
            switch (value_type) {
               case INT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.ival - prev.Get_fval()); break;
               case LINT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.lival - prev.Get_fval()); break;
               case FLOAT:
               case VOID:
                  v.SetType(FLOAT);
                  v.Set_fval(value.fval - prev.Get_fval()); break;
               case DOUBLE:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval - prev.Get_fval()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case DOUBLE:
            switch (value_type) {
               case INT:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.ival - prev.Get_dval()); break;
               case LINT:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.lival - prev.Get_dval()); break;
               case FLOAT:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.fval - prev.Get_dval()); break;
               case DOUBLE:
               case VOID:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval - prev.Get_dval()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case STRING:
            switch (value_type) {
               case INT: case LINT: case FLOAT: case DOUBLE:
                  error(CANNOT_CONVERT_TYPE); break;
               case STRING:
                  error (OPERATION_UNSUPPORTED);break;
               default:assert (0);
            }
         break;
         default:assert (0);
      }
      return v;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator - ()
   {
       Value val;
       if (value_type == INT)
       {
           val.SetType(INT);
           val.Set_ival(-Get_ival());
       }
       else
       if (value_type == FLOAT)
       {
           val.SetType(FLOAT);
           val.Set_fval(-Get_fval());
       }
       else error(CANNOT_CONVERT_TYPE);
       return val;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator * (const Value& prev)
   {
      Value v;
      switch (prev.GetType ())
      {
         case INT:
            switch (value_type) {
               case INT:
               case VOID:
                  v.SetType(INT);
                  v.Set_ival(value.ival * prev.Get_ival()); break;
               case LINT:
                  v.SetType(LINT);
                  v.Set_lival(value.lival * prev.Get_ival()); break;
               case FLOAT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.fval * prev.Get_ival()); break;
               case DOUBLE:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval * prev.Get_ival()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case LINT:
            switch (value_type) {
               case INT:
                  v.SetType(LINT);
                  v.Set_lival(value.ival * prev.Get_lival()); break;
               case LINT:
               case VOID:
                  v.SetType(LINT);
                  v.Set_lival(value.lival * prev.Get_lival()); break;
               case FLOAT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.fval * prev.Get_lival()); break;
               case DOUBLE:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval * prev.Get_lival()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case FLOAT:
            switch (value_type) {
               case INT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.ival * prev.Get_fval()); break;
               case LINT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.lival * prev.Get_fval()); break;
               case FLOAT:
               case VOID:
                  v.SetType(FLOAT);
                  v.Set_fval(value.fval * prev.Get_fval()); break;
               case DOUBLE:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval * prev.Get_fval()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case DOUBLE:
            switch (value_type) {
               case INT:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.ival * prev.Get_dval()); break;
               case LINT:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.lival * prev.Get_dval()); break;
               case FLOAT:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.fval * prev.Get_dval()); break;
               case DOUBLE:
               case VOID:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval * prev.Get_dval()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case STRING:
            switch (value_type) {
               case INT: case LINT: case FLOAT: case DOUBLE:
                  error (CANNOT_CONVERT_TYPE);  break;
               case STRING:
                  error (OPERATION_UNSUPPORTED);break;
               default:assert (0);
            }
         break;
         default:assert (0);
      }
      return v;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator / (const Value& prev)
   {
      Value v;
      switch (prev.GetType ())
      {
         case INT:
            switch (value_type) {
               case INT:
               case VOID:
                  v.SetType(INT);
                  v.Set_ival((double)value.ival / prev.Get_ival()); break;
               case LINT:
                  v.SetType(LINT);
                  v.Set_lival(value.lival / prev.Get_ival()); break;
               case FLOAT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.fval / prev.Get_ival()); break;
               case DOUBLE:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval / prev.Get_ival()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case LINT:
            switch (value_type) {
               case INT:
                  v.SetType(LINT);
                  v.Set_lival(value.ival / prev.Get_lival()); break;
               case LINT:
               case VOID:
                  v.SetType(LINT);
                  v.Set_lival(value.lival / prev.Get_lival()); break;
               case FLOAT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.fval / prev.Get_lival()); break;
               case DOUBLE:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval / prev.Get_lival()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case FLOAT:
            switch (value_type) {
               case INT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.ival / prev.Get_fval()); break;
               case LINT:
                  v.SetType(FLOAT);
                  v.Set_fval(value.lival / prev.Get_fval()); break;
               case FLOAT:
               case VOID:
                  v.SetType(FLOAT);
                  v.Set_fval(value.fval / prev.Get_fval()); break;
               case DOUBLE:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval / prev.Get_fval()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case DOUBLE:
            switch (value_type) {
               case INT:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.ival / prev.Get_dval()); break;
               case LINT:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.lival / prev.Get_dval()); break;
               case FLOAT:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.fval / prev.Get_dval()); break;
               case DOUBLE:
               case VOID:
                  v.SetType(DOUBLE);
                  v.Set_dval(value.dval / prev.Get_dval()); break;
               case STRING:
                  error(CANNOT_CONVERT_TYPE); break;
               default: assert (0);
            }
         break;
         case STRING:
            switch (value_type) {
               case INT: case LINT: case FLOAT: case DOUBLE:
                  error (CANNOT_CONVERT_TYPE);  break;
               case STRING:
                  error (OPERATION_UNSUPPORTED);break;
               default:assert (0);
            }
         break;
         default:assert (0);
      }
      return v;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator == (const Value& prev)
   {
       Value val;
       switch (prev.GetType ())
       {
          case INT:
             switch (value_type)
             {
                case INT:
                   if (value.ival==prev.Get_ival())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (value.lival == static_cast<_long_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval == static_cast<_float_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval == static_cast<_double_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case LINT:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_long_t>(value.ival) == prev.Get_lival())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (value.lival == prev.Get_lival())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval == static_cast<_float_t>(prev.Get_lival()))
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval == static_cast<_double_t>(prev.Get_lival()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case FLOAT:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_float_t>(value.ival) == prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (static_cast<_float_t>(value.lival) == prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval == prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval == static_cast<_double_t>(prev.Get_fval()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case DOUBLE:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_double_t>(value.ival) == prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (static_cast<_double_t>(value.lival) == prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (static_cast<_double_t>(value.fval) == prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval == prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case STRING:
             switch (value_type)
             {
                case INT:
                case LINT:
                case FLOAT:
                case DOUBLE:
                   error(CANNOT_CONVERT_TYPE);
                case STRING:
                   if (sval == prev.Get_sval())
                      val = 1;
                   else val = 0;
                break;
                default: assert (0);
             }
          break;
          default: assert (0);
       }
       return val;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator != (const Value& prev)
   {
       Value val;
       switch (prev.GetType ())
       {
          case INT:
             switch (value_type)
             {
                case INT:
                   if (value.ival != prev.Get_ival())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (value.lival != static_cast<_long_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval != static_cast<_float_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval != static_cast<_double_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case LINT:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_long_t>(value.ival) != prev.Get_lival())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (value.lival != prev.Get_lival())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval != static_cast<_float_t>(prev.Get_lival()))
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval != static_cast<_double_t>(prev.Get_lival()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case FLOAT:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_float_t>(value.ival) != prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (static_cast<_float_t>(value.lival) != prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval != prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval != static_cast<_double_t>(prev.Get_fval()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case DOUBLE:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_double_t>(value.ival) != prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (static_cast<_double_t>(value.lival) != prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (static_cast<_double_t>(value.fval) != prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval != prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case STRING:
             switch (value_type)
             {
                case INT:
                case LINT:
                case FLOAT:
                case DOUBLE:
                   error(CANNOT_CONVERT_TYPE);
                case STRING:
                   if (sval != prev.Get_sval())
                      val = 1;
                   else val = 0;
                break;
                default: assert (0);
             }
          break;
          default: assert (0);
       }
       return val;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator > (const Value& prev)
   {
       Value val;
       switch (prev.GetType ())
       {
          case INT:
             switch (value_type)
             {
                case INT:
                   if (value.ival > prev.Get_ival())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (value.lival > static_cast<_long_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval > static_cast<_float_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval > static_cast<_double_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case LINT:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_long_t>(value.ival) > prev.Get_lival())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (value.lival > prev.Get_lival())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval > static_cast<_float_t>(prev.Get_lival()))
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval > static_cast<_double_t>(prev.Get_lival()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case FLOAT:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_float_t>(value.ival) > prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (static_cast<_float_t>(value.lival) > prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval > prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval > static_cast<_double_t>(prev.Get_fval()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case DOUBLE:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_double_t>(value.ival) > prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (static_cast<_double_t>(value.lival) > prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (static_cast<_double_t>(value.fval) > prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval > prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case STRING:
             switch (value_type)
             {
                case INT:
                case LINT:
                case FLOAT:
                case DOUBLE:
                   error(CANNOT_CONVERT_TYPE);
                case STRING:
                   if (sval > prev.Get_sval())
                      val = 1;
                   else val = 0;
                break;
                default: assert (0);
             }
          break;
          default: assert (0);
       }
       return val;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator >= (const Value& prev)
   {
       Value val;
       switch (prev.GetType ())
       {
          case INT:
             switch (value_type)
             {
                case INT:
                   if (value.ival >= prev.Get_ival())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (value.lival >= static_cast<_long_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval >= static_cast<_float_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval >= static_cast<_double_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case LINT:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_long_t>(value.ival) >= prev.Get_lival())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (value.lival >= prev.Get_lival())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval >= static_cast<_float_t>(prev.Get_lival()))
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval >= static_cast<_double_t>(prev.Get_lival()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case FLOAT:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_float_t>(value.ival) >= prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (static_cast<_float_t>(value.lival) >= prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval >= prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval >= static_cast<_double_t>(prev.Get_fval()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case DOUBLE:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_double_t>(value.ival) >= prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (static_cast<_double_t>(value.lival) >= prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (static_cast<_double_t>(value.fval) >= prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval >= prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case STRING:
             switch (value_type)
             {
                case INT:
                case LINT:
                case FLOAT:
                case DOUBLE:
                   error(CANNOT_CONVERT_TYPE);
                case STRING:
                   if (sval >= prev.Get_sval())
                      val = 1;
                   else val = 0;
                break;
                default: assert (0);
             }
          break;
          default: assert (0);
       }
       return val;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator < (const Value& prev)
   {
       Value val;
       switch (prev.GetType ())
       {
          case INT:
             switch (value_type)
             {
                case INT:
                   if (value.ival < prev.Get_ival())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (value.lival < static_cast<_long_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval < static_cast<_float_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval < static_cast<_double_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case LINT:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_long_t>(value.ival) < prev.Get_lival())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (value.lival < prev.Get_lival())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval < static_cast<_float_t>(prev.Get_lival()))
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval < static_cast<_double_t>(prev.Get_lival()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case FLOAT:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_float_t>(value.ival) < prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (static_cast<_float_t>(value.lival) < prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval < prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval < static_cast<_double_t>(prev.Get_fval()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case DOUBLE:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_double_t>(value.ival) < prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (static_cast<_double_t>(value.lival) < prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (static_cast<_double_t>(value.fval) < prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval < prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case STRING:
             switch (value_type)
             {
                case INT:
                case LINT:
                case FLOAT:
                case DOUBLE:
                   error(CANNOT_CONVERT_TYPE);
                case STRING:
                   if (sval < prev.Get_sval())
                      val = 1;
                   else val = 0;
                break;
                default: assert (0);
             }
          break;
          default: assert (0);
       }
       return val;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator <= (const Value& prev)
   {
       Value val;
       switch (prev.GetType ())
       {
          case INT:
             switch (value_type)
             {
                case INT:
                   if (value.ival <= prev.Get_ival())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (value.lival <= static_cast<_long_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval <= static_cast<_float_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval <= static_cast<_double_t>(prev.Get_ival()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case LINT:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_long_t>(value.ival) <= prev.Get_lival())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (value.lival <= prev.Get_lival())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval <= static_cast<_float_t>(prev.Get_lival()))
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval <= static_cast<_double_t>(prev.Get_lival()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case FLOAT:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_float_t>(value.ival) <= prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (static_cast<_float_t>(value.lival) <= prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (value.fval <= prev.Get_fval())
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval <= static_cast<_double_t>(prev.Get_fval()))
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case DOUBLE:
             switch (value_type)
             {
                case INT:
                   if (static_cast<_double_t>(value.ival) <= prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case LINT:
                   if (static_cast<_double_t>(value.lival) <= prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case FLOAT:
                   if (static_cast<_double_t>(value.fval) <= prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case DOUBLE:
                   if (value.dval <= prev.Get_dval())
                      val = 1;
                   else val = 0;
                break;
                case STRING:
                   error(CANNOT_CONVERT_TYPE);
                default: assert (0);
             }
          break;
          case STRING:
             switch (value_type)
             {
                case INT:
                case LINT:
                case FLOAT:
                case DOUBLE:
                   error(CANNOT_CONVERT_TYPE);
                case STRING:
                   if (sval <= prev.Get_sval())
                      val = 1;
                   else val = 0;
                break;
                default: assert (0);
             }
          break;
          default: assert (0);
       }
       return val;
   }
/* -------------------------------------------------------------------------- */
  Value Value::operator || (const Value& prev)
  {
     _bool_t flag = false;
     switch (value_type)
     {
        case INT:   if (value.ival  != 0) flag = true;break;
        case LINT:  if (value.lival != 0) flag = true;break;
        case FLOAT: if (value.fval  != 0) flag = true;break;
        case DOUBLE:if (value.dval  != 0) flag = true;break;
        case STRING:if (sval.size()  > 0) flag = true;break;
        default: assert (0);
     }
     switch (prev.GetType())
     {
        case INT:   if (prev.Get_ival()        != 0) flag = true;break;
        case LINT:  if (prev.Get_lival()       != 0) flag = true;break;
        case FLOAT: if (prev.Get_fval()        != 0) flag = true;break;
        case DOUBLE:if (prev.Get_dval()        != 0) flag = true;break;
        case STRING:if (prev.Get_sval().size()  > 0) flag = true;break;
        default: assert (0);
     }
     Value val;
     if (flag) return val = 1;
     return val = 0;
  }
/* -------------------------------------------------------------------------- */
  Value Value::operator && (const Value& prev)
  {
     _bool_t flag1 = false;
     switch (value_type)
     {
        case INT:   if (value.ival  != 0) flag1 = true;break;
        case LINT:  if (value.lival != 0) flag1 = true;break;
        case FLOAT: if (value.fval  != 0) flag1 = true;break;
        case DOUBLE:if (value.dval  != 0) flag1 = true;break;
        case STRING:if (sval.size()  > 0) flag1 = true;break;
        default: assert (0);
     }
     _bool_t flag2 = false;
     switch (prev.GetType())
     {
        case INT:   if (prev.Get_ival()        != 0) flag2 = true;break;
        case LINT:  if (prev.Get_lival()       != 0) flag2 = true;break;
        case FLOAT: if (prev.Get_fval()        != 0) flag2 = true;break;
        case DOUBLE:if (prev.Get_dval()        != 0) flag2 = true;break;
        case STRING:if (prev.Get_sval().size()  > 0) flag2 = true;break;
        default: assert (0);
     }
     Value val;
     if (flag1 && flag2) return val = 1;
     return val = 0;
  }
/* -------------------------------------------------------------------------- */
   Value Value::operator &  (const Value& prev)
   {
     Value val;
     if (value_type == INT && prev.GetType () == INT)
        val.Set_ival(value.ival & prev.Get_ival());
     else error(BITWISE_OPERATIONS_WITH_FLOAT);
     return val;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator | (const Value& prev)
   {
     Value val;
     if (value_type == INT && prev.GetType () == INT)
        val.Set_ival(value.ival | prev.Get_ival());
     else error(BITWISE_OPERATIONS_WITH_FLOAT);
     return val;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator ^  (const Value& prev)
   {
     Value val;
     if (value_type == INT && prev.GetType () == INT)
        val.Set_ival(value.ival ^ prev.Get_ival());
     else error(BITWISE_OPERATIONS_WITH_FLOAT);
     return val;
   }
/* -------------------------------------------------------------------------- */
   Value Value::operator ~  ()
   {
     Value val;
     if (value_type == INT)
        val.Set_ival (~value.ival);
     else error(BITWISE_OPERATIONS_WITH_FLOAT);
     return val;
   }
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_TRACING
   void Value::Printer ()
   {
      if (!user_opt.is_trace_silent())
      {
          cout << "Type: "                     << ' '
               << TypesNames[Types(GetType())] << ' '
               << "Value: "                    << ' ';
          switch (value_type)
          {
             case INT:   std::cout << value.ival  ;break;
             case LINT:  std::cout << value.lival ;break;
             case FLOAT: std::cout << value.fval  ;break;
             case DOUBLE:std::cout << value.dval  ;break;
             case STRING:std::cout <<       sval  ;break;
             case VOID:  std::cout << "empty"     ;break;
             default:    std::cout << std::endl;assert(0);
          }
          return;
      }
      trace_file << "Type: "                     << ' '
                 << TypesNames[Types(GetType())] << ' '
                 << "Value: "                    << ' ';
      switch (value_type)
      {
         case INT:   trace_file << value.ival  ;break;
         case LINT:  trace_file << value.lival ;break;
         case FLOAT: trace_file << value.fval  ;break;
         case DOUBLE:trace_file << value.dval  ;break;
         case STRING:trace_file <<       sval  ;break;
         case VOID:  trace_file << "empty"     ;break;
         default:    trace_file << std::endl;assert(0);
      }
   }
#endif // DISABLE_TRACING
/*****************************END OF CLASS MEMBERS*****************************/
/* -------------------------------------------------------------------------- */
  void val_FreeArray (val_Arr_t *array)
  {
     if (array == NULL || array->empty()) return;
     _size_t size = array->size(); ///FIXME bug with size >>= +1
     for (_size_t i = 0; i < size; ++i) {
        if (array->at(i) != NULL)
           pool.free(array->at(i));
     }
  }
/* -------------------------------------------------------------------------- */
   std::ostream& operator << (std::ostream& output_stream,const Value& curr)
   {
       switch (curr.GetType ())
       {
           case FLOAT:
                output_stream << curr.Get_fval();
                return output_stream;
           case INT:
                output_stream << curr.Get_ival();
                return output_stream;
           case STRING:
                output_stream << curr.Get_sval();
                return output_stream;
           case LINT:
                output_stream << curr.Get_lival();
                return output_stream;
           case DOUBLE:
                output_stream << curr.Get_dval();
                return output_stream;
           case VOID://DEBUGGING PURPOSES ONLY
                output_stream << " type:VOID ";
                return output_stream;
           default:assert(0);
       }
   }
/* -------------------------------------------------------------------------- */
