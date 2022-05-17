\ENCODING=ISO-8859-1
\Problem name: IloCplex

Maximize
 obj: Cut#27 + [ - 2 naDual(0)(1)#0 ^2 - 2 naDual(1)(2)#1 ^2
      - 2 naDual(2)(3)#2 ^2 - 2 naDual(0)(2)#3 ^2 - 2 naDual(0)(3)#4 ^2
      - 2 naDual(1)(1)#5 ^2 - 2 naDual(1)(3)#6 ^2 - 2 naDual(2)(1)#7 ^2
      - 2 naDual(2)(2)#8 ^2 - 2 naDual(3)(1)#9 ^2 - 2 naDual(3)(2)#10 ^2
      - 2 naDual(3)(3)#11 ^2 - 2 naDual(4)(1)#12 ^2 - 2 naDual(4)(2)#13 ^2
      - 2 naDual(4)(3)#14 ^2 - 2 naDual(5)(1)#15 ^2 - 2 naDual(5)(2)#16 ^2
      - 2 naDual(5)(3)#17 ^2 - 2 naDual(6)(1)#18 ^2 - 2 naDual(6)(2)#19 ^2
      - 2 naDual(6)(3)#20 ^2 - 2 naDual(7)(1)#21 ^2 - 2 naDual(7)(2)#22 ^2
      - 2 naDual(7)(3)#23 ^2 - 2 naDual(8)(1)#24 ^2 - 2 naDual(8)(2)#25 ^2
      - 2 naDual(8)(3)#26 ^2 ] / 2
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
 _Expectation(4)#4: 0.200000002980232 naDual(4)(1)#12
                    + 0.300000011920929 naDual(4)(2)#13 + 0.5 naDual(4)(3)#14
                     = 0
 _Expectation(5)#5: 0.200000002980232 naDual(5)(1)#15
                    + 0.300000011920929 naDual(5)(2)#16 + 0.5 naDual(5)(3)#17
                     = 0
 _Expectation(6)#6: 0.200000002980232 naDual(6)(1)#18
                    + 0.300000011920929 naDual(6)(2)#19 + 0.5 naDual(6)(3)#20
                     = 0
 _Expectation(7)#7: 0.200000002980232 naDual(7)(1)#21
                    + 0.300000011920929 naDual(7)(2)#22 + 0.5 naDual(7)(3)#23
                     = 0
 _Expectation(8)#8: 0.200000002980232 naDual(8)(1)#24
                    + 0.300000011920929 naDual(8)(2)#25 + 0.5 naDual(8)(3)#26
                     = 0
 Cut(0)#9:          5.00000007450581 naDual(0)(1)#0
                    + 15.0000005960464 naDual(1)(2)#1 + 7.5 naDual(2)(3)#2
                    + 7.50000029802322 naDual(0)(2)#3 + 12.5 naDual(0)(3)#4
                    + 9.00000013411045 naDual(1)(1)#5 + 30 naDual(1)(3)#6
                    + 6.00000008940697 naDual(2)(1)#7
                    + 7.50000029802322 naDual(2)(2)#8
                    + 20.0000002980232 naDual(3)(1)#9
                    + 30.0000011920929 naDual(3)(2)#10 + 50 naDual(3)(3)#11
                    + 5.00000007450581 naDual(4)(1)#12
                    + 7.50000029802322 naDual(4)(2)#13 + 12.5 naDual(4)(3)#14
                    + 6.00000008940697 naDual(5)(1)#15
                    + 7.50000029802322 naDual(5)(2)#16 + 7.5 naDual(5)(3)#17
                    - 0.0500000007450581 naDual(7)(1)#21
                    - 0.0750000029802322 naDual(7)(2)#22
                    - 0.125 naDual(7)(3)#23
                    + 0.0100000001490116 naDual(8)(1)#24 - 0.05 naDual(8)(3)#26
                    + Cut#27 <= -99276.0014779121
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
      naDual(4)(1)#12 Free
      naDual(4)(2)#13 Free
      naDual(4)(3)#14 Free
      naDual(5)(1)#15 Free
      naDual(5)(2)#16 Free
      naDual(5)(3)#17 Free
      naDual(6)(1)#18 Free
      naDual(6)(2)#19 Free
      naDual(6)(3)#20 Free
      naDual(7)(1)#21 Free
      naDual(7)(2)#22 Free
      naDual(7)(3)#23 Free
      naDual(8)(1)#24 Free
      naDual(8)(2)#25 Free
      naDual(8)(3)#26 Free
-999999 <= Cut#27 <= 0
End
