#ifndef BP_PRICING_MIP_MODEL
#define BP_PRICING_MIP_MODEL

#include <ilcplex/ilocplex.h>
#include "bp_instance.h"

/// @brief Model for the pricing problem
/// Binary Knapsack Problem
class c_BP_Pricing_MIP_Model : public IloModel {
	const IloEnv& o_env;
	const c_BP_Instance& o_instance;

	// variables of the model
	IloObjective ilo_objective;
	IloRangeArray ilo_constraints;
	IloNumVarArray x; // new bin ?!

public:
	c_BP_Pricing_MIP_Model(
		const IloEnv& env, 
		const c_BP_Instance& instance,
		const IloNumArray& dual_prices
	);

	IloNumVarArray XVars() const { return x; }

};

#endif