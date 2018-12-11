#include "LayerPopupChapQuan.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "CommonChess.h"
#include "mUtils.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool LayerPopupChapQuan::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	//    auto rootNode = CSLoader::createNode("LayerChonGame.csb");
	//    this->addChild(rootNode);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupChapQuan.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (pnlMain)
	{
		/************************************************************************/
		/* get checkbox and set tag                                                                     */
		/************************************************************************/
		pnlMain->setSwallowTouches(true);
		cbXe1 = (CheckBox*)pnlMain->getChildByName("cbXe1");
		cbXe2 = (CheckBox*)pnlMain->getChildByName("cbXe2");
		cbPhao1 = (CheckBox*)pnlMain->getChildByName("cbPhao1");
		cbPhao2 = (CheckBox*)pnlMain->getChildByName("cbPhao2");
		cbMa1 = (CheckBox*)pnlMain->getChildByName("cbMa1");
		cbMa2 = (CheckBox*)pnlMain->getChildByName("cbMa2");
		cbTuong1 = (CheckBox*)pnlMain->getChildByName("cbTuong1");
		cbTuong2 = (CheckBox*)pnlMain->getChildByName("cbTuong2");
		cbSi1 = (CheckBox*)pnlMain->getChildByName("cbSi1");
		cbSi2 = (CheckBox*)pnlMain->getChildByName("cbSi2");
		cbTot1 = (CheckBox*)pnlMain->getChildByName("cbTot1");
		cbTot2 = (CheckBox*)pnlMain->getChildByName("cbTot2");
		cbTot3 = (CheckBox*)pnlMain->getChildByName("cbTot3");
		cbTot4 = (CheckBox*)pnlMain->getChildByName("cbTot4");
		cbTot5 = (CheckBox*)pnlMain->getChildByName("cbTot5");

		cbXe1->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbXe2->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbPhao1->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbPhao2->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbMa1->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbMa2->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbTuong1->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbTuong2->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbSi1->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbSi2->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbTot1->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbTot2->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbTot3->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbTot4->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));
		cbTot5->addEventListener(CC_CALLBACK_2(LayerPopupChapQuan::onCheckboxConfig, this));

		Button* btnSubmit = (Button*)pnlMain->getChildByName("btnSubmit");
		Button* btnQuit = (Button*)pnlMain->getChildByName("btnExit");

		btnSubmit->addClickEventListener(CC_CALLBACK_1(LayerPopupChapQuan::onButtonSubmit, this));
		btnQuit->addClickEventListener(CC_CALLBACK_1(LayerPopupChapQuan::onButtonQuit, this));
		spTable = (Sprite*)pnlMain->getChildByName("spTable");
		this->initTableChess();
	}

	return true;
}

LayerPopupChapQuan::LayerPopupChapQuan()
{
}

LayerPopupChapQuan::~LayerPopupChapQuan()
{

}

void LayerPopupChapQuan::onButtonQuit(Ref* pSender)
{
	//update 10/7
	/*this->removeFromParentAndCleanup(true);*/
	this->setVisible(false);
}

void LayerPopupChapQuan::initTableChess()
{
	if (spTable)
	{
		CHESS_TYPE typeChess1 = CHESS_TYPE::SIMPLE;
		// quan do
		Chess *xe_red_1 = Chess::create(2, ROOK, 0, typeChess1);
		Chess *ma_red_1 = Chess::create(2, KNIGHT, 1, typeChess1);
		Chess *tuongj_red_1 = Chess::create(2, ELEPHANT, 2, typeChess1);
		Chess *si_red_1 = Chess::create(2, BISHOP, 3, typeChess1);
		Chess *tuong_red = Chess::create(2, KING, 4, typeChess1);
		Chess *si_red_2 = Chess::create(2, BISHOP, 5, typeChess1);
		Chess *tuongj_red_2 = Chess::create(2, ELEPHANT, 6, typeChess1);
		Chess *ma_red_2 = Chess::create(2, KNIGHT, 7, typeChess1);
		Chess *xe_red_2 = Chess::create(2, ROOK, 8, typeChess1);
		Chess *phao_red_1 = Chess::create(2, CANNON, 19, typeChess1);
		Chess *phao_red_2 = Chess::create(2, CANNON, 25, typeChess1);
		Chess *tot_red_1 = Chess::create(2, PAWN, 27, typeChess1);
		Chess *tot_red_2 = Chess::create(2, PAWN, 29, typeChess1);
		Chess *tot_red_3 = Chess::create(2, PAWN, 31, typeChess1);
		Chess *tot_red_4 = Chess::create(2, PAWN, 33, typeChess1);
		Chess *tot_red_5 = Chess::create(2, PAWN, 35, typeChess1);
		arrChess.push_back(xe_red_1);
		arrChess.push_back(xe_red_2);
		arrChess.push_back(phao_red_1);
		arrChess.push_back(phao_red_2);
		arrChess.push_back(ma_red_1);
		arrChess.push_back(ma_red_2);

		arrChess.push_back(tuongj_red_1);
		arrChess.push_back(tuongj_red_2);
		arrChess.push_back(si_red_1);
		arrChess.push_back(si_red_2);

		arrChess.push_back(tot_red_1);
		arrChess.push_back(tot_red_2);
		arrChess.push_back(tot_red_3);
		arrChess.push_back(tot_red_4);
		arrChess.push_back(tot_red_5);

		arrChess.push_back(tuong_red);

		for (int i = 0; i < arrChess.size(); i++) {
			//Insert raw name of chess
			arrChess.at(i)->setRawName(FOLD);
			spTable->addChild(arrChess.at(i));
		}
		this->refreshChess();
	}
}

cocos2d::Vec2 LayerPopupChapQuan::getPositionFromIndex(int id)
{
	//start index tag from 220
	Node* nodeChild = spTable->getChildByTag(1072 + id);
	if (!nodeChild){
		return Vec2(0, 0);
	}
	return Vec2(nodeChild->getPosition().x, nodeChild->getPosition().y - 10);
}

void LayerPopupChapQuan::refreshChess()
{
	// Đặt lại vị trí của quân cờ theo đúng id_pos
	for (int i = 0; i < arrChess.size(); i++) {
		Chess *chess = arrChess.at(i);
		chess->setScale(0.7);
		chess->setPosition(getPositionFromIndex(chess->getIDPosInit()));
		chess->setIDPos(chess->getIDPosInit());
		chess->setVisible(true);
	}
}

void LayerPopupChapQuan::onCheckboxConfig(Ref* pSender, CheckBox::EventType type)
{
	bool isHide = false;
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		isHide = true;
		break;

	case CheckBox::EventType::UNSELECTED:
		isHide = false;
		break;

	default:
		break;
	}
	int tagCB = ((CheckBox*)pSender)->getTag();
	this->handleConfig(isHide, tagCB);
}

void LayerPopupChapQuan::handleConfig(bool isHide, int tag)
{

	switch (tag)
	{
	case 515://xe trai
		arrChess.at(0)->setVisible(isHide);
		break;
	case 517:
		arrChess.at(1)->setVisible(isHide);
		break;
	case 519://phao trai
		arrChess.at(2)->setVisible(isHide);
		break;
	case 521://phao phai
		arrChess.at(3)->setVisible(isHide);
		break;
	case 523://Ma
		arrChess.at(4)->setVisible(isHide);
		break;
	case 525:
		arrChess.at(5)->setVisible(isHide);
		break;
	case 527://Tuongj
		arrChess.at(6)->setVisible(isHide);
		break;
	case 529:
		arrChess.at(7)->setVisible(isHide);
		break;
	case 531://si
		arrChess.at(8)->setVisible(isHide);
		break;
	case 533:
		arrChess.at(9)->setVisible(isHide);
		break;
	case 535://tot
		arrChess.at(10)->setVisible(isHide);
		break;
	case 537:
		arrChess.at(11)->setVisible(isHide);
		break;
	case 539:
		arrChess.at(12)->setVisible(isHide);
		break;
	case 541:
		arrChess.at(13)->setVisible(isHide);
		break;
	case 543:
		arrChess.at(14)->setVisible(isHide);
		break;

	default:
		break;
	}
}

void LayerPopupChapQuan::onButtonSubmit(Ref* pSender)
{
	//update 10/7
	this->generateConfigString();
	if (callback != nullptr)
	{
		this->runAction(Sequence::create(CallFunc::create([=]{
			callback(this, _strConfig, _strDisplay);
		}), Hide::create(), NULL));

	}
}

void LayerPopupChapQuan::setCallBackConfirm(const popupCb& callback)
{
	this->callback = callback;
}

void LayerPopupChapQuan::generateConfigString()
{
	//string config ban dau 0-0 : không chấp quân nào, id
	//Tốt	Sĩ	Tượng	Mã	Pháo	Xe
	//	0	1	2		3	4		5
	int dem = 0;//bien kiem tra xem neu khong co quan co nao bi an thi se tra ve config = 0-0
	_strConfig = "";
	_strDisplay = "";
	int demTot = 0;
	int demMa = 0;
	int demSi = 0;
	int demTuongj = 0;
	int demPhao = 0;
	int demXe = 0;
	for (int i = 0; i < arrChess.size(); i++)
	{
		if (!arrChess.at(i)->isVisible())
		{
			dem++;
			switch (i)
			{
				//ngược lại do set tag node ngược từ cao xuống thấp
			case 0://xe
				_strConfig.append("5-1,");
				demXe++;
				break;
			case 1:
				_strConfig.append("5-9,");
				demXe++;
				break;
			case 2://phao
				_strConfig.append("4-2,");
				demPhao++;
				break;
			case 3:
				_strConfig.append("4-8,");
				demPhao++;
				break;
			case 4://ma
				_strConfig.append("3-2,");
				demMa++;
				break;
			case 5:
				_strConfig.append("3-8,");
				demMa++;
				break;
			case 6://tuong
				_strConfig.append("2-3,");
				demTuongj++;
				break;
			case 7:
				_strConfig.append("2-7,");
				demTuongj++;
				break;
			case 8://si
				_strConfig.append("1-4,");
				demSi++;
				break;
			case 9:
				_strConfig.append("1-6,");
				demSi++;
				break;
				//tot
			case 10://tot
				_strConfig.append("0-1,");
				demTot++;
				break;
			case 11:
				_strConfig.append("0-3,");
				demTot++;
				break;
			case 12:
				_strConfig.append("0-5,");
				demTot++;
				break;
			case 13:
				_strConfig.append("0-7,");
				demTot++;
				break;
			case 14:
				_strConfig.append("0-9,");
				demTot++;
				break;
			default:
				break;
			}
		}
	}
	if (dem == 0)
		_strConfig = "0-0";
	else{
		//remove last character is ","
		size_t f = _strConfig.find_last_of(',');
		if (f != std::string::npos)
		{
			_strConfig.erase(_strConfig.begin() + (_strConfig.length() - 1));
		}

		//generate display string 
		//Tốt	Sĩ	Tượng	Mã	Pháo	Xe
		//	0	1	2		3	4		5
		_strDisplay = StringUtils::format("0-%d,1-%d,2-%d,3-%d,4-%d,5-%d", demTot, demSi, demTuongj, demMa, demPhao, demXe);
	}
}

//update 12/7
void LayerPopupChapQuan::loadFromConfig(string configStr)
{
	if (configStr.compare("0-0") == 0)
		return;
	vector<string> arrConfig = mUtils::splitString(configStr, ',');
	for (int i = 0; i < arrConfig.size(); i++)
	{
		string config = arrConfig[i];
		int idxChess = -1;
		if (config.compare("5-1") == 0)
		{
			this->cbXe1->setSelected(false);
			idxChess = 0;
		}
		else if (config.compare("5-9") == 0)
		{
			this->cbXe2->setSelected(false);
			idxChess = 1;
		}
		else if (config.compare("4-2") == 0)
		{
			this->cbPhao1->setSelected(false);
			idxChess = 2;
		}
		else if (config.compare("4-8") == 0)
		{
			this->cbPhao2->setSelected(false);
			idxChess = 3;
		}
		else if (config.compare("3-2") == 0)
		{
			this->cbMa1->setSelected(false);
			idxChess = 4;
		}
		else if (config.compare("3-8") == 0)
		{
			this->cbMa2->setSelected(false);
			idxChess = 5;
		}
		else if (config.compare("2-3") == 0)
		{
			this->cbTuong1->setSelected(false);
			idxChess = 6;
		}
		else if (config.compare("2-7") == 0)
		{
			this->cbTuong2->setSelected(false);
			idxChess = 7;
		}
		else if (config.compare("1-4") == 0)
		{
			this->cbSi1->setSelected(false);
			idxChess = 8;
		}
		else if (config.compare("1-6") == 0)
		{
			this->cbSi2->setSelected(false);
			idxChess = 9;
		}
		else if (config.compare("0-1") == 0)
		{
			this->cbTot1->setSelected(false);
			idxChess = 10;
		}
		else if (config.compare("0-3") == 0)
		{
			this->cbTot2->setSelected(false);
			idxChess = 11;
		}
		else if (config.compare("0-5") == 0)
		{
			this->cbTot3->setSelected(false);
			idxChess = 12;
		}
		else if (config.compare("0-7") == 0)
		{
			this->cbTot4->setSelected(false);
			idxChess = 13;
		}
		else if (config.compare("0-9") == 0)
		{
			this->cbTot5->setSelected(false);
			idxChess = 14;
		}
		this->arrChess[idxChess]->setVisible(false);
	}
}


