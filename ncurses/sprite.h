#ifndef SPRITE_H
#define SPRITE_H

#include<ncurses.h>

#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 4
#define DIR_RIGHT 8

struct Position{
    int posx;
    int posy;
};

class Sprite{
public:

    Sprite(){
        this->posx = 1;
        this->posy = 1;
        this->lastx = 1;
        this->lasty = 1;
        this->img = 35;
	    this->visible = true;
    };
    Sprite(int x,int y,int img){
        this->posx = x;
        this->posy = y;
        this->lastx = x;
        this->lasty = y;
        this->img = img;
	    this->visible = true;
    }

    void setImg(int img){
        this->img = img;
    };
    int getImg(){
        return this->img;
    };
    void setVisible(bool visible){
        this->visible = visible;
    };
    bool getVisible(){
        return this->visible;
    };
    virtual void draw(){
        if(visible){
            mvprintw(posx,posy,"%c",img);    
        }
    };
    //should be called before the sprite is moved again
    virtual void drawClear(){
        if(visible){
            mvprintw(posx,posy," ");
        }
    };

    virtual void moveDirection(int direction){
        switch(direction){
            case DIR_UP:
                setPosition(--this->posx,this->posy);
                break;
            case DIR_DOWN:
                setPosition(++this->posx,this->posy);
                break;
            case DIR_LEFT:
                setPosition(this->posx,--this->posy);
                break;
            case DIR_RIGHT:
                setPosition(this->posx,++this->posy);
                break;
        }    
    };

    //only used when collisionchecking
    void moveBack(){
        posx = lastx;
        posy = lasty;
    };

    virtual void setPosition(int x,int y){
        saveOldPos();
        this->posx = x;
        this->posy = y;
    };

    virtual void move(int dx,int dy){
        setPosition(posx+dx,posy+dy);
    };

    int getPosX(){
        return this->posx;
    };
    int getPosY(){
        return this->posy;
    };
        

protected:
    bool visible;
    int img;
    int posx;
    int posy;
    int lastx;
    int lasty;


    //functions that is called when action is performed
    int (Sprite::*collidesWidth)(Sprite);
    int (Sprite::*attacks)(Sprite);

    void saveOldPos(){
        this->lastx = this->posx;
        this->lasty = this->posy;
    };

private:
    
};

#endif
