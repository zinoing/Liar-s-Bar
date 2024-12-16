#include "GameManager.h"

mutex GameManager::uiMutex;

Player* GameManager::getMe()
{
	return me;
}

void GameManager::setMe(string id)
{
	Player* me = new Player(id);
	this->me = me;
}

void GameManager::addOtherPlayer(string id)
{
	Player* otherPlayer = new Player(id);
	otherPlayers.push_back(otherPlayer);
}

Player* GameManager::getOtherPlayerStatus(string id)
{
	for (auto player : otherPlayers) {
		if (player->getId() == id) {
			return player;
		}
	}
	return nullptr;
}

void GameManager::setOtherPlayerStatus(string id, int numOfCardsOnOtherPlayerHand)
{
	for (auto player : otherPlayers) {
		if (player->getId() == id) {
			if (player->getNumOfCardsOnHand() > numOfCardsOnOtherPlayerHand) {
				cout << "[" << id << "] discarded " << player->getNumOfCardsOnHand() - numOfCardsOnOtherPlayerHand << "cards from hand.\n";
				player->setNumOfCards(numOfCardsOnOtherPlayerHand);
			}
		}
	}
	return;
}

void GameManager::myTurn()
{
	showUI();
	cout << "My turn\n";

	threadData threadData;

	thread timerThread([this, &threadData]() {
		this->countTime(&threadData);
		});	timerThread.detach();

	vector<Card> cardsToDiscard = me->chooseCardsToDiscard(threadData.remainingTime);
	me->discardCards(cardsToDiscard);

	NetworkManager* networkManager = NetworkManager::getInstance();
	PacketManager* packetManager = PacketManager::getInstance();
	LobbyManager* lobbyManager = LobbyManager::getInstance();

	PacketBuffer pb;
	pb.writeString(me->getId());
	pb.writeString(lobbyManager->getNameOfRoom());
	pb.writeCards(cardsToDiscard);

	networkManager->sendMessage(packetManager->serializePacket(ClientPacketType::PlAYER_TURN_OVER, pb));

	threadData.remainingTime = 0;
	threadData.turnOver = true;
	showUI();

	cout << "Turn over\n";
}

void GameManager::showUI()
{
	system("cls");
	drawOtherPlayersStatus();
	drawMyCard();
}

void GameManager::drawOtherPlayersStatus()
{
	for (auto otherPlayer : otherPlayers) {
		cout << "[" << otherPlayer->getId() << "] has " << otherPlayer->getNumOfCardsOnHand() << " cards on hand\n";
	}
}

void GameManager::drawMyCard()
{
	vector<Card> cards = me->getCards();

	for (int i = 0; i < me->getNumOfCardsOnHand(); ++i) {
		cards[i].draw(12 * i, 5);
	}
}

void GameManager::countTime(threadData* threadData)
{

	clock_t begin;
	double time_spent;
	unsigned int i;

	/* Mark beginning time */
	begin = clock();
	int lastTimeUpdate = 0;
	while (!threadData->turnOver)
	{
		/* Get CPU time since loop started */
		time_spent = (double)(clock() - begin) / CLOCKS_PER_SEC;
		if (time_spent >= TIME_LIMIT_PER_TURN)
			break;

		threadData->remainingTime = (int)(TIME_LIMIT_PER_TURN - time_spent);
		if (threadData->remainingTime != lastTimeUpdate)
		{
			lock_guard<mutex> lock(uiMutex);
			gotoxy(50, 0);
			// 시간 갱신 시 공백으로 덮어쓰고 출력
			cout << "             ";  // 이전 텍스트 지우기
			gotoxy(50, 0);
			cout << "time left: " << threadData->remainingTime;
			lastTimeUpdate = threadData->remainingTime;  // 마지막 갱신 시간을 현재 시간으로 설정
		}

		gotoxy(0, 15);
	}
	gotoxy(0, 15);
}
