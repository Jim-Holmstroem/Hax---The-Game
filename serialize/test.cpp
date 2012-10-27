#include"test.h"

#include<iostream>
#include<sstream>

namespace hax {


void filetest(ISerializable& testobj, ISerializable& testobj_cp, std::string filename)
{
    std::cout << "---testing[" << filename << "]---" << std::endl;

    std::cout << "in: " << testobj << std::endl;

    std::ofstream outFile(("saved/"+filename+".dat").c_str());
    outFile << testobj;
    outFile.close();
    
    std::ifstream inFile(("saved/"+filename+".dat").c_str());
    inFile >> testobj_cp;
    inFile.close();

    std::cout << "out:" << testobj_cp << std::endl;
   
    std::cout << "---end_test---"<< std::endl;

}

}

int main(){
    using namespace hax;

    MiniA miniA;
    MiniA miniA_cp;
    filetest(miniA,miniA_cp,"miniA");
    
    A a = A(13,37,"leet");
    A a_cp;
    filetest(a,a_cp,"A");
    
    B b;
    B b_cp;
    filetest(b,b_cp,"B");
   
    A* ba = new B();
    filetest(*ba,b_cp,"ba");
    delete ba;

    SerializableVector<ISerializable*> sv0;
    SerializableVector<ISerializable*> sv_cp0;
    filetest(sv0,sv_cp0,"SerializableVector_empty");

    SerializableMap<std::string,ISerializable*> sm0;
    SerializableMap<std::string,ISerializable*> sm_cp0;
    filetest(sm0,sm_cp0,"SerializableMap_empty");
    
    SerializableVector<MiniA*> sv;
    sv.push_back(new MiniA());
    sv.push_back(new MiniA());
    sv.push_back(new MiniA());
    sv.push_back(new MiniA());
    sv.push_back(new MiniA());
    sv.push_back(new MiniA());
    SerializableVector<MiniA*> sv_cp;
    filetest(sv,sv_cp,"SerializableVector");

    SerializableMap<SerializableString,MiniA*> sm;
    sm["1st"] = new MiniA();
    sm["2nd"] = new MiniA();
    sm["3rd"] = new MiniA();
    sm["4th"] = new MiniA();
    sm["5th"] = new MiniA();
    SerializableMap<SerializableString,MiniA*> sm_cp;
    filetest(sm,sm_cp,"SerializableMap");


    std::stringstream ss (std::stringstream::in|std::stringstream::out);

    void * vp = new B();
    ss << vp;
    std::cout<< "vp=[" << vp << "]" << std::endl;

};

