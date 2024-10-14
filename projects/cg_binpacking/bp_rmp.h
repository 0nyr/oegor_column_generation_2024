#ifndef BP_RMP_MODEL
#define BP_RMP_MODEL

#include <ilcplex/ilocplex.h>
#include "bp_instance.h"

class c_BP_RMP_Model : public IloModel {
	const IloEnv& o_env;
	const c_BP_Instance& o_instance;

	IloObjective ilo_objective;
	IloRangeArray ilo_constraints;
	IloNumVarArray ilo_bin_columns;

public:
	std::vector<std::vector<int>> all_generated_bins;

	c_BP_RMP_Model(const IloEnv& env, const c_BP_Instance& instance);

	IloRange& Constraint(int num) { return ilo_constraints[num]; }

	void InitializeRMP();
	void AddColumns(const std::vector<std::vector<int>>& bins);
	void AddColumns(const std::vector<IloNumArray>& bins);

	IloNumVarArray BinVars() const { return ilo_bin_columns; }
	int NumBinCols() { return (int)ilo_bin_columns.getSize(); }
};

#endif