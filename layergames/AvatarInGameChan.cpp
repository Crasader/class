#include "AvatarInGameChan.h"
#include "AvatarChan.h"
#include "AllData.h"

AvatarInGameChan::AvatarInGameChan()
{
}


AvatarInGameChan::~AvatarInGameChan()
{

}

bool AvatarInGameChan::init()
{
	if(!Layer::init())
		return false;

	return true;
}

void AvatarInGameChan::SetIcon(const int& pos, char *url)
{
}

void AvatarInGameChan::SetName(const int& pos, string name)
{
}

void AvatarInGameChan::SetMoney(const int& pos, string money)
{
}

void AvatarInGameChan::SetMoney(const int& pos, int money)
{
}

void AvatarInGameChan::SetMoney(const int& pos, double money)
{
}

void AvatarInGameChan::SetReady(const int& pos, bool isReady)
{
}

void AvatarInGameChan::SetVisibleLayerInvite(const int& pos, bool isShow)
{
}

void AvatarInGameChan::UnReadyAllUsers()
{
}

void AvatarInGameChan::ResetGame()
{
	UnReadyAllUsers();
}

void AvatarInGameChan::ResetAll()
{
}

AvatarChan* AvatarInGameChan::GetUserByPos(const int& pos)
{
	return NULL;
}

void AvatarInGameChan::RunTimer(const int& posUser)
{
}

void AvatarInGameChan::RunTimerWithTime(const int& posUser, const float& time)
{
}

void AvatarInGameChan::StopAllTimers()
{
}

void AvatarInGameChan::StopAllBlinkAvatar()
{
}
