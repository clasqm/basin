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

#ifndef VARIABLE_CLASS_H_INCLUDED
#define VARIABLE_CLASS_H_INCLUDED

#include "common.h"
   using std::cout;
   using std::endl;
   using std::string;
   using std::vector;
   using std::map;

#include "value.h"
/* -------------------------------------------------------------------------- */
   class Variable
   {
       private:
           Value *val;
           string var_name;
           Types var_type;
           _bool_t is_shared;
           _bool_t is_initialized;
           var_Arr_t *page; // In which page variable located.
       public:
           Variable (const string &name);
           Variable (const string &name,Types type);
           Variable (const string &name,Value *value);

           string     GetName () const {return var_name;};
           Types      GetType () const {return var_type;};
           Value     *GetValue() const {
              //assert(val != NULL);
              return val;
            };
           bool       isValueNULL   () const  {return val == NULL;};

           //var_Arr_t *GetPage () const {return page;};

           void   SetValue_type    (Types t)     {val->SetType(t);   };
           void   SetValue_ival    (_int_t i)    {val->Set_ival(i);  };
           void   SetValue_lival   (_long_t l)   {val->Set_lival(l); };
           void   SetValue_fval    (_float_t f)  {val->Set_fval(f);  };
           void   SetValue_dval    (_double_t d) {val->Set_dval(d);  };
           void   SetValue_sval    (string s)    {val->Set_sval(s);  };

           void   SetVar_name      (string s)    {var_name = s;};
           void   SetType          (Types t)     {var_type = t;};
           void   SetShared        (_bool_t b)   {is_shared = b;};

           void   SetPage          (var_Arr_t *p){page = p;};
           void   SetInitialization(_bool_t b)   {is_initialized = b;}
           void   SetValue         (Value *v){
              val = v;
              var_type = v->GetType();
              is_initialized = true;
           };
           void   SetValue         (Value const &v){
              assert(val != NULL);
              var_type = v.GetType();
              // FIXME if type already set
              val->SetType(v.GetType());
              *val = v;
              is_initialized = true;
           };

           _bool_t IsInitialized() const {return is_initialized;};
           _bool_t IsShared     () const {return is_shared;};

           Value &operator = (Variable& val);
           Value &operator = (Value& val);

           void Printer();
   };
/* -------------------------------------------------------------------------- */
   extern void ClearVariables (var_Arr_t *vars);
   void var_FreeArray (var_Arr_t *array);
/* -------------------------------------------------------------------------- */

#endif // VARIABLE_CLASS_H_INCLUDED
