SCREEN 12
LET r1 = 111:LET r2 = 16:LET r3 = 131:LET k = 0.85
FOR t = 0 TO 600 STEP 0.01
    LET x = (r1 + r2) * COS(r2 * t / r1) - r3 * COS((r1 + r2) / r1 * t)
    LET y = (r1 + r2) * SIN(r2 * t / r1) - r3 * SIN((r1 + r2) / r1 * t)
    PRESET (x * k + 320, y * k + 240), 5
NEXT t
