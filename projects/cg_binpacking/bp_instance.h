#ifndef BP_INSTANCE
#define BP_INSTANCE

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#define EPSILON 1.0e-6
#define BigM 1.0e6

class c_BP_Instance {
	//basic data
	std::string s_instance_name;
	int i_num_items;
	int i_capacity;
	std::vector<int> a_weights;
public:
	c_BP_Instance(std::string filename);
	virtual ~c_BP_Instance() {}
	// basic data, getter
	std::string InstanceName() const { return s_instance_name; }
	int Capacity() const { return i_capacity; }
	int NumItems() const { return i_num_items; }
	int Weight(int i) const { return a_weights[i]; }
};

#endif // of BP_INSTANCE