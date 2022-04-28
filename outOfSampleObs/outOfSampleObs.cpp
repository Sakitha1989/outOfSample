/*
 * outOfSampleObs.cpp
 *
 *  Created on: Apr 19, 2022
 *      Author: Sakitha Ariyarathne
 * Institution: Southern Methodist University
 *
 */

#include "model.hpp"

/* Algorithm */
void outOfSampleAlg(PowerSystem sys, string inputDir, ofstream &output_file, double incumbent_deviation, double dual_deviation) {

	ClearingModel subproblemModel(sys);
	MasterProblem master(sys);
	int numSVvariables = sys.numGenerators + sys.numLoads;

	vector<vector<double>> candidateNaDuals(sys.numScenarios); //scen_index, var_indx
	vector<vector<double>> incumbentNaDuals(sys.numScenarios);

	for (int s = 0; s < sys.numScenarios; s++) {
		candidateNaDuals[s] = vector<double>(numSVvariables);
		incumbentNaDuals[s] = vector<double>(numSVvariables);
		for (int i = 0; i < (numSVvariables); i++) {
			candidateNaDuals[s][i] = 0;
			incumbentNaDuals[s][i] = 0;
		}
	}

	subproblem(sys, subproblemModel, candidateNaDuals[0]);
	masterproblem(sys, master, dual_deviation);
	
	vector<vector<double>> alpha;								//iter_index, scen_indx
	vector<vector<vector<Beeta>>> beeta;						//iter_index, scen_indx, var_indx

	for (int k = 0; k < 5; k++) {
		masterSolution mSoln;
		vector<double> currentAlpha(sys.numScenarios);
		vector<vector<Beeta>> currentBeeta(sys.numScenarios);

		for (int s = 0; s < sys.numScenarios; s++) {

			solution soln;

			updateSubproblemObjective(sys, subproblemModel, candidateNaDuals[s], s);

			/* solve the model and obtain solutions */
			subproblemModel.solve();
			soln = getSolution(subproblemModel, sys);

			calculateAlpha(currentAlpha[s], subproblemModel, sys, soln, s, candidateNaDuals[s]);
			calculateBeeta(currentBeeta[s], sys, soln, s);
		}

		alpha.push_back(currentAlpha);
		beeta.push_back(currentBeeta);

		addMasterCut(master, sys, currentAlpha, currentBeeta, k);

		incumbentUpdate(incumbent_deviation, alpha, beeta, candidateNaDuals, incumbentNaDuals, k);

		updateMasterObjective(sys, master, dual_deviation, incumbentNaDuals);
		master.exportModel(sys, "updateMaster.lp");
		master.solve();
		mSoln = masterGetSolution(master, sys);

		candidateNaDuals = mSoln.x.naDuals;
	}
}// END outOfSampleAlg()

/* calculate alpha for scenario "scne" */
void calculateAlpha(double &alpha, ClearingModel M, PowerSystem sys, solution soln, int scen, vector<double> naDuals) {

	double expr = 0;
	for (int g = 0; g < sys.numGenerators; g++){
		expr += naDuals[g] * soln.x.DAgen[g];
	}
	for (int d = 0; d < sys.numLoads; d++){
		expr += naDuals[sys.numGenerators + d] * soln.x.DAdem[d];
	}

	alpha = sys.scenarios[scen].probability * (soln.obj + expr);

}// END calculateAlpha()

/* calculate beeta for scenario "scne" */
void calculateBeeta(vector<Beeta> &beeta, PowerSystem sys, solution soln, int scen) {

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

} // END calculateBeeta()

/* function nu */
double evaluateNu(vector<vector<double>> alpha, vector<vector<vector<Beeta>>> beeta, vector<vector<double>> naDuals, int it_count) {

	double incumbent = IloInfinity;
	double temp;

	for (int i = 0; i < it_count + 1; i++){
		temp = 0;
		for (int s = 0; s < naDuals.size(); s++){			// loop through the scenarios
			temp += alpha[it_count][s];
			for (int n = 0; n < naDuals[s].size(); n++){	// loop through the dual variables
				temp += beeta[it_count][s][n].value * naDuals[s][n];
			}
		}

		if (temp < incumbent)										// minimum
		{
			incumbent = temp;
		}
	}

	return incumbent;
}// END evaluateNu()


void incumbentUpdate(double gamma, vector<vector<double>> alpha, vector<vector<vector<Beeta>>> beeta, vector<vector<double>> candidateNaDuals, vector<vector<double>> &incumbentNaDuals, int it_count) {

	vector<double> nu(4);

	nu[0] = evaluateNu(alpha, beeta, candidateNaDuals, it_count);
	nu[1] = evaluateNu(alpha, beeta, xiBar(incumbentNaDuals), it_count);
	nu[2] = evaluateNu(alpha, beeta, candidateNaDuals, it_count - 1);
	nu[3] = evaluateNu(alpha, beeta, xiBar(incumbentNaDuals), it_count - 1);

	if (nu[0] - nu[1] >= gamma * (nu[2] - nu[3])){
		incumbentNaDuals = candidateNaDuals;
	}
}

vector<vector<double>> xiBar(vector<vector<double>> incumbentNaDual) {

	vector<double> mean = meanDual(incumbentNaDual);

	vector<vector<double>> meanDev(incumbentNaDual.size());

	for (int s = 0; s < incumbentNaDual.size(); s++) {

		meanDev[s] = vector<double>(incumbentNaDual[s].size());
		for (int i = 0; i < incumbentNaDual[s].size(); i++){
			meanDev[s][i] = incumbentNaDual[s][i] - mean[i];
		}
	}
	return meanDev;
}

/* calculate the mean scenario duals */
vector<double> meanDual(vector<vector<double>> dual) {

	vector<double> mean(dual[0].size());
	for (int i = 0; i < mean.size(); i++) { // loop through the dual variables
		mean[i] = 0;
		for (int s = 0; s < dual.size(); s++) { // loop through the scenarios
			mean[i] += dual[s][i];
		}
		mean[i] = mean[i] / dual.size();
	}
	return mean;
}// END meanDual()