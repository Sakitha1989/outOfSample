\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj: 40 RTgen(1)#6 + 45 RTgen(3)#8 + 42 RTgen(5)#10 + 80 RTgen(6)#11
      + 0.00100000004749745 RTetaGenP(1)#12
      + 0.00100000004749745 RTetaGenP(2)#13
      + 0.00100000004749745 RTetaGenP(3)#14
      + 0.00100000004749745 RTetaGenP(4)#15
      + 0.00100000004749745 RTetaGenP(5)#16
      + 0.00100000004749745 RTetaGenP(6)#17
      - 0.00100000004749745 RTetaGenM(1)#18
      - 0.00100000004749745 RTetaGenM(2)#19
      - 0.00100000004749745 RTetaGenM(3)#20
      - 0.00100000004749745 RTetaGenM(4)#21
      - 0.00100000004749745 RTetaGenM(5)#22
      - 0.00100000004749745 RTetaGenM(6)#23 - 1000 RTdem(1)#25
      + 0.00100000004749745 RTetaDemP(1)#26
      - 0.00100000004749745 RTetaDemM(1)#27
Subject To
 DAflowBalance(1)#0:        DAgen(1)#0 - DAflow(1,2)#28 + DAflow(6,1)#33  = 0
 DAflowBalance(2)#1:        DAgen(2)#1 + DAflow(1,2)#28 - DAflow(2,3)#29  = 0
 DAflowBalance(3)#2:        DAgen(3)#2 + DAflow(2,3)#29 - DAflow(3,4)#30  = 0
 DAflowBalance(4)#3:        DAgen(4)#3 + DAflow(3,4)#30 - DAflow(4,5)#31  = 0
 DAflowBalance(5)#4:        DAgen(5)#4 + DAgen(6)#5 + DAflow(4,5)#31
                            - DAflow(5,6)#32  = 0
 DAflowBalance(6)#5:        - DAdem(1)#24 + DAflow(5,6)#32 - DAflow(6,1)#33
                             = 0
 RTflowBalance(1)#6:        - DAgen(1)#0 + RTgen(1)#6 + DAflow(1,2)#28
                            - DAflow(6,1)#33 - RTflow(1,2)#34 + RTflow(6,1)#39
                             = 0
 RTflowBalance(2)#7:        - DAgen(2)#1 + RTgen(2)#7 - DAflow(1,2)#28
                            + DAflow(2,3)#29 + RTflow(1,2)#34 - RTflow(2,3)#35
                             = 0
 RTflowBalance(3)#8:        - DAgen(3)#2 + RTgen(3)#8 - DAflow(2,3)#29
                            + DAflow(3,4)#30 + RTflow(2,3)#35 - RTflow(3,4)#36
                             = 0
 RTflowBalance(4)#9:        - DAgen(4)#3 + RTgen(4)#9 - DAflow(3,4)#30
                            + DAflow(4,5)#31 + RTflow(3,4)#36 - RTflow(4,5)#37
                             = 0
 RTflowBalance(5)#10:       - DAgen(5)#4 - DAgen(6)#5 + RTgen(5)#10
                            + RTgen(6)#11 - DAflow(4,5)#31 + DAflow(5,6)#32
                            + RTflow(4,5)#37 - RTflow(5,6)#38  = 0
 RTflowBalance(6)#11:       DAdem(1)#24 - RTdem(1)#25 - DAflow(5,6)#32
                            + DAflow(6,1)#33 + RTflow(5,6)#38 - RTflow(6,1)#39
                             = 0
 DAdcApproximation(1,2)#12: DAflow(1,2)#28 - 100 DAtheta(1)#40
                            + 100 DAtheta(2)#41  = 0
 DAdcApproximation(2,3)#13: DAflow(2,3)#29 - 100 DAtheta(2)#41
                            + 100 DAtheta(3)#42  = 0
 DAdcApproximation(3,4)#14: DAflow(3,4)#30 - 100 DAtheta(3)#42
                            + 100 DAtheta(4)#43  = 0
 DAdcApproximation(4,5)#15: DAflow(4,5)#31 - 100 DAtheta(4)#43
                            + 100 DAtheta(5)#44  = 0
 DAdcApproximation(5,6)#16: DAflow(5,6)#32 - 100 DAtheta(5)#44
                            + 100 DAtheta(6)#45  = 0
 DAdcApproximation(6,1)#17: DAflow(6,1)#33 + 100 DAtheta(1)#40
                            - 100 DAtheta(6)#45  = 0
 RTdcApproximation(1,2)#18: RTflow(1,2)#34 - 100 RTtheta(1)#46
                            + 100 RTtheta(2)#47  = 0
 RTdcApproximation(2,3)#19: RTflow(2,3)#35 - 100 RTtheta(2)#47
                            + 100 RTtheta(3)#48  = 0
 RTdcApproximation(3,4)#20: RTflow(3,4)#36 - 100 RTtheta(3)#48
                            + 100 RTtheta(4)#49  = 0
 RTdcApproximation(4,5)#21: RTflow(4,5)#37 - 100 RTtheta(4)#49
                            + 100 RTtheta(5)#50  = 0
 RTdcApproximation(5,6)#22: RTflow(5,6)#38 - 100 RTtheta(5)#50
                            + 100 RTtheta(6)#51  = 0
 RTdcApproximation(6,1)#23: RTflow(6,1)#39 + 100 RTtheta(1)#46
                            - 100 RTtheta(6)#51  = 0
 DArefAngle(1)#24:          DAtheta(1)#40  = 0
 RTrefAngle(1)#25:          RTtheta(1)#46  = 0
 genPositive(1)#26:         - DAgen(1)#0 + RTgen(1)#6 - RTetaGenP(1)#12 <= 0
 genNegative(1)#27:         DAgen(1)#0 - RTgen(1)#6 + RTetaGenM(1)#18 <= 0
 genPositive(2)#28:         - DAgen(2)#1 + RTgen(2)#7 - RTetaGenP(2)#13 <= 0
 genNegative(2)#29:         DAgen(2)#1 - RTgen(2)#7 + RTetaGenM(2)#19 <= 0
 genPositive(3)#30:         - DAgen(3)#2 + RTgen(3)#8 - RTetaGenP(3)#14 <= 0
 genNegative(3)#31:         DAgen(3)#2 - RTgen(3)#8 + RTetaGenM(3)#20 <= 0
 genPositive(4)#32:         - DAgen(4)#3 + RTgen(4)#9 - RTetaGenP(4)#15 <= 0
 genNegative(4)#33:         DAgen(4)#3 - RTgen(4)#9 + RTetaGenM(4)#21 <= 0
 genPositive(5)#34:         - DAgen(5)#4 + RTgen(5)#10 - RTetaGenP(5)#16 <= 0
 genNegative(5)#35:         DAgen(5)#4 - RTgen(5)#10 + RTetaGenM(5)#22 <= 0
 genPositive(6)#36:         - DAgen(6)#5 + RTgen(6)#11 - RTetaGenP(6)#17 <= 0
 genNegative(6)#37:         DAgen(6)#5 - RTgen(6)#11 + RTetaGenM(6)#23 <= 0
 demPositive(1)#38:         - DAdem(1)#24 + RTdem(1)#25 - RTetaDemP(1)#26 <= 0
 demNegative(1)#39:         DAdem(1)#24 - RTdem(1)#25 + RTetaDemM(1)#27 <= 0
Bounds
 0 <= DAgen(1)#0 <= 100
 0 <= DAgen(2)#1 <= 90
 0 <= DAgen(3)#2 <= 100
 0 <= DAgen(4)#3 <= 90
 0 <= DAgen(5)#4 <= 50
 0 <= DAgen(6)#5 <= 60
 0 <= RTgen(1)#6 <= 100
 0 <= RTgen(2)#7 <= 90
 0 <= RTgen(3)#8 <= 100
 0 <= RTgen(4)#9 <= 90
 0 <= RTgen(5)#10 <= 50
 0 <= RTgen(6)#11 <= 60
 0 <= RTetaGenP(1)#12 <= 100
 0 <= RTetaGenP(2)#13 <= 90
 0 <= RTetaGenP(3)#14 <= 100
 0 <= RTetaGenP(4)#15 <= 90
 0 <= RTetaGenP(5)#16 <= 50
 0 <= RTetaGenP(6)#17 <= 60
-100 <= RTetaGenM(1)#18 <= 0
-90 <= RTetaGenM(2)#19 <= 0
-100 <= RTetaGenM(3)#20 <= 0
-90 <= RTetaGenM(4)#21 <= 0
-50 <= RTetaGenM(5)#22 <= 0
-60 <= RTetaGenM(6)#23 <= 0
 0 <= DAdem(1)#24 <= 264
 0 <= RTdem(1)#25 <= 264
 0 <= RTetaDemP(1)#26 <= 264
-264 <= RTetaDemM(1)#27 <= 0
-99999 <= DAflow(1,2)#28 <= 99999
-99999 <= DAflow(2,3)#29 <= 99999
-99999 <= DAflow(3,4)#30 <= 99999
-99999 <= DAflow(4,5)#31 <= 99999
-99999 <= DAflow(5,6)#32 <= 99999
-150 <= DAflow(6,1)#33 <= 150
-99999 <= RTflow(1,2)#34 <= 99999
-99999 <= RTflow(2,3)#35 <= 99999
-99999 <= RTflow(3,4)#36 <= 99999
-99999 <= RTflow(4,5)#37 <= 99999
-99999 <= RTflow(5,6)#38 <= 99999
-150 <= RTflow(6,1)#39 <= 150
-3.1451 <= DAtheta(1)#40 <= 3.1451
-3.1451 <= DAtheta(2)#41 <= 3.1451
-3.1451 <= DAtheta(3)#42 <= 3.1451
-3.1451 <= DAtheta(4)#43 <= 3.1451
-3.1451 <= DAtheta(5)#44 <= 3.1451
-3.1451 <= DAtheta(6)#45 <= 3.1451
-3.1451 <= RTtheta(1)#46 <= 3.1451
-3.1451 <= RTtheta(2)#47 <= 3.1451
-3.1451 <= RTtheta(3)#48 <= 3.1451
-3.1451 <= RTtheta(4)#49 <= 3.1451
-3.1451 <= RTtheta(5)#50 <= 3.1451
-3.1451 <= RTtheta(6)#51 <= 3.1451
End
