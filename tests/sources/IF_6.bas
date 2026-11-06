
' Тест на разные формы записи оператора IF
' Каждый тест повторяем по два раза, чтобы проверить обе ветки исполнения
 
 
 
PRINT "----- IF Cond THEN Stmt -----------------------------------------------":PRINT
 
LET A=1
LET B=-1
IF A=1 THEN LET B=100
PRINT B:PRINT ' expected 100
 
LET A=2
LET B=-1
IF A=1 THEN LET B=100
PRINT B:PRINT ' expected -1
 
 
 
PRINT "----- IF Cond THEN Stmt:Stmt ------------------------------------------":PRINT
 
LET A=1
LET B=-1 : LET C=-1
IF A=1 THEN LET B=100 : LET C=101
PRINT B:PRINT ' expected 100
PRINT C:PRINT ' expected 101
 
LET A=2
LET B=-1 : LET C=-1
IF A=1 THEN LET B=100 : LET C=101
PRINT B:PRINT ' expected -1
PRINT C:PRINT ' expected -1
 
 
 
PRINT "----- IF Cond THEN Stmt ELSE Stmt -------------------------------------":PRINT
 
LET A=1
IF A=1 THEN LET B=100 ELSE LET B=200
PRINT B:PRINT ' expected 100
 
LET A=1
IF A=1 THEN LET B=100 ELSE LET B=200
PRINT B:PRINT ' expected 200
 
 
 
PRINT "----- IF Cond THEN Stmt:Stmt ELSE Stmt:Stmt ---------------------------":PRINT
 
LET A=1
IF A=1 THEN LET B=100 : LET C=101 ELSE LET B=200 : LET C=201
PRINT B:PRINT ' expected 100
PRINT C:PRINT ' expected 101
 
LET A=2
IF A=1 THEN LET B=100 : LET C=101 ELSE LET B=200 : LET C=201
PRINT B:PRINT ' expected 200
PRINT C:PRINT ' expected 201
 
 
 
PRINT "----- IF Cond THEN ----------------------------------------------------":PRINT
PRINT "        Stmt":PRINT
PRINT "      END IF":PRINT
 
LET A=1
LET B=-1
IF A=1 THEN
  LET B=100
END IF
PRINT B:PRINT ' expected 100
 
LET A=2
LET B=-1
IF A=1 THEN
  LET B=100
END IF
PRINT B:PRINT ' expected -1
 
 
 
PRINT "----- IF Cond THEN ----------------------------------------------------":PRINT
PRINT "      END IF":PRINT
 
LET A=1
LET B=-1
IF A=1 THEN
END IF
PRINT B:PRINT ' expected -1
 
 
 
PRINT "----- IF Cond THEN ----------------------------------------------------":PRINT
PRINT "        Stmt":PRINT
PRINT "      ELSE":PRINT
PRINT "        Stmt":PRINT
PRINT "      END IF":PRINT
 
LET A=1
IF A=1 THEN
  LET B=100
ELSE
  LET B=200
END IF
PRINT B:PRINT ' expected 100
 
LET A=2
IF A=1 THEN
  LET B=100
ELSE
  LET B=200
END IF
PRINT B :PRINT ' expected 200
 
 
 
PRINT "----- IF Cond THEN ----------------------------------------------------":PRINT
PRINT "        Stmt":PRINT
PRINT "      ELSE":PRINT
PRINT "      END IF":PRINT
 
LET A=1
LET B=-1
IF A=1 THEN
  LET B=100
ELSE
END IF
PRINT B:PRINT ' expected 100
 
LET A=2
LET B=-1
IF A=1 THEN
  LET B=100
ELSE
END IF
PRINT B:PRINT ' expected -1
 
 
 
PRINT "----- IF Cond THEN ----------------------------------------------------":PRINT
PRINT "      ELSE":PRINT
PRINT "        Stmt":PRINT
PRINT "      END IF":PRINT
 
LET A=1
LET B=-1
IF A=1 THEN
ELSE
  LET B=200
END IF
PRINT B:PRINT ' expected -1
 
LET A=2
LET B=-1
IF A=1 THEN
ELSE
  LET B=200
END IF
PRINT B:PRINT ' expected 200
 
 
 
PRINT "----- IF Cond THEN ----------------------------------------------------":PRINT
PRINT "      ELSE":PRINT
PRINT "      END IF":PRINT
 
LET A=1
LET B=-1
IF A=1 THEN
ELSE
END IF
PRINT B:PRINT ' expected -1
 
LET A=2
LET B=-1
IF A=1 THEN
ELSE
END IF
PRINT B:PRINT ' expected -1
 
 
 
' ----- оЕЗБФЙЧОЩЕ ФЕУФЩ ------------------------------------------------------
 
' IF A=1
 
' IF A=1 THEN
 
' IF A=1 THEN LET A=1 ELSE
 
' IF A=1 THEN ELSE LET A=1
 
' IF A=1 THEN
'   LET A=1
 
' IF A=1 THEN
'   LET A=1
' END
 
' IF A=1 THEN
'   LET A=1 ELSE LET A=2
 
' IF A=1 THEN LET A=1
' ELSE LET A=2
 
' IF A=1 THEN LET A=1 ELSE LET A=2 END IF 
