#include "connection.h"
#include <fstream>
#include "gate.h"
#include "type.h"
#include <cmath>

ConnectionList::ConnectionList(char* fileName)
{
    ifstream s(fileName);
    try { for (;;) connections.insert(s); }
    catch (EOFException_connection) {}
}

//findet delay von gateId
double ConnectionList::finddelay(GateList gl, TypeList tl, int gateId) {
    for (auto i = tl.types.begin();i != tl.types.end();i++) {
        if (i->getTypeId() == gl.findGate(gateId)->getTypeId())
            return i->getDelay();
    }
    return -1;
}

//Gate g in Path path addieren
void Path::add_gate(Gate& g) {
    path.push_back(g);
}

void Path::remove_last_gate() {  //letztes Gate von Path path loschen
    path.pop_back();
}

void Path::print() const {
    bool b = false;
    for (auto& i : path) {
        if (b) cout << " -> ";
        cout << i.getGateId();
        b = true;
    }
}

void PathList::add_path(Path& p) {  //Path p in PathList paths addieren
    paths.push_back(p);
}

void PathList::print() {
    for (auto& i : paths) {
        i.print();
        cout << endl;
    }
}

//depth-first-search,um alle Pfade von gateid current nach gateid destination zu finden
// Hier auch die Anmerkung: Der vorgeschlagene Algorithmus im Implementierungstipp wäre viel schneller.  Rade
void ConnectionList::dfs(GateList& gl, TypeList& tl, ConnectionList& cl, int current, int destination, Path& p, PathList& pl) {
    gl.findGate(current)->setVisited(true); // gate mit id current als visited markieren
    p.add_gate(*gl.findGate(current));  // gate mit id current in Path p addieren

    if (current == destination) {
     //   Path path_saved;  //Path p in Variable path_saved speichern, weil originale p wahrend backtracking bei dfs geandert wird
        // path_saved = p würde auch gehen.  Ich glaub aber, man braucht das gar nicht, weil es beim push_back in add_path ohnehin kopiert wird (weil vector<Path> und nicht vector<Path&>).  Rade
    //    for (auto& g : p.path)
    //        path_saved.add_gate(g);
        pl.add_path(p);  // path in PathList pl addieren
    }
    else {
        for (auto& i : connections) {
            //fur jede Connection current->x, prufe ob x nicht besucht ist
            if (i.get_vonGateId() == current && gl.findGate(i.get_nachGateId())->getVisited() == false)
                dfs(gl, tl, cl, i.get_nachGateId(), destination, p, pl);
        }
    }
    gl.findGate(current)->setVisited(false); //gate mit id current als nicht besucht markieren
    p.remove_last_gate(); //backtracking
}

//gibt die Ausgabe
void ConnectionList::gates_and_delay(GateList& gl, TypeList& tl, ConnectionList& cl, int gateid1, int gateid2) {
    Path p;
    PathList pl;
    cl.dfs(gl, tl, cl, gateid1, gateid2, p, pl);

    cout << "Moegliche Pfade von gateid " << gateid1 << " nach gateid " << gateid2 << ":" << endl;
    pl.print();

    //finde minimale summe von zwischen Gates
    double min_sum = INFINITY;
    Path shortest_path;

    for (auto& p : pl.paths) {
        double sum = 0;
        for (auto& g : p.path) {
            sum += cl.finddelay(gl, tl, g.getGateId());
        }
        if (sum < min_sum) {
            min_sum = sum;
            shortest_path = p;  //Kurzeste Pfad innerhalb PathList speichern
        }
    }

    //initialisiere min_delay von gateid2=0;
    shortest_path.path[shortest_path.path.size() - 1].setMindelay(0);

    //berechne mindelay von gefundene kurzeste Pfad   zB a->b mindelay(a)=delay(a)+mindelay(b)
    for (int i = shortest_path.path.size() - 2;i > -1;i--) {
        shortest_path.path[i].setMindelay(shortest_path.path[i + 1].getMindelay() + cl.finddelay(gl, tl, shortest_path.path[i].getGateId()));
    }

    //mindelay ausgeben
    cout << "Kurzeste Gesamtdelay von gateId " << gateid1 << " nach gateId " << gateid2 << " ist " << shortest_path.path[0].getMindelay() << " und kurzeste Pfad ist:" << endl;

    // Reihenfolge von Gates und deren mindelay ausgeben
    cout << "   gateid:" << shortest_path.path[0].getGateId() << ",mindelay:" << shortest_path.path[0].getMindelay() << endl;
    for (unsigned i = 1;i < shortest_path.path.size();i++) {
        cout << "-> gateid:" << shortest_path.path[i].getGateId() << ",mindelay:" << shortest_path.path[i].getMindelay() << endl;
    }
}
