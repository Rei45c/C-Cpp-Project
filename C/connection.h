#ifndef CONNECTION_H
#define CONNECTION_H
#include "gate.h"
#include "type.h"

typedef struct
{
	int vonGateId;
	int vonPin;
	int nachGateId;
	int nachPin;
	Gate* vonGate;
	Gate* nachGate;
} Connection;

typedef struct
{
	Connection** connections;
	int count;
	int allocated;
} ConnectionList;

typedef struct
{
	Gate** path;
	int count;
	int allocated;
} Path;   //Pfad mit Gates

typedef struct
{
	Path** paths;
	int count;
	int allocated;
} PathList;  //Liste, die Pfade enthalt

ConnectionList* newConnectionList();
void deleteConnectionList(ConnectionList* cl);
Connection* newConnection(ConnectionList* cl);
void readConnectionList(ConnectionList* cl, GateList* gl, char* fileName);
Connection* findConnection(ConnectionList* cl, int fromgateId, int togateId);
void printConnection(Connection* c);

double finddelay(Gate* g);
void dfs(GateList* gl, TypeList* tl, ConnectionList* cl, Gate* currentGate, Gate* destinationGate, Path* p, PathList* pl);
void gates_and_delay(GateList* gl, TypeList* tl, ConnectionList* cl, Gate* gate1, Gate* gate2);

Path* newPath();
void deletePath(Path* p);
void add_gate(Path* p, Gate* g);
void printPath(Path* p);

PathList* newPathList();
void deletePathList(PathList* pl);
void add_path(PathList* pl, Path* p);
void printPathList(PathList* pl);
#endif