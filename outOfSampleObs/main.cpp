/*
 * main.cpp
 *
 *  Created on: Apr 19, 2022
 *      Author: Sakitha Ariyarathne
 * Institution: Southern Methodist University
 *     Project: Out of sample observations in Stochastic market clearing problem.
 *
 */
#include "model.hpp"

string outputDir;

int main(int argc, const char *argv[]) {

	string sysName;
	string inputDir;

	ofstream output_file;

	/* Obtain instance information if not provided. */
	parseCmdLine(argc, argv, inputDir, sysName);

	/* Read data and initialize an OPF instance. */
	PowerSystem ps(inputDir, sysName);

	/* Setup scenario tree: assuming only the renewables are uncertain. */
	//CSVcontent scenarioData = readScenario(inputDir, sysName, true);
	//ScenarioTree tree(0, numScen, numStage, ps.numRenewables, scenarioData);
	//ClearingModel daModel = setupDA(ps, tree, inputDir);

	//solnType soln;

	double incumbent_deviation = 0.1;
	double dual_deviation = 2;
	int iteration_num = 10;
	int incumbent_index = 0;

	outOfSampleAlg(ps, inputDir, output_file, incumbent_deviation, dual_deviation, iteration_num, incumbent_index);

	return 0;
}//END main()

void parseCmdLine(int argc, const char *argv[], string &inputDir, string &sysName) {

	for (int i = 1; (i < argc); i++) {
		if (argv[i][0] == '-') {
			switch ((argv[i])[1]) {
			case '?': printHelpMenu(); 
				exit(0);
			case 'i': inputDir = argv[++i];
				break;
			case 'o': outputDir = argv[++i]; 
				break;
			case 'p': sysName = argv[++i]; 
				break;
			}
		}
		else {
			printf("Input options (%s) must begin with a '-'. Use '-?' for help.\n", argv[i]); exit(0);
		}
	}

	if (sysName.empty() || inputDir.empty() || outputDir.empty()) {
		printf("System name, input and output directory are mandatory input.\n");
		exit(0);
	}

	/* create an output directory within the outOfSample folder*/
string cmd = "mkdir " + outputDir;
	system(cmd.c_str());
	outputDir = outputDir + sysName + "\\";
	cmd = "mkdir " + outputDir;
	system(cmd.c_str());

}//END parseCmdLine()

/* We allow only a few of the parameters to be selected through command line input. The rest can be accessed in the configuration file. */
void printHelpMenu() {

	printf("Input options:\n");
	/* Problem name, input and output directory */
	printf("         -i string    -> input directory where the system files are saved.\n");
	printf("         -o string    -> output directory where the result files will be written.\n");
	printf("         -p string    -> power system name.\n");

	return;
}// printHelpMenu()


//ClearingModel::ClearingModel(PowerSystem sys, bool isStocModel, ScenarioTree tree) {
//
//	model = IloModel(env);
//	cplex = IloCplex(model);
//
//	/* Initialize a dummy objective function */
//	obj = IloMinimize(env, 0.0);
//	model.add(obj);
//
//	cplex.setOut(env.getNullStream());
//	cplex.setWarning(env.getNullStream());
//
//	numPeriods = 1; //*****************
//
//	for (int g = 0; g < sys.numGenerators; g++)
//	{
//		if (sys.generators[g].data == stochastic)
//		{
//			rvIdx.push_back(sys.generators[g].id);
//			rvType.push_back(GENERATION);
//		}
//	}
//
//	for (int l = 0; l < sys.numLoads; l++)
//	{
//		if (sys.loads[l].data == stochastic)
//		{
//			rvIdx.push_back(sys.loads[l].id);
//			rvType.push_back(DEMAND);
//		}
//	}
//
//}// ClearingModel constructor ()
//
//ClearingModel::~ClearingModel() {
//
//}
//
//void ClearingModel::readBids(PowerSystem sys, string filePath, string sysName, string fileType, vector<vector<oneBid>> &genBids, vector<vector<oneBid>> &demBids, vector<vector<oneBid>> &lineBids, vector<vector<oneBid>> &thetaBids) {
//
//	//cout << "Reading bid data..." << endl;
//	string fname;
//	if (fileType == "DA")
//	{
//#ifdef _WIN32
//		fname = filePath + sysName + "\\DAbidData" + ".csv";
//#else
//		string fname = filePath + sysName + "/DAbidData" + ".csv";
//#endif
//	}
//	else if (fileType == "RT")
//	{
//#ifdef _WIN32
//		fname = filePath + sysName + "\\RTbidData" + ".csv";
//#else
//		string fname = filePath + sysName + "/RTbidData" + ".csv";
//#endif
//	}
//
//
//	/* Read the generator data */
//	CSVcontent bidData(fname, ',', true);
//
//	/* Extract information about individual generator. */
//	int numBids = (const int)bidData.content.size();
//
//	vector<string> tempbidType = bidData.getColumn("bidType");
//	vector<double> tempbidID = bidData.getColumn("bidID", true);
//	vector<double> tempbidPrice = bidData.getColumn("bidPrice", true);
//	vector<double> tempbidPriceChangeM = bidData.getColumn("bidPriceM", true);
//	vector<double> tempbidPriceChangeP = bidData.getColumn("bidPriceP", true);
//
//	for (int t = 0; t < numPeriods; t++)
//	{
//		vector<oneBid> tempgenBid, tempdemBid, templineBid, tempthetaBid;
//
//		for (int i = 0; i < numBids; i++) {
//			oneBid Bid;
//			if (!tempbidID.empty()) Bid.ID = tempbidID[i];
//			if (!tempbidPrice.empty()) Bid.price = tempbidPrice[i];
//			if (!tempbidPriceChangeM.empty()) Bid.priceM = tempbidPriceChangeM[i];
//			if (!tempbidPriceChangeP.empty()) Bid.priceP = tempbidPriceChangeP[i];
//
//			if (!tempbidType.empty() && tempbidType[i] == "gen") {
//
//				tempgenBid.push_back(Bid);
//			}
//			if (!tempbidID.empty() && tempbidType[i] == "dem") {
//
//				tempdemBid.push_back(Bid);
//			}
//			if (!tempbidID.empty() && tempbidType[i] == "line") {
//
//				templineBid.push_back(Bid);
//			}
//			if (!tempbidID.empty() && tempbidType[i] == "theta") {
//
//				tempthetaBid.push_back(Bid);
//			}
//		}
//		genBids.push_back(tempgenBid);
//		demBids.push_back(tempdemBid);
//		lineBids.push_back(templineBid);
//		thetaBids.push_back(tempthetaBid);
//	}
//
//	return;
//}//END readBids()
//
//bool ClearingModel::solve(PowerSystem sys, string type) {
//
//	try {
//
//		cplex.setParam(cplex.RootAlg, cplex.Primal);
//
//		bool status = cplex.solve();
//		return status;
//	}
//	catch (IloException &e) {
//		cout << e << endl;
//		return false;
//	}
//}
//
//void ClearingModel::exportModel(string fname) {
//	cplex.exportModel(("./outputDir/" + fname).c_str());
//}

//CSVcontent readScenario(string inputDir, string sysName, bool readFile) {
//
//	/* The scenarios are assumed to be in the scenarios.csv file within the inputDir */
//	cout << "Reading scenarios..." << endl;
//#ifdef _WIN32
//	string fname = inputDir + sysName + "\\scenario.csv";
//#else
//	string fname = inputDir + sysName + "/scenario.csv";
//#endif
//	CSVcontent scenarioData(fname, ',', true);
//	return scenarioData;
//}
