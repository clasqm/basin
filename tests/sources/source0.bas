' Переход назад
    LET A=1
10: IF A<3 THEN LET A=A+1: GOTO 10
    PRINT A: PRINT ' expected 3
 
' Переход вперёд
    LET A=1
    WHILE A<10
      IF A>3 THEN GOTO 20
      LET A=A+1
    WEND
20: PRINT A: PRINT ' expected 4
 
' Несколько переходов вперёд на одну метку (30)
    LET A=1
    LET B=0
    WHILE A<10
      LET A=A+1
      IF A<3 THEN GOTO 30
      IF A>8 THEN GOTO 30
      GOTO 40
30:   LET B=B+A
40: WEND
    PRINT B: PRINT ' expected 21
error
