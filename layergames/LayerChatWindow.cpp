#include "LayerChatWindow.h"
#include "SceneManager.h"
#include "mUtils.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../CrossPlatform/Android/Android.h"
#endif

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

Scene* LayerChatWindow::scene()
{
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	LayerChatWindow *layer = LayerChatWindow::create();

	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

static inline void printAllChildrenTypes(Node* node, int level)
{
	std::string spaces(level, ' ');

	CCLOG("%s %02d : %s - name: %s", spaces.c_str(), level, typeid(*node).name(), node->getName().c_str());

	++level;

	int index = 0;
	//PageView* widget1 = dynamic_cast<PageView*>(widget->getChildByName("pBackground"));
	while (index < node->getChildren().size())
	{
		Node* obj = node->getChildren().at(index);

		printAllChildrenTypes(static_cast<Node*>(obj), level);

		index++;
		//printAllChildrenTypes(static_cast(obj), level);
	}
}

// on "init" you need to initialize your instance
bool LayerChatWindow::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	lstStringMessage.clear();
	lstStringMessage.push_back("Nhanh lên nào (14)!");
	lstStringMessage.push_back("Mạng lag quá (9)!");
	lstStringMessage.push_back("Bài xấu quá (3)!");
	lstStringMessage.push_back("Hehe, lên nóc nhà\nmà bắt con gà (6)");
	lstStringMessage.push_back("Nhất rồi, haha (1)");
	lstStringMessage.push_back("Đánh gà thế (11)!");
	lstStringMessage.push_back("Đỏ vãi lúa (13)!");
	lstStringMessage.push_back("Nhọ như chó mực (5)!");

	//return true;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	_count = 100;
	layerButton = Layer::create();
	Layout *widget = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("LayerChatWindow_1.ExportJson"));
	layerButton->addChild(widget);
	this->addChild(layerButton);

	int index = 0;
	Layout* layoutButtons = dynamic_cast<Layout*>(widget->getChildByName("pBackground"));
	if (layoutButtons != NULL){
		PageView* pvChats = dynamic_cast<PageView*>(layoutButtons->getChildByName("pChat"));
		if (pvChats){
			Layout* pChatMessage = dynamic_cast<Layout*>(pvChats->getChildByName("pChatMessage"));
			if (pChatMessage){
				//Get all chat button 
				for (int i = 1; i <= 8; i++){
					Button* btnChat = dynamic_cast<Button*>(pChatMessage->getChildByName(StringUtils::format("btnChat%d", i)));
					btnChat->setTitleText(lstStringMessage.at(i - 1).c_str());
					btnChat->addClickEventListener(CC_CALLBACK_1(LayerChatWindow::onButtonChats, this));
				}
			}
			//Emos
			Layout* pEmo = dynamic_cast<Layout*>(pvChats->getChildByName("pEmo"));
			if (pEmo){
				ImageView* pImg = dynamic_cast<ImageView*>(pEmo->getChildByName("Image_25_0")); 
				int i = 1;
				int row = -1, col = 1;
				int widthOfPage = pImg->getContentSize().width;
				int numOfCols = 9;
				int space = 10;
				int widthOfImage = 50;
				int startPosX = widthOfPage - (widthOfImage + space)*numOfCols / 2 - 300;
				int startPosY = space - 100;
				lstEmo.clear();
				for (int i = 1; i <= 16; i++){
					Armature *armature = Armature::create(StringUtils::format("onion%d", 1));
					armature->getAnimation()->playWithIndex(i - 1);
					RichElementCustomNode* recustom = RichElementCustomNode::create(1, Color3B::WHITE, 255, armature);
					RichText* _richText = RichText::create();
					_richText->setContentSize(armature->getContentSize());
					_richText->pushBackElement(recustom);
					if (col > numOfCols){
						row++;
						col = 1;
					}
					_richText->setAnchorPoint(Point(0, 1));
					_richText->setPosition(Point(startPosX + 60 * (col - 1)
						, -1 * row * 50 - startPosY - (row + 1)*space));
					//_richText->addClickEventListener(CC_CALLBACK_1(LayerChatWindow::OnEmoClick, this));

					lstEmo.push_back(armature);

					Button* b = Button::create();
					b->setPosition(Point(startPosX + 60 * (col - 1)
						, -1 * row * 50 - startPosY - (row + 1)*space));
					b->setAnchorPoint(Point(0, 1));
					b->setContentSize(armature->getContentSize());
					b->addClickEventListener(CC_CALLBACK_1(LayerChatWindow::OnEmoClick, this));
					b->loadTextures("emo_blank.png", "", "");
					b->setTag(i);

					pImg->addChild(_richText);
					pImg->addChild(b);
					//log("button w: %lf h: %lf", b->getContentSize().width, b->getContentSize().height);
					col++;
				}
			}
		}
		//getTextEdit
		ImageView* Image_19 = dynamic_cast<ImageView*>(layoutButtons->getChildByName("Image_19"));
		if (Image_19){
			txtChat = dynamic_cast<TextField*>(Image_19->getChildByName("txtChat"));
			txtChat->setString("");
			txtChat->setPlaceHolder(dataManager.GetSysString(174));
			txtChat->setTextHorizontalAlignment(TextHAlignment::CENTER);
			txtChat->setTextVerticalAlignment(TextVAlignment::CENTER);
			txtChat->addEventListener(CC_CALLBACK_2(LayerChatWindow::textFieldEvent, this));
		}
		//Get button send
		Button* btnSend = dynamic_cast<Button*>(layoutButtons->getChildByName("btnSend"));
		btnSend->addTouchEventListener(CC_CALLBACK_1(LayerChatWindow::onButtonSend, this));
		btnSend->setContentSize(Size(70, 40));
		//Button close
		Button* btnClose = dynamic_cast<Button*>(layoutButtons->getChildByName("btnClose"));
		btnClose->addTouchEventListener(CC_CALLBACK_1(LayerChatWindow::onButtonClose, this));
	}
	return true;
}

void LayerChatWindow::onButtonClose( Ref* pSender )
{
	SceneManager::getSingleton().hideLayerChatWindow();
	txtChat->setString("");
	txtChat->setDetachWithIME(false);
	//
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
}

void LayerChatWindow::onButtonSend( Ref* pSender )
{
	if (m_callback && m_callbackListener)
	{
		(m_callback->*m_callbackListener)(this, (void*)txtChat->getString().c_str());
	}
	log("chat message: %s", txtChat->getString().c_str());
	SceneManager::getSingleton().hideLayerChatWindow();
	txtChat->setString("");
	txtChat->setDetachWithIME(false);
	//
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	//hide keyboard
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	//hideKeyboard();
#else
	log("Khong ho tro nen tang nay");
#endif
}

void LayerChatWindow::onButtonChats( Ref* pSender )
{
	Button* btnChat = dynamic_cast<Button*>(pSender);
	//if(type == Widget::TouchEventType::ENDED){
		log("onButtonChats %s", btnChat->getTitleText().c_str());
		txtChat->setString( btnChat->getTitleText() );
	//}
}


// void LayerChatWindow::registerWithTouchDispatcher( void )
// {
// 	//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
// }

bool LayerChatWindow::onTouchBegan( cocos2d::Touch *pTouch, cocos2d::Event *pEvent )
{
	return true;
}

void LayerChatWindow::onEnter()
{
	//
	Layer::onEnter();
	this->runAction(mUtils::getActionOpenPopup());
}

void LayerChatWindow::onExit()
{
	Layer::onExit();
// 	auto dispatcher = Director::getInstance()->getEventDispatcher();
// 	dispatcher->removeEventListener(_touchListener);
}

void LayerChatWindow::OnEmoClick( Ref* pSender )
{
	Button* btn = dynamic_cast<Button*>(pSender);
	//if( type == Widget::TouchEventType::ENDED ){
		log("OnEmoClick button tag: %d", btn->getTag());
		txtChat->setString( StringUtils::format("%s(%d)", txtChat->getString().c_str(), btn->getTag()) );
	//}
}

void LayerChatWindow::setCallbackFunc( Ref* target, SEL_CallFuncND callfun )
{
	m_callback = target;
	m_callbackListener = callfun;
}

LayerChatWindow::~LayerChatWindow()
{
	log("~LayerChatWindow()");
}

void LayerChatWindow::textFieldEvent(Ref *pSender, TextField::EventType type)
{
// 	switch (type)
// 	{
// 	case TEXTFIELD_EVENT_ATTACH_WITH_IME:
// 		{
// 			log("TEXTFIELD_EVENT_ATTACH_WITH_IME");
// 		}
// 		break;
// 
// 	case TEXTFIELD_EVENT_DETACH_WITH_IME:
// 		{
// 			log("TEXTFIELD_EVENT_DETACH_WITH_IME");
// 		}
// 		break;
// 
// 	case TEXTFIELD_EVENT_INSERT_TEXT:
// 		log("TEXTFIELD_EVENT_INSERT_TEXT");
// 		break;
// 	case TEXTFIELD_EVENT_DELETE_BACKWARD:
// 		log("TEXTFIELD_EVENT_DELETE_BACKWARD");
// 		break;
// 	default:
// 		break;
// 	}
}

void LayerChatWindow::pauseAllAnimations()
{
	for( int i = 0; i<lstEmo.size(); i++ ){
		lstEmo.at(i)->getAnimation()->pause();
	}
}

void LayerChatWindow::resumeAllAnimations()
{
	for( int i = 0; i<lstEmo.size(); i++ ){
		lstEmo.at(i)->getAnimation()->resume();
	}
}

void LayerChatWindow::setListChatMessage( vector<string> lst )
{
	if( lst.size()!=8 )
		return;
	lstStringMessage.clear();
	for( int i = 0; i<lst.size(); i++ ){
		lstStringMessage.push_back(lst.at(i));
	}
	//reload button title
	for( int i = 1; i <= 8; i++ ){
		Button* btnChat = dynamic_cast<Button*>(layerButton->getChildByName( StringUtils::format("btnChat%d", i) ));
		btnChat->setTitleText( lstStringMessage.at(i-1).c_str() );
	}
}

void LayerChatWindow::showChatWindow()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerChatWindow::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}
