/*
 * stocClearing.hpp
 *
 *  Created on: Dec 4, 2017
 *      Author: Harsha Gangammanavar
 * Institution: Southern Methodist University
 *
 * Please send your comments or bug report to harsha (at) smu (dot) edu
 *
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

#include "PowerSystem.hpp"
#include "scenario.hpp"
#include "utilities.hpp"
#include "outOfSampleObs.hpp"

#include <ilcplex/ilocplex.h>

using namespace std;

#define NAMESIZE 32

#define SAVE_FILES

// Objective
#undef PAYMENT_MODEL /*  */
#undef PENALTY_MODEL /* all deviations are penalized */
#define RT_PAYMENT_MODEL /* Only real-time payments considered along with etas */

// Flow balance
#undef RT_FLOW /* real-time flow balance equation is setup using RT variables */
#define DIFF_FLOW /* real-time flow balance equation is setup as the difference between the RT and DA quantities */
#undef DIFF_ETA /* real-time flow balance equation is setup using eta variables */

#undef PENALTY_MODEL /* penalty objective function. every devation is penalized */

struct onePrimal {
	vector<vector<vector<double>>> DAgen;
	vector<vector<vector<double>>> DAdem;
	vector<vector<vector<double>>> DAflow;
	vector<vector<vector<double>>> DAtheta;
	vector<vector<vector<double>>> RTgen;
	vector<vector<vector<double>>> RTx;
	vector<vector<vector<double>>> RTdem;
	vector<vector<vector<double>>> RTflow;
	vector<vector<vector<double>>> RTtheta;
	vector<vector<vector<double>>> RTetaP;
	vector<vector<vector<double>>> RTetaM;
};

struct oneDual {
	vector<vector<vector<double>>> DAflowBalance;
	vector<vector<vector<double>>> DAdc;
	vector<vector<vector<double>>> DAru;
	vector<vector<vector<double>>> DAgbu;
	vector<vector<vector<double>>> DAgbd;
	vector<vector<vector<double>>> DAdbu;
	vector<vector<vector<double>>> DAdbd;

	vector<vector<vector<double>>> 	naConst;
	vector<vector<vector<double>>> 	RTflowBalance;
	vector<vector<vector<double>>> 	RTdc;
	vector<vector<vector<double>>> 	RTru;
	vector<vector<vector<double>>> 	RTgbu;
	vector<vector<vector<double>>> 	RTgbd;
	vector<vector<vector<double>>> 	RTdbu;
	vector<vector<vector<double>>> 	RTdbd;
	vector<vector<vector<double>>> 	RTdgP;
	vector<vector<vector<double>>> 	RTdgM;
	vector<vector<vector<double>>> 	RTddP;
	vector<vector<vector<double>>> 	RTddM;
	vector<vector<vector<double>>> 	RTga;
	vector<vector<vector<double>>> 	RTda;
};

struct solnType {
	double	  obj;
	onePrimal x;
	oneDual   pi;
	vector<double> rd;
};

class ClearingModel {
public:
	ClearingModel(PowerSystem sys);

	bool solve(PowerSystem sys, string type);
	void exportModel(string fname);

	string name;

	IloEnv		env;
	IloModel	model;
	IloCplex	cplex;

	/* Decision variables, constraints and random variables */
	IloArray<IloNumVarArray> DAgen, DAdem, DAflow, DAtheta;
	IloArray<IloNumVarArray> RTgen, RTdem, RTflow, RTtheta, RTetaP, RTetaM;

	IloArray<IloRangeArray> DAflowBalance, DAdcApproximation, DArefAngle;
	IloArray<IloRangeArray> RTflowBalance, RTdcApproximation, RTrefAngle, genPositive, genNegative, demPositive, demNegative, stochGen, inflexGen;

	IloObjective obj;

private:
	vector<vector<vector<double>>> getSoln(int N, int T, int L, IloArray < IloArray <IloNumVarArray> > x);
};

/* main.cpp */
void parseCmdLine(int argc, const char *argv[], string &inputDir, string &sysName);
void printHelpMenu();
CSVcontent readScenario(string inputDir, string sysName, bool readFile);

/* setup.cpp */
//void detVariables(PowerSystem sys, ClearingModel &M, int scen);
//void detConstraints(PowerSystem sys, ClearingModel &M, int scen);
//void detObjective(PowerSystem sys, ClearingModel &M, double weight, int scen);
//void detObjective_expanded(PowerSystem sys, ClearingModel &M, double weight, int scen);
//void stocVariables(PowerSystem sys, ClearingModel &M, int scen);
//void stocConstraints(PowerSystem sys, ClearingModel &M, int scen, int parentScen, vector<vector<double>> observ);
//void stocObjective(PowerSystem sys, ClearingModel &M, double weight, int scen);
//void stocObjective_expanded(PowerSystem sys, ClearingModel &M, double weight, int scen);
//void nonanticipativeMeanVector(PowerSystem sys, ClearingModel &M, ScenarioTree tree);
//void nonanticipativeStateVector(PowerSystem sys, ClearingModel &M, ScenarioTree tree);
//
///* models.cpp */
//ClearingModel setupDA(PowerSystem sys, ScenarioTree tree, string inputDir);
//void reviseDA(PowerSystem sys, ScenarioTree tree, ClearingModel daModel, int proNum, CSVcontent scenarioData, solnType soln, bool solext, int scen);
//solnType sloveDA(PowerSystem sys, ScenarioTree tree, ClearingModel daModel, ofstream &output_file, int scen);
//
//void formDAproblem(PowerSystem sys, ScenarioTree tree, string inputDir, ofstream &output_file);
//void scenarioProblem(PowerSystem sys, ClearingModel &scenModel, vector<vector<double>> observ, double prob, int scenID, int parentID);
//void meanProblem(PowerSystem sys, vector<vector<double>> observ, ScenarioTree tree, string inputDir, ofstream &output_file);
//void evalProblem(PowerSystem sys, ScenarioTree tree, int scen, string inputDir, ofstream &output_file);
//void waitNsee(PowerSystem sys, ScenarioTree tree, string inputDir, ofstream &output_file);
//void twoSLProblem(PowerSystem sys, ScenarioTree tree, string inputDir, ofstream &output_file, int i);
//void twoSLProblem_expanded(PowerSystem sys, ScenarioTree tree, string inputDir, ofstream &output_file);
//void slpProblemMeanVector(PowerSystem sys, ScenarioTree tree, string inputDir, ofstream &output_file);
//void slpProblemStateVector(PowerSystem sys, ScenarioTree tree, string inputDir, ofstream &output_file);
//
///* solution.cpp */
//void duals(solnType soln, ClearingModel M);
//
//solnType getSolution(ClearingModel M, PowerSystem sys, bool isTwoStage, int numScenarios);
//
//void printSolution(ClearingModel M, PowerSystem sys, ScenarioTree tree, bool isTwoSatge, ofstream &output_file);
//void printPrimalSolutions(ClearingModel M, PowerSystem sys, ScenarioTree tree, bool status, bool redeucedCost, bool isTwoStage, ofstream &output_file);
//void printDualSolutions(ClearingModel M, PowerSystem sys, ScenarioTree tree, bool isTwoStage, ofstream &output_file);
//
//void printHeader(PowerSystem sys, string obj, ofstream &output_file);
//
//void printGenNames(PowerSystem sys, ofstream &output_file);
//void printLoadNames(PowerSystem sys, ofstream &output_file);
//void printLineNames(PowerSystem sys, ofstream &output_file);
//void printBusNames(PowerSystem sys, ofstream &output_file);
//
//void printGeneration(ClearingModel M, PowerSystem sys, int scen, bool status, bool redeucedCost, bool isTwoStage, ofstream &output_file);
//void printLoad(ClearingModel M, PowerSystem sys, int scen, bool status, bool redeucedCost, bool isTwoStage, ofstream &output_file);
//void printLine(ClearingModel M, PowerSystem sys, int scen, bool status, bool redeucedCost, bool isTwoStage, ofstream &output_file);
//void printAngle(ClearingModel M, PowerSystem sys, int scen, bool status, bool redeucedCost, bool isTwoStage, ofstream &output_file);
//
//void printFlowBalance(ClearingModel M, PowerSystem sys, int scen, bool isTwoStage, ofstream &output_file);
//void printInflex(ClearingModel M, PowerSystem sys, int scen, bool isTwoStage, ofstream &output_file);
//
//void getRTNegativeAuxSolutions(ClearingModel M, PowerSystem sys, ScenarioTree tree, ofstream &output_file);
//void getRTPositiveAuxSolutions(ClearingModel M, PowerSystem sys, ScenarioTree tree, ofstream &output_file);
//
//void printNAGen(ClearingModel M, PowerSystem sys, ScenarioTree tree, ofstream &output_file);
//void printNALoad(ClearingModel M, PowerSystem sys, ScenarioTree tree, ofstream &output_file);
//
//void getRTbidPSolStatus(ClearingModel M, PowerSystem sys, bool printHeader, int numScenarios, ScenarioTree tree);
//void getRTbidMSolStatus(ClearingModel M, PowerSystem sys, bool printHeader, int numScenarios, ScenarioTree tree);
//void getRTSolStatus(ClearingModel M, PowerSystem sys, int numScenarios);
//
//void printHeaders(PowerSystem sys, ofstream &output_file);
//void printNegAuxDuals(string M, PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree, ofstream &output_file);
//void printPosAuxDuals(string M, PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree, ofstream &output_file);
//void printdcDuals(string modelName, PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree);
//void printruDuals(ClearingModel M, PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree);
//void printgbuDuals(ClearingModel M, PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree);
//void printgbdDuals(ClearingModel M, PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree);
//void printdbuDuals(ClearingModel M, PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree);
//void printdgDuals(ClearingModel M, PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree);
//void printddDuals(ClearingModel M, PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree);
//void printDABoundDuals(ClearingModel M, PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree);
//void printRTBoundDuals(ClearingModel M, PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree);
//void printgaDuals(ClearingModel M, PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree);
//void printGeneration(PowerSystem sys, solnType soln, bool isTwoStage, int scen, bool printHeader, ScenarioTree tree, ofstream &output_file);
//
//vector<vector<double>> getPrimal(IloCplex cplex, IloArray <IloNumVarArray> x);
//vector<vector<double>> getDual(IloCplex cplex, IloArray <IloRangeArray> c);
//vector<vector<double>> getDual(IloCplex cplex, IloArray <IloRangeArray> c);

#endif /* STOCCLEARING_HPP_ */
