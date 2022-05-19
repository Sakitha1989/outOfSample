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
#define FULL_NA
#define ALG_CHECK

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

class oneCut {
public:
	oneCut(PowerSystem sys, int numVar);

	double					alpha;
	vector<vector<double>>	beta;
	int						k;
	char					name[NAMESIZE];
};

class masterType {
public:
	masterType();
	masterType(PowerSystem sys, int numVar);

	class MasterProblem {
	public:
		MasterProblem();
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

	MasterProblem			prob;
	vector<oneCut>			cuts;
	double					candidEst; //nu
	double					incumbEst;
	int						incumbCut;
	double					obj;
	vector<vector<double>>	candidNa; //xi
	vector<vector<double>>	incumbNa; //bar{xi}

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

/* main.cpp */
void parseCmdLine(int argc, const char *argv[], string &inputDir, string &sysName);
void printHelpMenu();

/* subproblemSetup.cpp */
void addVariables(PowerSystem sys, ClearingModel &M);
void addConstraints(PowerSystem sys, ClearingModel &M);
void detObjective(PowerSystem sys, ClearingModel &M, vector<double> naDuals);
void stocObjective(PowerSystem sys, ClearingModel &M);
void updateSubproblem(PowerSystem sys, ClearingModel &M, vector<double> naDuals, int scen);

/* masterproblemSetup */
void addMasterVariables(PowerSystem sys, masterType::MasterProblem &M);
void addMasterConstraints(PowerSystem sys, masterType::MasterProblem &M);
void addMasterObjective(PowerSystem sys, masterType::MasterProblem &M, double sigma);
void addMasterCut(masterType &M, PowerSystem sys, oneCut cut);
void updateMasterObjective(PowerSystem sys, masterType::MasterProblem &M, double sigma, vector<vector<double>> dual);
void updateMasterObjective(PowerSystem sys, masterType::MasterProblem &M, double sigma, vector<vector<double>> dual);

/* outOfSampleObs.cpp */
void outOfSampleAlg(PowerSystem sys, string inputDir, ofstream &output_file, double incumbent_deviation, double dual_deviation);
double calculateAlpha(PowerSystem sys, solution soln, int scen);
void calculateBeta(vector<double> &beeta, PowerSystem sys, solution soln, int scen);
double minCutHeight(masterType M, vector<vector<double>> naDuals);
double cutHeight(oneCut cut, vector<vector<double>> naDuals);
bool incumbentUpdate(double gamma, masterType &master, int k);
void computeIncumbent(vector<vector<double>> candidNa, vector<vector<double>> &incumbNa, PowerSystem sys);
vector<double> meanDual(vector<vector<double>> vec, PowerSystem sys);

/* models.cpp */
void subproblem(PowerSystem sys, ClearingModel &M, vector<double> naDuals);
void masterproblem(PowerSystem sys, masterType::MasterProblem &M, double sigma);
solution getSolution(ClearingModel M, PowerSystem sys);
void masterGetSolution(masterType &M, PowerSystem sys);
vector<double> getPrimal(IloCplex cplex, IloNumVarArray x);
vector<double> getDual(IloCplex cplex, IloRangeArray c);

#endif /* STOCCLEARING_HPP_ */
