#include "Solver.h"
#include "Types.h"
#include "Parse.h"

void Solver::setAssigns(int size)
{
    assigns.assign(size, LitVal::UNASSIGNED);
}

Lit Solver::makeDecision()
{
    for (Var v = 0; v < nVars; v++)
    {
        if (assigns[v] == LitVal::UNASSIGNED)
        {
            return mkLit(v, false);
        }
    }
    return Lit::undef();
}

void Solver::newDecisionLevel()
{
    trailAtLevel.push_back(trail.size());
    currentLevel++;
}

void Solver::assign(Lit lit)
{
    Var v = lit.var();
    assigns[v] = lit.sign() ? LitVal::FALSE : LitVal::TRUE;
    trail.push_back(lit);
}

bool Solver::propagate()
{
    for (auto &clause : clauses)
    {
        int numUnassigned = 0;
        Lit lastUnassigned;
        bool clauseSatisfied = false;

        for (Lit lit : clause)
        {
            if (assigns[lit.var()] == LitVal::UNASSIGNED)
            {
                numUnassigned++;
                lastUnassigned = lit;
            }
            else if ((assigns[lit.var()] == LitVal::TRUE && !lit.sign()) || (assigns[lit.var()] == LitVal::FALSE && lit.sign()))
            {
                clauseSatisfied = true;
                break;
            }
        }

        if (clauseSatisfied)
            continue;
        if (numUnassigned == 0)
            return false;
        if (numUnassigned == 1)
        {
            assign(lastUnassigned);
            return propagate();
        }
    }
    return true;
}

void Solver::undoOne()
{
    Lit lit = trail.back();
    trail.pop_back();
    assigns[lit.var()] = LitVal::UNASSIGNED;
}

void Solver::backtrackTo(int level)
{
    while (trail.size() > (size_t)trailAtLevel[level])
    {
        undoOne();
    }
    trailAtLevel.resize(level);
    currentLevel = level;
}

bool Solver::solve()
{
    while (true)
    { // Conflict
        if (!propagate())
        {
            // If a conflict occurs at root, unsat
            if (currentLevel == 0)
            {
                return false;
            }
            backtrackTo(currentLevel - 1);
        }
        else
        {
            Lit dec = makeDecision();
            if (dec == Lit::undef())
            {
                return true;
            }
            newDecisionLevel();
            assign(dec);
        }
    }
}

int main()
{
    Solver s;
    parseFile(s);

    bool res = s.solve();
    if (res)
    {
        std::cout << "SAT" << std::endl;
    }
    else
    {
        std::cout << "UNSAT" << std::endl;
    }
    return 0;
}