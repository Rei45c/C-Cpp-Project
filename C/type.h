#ifndef TYPE_H
#define TYPE_H

typedef struct
{
    int typeId;
    char name[20]; 
    int anzahlInputPins;
    int anzahlOutputPins;
    double delay;
} Type;

typedef struct{
    Type **types;
    int count;
    int allocated;
} TypeList;

TypeList *newTypeList();
void deleteTypeList (TypeList *tl);
Type *newType (TypeList *tl);
void readTypeList (TypeList *tl, char *fileName);
Type *findType (TypeList *tl, int typeId);
void printType (Type *t);
#endif