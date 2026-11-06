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

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <limits>
   using std::numeric_limits;

#include "lib.h"
#include "function.h"
#include "error_handler.h"
#include "graphics.h"
#include "pool.h"
#include "statement.h"
#include "expression.h"
#include "parser.h"
/* -------------------------------------------------------------------------- */
extern void ClearAllArraysElements(arr_Arr_t *arrays);
/* -------------------------------------------------------------------------- */
  Value func_Calculate(const expr_Node_t *expr,val_Arr_t *params)
  {
     assert(expr->GetKind() == EK_PROC);
     Value result;
     switch (expr->GetProcKind())
     {
        case PK_SIN:
           result.SetType(DOUBLE);
           switch ((*params)[0]->GetType())
           {
              case INT:
                 result.Set_dval(sin((*params)[0]->Get_ival()));
              break;
              case LINT:
                 result.Set_dval(sin((*params)[0]->Get_lival()));
              break;
              case FLOAT:
                 result.Set_dval(sin((*params)[0]->Get_fval()));
              break;
              case DOUBLE:result.Set_dval(sin((*params)[0]->Get_dval()));
              break;
              case STRING:  error(CANNOT_CONVERT_TYPE);
              default: assert(0);
           }
        break;
        case PK_COS:
           result.SetType(DOUBLE);
           switch ((*params)[0]->GetType())
           {
              case INT:
                 result.Set_dval(cos((*params)[0]->Get_ival()));
              break;
              case LINT:
                 result.Set_dval(cos((*params)[0]->Get_lival()));
              break;
              case FLOAT:
                 result.Set_dval(cos((*params)[0]->Get_fval()));
              break;
              case DOUBLE:result.Set_dval(cos((*params)[0]->Get_dval()));
              break;
              case STRING:  error(CANNOT_CONVERT_TYPE);
              default: assert(0);
           }
        break;
        case PK_TAN:
           result.SetType(DOUBLE);
           switch ((*params)[0]->GetType())
           {
              case INT:
                 result.Set_dval(tan((*params)[0]->Get_ival()));
              break;
              case LINT:
                 result.Set_dval(tan((*params)[0]->Get_lival()));
              break;
              case FLOAT:
                 result.Set_dval(tan((*params)[0]->Get_fval()));
              break;
              case DOUBLE: result.Set_dval(tan((*params)[0]->Get_dval()));
              break;
              case STRING: error(CANNOT_CONVERT_TYPE);
              default: assert(0);
           }
        break;
        case PK_ATN:
           result.SetType(DOUBLE);
           switch ((*params)[0]->GetType())
           {
              case INT:
                 result.Set_dval(atan((*params)[0]->Get_ival()));
              break;
              case LINT:
                 result.Set_dval(atan((*params)[0]->Get_lival()));
              break;
              case FLOAT:
                 result.Set_dval(atan((*params)[0]->Get_fval()));
              break;
              case DOUBLE: result.Set_dval(atan((*params)[0]->Get_dval()));
              break;
              case STRING: error(CANNOT_CONVERT_TYPE);
              default: assert(0);
           }
        break;
        case PK_VAL:{
           if ((*params)[0]->GetType() != STRING) error(CONST_STR_EXPECTED);
           string tmp = (*params)[0]->Get_sval();
           switch(parser_CheckNumInStr(tmp))
           {
              case TOKEN_CONST_INT:
                 result.SetType(INT);
                 result.Set_ival(atoi(tmp.c_str()));
              break;
              case TOKEN_CONST_LINT:
                 result.SetType(LINT);
                 result.Set_lival(atol(tmp.c_str()));
              break;
              case TOKEN_CONST_FLOAT:
                 result.SetType(FLOAT);
                 result.Set_fval(static_cast<_float_t>(atof(tmp.c_str())));
              break;
              case TOKEN_CONST_DOUBLE:
                 result.SetType(DOUBLE);
                 result.Set_dval(atof(tmp.c_str()));
              break;
              default:assert(0);
           }
        }
        break;
        case PK_STR:{
           Types type = (*params)[0]->GetType();
           if (type == STRING || type == VOID)     error(NUMBER_EXPECTED);
           result.SetType(STRING);
           switch (type)
           {
              case INT:  result.Set_sval(ntos((*params)[0]->Get_ival())); break;
              case LINT: result.Set_sval(ntos((*params)[0]->Get_lival()));break;
              case FLOAT: result.Set_sval(ntos((*params)[0]->Get_fval()));break;
              case DOUBLE:result.Set_sval(ntos((*params)[0]->Get_dval()));break;
              default:assert(0);
           }
        }
        break;
        case PK_LEN:{
           if (params->at(0)->GetType() != STRING)
              runtime_error(WRONG_PARAMETER_TYPE,expr->GetCurFile(),
                                                            expr->GetCurLine());
           result.SetType(INT);
           result.Set_ival(params->at(0)->Get_sval().size());
        }
        break;
        case PK_SQR:{
           Types t;
           if ((t = (*params)[0]->GetType()) == STRING)
              runtime_error(WRONG_PARAMETER_TYPE,expr->GetCurFile(),
                                                            expr->GetCurLine());
           result.SetType(t);
           switch (t)
           {
              case INT:{
                 _double_t d;
                 d = sqrt(static_cast<_double_t>((*params)[0]->Get_ival()));
                 result.Set_ival(static_cast<_int_t>(d));
              break;}
              case LINT:{
                 _double_t d;
                 d = sqrt(static_cast<_double_t>((*params)[0]->Get_lival()));
                 result.Set_lival(static_cast<_long_t>(d));
              break;}
              case FLOAT:{
                 _float_t f;
                 f = sqrt((*params)[0]->Get_fval());
                 result.Set_fval(f);
              break;}
              case DOUBLE:{
                 _double_t d;
                 d = sqrt((*params)[0]->Get_dval());
                 result.Set_dval(d);
              break;}
              default: assert (0);
           }
        break;}
        case PK_ABS:
           switch((*params)[0]->GetType())
           {
              case INT:
                 result.SetType(INT);
                 result.Set_ival(abs((*params)[0]->Get_ival()));
              break;
              case LINT:
                 result.SetType(LINT);
                 result.Set_lival(abs((*params)[0]->Get_lival()));
              break;
              case FLOAT:
                 result.SetType(FLOAT);
                 result.Set_fval(fabs((*params)[0]->Get_fval()));
              break;
              case DOUBLE:
                 result.SetType(DOUBLE);
                 result.Set_dval(fabs((*params)[0]->Get_dval()));
              break;
              case STRING:runtime_error(WRONG_PARAMETER_TYPE,expr->GetCurFile(),
                                                            expr->GetCurLine());
              default:assert(0);
           }
        break;
        case PK_INSTR:{
           size1 size = params->size();
           size_t check;
           switch (size)
           {
              case 2:{
                 if ((*params)[0]->GetType() != STRING ||
                     (*params)[1]->GetType() != STRING   )
                    error(WRONG_PARAMETER_TYPE);
                 string str1 = (*params)[0]->Get_sval();
                 string str2 = (*params)[1]->Get_sval();
                 check = str1.find(str2);
                 if (check == std::string::npos)
                 {
                    result.Set_ival(0);
                    break;
                 }
                 if (check > static_cast<size_t>(numeric_limits<_int_t>::max()))
                 {
                    if (check >
                            static_cast<size_t>(numeric_limits<_long_t>::max()))
                    {
                       error(VARIABLE_OUT_OF_RANGE);
                    }
                    result.SetType(LINT);
                    result.Set_lival(check+1);
                    break;
                 }
                 result.SetType(INT);
                 result.Set_ival(check+1);
              break;}
              case 3:{
                 if (((*params)[0]->GetType() != INT &&
                      (*params)[0]->GetType() != LINT  ) ||
                     (*params)[1]->GetType() != STRING   ||
                     (*params)[2]->GetType() != STRING     )
                    runtime_error(WRONG_PARAMETER_TYPE,expr->GetCurFile(),
                                                            expr->GetCurLine());
                 string str1 = (*params)[1]->Get_sval();
                 string str2 = (*params)[2]->Get_sval();
                 size_t pos;
                 if ((*params)[0]->GetType() == INT)
                    pos = (*params)[0]->Get_ival();
                 else
                    pos = (*params)[0]->Get_lival();
                 if (pos > str1.size())
                    error(PARAM_OUT_OF_STRING_RANGE);
                 check = str1.find(str2,pos);
                 if (check == std::string::npos)
                 {
                    result.Set_ival(0);
                    break;
                 }
                 if (check > static_cast<size_t>(numeric_limits<_int_t>::max()))
                 {
                    if (check >
                            static_cast<size_t>(numeric_limits<_long_t>::max()))
                    {
                       error(VARIABLE_OUT_OF_RANGE);
                    }
                    result.SetType(LINT);
                    result.Set_lival(check+1);
                    break;
                 }
                 result.SetType(INT);
                 result.Set_ival(check+1);
              break;}
              default:assert(0);
           }
        break;}
        case PK_LEFT:{
           Types t(VOID);
           if ((*params)[0]->GetType() != STRING   ||
               ( (t = (*params)[1]->GetType()) != INT && t != LINT))
              runtime_error(WRONG_PARAMETER_TYPE,expr->GetCurFile(),
                                                            expr->GetCurLine());
           string str1 = (*params)[0]->Get_sval();
           size_t pos;
           if (t == INT)
              pos = (*params)[1]->Get_ival();
           else
              pos = (*params)[1]->Get_lival();

           if (pos > str1.size()) error(PARAM_OUT_OF_STRING_RANGE);

           result.SetType(STRING);
           result.Set_sval(str1.substr(0,pos));
        break;}
        case PK_RIGHT:{
           Types t(VOID);
           if ((*params)[0]->GetType() != STRING   ||
               ( (t = (*params)[1]->GetType()) != INT && t != LINT))
              runtime_error(WRONG_PARAMETER_TYPE,expr->GetCurFile(),
                                                            expr->GetCurLine());
           string str1 = (*params)[0]->Get_sval();
           size_t pos;
           size_t size = str1.size();
           if (t == INT)
              pos = (*params)[1]->Get_ival();
           else
              pos = (*params)[1]->Get_lival();

           if (pos > str1.size()) error(PARAM_OUT_OF_STRING_RANGE);

           result.SetType(STRING);
           result.Set_sval(str1.substr(size-pos,size));
        break;}
        case PK_MID:{
           Types t1(VOID),t2(VOID);
           if ((*params)[0]->GetType() != STRING                     ||
               ((t1 = (*params)[1]->GetType()) != INT && t1 != LINT) ||
               ((t2 = (*params)[2]->GetType()) != INT && t2 != LINT)   )
              runtime_error(WRONG_PARAMETER_TYPE,expr->GetCurFile(),
                                                            expr->GetCurLine());
           string str1 = (*params)[0]->Get_sval();
           size_t pos1,pos2;
           size_t size = str1.size();
           if (t1 == INT)
              pos1 = (*params)[1]->Get_ival();
           else
              pos1 = (*params)[1]->Get_lival();

           if (t2 == INT)
              pos2 = (*params)[2]->Get_ival();
           else
              pos2 = (*params)[2]->Get_lival();

           if (pos1 > size || pos2 > size)
              error(PARAM_OUT_OF_STRING_RANGE);
           result.SetType(STRING);
           result.Set_sval(str1.substr(pos1-1,pos2));
        break;}
        case PK_INT:{
           Types type = (*params)[0]->GetType();

           if (type != FLOAT && type != DOUBLE && type != INT && type != LINT)
              runtime_error(WRONG_PARAMETER_TYPE,expr->GetCurFile(),
                                                            expr->GetCurLine());
           result.SetType(INT);
           switch (type) {
              case INT:
                 if ( (*params)[0]->Get_ival() < 0 )
                    result.Set_ival(-abs((*params)[0]->Get_ival()));
                 else
                    result.Set_ival( abs((*params)[0]->Get_ival())); break;
              case LINT:
                 if ( (*params)[0]->Get_ival() < 0 )
                    result.Set_ival(-abs((*params)[0]->Get_lival()));
                 else
                    result.Set_ival( abs((*params)[0]->Get_lival())); break;
              case FLOAT:
                 if ( (*params)[0]->Get_ival() < 0 )
                    result.Set_ival(-abs((*params)[0]->Get_fval()));
                 else
                    result.Set_ival( abs((*params)[0]->Get_fval())); break;
              case DOUBLE:
                 if ( (*params)[0]->Get_ival() < 0 )
                    result.Set_ival(-abs((*params)[0]->Get_dval()));
                 else
                    result.Set_ival( abs((*params)[0]->Get_dval())); break;
              default: assert (0);
           }
        break;}
        default:assert(0);
     }
     return result;
  }
/* -------------------------------------------------------------------------- */
  Value func_Calculate(const stmt_Node_t *stmt,val_Arr_t *params,
                                                              var_Arr_t *vars,
                                                              arr_Arr_t *arrays)
  {
     assert(stmt->GetKind() == SK_CALL);
     Value result;
#ifndef DISABLE_SDL_GRAPHICS
     size3 size = params->size();
#endif // DISABLE_SDL_GRAPHICS
     switch (stmt->GetFuncKind())
     {
        case PK_PRINT:{
           if (params->empty())
           {  // Empty PRINT: printing end of line.
              cout << endl;
              break;
           }
           for (count i = 0; i < size; ++i)
              cout << *(params->at(i));
        break;}
#ifndef DISABLE_SDL_GRAPHICS
        case PK_SCREEN:{
           for (size3 i = 0; i < size; ++i) // 4 is quantity of integer parameters
           {
              switch (params->at(i)->GetType())
              {
                 case INT:break;
                 case LINT:
                    params->at(i)->SetType(INT);
                    params->at(i)->
                            Set_ival(scast<_int_t>(params->at(i)->Get_lival()));
                 break;
                 case FLOAT:
                    params->at(i)->SetType(INT);
                    params->at(i)->
                            Set_ival(scast<_int_t>(params->at(i)->Get_fval()));
                 break;
                 case DOUBLE:
                    params->at(i)->SetType(INT);
                    params->at(i)->
                            Set_ival(scast<_int_t>(params->at(i)->Get_dval()));
                 break;
                 case STRING:
                    runtime_error(WRONG_PARAMETER_TYPE,stmt->GetCurFile(),
                                                            stmt->GetCurLine());
                 break;
                 default: assert(0);
              }
           }
           graphics_SCREEN (params);
        }
        break;
        case PK_PRESET:{
           for (size3 i = 0; i < size; ++i)
           {
              switch (params->at(i)->GetType())
              {
                 case INT:break;
                 case LINT:
                    params->at(i)->SetType(INT);
                    params->at(i)->
                            Set_ival(scast<_int_t>(params->at(i)->Get_lival()));
                 break;
                 case FLOAT:
                    params->at(i)->SetType(INT);
                    params->at(i)->
                            Set_ival(scast<_int_t>(params->at(i)->Get_fval()));
                 break;
                 case DOUBLE:
                    params->at(i)->SetType(INT);
                    params->at(i)->
                            Set_ival(scast<_int_t>(params->at(i)->Get_dval()));
                 break;
                 case STRING:
                    runtime_error(WRONG_PARAMETER_TYPE,stmt->GetCurFile(),
                                                            stmt->GetCurLine());
                 break;
                 default: assert(0);
              }
           }
           graphics_PRESET (params);
        }
        break;
        case PK_LINE:{
           for (size3 i = 0; i < 4; ++i) // 4 is quantity of integer parameters
           {
              switch (params->at(i)->GetType())
              {
                 case INT:break;
                 case LINT:
                    params->at(i)->SetType(INT);
                    params->at(i)->
                            Set_ival(scast<_int_t>(params->at(i)->Get_lival()));
                 break;
                 case FLOAT:
                    params->at(i)->SetType(INT);
                    params->at(i)->
                            Set_ival(scast<_int_t>(params->at(i)->Get_fval()));
                 break;
                 case DOUBLE:
                    params->at(i)->SetType(INT);
                    params->at(i)->
                            Set_ival(scast<_int_t>(params->at(i)->Get_dval()));
                 break;
                 case STRING:
                    runtime_error(WRONG_PARAMETER_TYPE,stmt->GetCurFile(),
                                                            stmt->GetCurLine());
                 break;
                 default: assert(0);
              }
           }
           for (size3 i = 0; i < size; ++i)
           {
              if (((i == 4) && (params->at(i)->GetType() != STRING &&
                                params->at(i)->GetType() != INT      )) ||

                  ((i == 5) && params->at(i)->GetType() != STRING)        )
                 runtime_error(WRONG_PARAMETER_TYPE,stmt->GetCurFile(),
                                                            stmt->GetCurLine());
           }
           graphics_LINE (params);
        }
        break;
        case PK_CIRCLE:{
           for (size3 i = 0; i < size; ++i)
           {
              switch (params->at(i)->GetType())
              {
                 case INT:break;
                 case LINT:
                    params->at(i)->SetType(INT);
                    params->at(i)->
                            Set_ival(scast<_int_t>(params->at(i)->Get_lival()));
                 break;
                 case FLOAT:
                    params->at(i)->SetType(INT);
                    params->at(i)->
                            Set_ival(scast<_int_t>(params->at(i)->Get_fval()));
                 break;
                 case DOUBLE:
                    params->at(i)->SetType(INT);
                    params->at(i)->
                            Set_ival(scast<_int_t>(params->at(i)->Get_dval()));
                 break;
                 case STRING:
                    runtime_error(WRONG_PARAMETER_TYPE,stmt->GetCurFile(),
                                                            stmt->GetCurLine());
                 break;
                 default: assert(0);
              }
           }
           graphics_CIRCLE (params);
        }
        break;
#endif
        case PK_CLS:{
           cout << string( 100, '\n' );
        }
        break;
        case PK_END:{
           pool.freall();
           exit(EXIT_SUCCESS);
        }
        break;
        case PK_BEEP:{
           cout << "\a";
        }
        break;
        case PK_CLEAR:{
           ClearAllArraysElements(arrays);
           ClearVariables(vars);
        }
        break;
        default:assert(0);
     }
     return result;
  }
/* -------------------------------------------------------------------------- */
