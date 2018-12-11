#include "CardInfo.h"
#include "mUtils.h"
CardInfo::CardInfo()
{

}

CardInfo::CardInfo(int id, int value, int substance)
{
	this->id = id;
	this->value = value;
	this->substance = substance;
	this->turnup = false;
}

CardInfo::CardInfo(int id, int value, int substance, bool turnup)
{
	this->id = id;
	this->value = value;
	this->substance = substance;
	this->turnup = turnup;
}

int CardInfo::compareTo(CardInfo *card)
{
	if (this->value < card->getValueCard())
		return -1;
	else if (this->value == card->getValueCard()){
		if (this->substance < card->getSubstanceCard())
			return -1;
		else if (substance == card->getSubstanceCard())
			return 0;
		else
			return 1;
	}
	else
		return 1;
}

std::string CardInfo::getTypeCardName(int type)
{
	switch (type)
	{
	case CARD_SUBSTANCE_BICH:
		return "Bich";
	case CARD_SUBSTANCE_TEP:
		return "Tep";
	case CARD_SUBSTANCE_HEART:
		return "co";
	case CARD_SUBSTANCE_DIAMOND:
		return "Do";
	default:
		return "";
	}
}

std::string CardInfo::getCardName(int num)
{
	if (num > 0 && num < 11)
		return StringUtils::format("%d", num);
	switch (num)
	{
	case 11:
		return "J";
	case 12:
		return "Q";
	case 13:
		return "K";
	case 14:
		return "A";
	case 15:
		return "2";
	default:
		return "";
	}
}

