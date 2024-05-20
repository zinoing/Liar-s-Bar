using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WPP.Data.ClientInfo.Account
{
    [Serializable]
    class ClientAccount
    {
        public string Username { get; set; }
        public string Password { get; set; }
        public int Gold { get; set; }
        public int Level { get; set; }
        public int Exp {  get; set; }
        public int Trophy {  get; set; }

        public ClientAccount() { }
    }
}
