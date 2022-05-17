/*
 * setup.cpp
 *
 *  Created on: Apr 22, 2022
 *      Author: Sakitha Ariyarathne
 * Institution: Southern Methodist University
 *
 */

#include "model.hpp"

oneCut::oneCut(PowerSystem sys, int numVar) {

	beta = vector<vector<double>>(sys.numScenarios, vector<double>(numVar));
}

masterType::masterType() {

}

masterType::masterType(PowerSystem sys, int numVar) {

	candidNa = vector<vector<double>>(sys.numScenarios, vector<double>(numVar));
	incumbNa = vector<vector<double>>(sys.numScenarios, vector<double>(numVar));
	candidEst = 0;
	incumbEst = 0;
}

void addMasterVariables(PowerSystem sys, masterType::MasterProblem &M) {
	char elemName[NAMESIZE];

	/* Cut */
	sprintf_s(elemName, "Cut");
	M.nu = IloNumVar(M.env, -999999, 0, ILOFLOAT);
	M.nu.setName(elemName);

	for (int s = 0; s < sys.numScenarios; s++){
		int numVar = 0;
#if defined (FULL_NA)
		numVar = sys.numGenerators + sys.numLoads + sys.numLines + sys.numBuses;
#else
		numVar = sys.numGenerators + sys.numLoads;
#endif
		M.naDual[s] = IloNumVarArray(M.env, numVar);
		for (int i = 0; i < M.naDual[s].getSize(); i++) {

			sprintf_s(elemName, "naDual[%d][%d]", i, s+1);
			M.naDual[s][i] = IloNumVar(M.env, -IloInfinity, IloInfinity, ILOFLOAT);
			M.naDual[s][i].setName(elemName);
		}
		M.model.add(M.naDual[s][s]);
	}

}//END addMasterVariables()

void addMasterConstraints(PowerSystem sys, masterType::MasterProblem &M) {
	char elemName[NAMESIZE];

	M.expectation = IloRangeArray(M.env, M.naDual[0].getSize());

	for (int i = 0; i < M.naDual[0].getSize(); i++) {

		IloExpr expr(M.env);
		for (int s = 0; s < sys.numScenarios; s++) {
			expr += M.naDual[s][i] * sys.scenarios[s].probability;
		}

		sprintf_s(elemName, "Expectation[%d]", i);
		IloRange tempCon(M.env, 0, expr, 0, elemName);
		M.expectation[i] = tempCon;

		expr.end();
	}
	M.model.add(M.expectation);

}//END addMasterConstraints()

void addMasterObjective(PowerSystem sys, masterType::MasterProblem &M, double sigma) {

	IloExpr expr(M.env);
	expr = M.obj.getExpr();

	M.model.remove(M.obj);

	expr += M.nu;

	for (int s = 0; s < sys.numScenarios; s++){
		for (int i = 0; i < M.naDual[s].getSize(); i++) {
			expr -= (sigma / 2) * (M.naDual[s][i]) * (M.naDual[s][i]);
		}
	}
	

	M.obj.setExpr(expr);
	M.model.add(M.obj);
	expr.end();

}//END addMasterObjective()

void addMasterCut(masterType &M, PowerSystem sys, oneCut cut) {

	M.cuts.push_back(cut);

	IloExpr expr(M.prob.env);

	expr = M.prob.nu;

	for (int s = 0; s < sys.numScenarios; s++) {
		for (int i = 0; i < cut.beta[s].size(); i++) {
			expr += cut.beta[s][i] * M.prob.naDual[s][i];
		}
	}

	IloRange tempCon(M.prob.env, -IloInfinity, expr, cut.alpha, cut.name);
	M.prob.model.add(tempCon);
	expr.end();

}//END addMasterCut()

void updateMasterObjective(PowerSystem sys, masterType::MasterProblem &M, double sigma, vector<vector<double>> dual) {

	IloExpr expr(M.env);
	M.model.remove(M.obj);

	expr = M.nu;

	for (int s = 0; s < sys.numScenarios; s++) {
		for (int i = 0; i < M.naDual[s].getSize(); i++) {
			expr -= (sigma / 2) * (M.naDual[s][i] - dual[s][i]) * (M.naDual[s][i] - dual[s][i]);
		}
	}

	M.obj.setExpr(expr);
	M.model.add(M.obj);
	expr.end();

}//END updateMasterObjective()