//
//  LayerMainGame.cpp
//  EnglishGame-mobile
//
//  Created by Sherlock Tuan on 11/30/18.
//

#include "LayerMainGame2.hpp"
#include "GameManager.hpp"
#include <algorithm>
#include "LayerMenuGame.hpp"

Scene* LayerMainGame2::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LayerMainGame2::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    //    CCSVParse *pCsv2 = new CCSVParse();
    //    pCsv2->openFile("englishgame/enemy.csv");
    //
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LayerMainGame2::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    string path = "LayerMainGame2.csb";
    auto rootNode = CSLoader::getInstance()->createNode(path);
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    pnlBg->setPosition(Point(visibleSize / 2));
    Sprite* Sprite_1 = static_cast<Sprite*>(rootNode->getChildByName("Sprite_1"));
    Sprite_1->setScaleY(visibleSize.height / 1080);
    //rootNode->setAnchorPoint(Point(0.5, 0.5));
    //rootNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    
    
    imgQuest = static_cast<ImageView*>(pnlBg->getChildByName("imgQuest"));
    imgQuest->setVisible(false);
    txtName = static_cast<Text*>(pnlBg->getChildByName("txtName"));
    this->txtName->setString(name);
    auto btnConfirm = static_cast<Button*>(pnlBg->getChildByName("btnConfirm"));
    btnConfirm->addClickEventListener(CC_CALLBACK_1(LayerMainGame2::OnBtnConfirm, this));
    auto btnDelete = static_cast<Button*>(pnlBg->getChildByName("btnDelete"));
    btnDelete->addClickEventListener(CC_CALLBACK_1(LayerMainGame2::OnBtnDelete, this));
    
    pnlPopup_ = static_cast<Layout*>(rootNode->getChildByName("pnlPopup"));
    pnlPopup_->setSwallowTouches(false);
    pnlPopup_->setVisible(false);
    pnlPopup_->setScaleY(visibleSize.height / 1080);

    
    pnlPopup = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    pnlPopup->setSwallowTouches(false);
    pnlPopup->setVisible(false);
    
    pnlPopup->setAnchorPoint(Point(0.5, 0.5));
    pnlPopup->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    
    btnMenu = static_cast<Button*>(pnlPopup->getChildByName("btnMenu"));
    btnMenu->addClickEventListener(CC_CALLBACK_1(LayerMainGame2::OnBtnMenu, this));
    
    btnReplay = static_cast<Button*>(pnlPopup->getChildByName("btnReplay"));
    btnReplay->addClickEventListener(CC_CALLBACK_1(LayerMainGame2::OnBtnReplay, this));
    
    btnNext = static_cast<Button*>(pnlPopup->getChildByName("btnNext"));
    btnNext->addClickEventListener(CC_CALLBACK_1(LayerMainGame2::OnBtnNext, this));
    
    txtNotice = static_cast<Text*>(pnlPopup->getChildByName("lblNotice"));
    
    for (int i = 1; i <= 26; i++){
        auto btn = static_cast<Button*>(pnlBg->getChildByName(StringUtils::format("btn%d",i)));
        btn->setTag(i-1);
        btn->addClickEventListener(CC_CALLBACK_1(LayerMainGame2::onClickLetter, this));
        this->lstBtnLetter.push_back(btn);
    }
    
    for (int i = 1 ; i <= GameManager::getInstance()->countQuest; i++){
        this->lstQuest.push_back(i);
    }
    
    std::random_shuffle ( lstQuest.begin(), lstQuest.end() );

    
    lstLetter = {"A","B","C","D","E","F","G","H","I","G","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
    this->scheduleOnce(schedule_selector(LayerMainGame2::showBgNewGame), 0.1);
    
    return true;
}
void LayerMainGame2::OnBtnNext(Ref* pSender)
{
    this->showQuest();
}
void LayerMainGame2::OnBtnReplay(Ref* pSender)
{
    std::random_shuffle ( lstQuest.begin(), lstQuest.end() );
    this->currentQuest = 0;
    this->showQuest();
}
void LayerMainGame2::OnBtnMenu(Ref* pSender)
{
    auto scene = LayerMenuGame::createScene();
    Director::getInstance()->replaceScene(scene);
}
void LayerMainGame2::showBgNewGame(float dt){
    pnlPopup_->setVisible(true);
    pnlPopup->setVisible(true);
    btnReplay->setVisible(false);
    txtNotice->setString("Input name for the following animals !");
}
void LayerMainGame2::showQuest(){
    imgQuest->setVisible(true);
    pnlPopup_->setVisible(false);
    pnlPopup->setVisible(false);
    pnlPopup_->setSwallowTouches(false);
    pnlPopup->setSwallowTouches(false);

    this->name = "";
    txtName->setString(name);
    this->imgQuest->loadTexture(StringUtils::format("englishgame/quest/%d.png",this->lstQuest[currentQuest]));
}
void LayerMainGame2::onClickLetter(Ref* pSender){
    if (name.length() >= 12) return;
    Button* btn = (Button*)pSender;
    int tag = btn->getTag();
    name.append(lstLetter[tag]);
    txtName->setString(name);
}
void LayerMainGame2::OnBtnConfirm(Ref* pSender)
{
    string answer = name;
    std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
    string result = GameManager::getInstance()->GetDataGame(this->lstQuest[currentQuest]);


    if (answer == result){
        log ("chuan roi");
        currentQuest++;
        if (currentQuest >= GameManager::getInstance()->countQuest){
            pnlPopup_->setVisible(true);
            pnlPopup->setVisible(true);
            pnlPopup_->setSwallowTouches(true);
            pnlPopup->setSwallowTouches(true);
            btnReplay->setVisible(true);
            txtNotice->setString("Congratulations ! You have completed the entire challenge!");
            btnNext->setVisible(false);
            
            return;
        }
        this->showSuccess();

    }else{
        log("loi roi");
        this->showFail();
    }
}
void LayerMainGame2::showFail(){
    pnlPopup_->setSwallowTouches(true);
    pnlPopup->setSwallowTouches(true);
    pnlPopup_->setVisible(true);
    pnlPopup->setVisible(true);
    btnReplay->setVisible(true);
    txtNotice->setString("Wrong ! Let's try next time!");
    btnNext->setVisible(false);
}
void LayerMainGame2::showSuccess(){
    pnlPopup_->setSwallowTouches(true);
    pnlPopup->setSwallowTouches(true);
    pnlPopup_->setVisible(true);
    pnlPopup->setVisible(true);
    btnReplay->setVisible(false);
    btnNext->setVisible(true);
    txtNotice->setString("Exactly ! Continue to the next question!");
}
void LayerMainGame2::Complete(){
    
}
void LayerMainGame2::OnBtnDelete(Ref* pSender)
{
    name = "";
    txtName->setString(name);
}
