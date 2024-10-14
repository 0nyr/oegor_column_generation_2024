#include "utils.hpp"

std::vector<uint> convert_column_to_bin_vector(
    const IloNumArray& column
) {
    std::vector<uint> bin_vector;
    for (uint i = 0; i < column.getSize(); i++) {
        if (column[i] >= 1)
            bin_vector.push_back(i);
    }
    return std::move(bin_vector);
}

/**
 * Assertion function that prints a message and 
 * exits if the condition is false.
 */
void assertion(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "Assertion failed: " 
            << message << std::endl;
        exit(1);
    }
}