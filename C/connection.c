#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "connection.h"
#include "gate.h"
#include "type.h"

ConnectionList* newConnectionList() {
    ConnectionList* cl = (ConnectionList*)malloc(sizeof(ConnectionList));
    cl->count = 0;
    cl->allocated = 10;
    cl->connections = (Connection**)calloc(cl->allocated, sizeof(Connection*));
    return cl;
}

void deleteConnectionList(ConnectionList* cl) {
    int i;
    for (i = 0; i < cl->count;i++) {
        free(cl->connections[i]);
    }
    free(cl->connections);
    free(cl);
}

Connection* newConnection(ConnectionList* cl) {
    if (cl->count >= cl->allocated) {
        cl->allocated *= 2;
        cl->connections = (Connection**)realloc(cl->connections, cl->allocated * sizeof(Connection*));
    }
    Connection* c = (Connection*)malloc(sizeof(Connection));
    cl->connections[cl->count++] = c;
    return c;
}

void readConnectionList(ConnectionList* cl, GateList* gl, char* fileName)
{
    FILE* file = fopen(fileName, "r");
    if (file == NULL) { perror(fileName);  exit(1); }
    int r, fromid;
    do
    {
        r = fscanf(file, "%d", &fromid);
        if (r != EOF)
        {
            Connection* c = newConnection(cl);
            c->vonGateId = fromid;
            r = fscanf(file, "%d %d %d", &c->vonPin, &c->nachGateId, &c->nachPin);
            c->vonGate = findGate(gl, c->vonGateId);
            c->nachGate = findGate(gl, c->nachGateId);
        }
    } while (r != EOF);
    fclose(file);
}

Connection* findConnection(ConnectionList* cl, int fromgateId, int togateId) {
    int i;
    for (i = 0; i < cl->count; i++)
        if (cl->connections[i]->vonGateId == fromgateId && cl->connections[i]->nachGateId == togateId)
            return (cl->connections[i]);
    fprintf(stderr, "Connection Ids %d, %d not found\n", fromgateId, togateId);
    exit(1);
}

void printConnection(Connection* c)
{
    printf("vonGateId: %d, vonPin: %d, nachGateId: %d, nachPin: %d \n", c->vonGateId, c->vonPin, c->nachGateId, c->nachPin);
}

double finddelay(Gate* g) {   //findet delay von g
    //int i;
    // Wenn man statt gateId ein Gate* übergeben würde, und Gate einen Pointer zum Type hätte, müsste man hier überhaupt nicht suchen.  Rade
    // for(i=0;i<tl->count;i++){
    //     if(findGate(gl,gateId)->typeId == tl->types[i]->typeId)
    //       return tl->types[i]->delay;
    // }
    // return -1;
    return g->type->delay;
}

//depth-first-search,um alle Pfade von gateid current nach gateid destination zu finden
void dfs(GateList* gl, TypeList* tl, ConnectionList* cl, Gate* currentGate, Gate* destinationGate, Path* p, PathList* pl) {
    //findGate(gl,current)->visited=1;   // gate mit id current als visited markieren
    //add_gate(p,findGate(gl,current));  // gate mit id current in Path p addieren
    currentGate->visited = 1;
    add_gate(p, currentGate);

    if (currentGate == destinationGate) {
        Path* path_saved = newPath();   //Path p in Variable path_saved speichern, weil originale p wahrend backtracking bei dfs geandert wird
        for (int i = 0;i < p->count;i++) {
            add_gate(path_saved, p->path[i]);
        }
        add_path(pl, path_saved);   // path in PathList pl addieren
    }
    else {
        for (int i = 0;i < cl->count;i++) {
            //fur jede Connection current->x, prufe ob x nicht besucht ist
            // if(cl->connections[i]->vonGateId==currentGate->gateId && findGate(gl,cl->connections[i]->nachGateId)->visited==0){  
            //   // Wenn die Connections Pointer zu den Gates hätten, könnte man hier statt nachGateId einen Gate-Pointer übergeben und man müsste nicht immer nach den Gates suchen.  Rade
            //   dfs(gl,tl,cl,cl->connections[i]->nachGateId,destination,p,pl);
            // }
            if (cl->connections[i]->vonGate == currentGate && cl->connections[i]->nachGate->visited == 0) {
                // Wenn die Connections Pointer zu den Gates hätten, könnte man hier statt nachGateId einen Gate-Pointer übergeben und man müsste nicht immer nach den Gates suchen.  Rade
                dfs(gl, tl, cl, cl->connections[i]->nachGate, destinationGate, p, pl);
            }

        }
    }

    p->count--;   //backtracking
    currentGate->visited = 0;  //current gate als nicht besucht markieren
}

// gibt die Ausgabe
// Der vorgeschlagene Algorithmus wär halt um einiges schneller, als alle Pfade zu generieren und dann nach Länge zu durchsuchen.  Rade
void gates_and_delay(GateList* gl, TypeList* tl, ConnectionList* cl, Gate* gate1, Gate* gate2) {
    Path* p = newPath();
    PathList* pl = newPathList();

    dfs(gl, tl, cl, gate1, gate2, p, pl);
    if (pl->count == 0) {
        printf("Es gibt keinen Pfad von gateId %d zu gateId %d", gate1->gateId, gate2->gateId);
        return;
    }
    printf("Moegliche Pfade von gateid %d nach gateid %d:\n", gate1->gateId, gate2->gateId);
    printPathList(pl);

    //initialisiere min_delay von gateid2=0;
    gate2->mindelay = 0;

    //finde minimale summe von zwischen Gates
    double min_sum = INFINITY;
    int shortest_path_pos = -1;

    for (int i = 0;i < pl->count;i++) {
        double sum = 0;
        for (int j = 1;j < pl->paths[i]->count - 1;j++) {
            sum += finddelay(pl->paths[i]->path[j]);
        }
        if (sum < min_sum) {
            min_sum = sum;
            shortest_path_pos = i;  //Position von kurzeste Pfad in PathList speichern, 
        }
    }

    //berechne mindelay von gefundene kurzeste Pfad, der shortest_path_pos in PathList hat    zB a->b mindelay(a)=delay(a)+mindelay(b)
    for (int i = pl->paths[shortest_path_pos]->count - 2;i > -1;i--) {   //gehe von gateid2 zu gateid1
        pl->paths[shortest_path_pos]->path[i]->mindelay = pl->paths[shortest_path_pos]->path[i + 1]->mindelay + finddelay(pl->paths[shortest_path_pos]->path[i]);
    }

    //mindelay ausgeben
    printf("Kurzeste Gesamtdelay von gateId %d nach gateId %d ist %lf und kurzeste Pfad ist: \n\n", gate1->gateId, gate2->gateId, pl->paths[shortest_path_pos]->path[0]->mindelay);

    // Reihenfolge von Gates und deren mindelay ausgeben
    printf("    gateId:%d,mindelay:%lf \n", pl->paths[shortest_path_pos]->path[0]->gateId, pl->paths[shortest_path_pos]->path[0]->mindelay);
    for (int i = 1;i < pl->paths[shortest_path_pos]->count;i++) {
        printf(" -> gateId:%d,mindelay:%lf\n", pl->paths[shortest_path_pos]->path[i]->gateId, pl->paths[shortest_path_pos]->path[i]->mindelay);
    }
    putchar('\n');

    deletePath(p);
    deletePathList(pl);
}

Path* newPath() {
    Path* p = (Path*)malloc(sizeof(Path));
    p->count = 0;
    p->allocated = 10;
    p->path = (Gate**)calloc(p->allocated, sizeof(Gate*));
    return p;
}

void deletePath(Path* p) {
    int i;
    for (i = 0; i < p->count;i++) {
        //free(p->path[i]);  // Die Gates werden in deleteGateList freigegeben.  Rade
    }
    free(p->path);
    free(p);
}

void add_gate(Path* p, Gate* g) {   //Gate g in Path p addieren
    if (p->count >= p->allocated) {
        p->allocated *= 2;
        p->path = (Gate**)realloc(p->path, p->allocated * sizeof(Gate*));
    }
    p->path[p->count++] = g;
}

void printPath(Path* p) {
    for (int i = 0;i < p->count;i++) {
        printf("%d, ", p->path[i]->gateId);
    }
    putchar('\n');
}

PathList* newPathList() {
    PathList* pl = (PathList*)malloc(sizeof(PathList));
    pl->count = 0;
    pl->allocated = 10;
    pl->paths = (Path**)calloc(pl->allocated, sizeof(Path*));
    return pl;
}

void deletePathList(PathList* pl) {
    int i;
    for (i = 0; i < pl->count;i++) {
        deletePath(pl->paths[i]);
    }
    free(pl->paths);
    free(pl);
}

void add_path(PathList* pl, Path* p) {  //Path p in PathList pl addieren
    if (pl->count >= pl->allocated) {
        pl->allocated *= 2;
        pl->paths = (Path**)realloc(pl->paths, pl->allocated * sizeof(Path*));
    }
    pl->paths[pl->count++] = p;
}

void printPathList(PathList* pl) {
    for (int i = 0;i < pl->count;i++) {
        printf("%d", pl->paths[i]->path[0]->gateId);
        for (int j = 1;j < pl->paths[i]->count;j++) {
            printf(" -> %d", pl->paths[i]->path[j]->gateId);
        }
        putchar('\n');
    }
}
