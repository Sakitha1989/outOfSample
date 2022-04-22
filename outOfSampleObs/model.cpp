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
		subproblemModel.DAgen = IloArray<IloNumVarArray>(subproblemModel.env, 1);
		subproblemModel.DAdem = IloArray<IloNumVarArray>(subproblemModel.env, 1);
		subproblemModel.DAflow = IloArray<IloNumVarArray>(subproblemModel.env, 1);
		subproblemModel.DAtheta = IloArray<IloNumVarArray>(subproblemModel.env, 1);

		subproblemModel.DAflowBalance = IloArray<IloRangeArray>(subproblemModel.env, 1);
		subproblemModel.DAdcApproximation = IloArray<IloRangeArray>(subproblemModel.env, 1);
		subproblemModel.DArefAngle = IloArray<IloRangeArray>(subproblemModel.env, 1);

		/* Initialize the anticipative stage components */
		subproblemModel.RTgen = IloArray<IloNumVarArray>(subproblemModel.env, 1);
		subproblemModel.RTdem = IloArray<IloNumVarArray>(subproblemModel.env, 1);
		subproblemModel.RTflow = IloArray<IloNumVarArray>(subproblemModel.env, 1);
		subproblemModel.RTtheta = IloArray<IloNumVarArray>(subproblemModel.env, 1);
		subproblemModel.RTetaP = IloArray<IloNumVarArray>(subproblemModel.env, 1);
		subproblemModel.RTetaM = IloArray<IloNumVarArray>(subproblemModel.env, 1);

		subproblemModel.RTflowBalance = IloArray<IloRangeArray>(subproblemModel.env, 1);
		subproblemModel.RTdcApproximation = IloArray<IloRangeArray>(subproblemModel.env, 1);
		subproblemModel.RTrefAngle = IloArray<IloRangeArray>(subproblemModel.env, 1);
		subproblemModel.genPositive = IloArray<IloRangeArray>(subproblemModel.env, 1);
		subproblemModel.genNegative = IloArray<IloRangeArray>(subproblemModel.env, 1);
		subproblemModel.demPositive = IloArray<IloRangeArray>(subproblemModel.env, 1);
		subproblemModel.demNegative = IloArray<IloRangeArray>(subproblemModel.env, 1);
		subproblemModel.stochGen = IloArray<IloRangeArray>(subproblemModel.env, 1);
		subproblemModel.inflexGen = IloArray<IloRangeArray>(subproblemModel.env, 1);


		//detVariables(sys, subproblemModel, scenID);
		//detConstraints(sys, subproblemModel, scenID);
		//detObjective(sys, subproblemModel, prob, scenID);

		//lagrangian(sys, subproblemModel, prob, scenID, NA_dual);

		//stocVariables(sys, subproblemModel, scenID);
		//stocConstraints(sys, subproblemModel, scenID, parentID, observ);
		//stocObjective(sys, subproblemModel, prob, scenID);

		//subproblemModel.exportModel("subproblemModel.lp");


		/* solve the model and obtain solutions */
		//subproblemModel.solve(sys, config.type);


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

bool ClearingModel::solve(PowerSystem sys, string type) {

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

void ClearingModel::exportModel(string fname) {
	cplex.exportModel((".//outputDir//" + fname).c_str());
}