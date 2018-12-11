//
//  RoundLayerSlot.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 8/31/16.
//
//

#include "RoundLayerSlot.h"
#include "../SceneManager.h"
#include "PhomMessDef.h"
bool RoundLayerSlot::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    
    this->isStart = true;
    this->m_pRound = Layer::create();
    
    //    this->arrayHeader.push_back(4);
    //    this->arrayHeader.push_back(7);
    //    this->arrayHeader.push_back(6);
    //    this->arrayHeader.push_back(8);
    //    this->arrayHeader.push_back(10);
    //    this->arrayHeader.push_back(13);
    //    this->arrayHeader.push_back(3);
    //    this->arrayHeader.push_back(14);
    //    this->arrayHeader.push_back(12);
    //    this->arrayHeader.push_back(2);
    //    this->arrayHeader.push_back(5);
    //    this->arrayHeader.push_back(9);
    //    this->arrayHeader.push_back(11);
    //    this->arrayHeader.push_back(15);
    this->m_pRound->setPosition(0,0);
    
    auto stencil = DrawNode::create();
    Vec2 rectangle[4];
    rectangle[0] = Vec2(0, -10);//2 dau
    rectangle[1] = Vec2(226, -10);//2 dau
    rectangle[2] = Vec2(226,553);
    rectangle[3] = Vec2(5, 553);//chieu dai
    
    this->initCollectibleView(this->m_pRound);
    
    
    Color4F color(0,78,165,255);
    stencil->drawPolygon(rectangle, 4,color, 26, color);
    auto clipper = ClippingNode::create(stencil);
    clipper->setPosition(0, 38);
    clipper->setInverted(false);
    this->addChild(clipper);
    
    
    this->m_pRoundTop = Layer::create();
    this->initCollectibleViewTop(this->m_pRoundTop);
    this->m_pRoundTop->setPosition(Vec2(0,this->layerHeight));
    
    clipper->addChild(this->m_pRoundTop);
    clipper->addChild(this->m_pRound);
    
    this->bRet = true;
    
    return true;
}
void RoundLayerSlot::updateRound(float dt)
{
    
    
    
    if (this->m_pRoundTop->getPositionY() < 0)
    {
        this->m_pRound->setPosition(Vec2(this->m_pRound->getPositionX(),-120));
    }
    this->m_pRound->setPosition(Vec2(this->m_pRound->getPositionX(),this->m_pRound->getPositionY()-120));
    this->m_pRoundTop->setPosition(Vec2(this->m_pRound->getPositionX(),this->m_pRound->getPositionY()+this->layerHeight));
    if (!this->isStart)
    {
        this->stopUpdate();
    }
}
void RoundLayerSlot::resetRound(){
    this->lstResult.clear();
    this->tagEat.clear();
    for (int i = 0; i<this->lstItem.size();i++){
        int t = i;
        if (i==9)
            t=99;
        this->lstItem[i]->setTexture(StringUtils::format("design/ResSlot/item/%d.png",t+1));
        this->lstItem[i]->stopAllActions();
        this->lstItem[i]->setScale(1);
        this->lstItem[i]->setTag(t+1);
        this->lstItem[i]->removeAllChildrenWithCleanup(true);
    }    
    this->m_pRound->removeChildByTag(this->tagItem2);
    this->m_pRound->removeChildByTag(this->tagItem3);
}
void RoundLayerSlot::startRound()
{
    this->isStart = true;
    this->m_pRoundTop->setVisible(true);
    this->lstResult.clear();
    this->tagEat.clear();
    for (int i = 0; i<this->lstItem.size();i++){
        int t = i;
        if (i==9)
            t=99;
        this->lstItem[i]->setTexture(StringUtils::format("design/ResSlot/item/%d.png",t+1));
        this->lstItem[i]->stopAllActions();
        this->lstItem[i]->setScale(1);
        this->lstItem[i]->setTag(t+1);
        this->lstItem[i]->removeAllChildrenWithCleanup(true);
    }
    Director::getInstance()->getScheduler()->schedule(schedule_selector(RoundLayerSlot::updateRound),this, 1/60, false);

    this->m_pRound->removeChildByTag(this->tagItem2);
    this->m_pRound->removeChildByTag(this->tagItem3);
    //Director::getInstance()->getScheduler()->schedule(schedule_selector(RoundLayerSlot::updateRound));
    //Director::getInstance()->getScheduler()->scheduleUpdate();
}
void RoundLayerSlot::resetRoundToshow(){
    for (int i = 0; i<this->lstResult.size();i++){
        this->lstResult[i]->stopAllActions();
        this->lstResult[i]->setScale(1);
        this->lstResult[i]->removeAllChildrenWithCleanup(true);
    }
    this->tagEat.clear();
}

void RoundLayerSlot::stopRound(string tag)
{
    
    this->stopTag = tag;
    auto lstTag = mUtils::splitString(this->stopTag,'_');
    if (lstTag.size()<3) return;
    int tagShow = atoi(lstTag[2].c_str());
    int tagShowPos = this->getPosition(tagShow);
    
    int tagShowPos2 = 0;
    Sprite* node = (Sprite*)this->m_pRound->getChildByTag(tagShow);
    if (!node) return;

    this->lstResult.push_back(node);

    if (tagShowPos == this->lstItem.size()-1)
    {
        int height = 214;
        string path1 = StringUtils::format("design/ResSlot/item/%s.png",lstTag[1].c_str());
        Sprite* pSprite1 = Sprite::create();
        pSprite1->setTexture(path1);
        pSprite1->setAnchorPoint(Vec2(0.5, 0.5));
        this->tagItem2 = this->tagItem+atoi(lstTag[1].c_str());
        pSprite1->setTag( this->tagItem2);
        pSprite1->setPosition(Vec2(this->width/2, node->getPositionY() +height));
        this->m_pRound->addChild(pSprite1);
        
        this->lstResult.push_back(pSprite1);
        
        string path2 = StringUtils::format("design/ResSlot/item/%s.png",lstTag[0].c_str());
        Sprite* pSprite2 = Sprite::create();
        pSprite2->setTexture(path2);
        pSprite2->setAnchorPoint(Vec2(0.5, 0.5));
        this->tagItem3 = this->tagItem+atoi(lstTag[0].c_str());

        pSprite2->setTag(this->tagItem3);
        pSprite2->setPosition(Vec2(this->width/2, node->getPositionY() +height*2));
        this->m_pRound->addChild(pSprite2);
        
        this->lstResult.push_back(pSprite2);
        
        this->isStart = false;

        return;
    }
    else{
        tagShowPos2=tagShowPos+1;
        Sprite* node2 = this->lstItem[tagShowPos2];
        if (!node2) return;
        node2->setTexture(StringUtils::format("design/ResSlot/item/%s.png",lstTag[1].c_str()));
        node2->setTag(atoi(lstTag[1].c_str()));
        this->lstResult.push_back(node2);
    }
    
    int tagShowPos3 = 0;
    if (tagShowPos2 == this->lstItem.size()-1)
    {
        Sprite* node2 = (Sprite*)this->m_pRound->getChildByTag(tagShowPos2);
        if (!node2) return;

        int height = 214;
        string path1 = StringUtils::format("design/ResSlot/item/%s.png",lstTag[0].c_str());
        Sprite* pSprite1 = Sprite::create();
        pSprite1->setTexture(path1);
        pSprite1->setAnchorPoint(Vec2(0.5, 0.5));
        this->tagItem3 = this->tagItem+atoi(lstTag[0].c_str());
        pSprite1->setTag(this->tagItem3);
        pSprite1->setPosition(Vec2(this->width/2, node2->getPositionY() +height*2));
        this->m_pRound->addChild(pSprite1);

        this->lstResult.push_back(pSprite1);
    }
    else{
        tagShowPos3=tagShowPos2+1;
        Sprite* node3 = this->lstItem[tagShowPos3];
        if (!node3) return;

        node3->setTexture(StringUtils::format("design/ResSlot/item/%s.png",lstTag[0].c_str()));
        node3->setTag(atoi(lstTag[0].c_str()));
        this->lstResult.push_back(node3);
    }
    
    this->isStart = false;


    //CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(<#unsigned int soundId#>)("bm-slot.mp3");
}
void RoundLayerSlot::swapItem(int tagFrom, int tagEnd){
    
}
void RoundLayerSlot::stopUpdate()
{
    auto lstTag = mUtils::splitString(this->stopTag,'_');
    if (lstTag.size()<3) return;
    int tagShow = atoi(lstTag[2].c_str());
    Sprite* node = (Sprite*)this->m_pRound->getChildByTag(tagShow);

    if (node == NULL)
        return;
    Rect rect = Rect(this->m_pRound->getPositionX(),node->getPositionY()+this->m_pRound->getPositionY(),node->getBoundingBox().size.width,node->getBoundingBox().size.height);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(RoundLayerSlot::updateRound),this);
    this->m_pRound->runAction(Sequence::create(MoveTo::create(0.2, Vec2(node->getPositionX()-this->width/2,-node->getPositionY()-60+this->height/2)),MoveTo::create(0.2, Vec2(node->getPositionX()-this->width/2,-node->getPositionY()+this->height/2-45)), NULL));
    this->m_pRoundTop->setVisible(false);
}
int RoundLayerSlot::getPosition(int tag){
    for (int i = 0 ; i<this->lstItem.size();i++)
    {
        if (this->lstItem[i]->getTag()==tag){
            return i;
        }
    }
}
void RoundLayerSlot::initCollectibleViewTop(Layer* layer)
{
    int height = 0;
    int i = 0;
    this->arrCard.clear();
    for (int j = 1;j <= 10;j++)
    {
        if (j == 10) j = 100;

        string path = StringUtils::format("design/ResSlot/item/%d.png",j );
        Sprite* pSprite = Sprite::create();
        pSprite->setTexture(path);
        pSprite->setAnchorPoint(Vec2(0, 0));
        pSprite->setTag(j);
        slot c;
        c.str = StringUtils::format("%d",j);
        c.tag = j;
        this->arrCard.push_back(c);
        height = 214;
        pSprite->setPosition(Vec2(0, (height) * i-60));
        layer->addChild(pSprite);
        i++;
    }
    this->layerHeight =  (height) * i ;
}
void RoundLayerSlot::initCollectibleView(Layer* layer)
{
    int height = 0;
    int i = 0;
    this->arrCard.clear();
    this->lstItem.clear();
    for (int j = 1;j <= 10;j++)
    {
        if (j == 10) j = 100;

        string path = StringUtils::format("design/ResSlot/item/%d.png",j );
        Sprite* pSprite = Sprite::create();
        pSprite->setTexture(path);
        pSprite->setAnchorPoint(Vec2(0.5, 0.5));
        pSprite->setTag(j);
        slot c;
        c.str = StringUtils::format("%d",j);
        c.tag = j;
        this->arrCard.push_back(c);
        height = 214;
        pSprite->setPosition(Vec2(this->width/2, (height) * i-60+height/2));
        layer->addChild(pSprite);
        this->lstItem.push_back(pSprite);
        i++;
    }
    this->layerHeight =  (height) * i ;

    
}
void RoundLayerSlot::setStopCallBackFunc(Ref* target, SEL_CallFuncO callfun)
{
    this->m_callbackListener = callfun;
    this->m_callback = target;
}
int RoundLayerSlot::getTagCardFromStr(string strCard){
   // log("tag card : %s",strCard.c_str());
    for (int i = 0; i < this->arrCard.size();i++){
        if (strcmp(this->arrCard[i].str.c_str(),strCard.c_str()) == 0) {
            return this->arrCard[i].tag;
        }
    }
   // log("tag card : fail to find : %s",strCard.c_str());
    return 0;
}
void RoundLayerSlot::pushItemEat(int type,int row){
    this->tagEat.push_back(type);
    for (int i = 0; i<this->tagEat.size(); i++) {
            if (this->lstResult[row-1]->getTag() == this->tagEat[i] || this->lstResult[row-1]->getTag() == (this->tagEat[i]+this->tagItem)){
                Sprite* pSprite1 = Sprite::create();
                pSprite1->setTexture("ResSlot/light_item.png");
                pSprite1->setAnchorPoint(Vec2(0.5,0.5));
                pSprite1->setPosition(Vec2(pSprite1->getContentSize().width/2,pSprite1->getContentSize().height/2));
                this->lstResult[row-1]->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.1),ScaleTo::create(0.5, 1), NULL)));
                this->lstResult[row-1]->addChild(pSprite1);
                pSprite1->runAction(RepeatForever::create(RotateBy::create(0.1, 60)));
                
        }
    }
}
void RoundLayerSlot::onEnter() {
    Layer::onEnter();
}
void RoundLayerSlot::onExit() {
    Layer::onExit();
}
