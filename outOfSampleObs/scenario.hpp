/*
 * scenario.hpp
 *
 *  Created on: Dec 2, 2017
 *      Author: Harsha Gangammanavar
 * Institution: Southern Methodist University
 *
 * Please send your comments or bug report to harsha (at) smu (dot) edu
 *
 */

#ifndef SCENARIO_HPP_
#define SCENARIO_HPP_

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

enum nodeType {
	ROOT,
	LEAF,
	INTER
};

/* This structure has the information about the random variables. */
struct OmegaType {
	int 					numOmega;	/* number of stochastic elements stored in structure */
	vector<int> 			numVals;	/* number of realization for each random variable */
	vector<vector<double>>	vals;		/* indexed array of discrete realizations of random variable */
	vector<double>			probs;		/* indexed array of probabilities associated with discrete realizations*/
	vector<double> 			mean;		/* mean of each rv */
	vector<int>				ref;		/* reference scenario: null indicates no reference */
	vector<int>				splitNode;	/* stage/node where scenario splits from the reference scenario. */
};

/* Structure for the scenario tree */
//class ScenarioTree {
//public:
//	ScenarioTree(string inputDir, string sysName, int T, int numOmega);
//	ScenarioTree(int timePeriod, int sim, int T, int numOmega, CSVcontent scenarioData);
//	~ScenarioTree();
//
//	/* Structure carries information about the nodes on a scenario tree. */
//	struct oneNode {
//		int 			id;				// ID
//		int				stage;			// Stage to which the node belongs to.
//		nodeType		type;			// Type of the node: root, leaf, intermediate
//		double			prob;			// Probability of the node.
//		vector<double> 	vals;			// Observation associated with the node.
//		int 			parent;			// Parent node (for root node parent = -1)
//		vector<int> 	children;		// Set of children node (for leaf nodes this is empty)
//		vector<int>		scenThruNode;	// Set of scenarios through the node
//	};
//	struct oneScenario {
//		vector<double> 	vals;		// Observation of all random variables starting after the root node until the end of horizon.
//		double	prob;				// Probability of the scenario.
//	};
//
//	int 				numStages;	// Number of stages
//	int					numNodes;	// Number of nodes
//	int					numRV;		// Number of random variables per stage (assumed to be the same in each stage).
//	int					numScenarios;	// Totol number of scenarios
//
//	vector<oneNode> 	nodes;		// Vector of all nodes
//	vector<vector<int>> stageNodes;	// List of all nodes in a particular stage
//
//	vector<oneScenario> scenarios;	// Vector of all scenarios
//
//	vector<vector<double>> meanScenario;	// Mean scn
//
//private:
//
//	OmegaType omega;
//	void computeMeanScenario();
//	void printScenarioTree();
//};
//
//vector<vector<vector<double>>> generateAllScenarios(ScenarioTree tree);

#endif /* SCENARIO_HPP_ */
