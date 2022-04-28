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

	IloArray<IloArray<IloNumArray>> NA_genDual(subproblemModel.env, 4);
	IloArray<IloArray<IloNumArray>> NA_demDual(subproblemModel.env, 4);

	for (int i = 0; i < 2; i++){
		NA_genDual[i] = IloArray<IloNumArray>(subproblemModel.env, sys.numScenarios);
		NA_demDual[i] = IloArray<IloNumArray>(subproblemModel.env, sys.numScenarios);
		NA_genDual[i+2] = IloArray<IloNumArray>(subproblemModel.env, 1);
		NA_demDual[i+2] = IloArray<IloNumArray>(subproblemModel.env, 1);
	}

	for (int s = 0; s < sys.numScenarios; s++){
		NA_genDual[0][s] = IloNumArray(subproblemModel.env, sys.numGenerators);
		NA_demDual[0][s] = IloNumArray(subproblemModel.env, sys.numLoads);
		for (int i = 0; i < (sys.numGenerators); i++){
			NA_genDual[0][s][i] = 0;
		}
		for (int i = 0; i < (sys.numLoads); i++){
			NA_demDual[0][s][i] = 0;
		}
	}

	subproblem(sys, subproblemModel, NA_genDual[0][0], NA_demDual[0][0]);
	masterproblem(sys, master, dual_deviation);

	vector<double> currentAlpha(sys.numScenarios);
	vector<vector<Beeta>> currentBeeta(sys.numScenarios);
	
	vector<vector<double>> alpha;
	vector<vector<vector<Beeta>>> beeta;
	int incumbent;

	for (int k = 0; k < 2; k++) {

		NA_genDual[2][0] = meanDual(master, NA_genDual[0]);
		NA_demDual[2][0] = meanDual(master, NA_demDual[0]);

		for (int i = 0; i < NA_genDual[2][0].getSize(); i++)
		{
			cout << NA_genDual[2][0][i] << endl;
		}
		for (int i = 0; i < NA_demDual[2][0].getSize(); i++)
		{
			cout << NA_demDual[2][0][i] << endl;
		}

		for (int s = 0; s < sys.numScenarios; s++) {

			updateSubproblemObjective(sys, subproblemModel, NA_genDual[0][s], NA_demDual[0][s], s);

			/* solve the model and obtain solutions */
			subproblemModel.solve();
			soln.push_back(getSolution(subproblemModel, sys));

			currentAlpha[s] = calculateAlpha(currentAlpha[s], subproblemModel, sys, soln[s], s, NA_genDual[0][s], NA_demDual[0][s], k);
			currentBeeta[s] = calculateBeeta(currentBeeta[s], sys, soln[s], s, k);
		}

		alpha.push_back(currentAlpha);
		beeta.push_back(currentBeeta);

		addMasterCut(master, sys, currentAlpha, currentBeeta, k);
		master.exportModel(sys, "model.lp");

		if (k > 0){
			incumbent = incumbentUpdate(incumbent_deviation,alpha, beeta, NA_genDual, NA_demDual, k);
		}

		NA_genDual[1] = NA_genDual[0];
		NA_demDual[1] = NA_demDual[0];
		NA_genDual[3] = NA_genDual[2];
		NA_demDual[3] = NA_demDual[2];

	}
}

IloNumArray meanDual(MasterProblem M, IloArray<IloNumArray> dual) {

	IloNumArray mean(M.env);
	for (int i = 0; i < dual[0].getSize(); i++){
		IloNum temp = 0;
		for (int s = 0; s < dual.getSize(); s++){
			temp += dual[s][i];
		}
		mean.add(temp / dual.getSize());
	}
	return mean;
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

double evaluateNu(vector<vector<double>> alpha, vector<vector<vector<Beeta>>> beeta, IloArray<IloNumArray> gen, IloArray<IloNumArray> dem, int it_count, bool mean) {

	double pre_temp = IloInfinity;
	double temp;
	double incumbent;
	int index;

	cout << beeta[it_count].size() << endl;

	for (int i = 0; i < it_count + 1; i++){
		temp = 0;
		for (int s = 0; s < beeta[it_count].size(); s++){
			if (mean) index = 0;
			else index = s;
			temp += alpha[it_count][s];
			for (int n = 0; n < beeta[it_count][s].size(); n++){
				if (beeta[it_count][s][n].type == Gen) {
					temp += beeta[it_count][s][n].value * gen[index][beeta[it_count][s][n].ID - 1];
				}
				else  if (beeta[it_count][s][n].type == Dem) {
					temp += beeta[it_count][s][n].value * dem[index][beeta[it_count][s][n].ID - 1];
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

int incumbentUpdate(double gamma, vector<vector<double>> alpha, vector<vector<vector<Beeta>>> beeta, IloArray<IloArray<IloNumArray>> gen, IloArray<IloArray<IloNumArray>> dem, int it_count) {

	vector<double> evals(4);

	evals[0] = evaluateNu(alpha, beeta, gen[0], dem[0], it_count, false);
	evals[1] = evaluateNu(alpha, beeta, gen[3], dem[3], it_count, true);
	evals[2] = evaluateNu(alpha, beeta, gen[0], dem[0], it_count - 1, false);
	evals[3] = evaluateNu(alpha, beeta, gen[3], dem[3], it_count - 1, true);

	if (evals[0] - evals[1] >= gamma * (evals[2] - evals[3])){
		return it_count;
	}
	return it_count -1;
}