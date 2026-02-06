#pragma once
typedef int Var;

struct Lit {
    int x;

    Var  var()  const { return x >> 1; }
    bool sign() const { return (x & 1) != 0; }

    Lit operator~() const { return Lit{ x ^ 1 }; }
};

inline Lit mkLit(Var v, bool sign) {
    return Lit{ (v << 1) | (sign ? 1 : 0) };
}

inline bool operator==(Lit a, Lit b) { return a.x == b.x; }
inline bool operator!=(Lit a, Lit b) { return a.x != b.x; }

