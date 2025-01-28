#include <stdio.h>
#include <stdlib.h>
#include "gate.h"

GateList* newGateList() {
    GateList* gl = (GateList*)malloc(sizeof(GateList));
    gl->count = 0;
    gl->allocated = 10;
    gl->gates = (Gate**)calloc(gl->allocated, sizeof(Gate*));
    return gl;
}

void deleteGateList(GateList* gl) {
    int i;
    for (i = 0; i < gl->count;i++) {
        free(gl->gates[i]);
    }
    free(gl->gates);
    free(gl);
}

Gate* newGate(GateList* gl) {
    if (gl->count >= gl->allocated) {
        gl->allocated *= 2;
        gl->gates = (Gate**)realloc(gl->gates, gl->allocated * sizeof(Gate*));
    }
    Gate* g = (Gate*)malloc(sizeof(Gate));
    g->visited = 0;  // Nötig, weil das nicht mit 0 initialisiert wird.  Rade
    gl->gates[gl->count++] = g;
    return g;
}

void readGateList(GateList* gl, TypeList* tl, char* fileName)
{
    FILE* file = fopen(fileName, "r");
    if (file == NULL) { perror(fileName);  exit(1); }
    int r, id;
    do
    {
        r = fscanf(file, "%d", &id);
        if (r != EOF)
        {
            Gate* g = newGate(gl);
            g->gateId = id;
            r = fscanf(file, "%d", &g->typeId);
            g->type = findType(tl, g->typeId);
        }
    } while (r != EOF);
    fclose(file);
}

Gate* findGate(GateList* gl, int id)
{
    int i;
    for (i = 0; i < gl->count; i++)
        if (gl->gates[i]->gateId == id)
            return (gl->gates[i]);
    fprintf(stderr, "Gate Id %d not found\n", id);
    exit(1);
}

void printGate(Gate* g)
{
    printf("gateId: %d, typeId: %d, mindelay: %lf \n", g->gateId, g->typeId, g->mindelay);
}
