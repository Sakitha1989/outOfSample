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
		M.NA_genDual[s] = IloNumVarArray(M.env, sys.numGenerators);
		for (int g = 0; g < sys.numGenerators; g++) {

			sprintf_s(elemName, "NA_genDual[%d][%d]", sys.generators[g].id, s+1);
			M.NA_genDual[s][g] = IloNumVar(M.env, -IloInfinity, IloInfinity, ILOFLOAT);
			M.NA_genDual[s][g].setName(elemName);
		}
		M.model.add(M.NA_genDual[s]);

		/* Demand */
		M.NA_demDual[s] = IloNumVarArray(M.env, sys.numLoads);
		for (int d = 0; d < sys.numLoads; d++) {

			sprintf_s(elemName, "NA_demDual[%d][%d]", sys.loads[d].id, s+1);
			M.NA_demDual[s][d] = IloNumVar(M.env, -IloInfinity, IloInfinity, ILOFLOAT);
			M.NA_demDual[s][d].setName(elemName);
		}
		M.model.add(M.NA_demDual[s]);
	}

}//END addMasterVariables()

void addMasterConstraints(PowerSystem sys, MasterProblem &M) {
	char elemName[NAMESIZE];

	M.expectation = IloRangeArray(M.env, sys.numGenerators + sys.numLoads);

	for (int g = 0; g < sys.numGenerators; g++) {

		IloExpr expr(M.env);
		for (int s = 0; s < sys.numScenarios; s++) {
			expr += M.NA_genDual[s][g] * sys.scenarios[s].probability;
		}

		sprintf_s(elemName, "Expectation[%d]", sys.generators[g].id);
		IloRange tempCon(M.env, 0, expr, 0, elemName);
		M.expectation[g] = tempCon;

		expr.end();
	}

	for (int d = 0; d < sys.numLoads; d++) {

		IloExpr expr(M.env);
		for (int s = 0; s < sys.numScenarios; s++) {
			expr += M.NA_demDual[s][d] * sys.scenarios[s].probability;
		}

		sprintf_s(elemName, "Expectation[%d]", sys.numGenerators + sys.loads[d].id);
		IloRange tempCon(M.env, 0, expr, 0, elemName);
		M.expectation[sys.numGenerators + d] = tempCon;

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
		for (int g = 0; g < sys.numGenerators; g++) {
			expr -= (sigma / 2) * (M.NA_genDual[s][g]) * (M.NA_genDual[s][g]);
		}
		for (int d = 0; d < sys.numLoads; d++) {
			expr -= (sigma / 2) * (M.NA_demDual[s][d]) * (M.NA_demDual[s][d]);
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

	for (int s = 0; s < sys.numScenarios; s++)
	{
		expr += alpha[s];

		for (int i = 0; i < beeta[s].size(); i++) {
			if (beeta[s][i].type == Gen) {
				expr += beeta[s][i].value * M.NA_genDual[s][beeta[s][i].ID - 1];
			}
			else if (beeta[s][i].type == Dem) {
				expr += beeta[s][i].value * M.NA_demDual[s][beeta[s][i].ID - 1];
			}
		}
	}
	IloRange tempCon(M.env, -IloInfinity, expr, 0, elemName);
	M.cut.add(tempCon);
	M.model.add(M.cut);
	expr.end();

}//END detObjective()