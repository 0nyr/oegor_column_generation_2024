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
	c_BP_Instance inst_bp = c_BP_Instance(filename);

	// output paths
	string output_folder = argv[2];
	string model_rmp_file = output_folder + "/model_rmp.lp";
	string model_pricing_file = output_folder + "/model_pricing.lp";

	// some global data
	int num_items = inst_bp.NumItems();
	double rmp_objective = BigM * BigM;
	double best_lb = 0.0;
	bool b_optimal = false;
	int iteration = 0;

	// timers
	c_Stopwatch timer_overall;
	c_Stopwatch timer_rmp;
	c_Stopwatch timer_pp;

	///////////////////////////////////////////////////////////////////////////
	// settings and parameter choices
	///////////////////////////////////////////////////////////////////////////

	bool b_debug = true;
	int i_time_limit_seconds = 60;

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
	c_BP_RMP_Model rmp_model = c_BP_RMP_Model(env, inst_bp);
	// RMP initialization
	rmp_model.InitializeRMP();
	IloCplex rmp_solver(rmp_model);

	// set time limit and threads
	rmp_solver.setParam(IloCplex::TiLim, i_time_limit_seconds);
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

	IloNumArray dual_price(env, num_items);//dual price
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
			cout << iteration << " " << rmp_solver.getObjValue() << endl;
		rmp_objective = rmp_solver.getObjValue();
		// get dual prices
		for (int c = 0; c < num_items; c++)
			dual_price[c] = rmp_solver.getDual(rmp_model.Constraint(c));

		///////////////////////////////////////////////////////////////////////////
		// pricing start
		///////////////////////////////////////////////////////////////////////////
		timer_pp.Restart();
		double rdc = 0.0;

		// pricing MIP


		timer_pp.Stop();
		if (!(rdc < -EPSILON))
			break;
		///////////////////////////////////////////////////////////////////////////
		// pricing end
		///////////////////////////////////////////////////////////////////////////
	}
	timer_overall.Stop();

	///////////////////////////////////////////////////////////////////////////
	// column-generation procedure end
	///////////////////////////////////////////////////////////////////////////

	if (rmp_solver.getObjValue())
		rmp_objective = rmp_solver.getObjValue();
	if (rmp_objective > best_lb)
		best_lb = rmp_objective;

	cout << "total solution time: " << timer_overall.TotalSeconds() << "[s]" << endl;
	cout << "current root_objective = " << rmp_objective << endl;
	cout << "best_lb = " << best_lb << endl;
	if (b_optimal)
		cout << "Solved to optimality" << endl;

	string output_file = output_folder + "/solutions.csv";
	ifstream test_file(output_file);
	if (!test_file.good())
	{
		ofstream myfile;
		myfile.open(output_file);
		myfile << "instance;";
		myfile << "time[s];";
		myfile << "opt?;";
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
	myfile << (b_optimal ? "true" : "false") << ";";
	myfile << best_lb << ";";
	myfile << timer_pp.TotalSeconds() << ";";
	myfile << timer_rmp.TotalSeconds() << ";";
	myfile << "\n";
	myfile.close();

	env.end();
	return 0;
};