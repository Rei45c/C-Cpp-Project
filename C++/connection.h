#ifndef CONNECTION_H
#define CONNECTION_H

#include <vector>
#include "gate.h"
#include "type.h"

using namespace std;

class EOFException_connection {};

class Connection
{
public:
    Connection(istream& s)
    {
        s >> vonGateId >> vonPin >> nachGateId >> nachPin;
        if (!s)  throw EOFException_connection();  // abort constructor!
    }

    Connection(int vonId, int nachId) { vonGateId = vonId; nachGateId = nachId; }  // for comparisons

    void print(ostream& o = cout) const
    {
        o << "vonGateId: " << vonGateId << ", vonPin: " << vonPin << ", nachGateId:" << nachGateId << ", nachPin: " << nachPin << endl;
    }

    bool operator< (const Connection& c) const {
        if (vonGateId < c.vonGateId)
            return true;
        else if (vonGateId > c.vonGateId)
            return false;
        else  // falls gleich
            return nachGateId < c.nachGateId;
    }

    int get_vonGateId() const {
        return vonGateId;
    }

    int get_nachGateId() const {
        return nachGateId;
    }

private:
    int vonGateId;
    int vonPin;
    int nachGateId;
    int nachPin;
};

class Path {
public:
    Path() {}
    void add_gate(Gate& g);
    void remove_last_gate();
    void print() const;
    //private:
    vector<Gate> path;  // Pfad mit Gates
};

class PathList {
public:
    PathList() {}
    void add_path(Path& p);
    void print();
    //private:
    vector<Path> paths;  //speichert Pfade
};

class ConnectionList {
public:
    ConnectionList(char* FileName);
    Connection const* findConnection(unsigned fromid, unsigned toid) const
    {
        Connection c(fromid, toid); return &*connections.find(c);
    }
    double finddelay(GateList gl, TypeList tl, int gateId);
    void dfs(GateList& gl, TypeList& tl, ConnectionList& cl, int current, int destination, Path& p, PathList& pl);
    void gates_and_delay(GateList& gl, TypeList& tl, ConnectionList& cl, int gateid1, int gateid2);
private:
    set<Connection> connections;
};
#endif

