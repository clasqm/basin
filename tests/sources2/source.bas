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
   SCREEN 12  

LET WIDTH = 639
LET HEIGHT = 479
LET ST = 80
LET COLOR1 = 3
LET COLOR2 = 14
LET COLOR3 = 9
LET COLOR4 = 10
LET COLOR5 = 12
LET COLOR6 = 4
FOR XS=0 TO WIDTH STEP 80 ' 80 = ST
  FOR YS=0 TO HEIGHT STEP 80 ' 80 = ST
    LET XF=XS+ST
    LET YF=YS+ST
    LET S=2
    LINE (XS+S, YS+S) - (XF-S, YF-S), COLOR1, BF
    LET S=10
    LET SS=20
    LINE (XS+S, YS+S) - (XS+S+SS, YS+S+SS), COLOR2, BF
    LINE (XS+S, YF-S) - (XS+S+SS, YF-S-SS), COLOR2, BF
    LINE (XF-S, YS+S) - (XF-S-SS, YS+S+SS), COLOR2, BF
    LINE (XF-S, YF-S) - (XF-S-SS, YF-S-SS), COLOR2, BF
    LET S=16
    LET SS=5
    FOR I=0 TO SS
      LINE (XS+S+I, YS+S+I) - (XF-S-I, YF-S-I), COLOR3, B
    NEXT I
    LET S=23
    LET SS=3
    LET I=0
    FOR I=0 TO SS
      LINE (XS+S+I, YS+S+I) - (XF-S-I, YF-S-I), COLOR4, B
    NEXT I
    LET S=3
    LINE (XS+S, YS+S) - (XF-S, YF-S), COLOR5, B
    LET S=35
    LINE (XS+S, YS+S) - (XF-S, YF-S), COLOR6, BF
  NEXT YS
NEXT XS
FOR FI=0.0 TO 639
  LET X1 = FI
  LET Y1 = 240
  LET X2 = 240 + 150.0 * SIN (FI/60)
  LET Y2 = 240 + 150.0 * COS (FI/60)
  LINE (X1,Y1) - (X2,Y2), 15
NEXT FI
'   FOR i=17 TO -1 STEP -1
'     FOR m=70 TO 350 STEP 40
'       FOR j=70 TO 350 STEP 40
'         CIRCLE (m+i,j+i),60,i
'       NEXT j
'     NEXT m          
'   NEXT i
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
 '  FOR i=5 TO 639 STEP 20
 '     FOR j=1 TO 479
 '       PRESET (i,j),3
 '      NEXT j
 '  NEXT i 
 '  FOR i=5 TO 479 STEP 20
 '     FOR j=1 TO 639
 '       PRESET (j,i),3
 '    NEXT j
 '  NEXT i
  ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
 '  LET k=1
 '  FOR i=20 TO 420 STEP 10
 '     FOR j=20 TO 420 STEP 10
 '        LINE (i,j)-(j,i+k),14
 '        LET k=k+1
 '     NEXT j
 '     LET k=0
 '  NEXT i
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
 '  LET k=0
 '  FOR i=0 TO 15
 '     FOR j=20 TO 420 STEP 40
 '      LINE (425+k,j)-(445+k,j+20),i,BF
 '     NEXT j
 '      LET k=k+13       
 '  NEXT i
' LINE (25,445)-(45,465),12,B   '(box)
 ' LINE (65,445)-(85,465),10,B  '(box)
 
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
  
  LET var2(1,1) = "string!"
  PRINT "var2(1,1) before function call (SHARED): ",var2(1,1):PRINT
  
  LET var1 = 1
  PRINT "var1 before function call (SHARED): ",var1:PRINT
  
  LET somevar = 0
  PRINT "somevar before function call: ",somevar:PRINT
  
   
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   DIM arr(7,6),arr2
   
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   DIM SHARED counter AS INTEGER
   LET counter = 0
   
   SUB myfunc 
   
      PRINT
      
      LET b$ = "aaa"
      PRINT "b$ inside function call: ", b$:PRINT
      
      LET var2(1,1) = "STRING!" 
      PRINT "var2(1,1) inside function call (SHARED): ",var2(1,1):PRINT
      
      LET var1 = 3
      PRINT "var1 inside function call (SHARED): ",var1:PRINT
      
      LET somevar = 10
      PRINT "somevar inside function call: ",somevar:PRINT
      
      PRINT
      
      LET counter = counter + 1
      IF counter < 10 THEN
       CALL myfunc
      END IF 
      
   END SUB
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   CALL myfunc 
   '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
   PRINT "b$ after funtion call: ",b$:PRINT
   PRINT "var2(1,1) after function call (SHARED): ",var2(1,1):PRINT
   PRINT "var1 after function call (SHARED): ",var1:PRINT
   PRINT "somevar after function call: ",somevar:PRINT  
