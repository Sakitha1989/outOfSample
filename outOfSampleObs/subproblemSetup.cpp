/*
 * setup.cpp
 *
 *  Created on: Dec 4, 2017
 *      Author: Harsha Gangammanavar
 * Institution: Southern Methodist University
 *
 * Please send your comments or bug report to harsha (at) smu (dot) edu
 *
 *  Updated on: Apr 20, 2022
 *          By: Sakitha Ariyarathne
 */

#include "model.hpp"

void addVariables(PowerSystem sys, ClearingModel &M) {
	char elemName[NAMESIZE];

	/* Generation */
	M.DAgen = IloNumVarArray(M.env, sys.numGenerators);
	M.RTgen = IloNumVarArray(M.env, sys.numGenerators);
	M.RTetaGenP = IloNumVarArray(M.env, sys.numGenerators);
	M.RTetaGenM = IloNumVarArray(M.env, sys.numGenerators);
	for (int i = 0; i < sys.numGenerators; i++) {

		sprintf_s(elemName, "DAgen[%d]", sys.generators[i].id);
		M.DAgen[i] = IloNumVar(M.env, sys.generators[i].Pmin, sys.generators[i].Pmax, ILOFLOAT);
		M.DAgen[i].setName(elemName);

		sprintf_s(elemName, "RTgen[%d]", sys.generators[i].id);
		M.RTgen[i] = IloNumVar(M.env, 0, sys.generators[i].Pmax, ILOFLOAT);
		M.RTgen[i].setName(elemName);

		sprintf_s(elemName, "RTetaGenP[%d]", sys.generators[i].id);
		M.RTetaGenP[i] = IloNumVar(M.env, 0, sys.generators[i].Pmax, ILOFLOAT);
		M.RTetaGenP[i].setName(elemName);

		sprintf_s(elemName, "RTetaGenM[%d]", sys.generators[i].id);
		M.RTetaGenM[i] = IloNumVar(M.env, -sys.generators[i].Pmax, 0, ILOFLOAT);
		M.RTetaGenM[i].setName(elemName);
	}
	M.model.add(M.DAgen);
	M.model.add(M.RTgen);
	M.model.add(M.RTetaGenP);
	M.model.add(M.RTetaGenM);

	/* Demand */
	M.DAdem = IloNumVarArray(M.env, sys.numLoads);
	M.RTdem = IloNumVarArray(M.env, sys.numLoads);
	M.RTetaDemP = IloNumVarArray(M.env, sys.numLoads);
	M.RTetaDemM = IloNumVarArray(M.env, sys.numLoads);
	for (int i = 0; i < sys.numLoads; i++) {

		sprintf_s(elemName, "DAdem[%d]", sys.loads[i].id);
		M.DAdem[i] = IloNumVar(M.env, 0, sys.loads[i].Pd, ILOFLOAT);
		M.DAdem[i].setName(elemName);

		sprintf_s(elemName, "RTdem[%d]", sys.loads[i].id);
		M.RTdem[i] = IloNumVar(M.env, 0, sys.loads[i].Pd, ILOFLOAT);
		M.RTdem[i].setName(elemName);

		sprintf_s(elemName, "RTetaDemP[%d]", sys.loads[i].id);
		M.RTetaDemP[i] = IloNumVar(M.env, 0, sys.loads[i].Pd, ILOFLOAT);
		M.RTetaDemP[i].setName(elemName);

		sprintf_s(elemName, "RTetaDemM[%d]", sys.loads[i].id);
		M.RTetaDemM[i] = IloNumVar(M.env, -sys.loads[i].Pd, 0, ILOFLOAT);
		M.RTetaDemM[i].setName(elemName);
	}
	M.model.add(M.DAdem);
	M.model.add(M.RTdem);
	M.model.add(M.RTetaDemP);
	M.model.add(M.RTetaDemM);

	/* Power flows */
	M.DAflow = IloNumVarArray(M.env, sys.numLines);
	M.RTflow = IloNumVarArray(M.env, sys.numLines);
	for (int i = 0; i < sys.numLines; i++) {

		sprintf_s(elemName, "DAflow[%d,%d]", sys.lines[i].source->id, sys.lines[i].destination->id);
		M.DAflow[i] = IloNumVar(M.env, sys.lines[i].minCap, sys.lines[i].maxCap, ILOFLOAT);
		M.DAflow[i].setName(elemName);

		sprintf_s(elemName, "RTflow[%d,%d]", sys.lines[i].source->id, sys.lines[i].destination->id);
		M.RTflow[i] = IloNumVar(M.env, sys.lines[i].minCap, sys.lines[i].maxCap, ILOFLOAT);
		M.RTflow[i].setName(elemName);
	}
	M.model.add(M.DAflow);
	M.model.add(M.RTflow);	

	/* Bus angles */
	M.DAtheta = IloNumVarArray(M.env, sys.numBuses);
	M.RTtheta = IloNumVarArray(M.env, sys.numBuses);
	for (int i = 0; i < sys.numBuses; i++) {

		sprintf_s(elemName, "DAtheta[%d]", sys.buses[i].id);
		M.DAtheta[i] = IloNumVar(M.env, sys.buses[i].minPhaseAngle, sys.buses[i].maxPhaseAngle, ILOFLOAT);
		M.DAtheta[i].setName(elemName);

		sprintf_s(elemName, "RTtheta[%d]", sys.buses[i].id);
		M.RTtheta[i] = IloNumVar(M.env, sys.buses[i].minPhaseAngle, sys.buses[i].maxPhaseAngle, ILOFLOAT);
		M.RTtheta[i].setName(elemName);
	}
	M.model.add(M.DAtheta);
	M.model.add(M.RTtheta);

}//END addVariables()

void addConstraints(PowerSystem sys, ClearingModel &M) {
	char elemName[NAMESIZE];
	char elemName_extra[NAMESIZE];

	/* Flow balance equation */
	M.DAflowBalance = IloRangeArray(M.env, sys.numBuses);
	M.RTflowBalance = IloRangeArray(M.env, sys.numBuses);

	for (int b = 0; b < sys.numBuses; b++) {

		IloExpr DAexpr(M.env);
		IloExpr RTexpr(M.env);
		for (int g = 0; g < sys.numGenerators; g++) {
			if (sys.generators[g].bus->id == sys.buses[b].id) {
				DAexpr += M.DAgen[g];
				RTexpr += (M.RTgen[g] - M.DAgen[g]);
			} 
		}
		for (int l = 0; l < sys.numLines; l++) {
			if (sys.lines[l].destination->id == sys.buses[b].id) {
				DAexpr += M.DAflow[l];
				RTexpr += (M.RTflow[l] - M.DAflow[l]);
			}
			if (sys.lines[l].source->id == sys.buses[b].id) {
				DAexpr -= M.DAflow[l];
				RTexpr -= (M.RTflow[l] - M.DAflow[l]);
			}
		}
		for (int d = 0; d < sys.numLoads; d++) {
			if (sys.loads[d].bus->id == sys.buses[b].id) {
				DAexpr -= M.DAdem[d];
				RTexpr -= (M.RTdem[d] - M.DAdem[d]);
			}
		}

		sprintf_s(elemName, "DAflowBalance[%d]", sys.buses[b].id);
		IloRange tempCon(M.env, 0, DAexpr, 0, elemName);
		M.DAflowBalance[b] = tempCon;

		sprintf_s(elemName_extra, "RTflowBalance[%d]", sys.buses[b].id);
		IloRange tempCon_extra(M.env, 0, RTexpr, 0, elemName_extra);
		M.RTflowBalance[b] = tempCon_extra;

		DAexpr.end();
		RTexpr.end();
	}
	M.model.add(M.DAflowBalance);
	M.model.add(M.RTflowBalance);

	//Line power flow equation : DC approximation
	M.DAdcApproximation = IloRangeArray(M.env, sys.numLines);
	M.RTdcApproximation = IloRangeArray(M.env, sys.numLines);
	for (int l = 0; l < sys.numLines; l++) {
		IloExpr DAexpr(M.env);
		IloExpr RTexpr(M.env);
		int orig = sys.lines[l].source->id - 1;
		int dest = sys.lines[l].destination->id - 1;
		sprintf_s(elemName, "DAdcApproximation[%d,%d]", orig + 1, dest + 1);
		sprintf_s(elemName_extra, "RTdcApproximation[%d,%d]", orig + 1, dest + 1);

		DAexpr = M.DAflow[l] - sys.baseKV*(M.DAtheta[orig] - M.DAtheta[dest]);
		RTexpr = M.RTflow[l] - sys.baseKV*(M.RTtheta[orig] - M.RTtheta[dest]);

		IloRange tempCon(M.env, 0, DAexpr, 0, elemName);
		M.DAdcApproximation[l] = tempCon;
		DAexpr.end();

		IloRange tempCon_extra(M.env, 0, RTexpr, 0, elemName_extra);
		M.RTdcApproximation[l] = tempCon_extra;
		RTexpr.end();
	}
	M.model.add(M.DAdcApproximation);
	M.model.add(M.RTdcApproximation);

	/* Reference angle */
	int refAngle = 0;
	M.DArefAngle = IloRangeArray(M.env, 1);
	M.RTrefAngle = IloRangeArray(M.env, 1);
	for (int b = 0; b < sys.numBuses; b++) {
		if (b == refAngle) {
			IloExpr DAexpr(M.env);
			IloExpr RTexpr(M.env);
			sprintf_s(elemName, "DArefAngle[%d]", sys.buses[b].id);
			sprintf_s(elemName_extra, "RTrefAngle[%d]", sys.buses[b].id);

			DAexpr = M.DAtheta[refAngle];
			RTexpr = M.RTtheta[refAngle];

			IloRange tempCon(M.env, 0, DAexpr, 0, elemName);
			M.DArefAngle[b] = tempCon;
			DAexpr.end();

			IloRange tempCon_extra(M.env, 0, RTexpr, 0, elemName_extra);
			M.RTrefAngle[b] = tempCon_extra;
			RTexpr.end();

			M.model.add(M.DArefAngle[b]);
			M.model.add(M.RTrefAngle[b]);
			break;
		}
	}

	/* Auxiliary variable definition */
	M.genPositive = IloRangeArray(M.env, sys.numGenerators);
	M.genNegative = IloRangeArray(M.env, sys.numGenerators);
	for (int g = 0; g < sys.numGenerators; g++) {

		sprintf_s(elemName, "genPositive[%d]", sys.generators[g].id);
		sprintf_s(elemName_extra, "genNegative[%d]", sys.generators[g].id);

		M.genPositive.add(IloRange(M.env, -IloInfinity, -M.RTetaGenP[g] + M.RTgen[g] - M.DAgen[g], 0, elemName));
		M.model.add(IloRange(M.env, -IloInfinity, -M.RTetaGenP[g] + M.RTgen[g] - M.DAgen[g], 0, elemName));

		M.genNegative.add(IloRange(M.env, -IloInfinity, M.RTetaGenM[g] - M.RTgen[g] + M.DAgen[g], 0, elemName_extra));
		M.model.add(IloRange(M.env, -IloInfinity, M.RTetaGenM[g] - M.RTgen[g] + M.DAgen[g], 0, elemName_extra));
	}
	

	/* Inflexible generators */
	M.inflexGen = IloRangeArray(M.env);
	for (int g = 0; g < sys.numGenerators; g++) {
		if (sys.generators[g].type == Generator::THERMAL)
		{
			IloExpr expr(M.env);
			sprintf_s(elemName, "inflexGen[%d]", g);

			M.genPositive.add(IloRange(M.env, 0, M.DAgen[g] - M.RTgen[g], 0, elemName));
			M.model.add(IloRange(M.env, 0, M.DAgen[g] - M.RTgen[g], 0, elemName));
		}
	}

}//END detConstraints()

void detObjective(PowerSystem sys, ClearingModel &M, vector<double> naDuals) {

	/***** Objective function *****/
	IloExpr dayAheadCost(M.env);
	dayAheadCost = M.obj.getExpr();
	M.model.remove(M.obj);

	for (int g = 0; g < sys.numGenerators; g++) {
		int it = 0;
		while (it < sys.genDA_bids.size()) {
			if (sys.genDA_bids[it].ID == sys.generators[g].id) {
				break;
			}
			it++;
		}
		if (it < sys.genDA_bids.size()) {
			dayAheadCost -= naDuals[it] * M.DAgen[it];
		}
	}
	for (int d = 0; d < sys.numLoads; d++) {
		int it = 0;
		while (it < sys.demDA_bids.size()) {
			if (sys.demDA_bids[it].ID == sys.loads[d].id) {
				break;
			}
			it++;
		}
		if (it < sys.demDA_bids.size()) {
			dayAheadCost -= naDuals[sys.numGenerators + it] * M.DAdem[it];
		}
	}

	M.obj.setExpr(dayAheadCost);
	M.model.add(M.obj);
	dayAheadCost.end();

}//END detObjective()

void stocObjective(PowerSystem sys, ClearingModel &M) {

	IloExpr realTimeCost(M.env);
	realTimeCost = M.obj.getExpr();

	M.model.remove(M.obj);
	for (int g = 0; g < sys.numGenerators; g++) {
		int it = 0;
		while (it < sys.genDA_bids.size()) {
			if (sys.genDA_bids[it].ID == sys.generators[g].id) {
				break;
			}
			it++;
		}
		if (it < sys.genDA_bids.size()) {
			realTimeCost += sys.genDA_bids[it].price * M.RTgen[g] + sys.genRT_bids[it].priceP * M.RTetaGenP[g] - sys.genRT_bids[it].priceM * M.RTetaGenM[g];
		}
	}

	for (int d = 0; d < sys.numLoads; d++) {
		int it = 0;
		while (it < sys.demDA_bids.size())
		{
			if (sys.demDA_bids[it].ID == sys.loads[d].id)
			{
				break;
			}
			it++;
		}

		if (it < sys.demDA_bids.size()) {

			realTimeCost -= sys.demDA_bids[it].price * M.RTdem[d] + sys.demRT_bids[it].priceP * M.RTetaDemM[d] - sys.demRT_bids[it].priceM * M.RTetaDemP[d];
		}
	}

	M.obj.setExpr(realTimeCost);
	M.model.add(M.obj);
	realTimeCost.end();

}//END stocObjective()

void updateSubproblemObjective(PowerSystem sys, ClearingModel &M, vector<double> naDuals, int scen) {

	for (int i = 0; i < sys.numGenerators; i++){
		M.obj.setLinearCoef(M.DAgen[i], naDuals[i]);
	}
	for (int i = 0; i < sys.numLoads; i++) {
		M.obj.setLinearCoef(M.DAdem[i], naDuals[sys.numGenerators + i]);
	}

	M.model.add(M.obj);

	int counter = 0;
	for (int g = 0; g < sys.numGenerators; g++){
		if (sys.generators[g].data == stochastic){
			M.RTgen[g].setBounds(sys.generators[g].Pmin, sys.scenarios[scen].ScenarioOutputs[counter]);
			counter++;
		}
	}

}//END detObjective()
