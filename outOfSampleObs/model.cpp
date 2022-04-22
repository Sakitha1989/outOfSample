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

		//ClearingModel subproblemModel(sys, true, tree);
		//subproblemModel.name = "subproblem";

		///* Initialize for first-stage component addition */
		//subproblemModel.DAgen = IloArray< IloArray <IloNumVarArray> >(subproblemModel.env, 1);
		//subproblemModel.DAdem = IloArray< IloArray <IloNumVarArray> >(subproblemModel.env, 1);
		//subproblemModel.DAflow = IloArray< IloArray <IloNumVarArray> >(subproblemModel.env, 1);
		//subproblemModel.DAtheta = IloArray< IloArray <IloNumVarArray> >(subproblemModel.env, 1);

		//subproblemModel.DAflowBalance = IloArray <IloArray <IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.DAdc = IloArray <IloArray <IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.DAru = IloArray <IloArray <IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.DArd = IloArray <IloArray <IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.DAgbu = IloArray <IloArray <IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.DAdbu = IloArray <IloArray <IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.DAdbu = IloArray <IloArray <IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.DArefAngle = IloArray <IloArray <IloRangeArray>>(subproblemModel.env, 1);

		///* Initialize the anticipative stage components */
		//subproblemModel.RTgen = IloArray< IloArray <IloNumVarArray> >(subproblemModel.env, 1);
		//subproblemModel.RTdem = IloArray< IloArray <IloNumVarArray> >(subproblemModel.env, 1);
		//subproblemModel.RTflow = IloArray< IloArray <IloNumVarArray> >(subproblemModel.env, 1);
		//subproblemModel.RTtheta = IloArray< IloArray <IloNumVarArray> >(subproblemModel.env, 1);
		//subproblemModel.RTetaP = IloArray< IloArray <IloNumVarArray> >(subproblemModel.env, 1);
		//subproblemModel.RTetaM = IloArray< IloArray <IloNumVarArray> >(subproblemModel.env, 1);

		//subproblemModel.RTflowBalance = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTdc = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTru = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTrd = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTgbu = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTgbd = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTdbu = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTdbd = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTdgP = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTdgM = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTddP = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTddM = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTga = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTda = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTrefAngle = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);
		//subproblemModel.RTinflex = IloArray<IloArray<IloRangeArray>>(subproblemModel.env, 1);

		/* Setup the sample problem */
		//createSubproblem(sys, subproblemModel, observ, NA_dual, config, 1.0, 0, 0);


		/* solve the model and obtain solutions */
		//subproblemModel.solve(sys, config.type);


		//soln.push_back(getSolution(subproblemModel, sys, true, 1));

	}

}//END waitNsee

ClearingModel::ClearingModel(PowerSystem sys, bool isStocModel) {

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