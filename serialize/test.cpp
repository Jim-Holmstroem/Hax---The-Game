#include"test.h"

#include<iostream>
#include<sstream>

namespace hax {

void filetest(ISerializable& refObj, ISerializable& newObj)
{
    std::string filename = refObj.getType();

    std::cout << "---testing[" << filename << "]---" << std::endl;

    std::cout << "Writing to file..." << std::endl;
    std::ofstream outFile(("saved/"+filename+".dat").c_str());
    outFile << refObj;
    outFile.close();

    std::cout << "Reading from file..." << std::endl;
    std::ifstream inFile(("saved/"+filename+".dat").c_str());
    inFile >> newObj;
    inFile.close();

    std::cout << "Comparing reference and new =>" << std::endl;
    std::cout << "refObj:" << refObj << std::endl;
    std::cout << "newObj:" << newObj << std::endl;
   
    std::cout << "---end_test---"<< std::endl << std::endl;
};

}

int main(){
    using namespace hax;

    MiniA miniA;
    MiniA miniA_cp;
    filetest(miniA,miniA_cp);
    
    A a = A(13,37,"leet");
    A a_cp;
    filetest(a,a_cp);
    
    B b;
    B b_cp;
    filetest(b,b_cp);
   
    A* ba = new B();
    filetest(*ba,b_cp);
    delete ba;

    SerializableVector<A> svRef0;
    SerializableVector<A> svNew0;
    filetest(svRef0, svNew0);

    SerializableVector<A*> svPtrRef0;
    SerializableVector<A*> svPtrNew0;
    filetest(svPtrRef0, svPtrNew0);

    SerializableMap<SerializableString,A> smRef0;
    SerializableMap<SerializableString,A> smNew0;
    filetest(smRef0, smNew0);

    SerializableMap<SerializableString,A*> smPtrRef0;
    SerializableMap<SerializableString,A*> smPtrNew0;
    filetest(smPtrRef0, smPtrNew0);

    SerializableVector<A> svRef;
    svRef.push_back(A());
    svRef.push_back(A());
    SerializableVector<A> svNew;
    filetest(svRef, svNew);

    SerializableVector<A*> svPtrRef;
    svPtrRef.push_back(new A());
    svPtrRef.push_back(new A());
    SerializableVector<A*> svPtrNew;
    filetest(svPtrRef, svPtrNew);

    SerializableMap<SerializableString,A> smRef;
    smRef["1st"] = A();
    smRef["2nd"] = A();
    SerializableMap<SerializableString,A> smNew;
    filetest(smRef, smNew);

    SerializableMap<SerializableString,A*> smPtrRef;
    smPtrRef["1st"] = new A();
    smPtrRef.insert(std::pair<SerializableString,A*>("2nd",new A()));
    smPtrRef.insert(std::pair<SerializableString,A*>("1st",new A())); //if insert works then should not be added
    SerializableMap<SerializableString,A*> smPtrNew;
    filetest(smPtrRef, smPtrNew);

    std::stringstream ss (std::stringstream::in|std::stringstream::out);
    void * vp = new B();
    ss << vp;
    std::cout<< "vp=[" << vp << "]" << std::endl;

};

