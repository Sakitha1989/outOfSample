\ENCODING=ISO-8859-1
\Problem name: IloCplex

Maximize
 obj: Cut#12 + [ - 2 naDual(0)(1)#0 ^2 - 2 naDual(1)(2)#1 ^2
      - 2 naDual(2)(3)#2 ^2 - 2 naDual(0)(2)#3 ^2 - 2 naDual(0)(3)#4 ^2
      - 2 naDual(1)(1)#5 ^2 - 2 naDual(1)(3)#6 ^2 - 2 naDual(2)(1)#7 ^2
      - 2 naDual(2)(2)#8 ^2 - 2 naDual(3)(1)#9 ^2 - 2 naDual(3)(2)#10 ^2
      - 2 naDual(3)(3)#11 ^2 ] / 2
Subject To
 _Expectation(0)#0: 0.200000002980232 naDual(0)(1)#0
                    + 0.300000011920929 naDual(0)(2)#3 + 0.5 naDual(0)(3)#4
                     = 0
 _Expectation(1)#1: 0.300000011920929 naDual(1)(2)#1
                    + 0.200000002980232 naDual(1)(1)#5 + 0.5 naDual(1)(3)#6
                     = 0
 _Expectation(2)#2: 0.5 naDual(2)(3)#2 + 0.200000002980232 naDual(2)(1)#7
                    + 0.300000011920929 naDual(2)(2)#8  = 0
 _Expectation(3)#3: 0.200000002980232 naDual(3)(1)#9
                    + 0.300000011920929 naDual(3)(2)#10 + 0.5 naDual(3)(3)#11
                     = 0
Bounds
      naDual(0)(1)#0 Free
      naDual(1)(2)#1 Free
      naDual(2)(3)#2 Free
      naDual(0)(2)#3 Free
      naDual(0)(3)#4 Free
      naDual(1)(1)#5 Free
      naDual(1)(3)#6 Free
      naDual(2)(1)#7 Free
      naDual(2)(2)#8 Free
      naDual(3)(1)#9 Free
      naDual(3)(2)#10 Free
      naDual(3)(3)#11 Free
      Cut#12 Free
End
