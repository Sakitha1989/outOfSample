//
//  PowSys.hpp
//  Stoch-UC
//
//  Created by Semih Atakan on 12/14/17.
//  Copyright © 2017 University of Southern California. All rights reserved.
//

#ifndef PowSys_hpp
#define PowSys_hpp

#include "utilities.hpp"

const double pi = 3.1451;

using namespace std;

class Generator;
class Load;

class Bus {

public:
	Bus() = default;

	// bus identifiers
	int 	id = 0;			// assigned by us
	string  name;

	// bus characteristics
	int     regionId = 0;

	double  busVmax = 0.0;
	double  busVmin = 0.0;				/* Voltage limits */
	double  maxPhaseAngle = pi;
	double  minPhaseAngle = -pi;	/* Limits on bus phase angle */
	double  latitude = 0.0;
	double  longitude = 0.0;			/* Location of the bus */

	vector<Generator*> 	connectedGenerators;			/* List of all generators connected to the bus */
	vector<Load*> 		connectedLoads;					/* List of all loads connected to the bus */
};

class Line {

public:
	Line() = default;

	// line identifiers
	int 	id = 0;		// assigned by us
	string 	name;

	// line characteristics
	double 	minCap = 0.0;
	double  maxCap = 0.0;  /* (MWh) Minimum and maximum capacity of the line */
	int 	circuit = 0;				 /* Circuit associated with the line */
	double 	resistance = 0.0;			 /* line resistance */
	double 	reactance = 0.0;			 /* line reactance */
	double 	susceptance = 0.0;			 /* line susceptance */

	Bus *source;  						/* origin bus */
	Bus *destination;  					/* destination bus */
};

class Generator {

public:
	Generator();

	// generator identifiers
	int id;			// assigned by us
	string name;

	// generator characteristics
	Bus     *bus;      					// bus that this generator is connected to

	double startupCost = 0.0;      		// $ / startup cost
	double noLoadCost;          		// $ / no load cost
	double variableCost;        		// $ / MWh production cost
	double quadCost;					// the quadratic part of generation cost

	double Pmax = 0.0;
	double Pmin = 0.0;    				/* MW: real part of maximum and minimum generation capacity of the unit */
	double Qmax = 0.0;
	double Qmin = 0.0; 					/* MW: imaginary part of maximum and minimum generation capacity of the unit. */

	double startupPmax = 0.0;			/* MW: the maximum startup generation limit */
	double shutdownPmin = 0.0;			/* MW: the minimum shut down generation limit */

	int minUpTime = 0;
	int minDownTime = 0;				/* hours: minimum up-time and down-time of the generator */

	double rampUpLim = 0.0;
	double rampDownLim = 0.0;			/* MW /hour: ramp-up and down limits */

	double CO2_emission_base;			// kg / hour CO2 (incurred, whenever a generator is operational)
	double CO2_emission_var;			// kg / MWh CO2 (incurred per production)

	double NOX_emission_base;			// kg / hour NOX (incurred, whenever a generator is operational)
	double NOX_emission_var;			// kg / MWh NOX (incurred per production)

	double SO2_emission_base;			// kg / hour SO2 (incurred, whenever a generator is operational)
	double SO2_emission_var;			// kg / MWh SO2 (incurred per production)

	int initState;						/* Initial state of the generator: 0 if OFF or non-zero value indicates how long it is ON. */

	enum GeneratorType {
		CONVENTIONAL,
		BIOMASS,
		NATURALGAS,
		OIL,
		GEOTHERMAL,
		HYDRO,
		COAL,
		SOLAR,
		WIND,
		THERMAL,
		OTHER
	};

	GeneratorType type;
	dataType data;
	int tsColumn;						/* If a time series is associated with this generator, this indicates its column */
};

class Load {

public:
	Load() = default;

	// load identifier
	int id;
	string name;

	// load characteristics
	Bus    *bus;				/* the bus where the load is connected */
	double Pd = 0.0;			/* real part of the demand at the load */
	double Qd = 0.0;			/* imaginary part of the demand at the load */

	dataType data;				/* Classifies the demand at this load as deterministic or stochastic. */

};

struct DAbid {
	int		ID;
	double	price;
};

struct RTbid {
	int		ID;
	double	priceM;
	double	priceP;
};

class Scenario {

public:
	Scenario() = default;

	int				ID;
	double			probability;
	vector<double>	ScenarioOutputs;
};

class PowerSystem {

public:
	PowerSystem();
	PowerSystem(string, string);

	string name;

	int numBuses;
	int numLines;
	int numGenerators;
	int numLoads;
	int numDAgenBids;
	int numDAdemBids;
	int numRTgenBids;
	int numRTdemBids;
	int numScenarios;
	int numInflexGens;

	int numRandomVariables = 0;

	double VOLL = 1000.0;
	double baseKV = 100;

	vector<Generator>   generators;			/* Vector of all generators in the system */
	vector<Bus>         buses;				/* Vector of all buses in the system */
	vector<Line>        lines;				/* Vector of all lines in the system */
	vector<Load>		loads;				/* Vector of all loads in the system */
	vector<DAbid>       genDA_bids;
	vector<DAbid>       demDA_bids;
	vector<RTbid>       genRT_bids;
	vector<RTbid>       demRT_bids;
	vector<Scenario>    scenarios;

private:
	bool readPSData(string inputDir, string sysName);
	bool readGeneratorData(string filePath);
	bool readBusData(string filePath);
	bool readLineData(string filePath);
	bool readLoadData(string filePath);
	bool readDA_bids(string filePath);
	bool readRT_bids(string filePath);
	bool readScenarioData(string filePath);

	void printSystemSummary();

	// helpers
	map<int, int> mapBusID2Index;
};

#endif /* PowSys_hpp */
