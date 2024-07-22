#ifndef BP_PRICING_MIP_MODEL
#define BP_PRICING_MIP_MODEL

#include <ilcplex/ilocplex.h>
#include "bp_instance.h"

class c_BP_Pricing_MIP_Model : public IloModel {
	const IloEnv& o_env;
	const c_BP_Instance& o_instance;

public:
	c_BP_Pricing_MIP_Model(const IloEnv& env, const c_BP_Instance& instance);

};

#endif