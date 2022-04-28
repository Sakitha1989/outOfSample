/*
 * setup.cpp
 *
 *  Created on: Apr 22, 2022
 *      Author: Sakitha Ariyarathne
 * Institution: Southern Methodist University
 *
 */

#include "model.hpp"

void addMasterVariables(PowerSystem sys, MasterProblem &M) {
	char elemName[NAMESIZE];

	/* Cut */
	sprintf_s(elemName, "Cut");
	M.nu = IloNumVar(M.env, -IloInfinity, IloInfinity, ILOFLOAT);
	M.nu.setName(elemName);

	for (int s = 0; s < sys.numScenarios; s++){
		M.naDual[s] = IloNumVarArray(M.env, sys.numGenerators + sys.numLoads);
		for (int i = 0; i < M.naDual[s].getSize(); i++) {

			sprintf_s(elemName, "naDual[%d][%d]", i, s+1);
			M.naDual[s][i] = IloNumVar(M.env, -IloInfinity, IloInfinity, ILOFLOAT);
			M.naDual[s][i].setName(elemName);
		}
		M.model.add(M.naDual[s][s]);
	}

}//END addMasterVariables()

void addMasterConstraints(PowerSystem sys, MasterProblem &M) {
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

void addMasterObjective(PowerSystem sys, MasterProblem &M, double sigma) {

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

void addMasterCut(MasterProblem &M, PowerSystem sys, vector<double> alpha, vector<vector<Beeta>> beeta, int it_count) {
	char elemName[NAMESIZE];

	M.cut = IloRangeArray(M.env);
	IloExpr expr(M.env);
	sprintf_s(elemName, "Cut[%d]", it_count);

	expr = M.nu;

	for (int s = 0; s < sys.numScenarios; s++){
		expr += alpha[s];

		for (int i = 0; i < beeta[s].size(); i++) {
			expr += beeta[s][i].value * M.naDual[s][i];
		}
	}
	IloRange tempCon(M.env, -IloInfinity, expr, 0, elemName);
	M.cut.add(tempCon);
	M.model.add(M.cut);
	expr.end();

}//END addMasterCut()

void updateMasterObjective(PowerSystem sys, MasterProblem &M, double sigma, vector<vector<double>> dual) {

	IloExpr expr(M.env);
	expr = M.obj.getExpr();

	M.model.remove(M.obj);

	expr += M.nu;

	for (int s = 0; s < sys.numScenarios; s++) {
		for (int i = 0; i < M.naDual[s].getSize(); i++) {
			expr -= (sigma / 2) * (M.naDual[s][i] - dual[s][i]) * (M.naDual[s][i] - dual[s][i]);
		}
	}


	M.obj.setExpr(expr);
	M.model.add(M.obj);
	expr.end();

}//END updateMasterObjective()