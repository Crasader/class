//
//  RoundLayer.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 8/31/16.
//
//

#include "RoundLayer.h"
#include "../SceneManager.h"
#include "PhomMessDef.h"
bool RoundLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    
    this->isStart = true;
    this->m_pRound = Layer::create();
    
    this->arrayHeader.push_back(4);
    this->arrayHeader.push_back(7);
    this->arrayHeader.push_back(6);
    this->arrayHeader.push_back(8);
    this->arrayHeader.push_back(10);
    this->arrayHeader.push_back(13);
    this->arrayHeader.push_back(3);
    this->arrayHeader.push_back(14);
    this->arrayHeader.push_back(12);
    this->arrayHeader.push_back(2);
    this->arrayHeader.push_back(5);
    this->arrayHeader.push_back(9);
    this->arrayHeader.push_back(11);
    this->arrayHeader.push_back(15);
    this->m_pRound->setPosition(0,0);
    
    auto stencil = DrawNode::create();
    Vec2 rectangle[4];
    if (SceneManager::getSingleton().isNagaWin){
        rectangle[0] = Vec2(0, -17);//2 dau
        rectangle[1] = Vec2(126, -17);//2 dau
        rectangle[2] = Vec2(126,133);
        rectangle[3] = Vec2(5, 133);//chieu dai
    }else{
        rectangle[0] = Vec2(0, -17);//2 dau
        rectangle[1] = Vec2(126, -17);//2 dau
        rectangle[2] = Vec2(126,53);
        rectangle[3] = Vec2(5, 53);//chieu dai
    }
   
    
    this->initCollectibleView(this->m_pRound);

    
    Color4F color(0,78,165,255);
    stencil->drawPolygon(rectangle, 4,color, 26, color);
    auto clipper = ClippingNode::create(stencil);
    clipper->setPosition(0, 38);
    clipper->setInverted(false);
    this->addChild(clipper);
    
    
    this->m_pRoundTop = Layer::create();
    this->initCollectibleView(this->m_pRoundTop);
    this->m_pRoundTop->setPosition(Vec2(0,this->layerHeight));
    
    clipper->addChild(this->m_pRoundTop);
    clipper->addChild(this->m_pRound);
    
    this->bRet = true;
    
    return true;
}
void RoundLayer::updateRound(float dt)
{
    

    
    if (this->m_pRoundTop->getPositionY() < 0)
    {
        this->m_pRound->setPosition(Vec2(this->m_pRound->getPositionX(),-90));
    }
    this->m_pRound->setPosition(Vec2(this->m_pRound->getPositionX(),this->m_pRound->getPositionY()-90));
    this->m_pRoundTop->setPosition(Vec2(this->m_pRound->getPositionX(),this->m_pRound->getPositionY()+this->layerHeight));
    if (!this->isStart)
    {
        this->stopUpdate();
    }
}
void RoundLayer::startRound()
{
    this->isStart = true;
    this->m_pRoundTop->setVisible(true);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(RoundLayer::updateRound),this, 1/60, false);
    //Director::getInstance()->getScheduler()->schedule(schedule_selector(RoundLayer::update));
    //Director::getInstance()->getScheduler()->scheduleUpdate();
}
void RoundLayer::stopRound(int tag)
{
    
    this->isStart = false;
    this->stopTag = tag;
    //CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(<#unsigned int soundId#>)("bm-slot.mp3");

}
void RoundLayer::stopUpdate()
{
    
    Sprite* node = (Sprite*)this->m_pRound->getChildByTag(this->stopTag);
    //cc.log("tag:"+this->stopTag);
    if (node == NULL)
        return;
    Rect rect = Rect(this->m_pRound->getPositionX(),node->getPositionY()+this->m_pRound->getPositionY(),node->getBoundingBox().size.width,node->getBoundingBox().size.height);
    // if(cc.rectContainsPoint(rect, cc.p(0,0)))
    //{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(RoundLayer::updateRound),this);
    this->m_pRound->runAction(Sequence::create(MoveTo::create(0.2, Vec2(node->getPositionX(),-node->getPositionY()-65)),MoveTo::create(0.2, Vec2(node->getPositionX(),-node->getPositionY()-45)), NULL));
    //this->m_pRound->setPosition(Vec2(node->getPositionX(),-node->getPositionY()-45));
    this->m_pRoundTop->setVisible(false);
//    if (this->m_callback && this->m_callbackListener){
//    
//        //(m_callback->*m_callbackListener)(param);
//    }
//  }
}
void RoundLayer::initCollectibleView(Layer* layer)
{
    int height = 126;
    if (SceneManager::getSingleton().isNagaWin){
        height = 202;
    }
    int i = 0;
    this->arrCard.clear();
    for (int j = 0;j < this->arrayHeader.size();j++)
    {
        for (int k = 1;k <= 4;k++) {
            
            if (this->arrayHeader[j] == 15){
                if (k == 4){
                    string path = StringUtils::format("%d_%d.png",this->arrayHeader[j],k );
                    Sprite* pSprite = Sprite::create(path);
                    pSprite->setAnchorPoint(Vec2(0, 0));
                    pSprite->setTag(i);
                    card c;
                    c.str = StringUtils::format("%d_%d",this->arrayHeader[j], k);
                    c.tag = i;
                    this->arrCard.push_back(c);
                 
                    pSprite->setPosition(Vec2(0, (height) * i));
                    layer->addChild(pSprite);
                    i++;
                }
            }
            else{
                string path = StringUtils::format("%d_%d.png",this->arrayHeader[j],k );
                Sprite* pSprite = Sprite::create(path);
                pSprite->setAnchorPoint(Vec2(0, 0));
                pSprite->setTag(i);
                card c;
                c.str = StringUtils::format("%d_%d",this->arrayHeader[j], k);
                c.tag = i;
                this->arrCard.push_back(c);
                
                pSprite->setPosition(Vec2(0, (height) * i));
                layer->addChild(pSprite);
                i++;

            }
        }
    }
    this->layerHeight =  (height) * i ;
}
void RoundLayer::setStopCallBackFunc(Ref* target, SEL_CallFuncO callfun)
{
    this->m_callbackListener = callfun;
    this->m_callback = target;
}
int RoundLayer::getTagCardFromStr(string strCard){
   // log("tag card : %s",strCard.c_str());
    for (int i = 0; i < this->arrCard.size();i++){
        if (strcmp(this->arrCard[i].str.c_str(),strCard.c_str()) == 0) {
            return this->arrCard[i].tag;
        }
    }
   // log("tag card : fail to find : %s",strCard.c_str());
    return 0;
}
void RoundLayer::onEnter() {
    Layer::onEnter();
}
void RoundLayer::onExit() {
    Layer::onExit();
}
