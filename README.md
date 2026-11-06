# basin
A small QBASIC-compatible BASIC interpreter

Forked from https://sourceforge.net/projects/basin/


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

1) First run configure script in terminal(after you moved to sources folder):

   ./configure

Check what packages you may be need.Most likely you will need to install
  
SDL library (libsdl.org). It is used for graphics 

(SCREEN,PRESET,LINE statements).If your platform is 

Debian-based, simple search in repository libsdl1.2-dev packet and install it.

In Ubuntu Linux:

sudo apt-get install libsdl-dev

2) Then run:

   make
   
3) It's all!   


================================================================================
    If you planning to add functionality to interpreter,first of all,compile the

program and run the test_script.All tests must write message '** TEST PASSED **'

This is mean, that each  source  file  with  error,located in /tests/ directory,

caused to  interpreter  react  for  this error exactly as needed.Error templates

located  in /tests/ folder and called *.stdout.After  you  completed  with  your

changes in  sources  of interpreter,re-make it and run test_script. Your changes

must  not  change  reaction  of  the  program  to  errors, and  all  tests  must 

be completed  with '** TEST PASSED **'  message. If you see '** TEST FAILED **',

find why this happening and try to fix it. If you need to build new sources with

errors  to be checked, put the sources in the  /test/  folder and run tmpl_maker 

script.This script will build all needed templates from stderr.
================================================================================
