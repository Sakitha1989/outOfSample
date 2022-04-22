/*
 * outOfSampleObs.hpp
 *
 *  Created on: Apr 19, 2022
 *      Author: Sakitha Ariyarathne
 * Institution: Southern Methodist University
 *
 */

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

#include <ilcplex/ilocplex.h>

void outOfSampleAlg(PowerSystem sys, CSVcontent scenarioData, string inputDir, ofstream &output_file, double incumbent_deviation, double dual_deviation, int iteration_num, int incumbent_index);
//void subproblem(PowerSystem sys, ScenarioTree tree, vector<double> NA_dual, string inputDir, ofstream &output_file);
//void createSubproblem(PowerSystem sys, ClearingModel &scenModel, vector<vector<double>> observ, vector<double> NA_dual, double prob, int scenID, int parentID);
//void lagrangian(PowerSystem sys, ClearingModel &M, double weight, int scen, vector<double> NA_dual);

void subproblem(PowerSystem sys, vector<double> NA_dual, string inputDir, ofstream &output_file);