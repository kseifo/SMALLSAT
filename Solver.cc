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
    while (trail.size() > (size_t)trailAtLevel[level + 1])
    {
        undoOne();
    }
    trailAtLevel.resize(level + 1);
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

            // Pop decisions, undo each level until either:
            // Negative literal is found or stack is exhausted
            Lit found = Lit::undef();
            while (!decisionStack.empty())
            {
                Lit top = decisionStack.back();
                decisionStack.pop_back();
                backtrackTo(currentLevel - 1);
                if (top.sign())
                {
                    found = top;
                    break;
                }
            }

            if (found == Lit::undef())
                return false;

            Lit flipped = mkLit(found.var(), 0);
            decisionStack.push_back(flipped);
            newDecisionLevel();
            assign(flipped);
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

void Solver::printModel()
{
    for (int i = 0; i < nVars; ++i)
    {
        std::cout << i + 1 << "->" << (assigns[i] == LitVal::TRUE ? "TRUE" : "FALSE") << std::endl;
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
        s.printModel();
    }
    else
        std::cout << "UNSAT" << std::endl;
    return 0;
}