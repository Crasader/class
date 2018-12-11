//
//  Dot.cpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 3/22/18.
//

#include "Dot.hpp"
#include "ThemeManager.hpp"

Dot::Dot(int id, int col,int row, int name, int posX, int posY, bool isClicked):id(id), row(row), col(col), name(name), X(posX), Y(posY), isClicked(isClicked){
    
}

Dot::~Dot(){
    this->setTexture(NULL);
}

bool Dot::init(){
    if(!Sprite::init()){
        return false;
    }
//    this->initWithSpriteFrameName("CloseNormal.png");
    
    this->setTexture(ThemeManager::getInstance()->getNameAnimal(name));
    return true;
    
}

Dot *Dot::create(int id, int row,int col, int name, int posX, int posY, bool isClicked){
    Dot * dot = new Dot(id, row, col, name, posX, posY, isClicked);
    if (dot && dot->init()) {
        dot->autorelease();
        return dot;
    }else{
        delete dot;
        dot = NULL;
        return NULL;
    }
}

int Dot::getId(){
    return this->id;
}

void Dot::setId(int id){
    this->id = id;
}

int Dot::getNameDot(){
    return this->name;
}
void Dot::setNameDot(int name){
    this->name = name;
}

void Dot::setRow(int row){
    this->row = row;
}
int Dot::getRow(){
    return this->row;
}

void Dot::setCol(int col){
    this->col = col;
}

int Dot::getCol(){
    return this->col;
}


int Dot::getX(){
    return this->X;
}

void Dot::setX(int x){
    this->X = x;
}

bool Dot::getClick(){
    return this->isClicked;
}

void Dot::setClick(bool isClick){
    this->isClicked = isClick;
}

bool Dot::getLastSquare(){
    return this->isLastSquare;
}

void Dot::setLastSquare(bool isClick){
    this->isLastSquare = isClick;
}

int Dot::getY(){
    return this->Y;
}

void Dot::setY(int y){
    this->Y = y;
}
void Dot::reload(){
    this->setTexture(ThemeManager::getInstance()->getNameAnimal(name));
}

