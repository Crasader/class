//
//  ThangTrangMauBinh.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/2/17.
//
//
#include "ThangTrangMauBinh.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MauBinhLayerXepBai.h"
#include "BaiMauBinh.h"
ThangTrangMauBinh::ThangTrangMauBinh()
{
}


ThangTrangMauBinh::~ThangTrangMauBinh()
{
    
}

bool ThangTrangMauBinh::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size sizeAdd = ChanUtils::getSizePos();
    
    auto rootNode = CSLoader::getInstance()->createNode("ThangTrangMauBinh.csb");
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    
    auto pnlBg = (Layout*)rootNode->getChildByName("pnlBG");
    pnlBg->setPositionY(sizeAdd.height);
    
    ImageView* imgBot = (ImageView*)pnlBg->getChildByName("imgBot");
    ImageView* bg1 = (ImageView*)pnlBg->getChildByName("bg1");
    ImageView* bg2 = (ImageView*)pnlBg->getChildByName("bg2");
    this->imgResult = (ImageView*)pnlBg->getChildByName("imgResult");
    
    imgBot->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.5),FadeOut::create(0.15),NULL)));
    bg1->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1),FadeIn::create(0.15),NULL)));
    bg2->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1),FadeOut::create(0.15),NULL)));
    
    auto bgName = (ImageView*)pnlBg->getChildByName("bgName");
    bgName->setLocalZOrder(9);
    this->txtName = (Text*)bgName->getChildByName("txtName");
    
    this->pnlType1 = (Layout*)pnlBg->getChildByName("pnlType1");
    this->pnlType1->setVisible(false);
    this->pnlType2 = (Layout*)pnlBg->getChildByName("pnlType2");
    this->pnlType2->setVisible(false);
    
    for (int i = 0 ; i < 13; i++) {
        Sprite* spr1 = (Sprite*)this->pnlType1->getChildByName(StringUtils::format("spr%d",i+1));
        //spr1->setLocalZOrder(10);
        //spr1->setScale(1);
        this->lstSpr1.push_back(spr1);
       // spr1->setLocalZOrder(30/ ((i)/5+1)  + (i)%5 );

        Sprite* spr2 = (Sprite*)this->pnlType2->getChildByName(StringUtils::format("spr%d",i+1));
        this->lstSpr2.push_back(spr2);
        //spr2->setLocalZOrder(10);
        //spr2->setScale(1);
      //  spr2->setLocalZOrder(30/ ((i)/5+1)  + (i)%5 );
    }
    this->setScale(0);
    this->runAction(ScaleTo::create(0.1, 1.3));
    
    return true;
}
void ThangTrangMauBinh::setName(string name){
    
    boost::shared_ptr<User> userInfo = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(name);
    
    if (userInfo == NULL
     || userInfo->GetVariable("adn") == NULL){
        return;
    }
    // account Name- moc3-change aN=>adn
    boost::shared_ptr<string> aN = userInfo->GetVariable("adn")->GetStringValue();
    if (aN != NULL){
        if (strcmp(aN->c_str(), "") == 0)
            this->txtName->setString(formatUserName(name, 11));
        else
            this->txtName->setString(formatUserName(*aN, 11));
    }
    
}
void ThangTrangMauBinh::setBoBai(string _lstCard){
    this->lstCard = _lstCard;
}
string ThangTrangMauBinh::getSrcCard(string cardtype){
    string url = "card_";
    auto lstchi = mUtils::splitString(cardtype,'_');
    if (lstchi.size()<2)
        return "";
    int num = atoi(lstchi[0].c_str());
    int type = atoi(lstchi[1].c_str());
    switch (num) {
            
        case 11:
            url += "j";
            break;
        case 12:
            url += "q";
            break;
        case 13:
            url += "k";
            break;
        case 14:
            url += "a";
            break;
        default:
            url += StringUtils::format("%d",num);
            break;
    }
    
    url += "_";
    
    switch (type) {
        case 1:
            url += "h.png"; // cơ
            break;
        case 2:
            url += "d.png"; // rô
            break;
        case 3:
            url += "c.png"; // tép
            break;
        case 4:
            url += "s.png"; // bích
            break;
        default:
            break;
    }
    return url;
    
}
void ThangTrangMauBinh::setThangTrangType(int type){
    string pathImg = "";
    switch (type){
        case SPECIAL_CARD_TYPE_SANH_RONG_CUON:{
            pathImg = "mb_rong_cuon.png";
            
            this->pnlType1->setVisible(true);
            auto lst = mUtils::splitString(this->lstCard,';');
            for (int i = 0; i < this->lstSpr1.size(); i ++){
                if (i <=4){
                    this->lstSpr1[i+4-i*2]->setSpriteFrame(this->getSrcCard(lst[i]));
                }
                if (i > 4 && i  <=9){
                    this->lstSpr1[i]->setSpriteFrame(this->getSrcCard(lst[i]));
                }
                if (i > 9 && i  <=12){
                    this->lstSpr1[12-(i-10)]->setSpriteFrame(this->getSrcCard(lst[i]));
                }
                
                //this->lstSpr1[i]->setScale(1.1);
            }
            
            break;
        }
        case SPECIAL_CARD_TYPE_SANH_RONG:{
            pathImg = "mb_sanh_rong.png";
            this->pnlType1->setVisible(true);
            auto lst = mUtils::splitString(this->lstCard,';');
            for (int i = 0; i < this->lstSpr1.size(); i ++){
                if (i <=4){
                    this->lstSpr1[i+4-i*2]->setSpriteFrame(this->getSrcCard(lst[i]));
                }
                if (i > 4 && i  <=9){
                    this->lstSpr1[i]->setSpriteFrame(this->getSrcCard(lst[i]));
                }
                if (i > 9 && i  <=12){
                    this->lstSpr1[12-(i-10)]->setSpriteFrame(this->getSrcCard(lst[i]));
                }
            }
            break;
        }
        case SPECIAL_CARD_TYPE_LUC_PHE_BON:{
            pathImg = "mb_luc_phe_bon.png";
            this->pnlType2->setVisible(true);
            auto lst = mUtils::splitString(this->lstCard,';');
            for (int i = 0; i < this->lstSpr2.size(); i ++){
                this->lstSpr2[i]->setSpriteFrame(this->getSrcCard(lst[i]));
                //this->lstSpr2[i]->setScale(1.1);

            }
            this->lstSpr2[12]->setColor(Color3B(150, 150, 150));
            break;
        }
        case SPECIAL_5DOI_1SAM:{
            pathImg = "mb_5_doi_1_sam.png";
            this->pnlType2->setVisible(true);
            auto lst = mUtils::splitString(this->lstCard,';');
            for (int i = 0; i < this->lstSpr2.size(); i ++){
                this->lstSpr2[i]->setSpriteFrame(this->getSrcCard(lst[i]));
                //this->lstSpr2[i]->setScale(1.1);

            }
            break;
        }
        case SPECIAL_CARD_TYPE_3_CAI_SANH:{
            pathImg = "mb_3_sanh.png";
            this->pnlType1->setVisible(true);
            auto lst = mUtils::splitString(this->lstCard,';');
            for (int i = 0; i < this->lstSpr1.size(); i ++){
                this->lstSpr1[i]->setSpriteFrame(this->getSrcCard(lst[i]));
                //this->lstSpr1[i]->setScale(1.1);

            }
            break;
        }
        case SPECIAL_CARD_TYPE_3_CAI_THUNG:{
            pathImg = "mb_3_thung.png";
            this->pnlType1->setVisible(true);
            auto lst = mUtils::splitString(this->lstCard,';');
            for (int i = 0; i < this->lstSpr1.size(); i ++){
                this->lstSpr1[i]->setSpriteFrame(this->getSrcCard(lst[i]));
                //this->lstSpr1[i]->setScale(1.1);

            }
            break;
        }
    }
    //layerWidget->removeChildByTag(24, true);
    string fullPath = StringUtils::format("icon_result/%s%s",SceneManager::getSingleton().getFolderLang().c_str(),pathImg.c_str());
    Sprite* spriteResult = Sprite::create(fullPath);
    spriteResult->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(spriteResult);
    spriteResult->setLocalZOrder(15);
    spriteResult->setPosition(this->imgResult->getPosition());
}
void ThangTrangMauBinh::showBoBai(){
    
}
