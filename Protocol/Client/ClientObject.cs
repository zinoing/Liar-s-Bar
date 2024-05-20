using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using WPP.Data;
using WPP.Data.ClientInfo.Account;
using WPP.Protocol.Client;
using WPP.Data.ClientInfo.Tower;
using WPP.Data.ClientInfo.Deck;
using WPP.Data.CardData;
using System.Net;
using WPP.AI.STAT;

namespace WPP.Protocol.Client
{
    class ClientObject
    {
        public ClientTCP Tcp { get; set; }
        public ClientAccount AccountInfo { get; set; }
        public Towers Towers  { get; set; }
        public Decks Decks  { get; set; }
        public Cards Cards  { get; set; }

        public int GameRoomId { get; set; }

        public ClientObject(Socket socket, int id)
        {
            Tcp = new ClientTCP(socket, id);
            AccountInfo = new ClientAccount();
            Towers = new Towers();
            Decks = new Decks();
            Cards = new Cards();
            GameRoomId = 0;
        }
    }
}
