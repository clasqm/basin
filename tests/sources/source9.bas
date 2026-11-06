DIM arry(7,6),rrr
LET mystring$ = STR$(34787868);
PRINT mystring$
   'OPTION TRACE ACTION 1
   LET e = tan(2);
   PRINT e:PRINT
   IF e < 0 THEN
      PRINT "SSS"   
   ELSEIF e >= 0 THEN
    PRINT
    PRINT "AVVVVV"
   ELSE
    PRINT 
    PRINT "WWWWWW" 
   ELSEIF e<0 THEN
    PRINT 
    PRINT "ELSEIF"
    'OPTION TRACE TREE 1
   ELSE 
    PRINT "BYE"
    LET r=4  
   END IF 
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   LET B1 = 1
   ' Условное исполнение
   IF B1 THEN LET C=1: LET D=2 ELSE LET C=10: LET D=20
   PRINT C
   PRINT
   PRINT D
   PRINT

   ' Повторяем предыдущий код, но с таким условием, чтобы исполнилось ELSE
   IF B1-460 THEN 
      LET C=1 
      LET D=2 
   ELSE 
      LET C=10 
      LET D=20
   END IF
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    'Here we define and calculate array
   OPTION BASE -2
   DIM array(3,3,3),array2(-3 TO -1,-4 TO 3) 'dimensions (3,8)

   LET i = -2
   LET j = -2
   LET k = -2
   LET n = 1

   WHILE i < 1
     WHILE j < 1
      WHILE k < 1
         LET array(i,j,k) = n
         PRINT "array(",i,",",j,",",k,") is: ",array(i,j,k):PRINT
         LET k = k+1
         LET n = n+1
      WEND
         LET k = -2
	      LET j = j+1
     WEND
     LET i = i+1
     LET j = -2
   WEND   

   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   PRINT:PRINT "The variant with (a TO n,...)":PRINT
   
   LET i = -3
   LET j = -4
   LET n = 1
   
   WHILE i < 0
      WHILE j < 4
         LET array2(i,j) = n
	      PRINT "array2(",i,",",j,") is: ",array2(i,j):PRINT
         LET j = j+1
         LET n = n+1
      WEND
      LET j = -4
      LET i = i+1
   WEND
   
   PRINT 
   
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   'Тут меняем base по умолчанию на 0(было -2)
   OPTION BASE 0
   'Here we define and calculate array
   DIM array3(3,10)

   LET array3(0,9) = 2

   LET i = 0
   LET j = 0

   WHILE i < 3
     WHILE j < 10
	      LET array3(i,j) = i+j
	      PRINT "array3(",i,",",j,") is: ",array3(i,j):PRINT
	      LET j = j+1
     WEND
     LET i = i+1
     LET j = 0
   WEND

   PRINT
   
   ' Работа с многомерным массивом
   DIM AA(2,3,4)
   LET COUNT = 100
   LET i = 0
   WHILE i < 2 
     LET j = 0
     WHILE j < 3 
       LET k = 0
       WHILE k < 4 
         LET COUNT = COUNT + 1
         LET AA(i,j,k) = COUNT
         LET k = k + 1
       WEND
       LET j = j + 1
     WEND
     LET i = i + 1
   WEND
   PRINT COUNT:    PRINT     ' expected 124
   PRINT AA(1,0,1):PRINT     ' expected 114 
   IF COUNT <= 124 THEN PRINT "124!":PRINT
   IF COUNT >= 123 THEN PRINT "Yahooah!":PRINT
   
  
  ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  ' FOR loop
   PRINT
   FOR i=array3(2,9) TO  array3(0,0) STEP -1
      PRINT "ZZZZZZZZ":PRINT
         FOR j=1 TO 3 STEP 1
          PRINT "Hello,world!":PRINT
         NEXT j
   NEXT i
   
   
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   PRINT
   LET i=1
   DO 
      PRINT "IN DO LOOP"
      PRINT
      LET i=i+1
   WHILE i < 4  
   
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   'OPTION TRACE ACTION 1
   
   FOR I=1 TO 5
      FOR J=I+1 TO 5
        PRINT "in loop I=", I, " ,J=", J : PRINT 
        'OPTION TRACE ACTION 0   
      NEXT J :NEXT I
   PRINT "after loop I=", I, " ,J=", J : PRINT
   PRINT
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   'GOTO
      ' Считаем сумму чисел от 1 до 50
   LET COUNT=0
   1:
   WHILE COUNT<100
      LET COUNT=COUNT+1
      PRINT COUNT," "
      IF COUNT<50 THEN GOTO 1 ELSE GOTO 2
   WEND
   2:
   PRINT
   PRINT "END OF PROGRAM"
 
  BEEP
  
  CLS
  
  LET STR = "qwerty"
  LET LENGTH = 6
  IF LENGTH = LEN(STR) THEN PRINT:PRINT "Match"
  
  LET vv = SQR(49)
  PRINT:PRINT vv:PRINT
  
  LET vv = ABS(-49)
  PRINT:PRINT vv:PRINT
  
  LET a$="This is a sentence that is being tested"
  LET b=INSTR(a$,"a")
  PRINT b:PRINT
  
  LET b$ = LEFT$(a$, 5)
  LET c$ = RIGHT$(a$, 2)
  PRINT b$:PRINT
  PRINT c$:PRINT
  LET b$ = MID$(a$, 5, 6)
  PRINT "Original b$ before function call: ",b$:PRINT

  'CLEAR
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   
   DIM SHARED var1 AS INTEGER, var2(9,6) AS STRING  
   
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   DIM arr(7,6),arr2
   
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   DIM SHARED counter AS INTEGER
 
