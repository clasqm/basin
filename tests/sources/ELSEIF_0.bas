' Тест на разные формы появления ELSEIF
' Каждый тест повторяем по нескольку раз, чтобы проверить разные ветки исполнения
 
PRINT "----- IF Cond THEN ----------------------------------------------------"
PRINT "        Stmt"
PRINT "      ELSE IF Cond THEN"
PRINT "      END IF"
 
LET A=1
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
END IF
PRINT B ' expected 100
 
LET A=2
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
END IF
PRINT B ' expected -1
 
 
 
PRINT "----- IF Cond THEN ----------------------------------------------------"
PRINT "        Stmt"
PRINT "      ELSE IF Cond THEN"
PRINT "        Stmt"
PRINT "      END IF"
 
LET A=1
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
END IF
PRINT B ' expected 100
 
LET A=2
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
END IF
PRINT B ' expected 200
 
LET A=3
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
END IF
PRINT B ' expected -1
 
 
 
PRINT "----- IF Cond THEN ----------------------------------------------------"
PRINT "        Stmt"
PRINT "      ELSE IF Cond THEN"
PRINT "        Stmt"
PRINT "      ELSE"
PRINT "      END IF"
 
LET A=1
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
ELSE
END IF
PRINT B ' expected 100
 
LET A=2
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
ELSE
END IF
PRINT B ' expected 200
 
LET A=3
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
ELSE
END IF
PRINT B ' expected -1
 
 
 
PRINT "----- IF Cond THEN ----------------------------------------------------"
PRINT "        Stmt"
PRINT "      ELSE IF Cond THEN"
PRINT "        Stmt"
PRINT "      ELSE"
PRINT "        Stmt"
PRINT "      END IF"
 
LET A=1
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
ELSE
  LET B=300
END IF
PRINT B ' expected 100
 
LET A=2
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
ELSE
  LET B=300
END IF
PRINT B ' expected 200
 
LET A=3
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
ELSE
  LET B=300
END IF
PRINT B ' expected 300
 
 
 
PRINT "----- IF Cond THEN ----------------------------------------------------"
PRINT "        Stmt"
PRINT "      ELSE IF Cond THEN"
PRINT "        Stmt"
PRINT "      ELSE IF Cond THEN"
PRINT "        Stmt"
PRINT "      ELSE"
PRINT "        Stmt"
PRINT "      END IF"
 
LET A=1
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
ELSEIF A=3 THEN
  LET B=300
ELSE
  LET B=400
END IF
PRINT B ' expected 100
 
LET A=2
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
ELSEIF A=3 THEN
  LET B=300
ELSE
  LET B=400
END IF
PRINT B ' expected 200
 
LET A=3
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
ELSEIF A=3 THEN
  LET B=300
ELSE
  LET B=400
END IF
PRINT B ' expected 300
 
LET A=4
LET B=-1
IF A=1 THEN
  LET B=100
ELSEIF A=2 THEN
  LET B=200
ELSEIF A=3 THEN
  LET B=300
ELSE
  LET B=400
END IF
PRINT B ' expected 400
