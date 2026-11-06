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

/* -------------------------------------------------------------------------- */
#include <cassert>
#include <iostream>
   using std::cout;
   using std::cin;
   using std::endl;
/* -------------------------------------------------------------------------- */
#include "interp.h"
#include "expression.h"
#include "statement.h"
#include "array.h"
#include "label.h"
#include "subroutine.h"
#include "error_handler.h"
#include "options.h"
#include "tracer.h"
#include "function.h"
#include "pool.h"
#include "lib.h"
/* -------------------------------------------------------------------------- */
  static void         interp_SetArrayElement (const expr_Node_t *expr,
                                                          Value *val,
                                                      var_Arr_t *vars,
                                                      arr_Arr_t *arrays,
                                                      lab_Arr_t *labels);

  static Value        interp_GetArrayElement(const expr_Node_t *expr,
                                                     var_Arr_t *vars,
                                                     arr_Arr_t *arrays,
                                                     lab_Arr_t *labels);

  static Value        interp_CalcExpr       (const expr_Node_t *expr,
                                                     var_Arr_t *vars,
                                                     arr_Arr_t *arrays,
                                                     lab_Arr_t *labels);

  static val_Arr_t   *interp_CalcParameters (const expr_Node_t *expr,
                                                     var_Arr_t *vars,
                                                     arr_Arr_t *arrays,
                                                     lab_Arr_t *labels);

  static val_Arr_t   *interp_CalcParameters (const stmt_Node_t *stmt,
                                                     var_Arr_t *vars,
                                                     arr_Arr_t *arrays,
                                                     lab_Arr_t *labels);

  static stmt_Node_t *interp_stmtLET        (      stmt_Node_t *stmt,
                                                     var_Arr_t *vars,
                                                     arr_Arr_t *arrays,
                                                     lab_Arr_t *labels);

  static stmt_Node_t *interp_stmtINPUT      (      stmt_Node_t *stmt,
                                                     var_Arr_t *vars,
                                                     arr_Arr_t *arrays,
                                                     lab_Arr_t *labels);

  static stmt_Node_t *interp_stmtCALL       (const stmt_Node_t *stmt,
                                                     var_Arr_t *vars,
                                                     arr_Arr_t *arrays,
                                                     lab_Arr_t *labels);

  static stmt_Node_t *interp_stmtRETURN     (const stmt_Node_t *stmt);

  static stmt_Node_t *interp_stmtCBranch    (const stmt_Node_t *stmt,
                                                     var_Arr_t *vars,
                                                     arr_Arr_t *arrays,
                                                     lab_Arr_t *labels);
/* -------------------------------------------------------------------------- */
  static void interp_SetArrayElement (const expr_Node_t *expr,Value *value,
                                                              var_Arr_t *vars,
                                                              arr_Arr_t *arrays,
                                                              lab_Arr_t *labels)
  {  // In case of array(i+1,j+5) we need to calculate all expressions
     // and get element in array with only one index.
     assert(expr->GetKind() == EK_ARR_EL);
     size3 array_global_i = findVarName (*expr->GetArrElemName(),arrays);
     if (array_global_i < 0)
     {  //Try search in shared arrays
        array_global_i = findVarName (*expr->GetArrElemName(),
                                       pool.GetSharedArrs()   );
        assert (array_global_i >= 0);
        arrays = pool.GetSharedArrs();
     }
     size3 arr_el_size = static_cast<size3>(expr->GetArrayIndexes()->size());
     if (arr_el_size != arrays->at(array_global_i)->GetDimensionsSize())
        runtime_error(WRONG_PARAMETERS_LIST,expr->GetCurFile(),
                                                            expr->GetCurLine());
     if (arr_el_size == 1)
     {  // In case of one dimension
        Value val(LINT);// This is for index,must contain at least long int.
        val =interp_CalcExpr(expr->GetArrayIndexes()->at(0),vars,arrays,labels);
        // Check is index in range
        if (val.Get_lival() >
            arrays->at(array_global_i)->GetStartIndexAt(0) +
            arrays->at(array_global_i)->GetDimension(0) -1     ||
            val.Get_lival() <
            arrays->at(array_global_i)->GetStartIndexAt(0)     )
           runtime_error(VARIABLE_OUT_OF_RANGE,expr->GetCurFile(),
                                                            expr->GetCurLine());
        size3 index = val.Get_lival() -
                                 arrays->at(array_global_i)->GetStartIndexAt(0);
        arrays->at(array_global_i)->SetElement(index,value);
        arrays->at(array_global_i)->SetInitAt(index,true);
        return;
     }
     // Here we have multidimensional array
     // Let's say we have array with dimensions array[x][y][z].Each array,even
     // multidimensional,can be represented as one linear set of elements,where
     // access to element will be calculated by this formula:
     /// array[a][b][c] = linear_array[(a-h)*y*z + (b-h)*z + (c-h)]
     // where 'h' is default base of array or set by user with
     // OPTION BASE command or in this: (h TO n,...) variant of array
     // definition.
     // To get element,located at array[a][b][c],we use this formula,where all
     // dimensions known and stored in arrays->dimensions array(look in array.h)
     sz3_Arr_t temp;// Temporary array for indexes,that are stored in after
                    // their calculation.Remember,we need to calculate each
                    // index as expression.
     for (size3 i = 0; i < arr_el_size; ++i)
     {
        Value val(LINT);// This is for index,must contain at least long int.
        val =interp_CalcExpr(expr->GetArrayIndexes()->at(i),vars,arrays,labels);
        temp.push_back(val.Get_lival());
     }
     // Now will be calculated index in linear array,by using the formula above.
     size3 tmp_size = static_cast<size3>(temp.size());
     assert(tmp_size == arr_el_size);
     size3 index = 0;
     for (size3 i = 0; i < tmp_size-1; ++i)
     {  // Checking that index is in range
        if (temp.at(i) > arrays->at(array_global_i)->GetStartIndexAt(i) +
                         arrays->at(array_global_i)->GetDimension(i) -1   ||
            temp.at(i) < arrays->at(array_global_i)->GetStartIndexAt(i)      )
           runtime_error(VARIABLE_OUT_OF_RANGE,expr->GetCurFile(),
                                                            expr->GetCurLine());
        size3 tmp_sum = 1;
        for (size3 j = i+1; j < tmp_size; ++j)
           tmp_sum *= arrays->at(array_global_i)->GetDimension(j);
        index += (temp.at(i) -
                  arrays->at(array_global_i)->GetStartIndexAt(i))*tmp_sum;
     }
     // Checking last index
     if (temp.at(tmp_size-1) >
            arrays->at(array_global_i)->GetStartIndexAt(tmp_size-1) +
            arrays->at(array_global_i)->GetDimension(tmp_size-1) -1    ||
         temp.at(tmp_size-1) <
            arrays->at(array_global_i)->GetStartIndexAt(tmp_size-1)     )
        runtime_error(VARIABLE_OUT_OF_RANGE,expr->GetCurFile(),
                                                            expr->GetCurLine());
     // (tmp_size-1) can be used below because of previous
     // assert(tmp_size == arr_el_size).
     index += temp.back() -
              arrays->at(array_global_i)->GetStartIndexAt(tmp_size-1);
     arrays->at(array_global_i)->SetElement(index,value);
     arrays->at(array_global_i)->SetInitAt(index,true);
  }
/* -------------------------------------------------------------------------- */
  static Value interp_GetArrayElement (const expr_Node_t *expr,
                                                              var_Arr_t *vars,
                                                              arr_Arr_t *arrays,
                                                              lab_Arr_t *labels)
  {  // In case of array(i+1,j+5) we need to calculate all expressions
     // and get element in array with only one index.
     assert(expr->GetKind() == EK_ARR_EL);
     size3 array_global_i = findVarName (*expr->GetArrElemName(),arrays);
     if (array_global_i < 0)
     {  //Try search in shared arrays
        array_global_i = findVarName (*expr->GetArrElemName(),
                                       pool.GetSharedArrs()   );
        assert (array_global_i >= 0);
        arrays = pool.GetSharedArrs();
     }
     size3 arr_el_size = static_cast<size3>(expr->GetArrayIndexes()->size());
     if (arr_el_size != arrays->at(array_global_i)->GetDimensionsSize())
        runtime_error(WRONG_PARAMETERS_LIST,expr->GetCurFile(),
                                                            expr->GetCurLine());
     if (arr_el_size == 1)
     {  // In case of one dimension
        Value val(LINT);// This is for index,must contain at least long int.
        val =interp_CalcExpr(expr->GetArrayIndexes()->at(0),vars,arrays,labels);
        // Check is index in range
        if (val.Get_lival() > arrays->at(array_global_i)->GetStartIndexAt(0) +
                              arrays->at(array_global_i)->GetDimension(0) -1  ||
            val.Get_lival() < arrays->at(array_global_i)->GetStartIndexAt(0)   )
           runtime_error(VARIABLE_OUT_OF_RANGE,expr->GetCurFile(),
                                                            expr->GetCurLine());

        if (cmd_options["--noinit-errors"]) //Initialization control option(cmd)
           if (!arrays->at(array_global_i)->IsInitAt(0))
              runtime_error(NON_INIT_VAR,arrays->at(array_global_i)->GetName(),
                                                            expr->GetCurFile(),
                                                            expr->GetCurLine());

        return arrays->at(array_global_i)->GetValueAt
                           (val.Get_lival()-
                            arrays->at(array_global_i)->GetStartIndexAt(0));
     }
     // Now we have multidimensional array
     // Let's say we have array with dimensions array[x][y][z].Each array,even
     // multidimensional,can be represented as one linear set of elements,where
     // access to element will be calculated by this formula:
     /// array[a][b][c] = linear_array[(a-h)*y*z + (b-h)*z + (c-h)]
     // where 'h' is default base of array or set by user with
     // OPTION BASE command or in this: (h TO n,...) variant of array
     // definition.
     // To get element,located at array[a][b][c],we use this formula,where all
     // dimensions known and stored in arrays->dimensions array(look in array.h)
     sz3_Arr_t temp;// Temporary array for indexes,that are stored in after
                    // their calculation.Remember,we need to calculate each
                    // index as expression.
     for (size3 i = 0; i < arr_el_size; ++i)
     {
        Value val(LINT);// This is for index,must contain at least long int.
        val =interp_CalcExpr(expr->GetArrayIndexes()->at(i),vars,arrays,labels);
        temp.push_back(val.Get_lival());
     }
     // Now will be calculated index in linear array,by using the formula above.
     size3 tmp_size = static_cast<size3>(temp.size());
     assert(tmp_size == arr_el_size);
     size3 index = 0;
     for (size3 i = 0; i < tmp_size-1; ++i)
     {  // Checking that index is in range
        if (temp.at(i) >
            arrays->at(array_global_i)->GetStartIndexAt(i) +
            arrays->at(array_global_i)->GetDimension(i) -1     ||
            temp.at(i) <
            arrays->at(array_global_i)->GetStartIndexAt(i)      )
           runtime_error(VARIABLE_OUT_OF_RANGE,expr->GetCurFile(),
                                                         expr->GetCurLine());
        size3 tmp_sum = 1;
        for (size3 j = i+1; j < tmp_size; ++j)
           tmp_sum *= arrays->at(array_global_i)->GetDimension(j);
        index +=
               (temp.at(i) -
                arrays->at(array_global_i)->GetStartIndexAt(i))*tmp_sum;
     }
     // Checking last index
     if (temp.at(tmp_size-1) >
            arrays->at(array_global_i)->GetStartIndexAt(tmp_size-1) +
            arrays->at(array_global_i)->GetDimension(tmp_size-1) -1    ||
         temp.at(tmp_size-1) <
            arrays->at(array_global_i)->GetStartIndexAt(tmp_size-1)     )
        runtime_error(VARIABLE_OUT_OF_RANGE,expr->GetCurFile(),
                                                         expr->GetCurLine());
     // (tmp_size-1) can be used below because of previous
     // assert(tmp_size == arr_el_size).
     index += temp.back() -
              arrays->at(array_global_i)->GetStartIndexAt(tmp_size-1);

     if (cmd_options["--noinit-errors"]) // Initialization control option
        if (!arrays->at(array_global_i)->IsInitAt(index))
           runtime_error(NON_INIT_VAR,arrays->at(array_global_i)->GetName(),
                                                            expr->GetCurFile(),
                                                            expr->GetCurLine());

     return arrays->at(array_global_i)->GetValueAt(index);
  }
/* -------------------------------------------------------------------------- */
  static size3 interp_CalcArrElemIndex (const stmt_Node_t *stmt,
                                                              var_Arr_t *vars,
                                                              arr_Arr_t *arrays,
                                                              lab_Arr_t *labels)
  {
     // FIXME! Add searching in shared arrays!
     switch (stmt->GetKind())
     {
        case SK_LET:{
           size3 arr_el_size =
                             static_cast<size3>(stmt->GetParameters()->size());
           if (arr_el_size != stmt->GetArrayPtr()->GetDimensionsSize())
              runtime_error(WRONG_PARAMETERS_LIST,stmt->GetCurFile(),
                                                            stmt->GetCurLine());
           if (arr_el_size == 1)
           {  // In case of one dimension
              Value val(LINT);// This is for index,must contain at least long
                              // int.
              val = interp_CalcExpr(stmt->GetParameters()->at(0),vars,arrays,
                                                                        labels);
              // Check is index in range
              if (val.Get_lival() >
                  stmt->GetArrayPtr()->GetStartIndexAt(0) +
                  stmt->GetArrayPtr()->GetDimension(0)  -1    ||
                  val.Get_lival() <
                  stmt->GetArrayPtr()->GetStartIndexAt(0)     )
                 runtime_error(VARIABLE_OUT_OF_RANGE,stmt->GetCurFile(),
                                                            stmt->GetCurLine());

              // Initialization control option(command-line option)
              if (cmd_options["--noinit-errors"])
                 if (!stmt->GetArrayPtr()->IsInitAt(0))
                    runtime_error(NON_INIT_VAR,stmt->GetArrayPtr()->GetName(),
                                                            stmt->GetCurFile(),
                                                            stmt->GetCurLine());

              return val.Get_lival()-stmt->GetArrayPtr()->GetStartIndexAt(0);
           }
           // Now we have multidimensional array
           // Let's say we have array with dimensions array[x][y][z].Each array,
           // even multidimensional,can be represented as one linear set of
           // elements,where access to element will be calculated by this
           // formula:
           /// array[a][b][c] = linear_array[(a-h)*y*z + (b-h)*z + (c-h)]
           // where 'h' is default base of array or set by user with
           // OPTION BASE command or in this: (h TO n,...) variant of array
           // definition.
           // To get element,located at array[a][b][c],we use this formula,where
           // all dimensions known and stored in
           // arrays->dimensions array(look in array.h).
           sz3_Arr_t temp;//Temporary array for indexes,that are stored in after
                          //their calculation.Remember,we need to calculate each
                          //index as expression.
           for (size3 i = 0; i < arr_el_size; ++i)
           {
              Value val(LINT);// This is for index,must contain at least long
                              // int.
              val = interp_CalcExpr(stmt->GetParameters()->at(i),vars,arrays,
                                                                        labels);
              temp.push_back(val.Get_lival());
           }
           // Now will be calculated index in linear array,by using the formula
           size3 tmp_size = static_cast<size3>(temp.size());           // above.
           assert(tmp_size == arr_el_size);
           size3 index = 0;
           for (size3 i = 0; i < tmp_size-1; ++i)
           {  // Checking that index is in range
              if (temp.at(i) >
                      stmt->GetArrayPtr()->GetStartIndexAt(i) +
                      stmt->GetArrayPtr()->GetDimension(i) -1     ||
                  temp.at(i) <
                      stmt->GetArrayPtr()->GetStartIndexAt(i)       )
                 runtime_error(VARIABLE_OUT_OF_RANGE,stmt->GetCurFile(),
                                                            stmt->GetCurLine());
              size3 tmp_sum = 1;
              for (size3 j = i+1; j < tmp_size; ++j)
                 tmp_sum *= stmt->GetArrayPtr()->GetDimension(j);
              index +=
                     (temp.at(i) -
                      stmt->GetArrayPtr()->GetStartIndexAt(i))*tmp_sum;
           }
           // Checking last index
           if (temp.at(tmp_size-1) >
                    stmt->GetArrayPtr()->GetStartIndexAt(tmp_size-1) +
                    stmt->GetArrayPtr()->GetDimension(tmp_size-1) -1    ||
               temp.at(tmp_size-1) <
                    stmt->GetArrayPtr()->GetStartIndexAt(tmp_size-1)      )
              runtime_error(VARIABLE_OUT_OF_RANGE,stmt->GetCurFile(),
                                                            stmt->GetCurLine());
           // (tmp_size-1) can be used below because of previous
           // assert(tmp_size == arr_el_size).
           index += temp.back() -
                    stmt->GetArrayPtr()->GetStartIndexAt(tmp_size-1);
           return index;
        }
        break;
        default: assert (0);
     }
  }
/* -------------------------------------------------------------------------- */
  static val_Arr_t *interp_CalcParameters (const expr_Node_t *expr,
                                                              var_Arr_t *vars,
                                                              arr_Arr_t *arrays,
                                                              lab_Arr_t *labels)
  {
     expr_List_t *tmp = expr->GetParameters();
     val_Arr_t *parameters = pool.alloc<val_Arr_t>();
     if (tmp->size() != 0)
     {
        size3 size = tmp->size();
        for (size3 i = 0; i < size; ++i)
        {
           Value *v = pool.alloc<Value>();
           *v = interp_CalcExpr(tmp->at(i),vars,arrays,labels);
           parameters->push_back(v);
        }
        return parameters;
     }
     return parameters;
  }
/* -------------------------------------------------------------------------- */
  static val_Arr_t *interp_CalcParameters (const stmt_Node_t *stmt,
                                                              var_Arr_t *vars,
                                                              arr_Arr_t *arrays,
                                                              lab_Arr_t *labels)
  {
     expr_List_t *tmp = stmt->GetParameters();
     val_Arr_t *parameters = pool.alloc<val_Arr_t>();
     if (tmp->size() != 0)
     {
        size3 size = tmp->size();
        for (size3 i = 0; i < size; ++i)
        {
           Value *v = pool.alloc<Value>();
           *v = interp_CalcExpr(tmp->at(i),vars,arrays,labels);
           parameters->push_back(v);
        }
        return parameters;
     }
     return parameters;
  }
/* -------------------------------------------------------------------------- */
  static Value interp_CalcExpr (const expr_Node_t *expr,var_Arr_t *vars,
                                                   arr_Arr_t *arrays,
                                                   lab_Arr_t *labels)
  {
     switch (expr->GetKind())
     {
        case EK_ARR_EL:return interp_GetArrayElement(expr,vars,arrays,labels);
        case EK_CONST: return expr->GetValue();
        case EK_VAR:   return expr->GetVarValue(vars);
        case EK_UNOP:  return expr->CalcUnOp(expr->GetUnOpKind(),
                                    interp_CalcExpr(expr->GetUnOperand(),
                                                           vars,arrays,labels));
        case EK_BINOP: return expr->CalcBinOp
                                       (expr->GetBinOpKind(),
                                        interp_CalcExpr (expr->GetBinOp1(),
                                                           vars,arrays,labels),
                                        interp_CalcExpr (expr->GetBinOp2(),
                                                           vars,arrays,labels));
        case EK_PROC:{
                       Value result;
                       val_Arr_t *parameters = interp_CalcParameters(expr,
                                                                     vars,
                                                                     arrays,
                                                                     labels);
                       result = func_Calculate(expr,parameters);
                       val_FreeArray(parameters);
                       pool.free(parameters);
                       return result;}
        default: assert(0);
     }
  }
/* -------------------------------------------------------------------------- */
  static stmt_Node_t *interp_stmtLET (stmt_Node_t *stmt,var_Arr_t *vars,
                                                        arr_Arr_t *arrays,
                                                        lab_Arr_t *labels)
  {
        switch (stmt->GetLetMode())
        {
           case stmt::let::vartype::ARRAY:{
              _size_t index_in_array = interp_CalcArrElemIndex(stmt,vars,
                                                                 arrays,labels);
              if (stmt->GetArrayPtr()->isValueNULLat(index_in_array)) {
                 // This only can happen when first time writing value to array
                 // element;it cannot be inside function call,because all data
                 // allocated already.
                 Value *value =
                 pool.alloc<Value>(interp_CalcExpr(stmt->GetStmtExpr(),
                                                           vars,arrays,labels));
                 stmt->SetStmtVar(value,index_in_array);
              }
              else {
                 Value value =
                        interp_CalcExpr(stmt->GetStmtExpr(),vars,arrays,labels);
                 stmt->SetStmtVar(value,index_in_array);
              }
           break;}
           case stmt::let::vartype::VAR: {
              if (stmt->GetStmtVar()->isValueNULL()) {
                 Value *value =
                 pool.alloc<Value>(interp_CalcExpr(stmt->GetStmtExpr(),
                                                           vars,arrays,labels));

                 pool.GetCurValsHeap()->push_back(value);
                 stmt->SetStmtVar(value);
              }
              else {
                 Value value (
                       interp_CalcExpr(stmt->GetStmtExpr(),vars,arrays,labels));
                 stmt->SetStmtVar(value);
              }
           break;}
           default: assert(0);
        }
     return stmt->GetNext();
  }
/* -------------------------------------------------------------------------- */
  static stmt_Node_t *interp_stmtINPUT(stmt_Node_t *stmt,var_Arr_t *vars,
                                                         arr_Arr_t *arrays,
                                                         lab_Arr_t *labels)
  {
     cout << stmt->GetHint ();
     if (stmt->PrintQuestionMark())
        cout << " ? ";
     size3 size = stmt->GetParameters()->size();
     for (size3 i = 0; i < size; ++i)
     {
        switch (stmt->GetParameters()->at(i)->GetKind())
        {
           case EK_VAR:    {
              string tmp = stmt->GetParameters()->at(i)->GetVar()->GetName();
              size3 j = findVarName (tmp,vars);
              if (stmt->GetParameters()->at(i)->GetVar()->isValueNULL()) {
                 Types t = stmt->GetParameters()->at(i)->GetVar()->GetType();
                 Value *zero   = pool.alloc<Value>("0",t);
                 stmt->GetParameters()->at(i)->GetVar()->SetValue(zero);
              }
              switch (stmt->GetParameters()->at(i)->GetVar()->GetType())
              {// FIXME may be to use void* reinterpret_cast?
               // One setValue function with typeid?
                 case INT:   {
                    _int_t temp;
                    cin >> temp;
                    vars->at(j)->SetValue_ival(temp);
                 break;}
                 case LINT:  {
                    _long_t temp;
                    cin >> temp;
                    vars->at(j)->SetValue_lival(temp);
                 break;}
                 case FLOAT: {
                    _float_t temp;
                    cin >> temp;
                    vars->at(j)->SetValue_fval(temp);
                 break;}
                 case DOUBLE:{
                    _double_t temp;
                    cin >> temp;
                    vars->at(j)->SetValue_dval(temp);
                 break;}
                 case STRING:{
                    string temp;
                    getline(cin,temp);
                    vars->at(j)->SetValue_sval(temp);
                 break;}
                 default: assert (0);
              }
              vars->at(j)->SetInitialization(true);
           break;}
           case EK_ARR_EL: {
              string tmp = *stmt->GetParameters()->at(i)->GetArrElemName();
              size3 j = findVarName (tmp,arrays);
              switch (arrays->at(j)->GetType())
              {
                 case INT:   {
                    _int_t temp;
                    cin >> temp;
                    Value val(temp);
                    interp_SetArrayElement(stmt->GetParameters()->at(i),&val,
                                                           vars,arrays,labels);
                 break;}
                 case LINT:  {
                    _long_t temp;
                    cin >> temp;
                    Value val(temp);
                    interp_SetArrayElement(stmt->GetParameters()->at(i),&val,
                                                           vars,arrays,labels);
                 break;}
                 case FLOAT: {
                    _float_t temp;
                    cin >> temp;
                    Value val(temp);
                    interp_SetArrayElement(stmt->GetParameters()->at(i),&val,
                                                           vars,arrays,labels);
                 break;}
                 case DOUBLE:{
                    _double_t temp;
                    cin >> temp;
                    Value val(temp);
                    interp_SetArrayElement(stmt->GetParameters()->at(i),&val,
                                                           vars,arrays,labels);
                 break;}
                 case STRING:{
                    string temp;
                    getline(cin,temp);
                    Value val(temp);
                    interp_SetArrayElement(stmt->GetParameters()->at(i),&val,
                                                           vars,arrays,labels);
                 break;}
                 default: assert (0);
              }
           break;}
           default: assert(0);
        }
     }
     return stmt->GetNext();
  }
/* -------------------------------------------------------------------------- */
  static stmt_Node_t *interp_stmtCALL (const stmt_Node_t *stmt,
                                                              var_Arr_t *vars,
                                                              arr_Arr_t *arrays,
                                                              lab_Arr_t *labels)
  {
     if (stmt->GetFuncKind() != PK_USER)
     {
        val_Arr_t *parameters = interp_CalcParameters(stmt,vars,arrays,labels);
        func_Calculate(stmt,parameters,vars,arrays);
        val_FreeArray(parameters);
        pool.free(parameters);
        return stmt->GetNext();
     }
     // User function (that was defined with SUB or FUNC)
     sub_Arr_t *curr_functions = pool.GetCurFuncs();

     size3 i = findVarName (*stmt->GetFuncName(),curr_functions);

     pool.setRunningFunc(i);
     pool.setCurFuncStackDepth(curr_functions->at(i)->getRuntimeCallsCount());

     Subroutine *sub = curr_functions->at(i);

     sub->resVarValsWindow(sub->getVars()->size());/// TODO for arrays too

     pool.SetCurVars(sub->getVars());
     pool.SetCurArrs(sub->getArrs());
     pool.SetCurLbls(sub->getLabs());

     if (sub->isRecursive()) {
        sub->createMemPagesForData();
        sub->setMemPagesForData();
     }

     sub->linkToNextExit();

     return sub->getAddr();
  }
/* -------------------------------------------------------------------------- */
  static stmt_Node_t *interp_stmtRETURN  (const stmt_Node_t *stmt)
  {
     size3 index = findVarName(*stmt->GetFuncName(),pool.GetCurFuncs());
     assert (index >= 0);

     if (stmt->isReturnFromRecursion())
     {  // In this case data (variables,arrays,labels) list will not
        // be changed, each variable's value pointer will be set to new address.
        // For found function in list first destroying memory page on the top of
        // the stack, and then setting all values to new top.
        pool.GetCurFuncs()->at(index)->destroyMemPagesForData();
        pool.GetCurFuncs()->at(index)->setMemPagesForData();
        pool.setCurFuncStackDepth(pool.GetCurFuncs()->
                                  at(index)->getRuntimeCallsCount());
        return stmt->GetNext();
     }

     //pool.GetCurFuncs()->at(index)->destroyMemPagesForData();
     //pool.GetCurFuncs()->at(index)->setMemPagesForData();

     pool.setCurFuncStackDepth(pool.GetCurFuncs()->
                               at(index)->getRuntimeCallsCount());

     pool.SetCurVars(pool.GetPrevVars());
     pool.SetCurArrs(pool.GetPrevArrs());
     pool.SetCurLbls(pool.GetPrevLbls());

     return stmt->GetNext();
  }
/* -------------------------------------------------------------------------- */
  static stmt_Node_t *interp_stmtCBranch (const stmt_Node_t *stmt,
                                                              var_Arr_t *vars,
                                                              arr_Arr_t *arrays,
                                                              lab_Arr_t *labels)
  {
     Value val = interp_CalcExpr(stmt->GetStmtExpr(),vars,arrays,labels);
     // This is relation expression,so in cases < and > always created value
     // of INT type.
     if (val.Get_ival() != 0)
        return stmt->GetCBrLabel(true);
     return stmt->GetCBrLabel(false);
  }
/* -------------------------------------------------------------------------- */
  void interp_Run()
  {
     assert(!stmt_list->empty());
     stmt_Node_t *stmt = stmt_list->front();
     StatementKind kind;
     while ((kind = stmt->GetKind()) != SK_PROGRAM_END)
     {
#ifndef DISABLE_TRACING
        if (stmt->is_tracing_tree())
        {
           stmt->Printer();
           if (kind == SK_CALL)
           {
              tracer.WriteTreeTracing(kind,stmt->GetFuncKind(),
                                                            stmt->GetCurFile(),
                                                            stmt->GetCurLine());
           }
           else
           {
              tracer.WriteTreeTracing(kind,stmt->GetCurFile(),
                                                            stmt->GetCurLine());
           }
        }
#endif // DISABLE_TRACING
        switch (kind)
        {
           case SK_LET:
              stmt = interp_stmtLET     (stmt,pool.GetCurVars(),
                                              pool.GetCurArrs(),
                                              pool.GetCurLbls()); break;
           case SK_CBRANCH:
              stmt = interp_stmtCBranch (stmt,pool.GetCurVars(),
                                              pool.GetCurArrs(),
                                              pool.GetCurLbls()); break;
           case SK_INPUT:
              stmt = interp_stmtINPUT   (stmt,pool.GetCurVars(),
                                              pool.GetCurArrs(),
                                              pool.GetCurLbls()); break;
           case SK_CALL:
              stmt = interp_stmtCALL    (stmt,pool.GetCurVars(),
                                              pool.GetCurArrs(),
                                              pool.GetCurLbls()); break;
           case SK_RETURN:
              stmt = interp_stmtRETURN  (stmt);                   break;
           case SK_LABEL:
              stmt = stmt->GetJumpAddress();                      break;
           case SK_EMPTY_LABEL:
           case SK_BEGIN:
              stmt = stmt->GetNext();                             break;
           default: assert(0);
        }
     }
  }
/* -------------------------------------------------------------------------- */
