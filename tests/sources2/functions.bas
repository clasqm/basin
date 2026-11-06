SCREEN 12
LET c = 2
LINE (0, 240)-(640, 240), c
 LINE (320, 0)-(320, 480), c
LINE (320, 0)-(318, 20), c
 LINE (320, 0)-(322, 20), c
LINE (640, 240)-(620, 238), c
 LINE (640, 240)-(620, 242), c


LET mx = 40
LET  my = 40
LET  c = 4
DIM x AS DOUBLE,y AS DOUBLE

FOR x=40 TO 640 STEP 40
    LINE (x, 238)-(x, 242),0
NEXT x
FOR y = 40 TO 480 STEP 40
    LINE (318, y)-(322, y),0
NEXT y
FOR x = -8 TO 8 STEP 0.01
    LET y = x * x
    PRESET (320 + x * mx, 240 - y * mx), c
NEXT x
FOR x = 0 TO 8 STEP 0.01
    LET y = SQR(x)
    PRESET (320 + x * mx, 240 - y * mx), c + 2
NEXT x
FOR x = -8 TO 8 STEP 0.001
    LET y = 1 / x
    PRESET (320 + x * mx, 240 - y * mx), c - 1
NEXT x
FOR x = -8 TO 8 STEP 0.001
    LET y = x
    PRESET (320 + x * mx, 240 - y * mx), c + 5
NEXT x
