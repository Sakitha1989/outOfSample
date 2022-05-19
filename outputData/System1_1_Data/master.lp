\ENCODING=ISO-8859-1
\Problem name: IloCplex

Maximize
 obj: Cut#27 + [ - 2 naDual(0)(0)#0 ^2 - 2 naDual(0)(1)#1 ^2
      - 2 naDual(0)(2)#2 ^2 - 2 naDual(0)(3)#3 ^2 - 2 naDual(0)(4)#4 ^2
      - 2 naDual(0)(5)#5 ^2 - 2 naDual(0)(6)#6 ^2 - 2 naDual(0)(7)#7 ^2
      - 2 naDual(0)(8)#8 ^2 - 2 naDual(1)(0)#9 ^2 - 2 naDual(1)(1)#10 ^2
      - 2 naDual(1)(2)#11 ^2 - 2 naDual(1)(3)#12 ^2 - 2 naDual(1)(4)#13 ^2
      - 2 naDual(1)(5)#14 ^2 - 2 naDual(1)(6)#15 ^2 - 2 naDual(1)(7)#16 ^2
      - 2 naDual(1)(8)#17 ^2 - 2 naDual(2)(0)#18 ^2 - 2 naDual(2)(1)#19 ^2
      - 2 naDual(2)(2)#20 ^2 - 2 naDual(2)(3)#21 ^2 - 2 naDual(2)(4)#22 ^2
      - 2 naDual(2)(5)#23 ^2 - 2 naDual(2)(6)#24 ^2 - 2 naDual(2)(7)#25 ^2
      - 2 naDual(2)(8)#26 ^2 ] / 2
Subject To
 _Expectation(0)#0: 0.200000002980232 naDual(0)(0)#0
                    + 0.300000011920929 naDual(1)(0)#9 + 0.5 naDual(2)(0)#18
                     = 0
 _Expectation(1)#1: 0.200000002980232 naDual(0)(1)#1
                    + 0.300000011920929 naDual(1)(1)#10 + 0.5 naDual(2)(1)#19
                     = 0
 _Expectation(2)#2: 0.200000002980232 naDual(0)(2)#2
                    + 0.300000011920929 naDual(1)(2)#11 + 0.5 naDual(2)(2)#20
                     = 0
 _Expectation(3)#3: 0.200000002980232 naDual(0)(3)#3
                    + 0.300000011920929 naDual(1)(3)#12 + 0.5 naDual(2)(3)#21
                     = 0
 _Expectation(4)#4: 0.200000002980232 naDual(0)(4)#4
                    + 0.300000011920929 naDual(1)(4)#13 + 0.5 naDual(2)(4)#22
                     = 0
 _Expectation(5)#5: 0.200000002980232 naDual(0)(5)#5
                    + 0.300000011920929 naDual(1)(5)#14 + 0.5 naDual(2)(5)#23
                     = 0
 _Expectation(6)#6: 0.200000002980232 naDual(0)(6)#6
                    + 0.300000011920929 naDual(1)(6)#15 + 0.5 naDual(2)(6)#24
                     = 0
 _Expectation(7)#7: 0.200000002980232 naDual(0)(7)#7
                    + 0.300000011920929 naDual(1)(7)#16 + 0.5 naDual(2)(7)#25
                     = 0
 _Expectation(8)#8: 0.200000002980232 naDual(0)(8)#8
                    + 0.300000011920929 naDual(1)(8)#17 + 0.5 naDual(2)(8)#26
                     = 0
Bounds
      naDual(0)(0)#0 Free
      naDual(0)(1)#1 Free
      naDual(0)(2)#2 Free
      naDual(0)(3)#3 Free
      naDual(0)(4)#4 Free
      naDual(0)(5)#5 Free
      naDual(0)(6)#6 Free
      naDual(0)(7)#7 Free
      naDual(0)(8)#8 Free
      naDual(1)(0)#9 Free
      naDual(1)(1)#10 Free
      naDual(1)(2)#11 Free
      naDual(1)(3)#12 Free
      naDual(1)(4)#13 Free
      naDual(1)(5)#14 Free
      naDual(1)(6)#15 Free
      naDual(1)(7)#16 Free
      naDual(1)(8)#17 Free
      naDual(2)(0)#18 Free
      naDual(2)(1)#19 Free
      naDual(2)(2)#20 Free
      naDual(2)(3)#21 Free
      naDual(2)(4)#22 Free
      naDual(2)(5)#23 Free
      naDual(2)(6)#24 Free
      naDual(2)(7)#25 Free
      naDual(2)(8)#26 Free
-999999 <= Cut#27 <= 0
End
