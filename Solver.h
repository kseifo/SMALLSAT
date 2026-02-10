#pragma once
#include "Types.h"
#include <vector>

class Solver {
private:
	int nVars;
	int nClauses;

	std::vector<Clause> clauses;
	std::vector<Lit> decisionVars;
	std::vector<Lit> trail;

public:
	Solver() {};
	~Solver();

	inline void setVars(int v) { nVars = v; };
	inline int getVars() const { return nVars; };
	inline void setClauses(int c) { nClauses = c; };
	inline int getClauses() const { return nClauses; };

};