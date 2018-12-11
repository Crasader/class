//
//  MauBinhLayerXepBai.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 4/11/17.
//
//

#include "MauBinhLayerXepBai.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PhomMessDef.h"
#include "LayerBoBaiMauBinh.h"
#include "LayerGameMauBinh.h"
USING_NS_CC;
using namespace cocos2d::ui;

MauBinhLayerXepBai::MauBinhLayerXepBai()
{

}


MauBinhLayerXepBai::~MauBinhLayerXepBai()
{

}

bool MauBinhLayerXepBai::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerXepBaiMauBinh.csb");

	this->pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	this->pnlBg->setSwallowTouches(false);

	Layout* Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
	Panel_2->setSwallowTouches(false);

	rootNode->setAnchorPoint(Point(0.5, 0.5));
	Size sizeAdd;
	if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
		sizeAdd = ChanUtils::getSizePos();

	}
	else{
		sizeAdd = Size(0, 0);
	}
	rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - sizeAdd.height));    ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);


	//Chi 1
	ImageView* chi1_1 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi1_1"));
	ImageView* chi1_2 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi1_2"));
	ImageView* chi1_3 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi1_3"));
	ImageView* chi1_4 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi1_4"));
	ImageView* chi1_5 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi1_5"));

	this->lstChi1.push_back(chi1_1);
	this->lstChi1.push_back(chi1_2);
	this->lstChi1.push_back(chi1_3);
	this->lstChi1.push_back(chi1_4);
	this->lstChi1.push_back(chi1_5);
    
    this->lstPoint.push_back(chi1_1->getPosition());
    this->lstPoint.push_back(chi1_2->getPosition());
    this->lstPoint.push_back(chi1_3->getPosition());
    this->lstPoint.push_back(chi1_4->getPosition());
    this->lstPoint.push_back(chi1_5->getPosition());

	//Chi 2
	ImageView* chi2_1 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi2_1"));
	ImageView* chi2_2 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi2_2"));
	ImageView* chi2_3 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi2_3"));
	ImageView* chi2_4 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi2_4"));
	ImageView* chi2_5 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi2_5"));

	this->lstChi2.push_back(chi2_1);
	this->lstChi2.push_back(chi2_2);
	this->lstChi2.push_back(chi2_3);
	this->lstChi2.push_back(chi2_4);
	this->lstChi2.push_back(chi2_5);

    this->lstPoint.push_back(chi2_1->getPosition());
    this->lstPoint.push_back(chi2_2->getPosition());
    this->lstPoint.push_back(chi2_3->getPosition());
    this->lstPoint.push_back(chi2_4->getPosition());
    this->lstPoint.push_back(chi2_5->getPosition());

	//Chi 3
	ImageView* chi3_1 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi3_1"));
	ImageView* chi3_2 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi3_2"));
	ImageView* chi3_3 = static_cast<ImageView*>(this->pnlBg->getChildByName("chi3_3"));
	this->lstChi3.push_back(chi3_1);
	this->lstChi3.push_back(chi3_2);
	this->lstChi3.push_back(chi3_3);
    
    this->lstPoint.push_back(chi3_1->getPosition());
    this->lstPoint.push_back(chi3_2->getPosition());
    this->lstPoint.push_back(chi3_3->getPosition());

	this->fromCard = NULL;
	this->toCard = NULL;
	//tong chi

	this->bgChi1 = static_cast<ImageView*>(this->pnlBg->getChildByName("bgChi1"));
	this->bgChi1->setLocalZOrder(20);
	this->bgChi2 = static_cast<ImageView*>(this->pnlBg->getChildByName("bgChi2"));
	this->bgChi2->setLocalZOrder(20);
	this->bgChi3 = static_cast<ImageView*>(this->pnlBg->getChildByName("bgChi3"));
	this->bgChi3->setLocalZOrder(20);

	this->txtChi1 = static_cast<Text*>(this->pnlBg->getChildByName("txtChi1"));
	this->txtChi1->setLocalZOrder(20);
	this->txtChi2 = static_cast<Text*>(this->pnlBg->getChildByName("txtChi2"));
	this->txtChi2->setLocalZOrder(20);
	this->txtChi3 = static_cast<Text*>(this->pnlBg->getChildByName("txtChi3"));
	this->txtChi3->setLocalZOrder(20);

	this->imgBinhLung = static_cast<ImageView*>(this->pnlBg->getChildByName("imgBinhLung"));
	this->imgBinhLung->setVisible(false);
    this->imgBinhLung->loadTexture(StringUtils::format("MauBinh/%smb_binh_lung.png",SceneManager::getSingleton().getFolderLang().c_str()));

	//this->loadCard("2_1-2_3-4_1-9_4-11_1;11_2-12_3-13_4-11_3-10_2;9_1-2_4-9_2");


	//this->displayCardValue();

	Button* btnXepXong = (Button*)this->pnlBg->getChildByName("btnXepXong");
	if (btnXepXong){
		btnXepXong->addClickEventListener(CC_CALLBACK_1(MauBinhLayerXepBai::onBtnXepXong, this));
        btnXepXong->loadTextures(StringUtils::format("MauBinh/%sxep-xong.png",SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("MauBinh/%sxep-xong.png",SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("MauBinh/%sxep-xong.png",SceneManager::getSingleton().getFolderLang().c_str()));
	}

    auto txtTimer_0 = static_cast<Text*>(this->pnlBg->getChildByName("txtTimer_0"));
    txtTimer_0->setString(dataManager.GetSysString(918));
    
	this->txtTimer = (Text*)this->pnlBg->getChildByName("txtTimer");
	this->txtTimer->setVisible(false);
    
//    this->bangcuoc = BangCuocMauBinh::create();
//    this->addChild(this->bangcuoc);
//    this->bangcuoc->setPosition(pnlBangCuoc->getPosition());


	return true;
}
void MauBinhLayerXepBai::setOrgPoint(Vec2 point){
    this->orgPoint = point;
}
void MauBinhLayerXepBai::setPointDone(vector<Point> lstPoint){
    this->lstPointDone = lstPoint;
}
void MauBinhLayerXepBai::setSkewXDone(vector<float> lstSkew){
    this->lstSkewX = lstSkew;
}
void MauBinhLayerXepBai::setSkewYDone(vector<float> lstSkew){
    this->lstSkewY = lstSkew;
}
vector<Point> MauBinhLayerXepBai::getPositionCard(){
    return this->lstPoint;
}
void MauBinhLayerXepBai::onBtnXepXong(Ref* pSender){
	LayerBoBaiMauBinh* parent = (LayerBoBaiMauBinh*)(this->getParent());
	if (parent){
		parent->showMyListCard();
        parent->setLocalZOrder(110);
        parent->setIsXepBai(false);
        LayerGameMauBinh* game = (LayerGameMauBinh*)parent->getParent();
        if (game){
            game->reloadCuoc(this->cuoc1,this->cuoc2,this->cuoc3);
        }

	}

//    for (int i = 0 ; i < this->tongChi.size(); i++)
//    {
//        this->tongChi[i]->runAction(MoveTo::create(0.2, Vec2(this->lstPoint[i].x,this->lstPoint[i].y)));
//        this->tongChi[i]->runAction(ScaleTo::create(0.2, 0.7));
//        this->tongChi[i]->setRotationSkewX(this->lstSkewX[i]);
//        this->tongChi[i]->setRotationSkewY(this->lstSkewY[i]);
//
//    }
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutInt("act", 1);
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_FINISH_CARD_LINK_REQ, params, lastRoom));
	GameServer::getSingleton().Send(request);
    this->runAction(Sequence::create(ScaleTo::create(0.2,0),CallFunc::create([this](){ this->removeFromParentAndCleanup(true);}), NULL));

}
void MauBinhLayerXepBai::setListCard(string listcard){
	this->loadCard(listcard);
}
void MauBinhLayerXepBai::setListCardID(string listid){
	this->lstcardid = listid;
}
void MauBinhLayerXepBai::loadCard(string listcard){
	auto lstchi = mUtils::splitString(listcard, ';');
	if (lstchi.size() < 3)
		return;

	//load chi 1
	auto lstchi1 = mUtils::splitString(lstchi[0], '-');
	auto lstid = mUtils::splitString(this->lstcardid, ',');
	for (int i = 0; i < lstchi1.size(); i++){
		MauBinhCard* sf = new MauBinhCard(this->getSrcCard(lstchi1[i]).c_str());
		sf->autorelease();
		sf->setValue(lstchi1[i]);
		sf->setOrginPos(this->lstChi1[i]->getPosition());
		sf->setType(atoi(StringUtils::format("1%d", i).c_str()));
		sf->setAnchorPoint(this->lstChi1[i]->getAnchorPoint());
		sf->setPosition(this->lstChi1[i]->getPosition());
		sf->SetID(atoi(lstid[i].c_str()));
		sf->setLocalZOrder(10);
		sf->setContentSize(Size(214, 280));
        //sf->setScaleX(0.995);

		this->pnlBg->addChild(sf);
		this->Chi1.push_back(sf);
		this->tongChi.push_back(sf);
	}

	//load chi 2
	auto lstchi2 = mUtils::splitString(lstchi[1], '-');
	for (int i = 0; i < lstchi2.size(); i++){
		MauBinhCard* sf = new MauBinhCard(this->getSrcCard(lstchi2[i]).c_str());
		sf->autorelease();
		sf->setValue(lstchi2[i]);
		sf->setOrginPos(this->lstChi2[i]->getPosition());
		sf->setType(atoi(StringUtils::format("2%d", i).c_str()));
		sf->setAnchorPoint(this->lstChi2[i]->getAnchorPoint());
		sf->setPosition(this->lstChi2[i]->getPosition());
		sf->setContentSize(Size(214, 280));
		sf->setLocalZOrder(10);
       // sf->setScaleX(0.995);

		sf->SetID(atoi(lstid[i + 5].c_str()));

		this->pnlBg->addChild(sf);
		this->Chi2.push_back(sf);
		this->tongChi.push_back(sf);

	}

	//load chi 3
	auto lstchi3 = mUtils::splitString(lstchi[2], '-');
	for (int i = 0; i < lstchi3.size(); i++){

		MauBinhCard* sf = new MauBinhCard(this->getSrcCard(lstchi3[i]).c_str());
		sf->autorelease();
		sf->setValue(lstchi3[i]);
		sf->setOrginPos(this->lstChi3[i]->getPosition());
		sf->setType(atoi(StringUtils::format("3%d", i).c_str()));
		sf->setAnchorPoint(this->lstChi3[i]->getAnchorPoint());
		sf->setPosition(this->lstChi3[i]->getPosition());
		sf->setContentSize(Size(214, 280));
		sf->setLocalZOrder(10);
       // sf->setScaleX(0.995);
		this->pnlBg->addChild(sf);
		sf->SetID(atoi(lstid[i + 10].c_str()));
		this->Chi3.push_back(sf);
		this->tongChi.push_back(sf);
	}

	this->displayCardValue();
}
string MauBinhLayerXepBai::getSrcCard(string cardtype){
	string url = "card_";
	auto lstchi = mUtils::splitString(cardtype, '_');
	if (lstchi.size() < 2)
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
		url += "1";
		break;
	default:
		url += StringUtils::format("%d", num);
		break;
	}

	url += "_";

	switch (type) {
	case 1:
		url += "h_big.png"; // cơ
		break;
	case 2:
		url += "d_big.png"; // rô
		break;
	case 3:
		url += "c_big.png"; // tép
		break;
	case 4:
		url += "s_big.png"; // bích
		break;
	default:
		break;
	}
	return url;

}
bool MauBinhLayerXepBai::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	auto locationInNode = pTouch->getLocation();
	//auto s = target.getContentSize();
	//Rect(this->menu->getPosition().x, this->menu->getPosition().y, 143,149);
	Size sizeAdd;

	sizeAdd = ChanUtils::getSizePos();


	//listener->setSwallowTouches(false);
	for (int i = 0; i < this->tongChi.size(); i++) {
		auto rectNode = this->tongChi[i]->getBoundingBox();
		auto rect = Rect(rectNode.origin.x + sizeAdd.width, rectNode.origin.y + sizeAdd.height, rectNode.size.width, rectNode.size.height);
		if (rect.containsPoint(locationInNode)) {
			this->fromCard = this->tongChi[i];
			this->fromCard->setLocalZOrder(15);
			return true;
		}
	}
	return true;
}

void MauBinhLayerXepBai::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
	//auto target = this->menu
	Size sizeAdd;

	sizeAdd = ChanUtils::getSizePos();
	auto delta = pTouch->getDelta();
	//  if (target.x ==0 || target.x == WIDTH_DESIGN-143 || target.y ==0 || target.y == HEIGHT_DESIGN-149)
	//     return;
    if (this->fromCard){
	auto x = this->fromCard->getPositionX() + delta.x;
	auto y = this->fromCard->getPositionY() + delta.y;

	this->fromCard->setPosition(Vec2(x, y));



	for (int i = 0; i < this->tongChi.size(); i++) {
		if (this->tongChi[i]->getType() != this->fromCard->getType()){
			auto rectNode = this->tongChi[i]->getBoundingBox();
			auto rect = Rect(rectNode.origin.x + sizeAdd.width, rectNode.origin.y + sizeAdd.height, rectNode.size.width, rectNode.size.height);
			if (rect.containsPoint(Vec2(x, y))) {
				this->toCard = this->tongChi[i];
				this->toCard->setLocalZOrder(14);
			}
		}
	}
    }
	//if (abs(delta.x) > 10 || abs(delta.y) > 10)
	//   this->isTouchMove =true;
	//listener->setSwallowTouches(true);
}
void MauBinhLayerXepBai::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
	if (this->fromCard != NULL && this->toCard != NULL){
		this->swapCard(this->fromCard, this->toCard);
	}
	if (this->fromCard != NULL && this->toCard == NULL){
		this->fromCard->runAction(MoveTo::create(0.1, this->fromCard->getOrginPos()));
		this->fromCard->setLocalZOrder(10);
	}
}
string MauBinhLayerXepBai::getListID(){
	string lst = "";
	for (int i = 0; i < this->tongChi.size(); i++) {

		if (i != 0 && i != this->tongChi.size())
			lst.append(",");
		string id = StringUtils::format("%d", this->tongChi[i]->GetID());
		lst.append(id);
	}

	return lst;
}
void MauBinhLayerXepBai::swapCard(MauBinhCard* _from, MauBinhCard* _to){

	Vec2 fromPos = this->toCard->getOrginPos();
	Vec2 toPos = this->fromCard->getOrginPos();

	this->fromCard->runAction(MoveTo::create(0.5, fromPos));
	this->toCard->runAction(MoveTo::create(0.5, toPos));

	this->fromCard->setOrginPos(fromPos);
	this->toCard->setOrginPos(toPos);

	this->fromCard->setLocalZOrder(10);
	this->toCard->setLocalZOrder(10);

	int from = 0;
	int to = 0;

	int typeFrom = this->fromCard->getType();
	int typeTo = this->toCard->getType();

	for (int i = 0; i < this->tongChi.size(); i++) {
		if (typeFrom == this->tongChi[i]->getType())
			from = i;
		if (typeTo == this->tongChi[i]->getType())
			to = i;
	}
	std::swap(this->tongChi[from], tongChi[to]);

	this->fromCard->setType(typeTo);
	this->toCard->setType(typeFrom);

	this->fromCard = NULL;
	this->toCard = NULL;

	this->displayCardValue();

	LayerBoBaiMauBinh* parent = (LayerBoBaiMauBinh*)(this->getParent());
	if (parent){
		parent->setMyListIDCard(this->getListID());
	}

	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString(EXT_FIELD_LIST_CARD, this->getListID());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_LIST_CARD_LINK, params, lastRoom));
	GameServer::getSingleton().Send(request);
   //this->scheduleOnce(schedule_selector(MauBinhLayerXepBai::reloadCard),0.5);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(MauBinhLayerXepBai::reloadCard), this, 0.5, 1, 0.5, false);

}
void MauBinhLayerXepBai::reloadCard(float dt){
    for (int i = 0 ; i < this->tongChi.size() ; i++){
        if (this->fromCard == this->tongChi[i] ||  this->toCard == this->tongChi[i]) continue;
        this->tongChi[i]->stopAllActions();
        this->tongChi[i]->runAction(MoveTo::create(0.1, this->tongChi[i]->getOrginPos()));
    }
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(MauBinhLayerXepBai::reloadCard),this);

}
string MauBinhLayerXepBai::linkCard(int type){
	switch (type){
	case MB_MAU_THAU:
		return dataManager.GetSysString(649);
	case MB_DOI:
		return dataManager.GetSysString(586);
	case MB_SAM_CO:
		return dataManager.GetSysString(647);
	case MB_SANH:
		return dataManager.GetSysString(648);
	case MB_THUNG:
		return dataManager.GetSysString(675);
	case MB_THU:
		return dataManager.GetSysString(674);
	case MB_CU_LU:
		return dataManager.GetSysString(557);
	case MB_TU_QUY:
		return dataManager.GetSysString(676);
	case MB_THUNG_PHA_SANH:
		return dataManager.GetSysString(677);
	default:
		return "Invalid";
	}
}
void MauBinhLayerXepBai::displayCardValue(){
	for (int i = 0; i < this->tongChi.size(); i++)
		this->tongChi[i]->reset();
	if (this->tongChi.size() != 13)
		return;
	string chi1 = this->convertCardToCheck(this->tongChi[0]->getValue()) + " " + this->convertCardToCheck(this->tongChi[1]->getValue()) + " " + this->convertCardToCheck(this->tongChi[2]->getValue()) + " " + this->convertCardToCheck(this->tongChi[3]->getValue()) + " " + this->convertCardToCheck(this->tongChi[4]->getValue());

	int linkChi1 = p.analyze(chi1);
	this->txtChi1->setString(this->linkCard(linkChi1));
	this->displayCardWasLinked(linkChi1, 1);

	string chi2 = this->convertCardToCheck(this->tongChi[5]->getValue()) + " " + this->convertCardToCheck(this->tongChi[6]->getValue()) + " " + this->convertCardToCheck(this->tongChi[7]->getValue()) + " " + this->convertCardToCheck(this->tongChi[8]->getValue()) + " " + this->convertCardToCheck(this->tongChi[9]->getValue());

	int linkChi2 = p.analyze(chi2);
	this->txtChi2->setString(this->linkCard(linkChi2));
	this->displayCardWasLinked(linkChi2, 2);

	string chi3 = this->convertCardToCheck(this->tongChi[10]->getValue()) + " " + this->convertCardToCheck(this->tongChi[11]->getValue()) + " " + this->convertCardToCheck(this->tongChi[12]->getValue());

	int linkChi3 = this->checkChi3La();
	this->txtChi3->setString(this->linkCard(linkChi3));
	this->displayCardWasLinked(linkChi3, 3);
    
    //this->bangcuoc->reloadValueCuoc(linkChi1,linkChi2,linkChi3);
    this->cuoc1 = linkChi1;
    this->cuoc2 = linkChi2;
    this->cuoc3 = linkChi3;


	if (linkChi1 > linkChi2 && linkChi2 > linkChi3)
		this->imgBinhLung->setVisible(false);
	else if (linkChi1 == linkChi2 && linkChi2 > linkChi3){
		if (this->kiemtraBinhLungChi1Chi2(linkChi1)){
			this->imgBinhLung->setVisible(true);
		}
		else{
			this->imgBinhLung->setVisible(false);
		}
	}
	else if (linkChi1 > linkChi2 && linkChi2 == linkChi3){
		if (this->kiemtraBinhLungChi2Chi3(linkChi2)){
			this->imgBinhLung->setVisible(true);
		}
		else
		{
			this->imgBinhLung->setVisible(false);
		}
	}
	else if (linkChi1 == linkChi2 && linkChi2 == linkChi3){
		if (!this->kiemtraBinhLungChi1Chi2(linkChi1) && !this->kiemtraBinhLungChi2Chi3(linkChi1)){
			this->imgBinhLung->setVisible(false);
		}
		else {
			this->imgBinhLung->setVisible(true);
		}
	}
	else
		this->imgBinhLung->setVisible(true);
}
bool MauBinhLayerXepBai::kiemtraBinhLungChi1Chi2(int _type){
    auto numchi1 = this->getNumChi(1);
    std::sort(numchi1.begin(),numchi1.end());
    auto numchi2 = this->getNumChi(2);
    std::sort(numchi2.begin(),numchi2.end());
    auto doiChi1 = 0;
    auto doiChi2 = 0;
    auto thuChi1_1 = 0;
    auto thuChi1_2 = 0;
    auto thuChi2_1 = 0;
    auto thuChi2_2 = 0;
    auto sanhnhonhatchi1 = false;
    auto sanhnhonhatchi2 = false;
    switch (_type){
        case MB_MAU_THAU:
            for (auto i = 4; i >= 0; i--){
                if (numchi1[i] > numchi2[i]) {
                    return false;
                }
                if (numchi1[i] < numchi2[i]) {
                    return true;
                }
                if (numchi1[i] == numchi2[2]) {
                    if (i == 0) {
                        return false;
                    }
                }
            }
            return false;
            break;
        case MB_DOI:
            for (auto i = 0; i < 4; i++){
                for (auto j = i + 1; j < 5; j++) {
                    if (numchi1[i] == numchi1[j]) {
                        doiChi1 = numchi1[i];
                        
                    }
                    
                }
            }
            for (auto i = 0; i < 4; i++){
                for (auto j = i + 1; j<5; j++) {
                    if (numchi2[i] == numchi2[j]) {
                        doiChi2 = numchi2[i];
                    }
                    
                }
            }
            numchi1.erase(std::remove(numchi1.begin(), numchi1.end(), doiChi1), numchi1.end());
            numchi1.erase(std::remove(numchi1.begin(), numchi1.end(), doiChi1), numchi1.end());
            numchi2.erase(std::remove(numchi2.begin(), numchi2.end(), doiChi2), numchi2.end());
            numchi2.erase(std::remove(numchi2.begin(), numchi2.end(), doiChi2), numchi2.end());

            if (doiChi1 > doiChi2) {
                return false;
            }
            else if (doiChi1 < doiChi2)
            {
                return true;
            }
            else if (doiChi1 == doiChi2){
                
                for (auto i = 2; i >= 0; i--){

                    if (numchi1[i] > numchi2[i]) {
                        return false;
                    }
                    if (numchi1[i] < numchi2[i]) {
                        return true;
                    }
                    if (numchi1[i] == numchi2[0]) {
                        if (i == 0) {
                            return false;
                        }
                    }
                }
            }
            return false;
            break;
        case MB_SAM_CO:
            for (auto i = 0; i < 4; i++){
                for (auto j = i + 1; j < 5; j++) {
                    if (numchi1[i] == numchi1[j]) {
                        doiChi1 = numchi1[i];
                    }
                    
                }
            }
            for (auto i = 0; i < 4; i++){
                for (auto j = i + 1; j<5; j++) {
                    if (numchi2[i] == numchi2[j]) {
                        doiChi2 = numchi2[i];
                    }
                    
                }
            }
            if (doiChi1 > doiChi2) {
                return false;
            }
            else if (doiChi1 < doiChi2)
            {
                return true;
            }
            return false;
            break;
        case MB_THU:
            for (auto i = 3; i >= 0; i--){
                for (auto j = i + 1; j > 0; j--) {
                    if (thuChi1_1 == 0){
                        if (numchi1[i] == numchi1[j]) {
                            thuChi1_1 = numchi1[i];
                           
                        }
                    }
                    else if (thuChi1_1 != numchi1[i]){
                        if (numchi1[i] == numchi1[j]) {
                            thuChi1_2 = numchi1[i];
                           
                        }
                    }
                    
                    
                }
            }
            for (auto i = 3; i >= 0; i--){
                for (auto j = i + 1; j > 0; j--) {
                    if (thuChi2_1 == 0){
                        if (numchi2[i] == numchi2[j]) {
                            thuChi2_1 = numchi2[i];
                        
                        }
                    }
                    else if (thuChi2_1 != numchi2[i]){
                        if (numchi2[i] == numchi2[j]) {
                            thuChi2_2 = numchi2[i];

                        }
                    }
                    
                    
                }
            }
            numchi1.erase(std::remove(numchi1.begin(), numchi1.end(), thuChi1_1), numchi1.end());
            numchi1.erase(std::remove(numchi1.begin(), numchi1.end(), thuChi1_1), numchi1.end());

            numchi1.erase(std::remove(numchi1.begin(), numchi1.end(), thuChi1_2), numchi1.end());
            numchi1.erase(std::remove(numchi1.begin(), numchi1.end(), thuChi1_2), numchi1.end());
            
            numchi2.erase(std::remove(numchi2.begin(), numchi2.end(), thuChi2_1), numchi2.end());
            numchi2.erase(std::remove(numchi2.begin(), numchi2.end(), thuChi2_1), numchi2.end());
            
            numchi2.erase(std::remove(numchi2.begin(), numchi2.end(), thuChi2_2), numchi2.end());
            numchi2.erase(std::remove(numchi2.begin(), numchi2.end(), thuChi2_2), numchi2.end());
            
            if (thuChi1_1 > thuChi2_1) {
                return false;
            }
            else if (thuChi1_1 < thuChi2_1){
                return true;
            }
            else if (thuChi1_1 == thuChi2_1 && thuChi1_2 > thuChi2_2)
            {
                return false;
            }
            else if (thuChi1_1 == thuChi2_1 && thuChi1_2 < thuChi2_2)
            {
                return true;
            }
            else if (thuChi1_1 == thuChi2_1 && thuChi1_2 == thuChi2_2)
            {
              
                    
                    if (numchi1[0] > numchi2[0]) {
                        return false;
                    }
                    if (numchi1[0] < numchi2[0]) {
                        return true;
                    }
                    if (numchi1[0] == numchi2[0]) {
                      
                            return false;
            
                    }
                
                
            }
            return false;
            break;
        case MB_TU_QUY:
            for (auto i = 0; i < 4; i++){
                for (auto j = i + 1; j < 5; j++) {
                    if (numchi1[i] == numchi1[j]) {
                        doiChi1 = numchi1[i];
                    }
                    
                }
            }
            for (auto i = 0; i < 4; i++){
                for (auto j = i + 1; j<5; j++) {
                    if (numchi2[i] == numchi2[j]) {
                        doiChi2 = numchi2[i];
                    }
                    
                }
            }
            if (doiChi1 > doiChi2) {
                return false;
            }
            else if (doiChi1 < doiChi2)
            {
                return true;
            }
            return false;
            break;
        case MB_SANH:
            
            for (auto i = 4; i >= 0; i--){
                if (numchi1[i] == 14){
                    for (auto j = 4; j >= 0; j--){
                        if (numchi1[j] == 2){
                            sanhnhonhatchi1 = true;
                        }
                    }
                }
            }
            for (auto i = 4; i >= 0; i--){
                if (numchi2[i] == 14){
                    for (auto j = 4; j >= 0; j--){
                        if (numchi2[j] == 2){
                            sanhnhonhatchi2 = true;
                        }
                    }
                }
            }
            
            if (sanhnhonhatchi1 && sanhnhonhatchi2){
                return false;
            }
            if (!sanhnhonhatchi1 && sanhnhonhatchi2){
                return false;
            }
            if (sanhnhonhatchi1 && !sanhnhonhatchi2){
                return true;
            }
            
            for (auto i = 4; i >= 0; i--){
                
                if (numchi1[i] > numchi2[i]) {
                    return false;
                }
                if (numchi1[i] < numchi2[i]) {
                    return true;
                }
                if (numchi1[i] == numchi2[2]) {
                    if (i == 0) {
                        return false;
                    }
                }
            }
            return false;
            break;
        case MB_THUNG:
            for (auto i = 4; i >= 0; i--){
                if (numchi1[i] > numchi2[i]) {
                    return false;
                }
                if (numchi1[i] < numchi2[i]) {
                    return true;
                }
                if (numchi1[i] == numchi2[2]) {
                    if (i == 0) {
                        return false;
                    }
                }
            }
            return false;
            break;
        case MB_CU_LU:{
            auto count = 0;
            for (auto i = 0; i < 4; i++){
                if (count == 3){
                    count = 0;
                    break;
                }
                count = 1;
                doiChi1 = numchi1[i];

                for (auto j = i + 1; j < 5; j++) {
                    if (numchi1[i] == numchi1[j]) {
                        count++;
                        if (count == 3)
                            break;
                        
                    }
                }
            }
            for (auto i = 0; i < 4; i++){
                if (count == 3){
                    count = 0;
                    break;
                }
                count = 1;
                doiChi2 = numchi2[i];
                for (auto j = i + 1; j<5; j++) {
                    if (numchi2[i] == numchi2[j]) {
                        count++;
                        if (count == 3)
                            break;
                    }
                   
                }
            }
            if (doiChi1 > doiChi2) {
                return false;
            }
            else if (doiChi1 < doiChi2)
            {
                return true;
            }
            else if (doiChi1 == doiChi2)
            {
                if (numchi1[0]<numchi2[0])
                {
                    return true;
                }else{
                    return false;
                }
            }
            return false;
            break;
        }
        case MB_THUNG_PHA_SANH:{
            
            for (auto i = 4; i >= 0; i--){
                if (numchi1[i] == 14){
                    for (auto j = 4; j >= 0; j--){
                        if (numchi1[j] == 2){
                            sanhnhonhatchi1 = true;
                        }
                    }
                }
            }
            for (auto i = 4; i >= 0; i--){
                if (numchi2[i] == 14){
                    for (auto j = 4; j >= 0; j--){
                        if (numchi2[j] == 2){
                            sanhnhonhatchi2 = true;
                        }
                    }
                }
            }
            
            if (sanhnhonhatchi1 && sanhnhonhatchi2){
                return false;
            }
            if (!sanhnhonhatchi1 && sanhnhonhatchi2){
                return false;
            }
            if (sanhnhonhatchi1 && !sanhnhonhatchi2){
                return true;
            }
            for (auto i = 4; i >= 0; i--){
                if (numchi1[i] > numchi2[i]) {
                    return false;
                }
                if (numchi1[i] < numchi2[i]) {
                    return true;
                }
                if (numchi1[i] == numchi2[2]) {
                    if (i == 0) {
                        return false;
                    }
                }
            }
            return false;
            break;
        }
    }
    return false;
}
bool MauBinhLayerXepBai::kiemtraBinhLungChi2Chi3(int _type){
	vector<int> numchi2 = this->getNumChi(2);
    std::sort(numchi2.begin(),numchi2.end());

	vector<int> numchi3 = this->getNumChi(3);
    std::sort(numchi3.begin(),numchi3.end());

	int doiChi2 = 0;
	int doiChi3 = 0;
	switch (_type){
	case MB_MAU_THAU:
		for (int i = 4; i >= 2; i--){
			if (numchi2[i] > numchi3[i - 2]) {
				return false;
			}
			if (numchi2[i] < numchi3[i - 2]) {
				return true;
			}
			if (numchi2[i] == numchi3[i - 2]) {
				if (i == 2) {
					return false;
				}
			}
		}
		break;
	case MB_DOI:
		for (int i = 0; i < 4; i++){
			for (int j = i + 1; j < 5; j++) {
				if (numchi2[i] == numchi2[j]) {
					doiChi2 = numchi2[i];
                
				}

			}
		}
		for (int i = 0; i < 2; i++){
			for (int j = i + 1; j<3; j++) {
				if (numchi3[i] == numchi3[j]) {
					doiChi3 = numchi3[i];
                  
				}

			}
		}
            numchi2.erase(std::remove(numchi2.begin(), numchi2.end(), doiChi2), numchi2.end());
            numchi2.erase(std::remove(numchi2.begin(), numchi2.end(), doiChi2), numchi2.end());
            numchi3.erase(std::remove(numchi3.begin(), numchi3.end(), doiChi3), numchi3.end());
            numchi3.erase(std::remove(numchi3.begin(), numchi3.end(), doiChi3), numchi3.end());
		if (doiChi2 > doiChi3) {
			return false;
		}
		else if (doiChi2 < doiChi3)
		{
			return true;
		}
		else if (doiChi2 == doiChi3){
            
			for (int i = 2; i >= 0; i--){
				if (numchi2[i] > numchi3[0]) {
					return false;
				}
				if (numchi2[i] < numchi3[0]) {
					return true;
				}
				if (numchi2[i] == numchi3[0]) {
					//if (i == 0) {
						return false;
					//}
				}
			}
		}

	case MB_SAM_CO:
		for (int i = 0; i < 4; i++){
			for (int j = i + 1; j < 5; j++) {
				if (numchi2[i] == numchi2[j]) {
					doiChi2 = numchi2[i];
				}

			}
		}
		for (int i = 0; i < 2; i++){
			for (int j = i + 1; j<3; j++) {
				if (numchi3[i] == numchi3[j]) {
					doiChi3 = numchi3[i];
				}

			}
		}
		if (doiChi2 > doiChi3) {
			return false;
		}
		else if (doiChi2 < doiChi3)
		{
			return true;
		}

	}
	return false;
}
vector<int> MauBinhLayerXepBai::getNumChi(int chi){
	vector<int> numChi;
	if (chi == 1){
		for (int i = 0; i < 5; i++){
			MauBinhCard* card = this->tongChi[i];
			string currentValue = card->getValue();
			int numCurrent = atoi(mUtils::splitString(currentValue, '_')[0].c_str());
			numChi.push_back(numCurrent);
		}
	}
	if (chi == 2){
		for (int i = 5; i < 10; i++){
			MauBinhCard* card = this->tongChi[i];
			string currentValue = card->getValue();
			int numCurrent = atoi(mUtils::splitString(currentValue, '_')[0].c_str());
			numChi.push_back(numCurrent);
		}
	}
	if (chi == 3){
		for (int i = 10; i < 13; i++){
			MauBinhCard* card = this->tongChi[i];
			string currentValue = card->getValue();
			int numCurrent = atoi(mUtils::splitString(currentValue, '_')[0].c_str());
			numChi.push_back(numCurrent);
		}
	}

	std::sort(numChi.begin(), numChi.end());
	if (numChi.size() == 4)
	{
		if (numChi[4] == 14 && numChi[4] == 5 && numChi[4] == 4 && numChi[4] == 3 && numChi[4] == 2){
			numChi.clear();
			for (int i = 1; i <= 5; i++){
				numChi.push_back(i);
			}
		}
	}

	return numChi;

}
int MauBinhLayerXepBai::checkChi3La(){
	int linktype = 100;

	MauBinhCard* card1 = this->tongChi[10];
	string cardvalue1 = card1->getValue();
	int num1 = atoi(mUtils::splitString(cardvalue1, '_')[0].c_str());

	MauBinhCard* card2 = this->tongChi[11];
	string cardvalue2 = card2->getValue();
	int num2 = atoi(mUtils::splitString(cardvalue2, '_')[0].c_str());

	MauBinhCard* card3 = this->tongChi[12];
	string cardvalue3 = card3->getValue();
	int num3 = atoi(mUtils::splitString(cardvalue3, '_')[0].c_str());

	if (num1 == num2 && num1 == num3)
		linktype = MB_SAM_CO;
	else if (num1 == num2){
		if (num1 != num3) {
			linktype = MB_DOI;
		}
	}
	else if (num2 == num3){
		if (num1 != num3) {
			linktype = MB_DOI;
		}
	}
	else if (num1 == num3){
		if (num1 != num2) {
			linktype = MB_DOI;
		}
	}
	else if (num1 != num2 && num1 != num3)
		linktype = MB_MAU_THAU;

	return linktype;
}

void MauBinhLayerXepBai::displayCardWasLinked(int _type, int chi){
	switch (_type){
	case MB_MAU_THAU:
		if (chi == 1){
			MauBinhCard* highCard = this->tongChi[0];

			for (int i = 0; i < 5; i++)
			{
				MauBinhCard* card = this->tongChi[i];
				string currentValue = card->getValue();
				int numCurrent = atoi(mUtils::splitString(currentValue, '_')[0].c_str());
				string highValue = highCard->getValue();
				int numHigh = atoi(mUtils::splitString(highValue, '_')[0].c_str());
				if (numCurrent > numHigh) {
					highCard = card;
				}
			}
			highCard->setIsLink();
		}
		if (chi == 2){
			MauBinhCard* highCard = this->tongChi[5];

			for (int i = 5; i < 10; i++)
			{
				MauBinhCard* card = this->tongChi[i];
				string currentValue = card->getValue();
				int numCurrent = atoi(mUtils::splitString(currentValue, '_')[0].c_str());
				string highValue = highCard->getValue();
				int numHigh = atoi(mUtils::splitString(highValue, '_')[0].c_str());
				if (numCurrent > numHigh) {
					highCard = card;
				}
			}
			highCard->setIsLink();
		}

		if (chi == 3){
			MauBinhCard* highCard = this->tongChi[10];

			for (int i = 10; i < 13; i++)
			{
				MauBinhCard* card = this->tongChi[i];
				string currentValue = card->getValue();
				int numCurrent = atoi(mUtils::splitString(currentValue, '_')[0].c_str());
				string highValue = highCard->getValue();
				int numHigh = atoi(mUtils::splitString(highValue, '_')[0].c_str());
				if (numCurrent > numHigh) {
					highCard = card;
				}
			}
			highCard->setIsLink();
		}

		break;
	case MB_DOI:
	case MB_SAM_CO:
	case MB_THU:
	case MB_TU_QUY:
		if (chi == 1){
			for (int i = 0; i < 4; i++){
				for (int j = i + 1; j < 5; j++) {
					MauBinhCard* card1 = this->tongChi[i];
					MauBinhCard* card2 = this->tongChi[j];
					string currentValue = card1->getValue();
					int numCurrent = atoi(mUtils::splitString(currentValue, '_')[0].c_str());
					string highValue = card2->getValue();
					int numHigh = atoi(mUtils::splitString(highValue, '_')[0].c_str());
					if (numCurrent == numHigh) {
						card1->setIsLink();
						card2->setIsLink();
					}

				}
			}
		}
		if (chi == 2){
			for (int i = 5; i < 9; i++){
				for (int j = i + 1; j < 10; j++) {
					MauBinhCard* card1 = this->tongChi[i];
					MauBinhCard* card2 = this->tongChi[j];
					string currentValue = card1->getValue();
					int numCurrent = atoi(mUtils::splitString(currentValue, '_')[0].c_str());
					string highValue = card2->getValue();
					int numHigh = atoi(mUtils::splitString(highValue, '_')[0].c_str());
					if (numCurrent == numHigh) {
						card1->setIsLink();
						card2->setIsLink();
					}

				}
			}
		}

		if (chi == 3){
			for (int i = 10; i < 12; i++){
				for (int j = i + 1; j < 13; j++) {
					MauBinhCard* card1 = this->tongChi[i];
					MauBinhCard* card2 = this->tongChi[j];
					string currentValue = card1->getValue();
					int numCurrent = atoi(mUtils::splitString(currentValue, '_')[0].c_str());
					string highValue = card2->getValue();
					int numHigh = atoi(mUtils::splitString(highValue, '_')[0].c_str());
					if (numCurrent == numHigh) {
						card1->setIsLink();
						card2->setIsLink();
					}

				}
			}
		}
		break;
	case MB_SANH:
	case MB_THUNG:
	case MB_CU_LU:
	case MB_THUNG_PHA_SANH:
		if (chi == 1){
			MauBinhCard* highCard = this->tongChi[0];

			for (int i = 0; i < 5; i++)
			{
				MauBinhCard* card = this->tongChi[i];
				card->setIsLink();
			}
		}
		if (chi == 2){
			MauBinhCard* highCard = this->tongChi[5];

			for (int i = 5; i < 10; i++)
			{
				MauBinhCard* card = this->tongChi[i];
				card->setIsLink();
			}
		}

		if (chi == 3){
			MauBinhCard* highCard = this->tongChi[10];

			for (int i = 10; i < 13; i++)
			{
				MauBinhCard* card = this->tongChi[i];
				card->setIsLink();
			}
		}

		break;
	default:
		break;
	}

}

string MauBinhLayerXepBai::convertCardToCheck(string _value){
	auto lstchi = mUtils::splitString(_value, '_');
	string url = "";
	if (lstchi.size() < 2)
		return "";
	int num = atoi(lstchi[0].c_str());
	int type = atoi(lstchi[1].c_str());
	switch (num) {
	case 10:
		url += "t";
		break;
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
		url += StringUtils::format("%d", num);
		break;
	}


	switch (type) {
	case 1:
		url += "h"; // cơ
		break;
	case 2:
		url += "d"; // rô
		break;
	case 3:
		url += "c"; // tép
		break;
	case 4:
		url += "s"; // bích
		break;
	default:
		break;
	}
	return url;
}
void MauBinhLayerXepBai::setTimeXepBai(int timexep){
	this->timeXepBai = timexep;
	Director::getInstance()->getScheduler()->schedule(schedule_selector(MauBinhLayerXepBai::updateTimeXepBai),this,1, false);

}
void MauBinhLayerXepBai::updateTimeXepBai(float dt){
	if (this->timeXepBai > 0){
		this->timeXepBai--;
		this->txtTimer->setString(StringUtils::format("%d", this->timeXepBai));
		this->txtTimer->setVisible(true);
	}
	else
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(MauBinhLayerXepBai::updateTimeXepBai),this);

}
void MauBinhLayerXepBai::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(MauBinhLayerXepBai::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MauBinhLayerXepBai::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MauBinhLayerXepBai::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
	//
	//    GameServer::getSingleton().addListeners(this);
}

void MauBinhLayerXepBai::onExit()
{
	Layer::onExit();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(listener);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(MauBinhLayerXepBai::updateTimeXepBai),this);
    LayerBoBaiMauBinh* parent = (LayerBoBaiMauBinh*)this->getParent();
    if (parent) {
        parent->showMyListCard();
        parent->setLocalZOrder(110);
        parent->setIsXepBai(false);
        
        LayerGameMauBinh* game = (LayerGameMauBinh*)parent->getParent();
        if (game){
            game->reloadCuoc(this->cuoc1,this->cuoc2,this->cuoc3);
        }
    }

	//boost::shared_ptr<ISFSObject> params(new SFSObject());
	//    boost::shared_ptr<IRequest> req (new ExtensionRequest("txl", params));
	//    GameServer::getSingleton().Send(req);
	//    GameServer::getSingleton().removeListeners(this);

	//    GameServer::getSingleton().removeListeners(this);
	//    Layer::onExit();
}
