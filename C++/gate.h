#ifndef GATE_H
#define GATE_H

#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

class EOFException_gate {};

class Gate
{
public:
    Gate(istream& s)
    {
        s >> gateId >> typeId;
        visited = false;
        if (!s)  throw EOFException_gate();  // abort constructor!
    }

    Gate(int gId) { gateId = gId; visited = false; }  // for comparisons

    void print(ostream& o = cout) const
    {
        o << "gateId: " << gateId << ", typeId: " << typeId;
    }

    bool operator< (const Gate& g) const { return gateId < g.gateId; }

    int getGateId() const {
        return gateId;
    }

    int getTypeId() const {
        return typeId;
    }

    bool getVisited() const {
        return visited;
    }

    void setVisited(bool v) {
        visited = v;
    }

    double getMindelay() {
        return mindelay;
    }

    void setMindelay(double v) {
        mindelay = v;
    }

private:
    int gateId;
    int typeId;
    double mindelay; // minimale delay von gateid2
    bool visited;    // falls Gate besucht ist, ist die Variable visited true, sonst false
};

class GateList
{
public:
    GateList(char* FileName);
    Gate* findGate(unsigned id) {
        Gate g(id);
        auto it = gates.find(g);
        return (it != gates.end()) ? &const_cast<Gate&>(*it) : nullptr;
    }
private:
    set<Gate> gates;
};
#endif

