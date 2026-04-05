#include "Solver.h"
#include "Types.h"
#include "Parse.h"

void Solver::setAssigns(int size)
{
    assigns.assign(size, LitVal::UNASSIGNED);
}

bool Solver::isLitTrue(Lit lit)
{
    return (assigns[lit.var()] == LitVal::TRUE && !lit.sign()) || (assigns[lit.var()] == LitVal::FALSE && lit.sign());
}

Lit Solver::makeDecision()
{
    // Assign negative to all decision variables
    for (Var v = 0; v < nVars; v++)
    {
        if (assigns[v] == LitVal::UNASSIGNED)
        {
            return mkLit(v, 1);
        }
    }
    // If all assigned, return undefined to indicate sat
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
            else if (isLitTrue(lit))
            {
                clauseSatisfied = true;
                break;
            }
        }

        if (clauseSatisfied)
            continue;
        if (numUnassigned == 0)
            return false;
        // Naive recursive approach
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
    // At decision level 0, the trail is empty
    trailAtLevel.push_back(0);

    while (true)
    {
        if (!propagate())
        {
            // Conflict at root is unsat
            if (currentLevel == 0)
                return false;

            // Backtrack to the previous decision level first
            backtrackTo(currentLevel - 1);

            Lit top = decisionStack.back();
            decisionStack.pop_back();

            if (top.sign())
            {
                // Was negative branch, try positive
                Lit postop = mkLit(top.var(), 0);
                decisionStack.push_back(postop);
                newDecisionLevel();
                assign(postop);
            }
            else
            {
                // Was positive branch so both branches exhausted, backtrack
                while (!decisionStack.empty() && !decisionStack.back().sign())
                {
                    decisionStack.pop_back();
                    backtrackTo(currentLevel - 1);
                }

                if (decisionStack.empty())
                    return false;

                // Flip the last negative decision to positive
                Lit top2 = decisionStack.back();
                decisionStack.pop_back();
                Lit postop = mkLit(top2.var(), 0);
                decisionStack.push_back(postop);
                newDecisionLevel();
                assign(postop);
            }
        }
        else
        {
            // Make a new decision
            Lit dec = makeDecision();

            // If a complete assignment is made and no conflict occurs, return sat
            if (dec == Lit::undef())
                return true;

            newDecisionLevel();
            assign(dec);
            decisionStack.push_back(dec);
        }
    }
}

void Solver::printTrail()
{
    for (Lit l : trail)
    {
        std::cout << l.var() + 1 << "->" << (int)assigns[l.var()] << "\n";
    }
}

int main(int argc, char *argv[])
{
    Solver s;

    if (argc < 2)
    {
        std::cerr << "Usage: ./solver <input.cnf>" << std::endl;
        return 1;
    }

    parseFile(s, argv[1]);

    bool res = s.solve();
    if (res)
    {
        std::cout << "SAT" << std::endl;
        s.printTrail();
    }
    else
        std::cout << "UNSAT" << std::endl;
    return 0;
}