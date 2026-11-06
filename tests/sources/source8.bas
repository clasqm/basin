 DIM a(10), b(10)
 FOR i = 1 TO 10
 INPUT "Введите очередной элемент ", a(i)
 NEXT i
 LET min = a(1)
 LET nmin = 1
 LET max = a(1)
 LET nmax = 1
 FOR i = 1 TO 10
 IF a(i) < min THEN LET min = a(i): LET nmin = i
 IF a(i) > max THEN LET max = a(i): LET nmax = i
 IF a(i) = 0 THEN LET k = k + 1: LET b(k) = i
 NEXT i
 IF k = 0 THEN PRINT "Нулевых элементов нет " else print "Номера нулевых элементов  ": FOR i = 1 TO k: PRINT b(i): NEXT i
 PRINT "Номер максимального элемента:"; nmax
 PRINT "Номер минимального элемента:"; nmin
 PRINT "Разница между max и min="; max - min
