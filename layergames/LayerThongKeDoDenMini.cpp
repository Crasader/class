//
//  LayerThongKeDoDenMini.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/19/17.
//
//

#include "LayerThongKeDoDenMini.h"
//
//  LayerThongKeDoDenMini.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/24/17.
//
//

#include "LayerThongKeDoDenMini.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
LayerThongKeDoDenMini::LayerThongKeDoDenMini()
{
    GameServer::getSingleton().addListeners(this);
}


LayerThongKeDoDenMini::~LayerThongKeDoDenMini()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerThongKeDoDenMini::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerThongKeDoDenMini.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
    
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
        rootNode->setAnchorPoint(Point(0.5, 0.5));
        rootNode->setPosition(Point(visibleSize / 2));
        Size sizeAdd;
     //   if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
            sizeAdd = ChanUtils::getSizePos();
    
//        }else{
//            sizeAdd = Size(0,0);
//        }
        Size sizeAdd2 = Size(0,0);
        rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(851));
    this->nodeThongKe =dynamic_cast<Node*>(pnlBg->getChildByName("nodeThongKe"));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerThongKeDoDenMini::onBtnClose, this));
    }
    
    this->txtPhienGanNhat = dynamic_cast<Text*>(this->nodeThongKe->getChildByName("txtPhienGanNhat"));

    this->imgBieuDo1 = (ImageView*)this->nodeThongKe->getChildByName("imgBieuDo1");
    this->imgBieuDo1->setVisible(false);
    this->imgBieuDo2 = (ImageView*)this->nodeThongKe->getChildByName("imgBieuDo2");
    this->imgBieuDo2->setVisible(false);


//    boost::shared_ptr<ISFSObject> params(new SFSObject());
//    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_HISTORY_ANALYTICS_TX_REQ, params));
//    GameServer::getSingleton().Send(request);

    
//    SceneManager::getSingleton().showLoading();
 //   this->onBtnBack(NULL);
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

       return true;
}

void LayerThongKeDoDenMini::drawThongKe1(){
//    bool iscau = false;
//    int vancau = 0;
//    int currCau = 0;
//    for (int i = 0; i < this->lstTongDiem.size();i++){
//        if (i == 0)
//            iscau = true;
//        else{
//            if (this->lstTongDiem[i] >=11 && this->lstTongDiem[i-1] >=11){
//                iscau = true;
//                vancau++;
//            }else if (this->lstTongDiem[i] <11 && this->lstTongDiem[i-1] >=11){
//                iscau = false;
//                currCau++;
//                vancau = 0;
//            }
//            if (this->lstTongDiem[i] <11 && this->lstTongDiem[i-1] <11){
//                iscau = true;
//                vancau++;
//            }else if (this->lstTongDiem[i] >=11 && this->lstTongDiem[i-1] <11){
//                iscau = false;
//                currCau++;
//                vancau = 0;
//            }
//            
//        }
//        int posx = this->thongke1Org->getPositionX()+(this->width*(currCau));
//        int posy = this->thongke1Org->getPositionY()-(this->height*(vancau%6));
//        if (this->lstTongDiem[i] >= 11) {
//
//            auto dot = Sprite::create("LayerDoDenMini/thongke/quanden.png");
//            dot->setPosition(Vec2(posx,posy));
//           // dot->setScale(0.7);
//                        Label* txt = Label::createWithTTF(StringUtils::format("%d",this->lstTongDiem[i]), "__Roboto-Bold.ttf", 30);
//                        txt->setAnchorPoint(Vec2(0.5, 0.5));
//                        dot->addChild(txt);
//                        txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
//            dot->setAnchorPoint(Vec2(0.5, 0.5));
//            this->nodeThongKe->addChild(dot);
//        }else{
//            auto dot = Sprite::create("LayerDoDenMini/thongke/quantrang.png");
//            dot->setPosition(Vec2(posx,posy));
//                        Label* txt = Label::createWithTTF(StringUtils::format("%d",this->lstTongDiem[i]), "__Roboto-Bold.ttf", 30);
//                        txt->setAnchorPoint(Vec2(0.5, 0.5));
//                        dot->addChild(txt);
//                        txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
//                        txt->setColor(Color3B::BLACK);
//            dot->setAnchorPoint(Vec2(0.5, 0.5));
//            this->nodeThongKe->addChild(dot);
//            //dot->setScale(0.7);
//            
//        }
//    }

}
void LayerThongKeDoDenMini::drawThongKe2(){
//    int tai =0;
//    int xiu = 0;
//    for (int i = 0; i < this->lstTongDiem.size();i++){
//        int posx = this->thongke2Org->getPositionX()+(this->width*(i/6));
//        int posy = this->thongke2Org->getPositionY()-(this->height*(i%6));
//        if (this->lstTongDiem[i] >= 11) {
//            tai++;
//            auto dot = Sprite::create("LayerDoDenMini/thongke/quanden.png");
//            dot->setPosition(Vec2(posx,posy));
//            //dot->setScale(0.7);
////            Label* txt = Label::createWithTTF(StringUtils::format("%d",this->lstTongDiem[i]), "__Roboto-Bold.ttf", 30);
////            txt->setAnchorPoint(Vec2(0.5, 0.5));
////            dot->addChild(txt);
////            txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
//            dot->setAnchorPoint(Vec2(0.5, 0.5));
//            this->nodeThongKe->addChild(dot);
//        }else{
//            xiu++;
//            auto dot = Sprite::create("LayerDoDenMini/thongke/quantrang.png");
//            dot->setPosition(Vec2(posx,posy));
////            Label* txt = Label::createWithTTF(StringUtils::format("%d",this->lstTongDiem[i]), "__Roboto-Bold.ttf", 30);
////            txt->setAnchorPoint(Vec2(0.5, 0.5));
////            dot->addChild(txt);
////            txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
////            txt->setColor(Color3B::BLACK);
//            dot->setAnchorPoint(Vec2(0.5, 0.5));
//            this->nodeThongKe->addChild(dot);
//            //dot->setScale(0.7);
//            
//        }
//    }

}
void LayerThongKeDoDenMini::loadThongKe(string lhs){
    this->lstXucXac1.clear();
    this->lstXucXac2.clear();
    this->lstXucXac3.clear();
    this->lstTongDiem.clear();
    auto lst = mUtils::splitString(lhs,';');
    int k = 0;
    if (lst.size()==20)
        k = 1;
    for (auto i = k; i < lst.size(); i++){
        auto it = mUtils::splitString(lst[i],'|');
        if (it.size() < 2)
            continue;
        auto kq = mUtils::splitString(it[1],'-');
        if (kq.size() < 3)
            continue;
        this->lstXucXac1.push_back(atoi(kq[0].c_str()));
        this->lstXucXac2.push_back(atoi(kq[1].c_str()));
        this->lstXucXac3.push_back(atoi(kq[2].c_str()));
        this->lstTongDiem.push_back(atoi(kq[0].c_str())+atoi(kq[1].c_str())+atoi(kq[2].c_str()));
        if (i == lst.size()-1){
            string kqs = "";
            int tongdiem = atoi(kq[0].c_str())+atoi(kq[1].c_str())+atoi(kq[2].c_str());
            if (tongdiem>=2){
                kqs = dataManager.GetSysString(591);
            }
            else{
                kqs = dataManager.GetSysString(592);
            }
            string s = StringUtils::format("%s (#%s)",dataManager.GetSysString(644).c_str(),it[0].c_str());
            this->txtPhienGanNhat->setString(s);
        }
    }
    

    
    this->drawBieuDoXucXac();
    this->drawBieuDoDiem();
//    this->drawThongKe1();
//    this->drawThongKe2();

}
void LayerThongKeDoDenMini::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerThongKeDoDenMini::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
     if (strcmp("cpc", cmd->c_str()) == 0){
         
    }
    else if (strcmp(EXT_EVENT_HISTORY_ANALYTICS_TX_RESP, cmd->c_str()) == 0){
            SceneManager::getSingleton().hideLoading();
                if (param->GetUtfString("lha") != NULL ){
                    auto strHis = *param->GetUtfString("lha");
                    this->lstXucXac1.clear();
                    this->lstXucXac2.clear();
                    this->lstXucXac3.clear();
                    this->lstTongDiem.clear();
                    auto lst = mUtils::splitString(strHis,';');
                    for (auto i = 0; i < lst.size(); i++){
                        auto it = mUtils::splitString(lst[i],'|');
                        if (it.size() < 2)
                            continue;
                        auto kq = mUtils::splitString(it[1],'-');
                        if (kq.size() < 3)
                            continue;
                        this->lstXucXac1.push_back(atoi(kq[0].c_str()));
                        this->lstXucXac2.push_back(atoi(kq[1].c_str()));
                        this->lstXucXac3.push_back(atoi(kq[2].c_str()));
                        this->lstTongDiem.push_back(atoi(kq[0].c_str())+atoi(kq[1].c_str())+atoi(kq[2].c_str()));
                        if (i == lst.size()-1){
                            string kqs = "";
                            if (atoi(kq[0].c_str())+atoi(kq[1].c_str())+atoi(kq[2].c_str())>=11)
                                kqs = dataManager.GetSysString(671);
                            else
                                kqs = dataManager.GetSysString(709);
                            string s = StringUtils::format("%s (#%s)",dataManager.GetSysString(644).c_str(),it[0].c_str());
                            this->txtPhienGanNhat->setString(s);
                        }
                    }
                    
                    
                    this->drawBieuDoXucXac();
                    this->drawBieuDoDiem();
        }
    }
}
void LayerThongKeDoDenMini::drawBieuDoXucXac(){
    auto orginPos = this->imgBieuDo2->getPosition();

    for (auto i = 0 ; i < this->lstXucXac1.size(); i++){
        
        
        //0 là den, 1 là đỏ
        if (this->lstXucXac1[i]==0){
            auto fromPos = Vec2(orginPos.x+i*this->width,orginPos.y);
            auto dot = Sprite::create("LayerDoDenMini/thongke/quanden.png");
            dot->setAnchorPoint(Vec2(0.5,0.5));
            dot->setPosition(fromPos);
            this->nodeThongKe->addChild(dot);

        }
        else{
            auto fromPos = Vec2(orginPos.x+i*this->width,orginPos.y);
            auto dot = Sprite::create("LayerDoDenMini/thongke/quando.png");
            dot->setAnchorPoint(Vec2(0.5,0.5));
            dot->setPosition(fromPos);
            this->nodeThongKe->addChild(dot);
        }
        

        
        
        
        
    }
    for (auto i = 0 ; i < this->lstXucXac2.size(); i++){
        
        if (this->lstXucXac2[i]==0){
            auto fromPos = Vec2(orginPos.x+i*this->width,orginPos.y + this->height);
            auto dot = Sprite::create("LayerDoDenMini/thongke/quanden.png");
            dot->setAnchorPoint(Vec2(0.5,0.5));
            dot->setPosition(fromPos);
            this->nodeThongKe->addChild(dot);

        }
        else{
            auto fromPos = Vec2(orginPos.x+i*this->width,orginPos.y + this->height);
            auto dot = Sprite::create("LayerDoDenMini/thongke/quando.png");
            dot->setAnchorPoint(Vec2(0.5,0.5));
            dot->setPosition(fromPos);
            this->nodeThongKe->addChild(dot);
        }

    }
    for (auto i = 0 ; i < this->lstXucXac3.size(); i++){
        
        if (this->lstXucXac3[i]==0){
            auto fromPos = Vec2(orginPos.x+i*this->width,orginPos.y+ this->height*2);
            auto dot = Sprite::create("LayerDoDenMini/thongke/quanden.png");
            dot->setAnchorPoint(Vec2(0.5,0.5));
            dot->setPosition(fromPos);
            this->nodeThongKe->addChild(dot);

        }
        else{
            auto fromPos = Vec2(orginPos.x+i*this->width,orginPos.y+ this->height*2);
            auto dot = Sprite::create("LayerDoDenMini/thongke/quando.png");
            dot->setAnchorPoint(Vec2(0.5,0.5));
            dot->setPosition(fromPos);
            this->nodeThongKe->addChild(dot);

        }
        
    }
}
void LayerThongKeDoDenMini::drawBieuDoDiem(){
    auto orginPos = this->imgBieuDo1->getPosition();

    for (auto i = 0 ; i < this->lstTongDiem.size()-1; i++) {
        
        
        
        auto fromPos = Vec2(orginPos.x + i * this->width, orginPos.y + (this->lstTongDiem[i] * (this->height / 1)));
        
        auto toPos = Vec2(orginPos.x + (i + 1) * this->width, orginPos.y + (this->lstTongDiem[i + 1] * (this->height / 1)));
        
        auto drawChart = DrawNode::create();
        drawChart->drawSegment(fromPos, toPos, 1.5, Color4F::GREEN);
        this->nodeThongKe->addChild(drawChart);
        if (this->lstTongDiem[i] >= 2) {
            auto dot = Sprite::create("LayerDoDenMini/thongke/quando.png");
            dot->setPosition(fromPos);
            //dot->setScale(0.7);
            Label* txt = Label::createWithSystemFont(StringUtils::format("%d",this->lstTongDiem[i]), "", 30);
            txt->setAnchorPoint(Vec2(0.5, 0.5));
            dot->addChild(txt);
            txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
            dot->setAnchorPoint(Vec2(0.5, 0.5));
            this->nodeThongKe->addChild(dot);
        }else{
            auto dot = Sprite::create("LayerDoDenMini/thongke/quanden.png");
            dot->setPosition(fromPos);
            Label* txt = Label::createWithSystemFont(StringUtils::format("%d",this->lstTongDiem[i]), "", 30);
            txt->setAnchorPoint(Vec2(0.5, 0.5));
            dot->addChild(txt);
            txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
            txt->setColor(Color3B::WHITE);
            dot->setAnchorPoint(Vec2(0.5, 0.5));
            this->nodeThongKe->addChild(dot);
            //dot->setScale(0.7);
            
        }
        if (this->lstTongDiem[i+1] >= 2) {
            auto dot = Sprite::create("LayerDoDenMini/thongke/quando.png");
            dot->setPosition(toPos);
            //dot->setScale(0.7);
            Label* txt = Label::createWithSystemFont(StringUtils::format("%d",this->lstTongDiem[i+1]), "", 30);
            txt->setAnchorPoint(Vec2(0.5, 0.5));
            dot->addChild(txt);
            txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));

            dot->setAnchorPoint(Vec2(0.5, 0.5));
            this->nodeThongKe->addChild(dot);
        }else{
            auto dot = Sprite::create("LayerDoDenMini/thongke/quanden.png");
            dot->setPosition(toPos);
            Label* txt = Label::createWithSystemFont(StringUtils::format("%d",this->lstTongDiem[i+1]), "", 30);
            txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
            txt->setColor(Color3B::WHITE);
            txt->setAnchorPoint(Vec2(0.5, 0.5));
            dot->addChild(txt);
            dot->setAnchorPoint(Vec2(0.5, 0.5));
            this->nodeThongKe->addChild(dot);
           // dot->setScale(0.7);
            
        }
        
        
        
        
        
    }
}
void LayerThongKeDoDenMini::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    
   }

bool LayerThongKeDoDenMini::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerThongKeDoDenMini::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerThongKeDoDenMini::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerThongKeDoDenMini::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

