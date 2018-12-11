//
//  LayerThongKeTXMini.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/19/17.
//
//

#include "LayerThongKeTXMini.h"
//
//  LayerThongKeTXMini.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/24/17.
//
//

#include "LayerThongKeTXMini.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
LayerThongKeTXMini::LayerThongKeTXMini()
{
    GameServer::getSingleton().addListeners(this);
}


LayerThongKeTXMini::~LayerThongKeTXMini()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerThongKeTXMini::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerThongKeTaiXiuMini.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
    
    
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
        rootNode->setAnchorPoint(Point(0.5, 0.5));
        rootNode->setPosition(Point(visibleSize / 2));
        Size sizeAdd;
       // if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
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
    this->nodeBieuDo =dynamic_cast<Node*>(pnlBg->getChildByName("nodeBieuDo"));
    auto lblT = static_cast<Text*> (this->nodeBieuDo->getChildByName("Text_3_3"));
    lblT->setString(dataManager.GetSysString(659));
    auto lblXX1 = static_cast<Text*> (this->nodeBieuDo->getChildByName("Text_3_0_0"));
    lblXX1->setString(dataManager.GetSysString(852));
    auto lblXX2 = static_cast<Text*> (this->nodeBieuDo->getChildByName("Text_3_1_0"));
    lblXX2->setString(dataManager.GetSysString(853));
    auto lblXX3 = static_cast<Text*> (this->nodeBieuDo->getChildByName("Text_3_2_0"));
    lblXX3->setString(dataManager.GetSysString(854));
    this->nodeThongKe =dynamic_cast<Node*>(pnlBg->getChildByName("nodeThongKe"));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerThongKeTXMini::onBtnClose, this));
    }
    this->btnNext = dynamic_cast<Button*>(pnlBg->getChildByName("btnNext"));
    if(this->btnNext){
        this->btnNext->addClickEventListener(CC_CALLBACK_1(LayerThongKeTXMini::onBtnNext, this));
    }
    this->btnBack = dynamic_cast<Button*>(pnlBg->getChildByName("btnBack"));
    if(this->btnBack){
        this->btnBack->addClickEventListener(CC_CALLBACK_1(LayerThongKeTXMini::onBtnBack, this));
    }
    
    this->txtPhienGanNhat = dynamic_cast<Text*>(this->nodeBieuDo->getChildByName("txtPhienGanNhat"));
    this->point1Org = (ImageView*)this->nodeBieuDo->getChildByName("imgBieuDo2");
    this->point1Org->setVisible(false);
    this->thongke1Org = (ImageView*)this->nodeThongKe->getChildByName("imgBieuDo1");
    this->thongke1Org->setVisible(false);
    this->thongke2Org = (ImageView*)this->nodeThongKe->getChildByName("imgBieuDo2");
    this->thongke2Org->setVisible(false);

    this->txtTai1 = (Text*)this->nodeThongKe->getChildByName("txtTai1");
    this->txtXiu1 = (Text*)this->nodeThongKe->getChildByName("txtXiu1");
    this->txtTai2 = (Text*)this->nodeThongKe->getChildByName("txtTai2");
    this->txtXiu2 = (Text*)this->nodeThongKe->getChildByName("txtXiu2");
    
    this->diemOrg = (ImageView*)this->nodeBieuDo->getChildByName("imgBieuDo1");
    this->diemOrg->setVisible(false);
//    boost::shared_ptr<ISFSObject> params(new SFSObject());
//    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_HISTORY_ANALYTICS_TX_REQ, params));
//    GameServer::getSingleton().Send(request);

    
//    SceneManager::getSingleton().showLoading();
    this->onBtnBack(NULL);
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));

       return true;
}
void LayerThongKeTXMini::onBtnNext(Ref* pSender){
    
    this->btnNext->setOpacity(170);
    this->btnBack->setOpacity(255);
    
    this->nodeBieuDo->setVisible(false);
    this->nodeThongKe->setVisible(true);

}
void LayerThongKeTXMini::onBtnBack(Ref* pSender){
    this->btnNext->setOpacity(255);
    this->btnBack->setOpacity(170);
    this->nodeBieuDo->setVisible(true);
    this->nodeThongKe->setVisible(false);
}
void LayerThongKeTXMini::drawThongKe1(){
    bool iscau = false;
    int vancau = 0;
    int currCau = 0;
    for (int i = 0; i < this->lstTongDiem.size();i++){
        if (i == 0){
            iscau = true;
        }
        else{
            if (this->lstTongDiem[i] >=11 && this->lstTongDiem[i-1] >=11){
                iscau = true;
                vancau++;
            }else if (this->lstTongDiem[i] <11 && this->lstTongDiem[i-1] >=11){
                iscau = false;
                currCau++;
                vancau = 0;
            }
            if (this->lstTongDiem[i] <11 && this->lstTongDiem[i-1] <11){
                iscau = true;
                vancau++;
            }else if (this->lstTongDiem[i] >=11 && this->lstTongDiem[i-1] <11){
                iscau = false;
                currCau++;
                vancau = 0;
            }
            
        }
        
        currCau+=vancau/6;
        if (vancau == 6)
            vancau = 0;
        int posx = this->thongke1Org->getPositionX()+(this->width*(currCau));
        int posy = this->thongke1Org->getPositionY()-(this->height*(vancau%6));
        if (this->lstTongDiem[i] >= 11) {

            auto dot = Sprite::create("LayerTaiXiuMini/thongke/quanden.png");
            dot->setPosition(Vec2(posx,posy));
           // dot->setScale(0.7);
                        Label* txt = Label::createWithSystemFont(StringUtils::format("%d",this->lstTongDiem[i]), "", 30);
                        txt->setAnchorPoint(Vec2(0.5, 0.5));
                        dot->addChild(txt);
                        txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
            dot->setAnchorPoint(Vec2(0.5, 0.5));
            this->nodeThongKe->addChild(dot);
        }else{
            auto dot = Sprite::create("LayerTaiXiuMini/thongke/quantrang.png");
            dot->setPosition(Vec2(posx,posy));
                        Label* txt = Label::createWithSystemFont(StringUtils::format("%d",this->lstTongDiem[i]), "", 30);
                        txt->setAnchorPoint(Vec2(0.5, 0.5));
                        dot->addChild(txt);
                        txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
                        txt->setColor(Color3B::BLACK);
            dot->setAnchorPoint(Vec2(0.5, 0.5));
            this->nodeThongKe->addChild(dot);
            //dot->setScale(0.7);
            
        }
    }

}
void LayerThongKeTXMini::drawThongKe2(){
    int tai =0;
    int xiu = 0;
    for (int i = 0; i < this->lstTongDiem.size();i++){
        int posx = this->thongke2Org->getPositionX()+(this->width*(i/6));
        int posy = this->thongke2Org->getPositionY()-(this->height*(i%6));
        if (this->lstTongDiem[i] >= 11) {
            tai++;
            auto dot = Sprite::create("LayerTaiXiuMini/thongke/quanden.png");
            dot->setPosition(Vec2(posx,posy));
            //dot->setScale(0.7);
//            Label* txt = Label::createWithTTF(StringUtils::format("%d",this->lstTongDiem[i]), "__Roboto-Bold.ttf", 30);
//            txt->setAnchorPoint(Vec2(0.5, 0.5));
//            dot->addChild(txt);
//            txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
            dot->setAnchorPoint(Vec2(0.5, 0.5));
            this->nodeThongKe->addChild(dot);
        }else{
            xiu++;
            auto dot = Sprite::create("LayerTaiXiuMini/thongke/quantrang.png");
            dot->setPosition(Vec2(posx,posy));
//            Label* txt = Label::createWithTTF(StringUtils::format("%d",this->lstTongDiem[i]), "__Roboto-Bold.ttf", 30);
//            txt->setAnchorPoint(Vec2(0.5, 0.5));
//            dot->addChild(txt);
//            txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
//            txt->setColor(Color3B::BLACK);
            dot->setAnchorPoint(Vec2(0.5, 0.5));
            this->nodeThongKe->addChild(dot);
            //dot->setScale(0.7);
            
        }
    }
    this->txtTai1->setString(StringUtils::format("%s: %d",dataManager.GetSysString(671).c_str(),tai));
    this->txtXiu1->setString(StringUtils::format("%s: %d",dataManager.GetSysString(709).c_str(),xiu));
    this->txtTai2->setString(StringUtils::format("%s: %d",dataManager.GetSysString(671).c_str(),tai));
    this->txtXiu2->setString(StringUtils::format("%s: %d",dataManager.GetSysString(709).c_str(),xiu));
}
void LayerThongKeTXMini::loadThongKe(string lhs){
    this->lstXucXac1.clear();
    this->lstXucXac2.clear();
    this->lstXucXac3.clear();
    this->lstTongDiem.clear();
  //  lhs = "34|6-6-2;35|6-6-2;36|6-5-2;37|3-5-4;38|6-4-5;39|6-6-2;40|6-6-2;41|6-6-2;42|6-6-2;43|5-4-5;44|2-4-6;45|2-3-4;46|1-1-5;47|6-5-2;48|6-3-5;49|5-5-4;50|6-4-4;51|4-4-2;52|5-1-1;53|6-6-2;";

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
            if (tongdiem>=11){
                kqs = dataManager.GetSysString(671);
            }
            else{
                kqs = dataManager.GetSysString(709);
            }
            string s = StringUtils::format("%s (#%s)     %s: %d (%s)",dataManager.GetSysString(644).c_str(),it[0].c_str(),kqs.c_str(),tongdiem,it[1].c_str());
            this->txtPhienGanNhat->setString(s);
        }
    }
    

    
    this->drawBieuDoXucXac();
    this->drawBieuDoDiem();
    this->drawThongKe1();
    this->drawThongKe2();

}
void LayerThongKeTXMini::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerThongKeTXMini::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
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
                            string s = StringUtils::format("%s (#%s)%s: (%s)",dataManager.GetSysString(644).c_str(),it[0].c_str(),kqs.c_str(),it[1].c_str());
                            this->txtPhienGanNhat->setString(s);
                        }
                    }
                    
                    
                    this->drawBieuDoXucXac();
                    this->drawBieuDoDiem();
        }
    }
}
void LayerThongKeTXMini::drawBieuDoXucXac(){
    for (auto i = 0 ; i < this->lstXucXac1.size()-1; i++){
        
        auto orginPos = this->point1Org->getPosition();
        
        
        auto fromPos = Vec2(orginPos.x+i*this->width,orginPos.y+(this->lstXucXac1[i]*this->height));
        auto dot = Sprite::create("LayerTaiXiuMini/thongke/quanvang.png");
        dot->setAnchorPoint(Vec2(0.5,0.5));
        dot->setPosition(fromPos);
        
        auto toPos = Vec2(orginPos.x+(i+1)*this->width,orginPos.y+(this->lstXucXac1[i+1]*this->height));
        auto dot1 = Sprite::create("LayerTaiXiuMini/thongke/quanvang.png");
        dot1->setPosition(toPos);
        
        auto drawChart = DrawNode::create();
        drawChart->drawSegment(fromPos, toPos,1.5,Color4F::RED);
        this->nodeBieuDo->addChild(drawChart);
        dot->setAnchorPoint(Vec2(0.5,0.5));
        dot1->setAnchorPoint(Vec2(0.5,0.5));
        
        this->nodeBieuDo->addChild(dot1);
        this->nodeBieuDo->addChild(dot);
        
    }
    for (auto i = 0 ; i < this->lstXucXac2.size()-1; i++){
        
        auto orginPos = this->point1Org->getPosition();
        
        
        auto fromPos = Vec2(orginPos.x+i*this->width,orginPos.y+(this->lstXucXac2[i]*this->height));
        auto dot = Sprite::create("LayerTaiXiuMini/thongke/quanxanh.png");
        dot->setPosition(fromPos);
        
        auto toPos = Vec2(orginPos.x+(i+1)*this->width,orginPos.y+(this->lstXucXac2[i+1]*this->height));
        auto dot1 = Sprite::create("LayerTaiXiuMini/thongke/quanxanh.png");
        dot1->setPosition(toPos);
        
        auto drawChart = DrawNode::create();
        drawChart->drawSegment(fromPos, toPos,1.5,Color4F::GREEN);
        this->nodeBieuDo->addChild(drawChart);
        dot->setAnchorPoint(Vec2(0.5,0.5));
        dot1->setAnchorPoint(Vec2(0.5,0.5));
        this->nodeBieuDo->addChild(dot1);
        this->nodeBieuDo->addChild(dot);
    }
    for (auto i = 0 ; i < this->lstXucXac3.size()-1; i++){
        
        auto orginPos = this->point1Org->getPosition();
        
        
        auto fromPos = Vec2(orginPos.x+i*this->width,orginPos.y+(this->lstXucXac3[i]*this->height));
        auto dot = Sprite::create("LayerTaiXiuMini/thongke/quando.png");
        dot->setPosition(fromPos);
        
        auto toPos = Vec2(orginPos.x+(i+1)*this->width,orginPos.y+(this->lstXucXac3[i+1]*this->height));
        auto dot1 = Sprite::create("LayerTaiXiuMini/thongke/quando.png");
        dot1->setPosition(toPos);
        
        auto drawChart = DrawNode::create();
        drawChart->drawSegment(fromPos, toPos,1.5,Color4F::RED);
        this->nodeBieuDo->addChild(drawChart);
        dot->setAnchorPoint(Vec2(0.5,0.5));
        dot1->setAnchorPoint(Vec2(0.5,0.5));
        this->nodeBieuDo->addChild(dot1);
        this->nodeBieuDo->addChild(dot);
        
        
    }
}
void LayerThongKeTXMini::drawBieuDoDiem(){
    for (auto i = 0 ; i < this->lstTongDiem.size()-1; i++) {
        
        auto orginPos = this->diemOrg->getPosition();
        
        
        auto fromPos = Vec2(orginPos.x + i * this->width, orginPos.y + (this->lstTongDiem[i] * (this->height / 3)));
        
        auto toPos = Vec2(orginPos.x + (i + 1) * this->width, orginPos.y + (this->lstTongDiem[i + 1] * (this->height / 3)));
        
        auto drawChart = DrawNode::create();
        drawChart->drawSegment(fromPos, toPos, 1.5, Color4F::GREEN);
        this->nodeBieuDo->addChild(drawChart);
        if (this->lstTongDiem[i] >= 11) {
            auto dot = Sprite::create("LayerTaiXiuMini/thongke/quanden.png");
            dot->setPosition(fromPos);
            //dot->setScale(0.7);
            Label* txt = Label::createWithSystemFont(StringUtils::format("%d",this->lstTongDiem[i]), "", 30);
            txt->setAnchorPoint(Vec2(0.5, 0.5));
            dot->addChild(txt);
            txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
            dot->setAnchorPoint(Vec2(0.5, 0.5));
            this->nodeBieuDo->addChild(dot);
        }else{
            auto dot = Sprite::create("LayerTaiXiuMini/thongke/quantrang.png");
            dot->setPosition(fromPos);
            Label* txt = Label::createWithSystemFont(StringUtils::format("%d",this->lstTongDiem[i]), "", 30);
            txt->setAnchorPoint(Vec2(0.5, 0.5));
            dot->addChild(txt);
            txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
            txt->setColor(Color3B::BLACK);
            dot->setAnchorPoint(Vec2(0.5, 0.5));
            this->nodeBieuDo->addChild(dot);
            //dot->setScale(0.7);
            
        }
        if (this->lstTongDiem[i+1] >= 11) {
            auto dot = Sprite::create("LayerTaiXiuMini/thongke/quanden.png");
            dot->setPosition(toPos);
            //dot->setScale(0.7);
            Label* txt = Label::createWithSystemFont(StringUtils::format("%d",this->lstTongDiem[i+1]), "", 30);
            txt->setAnchorPoint(Vec2(0.5, 0.5));
            dot->addChild(txt);
            txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));

            dot->setAnchorPoint(Vec2(0.5, 0.5));
            this->nodeBieuDo->addChild(dot);
        }else{
            auto dot = Sprite::create("LayerTaiXiuMini/thongke/quantrang.png");
            dot->setPosition(toPos);
            Label* txt = Label::createWithSystemFont(StringUtils::format("%d",this->lstTongDiem[i+1]), "", 30);
            txt->setPosition(Vec2(dot->getContentSize().width/2, dot->getContentSize().height/2));
            txt->setColor(Color3B::BLACK);
            txt->setAnchorPoint(Vec2(0.5, 0.5));
            dot->addChild(txt);
            dot->setAnchorPoint(Vec2(0.5, 0.5));
            this->nodeBieuDo->addChild(dot);
           // dot->setScale(0.7);
            
        }
        
        
        
        
        
    }
}
void LayerThongKeTXMini::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    
   }

bool LayerThongKeTXMini::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerThongKeTXMini::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerThongKeTXMini::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerThongKeTXMini::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

