#ifndef TYPE_H
#define TYPE_H

#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

class EOFException_type {};

class Type
{
public:
    Type(istream& s)
    {
        s >> typeId >> name >> anzahlInputPins >> anzahlOutputPins >> delay;
        if (!s)  throw EOFException_type();  // abort constructor!
    }

    Type(int tId) { typeId = tId; }  // for comparisons

    void print(ostream& o = cout) const
    {
        o << "typeId:" << typeId << ",name:" << name << ",anzahlInputPins:" << anzahlInputPins << ",anzahlOutputPins:" << anzahlOutputPins << ",name:" << name << endl;
    }

    bool operator< (const Type& t) const { return typeId < t.typeId; }

    int getTypeId() const {
        return typeId;
    }

    double getDelay() const {
        return delay;
    }

private:
    int typeId;
    string name;
    int anzahlInputPins;
    int anzahlOutputPins;
    double delay;
};

class TypeList
{
public:
    TypeList(char* FileName);
    Type const* findType(unsigned id) const
    {
        Type t(id); return &*types.find(t);
    }

    set<Type> getTypes() const {
        return types;
    }
    //private:
    set<Type> types;
};
#endif

