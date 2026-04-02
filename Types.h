#pragma once
#include <iostream>
#include <vector>
typedef int Var;

struct Lit
{
    int x;

    Var var() const { return x >> 1; }
    bool sign() const { return x & 1; }

    Lit operator~() const { return Lit{x ^ 1}; }
    static Lit undef() { return Lit{0}; }
};

enum class LitVal
{
    UNASSIGNED = 0,
    FALSE = -1,
    TRUE = 1
};

inline Lit mkLit(Var v, bool sign)
{
    return Lit{(v << 1) | (sign ? 1 : 0)};
}

inline bool operator==(Lit a, Lit b) { return a.x == b.x; }
inline bool operator!=(Lit a, Lit b) { return a.x != b.x; }

class Clause
{
private:
    std::vector<Lit> lits;

public:
    explicit Clause(const std::vector<Lit> &l) : lits(l) {}

    bool isEmpty() const { return lits.empty(); }
    size_t size() const { return lits.size(); }

    const Lit &operator[](size_t i) const { return lits[i]; }
    const std::vector<Lit> &literals() const { return lits; }

    const Lit *begin() const { return lits.data(); }
    const Lit *end() const { return lits.data() + lits.size(); }

    void print() const
    {
        for (const Lit &l : lits)
        {
            if (l.sign())
            {
                std::cout << "-" << l.var() << " ";
            }
            else
            {
                std::cout << l.var() << " ";
            }
        }
        std::cout << "\n";
    }
};
