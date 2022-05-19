\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj: 10 RTgen(1)#3 + RTgen(2)#4 + 20 RTgen(3)#5
      + 1.10000002384186 RTetaGenP(1)#6 + 0.109999999403954 RTetaGenP(2)#7
      + 2.09999990463257 RTetaGenP(3)#8 - RTetaGenM(1)#9
      - 0.100000001490116 RTetaGenM(2)#10 - 2 RTetaGenM(3)#11
      - 1000 RTdem(1)#13 + 0.00100000004749745 RTetaDemP(1)#14
      - 0.0000999999974737875 RTetaDemM(1)#15
Subject To
 DAflowBalance(1)#0:       DAgen(1)#0 - DAflow(1,2)#16  = 0
 DAflowBalance(2)#1:       DAgen(2)#1 - DAdem(1)#12 + DAflow(1,2)#16
                           + DAflow(3,2)#17  = 0
 DAflowBalance(3)#2:       DAgen(3)#2 - DAflow(3,2)#17  = 0
 RTflowBalance(1)#3:       - DAgen(1)#0 + RTgen(1)#3 + DAflow(1,2)#16
                           - RTflow(1,2)#18  = 0
 RTflowBalance(2)#4:       - DAgen(2)#1 + RTgen(2)#4 + DAdem(1)#12
                           - RTdem(1)#13 - DAflow(1,2)#16 - DAflow(3,2)#17
                           + RTflow(1,2)#18 + RTflow(3,2)#19  = 0
 RTflowBalance(3)#5:       - DAgen(3)#2 + RTgen(3)#5 + DAflow(3,2)#17
                           - RTflow(3,2)#19  = 0
 DAdcApproximation(1,2)#6: DAflow(1,2)#16 - 100 DAtheta(1)#20
                           + 100 DAtheta(2)#21  = 0
 DAdcApproximation(3,2)#7: DAflow(3,2)#17 + 100 DAtheta(2)#21
                           - 100 DAtheta(3)#22  = 0
 RTdcApproximation(1,2)#8: RTflow(1,2)#18 - 100 RTtheta(1)#23
                           + 100 RTtheta(2)#24  = 0
 RTdcApproximation(3,2)#9: RTflow(3,2)#19 + 100 RTtheta(2)#24
                           - 100 RTtheta(3)#25  = 0
 DArefAngle(1)#10:         DAtheta(1)#20  = 0
 RTrefAngle(1)#11:         RTtheta(1)#23  = 0
 genPositive(1)#12:        - DAgen(1)#0 + RTgen(1)#3 - RTetaGenP(1)#6 <= 0
 genNegative(1)#13:        DAgen(1)#0 - RTgen(1)#3 + RTetaGenM(1)#9 <= 0
 genPositive(2)#14:        - DAgen(2)#1 + RTgen(2)#4 - RTetaGenP(2)#7 <= 0
 genNegative(2)#15:        DAgen(2)#1 - RTgen(2)#4 + RTetaGenM(2)#10 <= 0
 genPositive(3)#16:        - DAgen(3)#2 + RTgen(3)#5 - RTetaGenP(3)#8 <= 0
 genNegative(3)#17:        DAgen(3)#2 - RTgen(3)#5 + RTetaGenM(3)#11 <= 0
 demPositive(1)#18:        - DAdem(1)#12 + RTdem(1)#13 - RTetaDemP(1)#14 <= 0
 demNegative(1)#19:        DAdem(1)#12 - RTdem(1)#13 + RTetaDemM(1)#15 <= 0
Bounds
 0 <= DAgen(1)#0 <= 70
 0 <= DAgen(2)#1 <= 60
 0 <= DAgen(3)#2 <= 80
 0 <= RTgen(1)#3 <= 70
 0 <= RTgen(2)#4 <= 60
 0 <= RTgen(3)#5 <= 80
 0 <= RTetaGenP(1)#6 <= 70
 0 <= RTetaGenP(2)#7 <= 60
 0 <= RTetaGenP(3)#8 <= 80
-70 <= RTetaGenM(1)#9 <= 0
-60 <= RTetaGenM(2)#10 <= 0
-80 <= RTetaGenM(3)#11 <= 0
 0 <= DAdem(1)#12 <= 100
 0 <= RTdem(1)#13 <= 100
 0 <= RTetaDemP(1)#14 <= 100
-100 <= RTetaDemM(1)#15 <= 0
-25 <= DAflow(1,2)#16 <= 25
-50 <= DAflow(3,2)#17 <= 50
-25 <= RTflow(1,2)#18 <= 25
-50 <= RTflow(3,2)#19 <= 50
-3.1451 <= DAtheta(1)#20 <= 3.1451
-3.1451 <= DAtheta(2)#21 <= 3.1451
-3.1451 <= DAtheta(3)#22 <= 3.1451
-3.1451 <= RTtheta(1)#23 <= 3.1451
-3.1451 <= RTtheta(2)#24 <= 3.1451
-3.1451 <= RTtheta(3)#25 <= 3.1451
End
