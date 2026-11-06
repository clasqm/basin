SCREEN 12

FOR m = 1 TO 640
   FOR n = 1 TO 480
      LET p = (m - 320) / 140.0
      LET q = (n - 240) / 140.0
      LET x = 0.0
      LET y = 0.0
      FOR i = 1 TO 90
         LET u = x * x - y * y + p
         LET v = 2 * x * y + q 
         IF x*x > 4 THEN
            LET color = i/15
            PRESET (m, n), color
            GOTO 1
         END IF
         LET x = u
         LET y = v
      NEXT i
      PRESET (m, n), color/m
      1:
   NEXT n
NEXT m

