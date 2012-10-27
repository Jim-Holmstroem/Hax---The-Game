#ifndef TEST_H
#define TEST_H

#include<string>
#include<vector>
#include<stdexcept>
#include<iterator>
#include<iostream>
#include<fstream>

#include"parsehelper.h"

#include"iserializable.h"
#include"serializablemap.h"
#include"serializablevector.h"

#include"serializablestring.h"
#include"heap.h"
/*
*  NOTE: only testingcode and experimantalcode
*
*/

/*
class Heap
{
  public:
    void load(stream){};
    void save(stream) const{};

    (void *) const operator[](const std::size_t UID) const
    {
               
    };
};

template<class T> class LoadFactory<T> 
{
  protected:
    Heap heap;
    void loadLevel(){
    
    };
    void loadTheHeap(){
    
    };
    void connectPointers(){
    
    };
  public:
    T* GetLevel(string filename)
    {
        
    };

};
*/
//gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html#Variadic-Macros
namespace hax {

void filetest(ISerializable& testobj, ISerializable& testobj_cp, std::string filename);

class MiniA : public ISerializable
{
    int mini1;
    int mini2;

  public:
    MiniA():mini1(13),mini2(37){};
    MiniA(int m1,int m2):mini1(m1),mini2(m2){};
    virtual std::string getType() const
    {
        return "MiniA";
    };
  protected:
    inline virtual void ToString(std::ostream& out) const
    {
        out << mini1 << ' ' << mini2 << ' '; //TODO make a function to wich you only register wich ones that should be serialized
    };
    inline virtual void FromString(std::istream& in)
    {
	    in >> mini1 >> mini2; 
    };
};


class A : public ISerializable
{
    int var1;
    int var2;
    std::string var3;
    MiniA minia;
    MiniA minib;

  public:
    A():var1(1337),var2(1449),var3("bajs"),minia(12,34){};
    A(int v1,int v2,std::string v3):var1(v1),var2(v2),var3(v3),minia(12,34){};
    virtual std::string getType() const
    {
        return "A";
    };    
  protected:
    inline virtual void ToString(std::ostream& out) const
    {
        out << var1 << ' ' << var2 << ' ' << var3 << ' ' << minia << ' ' << minib << ' ';
    };
    inline virtual void FromString(std::istream& in)
    {
        in >> var1 >> var2 >> var3 >> minia >> minib;
    };
    
};


class B : public A
{
    int varB1;
    std::string varB2;

  public:
    B():A(),varB1(9234),varB2("varB2"){};
    B(int v1,int v2,std::string v3,int vB1,std::string vB2):A(v1,v2,v3),varB1(vB1),varB2(vB2){};

    virtual std::string getType() const
    {
        return "B";
    };    

  protected:
    inline virtual void ToString(std::ostream& out) const
    {
        A::ToString(out);
        out << "| "; 
        out << varB1 << ' ' << varB2 << ' ';
    };
    inline virtual void FromString(std::istream& in)
    {
        A::FromString(in);
        checkToken(in,"|"); 
        in >> varB1 >> varB2;
    };
    
};
}

#endif
