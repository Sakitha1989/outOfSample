\ENCODING=ISO-8859-1
\Problem name: IloCplex

Maximize
 obj: 0 NA_genDual(1)#0 + Cut#4 + [ - 2 NA_genDual(1)#0 ^2
      - 2 NA_genDual(2)#1 ^2 - 2 NA_genDual(3)#2 ^2 - 2 NA_demDual(1)#3 ^2
      ] / 2
Subject To
 _Expectation(4)#0: 1.00000001490116 NA_demDual(1)#3  = 0
 _Expectation(2)#1: 1.00000001490116 NA_genDual(2)#1  = 0
 _Expectation(3)#2: 1.00000001490116 NA_genDual(3)#2  = 0
Bounds
      NA_genDual(1)#0 >= 2
      NA_genDual(2)#1 >= 2
      NA_genDual(3)#2 >= 2
      NA_demDual(1)#3 >= 2
      Cut#4 >= 2
End
