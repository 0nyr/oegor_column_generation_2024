#include "bp_pricing_mip.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
//   c_BP_Pricing_MIP_Model
/////////////////////////////////////////////////////////////////////////////

c_BP_Pricing_MIP_Model::c_BP_Pricing_MIP_Model(
	const IloEnv& env, 
	const c_BP_Instance& instance,
	const IloNumArray& dual_prices
):	o_env(env),
	IloModel(env),
	o_instance(instance)
{
	// define variables
	x = IloNumVarArray(env, o_instance.NumItems(), 0, 1, ILOINT);
	ilo_constraints = IloRangeArray(env);

	// objective function definition
	IloExpr objective_function(env);
	for (int j = 0; j < o_instance.NumItems(); j++)
		objective_function += dual_prices[j] * x[j];

	ilo_objective = IloMaximize(env, objective_function);
	add(ilo_objective); // add to the solver model

	// constraints
	IloExpr bin_capacity(env);
	for (int i = 0; i < o_instance.NumItems(); i++)
	{
		bin_capacity += o_instance.Weight(i) * x[i];
	}
	ilo_constraints.add(
		// equivalent to sum(w_j * x_j) <= C
		IloRange(env, 0, bin_capacity, o_instance.Capacity())
	);
	add(ilo_constraints); // add to the solver model
}