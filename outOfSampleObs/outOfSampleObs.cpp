/*
 * outOfSampleObs.cpp
 *
 *  Created on: Apr 19, 2022
 *      Author: Sakitha Ariyarathne
 * Institution: Southern Methodist University
 *
 */

#include "model.hpp"


void outOfSampleAlg(PowerSystem sys, string inputDir, ofstream &output_file, double incumbent_deviation, double dual_deviation, int iteration_num, int incumbent_index) {

	ClearingModel subproblemModel(sys);
	MasterProblem master(sys);
	vector<solution> soln;

	IloArray<IloNumArray> NA_genDual(subproblemModel.env, sys.numScenarios);
	IloArray<IloNumArray> NA_demDual(subproblemModel.env, sys.numScenarios);

	for (int s = 0; s < sys.numScenarios; s++){
		NA_genDual[s] = IloNumArray(subproblemModel.env, sys.numGenerators);
		NA_demDual[s] = IloNumArray(subproblemModel.env, sys.numLoads);
		for (int i = 0; i < (sys.numGenerators); i++){
			NA_genDual[s][i] = 0;
		}
		for (int i = 0; i < (sys.numLoads); i++){
			NA_demDual[s][i] = 0;
		}
	}

	subproblem(sys, subproblemModel, NA_genDual[0], NA_demDual[0]);
	masterproblem(sys, master, dual_deviation);

	vector<double> currentAlpha(sys.numScenarios);
	vector<vector<Beeta>> currentBeeta(sys.numScenarios);
	
	vector<vector<double>> alpha;
	vector<vector<vector<Beeta>>> beeta;
	double incumbent;

	for (int k = 0; k < 2; k++) {

		for (int s = 0; s < sys.numScenarios; s++) {

			updateSubproblemObjective(sys, subproblemModel, NA_genDual[s], NA_demDual[s], s);

			/* solve the model and obtain solutions */
			subproblemModel.solve();
			soln.push_back(getSolution(subproblemModel, sys));

			currentAlpha[s] = calculateAlpha(currentAlpha[s], subproblemModel, sys, soln[s], s, NA_genDual[s], NA_demDual[s], k);
			currentBeeta[s] = calculateBeeta(currentBeeta[s], sys, soln[s], s, k);
		}

		alpha.push_back(currentAlpha);
		beeta.push_back(currentBeeta);

		addMasterCut(master, sys, currentAlpha, currentBeeta, k);
		master.exportModel(sys, "model.lp");


		incumbent = evaluateNu(sys, alpha, beeta, NA_genDual, NA_demDual, k);

		/*if (k > 0){

		}*/


	}
}

double calculateAlpha(double alpha, ClearingModel M, PowerSystem sys, solution soln, int scen, IloNumArray NA_genDual, IloNumArray NA_demDual, int it_count) {

	double expr = 0;
	for (int g = 0; g < sys.numGenerators; g++){
		expr += NA_genDual[g] * soln.x.DAgen[g];
	}
	for (int d = 0; d < sys.numLoads; d++){
		expr += NA_demDual[d] * soln.x.DAdem[d];
	}

	alpha = sys.scenarios[scen].probability * (soln.obj + expr);

	return alpha;
}

vector<Beeta> calculateBeeta(vector<Beeta> beeta, PowerSystem sys, solution soln, int scen, int it_count) {

	for (int g = 0; g < sys.numGenerators; g++){
		Beeta tempBeeta;
		tempBeeta.ID = sys.generators[g].id;
		tempBeeta.scenario = scen;
		tempBeeta.type = Gen;
		tempBeeta.value = sys.scenarios[scen].probability * soln.x.DAgen[g];

		beeta.push_back(tempBeeta);
	}
	for (int d = 0; d < sys.numLoads; d++){
		Beeta tempBeeta;
		tempBeeta.ID = sys.loads[d].id;
		tempBeeta.scenario = scen;
		tempBeeta.type = Dem;
		tempBeeta.value = sys.scenarios[scen].probability * soln.x.DAdem[d];

		beeta.push_back(tempBeeta);
	}

	return beeta;
}

double evaluateNu(PowerSystem sys, vector<vector<double>> alpha, vector<vector<vector<Beeta>>> beeta, IloArray<IloNumArray> gen, IloArray<IloNumArray> dem, int it_count) {

	double pre_temp = IloInfinity;
	double temp;
	double incumbent;

	for (int i = 0; i < it_count; i++){
		temp = 0;
		for (int s = 0; s < sys.numScenarios; s++){
			temp += alpha[it_count][s];
			for (int n = 0; n < beeta[it_count][s].size(); n++){
				if (beeta[it_count][s][n].type == Gen) {
					temp += beeta[it_count][s][n].value * gen[s][beeta[it_count][s][n].ID - 1];
				}
				else  if (beeta[it_count][s][n].type == Dem) {
					temp += beeta[it_count][s][n].value * dem[s][beeta[it_count][s][n].ID - 1];
				}
			}
		}

		if (temp < pre_temp)
		{
			incumbent = temp;
		}
	}

	return incumbent;
}