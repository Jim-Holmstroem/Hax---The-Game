#ifndef MULTISPRITE_H
#define MULTISPRITE_H
    
    //http://www.java2s.com/Code/Cpp/Vector/Callmemberfunctionforeachelementinvector.htm
    //http://www.eng.cam.ac.uk/help/tpl/languages/C++/bind.html
    #include "sprite.h"
    #include<vector>
    #include<algorithm>
    #include<functional>
    
    class MultiSprite : public Sprite{
    public:
        
        MultiSprite(){
            
        };
        MultiSprite(std::vector<Sprite*> sprites){
            this->sprites = sprites;
        };

        void draw(){
            if(visible){
                std::for_each(sprites.begin(),sprites.end(),std::mem_fun(&Sprite::draw));  
            }
        };
        void drawClear(){
	//TODO if visible set to false it wont clear the very-last draw()
            if(visible){
                std::for_each(sprites.begin(),sprites.end(),std::mem_fun(&Sprite::drawClear));
            }
        };
        void addSprite(Sprite* s){
            if(this!=s){//avoid putting yourself in yourself            
                s->move(posx,posy);//move it from the relative position it has now
                sprites.push_back(s);
            }
        };
        void eraseSprite(std::vector<Sprite*>::iterator sprite){
	//TODO should it something be deleted here ??
            sprites.erase(sprite);
        };

        void moveDirection(int direction){
            Sprite::moveDirection(direction); //move yourself

            std::for_each(sprites.begin(),sprites.end(),std::bind2nd(std::mem_fun(&Sprite::moveDirection),direction));            

        };
        void setPosition(int x,int y){
            Sprite::setPosition(x,y);	
            int dx= x-posx; //the movement of the multisprite
            int dy= y-posy;

            std::vector<Sprite*>::iterator sprite;
            for(sprite = sprites.begin();sprite!=sprites.end();sprite++){
                (*sprite)->move(dx,dy);
            }
            
        };
        void move(int dx,int dy){
            Sprite::move(dx,dy);//move yourself
            
            //and then move all the children sprites
            //std::for_each(sprites.begin(),sprites.end(),std::bind2nd(std::bind2nd(std::mem_fun_ref(&Sprite::move),dx,dy)));
            std::vector<Sprite*>::iterator sprite;
            for(sprite = sprites.begin();sprite!=sprites.end();sprite++){
                (*sprite)->move(dx,dy);
            }
        };

    protected:
        //posxy/lastxy is the upper left most corner of the multisprite
        //have still got the img and other unused varibles, doesnt use em and cant be accesed from outside
        std::vector<Sprite*> sprites;
    private:

    };

#endif
