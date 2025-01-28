#include <stdio.h>
#include <stdlib.h>
#include "type.h"

TypeList *newTypeList(){
    TypeList *tl=(TypeList *) malloc (sizeof(TypeList));
    tl->count=0;
    tl->allocated=10;
    tl->types=(Type **) calloc (tl->allocated, sizeof(Type *));
    return tl;
}

void deleteTypeList (TypeList *tl){
    int i;
    for(i=0; i<tl->count;i++){
        free(tl->types[i]);
    }
    free(tl->types);
    free(tl);
}

Type *newType (TypeList *tl){
    if(tl->count >= tl->allocated){
        tl->allocated*=2;
        tl->types=(Type **) realloc (tl->types, tl->allocated*sizeof(Type *));
    }
    Type *t=(Type *) malloc (sizeof(Type));
    tl->types[tl->count++]=t;
    return t;
}

void readTypeList (TypeList *tl, char *fileName)
{
  FILE *file = fopen (fileName, "r");
  if (file == NULL)  { perror (fileName);  exit (1); }
  int r, id;
  do
  {
    r = fscanf (file, "%d", &id);
    if (r != EOF)
    {
      Type *t = newType (tl);
      t->typeId = id;
      //r = fscanf (file, "%100s %d %d %lf", t->name, &t->anzahlInputPins, &t->anzahlOutputPins, &t->delay);  // Das muss %19s heißen, wenn t->name ein char[20] ist.  Rade
      r = fscanf (file, "%19s %d %d %lf", t->name, &t->anzahlInputPins, &t->anzahlOutputPins, &t->delay);
    }
  }
  while (r != EOF);
  fclose (file);
}

Type *findType (TypeList *tl, int id)
{
  int i;
  for (i = 0; i < tl->count; i ++)
    if (tl->types[i]->typeId == id)
      return (tl->types[i]);
  fprintf (stderr, "Typee Id %d not found\n", id);
  exit (1);
}

void printType (Type *t)
{
  printf ("typeId: %d, name: %s, anzahlInputPins: %d, anzahlOutputPins: %d, delay: %lf\n", t->typeId, t->name, t->anzahlInputPins, t->anzahlOutputPins, t->delay);
}
