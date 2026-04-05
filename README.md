# SMALLSAT

The goal of this project is to strengthen my understanding of SAT solvers by building a CDCL-based solver from scratch. Alongside the core solving capabilities that CDCL enables, I plan to implement a number of optimizations, including:

- The two-watched literal scheme
- VSIDS variable weighting
- Restarts
- Symmetry breaking

> **Note:** The solver currently implements DPLL. CDCL and the optimizations above are planned for the future.

## Installation

To build the project, run `make` from the root of the repo. To remove the build, run `make clean`.

## Usage

After cloning the repo, run the solver with a DIMACS CNF file. Note that variables must be numbered contiguously from 1 to N:

```bash
make
./bin/solver <input.cnf>
```

### Input Format

The solver expects input in [DIMACS CNF format](https://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html). For example, a formula with 2 variables and 2 clauses would look like:

```
p cnf 2 2
1 2 0
-1 2 0
```

Each line is a clause, literals are space-separated, and 0 marks the end of a clause. Also, lines beginning with a c are comment lines.
