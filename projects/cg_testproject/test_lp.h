#ifndef TEST_MODEL
#define TEST_MODEL

#include <ilcplex/ilocplex.h>

class c_Test_Model : public IloModel {
	const IloEnv& o_env;
	IloObjective ilo_objective;

	void Objective();
	void CapacityConstraints();
public:
	IloNumVarArray var_x;
public:
	c_Test_Model(const IloEnv& env);

	IloNumVarArray XVars() const { return var_x; }
};

#endif