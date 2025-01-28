// Rei Kacani, 12219813
#include <stdio.h>
#include <stdlib.h>
#include "gate.h"
#include "type.h"
#include "connection.h"
int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        printf("Error, arguments invalid or wrong.\n");
    }
    
    TypeList *tl=newTypeList();
    readTypeList(tl,argv[3]);
    
    GateList *gl=newGateList();
    readGateList(gl,tl,argv[1]);
    
    ConnectionList *cl=newConnectionList();
    readConnectionList(cl,gl,argv[2]);

    gates_and_delay(gl, tl, cl, findGate(gl,atoi(argv[5])),findGate(gl,atoi(argv[6])));
    
    deleteTypeList (tl);
    deleteGateList (gl);
    deleteConnectionList (cl);

    return 0;
}
