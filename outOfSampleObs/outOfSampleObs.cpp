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
	vector<solution> soln;

	IloNumArray NA_genDual(subproblemModel.env, sys.numGenerators);
	IloNumArray NA_demDual(subproblemModel.env, sys.numLoads);

	for (int i = 0; i < (sys.numGenerators); i++)
	{
		NA_genDual[i] = 0;
	}
	for (int i = 0; i < (sys.numLoads); i++)
	{
		NA_demDual[i] = 0;
	}

	subproblem(sys, subproblemModel, NA_genDual, NA_demDual);

	vector<vector<double>> beeta(2, vector<double>(sys.numScenarios));
	vector<vector<double>> alpha(2, vector<double>(sys.numScenarios));

	for (int s = 0; s < sys.numScenarios; s++) {

		updateObjective(sys, subproblemModel, NA_genDual, NA_demDual, s);

		/* solve the model and obtain solutions */
		subproblemModel.solve();
		soln.push_back(getSolution(subproblemModel, sys));

		beeta = calculateBeeta(beeta, sys, soln[s], s);
		alpha = calculateAlpha(alpha, subproblemModel, sys, soln[s], s, NA_genDual, NA_demDual);
	}



}

vector<vector<double>> calculateBeeta(vector<vector<double>> beeta, PowerSystem sys, solution soln, int scen) {

	for (int g = 0; g < sys.numGenerators; g++)
	{
		beeta[0][scen] += sys.scenarios[scen].probability * soln.x.DAgen[g];
		beeta[0][scen] += sys.scenarios[scen].probability * soln.x.RTgen[g];
		beeta[0][scen] += sys.scenarios[scen].probability * soln.x.RTetaGenM[g];
		beeta[0][scen] += sys.scenarios[scen].probability * soln.x.RTetaGenP[g];
	}
	for (int d = 0; d < sys.numLoads; d++)
	{
		beeta[0][scen] += sys.scenarios[scen].probability * soln.x.DAdem[d];
		beeta[0][scen] += sys.scenarios[scen].probability * soln.x.RTdem[d];
		beeta[0][scen] += sys.scenarios[scen].probability * soln.x.RTetaDemM[d];
		beeta[0][scen] += sys.scenarios[scen].probability * soln.x.RTetaDemP[d];
	}
	for (int l = 0; l < sys.numLines; l++)
	{
		beeta[0][scen] += sys.scenarios[scen].probability * soln.x.DAflow[l];
		beeta[0][scen] += sys.scenarios[scen].probability * soln.x.RTflow[l];
	}
	for (int b = 0; b < sys.numBuses; b++)
	{
		beeta[0][scen] += sys.scenarios[scen].probability * soln.x.DAtheta[b];
		beeta[0][scen] += sys.scenarios[scen].probability * soln.x.RTtheta[b];
	}

	return beeta;
}

vector<vector<double>> calculateAlpha(vector<vector<double>> alpha, ClearingModel M, PowerSystem sys, solution soln, int scen, IloNumArray NA_genDual, IloNumArray NA_demDual) {

	double expr = 0;

	for (int g = 0; g < sys.numGenerators; g++)
	{
		expr += NA_genDual[g] * soln.x.DAgen[g];
	}
	for (int d = 0; d < sys.numLoads; d++)
	{
		expr += NA_demDual[d] * soln.x.DAdem[d];
	}

	alpha[0][scen] = sys.scenarios[scen].probability * (soln.obj + expr);

	return alpha;

}