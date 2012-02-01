#include "dataparser.h"

void hax::Node::CheckForNotFound(std::size_t find,std::size_t pos)
{
    if(find==std::string::npos)
    {
        std::cerr << "Syntax error near '" << pos  << "'" << std::endl;
        return;    
    }
}

hax::Primitive::Primitive(std::string data)
{
    this->value = data;
}


hax::Node::Node(std::string data)
{
    std::size_t pos = 0;

    std::size_t start = 0;
    std::size_t goal = 0;
    std::size_t counter = 0; //

    std::string name;
    std::string sub_data;

    pos = data.find_first_of('{',pos);//starts on object in json


    while(pos!=std::string::npos)
    {
        //TODO CHECK IF IT DOESN*T START WITH { ITS AN PRIMITIVE

        //READ THE NAME
        pos = start = data.find_first_of('"',pos+1);    
        pos = goal = data.find_first_of('"',pos+1);
        name = data.substr(start+1,goal-start-1);

        pos = data.find_first_of(':',pos+1);

        //look ahead to see if it's a primitve (then go back)
        start = data.find_first_of("{\"",pos+1); //it must be either or else syntax error
        pos = start - 1;//

       if(data[start]=='{')
        {
            //READ THE INNER DATA, counting the { to seen when you find the matching one
            do{ //TODO handle error if missing }
                pos = data.find_first_of("{}",pos+1);
                if(data[pos]=='{')
                {
                    counter++;
                }
                else if(data[pos]=='}')
                {
                    counter--;
                }
            }while(counter>0);
            goal = pos;
            
            sub_data = data.substr(start,goal-start+1); 
            this->nodes[name] = Node(sub_data); //recursive call
            //TODO Node destructed after outfscope?
        }
        else //primitive
        {
            pos = start = data.find_first_of('"',pos+1);
            pos = goal = data.find_first_of('"',pos+1);
            
            sub_data = data.substr(start,goal-start+1);
            //base case
            this->nodes[name] = Primitive(sub_data);        
            //TODO Node destructed after outfscope?
        }
        
        std::cout << name << std::endl << sub_data << std::endl << std::endl;

        pos = data.find_first_of("},",pos+1);//the last element at this level or those it have more?
        //TODO make common function for this (next this or that is what of them?)
        if(data[pos]=='}')//the last on in this level
        {
            pos = std::string::npos;//indicate that you are done at this level
        }
        //else nothing since the pos is already set 
        //save name,sub_data in nodes
    
    }
}
/*
std::string hax::Node::GetNode(const std::string id) const
{
    return nodes[id]; //TODO return Node ?
}
std::string hax::Primitive::GetNode(const std::string id) const
{
    //id doesn't matter
    return this->value; //TODO return string?, see return Node ?
}*/


/* For testing only:) */
int main(int argc, const char* a)
{
    hax::Node d = hax::Node(hax::Read("test1.json")); 

    std::cout << (std::string)d["object1"]["param1"]("prim_string") << std::endl; 

    return 0;
}

