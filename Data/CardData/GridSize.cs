using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WPP.Data.CardData
{
    public enum GridSizeType
    {
        unit_default,
        building_default
    }

    [Serializable]
    public class GridSize
    {
        public GridSizeType type;
        public int top;
        public int down;
        public int left;
        public int right;

        public GridSize TroopGrid()
        {
            GridSize gridSize = new GridSize();
            type = GridSizeType.unit_default;
            gridSize.top = 0;
            gridSize.down = 0;
            gridSize.left = 0;
            gridSize.right = 0;
            return gridSize;
        }

        public GridSize BuildingGrid()
        {
            GridSize gridSize = new GridSize();
            type = GridSizeType.building_default;
            gridSize.top = 1;
            gridSize.down = 1;
            gridSize.left = 1;
            gridSize.right = 1;
            return gridSize;
        }
    }
}
