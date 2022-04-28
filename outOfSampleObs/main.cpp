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

	double incumbent_deviation = 0.9;
	double dual_deviation = 2;

	outOfSampleAlg(ps, inputDir, output_file, incumbent_deviation, dual_deviation);

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

