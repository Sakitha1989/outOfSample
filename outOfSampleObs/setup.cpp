///*
// * setup.cpp
// *
// *  Created on: Dec 4, 2017
// *      Author: Harsha Gangammanavar
// * Institution: Southern Methodist University
// *
// * Please send your comments or bug report to harsha (at) smu (dot) edu
// *
// */
//
//#include "model.hpp"
//
//void detVariables(PowerSystem sys, ClearingModel &M, int scen) {
//	char elemName[NAMESIZE];
//
//	/* Generation */
//	M.DAgen[scen] = IloArray< IloNumVarArray >(M.env, sys.numGenerators);
//	for (int i = 0; i < sys.numGenerators; i++) {
//		sprintf(elemName, "gen[%d][%d]", scen, sys.generators[i].id);
//
//		M.DAgen[scen][i] = IloNumVarArray(M.env, config.T, sys.generators[i].Pmin, sys.generators[i].Pmax, ILOFLOAT);
//		M.DAgen[scen][i].setNames(elemName);
//		M.model.add(M.DAgen[scen][i]);
//	}
//
//	/* Demand */
//	M.DAdem[scen] = IloArray< IloNumVarArray >(M.env, sys.numLoads);
//	for (int i = 0; i < sys.numLoads; i++) {
//		sprintf(elemName, "dem[%d][%d]", scen, sys.loads[i].id);
//
//		M.DAdem[scen][i] = IloNumVarArray(M.env, config.T, 0, sys.loads[i].Pd, ILOFLOAT);
//		M.DAdem[scen][i].setNames(elemName);
//		M.model.add(M.DAdem[scen][i]);
//	}
//
//	/* Power flows */
//	M.DAflow[scen] = IloArray< IloNumVarArray >(M.env, sys.numLines);
//	for (int i = 0; i < sys.numLines; i++) {
//		sprintf(elemName, "flow[%d][%d,%d]", scen, sys.lines[i].source->id, sys.lines[i].destination->id);
//
//		M.DAflow[scen][i] = IloNumVarArray(M.env, config.T, sys.lines[i].minCap, sys.lines[i].maxCap, ILOFLOAT);
//		M.DAflow[scen][i].setNames(elemName);
//		M.model.add(M.DAflow[scen][i]);
//	}
//
//	/* Bus angles */
//	M.DAtheta[scen] = IloArray< IloNumVarArray >(M.env, sys.numBuses);
//	for (int i = 0; i < sys.numBuses; i++) {
//		sprintf(elemName, "theta[%d][%d]", scen, sys.buses[i].id);
//
//		M.DAtheta[scen][i] = IloNumVarArray(M.env, config.T, sys.buses[i].minPhaseAngle, sys.buses[i].maxPhaseAngle, ILOFLOAT);
//		M.DAtheta[scen][i].setNames(elemName);
//		M.model.add(M.DAtheta[scen][i]);
//	}
//
//}//END detVariables()
//
//void detConstraints(PowerSystem sys, ClearingModel &M, int scen) {
//	char elemName[NAMESIZE];
//
//	/* Flow balance equation */
//	M.DAflowBalance[scen] = IloArray<IloRangeArray>(M.env, sys.numBuses);
//	for (int b = 0; b < sys.numBuses; b++) {
//		M.DAflowBalance[scen][b] = IloRangeArray(M.env);
//		for (int t = 0; t < config.T; t++) {
//			IloExpr expr(M.env);
//			sprintf(elemName, "fbDA[%d][%d][%d]", scen, sys.buses[b].id, t);
//
//			for (int g = 0; g < sys.numGenerators; g++)
//				if (sys.generators[g].bus->id == b + 1) expr += M.DAgen[scen][g][t];
//			for (int l = 0; l < sys.numLines; l++) {
//				if (sys.lines[l].destination->id == b + 1) expr += M.DAflow[scen][l][t];
//				if (sys.lines[l].source->id == b + 1) expr -= M.DAflow[scen][l][t];
//			}
//			for (int d = 0; d < sys.numLoads; d++)
//				if (sys.loads[d].bus->id == b + 1) expr -= M.DAdem[scen][d][t];
//
//			M.DAflowBalance[scen][b].add(IloRange(M.env, 0, expr, 0, elemName));
//			expr.end();
//		}
//		M.model.add(M.DAflowBalance[scen][b]);
//	}
//
//	//Line power flow equation : DC approximation
//	M.DAdc[scen] = IloArray<IloRangeArray>(M.env, sys.numLines);
//	for (int l = 0; l < sys.numLines; l++) {
//		M.DAdc[scen][l] = IloRangeArray(M.env);
//		for (int t = 0; t < config.T; t++) {
//			IloExpr expr(M.env);
//			int orig = sys.lines[l].source->id - 1;
//			int dest = sys.lines[l].destination->id - 1;
//			sprintf(elemName, "dcDA[%d][%d,%d][%d]", scen, orig + 1, dest + 1, t);
//
//			//expr = M.DAflow[scen][l][t] - sys.baseKV*(1/sys.lines[l].reactance)*(M.DAtheta[scen][orig][t] - M.DAtheta[scen][dest][t]);
//			expr = M.DAflow[scen][l][t] - sys.baseKV*(M.DAtheta[scen][orig][t] - M.DAtheta[scen][dest][t]);
//			M.DAdc[scen][l].add(IloRange(M.env, 0, expr, 0, elemName));
//			expr.end();
//		}
//		M.model.add(M.DAdc[scen][l]);
//	}
//
//	/* Generation ramping up constraints */
//	M.DAru[scen] = IloArray<IloRangeArray>(M.env, sys.numGenerators);
//	for (int g = 0; g < sys.numGenerators; g++) {
//		M.DAru[scen][g] = IloRangeArray(M.env);
//		for (int t = 1; t < config.T; t++)
//		{
//			IloExpr expr(M.env);
//			if (sys.generators[g].type != Generator::WIND && sys.generators[g].type != Generator::SOLAR)
//			{
//				sprintf(elemName, "ruDA[%d][%d][%d]", scen, sys.generators[g].id, t);
//
//				expr = M.DAgen[scen][g][t] - M.DAgen[scen][g][t - 1];
//				M.DAru[scen][g].add(IloRange(M.env, -IloInfinity, expr, sys.generators[g].rampUpLim, elemName));
//				expr.end();
//			}
//		}
//		M.model.add(M.DAru[scen][g]);
//	}
//
//	/* Generation ramping down constraints */
//	M.DArd[scen] = IloArray<IloRangeArray>(M.env, sys.numGenerators);
//	for (int g = 0; g < sys.numGenerators; g++) {
//		M.DArd[scen][g] = IloRangeArray(M.env);
//		for (int t = 1; t < config.T; t++) {
//			IloExpr expr(M.env);
//			if (sys.generators[g].type != Generator::WIND && sys.generators[g].type != Generator::SOLAR) {
//				sprintf(elemName, "rdDA[%d][%d][%d]", scen, sys.generators[g].id, t);
//
//				expr = M.DAgen[scen][g][t] - M.DAgen[scen][g][t - 1];
//				M.DArd[scen][g].add(IloRange(M.env, -IloInfinity, expr, sys.generators[g].rampDownLim, elemName));
//				expr.end();
//			}
//		}
//		M.model.add(M.DArd[scen][g]);
//	}
//
//	/* Reference angle */
//	int refAngle = 0;
//	M.DArefAngle[scen] = IloArray<IloRangeArray>(M.env, sys.numBuses);
//	for (int b = 0; b < sys.numBuses; b++) {
//		M.DArefAngle[scen][b] = IloRangeArray(M.env);
//		for (int t = 0; t < config.T; t++) {
//			if (b == refAngle)
//			{
//				IloExpr expr(M.env);
//				sprintf(elemName, "refAngleDA[%d][%d][%d]", scen, sys.buses[b].id, t);
//
//				expr += M.DAtheta[scen][refAngle][t];
//				M.DArefAngle[scen][b].add(IloRange(M.env, 0, expr, 0, elemName));
//				expr.end();
//				M.model.add(M.DArefAngle[scen][b]);
//			}
//		}
//	}
//
//	/* Generator bid compliance */
//	//M.DAgbu[scen] = IloArray<IloRangeArray>(M.env, sys.numGenerators);
//	//for (int g = 0; g < sys.numGenerators; g++) {
//	//	M.DAgbu[scen][g] = IloRangeArray(M.env);
//	//	for (int t = 0; t < config.T; t++) {
//	//			//vector<int>::iterator it = find(M.bidDA.genBids[g].ID.begin(), M.bidDA.genBids[g][t].ID.end(), sys.generators[g].id);
//	//			
//	//		int it = 0;
//	//		while (it < M.bidDA.genBids.size())
//	//		{
//	//			if (M.bidDA.genBids[it][t].ID == sys.generators[g].id)
//	//			{
//	//				break;
//	//			}
//	//			it++;
//	//		}
//
//	//			if (it < M.bidDA.genBids.size()) {
//
//	//				sprintf(elemName, "gbuDA[%d][%d][%d]", scen, sys.generators[g].id, t);
//
//	//				M.DAgbu[scen][g].add(IloRange(M.env, -IloInfinity, M.DAgen[scen][g][t], M.bidDA.genBids[it][t].maxCap, elemName));
//	//			}
//	//			else {
//	//				sprintf(elemName, "gbuDA[%d][%d][%d]", scen, sys.generators[g].id, t);
//
//	//				M.DAgbu[scen][g].add(IloRange(M.env, -IloInfinity, M.DAgen[scen][g][t], sys.generators[g].Pmax, elemName));
//	//			}
//	//	}
//	//	M.model.add(M.DAgbu[scen][g]);
//	//}
//
//	//M.DAgbd[scen] = IloArray<IloRangeArray>(M.env, sys.numGenerators);
//	//for (int g = 0; g < sys.numGenerators; g++) {
//	//	M.DAgbd[scen][g] = IloRangeArray(M.env);
//	//	for (int t = 0; t < config.T; t++) {
//	//			//vector<int>::iterator it = find(M.bidDA.genID.begin(), M.bidDA.genID.end(), sys.generators[g].id);
//
//	//		int it = 0;
//	//		while (it < M.bidDA.genBids.size())
//	//		{
//	//			if (M.bidDA.genBids[it][t].ID == sys.generators[g].id)
//	//			{
//	//				break;
//	//			}
//	//			it++;
//	//		}
//
//	//			if (it < M.bidDA.genBids.size()) {
//
//	//				sprintf(elemName, "gbdDA[%d][%d][%d]", scen, sys.generators[g].id, t);
//
//	//				M.DAgbd[scen][g].add(IloRange(M.env, M.bidDA.genBids[it][t].minCap, M.DAgen[scen][g][t], IloInfinity, elemName));
//	//			}
//	//			else {
//	//				sprintf(elemName, "gbdDA[%d][%d][%d]", scen, sys.generators[g].id, t);
//
//	//				M.DAgbd[scen][g].add(IloRange(M.env, sys.generators[g].Pmin, M.DAgen[scen][g][t], IloInfinity, elemName));
//	//			}
//	//	}
//	//	M.model.add(M.DAgbd[scen][g]);
//	//}
//
//	///* Demand bid compliance */
//	//M.DAdbu[scen] = IloArray<IloRangeArray>(M.env, sys.numLoads);
//	//for (int d = 0; d < sys.numLoads; d++) {
//	//	M.DAdbu[scen][d] = IloRangeArray(M.env);
//	//	for (int t = 0; t < config.T; t++) {
//	//			//vector<int>::iterator it = find(M.bidDA.demID.begin(), M.bidDA.demID.end(), sys.loads[d].id);
//
//	//		int it = 0;
//	//		while (it < M.bidDA.demBids.size())
//	//		{
//	//			if (M.bidDA.demBids[it][t].ID == sys.loads[d].id)
//	//			{
//	//				break;
//	//			}
//	//			it++;
//	//		}
//
//	//			if (it < M.bidDA.demBids.size()) {
//	//				sprintf(elemName, "dbuDA[%d][%d][%d]", scen, sys.loads[d].id, t);
//
//	//				M.DAdbu[scen][d].add(IloRange(M.env, -IloInfinity, M.DAdem[scen][d][t], M.bidDA.demBids[it][t].maxCap, elemName));
//	//			}
//	//			else {
//	//				sprintf(elemName, "dbuDA[%d][%d][%d]", scen, sys.generators[d].id, t);
//
//	//				M.DAdbu[scen][d].add(IloRange(M.env, -IloInfinity, M.DAdem[scen][d][t], sys.loads[d].Pd, elemName));
//	//			}
//	//	}
//	//	M.model.add(M.DAdbu[scen][d]);
//	//}
//
//	//M.DAdbd[scen] = IloArray<IloRangeArray>(M.env, sys.numLoads);
//	//for (int d = 0; d < sys.numLoads; d++) {
//	//	M.DAdbd[scen][d] = IloRangeArray(M.env);
//	//	for (int t = 0; t < config.T; t++) {
//	//			//vector<int>::iterator it = find(M.bidDA.demID.begin(), M.bidDA.demID.end(), sys.loads[d].id);
//
//	//		int it = 0;
//	//		while (it < M.bidDA.demBids.size())
//	//		{
//	//			if (M.bidDA.demBids[it][t].ID == sys.loads[d].id)
//	//			{
//	//				break;
//	//			}
//	//			it++;
//	//		}
//
//	//			if (it < M.bidDA.demBids.size()) {
//	//				sprintf(elemName, "dbuDA[%d][%d][%d]", scen, sys.loads[d].id, t);
//
//	//				M.DAdbd[scen][d].add(IloRange(M.env, M.bidDA.demBids[it][t].minCap, M.DAdem[scen][d][t], IloInfinity, elemName));
//	//			}
//	//			else {
//	//				sprintf(elemName, "dbuDA[%d][%d][%d]", scen, sys.generators[d].id, t);
//
//	//				M.DAdbd[scen][d].add(IloRange(M.env, 0, M.DAdem[scen][d][t], IloInfinity, elemName));
//	//			}
//	//	}
//	//	M.model.add(M.DAdbd[scen][d]);
//	//}
//
//}//END detConstraints()
//
//void detObjective(PowerSystem sys, ClearingModel &M, double weight, int scen) {
//
//	/***** Objective function *****/
//	IloExpr dayAheadCost(M.env);
//	dayAheadCost = M.obj.getExpr();
//	M.model.remove(M.obj);
//
//	dayAheadCost += 0.0;
//	for (int t = 0; t < config.T; t++) {
//		for (int g = 0; g < sys.numGenerators; g++) {
//
//			int it = 0;
//			while (it < M.bidDA.genBids[t].size())
//			{
//				if (M.bidDA.genBids[t][it].ID == sys.generators[g].id)
//				{
//					break;
//				}
//				it++;
//			}
//
//#if defined (RT_PAYMENT_MODEL)
//			if (it < M.bidDA.genBids[t].size()) {
//				if (M.name == "subproblem") {
//					dayAheadCost -= weight * 0;
//				}
//				else {
//					dayAheadCost += weight * 0;
//				}
//			}
//#endif
//		}
//		for (int d = 0; d < sys.numLoads; d++) {
//
//			int it = 0;
//			while (it < M.bidDA.demBids[t].size())
//			{
//				if (M.bidDA.demBids[t][it].ID == sys.loads[d].id)
//				{
//					break;
//				}
//				it++;
//			}
//
//#if defined (RT_PAYMENT_MODEL)
//			if (it < M.bidDA.demBids[t].size()) {
//				dayAheadCost -= weight * 0;
//			}
//#else
//			if (it < M.bidDA.demBids[t].size()) {
//				dayAheadCost -= weight * M.bidDA.demBids[t][it].price*M.DAdem[scen][i][t];
//			}
//#endif
//		}
//	}
//
//	M.obj.setExpr(dayAheadCost);
//	M.model.add(M.obj);
//	dayAheadCost.end();
//
//}//END detObjective()
//
//void stocVariables(PowerSystem sys, ClearingModel &M, int scen) {
//	char elemName[NAMESIZE];
//
//	/* Generation */
//	M.RTgen[scen] = IloArray< IloNumVarArray >(M.env, sys.numGenerators);
//	for (int i = 0; i < sys.numGenerators; i++) {
//		sprintf(elemName, "RTgen[%d][%d]", scen, sys.generators[i].id);
//
//		M.RTgen[scen][i] = IloNumVarArray(M.env, config.T, 0, sys.generators[i].Pmax, ILOFLOAT);
//		M.RTgen[scen][i].setNames(elemName);
//		M.model.add(M.RTgen[scen][i]);
//	}
//#if defined (diff)
//	M.RTx[scen] = IloArray< IloNumVarArray >(M.env, sys.numGenerators);
//	for (int i = 0; i < sys.numGenerators; i++) {
//		sprintf(elemName, "RTx[%d][%d]", scen, sys.generators[i].id);
//
//		M.RTx[scen][i] = IloNumVarArray(M.env, config.T, -IloInfinity, IloInfinity, ILOFLOAT);
//		M.RTx[scen][i].setNames(elemName);
//		M.model.add(M.RTx[scen][i]);
//	}
//#endif
//
//	/* Demand */
//	M.RTdem[scen] = IloArray< IloNumVarArray >(M.env, sys.numLoads);
//	for (int i = 0; i < sys.numLoads; i++) {
//		sprintf(elemName, "RTdem[%d][%d]", scen, sys.loads[i].id);
//
//		M.RTdem[scen][i] = IloNumVarArray(M.env, config.T, 0, sys.loads[i].Pd, ILOFLOAT);
//		M.RTdem[scen][i].setNames(elemName);
//		M.model.add(M.RTdem[scen][i]);
//	}
//
//	/* Power flows */
//	M.RTflow[scen] = IloArray< IloNumVarArray >(M.env, sys.numLines);
//	for (int i = 0; i < sys.numLines; i++) {
//		sprintf(elemName, "RTflow[%d][%d,%d]", scen, sys.lines[i].source->id, sys.lines[i].destination->id);
//
//		M.RTflow[scen][i] = IloNumVarArray(M.env, config.T, sys.lines[i].minCap, sys.lines[i].maxCap, ILOFLOAT);
//		M.RTflow[scen][i].setNames(elemName);
//		M.model.add(M.RTflow[scen][i]);
//	}
//
//	/* Bus angles */
//	M.RTtheta[scen] = IloArray< IloNumVarArray >(M.env, sys.numBuses);
//	for (int i = 0; i < sys.numBuses; i++) {
//		sprintf(elemName, "RTtheta[%d][%d]", scen, sys.buses[i].id);
//
//		M.RTtheta[scen][i] = IloNumVarArray(M.env, config.T, sys.buses[i].minPhaseAngle, sys.buses[i].maxPhaseAngle, ILOFLOAT);
//		M.RTtheta[scen][i].setNames(elemName);
//		M.model.add(M.RTtheta[scen][i]);
//	}
//
//	/* auxiliary variables to capture changes in generation and demand */
//	M.RTetaP[scen] = IloArray< IloNumVarArray >(M.env, sys.numGenerators + sys.numLoads);
//	M.RTetaM[scen] = IloArray< IloNumVarArray >(M.env, sys.numGenerators + sys.numLoads);
//	for (int i = 0; i < sys.numGenerators; i++) {
//		sprintf(elemName, "RTetaP[%d][%d]", scen, sys.generators[i].id);
//		M.RTetaP[scen][i] = IloNumVarArray(M.env, config.T, 0, sys.generators[i].Pmax, ILOFLOAT);
//		M.RTetaP[scen][i].setNames(elemName);
//		M.model.add(M.RTetaP[scen][i]);
//
//		sprintf(elemName, "RTetaM[%d][%d]", scen, sys.generators[i].id);
//		M.RTetaM[scen][i] = IloNumVarArray(M.env, config.T, -sys.generators[i].Pmax, 0, ILOFLOAT);
//		M.RTetaM[scen][i].setNames(elemName);
//		M.model.add(M.RTetaM[scen][i]);
//	}
//	for (int i = 0; i < sys.numLoads; i++) {
//		sprintf(elemName, "RTetaP[%d][%d]", scen, sys.numGenerators + sys.loads[i].id);
//		M.RTetaP[scen][sys.numGenerators + i] = IloNumVarArray(M.env, config.T, 0, sys.loads[i].Pd, ILOFLOAT);
//		M.RTetaP[scen][sys.numGenerators + i].setNames(elemName);
//		M.model.add(M.RTetaP[scen][sys.numGenerators + i]);
//
//		sprintf(elemName, "RTetaM[%d][%d]", scen, sys.numGenerators + sys.loads[i].id);
//		M.RTetaM[scen][sys.numGenerators + i] = IloNumVarArray(M.env, config.T, -sys.loads[i].Pd, 0, ILOFLOAT);
//		M.RTetaM[scen][sys.numGenerators + i].setNames(elemName);
//		M.model.add(M.RTetaM[scen][sys.numGenerators + i]);
//	}
//
//}//END stocVariables()
//
//void stocConstraints(PowerSystem sys, ClearingModel &M, int scen, int parentScen, vector<vector<double> > observ) {
//	char elemName[NAMESIZE];
//	char elemNameTemp[NAMESIZE];
//
//	/* Flow balance equation */
//#if defined (DIFF_ETA)
//	M.RTflowBalance[scen] = IloArray<IloRangeArray>(M.env, sys.numBuses);
//	for (int b = 0; b < sys.numBuses; b++) {
//		M.RTflowBalance[scen][b] = IloRangeArray(M.env);
//		for (int t = 0; t < config.T; t++) {
//			IloExpr expr(M.env);
//			sprintf(elemName, "fbRT[%d][%d][%d]", scen, sys.buses[b].id, t);
//
//			for (int g = 0; g < sys.numGenerators; g++)
//				if (sys.generators[g].bus->id == sys.buses[b].id) expr += (M.RTetaP[scen][g][t] - M.RTetaM[scen][g][t]);
//			for (int l = 0; l < sys.numLines; l++) {
//				if (sys.lines[l].destination->id == sys.buses[b].id) expr += (M.RTflow[scen][l][t] - M.DAflow[parentScen][l][t]);
//				if (sys.lines[l].source->id == sys.buses[b].id) expr -= (M.RTflow[scen][l][t] - M.DAflow[parentScen][l][t]);
//			}
//			for (int d = 0; d < sys.numLoads; d++)
//				if (sys.loads[d].bus->id == sys.buses[b].id) expr -= (M.RTetaP[scen][sys.numGenerators + d][t] - M.RTetaM[scen][sys.numGenerators + d][t]);
//
//			M.RTflowBalance[scen][b].add(IloRange(M.env, 0, expr, 0, elemName));
//			expr.end();
//		}
//		M.model.add(M.RTflowBalance[scen][b]);
//	}
//#endif
//
//#if defined (DIFF_FLOW)
//	M.RTflowBalance[scen] = IloArray<IloRangeArray>(M.env, sys.numBuses);
//	for (int b = 0; b < sys.numBuses; b++) {
//		M.RTflowBalance[scen][b] = IloRangeArray(M.env);
//		for (int t = 0; t < config.T; t++) {
//			IloExpr expr(M.env);
//			sprintf(elemName, "fbRT[%d][%d][%d]", scen, sys.buses[b].id, t);
//
//			for (int g = 0; g < sys.numGenerators; g++)
//				if (sys.generators[g].bus->id == sys.buses[b].id) expr += (M.RTgen[scen][g][t] - M.DAgen[parentScen][g][t]);
//			for (int l = 0; l < sys.numLines; l++) {
//				if (sys.lines[l].destination->id == sys.buses[b].id) expr += (M.RTflow[scen][l][t] - M.DAflow[parentScen][l][t]);
//				if (sys.lines[l].source->id == sys.buses[b].id) expr -= (M.RTflow[scen][l][t] - M.DAflow[parentScen][l][t]);
//			}
//			for (int d = 0; d < sys.numLoads; d++)
//				if (sys.loads[d].bus->id == sys.buses[b].id) expr -= (M.RTdem[scen][d][t] - M.DAdem[parentScen][d][t]);
//
//			M.RTflowBalance[scen][b].add(IloRange(M.env, 0, expr, 0, elemName));
//			expr.end();
//		}
//		M.model.add(M.RTflowBalance[scen][b]);
//	}
//
//#endif
//
//#if defined (RT_FLOW)
//	M.RTflowBalance[scen] = IloArray<IloRangeArray>(M.env, sys.numBuses);
//	for (int b = 0; b < sys.numBuses; b++) {
//		M.RTflowBalance[scen][b] = IloRangeArray(M.env);
//		for (int t = 0; t < config.T; t++) {
//			IloExpr expr(M.env);
//			sprintf(elemName, "fbRT[%d][%d][%d]", scen, sys.buses[b].id, t);
//
//			for (int g = 0; g < sys.numGenerators; g++)
//				if (sys.generators[g].bus->id == sys.buses[b].id) expr += M.RTgen[scen][g][t];
//			for (int l = 0; l < sys.numLines; l++) {
//				if (sys.lines[l].destination->id == sys.buses[b].id) expr += M.RTflow[scen][l][t];
//				if (sys.lines[l].source->id == sys.buses[b].id) expr -= M.RTflow[scen][l][t];
//			}
//			for (int d = 0; d < sys.numLoads; d++)
//				if (sys.loads[d].bus->id == sys.buses[b].id) expr -= M.RTdem[scen][d][t];
//
//			M.RTflowBalance[scen][b].add(IloRange(M.env, 0, expr, 0, elemName));
//			expr.end();
//		}
//		M.model.add(M.RTflowBalance[scen][b]);
//	}
//#endif
//
//	//Line power flow equation : DC approximation
//	M.RTdc[scen] = IloArray<IloRangeArray>(M.env, sys.numLines);
//	for (int l = 0; l < sys.numLines; l++) {
//		M.RTdc[scen][l] = IloRangeArray(M.env);
//		for (int t = 0; t < config.T; t++) {
//			IloExpr expr(M.env);
//			int orig = sys.lines[l].destination->id - 1;
//			int dest = sys.lines[l].source->id - 1;
//			sprintf(elemName, "dcRT[%d][%d,%d][%d]", scen, orig + 1, dest + 1, t);
//
//			/*expr = M.RTflow[scen][l][t] - sys.lines[l].susceptance*(M.RTtheta[scen][orig][t] - M.RTtheta[scen][dest][t]);*/
//			expr = M.RTflow[scen][l][t] - sys.baseKV*(M.RTtheta[scen][orig][t] - M.RTtheta[scen][dest][t]);
//			M.RTdc[scen][l].add(IloRange(M.env, 0, expr, 0, elemName));
//			expr.end();
//		}
//		M.model.add(M.RTdc[scen][l]);
//	}
//
//	/* Generation ramping constraints */
//	/*for ( int t = 1; t < config.T; t++ ) {
//		for ( int g = 0; g < sys.numGenerators; g++ ) {
//			if ( sys.generators[g].type != Generator::WIND && sys.generators[g].type !=  Generator::SOLAR) {
//				sprintf(elemName, "ruRT[%d][%d][%d]", scen, sys.generators[g].id, t);
//				IloConstraint c1( M.RTgen[scen][g][t] - M.RTgen[scen][g][t-1] <= sys.generators[g].rampUpLim); c1.setName(elemName); M.model.add(c1);
//
//				sprintf(elemName, "rdRT[%d][%d][%d]", scen, sys.generators[g].id, t);
//				IloConstraint c2( sys.generators[g].rampDownLim >= - M.RTgen[scen][g][t] + M.RTgen[scen][g][t-1]); c2.setName(elemName); M.model.add(c2);
//			}
//		}
//	}*/
//
//	/* Generation ramping up constraints */
//	M.RTru[scen] = IloArray<IloRangeArray>(M.env, sys.numGenerators);
//	for (int g = 0; g < sys.numGenerators; g++) {
//		M.RTru[scen][g] = IloRangeArray(M.env);
//		for (int t = 1; t < config.T; t++) {
//			IloExpr expr(M.env);
//			if (sys.generators[g].type != Generator::WIND && sys.generators[g].type != Generator::SOLAR) {
//				sprintf(elemName, "ruRT[%d][%d][%d]", scen, sys.generators[g].id, t);
//
//				expr = M.RTgen[scen][g][t] - M.RTgen[scen][g][t - 1];
//				M.RTru[scen][g].add(IloRange(M.env, -IloInfinity, expr, sys.generators[g].rampUpLim, elemName));
//				expr.end();
//			}
//		}
//		M.model.add(M.RTru[scen][g]);
//	}
//
//	/* Generation ramping down constraints */
//	M.RTrd[scen] = IloArray<IloRangeArray>(M.env, sys.numGenerators);
//	for (int g = 0; g < sys.numGenerators; g++) {
//		M.RTrd[scen][g] = IloRangeArray(M.env);
//		for (int t = 1; t < config.T; t++) {
//			IloExpr expr(M.env);
//			if (sys.generators[g].type != Generator::WIND && sys.generators[g].type != Generator::SOLAR) {
//				sprintf(elemName, "rdRT[%d][%d][%d]", scen, sys.generators[g].id, t);
//
//				expr = M.RTgen[scen][g][t] - M.RTgen[scen][g][t - 1];
//				M.RTrd[scen][g].add(IloRange(M.env, -IloInfinity, expr, sys.generators[g].rampDownLim, elemName));
//				expr.end();
//			}
//		}
//		M.model.add(M.RTrd[scen][g]);
//	}
//
//	/* Reference angle */
//	int refAngle = 0;
//	M.RTrefAngle[scen] = IloArray<IloRangeArray>(M.env, sys.numBuses);
//	for (int b = 0; b < sys.numBuses; b++) {
//		M.RTrefAngle[scen][b] = IloRangeArray(M.env);
//		for (int t = 0; t < config.T; t++) {
//			if (b == refAngle)
//			{
//				IloExpr expr(M.env);
//				sprintf(elemName, "refAngleRT[%d][%d][%d]", scen, sys.buses[b].id, t);
//
//				expr += M.RTtheta[scen][refAngle][t];
//				M.RTrefAngle[scen][b].add(IloRange(M.env, 0, expr, 0, elemName));
//				expr.end();
//				M.model.add(M.RTrefAngle[scen][b]);
//			}
//		}
//	}
//
//	/* Generator bid compliance */
//	//M.RTgbu[scen] = IloArray<IloRangeArray>(M.env);
//	//int i = 0;
//	//for (int g = 0; g < sys.numGenerators; g++) {
//	//	//vector<int>::iterator it = find(M.bidRT.genID.begin(), M.bidRT.genID.end(), sys.generators[g].id);
//
//	//	
//	//	for (int t = 0; t < config.T; t++) {
//
//	//		int it = 0;
//	//		while (it < M.bidDA.genBids.size())
//	//		{
//	//			if (M.bidDA.genBids[it][t].ID == sys.generators[g].id)
//	//			{
//	//				break;
//	//			}
//	//			it++;
//	//		}
//
//	//		if (it < M.bidDA.genBids.size() || sys.generators[g].type != Generator::WIND)
//	//		{
//	//			M.RTgbu[scen].add(IloRangeArray(M.env));
//	//			i++;
//	//		}
//
//	//		if (it < M.bidDA.genBids.size()) {
//	//			//ptrdiff_t pos = it - M.bidRT.genID.begin();
//	//			sprintf(elemName, "gbuRT[%d][%d][%d]", scen, sys.generators[g].id, t);
//	//			M.RTgbu[scen][i - 1].add(IloRange(M.env, -IloInfinity, M.RTetaP[scen][g][t], M.bidRT.genBids[it][t].maxCap, elemName));
//	//		}
//	//		else if (sys.generators[g].type != Generator::WIND) {
//	//			sprintf(elemName, "gbuRT[%d][%d][%d]", scen, sys.generators[g].id, t);
//	//			M.RTgbu[scen][i - 1].add(IloRange(M.env, -IloInfinity, M.RTetaP[scen][g][t], 0, elemName));
//	//		}
//	//		if (it < M.bidDA.genBids.size() || sys.generators[g].type != Generator::WIND)
//	//		{
//	//			M.model.add(M.RTgbu[scen][i - 1]);
//	//		}
//	//	}
//	//}
//
//	//M.RTgbd[scen] = IloArray<IloRangeArray>(M.env);
//	//int j = 0;
//	//for (int g = 0; g < sys.numGenerators; g++) {
//	//	//vector<int>::iterator it = find(M.bidRT.genID.begin(), M.bidRT.genID.end(), sys.generators[g].id);
//	//	
//	//	for (int t = 0; t < config.T; t++) {
//	//		int it = 0;
//	//		while (it < M.bidDA.genBids.size())
//	//		{
//	//			if (M.bidDA.genBids[it][t].ID == sys.generators[g].id)
//	//			{
//	//				break;
//	//			}
//	//			it++;
//	//		}
//
//	//		if (it < M.bidDA.genBids.size() || sys.generators[g].type != Generator::WIND) {
//	//			M.RTgbd[scen].add(IloRangeArray(M.env));
//	//			j++;
//	//		}
//
//	//		if (it < M.bidDA.genBids.size()) {
//	//			//ptrdiff_t pos = it - M.bidRT.genID.begin();
//	//			sprintf(elemName, "gbdRT[%d][%d][%d]", scen, sys.generators[g].id, t);
//	//			M.RTgbd[scen][j - 1].add(IloRange(M.env, -IloInfinity, M.RTetaM[scen][g][t], -M.bidRT.genBids[it][t].minCap, elemName));
//	//		}
//	//		else if (sys.generators[g].type != Generator::WIND) {
//	//			sprintf(elemName, "gbdRT[%d][%d][%d]", scen, sys.generators[g].id, t);
//	//			M.RTgbd[scen][j - 1].add(IloRange(M.env, -IloInfinity, M.RTetaM[scen][g][t], 0, elemName));
//	//		}
//	//		if (it < M.bidDA.genBids.size() || sys.generators[g].type != Generator::WIND) {
//	//			M.model.add(M.RTgbd[scen][j - 1]);
//	//		}
//	//	}
//	//}
//
//	///* Demand bid compliance */
//	//M.RTdbu[scen] = IloArray<IloRangeArray>(M.env, sys.numLoads);
//	//for (int d = 0; d < sys.numLoads; d++) {
//	//	M.RTdbu[scen][d] = IloRangeArray(M.env);
//	//	for (int t = 0; t < config.T; t++) {
//	//			//vector<int>::iterator it = find(M.bidRT.demID.begin(), M.bidRT.demID.end(), sys.loads[d].id);
//
//	//		int it = 0;
//	//		while (it < M.bidDA.demBids.size())
//	//		{
//	//			if (M.bidDA.demBids[it][t].ID == sys.generators[d].id)
//	//			{
//	//				break;
//	//			}
//	//			it++;
//	//		}
//
//	//			if (it < M.bidDA.demBids.size()) {
//	//				//ptrdiff_t pos = it - M.bidRT.demID.begin();
//	//				sprintf(elemName, "dbuRT[%d][%d][%d]", scen, sys.numGenerators + d + 1, t);
//	//				M.RTdbu[scen][d].add(IloRange(M.env, -IloInfinity, M.RTetaP[scen][sys.numGenerators + d][t], M.bidRT.demBids[it][t].maxCap, elemName));
//	//			}
//	//			else {
//	//				sprintf(elemName, "dbuRT[%d][%d][%d]", scen, sys.numGenerators + d + 1, t);
//	//				M.RTdbu[scen][d].add(IloRange(M.env, -IloInfinity, M.RTetaP[scen][sys.numGenerators + d][t], 0, elemName));
//	//			}
//	//	}
//	//	M.model.add(M.RTdbu[scen][d]);
//	//}
//
//	//M.RTdbd[scen] = IloArray<IloRangeArray>(M.env, sys.numLoads);
//	//for (int d = 0; d < sys.numLoads; d++) {
//	//	M.RTdbd[scen][d] = IloRangeArray(M.env);
//	//	for (int t = 0; t < config.T; t++) {
//	//			//vector<int>::iterator it = find(M.bidRT.demID.begin(), M.bidRT.demID.end(), sys.loads[d].id);
//
//	//		int it = 0;
//	//		while (it < M.bidDA.demBids.size())
//	//		{
//	//			if (M.bidDA.demBids[it][t].ID == sys.generators[d].id)
//	//			{
//	//				break;
//	//			}
//	//			it++;
//	//		}
//
//	//			if (it < M.bidDA.demBids.size()) {
//	//				//ptrdiff_t pos = it - M.bidRT.demID.begin();
//	//				sprintf(elemName, "dbdRT[%d][%d][%d]", scen, sys.numGenerators + d + 1, t);
//	//				M.RTdbd[scen][d].add(IloRange(M.env, -IloInfinity, M.RTetaM[scen][sys.numGenerators + d][t], -M.bidRT.demBids[it][t].minCap, elemName));
//	//			}
//	//			else {
//	//				sprintf(elemName, "dbdRT[%d][%d][%d]", scen, sys.numGenerators + d + 1, t);
//	//				M.RTdbd[scen][d].add(IloRange(M.env, -IloInfinity, M.RTetaM[scen][sys.numGenerators + d][t], 0, elemName));
//	//			}
//	//	}
//	//	M.model.add(M.RTdbd[scen][d]);
//	//}
//
//	/* Auxiliary variable definition */
//	M.RTdgP[scen] = IloArray<IloRangeArray>(M.env, sys.numGenerators);
//	M.RTdgM[scen] = IloArray<IloRangeArray>(M.env, sys.numGenerators);
//	for (int g = 0; g < sys.numGenerators; g++) {
//		M.RTdgP[scen][g] = IloRangeArray(M.env);
//		M.RTdgM[scen][g] = IloRangeArray(M.env);
//		for (int t = 0; t < config.T; t++) {
//			sprintf(elemName, "dgRTP[%d][%d][%d]", scen, sys.generators[g].id, t);
//			sprintf(elemNameTemp, "dgRTM[%d][%d][%d]", scen, sys.generators[g].id, t);
//
//			//M.RTdgP[scen][g].add(IloRange(M.env,  M.RTgen[scen][g][t] - M.DAgen[parentScen][g][t] - M.RTetaP[scen][g][t] + M.RTetaM[scen][g][t], 0, elemName));
//
//			M.RTdgP[scen][g].add(IloRange(M.env, -M.RTetaP[scen][g][t] + M.RTgen[scen][g][t] - M.DAgen[parentScen][g][t], 0, elemName));
//			M.RTdgM[scen][g].add(IloRange(M.env, M.RTetaM[scen][g][t] - M.RTgen[scen][g][t] + M.DAgen[parentScen][g][t], 0, elemNameTemp));
//		}
//		M.model.add(M.RTdgP[scen][g]);
//		M.model.add(M.RTdgM[scen][g]);
//	}
//
//	M.RTddP[scen] = IloArray<IloRangeArray>(M.env, sys.numLoads);
//	M.RTddM[scen] = IloArray<IloRangeArray>(M.env, sys.numLoads);
//	for (int d = 0; d < sys.numLoads; d++) {
//		M.RTddP[scen][d] = IloRangeArray(M.env);
//		M.RTddM[scen][d] = IloRangeArray(M.env);
//		for (int t = 0; t < config.T; t++) {
//			sprintf(elemName, "ddRTP[%d][%d][%d]", scen, sys.loads[d].id, t);
//			sprintf(elemNameTemp, "ddRTM[%d][%d][%d]", scen, sys.loads[d].id, t);
//
//			//M.RTddP[scen][d].add(IloRange(M.env, M.RTdem[scen][d][t] - M.DAdem[parentScen][d][t] - M.RTetaP[scen][sys.numGenerators + d][t] + M.RTetaM[scen][sys.numGenerators + d][t], 0, elemName));
//
//			M.RTddP[scen][d].add(IloRange(M.env, -M.RTetaP[scen][sys.numGenerators + d][t] + M.RTdem[scen][d][t] - M.DAdem[parentScen][d][t], 0, elemName));
//			M.RTddM[scen][d].add(IloRange(M.env, M.RTetaM[scen][sys.numGenerators + d][t] - M.RTdem[scen][d][t] + M.DAdem[parentScen][d][t], 0, elemNameTemp));
//		}
//		M.model.add(M.RTddP[scen][d]);
//		M.model.add(M.RTddM[scen][d]);
//	}
//
//#if 0
//	for (int t = 0; t < config.T; t++) {
//		for (int i = 0; i < sys.numGenerators; i++) {
//			sprintf(elemName, "dgpRT[%d][%d][%d]", scen, sys.generators[i].id, t);
//			IloConstraint c1(M.RTetaP[scen][i][t] >= M.RTgen[scen][i][t] - M.DAgen[parentScen][i][t]);
//			c1.setName(elemName); M.model.add(c1);
//
//			sprintf(elemName, "dgmRT[%d][%d][%d]", scen, sys.generators[i].id, t);
//			IloConstraint c2(M.RTetaM[scen][i][t] >= M.DAgen[parentScen][i][t] - M.RTgen[scen][i][t]);
//			c2.setName(elemName); M.model.add(c2);
//		}
//		for (int i = 0; i < sys.numLoads; i++) {
//			sprintf(elemName, "ddpRT[%d][%d][%d]", scen, sys.numGenerators + sys.loads[i].id, t);
//			IloConstraint c3(M.RTetaP[scen][sys.numGenerators + i][t] >= M.RTdem[scen][i][t] - M.DAdem[parentScen][i][t]);
//			c3.setName(elemName); M.model.add(c3);
//
//			sprintf(elemName, "ddmRT[%d][%d][%d]", scen, sys.numGenerators + sys.loads[i].id, t);
//			IloConstraint c4(M.RTetaM[scen][sys.numGenerators + i][t] >= M.DAdem[parentScen][i][t] - M.RTdem[scen][i][t]);
//			c4.setName(elemName); M.model.add(c4);
//		}
//	}
//#endif
//
//	/* Availability of renewable resources */
//	M.RTga[scen] = IloArray<IloRangeArray>(M.env);
//	int j = 0;
//
//	for (int i = 0; i < (int)M.rvIdx.size(); i++) {
//		//if (M.rvType[i] == GENERATION) 
//		{
//			M.RTga[scen].add(IloRangeArray(M.env));
//			j++;
//			for (int t = 0; t < config.T; t++) {
//				int pos = 0;
//				while (pos < (int)sys.generators.size()) {
//					if (sys.generators[pos].id == M.rvIdx[i]) {
//						break;
//					}
//					pos++;
//				}
//				sprintf(elemName, "gaRT[%d][%d][%d]", scen, pos, t);
//				M.RTga[scen][j - 1].add(IloRange(M.env, -IloInfinity, M.RTgen[scen][pos][t], observ[t][i], elemName));
//				M.RTgen[scen][pos][0].setBounds(0, observ[t][i]);
//			}
//			M.model.add(M.RTga[scen][j - 1]);
//		}
//	}
//
//	/* Inflexible generators */
//	M.RTinflex[scen] = IloArray<IloRangeArray>(M.env);
//	j = 0;
//
//	for (int g = 0; g < sys.numGenerators; g++) {
//		if (sys.generators[g].type == Generator::THERMAL)
//		{
//			M.RTinflex[scen].add(IloRangeArray(M.env));
//			for (int t = 0; t < config.T; t++) {
//				sprintf(elemName, "inflexRT[%d][%d][%d]", scen, g, t);
//				M.RTinflex[scen][j].add(IloRange(M.env, 0, M.DAgen[parentScen][g][t] - M.RTgen[scen][g][t], 0, elemName));
//			}
//			M.model.add(M.RTinflex[scen][j]);
//			j++;
//		}
//	}
//
//
//
//	/*M.RTda[scen] = IloArray<IloRangeArray>(M.env);
//	j = 0;
//	for (int i = 0; i < (int)M.rvIdx.size(); i++) {
//		if (M.rvType[i] == DEMAND)
//		{
//			M.RTda[scen].add(IloRangeArray(M.env));
//			j++;
//			for (int t = 0; t < config.T; t++) {
//				int pos = 0;
//				while (pos < (int)sys.loads.size()) {
//					if (sys.loads[i].id == M.rvIdx[i]) {
//						break;
//					}
//					pos++;
//				}
//				sprintf(elemName, "daRT[%d][%d][%d]", scen, pos, t);
//				M.RTda[scen][j - 1].add(IloRange(M.env, -IloInfinity, M.RTgen[scen][pos][t], observ[t][i], elemName));
//			}
//			M.model.add(M.RTda[scen][j - 1]);
//		}
//	}*/
//
//}//END stocConstraints()
//
//void stocObjective(PowerSystem sys, ClearingModel &M, double weight, int scen) {
//
//	int n = 0;
//	if (M.name == "2slp") {
//		n = 0;
//	}
//	else {
//		n = scen;
//	}
//
//	IloExpr realTimeCost(M.env);
//	realTimeCost = M.obj.getExpr();
//
//	M.model.remove(M.obj);
//	for (int t = 0; t < config.T; t++) {
//		for (int g = 0; g < sys.numGenerators; g++) {
//			int it = 0;
//			while (it < M.bidDA.genBids[t].size())
//			{
//				if (M.bidDA.genBids[t][it].ID == sys.generators[g].id)
//				{
//					break;
//				}
//				it++;
//			}
//
//			if (it < M.bidDA.genBids[t].size()) {
//
//#if defined (PAYMENT_MODEL)
//				realTimeCost += weight *
//					((M.bidDA.genBids[t][it].price + M.bidRT.genBids[t][it].priceP)*M.RTetaP[scen][g][t] - ((M.bidDA.genBids[t][it].price - M.bidRT.genBids[t][it].priceM)*M.RTetaM[scen][g][t]));
//#endif
//#if defined (PENALTY_MODEL)
//				realTimeCost += weight *
//					((M.bidDA.genBids[t][it].price + M.bidRT.genBids[t][it].priceP)*M.RTetaP[scen][g][t] + ((M.bidDA.genBids[t][it].price + M.bidRT.genBids[t][it].priceM)*M.RTetaM[scen][g][t]));
//#endif
//#if defined (RT_PAYMENT_MODEL)
//				realTimeCost += weight *
//					(M.bidDA.genBids[t][it].price*M.RTgen[scen][g][t] + M.bidRT.genBids[t][it].priceP*M.RTetaP[scen][g][t] - M.bidRT.genBids[t][it].priceM*M.RTetaM[scen][g][t]);
//#endif
//			}
//		}
//		for (int d = 0; d < sys.numLoads; d++) {
//			int it = 0;
//			while (it < M.bidDA.demBids[t].size())
//			{
//				if (M.bidDA.demBids[t][it].ID == sys.loads[d].id)
//				{
//					break;
//				}
//				it++;
//			}
//
//			if (it < M.bidDA.demBids[t].size()) {
//
//#if defined (PAYMENT_MODEL)
//				realTimeCost -= weight *
//					((M.bidDA.demBids[t][it].price + M.bidRT.demBids[t][it].priceP)*M.RTetaP[scen][sys.numGenerators + d][t] - (M.bidDA.demBids[t][it].price - M.bidRT.demBids[t][it].priceM)*M.RTetaM[scen][sys.numGenerators + d][t]);
//#endif
//#if defined (PENALTY_MODEL)
//				realTimeCost += weight *
//					((M.bidDA.demBids[t][it].price + M.bidRT.demBids[t][it].priceP)*M.RTetaP[scen][sys.numGenerators + d][t] + (M.bidDA.demBids[t][it].price - M.bidRT.demBids[t][it].priceM)*M.RTetaM[scen][sys.numGenerators + d][t]);
//#endif
//#if defined (RT_PAYMENT_MODEL)
//				realTimeCost -= weight *
//					(M.bidDA.demBids[t][it].price*M.RTdem[scen][d][t] + M.bidRT.demBids[t][it].priceP*M.RTetaM[scen][sys.numGenerators + d][t] - M.bidRT.demBids[t][it].priceM*M.RTetaP[scen][sys.numGenerators + d][t]);
//#endif
//			}
//		}
//		/*for (int l = 0; l < sys.numLines; l++) {
//			int it = 0;
//			while (it < M.bidRT.lineBids[t].size())
//			{
//				if (M.bidRT.lineBids[t][it].ID == sys.lines[l].id)
//				{
//					break;
//				}
//				it++;
//			}
//
//			if (it < M.bidRT.lineBids[t].size()) {
//#if defined (RT_PAYMENT_MODEL)
//				realTimeCost -= weight *
//					(M.bidRT.lineBids[t][it].priceP*(M.RTflow[scen][l][t] - M.DAflow[n][l][t]));
//#endif
//			}
//		}
//		for (int b = 0; b < sys.numBuses; b++) {
//			int it = 0;
//			while (it < M.bidRT.thetaBids[t].size())
//			{
//				if (M.bidRT.thetaBids[t][it].ID == sys.buses[b].id)
//				{
//					break;
//				}
//				it++;
//			}
//
//			if (it < M.bidRT.thetaBids[t].size()) {
//#if defined (RT_PAYMENT_MODEL)
//				realTimeCost -= weight *
//					(M.bidRT.thetaBids[t][it].priceP*(M.RTtheta[scen][b][t] - M.DAtheta[n][b][t]));
//#endif
//			}
//		}*/
//	}
//	M.obj.setExpr(realTimeCost);
//	M.model.add(M.obj);
//	realTimeCost.end();
//
//}//END stocObjective()
//
//void nonanticipativeStateVector(PowerSystem sys, ClearingModel &M, ScenarioTree tree) {
//	char elemName[NAMESIZE];
//
//	int numNonTerminalNodes = tree.numNodes - (int)tree.stageNodes[tree.numStages - 1].size();
//	IloArray <IloNumVarArray> stateVector(M.env, numNonTerminalNodes);
//	M.nonAnticipative = IloArray<IloArray<IloRangeArray>>(M.env, numNonTerminalNodes);
//
//	/* Loop through all non-terminal nodes to add non-anticipativity constraints */
//	for (int n = 0; n < numNonTerminalNodes; n++) {
//		int t = tree.nodes[n].stage;
//		if (t != tree.numStages - 1) {
//			M.nonAnticipative[n] = IloArray<IloRangeArray>(M.env, (int)tree.nodes[n].scenThruNode.size());
//			stateVector[n] = IloNumVarArray(M.env);
//			vector<int> startIdx;
//			int offset = 0;
//
//			for (int s = 0; s < (int)tree.nodes[n].scenThruNode.size(); s++) {
//				M.nonAnticipative[n][s] = IloRangeArray(M.env);
//
//				/* Non-anticipative generation */
//				startIdx.push_back(offset);
//				for (int g = 0; g < sys.numGenerators; g++) {
//					if (s == 0) {
//						sprintf(elemName, "svG[%d][%d]", n, sys.generators[g].id);
//						stateVector[n].add(IloNumVar(M.env, 0, sys.generators[g].Pmax));
//						stateVector[n][offset].setName(elemName);
//						M.model.add(stateVector[n][offset]);
//						offset++;
//					}
//
//					IloExpr expr(M.env);
//
//					expr = M.DAgen[s][g][t] - stateVector[n][startIdx[0] + g];
//
//
//					sprintf(elemName, "naGen[%d][%d][%d]", s, sys.generators[g].id, t);
//
//					IloRange c(M.env, 0, expr, 0, elemName);
//					M.nonAnticipative[n][s].add(c);
//					M.model.add(M.nonAnticipative[n][s]);
//				}
//
//				/* Non-anticipative demand */
//				startIdx.push_back(offset);
//				for (int d = 0; d < sys.numLoads; d++) {
//					if (s == 0) {
//						sprintf(elemName, "svD[%d][%d]", n, sys.loads[d].id);
//						stateVector[n].add(IloNumVar(M.env, 0, sys.loads[d].Pd));
//						stateVector[n][offset].setName(elemName);
//						M.model.add(stateVector[n][offset]);
//						offset++;
//					}
//
//					IloExpr expr(M.env);
//					expr = M.DAdem[s][d][t] - stateVector[n][startIdx[1] + d];
//
//					sprintf(elemName, "naDem[%d][%d][%d]", s, sys.loads[d].id, t);
//
//					IloRange c(M.env, 0, expr, 0, elemName);
//					M.nonAnticipative[n][s].add(c);
//					M.model.add(M.nonAnticipative[n][s]);
//				}
//
//				/* Non-anticipative power flow */
//				/*startIdx.push_back(offset);
//				for ( int l = 0; l < sys.numLines; l++ ) {
//					if ( s == 0) {
//						sprintf(elemName, "svL[%d][%d]", n, sys.lines[l].id);
//						stateVector[n].add(IloNumVar (M.env, sys.lines[l].minCap, sys.lines[l].maxCap));
//						stateVector[n][offset].setName(elemName);
//						M.model.add(stateVector[n][offset]);
//						offset++;
//					}
//
//					IloExpr expr (M.env);
//					expr = M.DAflow[s][l][t] - stateVector[n][startIdx[2]+l];
//					sprintf(elemName, "naFlow[%d][%d][%d]", s, sys.lines[l].id, t);
//
//					IloRange c (M.env, 0, expr, 0, elemName);
//					M.nonAnticipative[n][s].add(c);
//					M.model.add(M.nonAnticipative[n][s]);
//				}*/
//
//				/* Non-anticipative bus angle */
//				/*startIdx.push_back(offset);
//				for ( int b = 0; b < sys.numBuses; b++ ) {
//					if ( s == 0) {
//						sprintf(elemName, "svB[%d][%d]", n, sys.buses[b].id);
//						stateVector[n].add(IloNumVar (M.env, sys.buses[b].minPhaseAngle, sys.buses[b].maxPhaseAngle));
//						stateVector[n][offset].setName(elemName);
//						M.model.add(stateVector[n][startIdx[3]+b]);
//						offset++;
//					}
//
//					IloExpr expr (M.env);
//					expr += M.DAtheta[s][b][t] - stateVector[n][startIdx[3]+b];
//					sprintf(elemName, "naTheta[%d][%d][%d]", s, sys.buses[b].id, t);
//
//					IloRange c (M.env, 0, expr, 0, elemName);
//					M.nonAnticipative[n][s].add(c);
//					M.model.add(M.nonAnticipative[n][s]);
//				}*/
//			}
//		}
//	}
//
//}//END nonanticipativeStateVector()
//
//void nonanticipativeMeanVector(PowerSystem sys, ClearingModel &M, ScenarioTree tree) {
//	char elemName[NAMESIZE];
//
//	int numNonTerminalNodes = tree.numNodes - (int)tree.stageNodes[tree.numStages - 1].size();
//	M.nonAnticipative = IloArray< IloArray < IloRangeArray>>(M.env, numNonTerminalNodes);
//
//	/* Loop through all non-terminal nodes to add non-anticipativity constraints */
//	for (int n = 0; n < numNonTerminalNodes; n++) {
//		if (tree.nodes[n].stage != tree.numStages - 1) {
//			int t = tree.nodes[n].stage;
//			M.nonAnticipative[n] = IloArray<IloRangeArray>(M.env, (int)tree.nodes[n].scenThruNode.size());
//
//			vector<double> scenProb;
//			for (int s = 0; s < (int)tree.nodes[n].scenThruNode.size(); s++) {
//				double condProb = tree.nodes[tree.stageNodes[tree.numStages - 1][tree.nodes[n].scenThruNode[s]]].prob / tree.nodes[n].prob;
//				scenProb.push_back(condProb);
//			}
//
//			for (int s = 0; s < (int)tree.nodes[n].scenThruNode.size(); s++) {
//				M.nonAnticipative[n][s] = IloRangeArray(M.env);
//				/* Non-anticipative generation */
//				for (int g = 0; g < sys.numGenerators; g++) {
//					IloExpr expr(M.env);
//					for (int ss = 0; ss < (int)tree.nodes[n].scenThruNode.size(); ss++) {
//						expr += scenProb[ss] * M.DAgen[ss][g][t];
//					}
//					sprintf(elemName, "naGen[%d][%d][%d]", s, sys.generators[g].id, t);
//
//					IloRange c(M.env, 0, expr - M.DAgen[s][g][t], 0, elemName);
//					M.nonAnticipative[n][s].add(c);
//					M.model.add(M.nonAnticipative[n][s]);
//				}
//
//				/* Non-anticipative demand */
//				for (int d = 0; d < sys.numLoads; d++) {
//					IloExpr expr(M.env);
//					for (int ss = 0; ss < (int)tree.nodes[n].scenThruNode.size(); ss++) {
//						expr += scenProb[ss] * M.DAdem[ss][d][t];
//					}
//					sprintf(elemName, "naDem[%d][%d][%d]", s, sys.loads[d].id, t);
//
//					IloRange c(M.env, 0, expr - M.DAdem[s][d][t], 0, elemName);
//					M.nonAnticipative[n][s].add(c);
//					M.model.add(M.nonAnticipative[n][s]);
//				}
//
//				/* Non-anticipative power flow */
//				/*for ( int l = 0; l < sys.numLines; l++ ) {
//					IloExpr expr (M.env);
//					for ( int ss = 0; ss < (int)tree.nodes[n].scenThruNode.size(); ss++ ) {
//						expr += scenProb[ss]*M.DAflow[ss][l][t];
//					}
//					sprintf(elemName, "naFlow[%d][%d][%d]", s, sys.lines[l].id, t);
//
//					IloRange c (M.env, 0, expr - M.DAflow[s][l][t], 0, elemName);
//					M.nonAnticipative[n][s].add(c);
//					M.model.add(M.nonAnticipative[n][s]);
//				}*/
//
//				/* Non-anticipative bus angle */
//				/*for ( int b = 0; b < sys.numBuses; b++ ) {
//					IloExpr expr (M.env);
//					for ( int ss = 0; ss < (int)tree.nodes[n].scenThruNode.size(); ss++ ) {
//						expr += scenProb[ss]*M.DAtheta[ss][b][t];
//					}
//					sprintf(elemName, "naTheta[%d][%d][%d]", s, sys.buses[b].id, t);
//
//					IloRange c (M.env, 0, expr - M.DAtheta[s][b][t], 0, elemName);
//					M.nonAnticipative[n][s].add(c);
//					M.model.add(M.nonAnticipative[n][s]);
//				}*/
//			}
//		}
//	}
//}//END nonanticipativeMeanVector()
//
//
