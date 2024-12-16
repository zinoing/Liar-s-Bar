#include "PacketManager.h"

const char* PacketManager::serializePacket(ClientPacketType type, PacketBuffer buffer)
{
	int size = buffer.getWriteOffset() + sizeof(int) * 2;

	PacketBuffer rtBuffer;
	rtBuffer.writeInt(size);
	rtBuffer.writeType(type);
	rtBuffer.writeBuffer(buffer);

	char* serializedData = new char[DATA_BUF_SIZE];
	memcpy(serializedData, rtBuffer.getData(), DATA_BUF_SIZE);

	return serializedData;
}

void PacketManager::handlePacket(PacketBuffer& packet)
{
	PacketBuffer packetToHandle(packet);
	int size = packetToHandle.readInt();
	packetToHandle.setWriteOffset(size);

	int type = packetToHandle.readInt();

    // This id variable is only read for the readOffset, but not used elsewhere.
    string id = packetToHandle.readString();

	ServerPacketType packetType = static_cast<ServerPacketType>(type);

    switch (packetType) {
    case ServerPacketType::ACCEPT_LOG_IN: {
        GameManager* gameManager = GameManager::getInstance();
        gameManager->setMe(id);

        LoginManager* loginManager = LoginManager::getInstance();
        loginManager->alertLoginResult(true);

        LobbyManager* lobbyManager = LobbyManager::getInstance();
        lobbyManager->showLobby();
        break;
    }
    case ServerPacketType::REJECT_LOG_IN: {
        LoginManager* loginManager = LoginManager::getInstance();
        loginManager->alertLoginResult(false);
        break;
    }
    case ServerPacketType::ACCEPT_REGISTER: {
        LoginManager* loginManager = LoginManager::getInstance();
        loginManager->alertRegisterResult(true);

        LobbyManager* lobbyManager = LobbyManager::getInstance();
        lobbyManager->showLobby();
        break;
    }
    case ServerPacketType::REJECT_REGISTER: {
        LoginManager* loginManager = LoginManager::getInstance();
        loginManager->alertRegisterResult(false);
        break;
    }
    case ServerPacketType::ALLOW_HOST_ROOM: {
        cout << "host completed\n";
        break;
    }
    case ServerPacketType::REJECT_HOST_ROOM: {
        cout << "host failed\n";
        Sleep(2000);
        LobbyManager* lobbyManager = LobbyManager::getInstance();
        lobbyManager->showLobby();
        break;
    }
    case ServerPacketType::ALLOW_ENTER_ROOM: {
        cout << "enter completed\n";
        break;
    }
    case ServerPacketType::REJECT_ENTER_ROOM: {
        cout << "enter failed\n";
        Sleep(2000);
        LobbyManager* lobbyManager = LobbyManager::getInstance();
        lobbyManager->showLobby();
        break;
    }
    case ServerPacketType::ALLOW_PLAY_GAME: {
        int numOfOtherPlayers = packetToHandle.readInt();

        cout << "There are " << numOfOtherPlayers << " other players in the room\n";

        GameManager* gameManager = GameManager::getInstance();

        for (int i = 0; i < numOfOtherPlayers; ++i) {
            string otherPlayerId = packetToHandle.readString();
            gameManager->addOtherPlayer(otherPlayerId);
            cout << "[" << otherPlayerId << "]\n";
        }

        cout << "play game\n";
        break;
    }
    case ServerPacketType::DEAL_CARDS: {
        vector<Card> cards = packetToHandle.readCards();
        cout << "got cards\n";

        for (int i = 0; i < cards.size(); ++i) {
            cout << "받은 카드: " << (int)cards[i].getCardType() << endl;
        }

        GameManager* gameManager = GameManager::getInstance();
        gameManager->getMe()->setCardsFromDealer(cards);
        gameManager->showUI();
        break;
    }
    case ServerPacketType::PLAYER_TURN: {
        GameManager* gameManager = GameManager::getInstance();

        string otherPlayerName = packetToHandle.readString();
        int numOfCardsDiscarded = packetToHandle.readInt();

        int numOfCardsOnOtherPlayerHand = MAX_CARDS_ON_HAND;
        if (!otherPlayerName.empty()) {
            numOfCardsOnOtherPlayerHand = gameManager->getOtherPlayerStatus(otherPlayerName)->getNumOfCardsOnHand() - numOfCardsDiscarded;
        }
        gameManager->setOtherPlayerStatus(otherPlayerName, numOfCardsOnOtherPlayerHand);
        gameManager->myTurn();
        
        break;
    }
    default:
        int errorCode = 9999;
        ErrorLogger::log("handlePacket", errorCode);
        break;
    }
}
