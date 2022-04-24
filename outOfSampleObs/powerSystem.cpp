/*
 * PowerSystem.cpp
 *
 *  Created on: Feb 23, 2019
 *      Author: Harsha Gangammanavar
 * Institution: Southern Methodist University
 *
 * Please send your comments or bug report to harsha (at) smu (dot) edu
 *
 */

#include "PowerSystem.hpp"

PowerSystem::PowerSystem() {
	numBuses = 0;
	numLines = 0;
	numGenerators = 0;
	numLoads = 0;
	numScenarios = 0;
	numRandomVariables = 0;
}

PowerSystem::PowerSystem(string inputDir, string sysName) {

	/* Initialize */
	numBuses = 0;
	numLines = 0;
	numGenerators = 0;
	numLoads = 0;
	numScenarios = 0;
	numRandomVariables = 0;

	/* Read the system data from the files */
	readPSData(inputDir, sysName);

}//END powerSystem()

bool PowerSystem::readPSData(string inputDir, string sysName) {
	bool status = true;

	name = sysName;

	printDoubleLine();
	// read bus data
	status = readBusData(inputDir + sysName);
	if (!status)
		cerr << "Error: Power system could not be read." << endl;

	// read line data
	status = readLineData(inputDir + sysName);
	if (!status)
		cerr << "Error: Power system could not be read." << endl;

	// read generator data
	status = readGeneratorData(inputDir + sysName);
	if (!status)
		cerr << "Error: Power system could not be read." << endl;

		// read load data
	status = readLoadData(inputDir + sysName);
	if (!status)
		cerr << "Error: Power system could not be read." << endl;

	// read day-ahead bid data
	status = readDA_bids(inputDir + sysName);
	if (!status)
		cerr << "Error: Power system could not be read." << endl;

	// read real-time bid data
	status = readRT_bids(inputDir + sysName);
	if (!status)
		cerr << "Error: Power system could not be read." << endl;

	// read scenario data
	status = readScenarioData(inputDir + sysName);
	if (!status)
		cerr << "Error: Power system could not be read." << endl;

	cout << "Successfully read the data for power system : " << sysName << endl;
	printSystemSummary();
	printDoubleLine();

	return status;
}//END readData()

bool PowerSystem::readBusData(string filePath) {

	cout << "Reading bus data..." << endl;
#ifdef _WIN32
	string fname = filePath + "\\busData.csv";
#else
	string fname = filePath + "/busData.csv";
#endif

	/* Read the bus data */
	CSVcontent busData(fname, ',', true);

	/* Extract information about individual buses. */
	numBuses = (const int)busData.content.size();

	vector<double> tempbusID = busData.getColumn("busID", true);
	vector<double> tempbusLat = busData.getColumn("busLat", true);
	vector<double> tempbusLong = busData.getColumn("busLong", true);

	for (int i = 0; i < numBuses; i++) {
		Bus oneBus;
		if (!tempbusID.empty()) oneBus.id = tempbusID[i];
		if (!tempbusLat.empty()) oneBus.latitude = tempbusLat[i];
		if (!tempbusLong.empty()) oneBus.longitude = tempbusLong[i];
		oneBus.name = "bus" + to_string(oneBus.id);

		mapBusID2Index.insert(pair<int, int>(oneBus.id, i));

		buses.push_back(oneBus);
	}

	return true;
}//END readGeneratorData()

bool PowerSystem::readLineData(string filePath) {

	cout << "Reading line data..." << endl;
#ifdef _WIN32
	string fname = filePath + "\\lineData.csv";
#else
	string fname = filePath + "/lineData.csv";
#endif

	/* Read the line data */
	CSVcontent lineData(fname, ',', true);

	/* Extract information about individual lines. */
	numLines = (const int)lineData.content.size();

	vector<double> templineID = lineData.getColumn("lineID", true);
	vector<double> templineSource = lineData.getColumn("lineSource", true);
	vector<double> templineDestination = lineData.getColumn("lineDestination", true);
	vector<double> templineB = lineData.getColumn("lineB", true);
	vector<double> templinePmax = lineData.getColumn("linePmax", true);

	for (int i = 0; i < numLines; i++) {
		Line oneLine;

		if (!templineID.empty()) oneLine.id = templineID[i];
		if (!templineSource.empty()) oneLine.source = &(buses[mapBusID2Index[templineSource[i]]]);
		if (!templineDestination.empty()) oneLine.destination = &(buses[mapBusID2Index[templineDestination[i]]]);
		if (!templineB.empty()) oneLine.susceptance = templineB[i];
		if (!templinePmax.empty()) {
			oneLine.maxCap = templinePmax[i];
			oneLine.minCap = -templinePmax[i];
		}
		oneLine.name = "line" + to_string(oneLine.id);
		lines.push_back(oneLine);
	}

	return true;
}//END readLineData()

bool PowerSystem::readGeneratorData(string filePath) {

	cout << "Reading generator data..." << endl;
#ifdef _WIN32
	string fname = filePath + "\\genData.csv";
#else
	string fname = filePath + "/genData.csv";
#endif

	/* Read the generator data */
	CSVcontent genData(fname, ',', true);

	/* Extract information about individual lines. */
	numGenerators = (const int)genData.content.size();

	vector<double> tempgenID = genData.getColumn("genID", true);
	vector<double> tempgenBus = genData.getColumn("genBus", true);
	vector<double> tempgenCost_a = genData.getColumn("genCost_a", true);
	vector<double> tempgenCost_b = genData.getColumn("genCost_b", true);
	vector<double> tempgenCost_c = genData.getColumn("gen_eps", true);
	vector<double> tempgenPmax = genData.getColumn("genPmax", true);
	vector<double> tempgenRamp = genData.getColumn("genRamp", true);
	vector<double> tempgenStartup = genData.getColumn("genStartup", true);
	vector<string> tempgenData = genData.getColumn("genType");
	vector<string> tempgenType = genData.getColumn("gen_flex");

	for (int i = 0; i < numGenerators; i++) {
		Generator oneGen;

		if (!tempgenID.empty()) oneGen.id = tempgenID[i];
		if (!tempgenBus.empty()) oneGen.bus = &(buses[mapBusID2Index[tempgenBus[i]]]);
		if (!tempgenPmax.empty()) oneGen.Pmax = tempgenPmax[i];
		if (!tempgenCost_b.empty()) oneGen.variableCost = tempgenCost_b[i];
		if (!tempgenCost_a.empty()) oneGen.quadCost = tempgenCost_a[i];
		if (!tempgenRamp.empty()) oneGen.rampUpLim = tempgenRamp[i];
		if (!tempgenRamp.empty()) oneGen.rampDownLim = -tempgenRamp[i];
		if (!tempgenStartup.empty()) oneGen.startupCost = tempgenStartup[i];
		if (!tempgenData.empty()) {
			if (tempgenData[i][0] == 'd' || tempgenData[i][0] == 'D' || tempgenData[i][0] == 'det')
				oneGen.data = deterministic;
			else if (tempgenData[i][0] == 's' || tempgenData[i][0] == 'S' || tempgenData[i][0] == 'sto'){
				oneGen.data = stochastic;
				numRandomVariables++;
			}
			else
				perror("Unknown data type for the generator.\n");
		}
		if (!tempgenType.empty()) {
			if (tempgenType[i][0] == 'd' || tempgenType[i][0] == 'D') {
				oneGen.type = oneGen.THERMAL;
				numInflexGens++;
			}
			else if (tempgenType[i][0] == 'f' || tempgenType[i][0] == 'F')
				oneGen.type = oneGen.OTHER;
			else
				perror("Unknown data type for the generator.\n");
		}
		oneGen.name = "gen" + to_string(oneGen.id);

		// missing data initialization
		if (!tempgenPmax.empty()) oneGen.startupPmax = tempgenPmax[i];
		if (!tempgenPmax.empty()) oneGen.shutdownPmin = tempgenPmax[i];

		generators.push_back(oneGen);
	}

	return true;
}//END readGeneratorData()

bool PowerSystem::readLoadData(string filePath) {

	cout << "Reading load data..." << endl;
#ifdef _WIN32
	string fname = filePath + "\\loadData.csv";
#else
	string fname = filePath + "/loadData.csv";
#endif

	/* Read the load data */
	CSVcontent loadData(fname, ',', true);

	/* Extract information for individual load */
	numLoads = (const int)loadData.content.size();

	vector<double> temploadID = loadData.getColumn("loadID", true);
	vector<double> temploadBus = loadData.getColumn("loadBus", true);
	vector<double> temploadPd = loadData.getColumn("loadPd", true);
	vector<string> temploadType = loadData.getColumn("loadType");

	for (int i = 0; i < numLoads; i++) {
		Load oneLoad;

		if (!temploadID.empty()) oneLoad.id = temploadID[i];
		if (!temploadBus.empty()) oneLoad.bus = &(buses[mapBusID2Index[temploadBus[i]]]);
		if (!temploadPd.empty()) oneLoad.Pd = temploadPd[i];
		if (!temploadType.empty()) {
			if (temploadType[i][0] == 'd' || temploadType[i][0] == 'D' || temploadType[i][0] == 'det')
				oneLoad.data = deterministic;
			else if (temploadType[i][0] == 's' || temploadType[i][0] == 'S' || temploadType[i][0] == 'sto')
				oneLoad.data = stochastic;
			else {
				perror("Unknown data type for load.\n");
			}
		}
		oneLoad.name = "load" + to_string(oneLoad.id);
		loads.push_back(oneLoad);
	}

	return true;
}//END readloadData()

bool PowerSystem::readDA_bids(string filePath) {

	cout << "Reading day-ahead bid data..." << endl;
#ifdef _WIN32
	string fname = filePath + "\\DAbidData.csv";
#else
	string fname = filePath + "/DAbidData.csv";
#endif

	/* Read the bid data */
	CSVcontent DAbidData(fname, ',', true);

	/* Extract information for individual bid */

	vector<string> tempbidType = DAbidData.getColumn("bidType");
	vector<double> tempbidID = DAbidData.getColumn("bidID", true);
	vector<double> tempbidPrice = DAbidData.getColumn("bidPrice", true);

	for (int i = 0; i < DAbidData.content.size(); i++) {
		DAbid Bid;

		if (!tempbidID.empty()) Bid.ID = tempbidID[i];
		if (!tempbidPrice.empty()) Bid.price = tempbidPrice[i];
		if (!tempbidType.empty() && tempbidType[i] == "gen") {

			genDA_bids.push_back(Bid);
		}
		if (!tempbidID.empty() && tempbidType[i] == "dem") {

			demDA_bids.push_back(Bid);
		}
	}

	numDAgenBids = (const int)genDA_bids.size();
	numDAdemBids = (const int)demDA_bids.size();

	return true;
}//END readDA_bids()

bool PowerSystem::readRT_bids(string filePath) {

	cout << "Reading real-time bid data..." << endl;
#ifdef _WIN32
	string fname = filePath + "\\RTbidData.csv";
#else
	string fname = filePath + "/RTbidData.csv";
#endif

	/* Read the bid data */
	CSVcontent RTbidData(fname, ',', true);

	/* Extract information for individual bid */

	vector<string> tempbidType = RTbidData.getColumn("bidType");
	vector<double> tempbidID = RTbidData.getColumn("bidID", true);
	vector<double> tempbidPriceM = RTbidData.getColumn("bidPriceM", true);
	vector<double> tempbidPriceP = RTbidData.getColumn("bidPriceP", true);

	for (int i = 0; i < RTbidData.content.size(); i++) {
		RTbid Bid;

		if (!tempbidID.empty()) Bid.ID = tempbidID[i];
		if (!tempbidPriceM.empty()) Bid.priceM = tempbidPriceM[i];
		if (!tempbidPriceP.empty()) Bid.priceP = tempbidPriceP[i];
		if (!tempbidType.empty() && tempbidType[i] == "gen") {

			genRT_bids.push_back(Bid);
		}
		if (!tempbidID.empty() && tempbidType[i] == "dem") {

			demRT_bids.push_back(Bid);
		}
	}
	numRTgenBids = (const int)genRT_bids.size();
	numRTdemBids = (const int)demRT_bids.size();

	return true;
}//END readDA_bids()

bool PowerSystem::readScenarioData(string filePath) {

	/* The scenarios are assumed to be in the scenarios.csv file within the inputDir */
	cout << "Reading scenarios..." << endl;
#ifdef _WIN32
	string fname = filePath + "\\scenario.csv";
#else
	string fname = filePath + "/scenario.csv";
#endif

	/* Read the bid data */
	CSVcontent scenarioData(fname, ',', true);

	/* Extract information for individual bid */
	numScenarios = (const int)scenarioData.content.size();

	vector<double> tempscenID = scenarioData.getColumn("scenID", true);
	vector<double> tempscenProb = scenarioData.getColumn("scenProb", true);
	vector<vector<double>> tempscenarios;
	for (int i = 0; i < numRandomVariables; i++)
	{
		tempscenarios.push_back(scenarioData.getColumn("stoch" + to_string(i+1), true));
	}

	for (int i = 0; i < numScenarios; i++) {
		Scenario scen;

		if (!tempscenID.empty()) scen.ID = tempscenID[i];
		if (!tempscenProb.empty()) scen.probability = tempscenProb[i];
		for (int j = 0; j < numRandomVariables; j++)
		{
			scen.ScenarioOutputs.push_back(tempscenarios[j][i]);
		}

		scenarios.push_back(scen);
	}
	
	return true;
}

void PowerSystem::printSystemSummary() {

	printLine();
	cout << "\tNumber of buses			= " << numBuses << endl;
	cout << "\tNumber of lines			= " << numLines << endl;
	cout << "\tNumber of generators		= " << numGenerators << endl;
	cout << "\tNumber of loads			= " << numLoads << endl;
	cout << "\tNumber of day-ahead gen bids	= " << numDAgenBids << endl;
	cout << "\tNumber of day-ahead dem bids	= " << numDAdemBids << endl;
	cout << "\tNumber of real-time gen bids	= " << numRTgenBids << endl;
	cout << "\tNumber of real-time dem bids	= " << numRTdemBids << endl;
	cout << "\tNumber of scenarios		= " << numScenarios << endl;
}

Generator::Generator() {

	id = 0;
	bus = 0;

	startupCost = noLoadCost = variableCost = quadCost = 0.0;

	Pmax = Pmin = Qmax = Qmin = 0.0;
	startupPmax = shutdownPmin = 0.0;
	rampUpLim = rampDownLim = 0.0;

	minUpTime = minDownTime = 0.0;

	CO2_emission_base = CO2_emission_var = 0.0;
	NOX_emission_base = NOX_emission_var = 0.0;
	SO2_emission_base = SO2_emission_var = 0.0;

	initState = tsColumn = 0;

	type = CONVENTIONAL;
	data = deterministic;
}
