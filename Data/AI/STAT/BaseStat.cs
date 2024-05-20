using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WPP.AI.STAT
{
    [Serializable]
    public class BaseStat
    {
        public int _id;
        public int _level;
        public string _name;

        public BaseStat(int id, int level, string name)
        {
            _id = id;
            _level = level;
            _name = name;
        }
    }
}
