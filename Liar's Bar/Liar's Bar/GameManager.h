#pragma once

#define TIME_LIMIT_PER_TURN 60
#define MAX_LIMIT_DISCARD_CARDS 3

#include "Singleton.h"
#include "Player.h"
#include "NetworkManager.h"
#include "PacketManager.h"
#include "Utility.h"

class Player;

struct threadData {
	int remainingTime = TIME_LIMIT_PER_TURN;
	bool turnOver = false;
};

class GameManager : public Singleton<GameManager>
{
public:
	static mutex uiMutex;
private:
	Player* me;
	vector<Player*> otherPlayers;

public:
	Player* getMe();
	void setMe(string id);

	void addOtherPlayer(string id);
	Player* getOtherPlayerStatus(string id);
	void setOtherPlayerStatus(string id, int numOfCardsOnOtherPlayerHand);
	void myTurn();
	void showUI();
private:
	void drawOtherPlayersStatus();
	void drawMyCard();
	void countTime(threadData* threadData);
};