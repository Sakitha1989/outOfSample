/*
 * setup.cpp
 *
 *  Created on: Dec 4, 2017
 *      Author: Harsha Gangammanavar
 * Institution: Southern Methodist University
 *
 * Please send your comments or bug report to harsha (at) smu (dot) edu
 *
 */

#include "model.hpp"

void detVariables(PowerSystem sys, ClearingModel &M) {
	char elemName[NAMESIZE];

	/* Generation */
	M.DAgen = IloNumVarArray(M.env, sys.numGenerators);
	for (int i = 0; i < sys.numGenerators; i++) {
		sprintf_s(elemName, "DAgen[%d]", sys.generators[i].id);

		M.DAgen[i] = IloNumVar(M.env, sys.generators[i].Pmin, sys.generators[i].Pmax, ILOFLOAT);
		M.DAgen[i].setName(elemName);
		M.model.add(M.DAgen[i]);
	}

	/* Demand */
	M.DAdem = IloNumVarArray(M.env, sys.numLoads);
	for (int i = 0; i < sys.numLoads; i++) {
		sprintf_s(elemName, "DAdem[%d]", sys.loads[i].id);

		M.DAdem[i] = IloNumVar(M.env, 0, sys.loads[i].Pd, ILOFLOAT);
		M.DAdem[i].setName(elemName);
		M.model.add(M.DAdem[i]);
	}

	/* Power flows */
	M.DAflow = IloNumVarArray(M.env, sys.numLines);
	for (int i = 0; i < sys.numLines; i++) {
		sprintf_s(elemName, "DAflow[%d,%d]", sys.lines[i].source->id, sys.lines[i].destination->id);

		M.DAflow[i] = IloNumVar(M.env, sys.lines[i].minCap, sys.lines[i].maxCap, ILOFLOAT);
		M.DAflow[i].setName(elemName);
		M.model.add(M.DAflow[i]);
	}

	/* Bus angles */
	M.DAtheta = IloNumVarArray(M.env, sys.numBuses);
	for (int i = 0; i < sys.numBuses; i++) {
		sprintf_s(elemName, "DAtheta[%d]", sys.buses[i].id);

		M.DAtheta[i] = IloNumVar(M.env, sys.buses[i].minPhaseAngle, sys.buses[i].maxPhaseAngle, ILOFLOAT);
		M.DAtheta[i].setName(elemName);
		M.model.add(M.DAtheta[i]);
	}

}//END detVariables()

void detConstraints(PowerSystem sys, ClearingModel &M) {
	char elemName[NAMESIZE];

	/* Flow balance equation */
	M.DAflowBalance = IloRangeArray(M.env, sys.numBuses);
	for (int b = 0; b < sys.numBuses; b++) {
		IloExpr expr(M.env);
		sprintf_s(elemName, "DAflowBalance[%d]", sys.buses[b].id);

		for (int g = 0; g < sys.numGenerators; g++)
			if (sys.generators[g].bus->id == b + 1) expr += M.DAgen[g];
		for (int l = 0; l < sys.numLines; l++) {
			if (sys.lines[l].destination->id == b + 1) expr += M.DAflow[l];
			if (sys.lines[l].source->id == b + 1) expr -= M.DAflow[l];
		}
		for (int d = 0; d < sys.numLoads; d++)
			if (sys.loads[d].bus->id == b + 1) expr -= M.DAdem[d];

		IloRange tempCon(M.env, 0, expr, 0, elemName);
		M.DAflowBalance[b] = tempCon;

		expr.end();
		M.model.add(M.DAflowBalance[b]);
	}

	//Line power flow equation : DC approximation
	M.DAdcApproximation = IloRangeArray(M.env, sys.numLines);
	for (int l = 0; l < sys.numLines; l++) {
		IloExpr expr(M.env);
		int orig = sys.lines[l].source->id - 1;
		int dest = sys.lines[l].destination->id - 1;
		sprintf_s(elemName, "DAdcApproximation[%d,%d]", orig + 1, dest + 1);

		//expr = M.DAflow[l][t] - sys.baseKV*(1/sys.lines[l].reactance)*(M.DAtheta[orig][t] - M.DAtheta[dest][t]);
		expr = M.DAflow[l] - sys.baseKV*(M.DAtheta[orig] - M.DAtheta[dest]);

		IloRange tempCon(M.env, 0, expr, 0, elemName);
		M.DAdcApproximation[l] = tempCon;
		expr.end();

		M.model.add(M.DAdcApproximation[l]);
	}

	/* Reference angle */
	int refAngle = 0;
	M.DArefAngle = IloRangeArray(M.env, 1);
	for (int b = 0; b < sys.numBuses; b++) {
		if (b == refAngle) {
			IloExpr expr(M.env);
			sprintf_s(elemName, "DArefAngle[%d]", sys.buses[b].id);

			expr = M.DAtheta[refAngle];

			IloRange tempCon(M.env, 0, expr, 0, elemName);
			M.DArefAngle[b] = tempCon;
			expr.end();

			M.model.add(M.DArefAngle[b]);
			break;
		}
	}

}//END detConstraints()

void detObjective(PowerSystem sys, ClearingModel &M, vector<double> NA_dual) {

	/***** Objective function *****/
	IloExpr dayAheadCost(M.env);
	dayAheadCost = M.obj.getExpr();
	M.model.remove(M.obj);

	dayAheadCost += 0.0;
	for (int g = 0; g < sys.numGenerators; g++) {
		int it = 0;
		while (it < sys.genDA_bids.size()){
			if (sys.genDA_bids[it].ID == sys.generators[g].id){
				break;
			}
			it++;
		}
		if (it < sys.genDA_bids.size()) {
			dayAheadCost -= NA_dual[it] * M.DAgen[it];
		}
	}
	for (int d = 0; d < sys.numLoads; d++) {
		int it = 0;
		while (it < sys.demDA_bids.size()){
			if (sys.demDA_bids[it].ID == sys.loads[d].id){
				break;
			}
			it++;
		}
		if (it < sys.demDA_bids.size()) {
			dayAheadCost -= NA_dual[sys.numGenerators + it] * M.DAdem[it];
		}
	}

	M.obj.setExpr(dayAheadCost);
	M.model.add(M.obj);
	dayAheadCost.end();

}//END detObjective()

void stocVariables(PowerSystem sys, ClearingModel &M) {
	char elemName[NAMESIZE];

	/* Generation */
	M.RTgen = IloNumVarArray(M.env, sys.numGenerators);
	for (int i = 0; i < sys.numGenerators; i++) {
		sprintf_s(elemName, "RTgen[%d]", sys.generators[i].id);

		M.RTgen[i] = IloNumVar(M.env, 0, sys.generators[i].Pmax, ILOFLOAT);
		M.RTgen[i].setName(elemName);
		M.model.add(M.RTgen[i]);
	}

	/* Demand */
	M.RTdem = IloNumVarArray(M.env, sys.numLoads);
	for (int i = 0; i < sys.numLoads; i++) {
		sprintf_s(elemName, "RTdem[%d]", sys.loads[i].id);

		M.RTdem[i] = IloNumVar(M.env, 0, sys.loads[i].Pd, ILOFLOAT);
		M.RTdem[i].setName(elemName);
		M.model.add(M.RTdem[i]);
	}

	/* Power flows */
	M.RTflow = IloNumVarArray(M.env, sys.numLines);
	for (int i = 0; i < sys.numLines; i++) {
		sprintf_s(elemName, "RTflow[%d,%d]", sys.lines[i].source->id, sys.lines[i].destination->id);

		M.RTflow[i] = IloNumVar(M.env, sys.lines[i].minCap, sys.lines[i].maxCap, ILOFLOAT);
		M.RTflow[i].setName(elemName);
		M.model.add(M.RTflow[i]);
	}

	/* Bus angles */
	M.RTtheta = IloNumVarArray(M.env, sys.numBuses);
	for (int i = 0; i < sys.numBuses; i++) {
		sprintf_s(elemName, "RTtheta[%d]", sys.buses[i].id);

		M.RTtheta[i] = IloNumVar(M.env, sys.buses[i].minPhaseAngle, sys.buses[i].maxPhaseAngle, ILOFLOAT);
		M.RTtheta[i].setName(elemName);
		M.model.add(M.RTtheta[i]);
	}

	/* auxiliary variables to capture changes in generation and demand */
	M.RTetaGenP = IloNumVarArray(M.env, sys.numGenerators);
	M.RTetaGenM = IloNumVarArray(M.env, sys.numGenerators);
	for (int i = 0; i < sys.numGenerators; i++) {
		sprintf_s(elemName, "RTetaGenP[%d]", sys.generators[i].id);
		M.RTetaGenP[i] = IloNumVar(M.env, 0, sys.generators[i].Pmax, ILOFLOAT);
		M.RTetaGenP[i].setName(elemName);
		M.model.add(M.RTetaGenP[i]);

		sprintf_s(elemName, "RTetaGenM[%d]", sys.generators[i].id);
		M.RTetaGenM[i] = IloNumVar(M.env, -sys.generators[i].Pmax, 0, ILOFLOAT);
		M.RTetaGenM[i].setName(elemName);
		M.model.add(M.RTetaGenM[i]);
	}

	/* auxiliary variables to capture changes in demand */
	M.RTetaDemP = IloNumVarArray(M.env, sys.numLoads);
	M.RTetaDemM = IloNumVarArray(M.env, sys.numLoads);
	for (int i = 0; i < sys.numLoads; i++) {
		sprintf_s(elemName, "RTetaDemP[%d]",sys.loads[i].id);
		M.RTetaDemP[i] = IloNumVar(M.env, 0, sys.loads[i].Pd, ILOFLOAT);
		M.RTetaDemP[i].setName(elemName);
		M.model.add(M.RTetaDemP[i]);

		sprintf_s(elemName, "RTetaDemM[%d]", sys.loads[i].id);
		M.RTetaDemM[i] = IloNumVar(M.env, -sys.loads[i].Pd, 0, ILOFLOAT);
		M.RTetaDemM[i].setName(elemName);
		M.model.add(M.RTetaDemM[i]);
	}

}//END stocVariables()

void stocConstraints(PowerSystem sys, ClearingModel &M, int scen) {
	char elemName[NAMESIZE];
	char elemName_copy[NAMESIZE];

	/* Flow balance equation */
	M.RTflowBalance = IloRangeArray(M.env, sys.numBuses);
	for (int b = 0; b < sys.numBuses; b++) {
		IloExpr expr(M.env);
		sprintf_s(elemName, "RTflowBalance[%d]", sys.buses[b].id);

		for (int g = 0; g < sys.numGenerators; g++)
			if (sys.generators[g].bus->id == sys.buses[b].id) expr += (M.RTgen[g] - M.DAgen[g]);
		for (int l = 0; l < sys.numLines; l++) {
			if (sys.lines[l].destination->id == sys.buses[b].id) expr += (M.RTflow[l] - M.DAflow[l]);
			if (sys.lines[l].source->id == sys.buses[b].id) expr -= (M.RTflow[l] - M.DAflow[l]);
		}
		for (int d = 0; d < sys.numLoads; d++)
			if (sys.loads[d].bus->id == sys.buses[b].id) expr -= (M.RTdem[d] - M.DAdem[d]);

		IloRange tempCon(M.env, 0, expr, 0, elemName);
		M.RTflowBalance[b] = tempCon;
		expr.end();

		M.model.add(M.RTflowBalance[b]);
	}

	//Line power flow equation : DC approximation
	M.RTdcApproximation = IloRangeArray(M.env, sys.numLines);
	for (int l = 0; l < sys.numLines; l++) {
		IloExpr expr(M.env);
		int orig = sys.lines[l].destination->id - 1;
		int dest = sys.lines[l].source->id - 1;
		sprintf_s(elemName, "RTdcApproximation[%d,%d]", orig + 1, dest + 1);

		/*expr = M.RTflow[l] - sys.lines[l].susceptance*(M.RTtheta[orig] - M.RTtheta[dest]);*/
		expr = M.RTflow[l] - sys.baseKV*(M.RTtheta[orig] - M.RTtheta[dest]);

		IloRange tempCon(M.env, 0, expr, 0, elemName);
		M.RTdcApproximation[l] = tempCon;
		expr.end();

		M.model.add(M.RTdcApproximation[l]);
	}

	/* Reference angle */
	int refAngle = 0;
	M.RTrefAngle = IloRangeArray(M.env, 1);
	for (int b = 0; b < sys.numBuses; b++) {
		if (b == refAngle){
			IloExpr expr(M.env);
			sprintf_s(elemName, "RTrefAngle[%d]", sys.buses[b].id);

			expr = M.RTtheta[refAngle];

			IloRange tempCon(M.env, 0, expr, 0, elemName);
			M.RTrefAngle[b] = tempCon;
			expr.end();

			M.model.add(M.RTrefAngle[b]);
			break;
		}
	}

	/* Auxiliary variable definition */
	M.genPositive = IloRangeArray(M.env, sys.numGenerators);
	M.genNegative = IloRangeArray(M.env, sys.numGenerators);
	for (int g = 0; g < sys.numGenerators; g++) {
		IloExpr positive_expr(M.env);
		IloExpr negative_expr(M.env);
		sprintf_s(elemName, "genPositive[%d]", sys.generators[g].id);
		sprintf_s(elemName_copy, "genNegative[%d]", sys.generators[g].id);

		positive_expr = -M.RTetaGenP[g] + M.RTgen[g] - M.DAgen[g];
		negative_expr = M.RTetaGenM[g] - M.RTgen[g] + M.DAgen[g];

		IloRange positive_tempCon(M.env, positive_expr, 0, elemName);
		IloRange negative_tempCon(M.env, negative_expr, 0, elemName_copy);
		M.genPositive[g] = positive_tempCon;
		M.genNegative[g] = negative_tempCon;

		M.model.add(M.genPositive[g]);
		M.model.add(M.genNegative[g]);

		positive_tempCon.end();
		negative_tempCon.end();

	}

	M.demPositive = IloRangeArray(M.env, sys.numLoads);
	M.demNegative = IloRangeArray(M.env, sys.numLoads);
	for (int d = 0; d < sys.numLoads; d++) {
		IloExpr positive_expr(M.env);
		IloExpr negative_expr(M.env);
		sprintf_s(elemName, "demPositive[%d]", sys.loads[d].id);
		sprintf_s(elemName_copy, "demNegative[%d]", sys.loads[d].id);

		positive_expr = -M.RTetaDemP[d] + M.RTdem[d] - M.DAdem[d];
		negative_expr = M.RTetaDemM[d] - M.RTdem[d] + M.DAdem[d];

		IloRange positive_tempCon(M.env, positive_expr, 0, elemName);
		IloRange negative_tempCon(M.env, negative_expr, 0, elemName_copy);
		M.demPositive[d] = positive_tempCon;
		M.demNegative[d] = negative_tempCon;

		M.model.add(M.demPositive[d]);
		M.model.add(M.demNegative[d]);

		positive_tempCon.end();
		negative_tempCon.end();
	}

	/* Availability of renewable resources */
	M.stochGen = IloRangeArray(M.env, sys.numRandomVariables);
	int counter = 0;
	for (int i = 0; i < sys.numGenerators; i++) {
		if (sys.generators[i].data == stochastic){
			IloExpr expr(M.env);
			sprintf_s(elemName, "stochGen[%d]", i);

			expr = M.RTgen[i] - sys.scenarios[scen].ScenarioOutputs[counter];

			IloRange tempCon(M.env, -IloInfinity, expr, 0, elemName);
			M.stochGen[counter] = tempCon;

			M.model.add(M.stochGen[counter]);
			tempCon.end();
			counter++;
		}
	}

	/* Inflexible generators */
	M.inflexGen = IloRangeArray(M.env);
	counter = 0;
	for (int g = 0; g < sys.numGenerators; g++) {
		if (sys.generators[g].type == Generator::THERMAL)
		{
			IloExpr expr(M.env);
			sprintf_s(elemName, "inflexGen[%d]", g);

			expr = M.DAgen[g] - M.RTgen[g];

			IloRange tempCon(M.env, 0, expr, 0, elemName);
			M.stochGen[counter] = tempCon;
			tempCon.end();

			M.model.add(M.inflexGen[counter]);
			counter++;
		}
	}

}//END stocConstraints()

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

