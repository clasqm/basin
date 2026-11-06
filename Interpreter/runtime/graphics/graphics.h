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

#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "value.h"
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_SDL_GRAPHICS
  void graphics_SCREEN (val_Arr_t *params);
  void graphics_PRESET (val_Arr_t *params);
  void graphics_LINE   (val_Arr_t *params);
  void graphics_CIRCLE (val_Arr_t *params);
#endif
/* -------------------------------------------------------------------------- */
#endif // GRAPHICS_H_INCLUDED
