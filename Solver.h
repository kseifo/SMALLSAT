#pragma once
#include "Types.h"
#include <vector>

class Solver
{
private:
	int nVars;
	int nClauses;

	std::vector<Clause> clauses;
	std::vector<Lit> decisionVars;
	std::vector<Lit> trail;

public:
	Solver() = default;
	~Solver() = default;

	inline void setVars(int v) { nVars = v; };
	inline int getVars() const { return nVars; };
	inline void setClauses(int c) { nClauses = c; };
	inline const std::vector<Clause>& getClauses() const { return clauses; }
	inline void addClause(Clause c) { clauses.push_back(c); };
	int solve();

};