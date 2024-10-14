#include <sstream>

#include "stopwatch.h"
#include "bp_instance.h"
#include "bp_rmp.h"
#include "bp_pricing_mip.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// main
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		cout << "Current command arguments: ";
		for (int i = 0; i < argc; i++)
			cout << argv[i] << " ";
		cout << endl;
		cout << "Correct: " << argv[0] << " <instance_file> <output_folder>" << endl;
		cout << "Please change the command arguments in project properties!" << endl;
		throw;
	}

	///////////////////////////////////////////////////////////////////////////
	// general data and information
	///////////////////////////////////////////////////////////////////////////

	// create and read instance
	string filename = argv[1];
	c_BP_Instance bin_packing_instance = c_BP_Instance(filename);

	// output paths
	string output_folder = argv[2];
	string model_rmp_file = output_folder + "/model_rmp.lp";
	string model_pricing_file = output_folder + "/model_pricing.lp";

	// some global data
	int num_items = bin_packing_instance.NumItems();
	double rmp_objective = BigM * BigM;
	double best_lb = 0.0; // TODO: not used 
	bool is_optimal = false;
	int iteration = 0;

	// timers
	c_Stopwatch timer_overall;
	c_Stopwatch timer_rmp;
	c_Stopwatch timer_pp;

	///////////////////////////////////////////////////////////////////////////
	// settings and parameter choices
	///////////////////////////////////////////////////////////////////////////

	bool b_debug = true;
	int i_time_limit_seconds = 600;

	///////////////////////////////////////////////////////////////////////////
	// initialize CPLEX
	///////////////////////////////////////////////////////////////////////////
	IloEnv env;
	// set cplex silent
	env.setOut(env.getNullStream());

	///////////////////////////////////////////////////////////////////////////
	// create RMP model
	///////////////////////////////////////////////////////////////////////////

	// model is built once at the beginning and used for the whole solution process
	c_BP_RMP_Model rmp_model = c_BP_RMP_Model(env, bin_packing_instance);
	// RMP initialization, prior to CG process
	rmp_model.InitializeRMP();
	IloCplex rmp_solver(rmp_model);

	// set time limit and threads
	rmp_solver.setParam(IloCplex::Param::TimeLimit, i_time_limit_seconds);
	rmp_solver.setParam(IloCplex::Param::Threads, 1);
	// export model
	rmp_solver.extract(rmp_model);
	if (b_debug)
		rmp_solver.exportModel(model_rmp_file.c_str());
	// set cplex silent
	rmp_solver.setOut(env.getNullStream());
	rmp_solver.setWarning(env.getNullStream());

	///////////////////////////////////////////////////////////////////////////
	// exchange information between rmp and pricing problem 
	///////////////////////////////////////////////////////////////////////////

	IloNumArray dual_prices(env, num_items);//dual price, array of floating point numbers
	IloNumArray new_bin(env, num_items);//solution of pricing (MIP)
	vector<IloNumArray> bins_to_add;//new bins to add to RMP

	///////////////////////////////////////////////////////////////////////////
	// column-generation procedure start
	///////////////////////////////////////////////////////////////////////////
	timer_overall.Start();
	while (timer_overall.TotalSeconds() < i_time_limit_seconds)
	{
		iteration++;
		bins_to_add.clear();

		///////////////////////////////////////////////////////////////////////////
		// RMP
		///////////////////////////////////////////////////////////////////////////
		// optimize RMP
		rmp_solver.extract(rmp_model);
		if(b_debug)
			rmp_solver.exportModel(model_rmp_file.c_str());
		timer_rmp.Restart();
		rmp_solver.solve();
		timer_rmp.Stop();
		if (b_debug)
			cout << "[iteration: " << iteration << "] current objective: " << rmp_solver.getObjValue() << endl;
		rmp_objective = rmp_solver.getObjValue();
		// get dual prices
		for (int c = 0; c < num_items; c++)
			dual_prices[c] = rmp_solver.getDual(rmp_model.Constraint(c)); // get dual price of the constraint associated with item c

		///////////////////////////////////////////////////////////////////////////
		// pricing start
		// pricing MIP -> get new columns ! Heart of the column generation
		//TODO: add the variables, the constrains, the objective function, get the solution
		// the bin packing (binary knapsack problem) problem is solved here
		// selection of items with negative reduced cost that respects the bin capacity
		///////////////////////////////////////////////////////////////////////////
		timer_pp.Restart();
		double rdc = 0.0;
		double reduced_cost = -1.0;

		// solve pricing MIP
		c_BP_Pricing_MIP_Model pricing_model = c_BP_Pricing_MIP_Model(
			env, bin_packing_instance, dual_prices
		);

		// create the solver for the pricing problem
		IloCplex pricing_solver(pricing_model);

		// set time limit and threads
		pricing_solver.setParam(IloCplex::Param::TimeLimit, i_time_limit_seconds);
		pricing_solver.setParam(IloCplex::Param::Threads, 1);
		pricing_solver.extract(pricing_model);
		if (b_debug)
			pricing_solver.exportModel(model_pricing_file.c_str());
		
		// solve the pricing problem
		pricing_solver.setOut(env.getNullStream());
		pricing_solver.setWarning(env.getNullStream());
		pricing_solver.solve();

		// get the solution (new column)
		pricing_solver.getValues(new_bin, pricing_model.XVars());
		
		// add the new column to the RMP
		rdc = 1 - pricing_solver.getObjValue();

		timer_pp.Stop();
		// if no new columns with negative reduced cost, then stop (optimality)
		if (!(rdc < -EPSILON))
		{	
			is_optimal = true;
			break;
		}
		
		bins_to_add.push_back(new_bin);
		rmp_model.AddColumns(bins_to_add);
		
		///////////////////////////////////////////////////////////////////////////
		// pricing end
		///////////////////////////////////////////////////////////////////////////
	}
	timer_overall.Stop();

	///////////////////////////////////////////////////////////////////////////
	// column-generation procedure end
	///////////////////////////////////////////////////////////////////////////

	// update results from last iteration
	if (rmp_solver.getObjValue())
		rmp_objective = rmp_solver.getObjValue();
	if (rmp_objective > best_lb)
		best_lb = rmp_objective;

	cout << "total solution time: " << timer_overall.TotalSeconds() << " s" << endl;
	cout << "current root_objective = " << rmp_objective << endl;
	cout << "best_lb = " << best_lb << endl;
	if (is_optimal) {
		cout << "Solved to optimality" << endl;
	
		// Print the solution (the bins)
		// Get selected bins from RMP solver ilo_bin_columns and print them
		IloNumArray binValues(env);
		rmp_solver.getValues(binValues, rmp_model.BinVars());
		for (int i = 0; i < binValues.getSize(); i++) {
			if (binValues[i] > 0.5) {
				cout << "Bin " << i;
				printf(" [%.2f]", binValues[i]);
				cout << ": {";
				IloNumArray& bin = rmp_model.all_generated_bins[i];
				for (int j = 0; j < num_items; j++) {
					if (bin[j] == 1) {
						cout << j << " ";
					}
				}
				cout << "}" << endl;
			}
		}
	} else {
		cout << "NOT solved to optimality" << endl;
	}
	string output_file = output_folder + "/solutions.csv";
	ifstream test_file(output_file);
	if (!test_file.good())
	{
		ofstream myfile;
		myfile.open(output_file);
		myfile << "instance;";
		myfile << "time_in_seconds;";
		myfile << "is_optimal;";
		myfile << "best_lb;";
		myfile << "time pricing;";
		myfile << "time rmp;";
		myfile << "\n";
		myfile.close();
	}

	ofstream myfile;
	myfile.open(output_file, std::ios_base::app);
	myfile << filename << ";";
	myfile << timer_overall.TotalSeconds() << ";";
	myfile << (is_optimal ? "true" : "false") << ";";
	myfile << best_lb << ";";
	myfile << timer_pp.TotalSeconds() << ";";
	myfile << timer_rmp.TotalSeconds() << ";";
	myfile << "\n";
	myfile.close();

	env.end();
	return 0;
};