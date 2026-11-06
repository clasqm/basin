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

#include "options.h"
/* -------------------------------------------------------------------------- */
  Options user_opt;
  map <string,_bool_t> cmd_options;
  static void cmd_PrintMenu();
#ifndef DISABLE_TRACING
  ofstream trace_file;
#endif // DISABLE_TRACING
/* -------------------------------------------------------------------------- */
  Options::Options ()
  {
     array_base = 1;       // Default value for start indexes in arrays
#ifndef DISABLE_TRACING
     trace_actions   = false;// Default mode for tracing actions
     trace_silent    = false;// Tracing mode(output to file or to console)
     trace_tree      = false;// Tracing of statements tree
     trace_tokens    = false;// Tracing parcer's work
     debug_functions = false;// Write info about functions into debugging stream
#endif // DISABLE_TRACING
  }
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_TRACING
  void Options::Init()
  {
     trace_actions     = cmd_options["--trace-act"   ];
     trace_tree        = cmd_options["--trace-tree"  ];
     trace_silent      = cmd_options["--trace-silent"];
     trace_tokens      = cmd_options["--trace-tokens"];
     debug_functions   = cmd_options["--debug"       ];

     if (!trace_silent) return;

     trace_file.open("tracing.log");

     if (!trace_file.is_open()) {
        std::cerr << "Error opening a file \"tracing.log\". Trace info will"
                     "be written down to the std::cout." << endl;
        trace_silent = false;
        return;
     }

     trace_file.clear();
  }
#endif // DISABLE_TRACING
/* -------------------------------------------------------------------------- */
  Options::~Options ()
  {
#ifndef DISABLE_TRACING
     if (trace_file.is_open())
        trace_file.close();
#endif // DISABLE_TRACING
  }
/* -------------------------------------------------------------------------- */
  void cmd_ParamsDefaultInit()
  {
     cmd_options[         "--help"] = false;
     cmd_options["--noinit-errors"] = false;
     cmd_options[ "--syntax-only" ] = false;
#ifndef DISABLE_TRACING
     cmd_options[ "--trace-act"   ] = false;
     cmd_options[ "--trace-tree"  ] = false;
     cmd_options[ "--trace-silent"] = false;
     cmd_options[ "--trace-tokens"] = false;
     cmd_options[ "--debug"       ] = false;
#endif // DISABLE_TRACING
  }
/* -------------------------------------------------------------------------- */
  int cmd_ParamsCheck(char *const filename,const _int_t argc,
                                                    const _char_t *const argv[])
  {
      // FIXME cmd_options -> user_opt
      if (argc > static_cast<_int_t>(cmd_options.size() + 2))
      {
         cerr << "\nError: Too much parameters.      \n";
         cmd_PrintMenu();
         return 1;
      }
      if (argc > 2)
      {
         map<string,_bool_t>::const_iterator it;
         map<string,_int_t>temp;
         map<string,_int_t>::const_iterator temp_it;
         for (_int_t i = argc-2; i >= 1; --i)
         {
            it = cmd_options.find(argv[i]);
            temp_it = temp.find(argv[i]);
            if (it == cmd_options.end())
            {
               cerr << "\nError: Unknown parameter.         \n";
               cmd_PrintMenu();
               return 1;
            }
            if (temp_it != temp.end())
            {
               cerr << "\nError: Duplicate parameter.       \n";
               cmd_PrintMenu();
               return 1;
            }
            cmd_options[argv[i]] = true;
            temp[argv[i]];
         }
         strcpy(filename,argv[argc-1]);
         return 0;
      }
      else
      if (argc > 1) {
         strcpy(filename,argv[1]);
         return 0;
      }
      else {
         cmd_PrintMenu();
         return 1;
      }
  }
/* -------------------------------------------------------------------------- */
  static void cmd_PrintMenu()
  {
     cerr << "[Start of menu]\n"
          << "Usage: ./basin [options] [filename]\n"
          << "[options] --noinit-errors - If in program are variables without\n"
          << "            initialization,error will be generated.            \n"
         << "          --syntax-only - Check syntax and quit.               \n";
#ifndef DISABLE_TRACING
     cerr << "          --trace-act - Enable tracing for actions,performed   \n"
          << "            in the program on run-time.                        \n"
          << "          --trace-tree - Enable tracing for program tree,to    \n"
          << "            view which instruction in the program is running.  \n"
          << "          --trace-tokens-Enable tracing for tokens,to view all \n"
          << "            tokens that parser return.                         \n"
          << "          --trace-silent - Write tracing output to file        \n"
          << "            instead of console (Default is to console).Name of \n"
          << "            data file,if writing to file,is tracing.log.       \n"
          << "            Temporary it's cannot be changed.                  \n"
          << "          --debug - Print degug information for functions      \n"
          << "            inside interpreter.                                \n"
          << "          --help - Show this menu end exit.                    \n"
          << "[filename]       - Path to file for interpreting               \n"
         << "[End of menu]                                                  \n";
#else
     cerr << "          --help - Show this menu end exit.                    \n"
          << "[filename]       - Path to file for interpreting               \n"
         << "[End of menu]                                                  \n";
#endif // DISABLE_TRACING
  }
/* -------------------------------------------------------------------------- */
