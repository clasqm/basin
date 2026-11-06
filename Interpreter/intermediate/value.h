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

#ifndef VALUE_CLASS_H_INCLUDED
#define VALUE_CLASS_H_INCLUDED

#include "common.h"
   using std::cout;
   using std::endl;
   using std::string;
   using std::vector;
/* -------------------------------------------------------------------------- */
   class Value
   {
       private:
              // value_TypeName will be used for
              // setting type of the constant
              Types value_type; // Тип переменной
              union
              {
                  _int_t    ival;
                  _long_t   lival;
                  _float_t  fval;
                  _double_t dval;
              } value;
              std::string sval;// if value_type is string constant
       public:
             Value():value_type(VOID),sval(""){};
             Value (Types ValType);
            /** @brief Costructor receive string from parser
             *  and turn it into variable.The type of variable
             *  depends on VarType value(types.h).*/
             Value(const std::string str,Types ValType);
             Value(const string str, Types ValType,_int_t ival);
             //Value(Types t){value_type = t;};
             Value(_int_t i){value_type = INT;value.ival = i;};
             Value(_long_t l){value_type = LINT;value.lival = l;};
             Value(_float_t f){value_type = FLOAT;value.fval = f;};
             Value(_double_t d){value_type = DOUBLE;value.dval = d;};
             Value(string s){value_type = STRING;sval = s;};
           // ~Value(){};
             Value operator +  (const Value& prev);
             Value operator -  (const Value& prev);                    // FIXME!
             Value operator -  ();
             Value operator *  (const Value& prev);
             Value operator /  (const Value& prev);
             Value operator &  (const Value& prev);
             Value operator |  (const Value& prev);
             Value operator ^  (const Value& prev);
             Value operator ~  ();
             Value &operator = (const Value& prev);
             Value &operator = (const _int_t i);
             //_bool_t  operator == (null ZERO);
             Value operator == (const Value& prev);
             Value operator != (const Value& prev);
             Value operator >  (const Value& prev);
             Value operator >= (const Value& prev);
             Value operator <  (const Value& prev);
             Value operator <= (const Value& prev);
             Value operator || (const Value& prev);
             Value operator && (const Value& prev);

             inline _int_t    Get_ival   () const {return value.ival;};
             inline _long_t   Get_lival  () const {return value.lival;};
             inline _float_t  Get_fval   () const {return value.fval;};
             inline _double_t Get_dval   () const {return value.dval;};
             std::string      Get_sval   () const {return sval;};
             Types GetType () const {return value_type;};

             void  SetType(Types type){value_type = type;};

             void  Set_ival(_int_t i) {value.ival = i;};
             void  Set_lival(_long_t l) {value.lival = l;};
             void  Set_fval(_double_t f) {value.fval = f;};
             void  Set_dval(_double_t d) {value.dval = d;};
             void  Set_sval(string s) {sval = s;};
#ifndef DISABLE_TRACING
            /** @brief Debugging print.void Printer ()
             *  printing to std::cout,or to output file
             *  stream
             */
             void Printer ();
#endif // DISABLE_TRACING
   };
/* -------------------------------------------------------------------------- */
  extern std::ostream& operator << (std::ostream& os,const Value& curr);
/* -------------------------------------------------------------------------- */
  void val_FreeArray (val_Arr_t *array);
/* -------------------------------------------------------------------------- */
#endif // VALUE_CLASS_H_INCLUDED
