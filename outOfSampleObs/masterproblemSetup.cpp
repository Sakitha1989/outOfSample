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
	M.nu = IloNumVar(M.env, ILOFLOAT);
	M.nu.setName(elemName);

	for (int s = 0; s < sys.numScenarios; s++){
		M.NA_genDual[s] = IloNumVarArray(M.env, sys.numGenerators);
		for (int g = 0; g < sys.numGenerators; g++) {

			sprintf_s(elemName, "NA_genDual[%d][%d]", s, sys.generators[g].id);
			M.NA_genDual[s][g] = IloNumVar(M.env, ILOFLOAT);
			M.NA_genDual[s][g].setName(elemName);
		}
		M.model.add(M.NA_genDual[s]);

		/* Demand */
		M.NA_demDual = IloArray<IloNumVarArray>(M.env, sys.numLoads);
		for (int d = 0; d < sys.numLoads; d++) {

			sprintf_s(elemName, "NA_demDual[%d][%d]", s, sys.loads[d].id);
			M.NA_demDual[s][d] = IloNumVar(M.env, ILOFLOAT);
			M.NA_demDual[s][d].setName(elemName);
		}
		M.model.add(M.NA_demDual[s]);
	}

}//END addMasterVariables()

void addMasterConstraints(PowerSystem sys, MasterProblem &M) {
	char elemName[NAMESIZE];

	/* Flow balance equation */
	M.expectation = IloRangeArray(M.env, sys.numBuses);

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
		M.expectation[d] = tempCon;

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