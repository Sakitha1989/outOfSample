/*
 * stocClearing.hpp
 *
 *  Created on: Dec 4, 2017
 *      Author: Harsha Gangammanavar
 * Institution: Southern Methodist University
 *
 * Please send your comments or bug report to harsha (at) smu (dot) edu
 *
 *  Updated on: Apr 21, 2022
 *          by: Sakitha Ariyarathne
 */

#ifndef STOCCLEARING_HPP_
#define STOCCLEARING_HPP_

#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <cmath>
#include <numeric>

#include "PowerSystem.hpp"
#include "utilities.hpp"

#include <ilcplex/ilocplex.h>

using namespace std;

#define NAMESIZE 32

struct onePrimal {
	vector<double> DAgen;
	vector<double> DAdem;
	vector<double> DAflow;
	vector<double> DAtheta;

	vector<double> RTgen;
	vector<double> RTdem;
	vector<double> RTflow;
	vector<double> RTtheta;
	vector<double> RTetaGenP;
	vector<double> RTetaGenM;
	vector<double> RTetaDemP;
	vector<double> RTetaDemM;
};

struct oneDual {
	vector<double> DAflowBalance;
	vector<double> DAdcApproximation;
	vector<double> DArefAngle;

	vector<double> 	RTflowBalance;
	vector<double> 	RTdcApproximation;
	vector<double> 	RTrefAngle;
	vector<double> 	genPositive;
	vector<double> 	genNegative;
	vector<double> 	demPositive;
	vector<double> 	demNegative;
	vector<double> 	inflexGen;
};

struct solution {
	double	  obj;
	onePrimal x;
	oneDual   pi;
};

struct masterPrimal {
	double nu;
	vector<vector<double>> naDuals;

};

struct masterSolution {
	double	  obj;
	masterPrimal x;
};

class ClearingModel {
public:
	ClearingModel(PowerSystem sys);

	bool solve();
	void exportModel(PowerSystem sys, string fname);

	string name;

	IloEnv		env;
	IloModel	model;
	IloCplex	cplex;

	/* Decision variables, constraints and random variables */
	IloNumVarArray DAgen, DAdem, DAflow, DAtheta;
	IloNumVarArray RTgen, RTdem, RTflow, RTtheta, RTetaGenP, RTetaGenM, RTetaDemP, RTetaDemM;

	IloRangeArray DAflowBalance, DAdcApproximation, DArefAngle;
	IloRangeArray RTflowBalance, RTdcApproximation, RTrefAngle, genPositive, genNegative, demPositive, demNegative, inflexGen;
	IloRangeArray cut;

	IloObjective obj;
};

class MasterProblem {
public:
	MasterProblem(PowerSystem sys);

	bool solve();
	void exportModel(PowerSystem sys, string fname);

	string name;

	IloEnv		env;
	IloModel	model;
	IloCplex	cplex;

	/* Decision variables, constraints and random variables */
	IloArray<IloNumVarArray> naDual;
	IloNumVar nu;

	IloRangeArray expectation, cut;

	IloObjective obj;
};

enum NAtype {
	Gen,
	Dem
};

struct Beeta {
	int		ID;
	int		scenario;
	NAtype	type;
	double  value;
};

/* main.cpp */
void parseCmdLine(int argc, const char *argv[], string &inputDir, string &sysName);
void printHelpMenu();

/* subproblemSetup.cpp */
void addVariables(PowerSystem sys, ClearingModel &M);
void addConstraints(PowerSystem sys, ClearingModel &M);
void detObjective(PowerSystem sys, ClearingModel &M, vector<double> naDuals);
void stocObjective(PowerSystem sys, ClearingModel &M);
void updateSubproblemObjective(PowerSystem sys, ClearingModel &M, vector<double> naDuals, int scen);

/* masterproblemSetup */
void addMasterVariables(PowerSystem sys, MasterProblem &M);
void addMasterConstraints(PowerSystem sys, MasterProblem &M);
void addMasterObjective(PowerSystem sys, MasterProblem &M, double sigma);
void addMasterCut(MasterProblem &M, PowerSystem sys, vector<double> alpha, vector<vector<Beeta>> beeta, int it_count);
void updateMasterObjective(PowerSystem sys, MasterProblem &M, double sigma, vector<vector<double>> dual);
void updateMasterObjective(PowerSystem sys, MasterProblem &M, double sigma, vector<vector<double>> dual);

/* outOfSampleObs.cpp */
void outOfSampleAlg(PowerSystem sys, string inputDir, ofstream &output_file, double incumbent_deviation, double dual_deviation);
void calculateAlpha(double &alpha, ClearingModel subproblemModel, PowerSystem sys, solution soln, int scen, vector<double> naDuals);
void calculateBeeta(vector<Beeta> &beeta, PowerSystem sys, solution soln, int scen);
double evaluateNu(vector<vector<double>> alpha, vector<vector<vector<Beeta>>> beeta, vector<vector<double>> naDuals, int it_count);
void incumbentUpdate(double gamma, vector<vector<double>> alpha, vector<vector<vector<Beeta>>> beeta, vector<vector<double>> candidateNaDuals, vector<vector<double>> &incumbentNaDuals, int it_count);
vector<vector<double>> xiBar(vector<vector<double>> indumbentNaDuals);
vector<double> meanDual(vector<vector<double>> dual);

/* models.cpp */
void subproblem(PowerSystem sys, ClearingModel &M, vector<double> naDuals);
void masterproblem(PowerSystem sys, MasterProblem &M, double sigma);
solution getSolution(ClearingModel M, PowerSystem sys);
masterSolution masterGetSolution(MasterProblem M, PowerSystem sys);
vector<double> getPrimal(IloCplex cplex, IloNumVarArray x);
vector<double> getDual(IloCplex cplex, IloRangeArray c);

#endif /* STOCCLEARING_HPP_ */
