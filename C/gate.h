#ifndef GATE_H
#define GATE_H
#include "type.h"

typedef struct
{
    int gateId;
    int typeId;
    double mindelay;  // minimale delay von gateid2
    int visited;      // falls Gate besucht ist, ist die Variable visited 1, sonst 0
    Type* type;
} Gate;

typedef struct {
    Gate** gates;
    int count;
    int allocated;
} GateList;

GateList* newGateList();
void deleteGateList(GateList* gl);
Gate* newGate(GateList* gl);
void readGateList(GateList* gl, TypeList* tl, char* fileName);
Gate* findGate(GateList* gl, int gateId);
void printGate(Gate* g);
#endif 