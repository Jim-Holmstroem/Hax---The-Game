#include"test.h"

#include<sstream>

namespace hax {


void filetest(ISerializable& testobj,ISerializable& testobj_cp, std::string filename)
{
    std::cout << "---testing[" << filename << "]---" << std::endl;

    std::cout << "in: " << testobj << std::endl;

    std::ofstream outFile(("saved/"+filename+".dat").c_str());
    outFile << testobj;
    outFile.close();
    
    std::ifstream inFile(("saved/"+filename+".dat").c_str());
    inFile >> testobj_cp;
    inFile.close();

    std::cout << "out:" << testobj << std::endl;
   
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
    
    SerializableVector<MiniA> sv;
    sv.push_back(MiniA());
    sv.push_back(MiniA());
    sv.push_back(MiniA());
    sv.push_back(MiniA());
    sv.push_back(MiniA());
    sv.push_back(MiniA());
    SerializableVector<MiniA> sv_cp;
    filetest(sv,sv_cp,"SerializableVector");

    SerializableMap<SerializableString,MiniA> sm;
    sm["1st"] = MiniA();
    sm["2nd"] = MiniA();
    sm["3rd"] = MiniA();
    sm["4th"] = MiniA();
    sm["5th"] = MiniA();
    SerializableMap<SerializableString,MiniA> sm_cp;
    filetest(sm,sm_cp,"SerializableVector");


    SerializableVector<MiniA> sv0;
    SerializableVector<MiniA> sv_cp0;
    filetest(sv0,sv_cp0,"SerializableVector0");

    SerializableMap<std::string,MiniA> sm0;
    SerializableMap<std::string,MiniA> sm_cp0;
    filetest(sm0,sm_cp0,"SerializableMap0");


    std::stringstream ss (std::stringstream::in|std::stringstream::out);

    void * vp = new B();
    ss << vp;
    std::cout<< "vp=[" << vp << "]" << std::endl;

};

