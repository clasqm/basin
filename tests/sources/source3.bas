' Простая арифметика
LET A1 = 10
LET A2 = 100
LET B1 = A1 + 5 * A2 - 50
PRINT B1 ' expected 460
 
' Неявные преобразования типов делать пока не умеем
'LET A1 = 1 + 2.7
'PRINT A1 ' expected 3
'LET A1 = 2.7 + 1
'PRINT A1 ' expected 3.7
 
' Ситуация, когда по тексту переменная сначала появляется в правой части
' присваивания, а потом влевой (т.е. типа якобы сначала использование,
' а потом запись). Однако по управлению (в run-time) всё нормально:
' сначала идёт запись, и только потом чтение
' Заодно и проверка работы IF'а
LET C = 0
WHILE C < 2 
  IF C 
    LET B = SHAMAN + 1 ' сюда попадаем только на второй итерации цикла
  ELSE
    LET SHAMAN = 999 ' а сюда - на первой
  END IF
  LET C = C + 1
WEND
PRINT SHAMAN ' expected 999
PRINT B      ' expected 1000
 
' Считаем сумму чисел от 1 до 100
LET COUNT=0
LET SUM=0
WHILE COUNT < 100
  LET COUNT=COUNT+1
  LET SUM=SUM+COUNT
WEND
PRINT COUNT ' expected 100
PRINT SUM   ' expected 5050
