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

	int numSVvariables = sys.numGenerators + sys.numLoads;
	bool incumbUpdate = false;
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
#if defined (ALG_CHECK)
			cout << "Iteration-" << k << "::";
#else
		if (k%10 == 0)
		{
			cout << endl;
			cout << "Iteration-" << k << "::";
		}
#endif
		oneCut currentCut(sys, numSVvariables);

		for (int s = 0; s < sys.numScenarios; s++) {

			solution soln;

			updateSubproblemObjective(sys, subproblemModel, master.candidNa[s], s);
			subproblemModel.exportModel(sys, "sub.lp");

			/* solve the model and obtain solutions */
			subproblemModel.solve();
			soln = getSolution(subproblemModel, sys);

			currentCut.alpha += calculateAlpha(subproblemModel, sys, soln, s, master.candidNa[s]);
			calculateBeta(currentCut.beta[s], sys, soln, s);
		}
		currentCut.k = k;
		sprintf_s(currentCut.name, "Cut[%d]", k);

		addMasterCut(master, sys, currentCut);
		
		if (k > 0){
			incumbUpdate = incumbentUpdate(incumbent_deviation, master, k);
		}
		

		if (incumbUpdate){
			updateMasterObjective(sys, master.prob, dual_deviation, master.incumbNa);
			master.prob.exportModel(sys, "updateMaster.lp");
		}
		
		master.prob.solve();
		masterGetSolution(master, sys);

		k++;
	}
}// END outOfSampleAlg()

/* calculate alpha for scenario "scne" */
double calculateAlpha(ClearingModel M, PowerSystem sys, solution soln, int scen, vector<double> naDuals) {

	double expr = 0;
	for (int g = 0; g < sys.numGenerators; g++){
		expr += naDuals[g] * soln.x.DAgen[g];
	}
	for (int d = 0; d < sys.numLoads; d++){
		expr += naDuals[sys.numGenerators + d] * soln.x.DAdem[d];
	}
#if defined (FULL_NA)
	for (int l = 0; l < sys.numLines; l++)
	{
		expr += naDuals[sys.numGenerators + sys.numLoads + l] * soln.x.DAflow[l];
	}
	for (int b = 0; b < sys.numBuses; b++)
	{
		expr += naDuals[sys.numGenerators + sys.numLoads + sys.numLines + b] * soln.x.DAtheta[b];
	}
#endif

	double alpha = sys.scenarios[scen].probability * (soln.obj + expr);
	return alpha;

}// END calculateAlpha()

/* calculate beeta for scenario "scne" */
void calculateBeta(vector<double> &beta, PowerSystem sys, solution soln, int scen) {

	int offSet = 0;

	for (int g = 0; g < sys.numGenerators; g++){
		//double tempBeta;
		//tempBeeta.ID = sys.generators[g].id;
		//tempBeeta.scenario = scen;
		//tempBeeta.type = Gen;
		beta[g + offSet] = sys.scenarios[scen].probability * soln.x.DAgen[g];

		//beta[g+1] = tempBeta;
	}
	offSet += sys.numGenerators;
	for (int d = 0; d < sys.numLoads; d++){
		//double tempBeta;
		//tempBeeta.ID = sys.loads[d].id;
		//tempBeeta.scenario = scen;
		//tempBeeta.type = Dem;
		beta[d + offSet] = sys.scenarios[scen].probability * soln.x.DAdem[d];

		//beta[sys.numGenerators + d +1] =  tempBeta;
	}
	offSet += sys.numLoads;
#if defined (FULL_NA)
	for (int l = 0; l < sys.numLines; l++) {
		//double tempBeta;
		/*tempBeeta.ID = sys.lines[l].id;
		tempBeeta.scenario = scen;*/
		beta[l + offSet] = sys.scenarios[scen].probability * soln.x.DAflow[l];

		//beeta.push_back(tempBeta);
	}
	offSet += sys.numLines;
	for (int b = 0; b < sys.numBuses; b++) {
		//double tempBeta;
		/*tempBeeta.ID = sys.buses[b].id;
		tempBeeta.scenario = scen;*/
		beta[b + offSet] = sys.scenarios[scen].probability * soln.x.DAtheta[b];

		//beeta.push_back(tempBeta);
	}
#endif

} // END calculateBeeta()

/* Minimum cut height calculation */
double minCutHeight(masterType M) {

	double incumbent = IloInfinity;
	double temp;

	for (int i = 0; i < M.cuts.size(); i++) {
		temp = cutHeight(M.cuts[i], M.candidNa);

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
			height += cut.beta[s][n] * naDuals[s][n];
		}
	}

	return height;
}// END cutHeight()

/* update the incumbent solution */
bool incumbentUpdate(double gamma, masterType &master, int current_it){

	bool incumbUpdate = false;
	double tempCandid;
	double tempIncumb;

	tempCandid = minCutHeight(master);
	if (master.incumbCut == current_it - 1){

	}
	//tempCandid = min(master.candidEst, cutHeight(master.cuts[current_it], master.candidNa));
	tempIncumb = min(master.incumbEst, cutHeight(master.cuts[current_it], master.incumbNa));

#if defined (ALG_CHECK)
	cout << "Incumbent = " << master.incumbEst << "\tCandidate = " << master.candidEst << ",\tIncumbent Temp" << tempIncumb << ",\tCandidate Temp" << tempCandid << endl;
#endif

	if (tempCandid - tempIncumb >= gamma * (master.candidEst - master.incumbEst)) {
		master.incumbCut = current_it;
		master.incumbEst = minCutHeight(master);
		computeIncumbent(master.candidNa, master.incumbNa);
		
		incumbUpdate = true;
		cout << "+"; fflush(stdout);
	}
	else {
		master.incumbEst = tempIncumb;
	}

	return incumbUpdate;
}// END incumbentUpdate()

/* function nu */
void computeIncumbent(vector<vector<double>> candidNa, vector<vector<double>> &incumbNa) {

	vector<double> mean = meanDual(candidNa);

	for (int s = 0; s < candidNa.size(); s++) {

		incumbNa[s] = vector<double>(candidNa[s].size());
		for (int i = 0; i < candidNa[s].size(); i++){
			incumbNa[s][i] = candidNa[s][i] - mean[i];
		}
	}
}// END xiBar()

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