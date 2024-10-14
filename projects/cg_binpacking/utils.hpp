#pragma once

#include <vector>

#include <ilcplex/ilocplex.h>

std::vector<uint> convert_column_to_bin_vector(
    const IloNumArray& column
);


void assertion(bool condition, const std::string& message);