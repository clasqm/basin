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

#include <cstring>

#include "common.h"
   using std::string;
   using std::map;
   using std::ifstream;
   using std::cout;
   using std::endl;
   using std::cerr;

#include "config.h"

#include "main.h"
#include "parser.h"
#include "value.h"
#include "variable.h"
#include "syntax.h"
#include "statement.h"
#include "interp.h"
#include "memory.h"
#include "types.h"
#include "options.h"
#include "pool.h"
/* -------------------------------------------------------------------------- */
   ifstream source;
   _char_t* filename;
   const _int_t buffer_size = 255;
/* -------------------------------------------------------------------------- */
   _int_t main(_int_t argc, _char_t* argv[])
   {
       filename = new _char_t[buffer_size];
       cmd_ParamsDefaultInit();
       if (cmd_ParamsCheck(filename,argc,argv))
       {
          delete [] filename;
          exit(EXIT_FAILURE);
       }
       user_opt.Init();
       // ifstream::binary is needed for better portability
       source.open(filename, ifstream::in | ifstream::binary);

       if (!source.is_open())
       {
          std::cerr << "Error opening a file" << endl;
          delete [] filename;
          exit(EXIT_FAILURE);
       }
       if (user_opt.check("--syntax-only"))
       {
          pool.init();
          parser_Init (filename);
          syntax_Check();
          pool.freall();
          source.close();
       }
       else
       {
          pool.init();
          parser_Init (filename);
          syntax_Check();
          interp_Run ();
          pool.freall();
          source.close();
       }

       delete [] filename;
       exit(EXIT_SUCCESS);
   }
/* -------------------------------------------------------------------------- */
