# Bin Packing

In this example, we use 2 models in CPLEX, one from the RMP, and one for the Pricing Problem, to solve Bin Packing.

### Compile and run

1. Go in the root dir of the project.
2. Generate Makefile using `cmake .`
3. Make executable with `make`
4. Run executable `./CG_BinPacking`, example of usage: `./CG_BinPacking instances/Scholl_1/N1C1W1_A.txt output/`
5. Check results in `output/`

```shell
 ❮onyr ★ nixos❯ ❮oegor_column_generation_2024❯❯ cd projects/cg_binpacking/
 ❮onyr ★ nixos❯ ❮cg_binpacking❯❯ ./CG_BinPacking instances/Scholl_1/N1C1W1_A.txt output/
Default variable names x1, x2 ... being created.
Default row names c1, c2 ... being created.
1 50
total solution time: 0.000522331[s]
current root_objective = 50
best_lb = 50
```

### Bin Packing Problem

The **Bin Packing Problem (BPP)** is a combinatorial optimization problem where the goal is to pack a set of items of varying sizes into the fewest number of fixed-capacity bins. It is classified as an NP-hard problem, meaning that finding the optimal solution is computationally challenging as the size of the problem increases.

### Problem Definition:

Given:

* A set of items, each with a specific weight.
* A set of bins, each with a fixed capacity.

Objective:

* **Minimize** the number of bins used to pack all the items, ensuring that the total size of the items in each bin does not exceed the bin's capacity.

Instance input files format:

```txt
Number_of_items
Bin_capacity
[...] list of weights for each items
```

### Column Generation

Valid bins are **columns**, i.e. non-empty sets of items whose sum of weight doesn't exceed the bin capacity.

Withing CP-Optimizer, each column is an array of *Number_of_items* numbers, where 1.0 at index i indicates the presence of the item i in the bin.

Note that each item, due to its weight, has a constraint in the RMP, and thus, a dual price associated to this constraint in the RMP.
```cpp
// get dual prices
for (int c = 0; c < num_items; c++)
    dual_prices[c] = rmp_solver.getDual(rmp_model.Constraint(c));
```

### Notes about CP-Optimizer

+ `IloNumArray`: fixed-sized array of integers
