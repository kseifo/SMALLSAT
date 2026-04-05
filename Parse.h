#pragma once
#include "Solver.h"
#include "Types.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
inline void parseFile(Solver &S, std::string problemName)
{
    std::string line;
    std::ifstream file(problemName);
    int numVars, numClauses;
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << problemName << std::endl;
        exit(1);
    }

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == 'c')
            continue;
        std::stringstream ss(line);

        // Handle header
        if (line[0] == 'p')
        {
            std::string tmp, format;
            ss >> tmp >> format >> numVars >> numClauses;

            S.setVars(numVars);
            continue;
        }
        int lit;
        std::vector<Lit> clause;

        while (ss >> lit)
        {
            // End of clause
            if (lit == 0)
                break;

            // Construct lit
            int var = std::abs(lit) - 1;
            bool neg = (lit < 0);

            clause.push_back(mkLit(var, neg));
        }

        Clause c{clause};
        S.addClause(c);
    }

    S.setAssigns(numVars);
    file.close();
}
