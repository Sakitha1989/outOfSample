\ENCODING=ISO-8859-1
\Problem name: IloCplex

Maximize
 obj: Cut#12 + [ - 2 NA_genDual(1)(1)#0 ^2 - 2 NA_genDual(2)(1)#1 ^2
      - 2 NA_genDual(3)(1)#2 ^2 - 2 NA_demDual(1)(1)#3 ^2
      - 2 NA_genDual(1)(2)#4 ^2 - 2 NA_genDual(2)(2)#5 ^2
      - 2 NA_genDual(3)(2)#6 ^2 - 2 NA_demDual(1)(2)#7 ^2
      - 2 NA_genDual(1)(3)#8 ^2 - 2 NA_genDual(2)(3)#9 ^2
      - 2 NA_genDual(3)(3)#10 ^2 - 2 NA_demDual(1)(3)#11 ^2 ] / 2
Subject To
 _Expectation(1)#0: 0.200000002980232 NA_genDual(1)(1)#0
                    + 0.300000011920929 NA_genDual(1)(2)#4
                    + 0.5 NA_genDual(1)(3)#8  = 0
 _Expectation(2)#1: 0.200000002980232 NA_genDual(2)(1)#1
                    + 0.300000011920929 NA_genDual(2)(2)#5
                    + 0.5 NA_genDual(2)(3)#9  = 0
 _Expectation(3)#2: 0.200000002980232 NA_genDual(3)(1)#2
                    + 0.300000011920929 NA_genDual(3)(2)#6
                    + 0.5 NA_genDual(3)(3)#10  = 0
 _Expectation(4)#3: 0.200000002980232 NA_demDual(1)(1)#3
                    + 0.300000011920929 NA_demDual(1)(2)#7
                    + 0.5 NA_demDual(1)(3)#11  = 0
 Cut(0)#4:          5.00000007450581 NA_genDual(1)(1)#0
                    + 9.00000013411045 NA_genDual(2)(1)#1
                    + 6.00000008940697 NA_genDual(3)(1)#2
                    + 20.0000002980232 NA_demDual(1)(1)#3
                    + 7.50000029802322 NA_genDual(1)(2)#4
                    + 15.0000005960464 NA_genDual(2)(2)#5
                    + 7.50000029802322 NA_genDual(3)(2)#6
                    + 30.0000011920929 NA_demDual(1)(2)#7
                    + 12.5 NA_genDual(1)(3)#8 + 30 NA_genDual(2)(3)#9
                    + 7.5 NA_genDual(3)(3)#10 + 50 NA_demDual(1)(3)#11 + Cut#12
                    <= 99276.0014779121
 Cut(1)#5:          10.0000001490116 NA_genDual(1)(1)#0
                    + 18.0000002682209 NA_genDual(2)(1)#1
                    + 12.0000001788139 NA_genDual(3)(1)#2
                    + 40.0000005960464 NA_demDual(1)(1)#3
                    + 15.0000005960464 NA_genDual(1)(2)#4
                    + 30.0000011920929 NA_genDual(2)(2)#5
                    + 15.0000005960464 NA_genDual(3)(2)#6
                    + 60.0000023841858 NA_demDual(1)(2)#7
                    + 25 NA_genDual(1)(3)#8 + 60 NA_genDual(2)(3)#9
                    + 15 NA_genDual(3)(3)#10 + 100 NA_demDual(1)(3)#11 + Cut#12
                    <= 99276.0014779121
Bounds
      NA_genDual(1)(1)#0 Free
      NA_genDual(2)(1)#1 Free
      NA_genDual(3)(1)#2 Free
      NA_demDual(1)(1)#3 Free
      NA_genDual(1)(2)#4 Free
      NA_genDual(2)(2)#5 Free
      NA_genDual(3)(2)#6 Free
      NA_demDual(1)(2)#7 Free
      NA_genDual(1)(3)#8 Free
      NA_genDual(2)(3)#9 Free
      NA_genDual(3)(3)#10 Free
      NA_demDual(1)(3)#11 Free
      Cut#12 Free
End
