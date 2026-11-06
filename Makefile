# basin ,simple QBASIC interpreter.
# Copyright (C) 2009-2011  #pragma, Evg

# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation,  either  version  3  of the License, or(at your option) any later
# version.

# This  program  is  distributed  in  the  hope  that  it  will  be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
# details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#
# basin makefile
#
#

# Принятые обозначения: CC - компилятор Си, CXX - компилятор Си++
CXX = g++
CXXFLAGS = -ansi -pedantic -pedantic-errors -Wall -Wextra -O2 -Os
DFLAGS = -MD
LD = $(CXX)
LDFLAGS = -lSDL
OFILES = Interpreter/debug/error_handler.o \
         Interpreter/main.o \
         Interpreter/lib/lib.o \
         Interpreter/cmd/options.o \
         Interpreter/memory/pool.o \
         \
         Interpreter/frontend/parser.o  \
         Interpreter/frontend/syntax.o \
         \
         Interpreter/intermediate/value.o \
         Interpreter/intermediate/variable.o \
         Interpreter/intermediate/expression.o \
         Interpreter/intermediate/statement.o \
         Interpreter/intermediate/array.o \
         Interpreter/intermediate/label.o \
         \
         Interpreter/runtime/interp.o \
         Interpreter/runtime/function.o \
         Interpreter/runtime/graphics/graphics.o \
         Interpreter/runtime/tracer/tracer.o 

CXXFLAGS += -I. -IInterpreter/runtime -IInterpreter/runtime/graphics  \
-IInterpreter/runtime/tracer -IInterpreter/cmd -IInterpreter/config   \
-IInterpreter/debug -IInterpreter/frontend -IInterpreter/intermediate \
-IInterpreter/lib -IInterpreter/include -IInterpreter/memory          \
-IInterpreter

RM         = rm -f
TARGET     = basin
SUBTARGET  = Basin-IDE
INSTALLDIR = ./bin
MV         = /bin/mv
LS         = /bin/ls
CURRDIR    = `pwd`
OPT        = *.h *.cpp */*.h */*.cpp
LSFLAGS    = -R $(OPT)
PROGDIR    = ./Interpreter
GUIDIR     = ./GUI/Qt/Basin-IDE
ifeq ($(!$(test -e /usr/bin/qmake)),$(test -d /usr/bin/))
MAKE       = /usr/bin/make
QMAKE      = /usr/bin/qmake
QMKFLAGS   = -project
endif

# Костыль i don't know even this working or not
ifeq ($(!$(test -e /usr/bin/qmake)),$(test -d /usr/bin/))

# По умолчанию команда "make" собирает самую первую цель.
# Обычно её обзывают "all" и уже расписывают, что там должно быть
all: $(TARGET)  $(GUIDIR)/$(SUBTARGET) 
else

all: $(TARGET)

endif
# Линковка из объектников
ifeq ($(!$(test -e /usr/bin/qmake)),$(test -d /usr/bin/))

$(TARGET): $(OFILES)
	$(LD) $(LDFLAGS) $(OFILES) -o $(TARGET) \
	&& $(MV) ./$(TARGET) $(INSTALLDIR)/$(TARGET)

$(GUIDIR)/$(SUBTARGET):
	 cd $(GUIDIR) && $(QMAKE) $(QMKFLAGS) $($(LS) $(LSFLAGS)) \
	 && $(QMAKE) && $(MAKE) && cd .. && cd .. && cd .. \
	 && $(MV) $(GUIDIR)/$(SUBTARGET) $(INSTALLDIR)/$(SUBTARGET)	
else
$(TARGET): $(OFILES)
	$(LD) $(LDFLAGS) $(OFILES) -o $(TARGET) \
	&& $(MV) ./$(TARGET) $(INSTALLDIR)/$(TARGET)
endif	 
# Подключаем файлы зависимостей
# Заумное выражение справа означает взять переменную OFILES и заменить ".o"
# на ".d"  Т.е. здесь по сути написано "-include debugger.d error_handler.d ..."
# Директива include работает точно так же, как и препроцессорная #include
-include $(OFILES:.o=.d)

# Общее правило формирования объектника. $< означает левую часть зависимости,
# $@ - правую
%.o : %.cpp
	$(CXX) -c $(DFLAGS) $(CXXFLAGS) $< -o $@

clean:
	$(RM) $(GUIDIR)/Makefile */*.pro *.o *.d *.bak *~*  *.log *.status \
   */*.o */*.d */*.bak *.dat */moc_* */qrc_* */qrc_* */*.dat    \
	*/*/*/*.o */*/*/*.d */*/*/*.bak */*/*/moc_* */*/*/qrc_* */*/*/*.dat \
	*/*/*/*.log */*/*/*.pro */*/*.o */*/*.d
distclean:
	$(RM) $(INSTALLDIR)/$(TARGET) \
	$(INSTALLDIR)/$(SUBTARGET)   \
	$(GUIDIR)/Makefile */*.pro *.o *.d *.bak *~*  *.log *.status \
   */*.o */*.d */*.bak *.dat */moc_* */qrc_* */qrc_* */*.dat    \
	*/*/*/*.o */*/*/*.d */*/*/*.bak */*/*/moc_* */*/*/qrc_* */*/*/*.dat \
	*/*/*/*.log */*/*/*.pro */*/*.o */*/*.d
