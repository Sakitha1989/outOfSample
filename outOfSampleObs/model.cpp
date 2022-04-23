/*
 * outOfSampleObs.hpp
 *
 *  Created on: Apr 20, 2022
 *      Author: Sakitha Ariyarathne
 * Institution: Southern Methodist University
 *
 */

#include "model.hpp"
#include "outOfSampleObs.hpp"


void subproblem(PowerSystem sys, vector<double> NA_dual, string inputDir, ofstream &output_file) {
	vector<vector<double>> observ;
	vector<solnType> soln;
	double obj = 0.0;

	for (int s = 0; s < 3; s++)
	{

		ClearingModel subproblemModel(sys);
		subproblemModel.name = "subproblem";

		/* Initialize for first-stage component addition */
		subproblemModel.DAgen = IloNumVarArray(subproblemModel.env);
		subproblemModel.DAdem = IloNumVarArray(subproblemModel.env);
		subproblemModel.DAflow = IloNumVarArray(subproblemModel.env);
		subproblemModel.DAtheta = IloNumVarArray(subproblemModel.env);

		subproblemModel.DAflowBalance = IloRangeArray(subproblemModel.env);
		subproblemModel.DAdcApproximation = IloRangeArray(subproblemModel.env);
		subproblemModel.DArefAngle = IloRangeArray(subproblemModel.env);

		/* Initialize the anticipative stage components */
		subproblemModel.RTgen = IloNumVarArray(subproblemModel.env);
		subproblemModel.RTdem = IloNumVarArray(subproblemModel.env);
		subproblemModel.RTflow = IloNumVarArray(subproblemModel.env);
		subproblemModel.RTtheta = IloNumVarArray(subproblemModel.env);
		subproblemModel.RTetaGenP = IloNumVarArray(subproblemModel.env);
		subproblemModel.RTetaGenM = IloNumVarArray(subproblemModel.env);
		subproblemModel.RTetaDemP = IloNumVarArray(subproblemModel.env);
		subproblemModel.RTetaDemM = IloNumVarArray(subproblemModel.env);

		subproblemModel.RTflowBalance = IloRangeArray(subproblemModel.env);
		subproblemModel.RTdcApproximation = IloRangeArray(subproblemModel.env);
		subproblemModel.RTrefAngle = IloRangeArray(subproblemModel.env);
		subproblemModel.genPositive = IloRangeArray(subproblemModel.env);
		subproblemModel.genNegative = IloRangeArray(subproblemModel.env);
		subproblemModel.demPositive = IloRangeArray(subproblemModel.env);
		subproblemModel.demNegative = IloRangeArray(subproblemModel.env);
		subproblemModel.stochGen = IloRangeArray(subproblemModel.env);
		subproblemModel.inflexGen = IloRangeArray(subproblemModel.env);


		detVariables(sys, subproblemModel);
		detConstraints(sys, subproblemModel);
		detObjective(sys, subproblemModel, NA_dual);

		stocVariables(sys, subproblemModel);
		stocConstraints(sys, subproblemModel, s);
		stocObjective(sys, subproblemModel);

		subproblemModel.exportModel(sys, "subproblemModel.lp");


		/* solve the model and obtain solutions */
		subproblemModel.solve();


		//soln.push_back(getSolution(subproblemModel, sys, true, 1));

	}

}//END waitNsee

ClearingModel::ClearingModel(PowerSystem sys) {

	model = IloModel(env);
	cplex = IloCplex(model);

	/* Initialize a dummy objective function */
	obj = IloMinimize(env, 0.0);
	model.add(obj);

	//cplex.setOut(env.getNullStream());
	//cplex.setWarning(env.getNullStream());

}// ClearingModel constructor ()

bool ClearingModel::solve() {

	try {

		cplex.setParam(cplex.RootAlg, cplex.Primal);

		bool status = cplex.solve();
		return status;
	}
	catch (IloException &e) {
		cout << e << endl;
		return false;
	}
}

void ClearingModel::exportModel(PowerSystem sys, string fname) {
	cplex.exportModel(("..\\outputData\\" + sys.name + "\\" + fname).c_str());
}