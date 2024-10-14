#include "bp_rmp.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
//   c_PricingMIP_Model
/////////////////////////////////////////////////////////////////////////////

c_BP_RMP_Model::c_BP_RMP_Model(const IloEnv& env, const c_BP_Instance& instance)
:	o_env(env),
	IloModel(env),
	o_instance(instance),
	ilo_constraints(env),
	ilo_bin_columns(env)
{
	// create model
	int num_items = o_instance.NumItems();

	// objective
	ilo_objective = IloMinimize(env);
	add(ilo_objective);

	//constraints
	IloNumArray rhs_array(env, num_items);
	for (int j = 0; j < num_items; j++)
		rhs_array[j] = 1.0;
	ilo_constraints.add(IloRangeArray(env, rhs_array, IloInfinity));
	add(ilo_constraints);
}

/// @brief Initialize the RMP with trivial columns
/// This is trivial. We just add a column for each item.
void c_BP_RMP_Model::InitializeRMP()
{
	vector<vector<int>> bins;
	//add trivial columns to the RMP
	for (int j = 0; j < o_instance.NumItems(); j++)
	{
		vector<int> bin;
		bin.push_back(j);
		bins.push_back(bin);
	}
	AddColumns(bins);
}

void c_BP_RMP_Model::AddColumns(const vector<vector<int>>& bins)
{
	for (int i = 0; i < (int)bins.size(); ++i)
	{
		auto& bin = bins[i];
		IloNumArray items_of_bin(o_env, o_instance.NumItems());
		for (auto item : bin)
			items_of_bin[item] = 1.0;
		ilo_bin_columns.add(IloNumVar(ilo_objective(1.0) + ilo_constraints(items_of_bin)));

		all_generated_bins.push_back(bin);
	}
}

void c_BP_RMP_Model::AddColumns(const vector<IloNumArray>& bins)
{
	for (int i = 0; i < (int)bins.size(); ++i) {
		ilo_bin_columns.add(IloNumVar(ilo_objective(1.0) + ilo_constraints(bins[i])));
		
		// convert IloNumArray to vector<int>
		vector<int> bin;
		for (int j = 0; j < o_instance.NumItems(); j++) {
			if (bins[i][j] >= 1)
				bin.push_back(j);
		}
		all_generated_bins.push_back(std::move(bin));

	}
}