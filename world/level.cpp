#include<cstdio>
#include<cstdlib>
#include<fstream>
#include "level.h"
#include "../area/area.h"
#include "../ch/character.h"
#include "../obj/container.h"
#include "../serialize/simpleHeap.h"
/*#include<algorithm>
  #include<functional>*/

hax::Level::Level()
{
    curChar = NULL;

    randomActions.push_back(&Character::go_random);
    randomActions.push_back(&Character::fight_random);
    randomActions.push_back(&Character::rest);

    /*Global commands*/
    opmap["save"] = new LevelOpBool1String(&Level::save);
    opmap["load"] = new LevelOpBool1String(&Level::load);
    /*Level specific commands*/
    opmap["focus"] = new LevelOpBool1String(&Level::focus);
    opmap["kill"] = new LevelOpBool1String(&Level::kill);
    opmap["help"] = new LevelOpVoid(&Level::help);
    opmap["info"] = new LevelOpVoid(&Level::info);
    opmap["spawn"] = new LevelOpVoid(&Level::spawn);
    opmap["action"] = new LevelOpVoid(&Level::action);
    opmap["a"] = new LevelOpVoid(&Level::action); //TODO remove later, only for easy testing
    /*Character specific commands*/
    opmap["go"] = new CharOpBool1String(&Character::go);
    opmap["fight"] = new CharOpBool1String(&Character::fight);
    opmap["rest"] = new CharOpBoolVoid(&Character::rest);
    opmap["pu"] = new CharOpBool1String(&Character::pick_up);
    opmap["drop"] = new CharOpBool1String(&Character::drop);
    opmap["buy"] = new CharOpBool1String(&Character::buy);
    opmap["sell"] = new CharOpBool1String(&Character::sell);
    opmap["stats"] = new CharOpVoidConst(&Character::view_stats);
    opmap["view"] = new CharOpVoidConst(&Character::view_area);
    opmap["inv"] = new CharOpVoidConst(&Character::view_inventory);
    opmap["ls"] = new CharOpVoidConst(&Character::view_curContainer);
    opmap["cd"] = new CharOpBool1String(&Character::change_curContainer);
    opmap["mv"] = new CharOpBool3String(&Character::move_obj);

    //the keys will be printed in alphabetical order when iterated
    levelHelp["focus"] = "Select a character to perform actions: focus <character>";
    levelHelp["kill"] = "Kill a character: kill <character>";
    levelHelp["help"] = "Show a list of available commands";
    levelHelp["info"] = "Show current status of this level";
    levelHelp["spawn"] = "Spawn a random character";
    levelHelp["action"] ="Each character performs a random action";
    charHelp["go"] = "Go to another area: go <direction>";
    charHelp["fight"] = "Fight another character to the death: fight <character>";
    charHelp["rest"] = "Rest to regain HP, MP and stamina";
    charHelp["pu"] = "Pick up an object: pick up <object>";
    charHelp["drop"] = "Drop an object: drop <object>";
    charHelp["buy"] = "Buy object from a shop: buy <object>";
    charHelp["sell"] = "Sell object to a shop: sell <object>";
    charHelp["stats"] = "Show the stats of the character";
    charHelp["view"] = "Show the characters: objects and routes in current area";
    charHelp["inv"] = "Show objects in all containers";
    charHelp["ls"] = "Show objects in current container";
    charHelp["cd"] = "Change curent container: cd <container>, cd # to return home";
    charHelp["mv"] = "Move object from somewhere to somewhere: mv <object><source><destination>";
}
hax::Level::Level(int n)
{
    Area* alist[] = {}; //fill with indoor and outdoor objects
    for(int i=0; i<n; i++){
        vec_area.push_back(alist[i]);
    }
}
/*hax::Level::Level(const Level& le){
  }*/
hax::Level::~Level()
{
    std::cout<<"~Level: deleting Level..."<<std::endl;
//    std::for_each(vec_area.begin(), vec_area.end(), std::mem_fun(&Level::delete));
    deleteAreas();
    std::map<std::string, Operation*>::iterator it = opmap.begin();
    while(it != opmap.end()){
        delete it->second;
        it++;
    }
    std::cout<<"Level deleted"<<std::endl;
}

void hax::Level::add(Area* ar)
{
    vec_area.push_back(ar);
}
void hax::Level::add(Character* ch)
{
    std::pair< std::map<std::string,Character*>::iterator, bool > ret;
    ret = players.insert( std::pair<std::string,Character*>(ch->getName(),ch) ); //will automatically check for key collisions
    if(ret.second == false){
        std::cout << "Character with name " << ret.first->second->getName() << " already exists. Adding appendix to name." << std::endl;
        int appendix = 0;
        while(ret.second == false){ //change appendix until find unique name
            std::ostringstream oss;
            oss << ++appendix;
            ret = players.insert( std::pair<std::string,Character*>(ch->getName()+oss.str(),ch) );
        }
    }
}
void hax::Level::parse(const std::vector<std::string> words)
{
    Character* myChar;
    std::string op = "";
    size_t pos = 0;
    bool execute = false;
    if(words.size()>0 && opmap.find(words[0]) != opmap.end()){ //any single word command, ex. "info"
        myChar = curChar;
        op = words[0];
        pos = 1;
        execute = true;
    }else if(words.size()>1 && opmap.find(words[0]+" "+words[1]) != opmap.end() ){ //any double word command, ex. "pick up"
        myChar = curChar;
        op = words[0]+" "+words[1];
        pos = 2;
        execute = true;
    }else if(words.size()>1 && players.find(words[0]) != players.end() && opmap.find(words[1]) != opmap.end()){ //any single word character command
        myChar = players[words[0]];
        op = words[1];
        pos = 2;
        if(charHelp.find(op) != charHelp.end()){
            execute = true;
        }else{
            execute = false;
            std::cout << "Not a character command" << std::endl;
        }
    }else if(words.size()>2 && players.find(words[0]) != players.end() && opmap.find(words[1]+" "+words[2]) != opmap.end()){ //any double word character command
        myChar = players[words[0]];
        op = words[1]+" "+words[2];
        pos = 3;
        if(charHelp.find(op) != charHelp.end()){
            execute = true;
        }else{
            execute = false;
            std::cout << "Not a character command" << std::endl;
        }
    }else{
        execute = false;
        std::cout << "Unknown command" << std::endl;
    }

    if(execute){
        if(myChar != NULL){
            std::cout << "Character: " << myChar->getName() << std::endl;
        }
        std::cout << "Command: " << op << std::endl;
//      std::cout << "pos: " << pos << " words.size(): " << words.size() << std::endl;
        std::string arg = "";
        if(pos < words.size()){ //concatenate rest of input
            arg.append(words[pos]);
            pos++;
            for(; pos < words.size(); pos++){
                arg.append(" "+words[pos]);
            }
            std::cout << "Argument: " << arg << std::endl;
        }
        std::cout << std::endl;

        opmap[op]->call(arg, myChar, this); //TODO should use dynamic_cast to check if CharOp or LevelOp
        updatePlayers(); //if a Character died then remove from std::map players otherwise info() will give segfault

    }
}
void hax::Level::ToString(std::ostream& out) const
{
    out << this <<":"<< getType() <<":";
    for(size_t i=0; i<vec_area.size(); i++)
    {
        out << vec_area[i] << ":";
        serializeQueue.push(vec_area[i]); //we can do this because we know that the Areas are only owned by Level, TODO use C++11 feature std::unique_ptr
    }
    out << "end:";
    out << curChar <<":"<< curCharName;
}
void hax::Level::FromString(std::istream& in)
{
    std::ofstream dbg;
    dbg.open("load_debug.dat", std::ios::out | std::ios::app);
    dbg << "Level::FromString" << std::endl;

    std::string data;
    std::getline(in,data,':'); //read type
    if(data != getType())
    {
        dbg << "Type mismatch! Aborting load from file." << std::endl;
        dbg.close();
        return;
    }
    std::getline(in,data,':');
    //first part is data for vec_area, we know this from Level::ToString
    while(data!="end")
    {
        vec_area.push_back(dynamic_cast<Area*>(pointerTable[data]));
        dbg << "Area UID = " << data << " | Area new address = " << vec_area.back() << std::endl;
        std::getline(in,data,':');
    }

    std::getline(in,data,':');
    if(data == "0"){curChar = NULL;}
    else{curChar = dynamic_cast<Character*>(pointerTable[data]);}
    dbg << "curChar = " << curChar << std::endl;

    std::getline(in,data); //last column
    curCharName = data; //TODO safe to save even if curChar = NULL??
    dbg << "curCharName = " << curCharName << std::endl;
    dbg.close();
}
std::string hax::Level::getType() const{return "level";}
void hax::Level::updatePlayers()
{
    players.clear();
    //re-add all controllable characters
    for(size_t i=0; i<vec_area.size(); i++)
    {
        const SerializableVector<Character*> charsInArea = vec_area[i]->chars();
        for(size_t k=0; k<charsInArea.size(); k++)
        {
            Character* ch = charsInArea[k]; //this Character could have died
            if(ch->isControllable()){add(ch);}
/*
            if(ch->getcurHp() <= 0){
                players.erase(ch->getName()); //does not matter if name is not in players, erase just returns NULL
                delete ch; //character was killed
            }
*/
        }
    }

    if(players.find(curCharName) == players.end()){ //curChar is dead, so reset curChar
        curChar = NULL;
    }
}
void hax::Level::action()
{
    std::map<std::string, Character*> playerHasPerformedAction;
    bool foundPlayerWaitingToAct = false;
    std::map<std::string, Character*>::iterator it = players.begin();
    while(true)
    {
        it = players.begin();
        while(!foundPlayerWaitingToAct && it!=players.end())
        {
            Character* ch = it->second;
            if(playerHasPerformedAction.count(ch->getName()) == 0)
            {
                foundPlayerWaitingToAct = true;
                playerHasPerformedAction.insert( std::pair<std::string,Character*>(ch->getName(),ch) );
                std::cout << ch->getName() << " is performing action..." << std::endl;
                unsigned int actionNr = rand() % randomActions.size();
                //if action not possible then try again until an action is performed
                while( !(ch->*randomActions[actionNr])() )
                {
                    actionNr = rand() % randomActions.size();
                }
            }
            it++;
        }
        foundPlayerWaitingToAct = false;
        if(it == players.end()){break;}
        updatePlayers();
    }

/*
    std::queue<Character*> actionQ;
    std::map<std::string, Character*>::iterator it = players.begin();
    while(it != players.end())
    {
        actionQ.push(it->second);
        it++;
    }
    std::cout << "Action queue filled with " << actionQ.size() << " controllable characters." << std::endl;
    //queue contains all controllable characters, now let them perform random action
    while(!actionQ.empty())
    {
        std::weak_ptr<Character> ch = actionQ.front();
        actionQ.pop();
        //ch could have died and been deleted, so check first
        if(!ch.expired())
        {
            std::cout << ch->getName() << " is performing action..." << std::endl;
            unsigned int actionNr = rand() % randomActions.size();
            //if action not possible then try again until an action is performed
            while( !(ch->*randomActions[actionNr])() )
            {
                actionNr = rand() % randomActions.size();
            }
            updatePlayers();
        }
    }
*/
}
bool hax::Level::focus(std::string selectChar) //select a Character for executing character specific commands, no need to input name each time
{
    if(players.find(selectChar) != players.end()){
        curChar = players[selectChar];
        curCharName = selectChar;
        return true;
    }else{
        std::cout << "This character does not exist" << std::endl;
        return false;
    }
}
void hax::Level::info()
{
    std::cout << "Total number of areas: " << vec_area.size() << std::endl;
    std::cout << "Total number of characters: " << players.size() << std::endl;
    std::string tmp;
    tmp.append("Current players: ");
    std::map<std::string, Character*>::const_iterator it = players.begin();
    while(it != players.end()){
        tmp.append(it->second->getName() + " in " + it->second->curArea->getName() + ", ");
        it++;
    }
    std::cout << tmp << std::endl;
}
void hax::Level::help()
{
    std::ostringstream oss;
    oss << "**Global commands**" << std::endl;
    oss << "quit - Quit this level" << std::endl;
    oss << "save <filename> - Save level" << std::endl;
    oss << "load <filename> - Load level" << std::endl;
    //TODO new game

    oss << std::endl << "**Level specific commands**" << std::endl;
    std::map<std::string,std::string>::const_iterator it = levelHelp.begin();
    while(it != levelHelp.end()){
        oss << it->first << " - " << it->second << std::endl;
        it++;
    }
    oss << std::endl << "**Character specific commands**" << std::endl;
    oss << "These commands can be executed by either entering \"<character> <command> <argument>\", or first focusing a character and then omitting the character name, i.e. \"focus <character>\" and then \"<command> <argument>\"." << std::endl << std::endl;
    it = charHelp.begin();
    while(it != charHelp.end()){
        oss << it->first << " - " << it->second << std::endl;
        it++;
    }

#ifdef DEBUG
    std::cout << oss.str();
#else
    hax::log.write(oss.str());
#endif
}
void hax::Level::spawn()
{
//TODO get random pointer from vec_area
    Area* spawnLocation = vec_area[0];
    Character* spawnedPlayer = spawnLocation->spawn();
    if(spawnedPlayer != NULL){
        add(spawnedPlayer);
    }
}
bool hax::Level::kill(std::string name)
{
    if(players.find(name) != players.end()){
        if(curChar != NULL && curChar->getName() == name){
            curChar = NULL;
        }
        Character* killedPlayer = players.find(name)->second;
        players.erase(name);
        delete killedPlayer;
        return true;
    }else{
        std::cout << "This character does not exist!" << std::endl;
        return false;
    }
}
bool hax::Level::save(std::string filename)
{
//    serializeQueue.clear(); //TODO is this needed?
    std::ofstream outFile(("saved/"+filename+".dat").c_str());
    serializeQueue.push(this);
    while(!serializeQueue.empty())
    {
        outFile << *(serializeQueue.front());
        serializeQueue.pop();
    }
    outFile.close();
}
bool hax::Level::load(std::string filename)
{
    std::ifstream inFile(("saved/"+filename+".dat").c_str());
    if(!inFile)
    {
        std::cout << "File not found!" << std::endl;
        return false;
    }

    deleteAreas();
    players.clear();
    pointerTable.clear();
    std::cout << "Current objects deleted. Starting to load from "+filename+"..." << std::endl;

    std::ofstream dbg;
    dbg.open("load_debug.dat", std::ios::out | std::ios::trunc); //if file existed before it will be overwritten
    dbg << "**Load game**\n";

    //first line is data for Level, this is a special case since Level does not need to be allocated
    std::string data;
    std::getline(inFile,data);
    dbg << "Reading line: " << data << std::endl;
    std::vector<std::string> parsedObj = split(data,':');
    std::string UID = parsedObj[0];
    std::string type = parsedObj[1];
    pointerTable.insert( std::pair<std::string,ISerializable*>(UID,this) );

    //Step 1: Go through file and allocate every object (one line per object) with default(+name) constructor
    while(std::getline(inFile,data))
    {
        dbg << "Reading line: " << data << std::endl;
        std::vector<std::string> parsedObj = split(data,':');
        //Step 2: Generate a std::map with the UID of the object as key value and a pointer to the object as mapped value (this is done during step 1)
        std::string UID = parsedObj[0];
        std::string type = parsedObj[1];
        std::string name = parsedObj[2];
        pointerTable.insert( std::pair<std::string,ISerializable*>(UID, allocateData(type,name)) );
        if(pointerTable[UID] != NULL)
        {
            dbg <<"Allocated "<< type <<"@"<< UID << std::endl;
        }
        else
        {
            dbg <<"The object type "<< type <<" is unknown, cannot allocate data!" << std::endl;
        }
    }
    dbg << std::endl << "All objects are allocated and their pointers are stored in a lookup table." << std::endl;

    //Step 3: Go through the file again (or the saved stream data) and read data into each object
    inFile.clear();
    inFile.seekg(0, std::ios::beg);
    dbg << std::endl << "Re-reading the file..." << std::endl;
    std::getline(inFile,data,':'); //read UID
    while(!inFile.eof())
    {
        dbg << std::endl << "Reading data into object..." << std::endl;
        dbg << "UID = " << data << " | New address = " << pointerTable[data] << std::endl;
        dbg.close();
        inFile >> *(pointerTable[data]);
//        std::getline(inFile,data); //throw away rest of line OBS segfault, inFile is a reference so the rest of the line has already being read!!!!!
        dbg.open("load_debug.dat", std::ios::out | std::ios::app); //append to file
        dbg << "Finished reading data into object." << std::endl;
        std::getline(inFile,data,':'); //read UID (or EOF)
    }
    dbg << std::endl << "All data read. Checking and adding controllable players..." << std::endl;
    updatePlayers();
    dbg << std::endl << "Finished loading game state. GLHF!" << std::endl;
    inFile.close();
    dbg.close();
    return true;
}
void hax::Level::deleteAreas()
{
    for(size_t i=0; i<vec_area.size(); i++){delete vec_area[i];}
    vec_area.clear();
}
