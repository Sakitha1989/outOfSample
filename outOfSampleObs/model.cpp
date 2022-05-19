/*
 * outOfSampleObs.hpp
 *
 *  Created on: Apr 20, 2022
 *      Author: Sakitha Ariyarathne
 * Institution: Southern Methodist University
 *
 */

#include "model.hpp"


void subproblem(PowerSystem sys, ClearingModel &M, vector<double> naDuals) {

	M.name = "subproblem";

	/* Initialize for first-stage component addition */
	M.DAgen = IloNumVarArray(M.env);
	M.DAdem = IloNumVarArray(M.env);
	M.DAflow = IloNumVarArray(M.env);
	M.DAtheta = IloNumVarArray(M.env);

	M.DAflowBalance = IloRangeArray(M.env);
	M.DAdcApproximation = IloRangeArray(M.env);
	M.DArefAngle = IloRangeArray(M.env);

	/* Initialize the anticipative stage components */
	M.RTgen = IloNumVarArray(M.env);
	M.RTdem = IloNumVarArray(M.env);
	M.RTflow = IloNumVarArray(M.env);
	M.RTtheta = IloNumVarArray(M.env);
	M.RTetaGenP = IloNumVarArray(M.env);
	M.RTetaGenM = IloNumVarArray(M.env);
	M.RTetaDemP = IloNumVarArray(M.env);
	M.RTetaDemM = IloNumVarArray(M.env);

	M.RTflowBalance = IloRangeArray(M.env);
	M.RTdcApproximation = IloRangeArray(M.env);
	M.RTrefAngle = IloRangeArray(M.env);
	M.genPositive = IloRangeArray(M.env);
	M.genNegative = IloRangeArray(M.env);
	M.demPositive = IloRangeArray(M.env);
	M.demNegative = IloRangeArray(M.env);
	M.inflexGen = IloRangeArray(M.env);

	addVariables(sys, M);
	addConstraints(sys, M);
	detObjective(sys, M, naDuals);
	stocObjective(sys, M);

	M.exportModel(sys, "subproblemModel.lp");

}//END subproblem

void masterproblem(PowerSystem sys, masterType::MasterProblem &M, double sigma) {

	M.name = "master";

	/* Initialize for first-stage component addition */
	M.nu = IloNumVar(M.env);
	M.naDual = IloArray<IloNumVarArray>(M.env, sys.numScenarios);

	/* Initialize the anticipative stage components */
	M.expectation = IloRangeArray(M.env);

	addMasterVariables(sys, M);
	addMasterConstraints(sys, M);
	addMasterObjective(sys, M, sigma);

	M.exportModel(sys, "master.lp");

}//END masterproblem

ClearingModel::ClearingModel(PowerSystem sys) {

	model = IloModel(env);
	cplex = IloCplex(model);

	/* Initialize a dummy objective function */
	obj = IloMinimize(env, 0.0);
	model.add(obj);

	cplex.setOut(env.getNullStream());
	cplex.setWarning(env.getNullStream());

}// ClearingModel constructor ()

bool ClearingModel::solve() {

	try {

		cplex.setParam(cplex.RootAlg, cplex.Primal);

		cplex.solve();

		int status = cplex.getStatus();
		if (status != IloAlgorithm::Optimal){ // CPX_STAT_OPTIMAL
			env.out() << "Solution status = " << cplex.getCplexStatus() << endl;
			env.out() << "Solution value  = " << cplex.getObjValue() << endl;
		}
			
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

masterType::MasterProblem::MasterProblem() {

	model = IloModel(env);
	cplex = IloCplex(model);

	/* Initialize a dummy objective function */
	obj = IloMaximize(env, 0.0);
	model.add(obj);

	cplex.setOut(env.getNullStream());
	cplex.setWarning(env.getNullStream());

}// ClearingModel constructor ()
masterType::MasterProblem::MasterProblem(PowerSystem sys) {

	model = IloModel(env);
	cplex = IloCplex(model);

	/* Initialize a dummy objective function */
	obj = IloMaximize(env, 0.0);
	model.add(obj);

	cplex.setOut(env.getNullStream());
	cplex.setWarning(env.getNullStream());

}// ClearingModel constructor ()

bool masterType::MasterProblem::solve() {

	try {

		cplex.setParam(cplex.RootAlg, cplex.Primal);

		cplex.solve();

		int status = cplex.getStatus();
		if (status != IloAlgorithm::Optimal) { // CPX_STAT_OPTIMAL
			env.out() << "Solution status = " << cplex.getCplexStatus() << endl;
			env.out() << "Solution value  = " << cplex.getObjValue() << endl;
		}
		return status;
	}
	catch (IloException &e) {
		cout << e << endl;
		return false;
	}
}

void masterType::MasterProblem::exportModel(PowerSystem sys, string fname) {
	cplex.exportModel(("..\\outputData\\" + sys.name + "\\" + fname).c_str());
}

solution getSolution(ClearingModel M, PowerSystem sys) {

	solution soln;

	soln.obj = M.cplex.getValue(M.obj);

	soln.x.DAgen = vector<double>(sys.numGenerators);
	soln.x.DAdem = vector<double>(sys.numLoads);
	soln.x.DAflow = vector<double>(sys.numLines);
	soln.x.DAtheta = vector<double>(sys.numBuses);

	soln.x.RTgen = vector<double>(sys.numGenerators);
	soln.x.RTdem = vector<double>(sys.numLoads);
	soln.x.RTflow = vector<double>(sys.numLines);
	soln.x.RTtheta = vector<double>(sys.numBuses);
	soln.x.RTetaGenP = vector<double>(sys.numGenerators);
	soln.x.RTetaGenM = vector<double>(sys.numGenerators);
	soln.x.RTetaDemP = vector<double>(sys.numLoads);
	soln.x.RTetaDemM = vector<double>(sys.numLoads);

	soln.pi.DAflowBalance = vector<double>(sys.numBuses);
	soln.pi.DAdcApproximation = vector<double>(sys.numLines);
	soln.pi.DArefAngle = vector<double>(sys.numBuses);

	soln.pi.RTflowBalance = vector<double>(sys.numBuses);
	soln.pi.RTdcApproximation = vector<double>(sys.numLines);
	soln.pi.RTrefAngle = vector<double>(sys.numBuses);
	soln.pi.genPositive = vector<double>(sys.numGenerators);
	soln.pi.genNegative = vector<double>(sys.numGenerators);
	soln.pi.demPositive = vector<double>(sys.numGenerators);
	soln.pi.demNegative = vector<double>(sys.numGenerators);
	soln.pi.inflexGen = vector<double>(sys.numInflexGens);
	
	soln.x.DAgen = getPrimal(M.cplex, M.DAgen);
	soln.x.DAdem = getPrimal(M.cplex, M.DAdem);
	soln.x.DAflow = getPrimal(M.cplex, M.DAflow);
	soln.x.DAtheta = getPrimal(M.cplex, M.DAtheta);

	soln.x.RTgen = getPrimal(M.cplex, M.RTgen);
	soln.x.RTdem = getPrimal(M.cplex, M.RTdem);
	soln.x.RTflow = getPrimal(M.cplex, M.RTflow);
	soln.x.RTtheta = getPrimal(M.cplex, M.RTtheta);
	soln.x.RTetaGenP = getPrimal(M.cplex, M.RTetaGenP);
	soln.x.RTetaGenM = getPrimal(M.cplex, M.RTetaGenM);
	soln.x.RTetaDemP = getPrimal(M.cplex, M.RTetaDemP);
	soln.x.RTetaDemM = getPrimal(M.cplex, M.RTetaDemM);

	soln.pi.DAflowBalance = getDual(M.cplex, M.DAflowBalance);
	soln.pi.DAdcApproximation = getDual(M.cplex, M.DAdcApproximation);
	soln.pi.DArefAngle = getDual(M.cplex, M.DArefAngle);

	soln.pi.RTflowBalance = getDual(M.cplex, M.RTflowBalance);
	soln.pi.RTdcApproximation = getDual(M.cplex, M.RTdcApproximation);
	soln.pi.RTrefAngle = getDual(M.cplex, M.RTrefAngle);
	/*soln.pi.genPositive = getDual(M.cplex, M.genPositive);
	soln.pi.genNegative = getDual(M.cplex, M.genNegative);
	soln.pi.demPositive = getDual(M.cplex, M.demPositive);
	soln.pi.demNegative = getDual(M.cplex, M.demNegative);
	soln.pi.inflexGen = getDual(M.cplex, M.inflexGen);*/

	return soln;

}//END getSolution()

void masterGetSolution(masterType &M, PowerSystem sys) {

	M.obj = M.prob.cplex.getValue(M.prob.obj);
	M.candidEst = M.prob.cplex.getValue(M.prob.nu);

	
	for (int s = 0; s < sys.numScenarios; s++){

		M.candidNa[s] = vector<double>(M.prob.naDual[s].getSize());
		M.candidNa[s] = getPrimal(M.prob.cplex, M.prob.naDual[s]);
	}
}

vector<double> getPrimal(IloCplex cplex, IloNumVarArray x) {

	int dim1 = x.getSize();

	vector<double> soln(dim1);
	for (int m = 0; m < dim1; m++) {
		soln[m] = cplex.getValue(x[m]);
	}
	return soln;
}//END getPrimal()

vector<double> getDual(IloCplex cplex, IloRangeArray c) {

	int dim1 = c.getSize();

	vector<double> dual(dim1);
	for (int m = 0; m < dim1; m++) {
		dual[m] = cplex.getDual(c[m]);
	}

	return dual;
}//END getDual()