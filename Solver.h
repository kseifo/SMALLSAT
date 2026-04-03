#pragma once
#include "Types.h"
#include <vector>

class Solver
{
private:
	int nVars;
	int nClauses;
	int currentLevel = 0;

	std::vector<Clause> clauses;
	std::vector<Lit> trail;
	std::vector<LitVal> assigns;
	std::vector<int> trailAtLevel;
	std::vector<Lit> decisionStack;

public:
	Solver() = default;
	~Solver() = default;

	inline void setVars(int v) { nVars = v; };
	inline int getVars() const { return nVars; };
	inline int getNumClauses() { return clauses.size(); };
	inline const std::vector<Clause> &getClauses() const { return clauses; }
	inline void addClause(Clause c) { clauses.push_back(c); };
	bool solve();
	void setAssigns(int size);
	void newDecisionLevel();
	Lit makeDecision();
	void assign(Lit lit);
	bool propagate();
	void undoOne();
	void backtrackTo(int level);
	bool isLitTrue(Lit lit);
	void printTrail();
};