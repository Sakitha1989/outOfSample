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

	bool incumbUpdate = false;
	int numSVvariables = sys.numGenerators + sys.numLoads;
#if defined (FULL_NA)
	numSVvariables += sys.numLines + sys.numBuses;
#endif

	ClearingModel subproblemModel(sys);
	masterType master(sys, numSVvariables);

	for (int s = 0; s < master.candidNa.size(); s++) {
		for (int i = 0; i < master.candidNa[s].size(); i++) {
			master.candidNa[s][i] = 0;
			master.incumbNa[s][i] = 0;
		}
	}

	subproblem(sys, subproblemModel, master.candidNa[0]);
	masterproblem(sys, master.prob, dual_deviation);
	
	int k = 0;
	while (k < 100) {

		oneCut currentCut(sys, numSVvariables);

		//double x = 0;

		for (int s = 0; s < sys.numScenarios; s++) {

			solution soln;

			updateSubproblem(sys, subproblemModel, master.candidNa[s], s);
			subproblemModel.exportModel(sys, "sub.lp");

			/* solve the model and obtain solutions */
			subproblemModel.solve();
			soln = getSolution(subproblemModel, sys);
			//x += sys.scenarios[s].probability * soln.obj;

			currentCut.alpha += calculateAlpha(sys, soln, s);
			calculateBeta(currentCut.beta[s], sys, soln, s);
		}

#if defined (ALG_CHECK)
		cout << "Iteration-" << k << "::" << endl;

		/*cout << "Obj: " << x << endl;
		x = 0;

		for (int s = 0; s < sys.numScenarios; s++) {
			for (int i = 0; i < numSVvariables; i++) {
				x -= currentCut.beta[s][i] * master.candidNa[s][i];
			}
		}

		x += currentCut.alpha;

		cout << "Cut: " << x << endl;*/
#else
		if (k % 10 == 0)
		{
			cout << endl;
			cout << "Iteration-" << k << "::";
		}
#endif

		currentCut.k = k;
		sprintf_s(currentCut.name, "Cut[%d]", k);

		addMasterCut(master, sys, currentCut);
		
		if (k >= 0){
			incumbUpdate = incumbentUpdate(incumbent_deviation, master, k);
		}
		

		if (incumbUpdate){
			updateMasterObjective(sys, master.prob, dual_deviation, master.incumbNa);
		}
		
		master.prob.exportModel(sys, "updateMaster.lp");

		master.prob.solve();
		masterGetSolution(master, sys);

		k++;
	}
}// END outOfSampleAlg()

/* calculate alpha for scenario "scne" */
double calculateAlpha(PowerSystem sys, solution soln, int scen) {

	double alpha = 0;

	for (int g = 0; g < sys.numGenerators; g++) {
		alpha += sys.genDA_bids[g].price * soln.x.RTgen[g] + sys.genRT_bids[g].priceP * soln.x.RTetaGenP[g] - sys.genRT_bids[g].priceM * soln.x.RTetaGenM[g];
	}
	for (int d = 0; d < sys.numLoads; d++) {
		alpha -= sys.demDA_bids[d].price * soln.x.RTdem[d] + sys.demRT_bids[d].priceP * soln.x.RTetaDemM[d] - sys.demRT_bids[d].priceM * soln.x.RTetaDemP[d];
	}

	return alpha * sys.scenarios[scen].probability;

}// END calculateAlpha()

/* calculate beeta for scenario "scne" */
void calculateBeta(vector<double> &beta, PowerSystem sys, solution soln, int scen) {

	int offSet = 0;

	for (int g = 0; g < sys.numGenerators; g++){
		beta[g + offSet] = sys.scenarios[scen].probability * soln.x.DAgen[g];
	}
	offSet += sys.numGenerators;
	for (int d = 0; d < sys.numLoads; d++){
		beta[d + offSet] = sys.scenarios[scen].probability * soln.x.DAdem[d];
	}
	offSet += sys.numLoads;
#if defined (FULL_NA)
	for (int l = 0; l < sys.numLines; l++) {
		beta[l + offSet] = sys.scenarios[scen].probability * soln.x.DAflow[l];
	}
	offSet += sys.numLines;
	for (int b = 0; b < sys.numBuses; b++) {
		beta[b + offSet] = sys.scenarios[scen].probability * soln.x.DAtheta[b];
	}
#endif
} // END calculateBeeta()

/* Minimum cut height calculation */
double minCutHeight(masterType M, vector<vector<double>> naDuals) {

	double incumbent = IloInfinity;
	double temp;

	for (int i = 0; i < M.cuts.size(); i++) {
		temp = cutHeight(M.cuts[i], naDuals);

		if (temp < incumbent){			//minimum
			incumbent = temp;
		}
	}

	return incumbent;
}// END minCutHeight()

/* Cut height calculation */
double cutHeight(oneCut cut, vector<vector<double>> naDuals) {

	double height = cut.alpha;

	for (int s = 0; s < cut.beta.size(); s++) {			// loop through the scenarios
		for (int n = 0; n < cut.beta[s].size(); n++) {	// loop through the dual variables
			height -= cut.beta[s][n] * naDuals[s][n];
		}
	}

	return height;
}// END cutHeight()

/* update the incumbent solution */
bool incumbentUpdate(double gamma, masterType &master, int current_it){

	bool incumbUpdate = false;
	double tempCandid;
	double tempIncumb;

	tempCandid = min(master.candidEst, cutHeight(master.cuts[current_it], master.candidNa));
	tempIncumb = min(master.incumbEst, cutHeight(master.cuts[current_it], master.incumbNa));

#if defined (ALG_CHECK)

	cout << "Incumbent = " << master.incumbEst << "\tCandidate = " << master.candidEst << ",\tIncumbent Temp" << tempIncumb << ",\tCandidate Temp" << tempCandid;
#endif

	if (tempCandid - tempIncumb >= gamma * (master.candidEst - master.incumbEst)) {
		master.incumbCut = current_it;
		master.incumbEst = tempCandid;
		//master.incumbEst = minCutHeight(master, master.candidNa);
		//computeIncumbent(master.candidNa, master.incumbNa);
		master.incumbNa = master.candidNa;
		
		incumbUpdate = true;
		cout << "+"; 
		fflush(stdout);
	}
	else {
		master.incumbEst = tempIncumb; //******************************************************************************** why do we need to update incumbent if the condition fails?
	}
	cout << endl;

	return incumbUpdate;
}// END incumbentUpdate()

/* function nu */
void computeIncumbent(vector<vector<double>> candidNa, vector<vector<double>> &incumbNa, PowerSystem sys) {

	vector<double> mean = meanDual(candidNa, sys);

	for (int s = 0; s < candidNa.size(); s++) {

		incumbNa[s] = vector<double>(candidNa[s].size());
		for (int i = 0; i < candidNa[s].size(); i++){
			incumbNa[s][i] = candidNa[s][i] - mean[i];
		}
	}
}// END xiBar()

/* calculate the mean scenario duals */
vector<double> meanDual(vector<vector<double>> vec, PowerSystem sys) {

	vector<double> mean(vec[0].size());
	for (int i = 0; i < mean.size(); i++) { // loop through the dual variables
		mean[i] = 0;
		for (int s = 0; s < vec.size(); s++) { // loop through the scenarios
			mean[i] += vec[s][i] * sys.scenarios[s].probability;
		}
		cout << "mean " << mean[i] << endl;
	}
	return mean;
}// END meanDual()