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
