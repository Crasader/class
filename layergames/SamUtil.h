/*!
 * \class
 *
 * \brief
 *
 * \author Administrator
 * \date November 2015
 */

#ifndef SamUtil_h__
#define SamUtil_h__
#include "cocos2d.h"
#include "CardInfo.h"
USING_NS_CC;
using namespace std;

class SamUtil{
private:
	static string listOrderCard;
public:

	static bool isTwo(vector<CardInfo*> listc){
		/*
		* Ham check xem co phai la doi ko
		* */
		log("IsTwo");
		if (listc.size() != 2)
			return false;
		if (listc.at(0)->getValueCard() != listc.at(1)->getValueCard()){
			log("CheckTow: First Round - 2 card not equal or subtance not valid");
			return false;
		}
		else
			return true;
	}

	static bool isThree(vector<CardInfo*> listc){
		log("IsThree");
		if (listc.size() != 3)
			return false;
		if (listc.at(0)->getValueCard() == listc.at(1)->getValueCard()
			&& listc.at(1)->getValueCard() == listc.at(2)->getValueCard()){
			log("CheckThree: First Round -3 card equal");
			return true;
		}
		else
			return false;
	}

	static bool isFour(vector<CardInfo*> listc){
		/*
		* Ham check xem co phai tu quy ko
		* */
		if (listc.size() != 4)
			return false;
		if (listc.at(0)->getValueCard() == listc.at(1)->getValueCard()
			&& listc.at(1)->getValueCard() == listc.at(2)->getValueCard()
			&& listc.at(2)->getValueCard() == listc.at(3)->getValueCard()
			){
			log("CheckFour: First Round - 4 card is equal");
			return true;
		}
		else
			return false;
	}

	static bool isSequence(vector<CardInfo*> &listc){
		/*
		* Ham check xem co phai day ko
		* */
		std::sort(listc.begin(), listc.end(), less_than_key());
		if (listc.size() < 3)
			return false;
		bool issequence = true;
		//neu quan cuoi cung la 2
		if (listc.at(listc.size() - 1)->getValueCard() == 15){
			int count = 2;
			//neu quan cuoi cung la At
			if (listc.at(listc.size() - 2)->getValueCard() == 14)
				count = 3;
			//check xem co phai A23
			if (listc.at(0)->getValueCard() == 3){
				for (int i = 0; i < listc.size() - count; ++i){
					if (listc.at(i + 1)->getValueCard() - listc.at(i)->getValueCard() != 1){
						issequence = false;
						break;
					}
				}
				//////doan nay chua hoan thien, can hoi lai????
				if (issequence){
					log("day la 123");
					for (int i = 0; i < count - 1; ++i){
						CardInfo *cinfo = listc.at(listc.size() - 1);
						listc.erase(listc.begin() + listc.size() - 1);
						listc.insert(listc.begin() + i, cinfo);
						// 						//listc.insert(listc.begin(), cinfo, i);
					}
				}
				//setListOrder(reOrderListCard(listc));
				return issequence;
			}
			else
				return false;
		}
		else{
			for (int i = 0; i < listc.size() - 1; ++i){
				if (listc.at(i + 1)->getValueCard() - listc.at(i)->getValueCard() != 1){
					issequence = false;
					break;
				}
			}
		}
		return issequence;
	}

	static bool have4(vector<CardInfo*> listc, int value){
		/*
		* Ham check xem co tu quy trong 1 danh sach quan bai ko
		* */
		int count = 0;
		for (int i = 0; i < listc.size(); ++i){
			if (listc.at(i)->getValueCard() == value)
				count++;
		}
		if (count == 4)
			return true;
		else
			return false;
	}

	static string reOrderListCard(vector<CardInfo*> listcardinfo)
	{
		string listID = "";
		for (int i = 0; i < listcardinfo.size(); i++) {
			if (i != 0) {
				listID += ",";
			}
			ostringstream os;
			log("ID quan bai danh la: %d", listcardinfo.at(i)->getIdCard());
			os << listcardinfo.at(i)->getIdCard();
			listID += os.str();
		}
		return listID;
	}
	static void setListOrder(string list){
		listOrderCard = list;
	}
	static string getListOrder(){
		return listOrderCard;
	}

	static vector<CardInfo*> getSequence(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard){
		//tim sanh tu bo bai voi dau vao la day quan bai da danh tren ban
		vector<CardInfo*> cards;
		int count = priviousplaycard.size();
		if (count < 3)
			return cards;
		if (listc.size() < count)
			return cards;
		for (int i = 0; i < listc.size(); ++i)
			cards.push_back(listc.at(i));
		std::sort(cards.begin(), cards.end(), less_than_key());
		int i = 0;
		while (true)
		{
			if (i >= (cards.size() - 1))
				break;
			if (cards.at(i)->getValueCard() == cards.at(i + 1)->getValueCard())
				cards.erase(cards.begin() + i);
			else
				i++;
		}
		//dao lai vi tri neu co quan 2, hoac ca quan 2 va quan at o dau
		//cards = revertArraySam(cards);
		getCardForSam(cards);
		// 		if (cards.size() < count)
		// 			return cards;
		for (i = 0; i < cards.size() - (count - 1); ++i)
		{
			vector<CardInfo*> sublist;
			for (int j = 0; j < count; j++)
				sublist.push_back(cards.at(i + j));
			if (isSequence(sublist))
			{
				//so sanh tiep 2 quan cuoi cua 2 mang xem co hop le khong do dinh truong hop co quan 2 nen phai check 3 case
				//1,2,3
				std::sort(sublist.begin(), sublist.end(), less_than_key());//sau khi push vao mang sort lai do ham sequence check quan 2 o cuoi
				int indexCompare = getIndexCompare(sublist);
				int indexComparePrivous = getIndexCompare(priviousplaycard);
				if (sublist.at(indexCompare)->getValueCard() > priviousplaycard.at(indexComparePrivous)->getValueCard())
					return sublist;
			}
		}
		cards.clear();
		return cards;
	}

	static bool checkRecommendOne(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard)
	{
		bool res = false;
		for (int i = 0; i < listc.size(); ++i)
		{
			if (listc.at(i)->getValueCard() > priviousplaycard.at(0)->getValueCard())
			{
				res = true;
				break;
			}
		}
		return res;
	}

	static bool checkRecommendTwo(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard)
	{
		bool res = false;
		if (listc.size() < 2)
			return res;
		for (int i = 0, n = listc.size(); i < n; ++i)
		{
			if (i < n - 1)
			{
				if (listc.at(i)->getValueCard() == listc.at(i + 1)->getValueCard())
				{
					if (listc.at(i + 1)->getValueCard() > priviousplaycard.at(1)->getValueCard())
					{
						res = true;
						break;
					}

				}
			}
		}
		return res;
	}

	static int getIndexCompare(vector<CardInfo*> listc)// ham de lay index de so sanh - danh cho cac day co chua quan 2
	{
		int count = listc.size();
		int indexCompare = count - 1;
		if (listc.at(count - 1)->getValueCard() == 15 && listc.at(count - 2)->getValueCard() == 14)
		{
			indexCompare = count - 3;
		}
		else if (listc.at(count - 1)->getValueCard() == 15 && listc.at(count - 2)->getValueCard() != 14){
			indexCompare = count - 2;
		}
		return indexCompare;
	}

	static vector<CardInfo*> getRecommend2(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
	{
		//ham lay danh sach goi y de chan quan 2
		vector<CardInfo*> cards;
		if (chooseCards.size() < 1 || listc.size() < 4)
			return cards;
		/// cuoi cung moi xet den tu quy
		vector<CardInfo*> list4quy = TienLenMNUtil::get4(listc);
		if (list4quy.size() == 4)
		{
			bool exist = TienLenMNUtil::checkValueExistInList(list4quy, chooseCards);
			if (exist)
			{
				cards = TienLenMNUtil::removeListFromList(list4quy, chooseCards);

			}
			return cards;
		}
		return cards;
	}
	static vector<CardInfo*> getRecommendOne(vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
	{
		vector<CardInfo*> cards;
		if (chooseCards.size() == 0)
			return cards;
		if (chooseCards.size() != priviousplaycard.size())
			return cards;
		if (priviousplaycard.size() > 0)
		{
			if (chooseCards.at(0)->getValueCard() > priviousplaycard.at(0)->getValueCard())
			{
				cards.push_back(chooseCards.at(0));
			}
		}
		else{
			cards.push_back(chooseCards.at(0));
		}

		return cards;
	}
	static vector<CardInfo*> getRecommendTwo(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
	{
		vector<CardInfo*> cards;
		if (chooseCards.size() == 0)
			return cards;
		vector<CardInfo*> sublist;
		sublist.push_back(chooseCards.at(0));
		for (int i = 0; i < listc.size(); i++)
		{
			if (chooseCards.at(0)->getIdCard() != listc.at(i)->getIdCard() &&
				chooseCards.at(0)->getValueCard() == listc.at(i)->getValueCard())
			{
				sublist.push_back(listc.at(i));
				break;
			}
		}
		if (priviousplaycard.size() > 1)
		{
			if (sublist.at(1)->getValueCard() > priviousplaycard.at(1)->getValueCard())
			{
				cards = sublist;
			}
		}
		else
		{
			cards = sublist;
		}
		return cards;
	}
	static vector<CardInfo*> checkBetterSequence(int value, vector<CardInfo*> listc, int count)
	{
		//ham lay 1 sanh trong bo bai voi dau vao la 1 gia tri de so sanh
		vector<CardInfo*> cards;
		if (count < 3)
			return cards;
		if (listc.size() < count)
			return cards;
		for (int i = 0; i < listc.size(); ++i)
			cards.push_back(listc.at(i));
		std::sort(cards.begin(), cards.end(), less_than_key());
		int i = 0;
		while (true)
		{
			if (i >= (cards.size() - 1))
				break;
			if (cards.at(i)->getValueCard() == cards.at(i + 1)->getValueCard())
				cards.erase(cards.begin() + i);
			else
				i++;
		}
		for (i = 0; i < cards.size() - (count - 1); ++i)
		{
			if (cards.at(i)->getValueCard() > value)
			{
				vector<CardInfo*> sublist;
				for (int j = 0; j < count; j++)
					sublist.push_back(cards.at(i + j));
				if (isSequence(sublist))
					return sublist;
			}
		}

		return cards;
	}
	static vector<CardInfo*> getBestSequence(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
	{
		vector<CardInfo*> cards = listc;
		if (chooseCards.size() < 1)
		{
			cards.clear();
			return cards;
		}
		else if (chooseCards.at(0)->getValueCard() == chooseCards.at(1)->getValueCard())
		{
			cards.clear();
			return cards;
		}
		int indexPoint = 0;//danh dau vi tri cua quan bai thu 1 trong mang
		//check quan dau tien cua 2 quan bai duoc chon
		for (int i = 0; i < cards.size(); ++i)
		{
			//chi can lay mang tu quan thu nhat tro len
			if (cards.at(i)->getValueCard() == chooseCards.at(0)->getValueCard())
			{
				indexPoint = i;
				break;
			}
		}
		if (chooseCards.at(1)->getValueCard() == 15 || chooseCards.at(0)->getValueCard() == 15)
			indexPoint = 0;

		int len = priviousplaycard.size();

		int q = 0, h = 0;
		vector<CardInfo*> subcards;//mang chua cac quan bai tu quan chon thu nhat tro di
		for (int i = indexPoint; i < cards.size(); i++)
			subcards.push_back(cards.at(i));
		cards = subcards;
		while (true)
		{
			if (q >= cards.size() - 1)
				break;
			if (cards.at(q)->getValueCard() == cards.at(q + 1)->getValueCard())
			{
				cards.erase(cards.begin() + q);
			}
			else
				q++;
		}
		//cai nay chi danh cho goi y danh lan dau tien
		bool isSpecial = false;
		if (priviousplaycard.size() == 0)
		{
			isSpecial = getCardForSam(cards);
			//set lai value de tien cho viec so sanh tang tien
			if (isSpecial)
			{
				for (int i = 0; i < cards.size(); i++)
				{
					if (cards.at(i)->getValueCard() == 15)
						cards.at(i)->setValueCard(2);
					else if (cards.at(i)->getValueCard() == 14)
						cards.at(i)->setValueCard(1);
				}
			}
			else{
				//neu khong co remove thang 2 o cuoi tranh nham khi check sanh qkat 2
				for (int i = 0; i < cards.size(); i++)
				{
					if (cards.at(i)->getValueCard() == 15)
					{
						cards.erase(cards.begin() + i);
					}
				}
			}
		}
		getCardForSam(cards);

		//so voi quan danh duoi ban
		vector<CardInfo*> sublist;
		//get mang chua day lien tiep co 2 quan bai duoc chon
		while (true)
		{
			if (h >= cards.size() - 1)
			{
				//get last element
				if (cards.size() > 2)
				{
					if (cards.at(h)->getValueCard() == cards.at(h - 1)->getValueCard() + 1)
						sublist.push_back(cards.at(h));
				}
				break;
			}
			if (cards.at(h)->getValueCard() == 15 && cards.at(h + 1)->getValueCard() == 3)
				sublist.push_back(cards.at(h));
			else if (cards.at(h)->getValueCard() + 1 == cards.at(h + 1)->getValueCard())
			{
				sublist.push_back(cards.at(h));
			}
			else{
				//get last element
				if (cards.size() > 2 && h > 0)
				{
					if (cards.at(h)->getValueCard() == cards.at(h - 1)->getValueCard() + 1)
						sublist.push_back(cards.at(h));
				}
				break;
			}
			h++;
		}
		if (sublist.size() < 3)//khong phai la 1 mang check tiep lam gi? hoac la 2 mang khong cung size
		{
			sublist.clear();
			return sublist;
		}
		if (priviousplaycard.size() == 0 && isSpecial)
		{
			//set lai value ve gia tri ban dau
			for (int i = 0; i < cards.size(); i++)
			{
				if (cards.at(i)->getValueCard() == 2)
					cards.at(i)->setValueCard(15);
				else if (cards.at(i)->getValueCard() == 1)
					cards.at(i)->setValueCard(14);
			}
		}
		bool isExist = TienLenMNUtil::checkValueExistInList(sublist, chooseCards);
		if (isExist)
		{
			if (len > 0)
			{
				if (sublist.size() >= len)
				{
					//cat bot cho dung size
					int z = sublist.size();
					for (int i = len; i < z; i++)
					{
						sublist.pop_back();
					}

					//sau khi cat check tiep neu khong chua 2 quan da chon thi khong chap nhan mang nay
					//hoac 2 mang khong cung do dai thi so sanh lam gi?
					bool isOK = TienLenMNUtil::checkValueExistInList(sublist, chooseCards);
					if (!isOK || sublist.size() != len)
					{
						sublist.clear();
						return sublist;
					}
					int indexcompare = getIndexCompare(sublist);
					int indexcomparepre = getIndexCompare(priviousplaycard);

					if (sublist.at(indexcompare)->getValueCard() > priviousplaycard.at(indexcomparepre)->getValueCard())
					{
						//truoc khi lay thi kiem tra xem co 2 quan da chon thi remove
						for (int i = 0; i < sublist.size(); ++i)
						{
							for (int j = 0; j < chooseCards.size(); j++)
							{
								if (sublist.at(i)->getIdCard() == chooseCards.at(j)->getIdCard() ||
									sublist.at(i)->getValueCard() == chooseCards.at(j)->getValueCard())
									sublist.erase(sublist.begin() + i);
							}
						}
						return sublist;
					}
				}
			}
			else{
				//check lai 1 lan nua xem co phai sequence 
					//goi y lan dau tien
					//truoc khi lay thi kiem tra xem co 2 quan da chon thi remove
					if (sublist.size() >= chooseCards.size()){
						std::sort(sublist.begin(), sublist.end(), less_than_key());
						for (int i = 0; i < sublist.size(); ++i)
						{
							for (int j = 0; j < chooseCards.size(); j++)
							{
								if (sublist.at(i)->getIdCard() == chooseCards.at(j)->getIdCard() ||
									sublist.at(i)->getValueCard() == chooseCards.at(j)->getValueCard())
									sublist.erase(sublist.begin() + i);
							}
						}
						return sublist;
					}
				
			}
		}
		cards.clear();
		return cards;
	}
	static bool getCardForSam(vector<CardInfo*> &listc)
	{
		//ham check cho viec co day 1 2 3 hay khong
		bool issequence = false;
		int arr_size = listc.size();
		//neu quan cuoi cung la 2
		if (listc.at(arr_size - 1)->getValueCard() == 15){
			int count = 1;
			//neu quan cuoi cung la At
			if (listc.at(arr_size - 2)->getValueCard() == 14)
				count = 2;
			//check xem co phai A23 nhung cac quan lien ke khac q va k de tranh bo mat sequence qk at
			//revert sequence
			if (listc.at(0)->getValueCard() == 3)
			{
				if (listc.size() > 4)
				{
					if (listc.at(arr_size - 3)->getValueCard() == 13 && listc.at(arr_size - 4)->getValueCard() == 12)
					{
						return issequence;
					}
				}
				issequence = true;
				if (count == 2)
				{
					CardInfo* cinfo1 = listc.at(arr_size - 1);
					CardInfo* cinfo2 = listc.at(arr_size - 2);
					listc.pop_back();
					listc.pop_back();
					listc.insert(listc.begin(), cinfo1);
					listc.insert(listc.begin(), cinfo2);
				}
				else if (count == 1)
				{
					CardInfo* cinfo1 = listc.at(arr_size - 1);
					listc.pop_back();
					listc.insert(listc.begin(), cinfo1);
				}

			}
		}
		return issequence;
	}

	static vector<CardInfo*> revertArraySam(vector<CardInfo*> listc)
	{
		vector<CardInfo*> cards = listc;
		//dao lai vi tri neu co quan 2, hoac ca quan 2 va quan at o dau
		if (cards.at(cards.size() - 1)->getValueCard() == 15
			&& cards.at(cards.size() - 2)->getValueCard() == 14)
		{
			CardInfo* card2 = cards.at(cards.size() - 1);
			CardInfo* cardAt = cards.at(cards.size() - 2);
			cards.pop_back();
			cards.pop_back();
			cards.insert(cards.begin(), cardAt);
			cards.insert(cards.begin() + 1, card2);
		}
		else if (cards.at(cards.size() - 1)->getValueCard() == 15)
		{
			CardInfo* cardinfo = cards.at(cards.size() - 1);
			cards.pop_back();
			cards.insert(cards.begin(), cardinfo);
		}

		return cards;
	}
};
string SamUtil::listOrderCard;
#endif // SamUtil_h__


