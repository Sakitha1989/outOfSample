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
	IloArray<IloNumVarArray> NA_genDual, NA_demDual;
	IloNumVar nu;

	IloRangeArray expectation;

	IloObjective obj;
};

/* main.cpp */
void parseCmdLine(int argc, const char *argv[], string &inputDir, string &sysName);
void printHelpMenu();

/* subproblemSetup.cpp */
void addVariables(PowerSystem sys, ClearingModel &M);
void addConstraints(PowerSystem sys, ClearingModel &M);
void detObjective(PowerSystem sys, ClearingModel &M, IloNumArray NA_genDual, IloNumArray NA_demDual);
void stocObjective(PowerSystem sys, ClearingModel &M);
void updateObjective(PowerSystem sys, ClearingModel &M, IloNumArray NA_genDual, IloNumArray NA_demDual, int scen);

/* masterproblemSetup */
void addMasterVariables(PowerSystem sys, MasterProblem &M);
void addMasterConstraints(PowerSystem sys, MasterProblem &M);
void addMasterObjective(PowerSystem sys, MasterProblem &M, double sigma);

/* outOfSampleObs.cpp */
void outOfSampleAlg(PowerSystem sys, string inputDir, ofstream &output_file, double incumbent_deviation, double dual_deviation, int iteration_num, int incumbent_index);
vector<vector<double>> calculateBeeta(vector<vector<double>> beeta, PowerSystem sys, solution soln, int scen);
vector<vector<double>> calculateAlpha(vector<vector<double>> beeta, ClearingModel subproblemModel, PowerSystem sys, solution soln, int scen, IloNumArray NA_genDual, IloNumArray NA_demDual);

/* models.cpp */
void subproblem(PowerSystem sys, ClearingModel &M, IloNumArray NA_genDual, IloNumArray NA_demDual);
void masterproblem(PowerSystem sys, MasterProblem &M, double sigma);
solution getSolution(ClearingModel M, PowerSystem sys);
vector<double> getPrimal(IloCplex cplex, IloNumVarArray x);
vector<double> getDual(IloCplex cplex, IloRangeArray c);

#endif /* STOCCLEARING_HPP_ */
