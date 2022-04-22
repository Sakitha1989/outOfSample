/*
 * outOfSampleObs.cpp
 *
 *  Created on: Apr 19, 2022
 *      Author: Sakitha Ariyarathne
 * Institution: Southern Methodist University
 *
 */

#include "PowerSystem.hpp"
#include "outOfSampleObs.hpp"

void outOfSampleAlg(PowerSystem sys, CSVcontent scenarioData, string inputDir, ofstream &output_file, double incumbent_deviation, double dual_deviation, int iteration_num, int incumbent_index) {

	vector<double> NA_dual;
	for (int i = 0; i < (sys.numGenerators + sys.numLoads); i++)
	{
		NA_dual.push_back(0);
	}

	subproblem(sys, NA_dual, inputDir, output_file);

}

//void createSubproblem(PowerSystem sys, ClearingModel &subproblemModel, vector<vector<double>> observ, vector<double> NA_dual, double prob, int scenID, int parentID) {
//
//	solnType soln;
//
//	detVariables(sys, subproblemModel, scenID);
//	detConstraints(sys, subproblemModel, scenID);
//	detObjective(sys, subproblemModel, prob, scenID);
//
//	lagrangian(sys, subproblemModel, prob, scenID, NA_dual);
//
//	stocVariables(sys, subproblemModel, scenID);
//	stocConstraints(sys, subproblemModel, scenID, parentID, observ);
//	stocObjective(sys, subproblemModel, prob, scenID);
//
//#if defined (SAVE_FILES)
//	subproblemModel.exportModel("subproblemModel.lp");
//#endif
//
//}//END scenarioProblem()

//void lagrangian(PowerSystem sys, ClearingModel &M, double weight, int scen, vector<double> NA_dual) {
//
//	/***** Objective function *****/
//	IloExpr lagrangianCost(M.env);
//	lagrangianCost = M.obj.getExpr();
//	M.model.remove(M.obj);
//
//	lagrangianCost += 0.0;
//		for (int g = 0; g < sys.numGenerators; g++) {
//
//			int it = 0;
//			while (it < M.bidDA.genBids[t].size())
//			{
//				if (M.bidDA.genBids[t][it].ID == sys.generators[g].id)
//				{
//					break;
//				}
//				it++;
//			}
//
//#if defined (RT_PAYMENT_MODEL)
//			if (it < M.bidDA.genBids[t].size()) {
//				lagrangianCost -= weight * NA_dual[it] * M.DAgen[0][it][t];
//			}
//#endif
//		}
//		for (int d = 0; d < sys.numLoads; d++) {
//
//			int it = 0;
//			while (it < M.bidDA.demBids[t].size())
//			{
//				if (M.bidDA.demBids[t][it].ID == sys.loads[d].id)
//				{
//					break;
//				}
//				it++;
//			}
//
//#if defined (RT_PAYMENT_MODEL)
//			if (it < M.bidDA.demBids[t].size()) {
//				lagrangianCost -= weight * NA_dual[sys.numGenerators + it] * M.DAdem[0][it][t];
//			}
//#else
//			if (it < M.bidDA.demBids[t].size()) {
//				dayAheadCost -= weight * M.bidDA.demBids[t][it].price*M.DAdem[scen][i][t];
//			}
//#endif
//		}
//
//	M.obj.setExpr(lagrangianCost);
//	M.model.add(M.obj);
//	lagrangianCost.end();
//
//}//END detObjective()