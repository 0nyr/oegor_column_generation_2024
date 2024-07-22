#include "bp_pricing_mip.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
//   c_BP_Pricing_MIP_Model
/////////////////////////////////////////////////////////////////////////////

c_BP_Pricing_MIP_Model::c_BP_Pricing_MIP_Model(const IloEnv& env, const c_BP_Instance& instance)
:	o_env(env),
	IloModel(env),
	o_instance(instance)
{
}