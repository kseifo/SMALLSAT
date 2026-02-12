#pragma once
#include "Solver.h"
#include "Types.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
inline void parseFile(Solver &S)
{
    std::string line;
    std::ifstream file("problems/example.cnf");
    if (!file.is_open())
    {
        std::cerr << "Could not open file";
        return;
    }
    while (std::getline(file, line))
    {
        // Skip first, comment and empty lines
        if (line.empty() || line[0] == 'c' || line[0] == 'p')
            continue;

        std::stringstream ss(line);
        int lit;
        std::vector<Lit> clause;

        while (ss >> lit)
        {
            // End of clause
            if (lit == 0)
                break;

            // Construct lit
            int var = std::abs(lit);
            bool neg = (lit < 0);

            clause.push_back(mkLit(var, neg));
        }

        Clause c{clause};
        S.addClause(c);
    }

    file.close();
}
