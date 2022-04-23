\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj: 10 RTgen(1)#9 + RTgen(2)#10 + 20 RTgen(3)#11 - 1000 RTdem(1)#12
      + 1.10000002384186 RTetaGenP(1)#18 - RTetaGenM(1)#19
      + 0.109999999403954 RTetaGenP(2)#20 - 0.100000001490116 RTetaGenM(2)#21
      + 2.09999990463257 RTetaGenP(3)#22 - 2 RTetaGenM(3)#23
      + 0.00100000004749745 RTetaDemP(1)#24
      - 0.0000999999974737875 RTetaDemM(1)#25
Subject To
 DAflowBalance(1)#0:        DAgen(1)#0 - DAflow(1,2)#4  = 0
 DAflowBalance(2)#1:        DAgen(2)#1 - DAdem(1)#3 + DAflow(1,2)#4
                            + DAflow(3,2)#5  = 0
 DAflowBalance(3)#2:        DAgen(3)#2 - DAflow(3,2)#5  = 0
 DAdcApproximation(1,2)#3:  DAflow(1,2)#4 - 100 DAtheta(1)#6 + 100 DAtheta(2)#7
                             = 0
 DAdcApproximation(3,2)#4:  DAflow(3,2)#5 + 100 DAtheta(2)#7 - 100 DAtheta(3)#8
                             = 0
 DArefAngle(1)#5:           DAtheta(1)#6  = 0
 RTflowBalance(1)#6:        - DAgen(1)#0 + DAflow(1,2)#4 + RTgen(1)#9
                            - RTflow(1,2)#13  = 0
 RTflowBalance(2)#7:        - DAgen(2)#1 + DAdem(1)#3 - DAflow(1,2)#4
                            - DAflow(3,2)#5 + RTgen(2)#10 - RTdem(1)#12
                            + RTflow(1,2)#13 + RTflow(3,2)#14  = 0
 RTflowBalance(3)#8:        - DAgen(3)#2 + DAflow(3,2)#5 + RTgen(3)#11
                            - RTflow(3,2)#14  = 0
 RTdcApproximation(2,1)#9:  RTflow(1,2)#13 + 100 RTtheta(1)#15
                            - 100 RTtheta(2)#16  = 0
 RTdcApproximation(2,3)#10: RTflow(3,2)#14 - 100 RTtheta(2)#16
                            + 100 RTtheta(3)#17  = 0
 RTrefAngle(1)#11:          RTtheta(1)#15  = 0
Bounds
 0 <= DAgen(1)#0 <= 70
 0 <= DAgen(2)#1 <= 60
 0 <= DAgen(3)#2 <= 80
 0 <= DAdem(1)#3 <= 100
-25 <= DAflow(1,2)#4 <= 25
-50 <= DAflow(3,2)#5 <= 50
-3.1451 <= DAtheta(1)#6 <= 3.1451
-3.1451 <= DAtheta(2)#7 <= 3.1451
-3.1451 <= DAtheta(3)#8 <= 3.1451
 0 <= RTgen(1)#9 <= 70
 0 <= RTgen(2)#10 <= 60
 0 <= RTgen(3)#11 <= 80
 0 <= RTdem(1)#12 <= 100
-25 <= RTflow(1,2)#13 <= 25
-50 <= RTflow(3,2)#14 <= 50
-3.1451 <= RTtheta(1)#15 <= 3.1451
-3.1451 <= RTtheta(2)#16 <= 3.1451
-3.1451 <= RTtheta(3)#17 <= 3.1451
 0 <= RTetaGenP(1)#18 <= 70
-70 <= RTetaGenM(1)#19 <= 0
 0 <= RTetaGenP(2)#20 <= 60
-60 <= RTetaGenM(2)#21 <= 0
 0 <= RTetaGenP(3)#22 <= 80
-80 <= RTetaGenM(3)#23 <= 0
 0 <= RTetaDemP(1)#24 <= 100
-100 <= RTetaDemM(1)#25 <= 0
End
