#include "stopwatch.h"
#include "test_lp.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// main
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	///////////////////////////////////////////////////////////////////////////
	// initialize CPLEX
	///////////////////////////////////////////////////////////////////////////
	IloEnv env;
	env.setOut(env.getNullStream()); //set quiet

	///////////////////////////////////////////////////////////////////////////
	// create test model
	///////////////////////////////////////////////////////////////////////////

	c_Test_Model test_model = c_Test_Model(env);
	IloCplex test_solver(test_model);
	test_solver.setOut(env.getNullStream()); //set quiet
	test_solver.setWarning(env.getNullStream()); //set quiet

	///////////////////////////////////////////////////////////////////////////
	// solve test model
	///////////////////////////////////////////////////////////////////////////
	c_Stopwatch timer;
	timer.Start();
	test_solver.extract(test_model);
	test_solver.solve();
	timer.Stop();

	if (test_solver.getStatus() != IloAlgorithm::Optimal)
	{
		cout << test_solver.getStatus() << endl;
		cout << "Error: test problem can't be solved";
		throw;
	}
	else
	{
		cout << "total solution time: " << timer.TotalSeconds() << "[s]" << endl;
		cout << "optimal solution value: " << test_solver.getObjValue() << endl;
		IloNumArray sol_values(env, 2);
		test_solver.getValues(sol_values, test_model.XVars());
		cout << "x_1 = " << sol_values[0] << ";  " << "x_2 = " << sol_values[1] << endl;
	}

	env.end();
	return 0;
};