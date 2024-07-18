#include "test_lp.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
//   c_Test_Model
/////////////////////////////////////////////////////////////////////////////

c_Test_Model::c_Test_Model(const IloEnv& env)
:	o_env(env),
	IloModel(env),
	ilo_objective(IloMaximize(env)),
	var_x(env, 2, 0, IloInfinity, ILOINT)
{
	// create model
	// objective
	Objective();
	// constraints
	CapacityConstraints();
}

void c_Test_Model::Objective()
{
	IloExpr lhs(o_env);
	lhs = 12 * var_x[0] + 8 * var_x[1];
	ilo_objective.setExpr(lhs);
	add(ilo_objective);
}

void c_Test_Model::CapacityConstraints()
{
	IloExpr lhs(o_env);
	//Constr 1
	lhs = 4 * var_x[0];
	lhs += 2 * var_x[1];
	int rhs = 80;
	add(IloConstraint(lhs <= rhs));
	//Constr 2
	lhs = 2 * var_x[0] + 3 * var_x[1];
	rhs = 100;
	add(IloConstraint(lhs <= rhs));
	//Constr 3
	lhs = 5 * var_x[0] + var_x[1];
	rhs = 75;
	add(IloConstraint(lhs <= rhs));
}
