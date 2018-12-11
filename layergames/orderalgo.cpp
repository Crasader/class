#include "orderalgo.h"
#include <string.h>

FullCardInfo::FullCardInfo() {}
FullCardInfo::FullCardInfo(int idx, int num, int s)
:index(idx), number(num), suit(s){
	bTaken = false;
}

void FullCardInfo::setTaken(bool bVal) {
	this->bTaken = bVal;
}

bool FullCardInfo::isTaken() {
	return this->bTaken;
}

int FullCardInfo::getIndex() {
	return index;
}

int FullCardInfo::getNumber() {
	return number;
}

int FullCardInfo::getSuit() {
	return suit;
}


CardOrder::CardOrder(){
	bTaken = false;
}


CardDeck * CardDeck::instance_ = new CardDeck();
CardDeck::CardDeck() {
	cardlst = new FullCardInfo*[52];

	int j = 0;

	for (int i = 1; i <= 13; i++, j++)
		cardlst[j] = new FullCardInfo(j, i, 1);

	for (int i = 1; i <= 13; i++, j++)
		cardlst[j] = new FullCardInfo(j, i, 2);

	for (int i = 1; i <= 13; i++, j++)
		cardlst[j] = new FullCardInfo(j, i, 3);

	for (int i = 1; i <= 13; i++, j++)
		cardlst[j] = new FullCardInfo(j, i, 4);
}

CardDeck::~CardDeck() {
	for (int i = 0; i < 52; i++)
		delete cardlst[i];

	delete[] cardlst;
}
CardDeck * CardDeck::instance() {
	return instance_;
}

void CardDeck::release() {
	delete instance_;
	instance_ = 0;
}

FullCardInfo * CardDeck::getCard(int idx) {
	return cardlst[idx];
}

FullCardInfo ** CardDeck::getListFromIdxList(CardOrder *pCards, int count) {
	FullCardInfo ** pList = new FullCardInfo *[count];

	for (int i = 0; i < count; i++) {
		pList[i] = cardlst[pCards[i].index];
		pList[i]->setTaken(pCards[i].bTaken);
	}

	return pList;
}


Argument::Argument(FullCardInfo ** pLst, int c)
:lst(pLst), count(c){}

bool CardOrderAlgo::isMinimum(FullCardInfo **lst, int count) {
	int numOfTaken = 0;
	try{
		for (int i = 0; i < count; i++) {
			if (lst[i] == NULL)
				return false;
			if (lst[i]->isTaken())
				numOfTaken++;
		}
	}
	catch (std::exception e)
	{

	}
	return numOfTaken <= 1;
}

bool  CardOrderAlgo::isMaximum(Argument * seq) {
	for (int i = 0; i < seq->count; i++)
	if (seq->lst[i]->isTaken())
		return true;

	return false;
}

bool CardOrderAlgo::resolve(vector<Argument> * lst, int * mark, Argument * seq) {
	for (int i = 0; i < lst->size(); i++) {
		if (mark[i]) {
			Argument pTmpSeq = (*lst)[i];
			for (int j = 0; j < pTmpSeq.count; j++) {
				for (int k = 0; k < seq->count; k++)
				{
					if (pTmpSeq.lst[j]->getNumber() == seq->lst[k]->getNumber()
						&& pTmpSeq.lst[j]->getSuit() == seq->lst[k]->getSuit())
						return false;
				}
			}
		}
	}

	return true;
}

void CardOrderAlgo::work(vector<Argument> * tl, int * c1, int * d1,
	int * rets, int * retm, int upper, int sequence,
	int * outc, int * outs) {

	if (sequence < tl->size()) {
		bool maximum = false;
		Argument seq = (*tl)[sequence];
		if (resolve(tl, rets, &seq)) {
			rets[sequence] = 1;
			(*c1) += seq.count;
			maximum = isMaximum(&seq);

			if (maximum)
				(*outc) += 1;
		}

		// continue working up
		work(tl, c1, d1, rets,
			retm, upper, sequence + 1, outc, outs);

		if (*d1 == upper)
			return;

		if (rets[sequence]) {
			rets[sequence] = 0;
			(*c1) -= seq.count;
			if (maximum)
				(*outc) -= 1;

			work(tl, c1, d1, rets,
				retm, upper, sequence + 1, outc, outs);
		}
	}
	else {
		// 
		if ((*outc > *outs)
			|| (*c1 > *d1 && *outc == *outs)) {
			*d1 = *c1;
			*outs = *outc;
			for (int i = 0; i < tl->size(); i++)
				retm[i] = rets[i];
		}
	}
}

void CardOrderAlgo::order(CardOrder *pCards, int count) {
	FullCardInfo ** pLst = CardDeck::instance()->getListFromIdxList(pCards, count);

	for (int i = 0; i < count - 1; i++) {
		for (int j = i + 1; j < count; j++) {
			if ((pLst[i]->getSuit() > pLst[j]->getSuit())
				|| (pLst[i]->getSuit() == pLst[j]->getSuit()
				&& pLst[i]->getNumber() > pLst[j]->getNumber())) {
				FullCardInfo * pTmpInfo = pLst[i];
				pLst[i] = pLst[j];
				pLst[j] = pTmpInfo;
			}
		}
	}

	vector<Argument> arr;
	int n1 = 1;
	for (int i = 1; i <= count; i++) {
		if (i < count && (pLst[i]->getNumber() - 1 == pLst[i - 1]->getNumber()
			&& pLst[i]->getSuit() == pLst[i - 1]->getSuit())) {
			n1++;
		}
		else {
			if (n1 >= 3) {
				for (int digest = 3; digest <= n1; digest++){
					for (int idx = i - n1; idx <= i - digest; idx++) {
						FullCardInfo ** lst = new FullCardInfo *[digest];
						for (int j = idx; j < (idx + digest); j++){
							lst[j - idx] = pLst[j];
						}

						if (isMinimum(lst, digest))
							arr.push_back(Argument(lst, digest));
						else
							delete[] lst;
					}
				}
			}

			n1 = 1;
		}
	}

	for (int i = 0; i < count - 1; i++) {
		for (int j = i + 1; j < count; j++) {
			if ((pLst[i]->getNumber() > pLst[j]->getNumber())
				|| (pLst[i]->getNumber() == pLst[j]->getNumber()
				&& pLst[i]->getSuit() > pLst[j]->getSuit())) {
				FullCardInfo * pTmpInfo = pLst[i];
				pLst[i] = pLst[j];
				pLst[j] = pTmpInfo;
			}
		}
	}

	int s1 = 1;
	for (int i = 1; i <= count; i++) {
		if (i < count && pLst[i]->getNumber() == pLst[i - 1]->getNumber())
			s1++;
		else {
			if (s1 >= 3) {

				FullCardInfo **bArr = new FullCardInfo *[s1];
				for (int j = 0; j < s1; j++)
					bArr[j] = pLst[i + j - s1];

				bool bArrMark = true;
				if (isMinimum(bArr, s1)) {
					arr.push_back(Argument(bArr, s1));
				}
				else {
					bArrMark = false;
				}

				if (s1 > 3) {
					FullCardInfo **sArr = new FullCardInfo *[s1 - 1];
					memcpy(sArr, bArr, sizeof(FullCardInfo*)* 3);

					if (isMinimum(sArr, 3))
						arr.push_back(Argument(sArr, 3));
					else
						delete[] sArr;

					FullCardInfo **dArr = new FullCardInfo *[s1 - 1];
					memcpy(dArr, bArr, sizeof(FullCardInfo*)* 2);
					memcpy(dArr + 2, bArr + 3, sizeof(FullCardInfo*));

					if (isMinimum(dArr, 3))
						arr.push_back(Argument(dArr, 3));
					else
						delete[] dArr;

					dArr = new FullCardInfo *[s1 - 1];
					memcpy(dArr, bArr, sizeof(FullCardInfo*));
					memcpy(dArr + 1, bArr + 2, sizeof(FullCardInfo*)* 2);

					if (isMinimum(dArr, 3))
						arr.push_back(Argument(dArr, 3));
					else
						delete[] dArr;

					sArr = new FullCardInfo *[s1 - 1];
					memcpy(sArr, bArr + 1, sizeof(FullCardInfo*)* 3);

					if (isMinimum(sArr, 3))
						arr.push_back(Argument(sArr, 3));
					else
						delete[] sArr;
				}

				if (!bArrMark) {
					delete[] bArr;
				}
			}
			s1 = 1;
		}
	}

	vector<FullCardInfo*> balancelst;
	if (arr.size() > 0) {
		int * rN = new int;
		int * uN = new int;
		int * ru = new int[arr.size()];
		int * bu = new int[arr.size()];
		int * kv = new int;
		int * rv = new int;

		*rN = 0;
		*uN = 0;
		*kv = 0;
		*rv = 0;

		for (int i = 0; i < arr.size(); i++)
			ru[i] = bu[i] = 0;

		work(&arr, rN, uN, ru, bu, count,
			0, kv, rv);

		for (int i = 0; i < arr.size(); i++) {
			if (bu[i]) {
				Argument arrs = arr[i];
				for (int j = 0; j < arrs.count; j++)
					balancelst.push_back(arrs.lst[j]);
			}
		}

		delete rN;
		delete uN;
		delete[] ru;
		delete[] bu;
		delete kv;
		delete rv;
	}

	if (count > balancelst.size()) {
		FullCardInfo** pTmp = new FullCardInfo*[count - balancelst.size()];
		int beginIdx = 0;
		for (int i = 0; i < count; i++) {
			bool bChoice = false;
			for (int j = 0; j < balancelst.size(); j++)
			if (pLst[i]->getNumber() == balancelst[j]->getNumber()
				&& pLst[i]->getSuit() == balancelst[j]->getSuit()) {
				bChoice = true;
				break;
			}

			if (!bChoice)
				pTmp[beginIdx++] = pLst[i];
		}

		for (int i = 0; i < beginIdx; i++)
			balancelst.push_back(pTmp[i]);

		delete[] pTmp;
	}

	for (int i = 0; i < count; i++) {
		pCards[i].index = balancelst[i]->getIndex();
		pCards[i].bTaken = balancelst[i]->isTaken();
	}

	delete[] pLst;

	for (int i = 0; i < arr.size(); i++) {
		Argument arg = arr[i];
		delete[] arg.lst;
	}

	arr.clear();
}

// int main() {
// 	CardDeck::instance();
// 	CardOrderAlgo algo;
// 
// 	// Tao input la 1 chuoi 10 cay bai
// 	CardOrder * pCards = new CardOrder[10];
// 
// 	pCards[0].index = 1;
// 	pCards[0].bTaken = 1;	// đánh dấu là cây bài ăn
// 	pCards[1].index = 2;
// 	pCards[2].index = 3;
// 	pCards[3].index = 14;
// 	pCards[3].bTaken = 1;
// 	pCards[4].index = 28;
// 	pCards[5].index = 12;
// 	pCards[5].bTaken = 0;
// 	pCards[6].index = 41;
// 	pCards[7].index = 40;
// 	pCards[8].index = 6;
// 	pCards[9].index = 27;
// 	pCards[9].bTaken = 1;
// 
// 	printf("Input\n");
// 	for (int i = 0; i < 10; i++)
// 	{
// 		printf("%d:%d;", pCards[i].index, pCards[i].bTaken);
// 	}
// 	printf("\n");
// 
// 	// pCards là input, đồng thời cũng chính là output
// 	// do dai chuoi = 10
// 	algo.order(pCards, 10);
// 
// 	// output
// 	printf("Result\n");
// 	for (int i = 0; i < 10; i++)
// 	{
// 		printf("%d:%d;", pCards[i].index, pCards[i].bTaken);
// 	}
// 	printf("\n");
// }
