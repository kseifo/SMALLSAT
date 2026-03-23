#include "Solver.h"
#include "Types.h"
#include "Parse.h"

void Solver::setAssigns(int size)
{
    assigns.assign(size, -1);
}

Lit Solver::makeDecision()
{
    for (Var v = 0; v < nVars; v++)
    {
        if (assigns[v] == -1)
        {
            return mkLit(v, false);
        }
    }
}

void Solver::newDecisionLevel()
{
    trailAtLevel.push_back(trail.size());
    currentLevel++;
}

int Solver::solve()
{
    return 1;
}

int main()
{
    Solver *s = new Solver();
    parseFile(*s);

    return 0;
}