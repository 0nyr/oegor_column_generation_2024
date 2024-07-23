# Bin Packing

In this example, we use 2 models in CPLEX, one from the RMP, and one for the Pricing Problem.

### Compile and run

1. Go in the the root dir of the project.
2. Generate Makefile using `cmake .`
3. Make executable with `make`
4. Run executable `./CG_BinPacking`, example of usage: `./CG_BinPacking instances/Scholl_1/N1C1W1_A.txt output/`

```shell
 ❮onyr ★ nixos❯ ❮cg_binpacking❯❯ ./CG_BinPacking instances/Scholl_1/N1C1W1_A.txt output/
Default variable names x1, x2 ... being created.
Default row names c1, c2 ... being created.
1 50
total solution time: 0.000522331[s]
current root_objective = 50
best_lb = 50
```
