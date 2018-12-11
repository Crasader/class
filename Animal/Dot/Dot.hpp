//
//  Dot.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 3/22/18.
//

#ifndef Dot_hpp
#define Dot_hpp

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;
using namespace cocos2d::ui;
class Dot: public Sprite
{
public:
    bool init();
    static Dot* create(int id, int col,int row, int name, int posX, int posY, bool isClicked);
    Dot(int id, int col,int row, int name, int posX, int posY, bool isClicked);
    ~Dot();
//    static Dot* create(int id, int position, int name, );
//    void
    
    void setId(int id);
    int  getId();
    void setNameDot(int name);
    int  getNameDot();
    
    void setX(int name);
    int  getX();
    void setY(int pos);
    int  getY();
    
    void setRow(int name);
    int  getRow();
    void setCol(int pos);
    int  getCol();
//    void setPosition(int id);
//    int  getPosition();
    void setClick(bool isClick);
    bool getClick();
    
    void setLastSquare(bool isClick);
    bool getLastSquare();
    
    void reload();
private:
    int id;
    int row;
    int col;
    int size;
    int name;
    int X;
    int Y;
    bool isClicked;
    
  
    // lưu giá trị được phép nối ngược lại khi đã thành square
    bool isLastSquare = false;
};
#endif /* Dot_hpp */
