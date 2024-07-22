#include "bp_instance.h"

using namespace std;

c_BP_Instance::c_BP_Instance(string filename)
	: s_instance_name(filename),
	i_num_items(-1),
	i_capacity(-1)
{
	ifstream infile(filename);
	if (!infile.good())
	{
		cout << "File " << filename << " cannot be opened.  Serious error." << endl;
		throw;
	}
	// number of items
	infile >> i_num_items;
	// capacity
	infile >> i_capacity;
	// items
	a_weights.resize(i_num_items);
	for (int i = 0; i < i_num_items; i++)
		infile >> a_weights[i];
	//make sure that items are sorted
	sort(a_weights.begin(), a_weights.end(), greater<int>());
}