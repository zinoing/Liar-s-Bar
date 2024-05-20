using MySql.Data.MySqlClient;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Data;
using System.Numerics;
using System.Runtime.CompilerServices;
using WPP.AI.STAT;
using WPP.Data.CardData;
using WPP.Data.ClientInfo.Account;
using WPP.Data.ClientInfo.Deck;
using WPP.Data.ClientInfo.Tower;
using WPP.Data.Collection;
using WPP.LogData;
using WPP.Protocol.Server;

namespace WPP.Data
{
    class DatabaseManager : Singleton<DatabaseManager>
    {
        private MySqlConnection _mySqlConnection;

        private static string _server = "127.0.0.1";
        private static string _server_port = "3306";
        private static string _dbID = "root";
        private static string _db_pw = "0000";
        private static string _db_name = "duelofkingdom";

        private string _strConn = string.Format("server={0};port={1};database={2};uid={3};pwd={4};SSL Mode=None;", _server, _server_port, _db_name, _dbID, _db_pw);

        public void InitializeMySQLServer()
        {
            InitializeMySqlConnection();
            ConnectToMySqlServer();
        }

        private void InitializeMySqlConnection()
        {
            try
            {
                _mySqlConnection = new MySqlConnection(_strConn);
            }
            catch (Exception e)
            {
                Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.Message);
                Console.WriteLine(e.Message);
            }
        }

        private void ConnectToMySqlServer()
        {
            try
            {
                _mySqlConnection.Open();
            }
            catch (Exception e)
            {
                Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.Message);
                Console.WriteLine(e.Message);
            }
        }

        private void CloseConnection()
        {
            try
            {
                _mySqlConnection.Close();
            }
            catch (Exception e)
            {
                Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.Message);
                Console.WriteLine(e.Message);
            }
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public int FindAcountID(string username)
        {
            string storedProcedureName = "GetAccount";
            MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection);
            cmd.CommandType = CommandType.StoredProcedure;

            cmd.Parameters.AddWithValue("@in_username", username);

            MySqlDataReader reader = cmd.ExecuteReader();
            if (reader.Read())
            {
                int id = Convert.ToInt32(reader["account_id"]);
                reader.Close();
                return id;
            }
            reader.Close();
            return -1;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public int FindTowerID(string towerName, int Level)
        {
            string storedProcedureName = "GetTowerID";
            MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection);
            cmd.CommandType = CommandType.StoredProcedure;

            cmd.Parameters.AddWithValue("@in_name", towerName);
            cmd.Parameters.AddWithValue("@in_level", Level);

            MySqlDataReader reader = cmd.ExecuteReader();

            if (reader.Read())
            {
                int towerID = Convert.ToInt32(reader["building_id"]);
                reader.Close();
                return towerID;
            }
            reader.Close();
            return -1;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public Tower FindTower(int towerID)
        {
            string storedProcedureName = "GetTower";
            MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection);
            cmd.CommandType = CommandType.StoredProcedure;

            cmd.Parameters.AddWithValue("@in_tower_id", towerID);

            MySqlDataReader reader = cmd.ExecuteReader();

            if (reader.Read())
            {
                int building_id = Convert.ToInt32(reader["building_id"]);
                string name = (string)reader["name"];
                int Level = Convert.ToInt32(reader["Level"]);
                float hitpoints = Convert.ToInt32(reader["hitpoints"]);
                float damage = Convert.ToInt32(reader["damage"]);
                float hit_speed = Convert.ToInt32(reader["hit_speed"]);
                float range = Convert.ToInt32(reader["range"]);
                float captureRange = Convert.ToInt32(reader["capture_range"]);

                string captureTagString = (string)reader["capture_tag"];
                List<CaptureTag> targetTag = new List<CaptureTag>();
                var captureTagArray = JArray.Parse(captureTagString);
                for (int i = 0; i < captureTagArray.Count; i++)
                {
                    JToken token = captureTagArray[i];
                    if (String.Equals((string)token, CaptureTag.ground_unit.ToString()))
                    {
                        targetTag.Add(CaptureTag.ground_unit);
                        continue;
                    }
                    if (String.Equals((string)token, CaptureTag.air_unit.ToString()))
                    {
                        targetTag.Add(CaptureTag.air_unit);
                        continue;
                    }
                    if (String.Equals((string)token, CaptureTag.building.ToString()))
                    {
                        targetTag.Add(CaptureTag.building);
                        continue;
                    }
                }

                AttackBuildingStat towerUnit = new AttackBuildingStat(building_id, Level, name, hitpoints, targetTag, damage, hit_speed, range, captureRange);
                Tower tower = new Tower(towerUnit);

                reader.Close();
                return tower;
            }
            reader.Close();
            return null;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public Card FindCard(int cardID)
        {
            string storedProcedureName = "GetCard";

            using (MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection))
            {
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.AddWithValue("@in_card_id", cardID);

                using (MySqlDataReader reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        int? troopID = reader["troop_id"] is DBNull ? (int?)null : Convert.ToInt32(reader["troop_id"]);
                        int? buildingID = reader["building_id"] is DBNull ? (int?)null : Convert.ToInt32(reader["building_id"]);
                        int? spellID = reader["spell_id"] is DBNull ? (int?)null : Convert.ToInt32(reader["spell_id"]);

                        CardType type;
                        int unitID;
                        if (troopID != null)
                        {
                            type = CardType.troop;
                            unitID = (int)troopID;
                        }
                        else if (buildingID != null)
                        {
                            type = CardType.building;
                            unitID = (int)buildingID;
                        }
                        else if (spellID != null)
                        {
                            type = CardType.spell;
                            unitID = (int)spellID;
                        }
                        else
                        {
                            type = 0;
                            unitID = 0;
                        }

                        Enum.TryParse<CardRarity>((string)reader["rarity"], out CardRarity rarity);
                        int needElixir = Convert.ToInt32(reader["needElixir"]);
                        float duration = Convert.ToSingle(reader["duration"]);
                        float radius = Convert.ToSingle(reader["radius"]);

                        Enum.TryParse<GridSizeType>((string)reader["grid_size_type"], out GridSizeType gridSizeType);
                        Enum.TryParse<SpawnType>((string)reader["spawn_type"], out SpawnType spawnType);
                        reader.Close();

                        GridSize gridSize = FindGridSize(gridSizeType);
                        SpawnData spawnData = FindSpawnData(spawnType);

                        Card card = new Card(cardID, unitID, type, rarity, needElixir, duration, radius, gridSize, spawnData);
                        return card;
                    }
                }
            }

            return null;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public BaseStat FindUnit(CardType type, int unitID)
        {
            string tableName;
            string idName;

            // selecting table name
            switch (type)
            {
                case CardType.troop:
                    tableName = "troop";
                    idName = "troop_id";
                    break;
                case CardType.building:
                    tableName = "building";
                    idName = "building_id";
                    break;
                case CardType.spell:
                    tableName = "spell";
                    idName = "spell_id";
                    break;
                default:
                    tableName = "";
                    idName = "";
                    break;
            }
            string query = null;
            // building은 조인을 필요로 함
            if (String.Equals("building", tableName))
            {
                query = "SELECT * FROM building LEFT JOIN attack_building ON building.attack_building_id = attack_building.attack_building_id LEFT JOIN spawn_building ON building.spawn_building_id = spawn_building.spawn_building_id WHERE building_id = '" + unitID + "'";

            }
            else
            {
                query = "SELECT * FROM " + tableName + " WHERE " + idName + " = '" + unitID + "'";
            }
            MySqlCommand cmd = new MySqlCommand(query, _mySqlConnection);

            MySqlDataReader reader = cmd.ExecuteReader();

            if (reader.Read())
            {
                string name = null;
                for (int i = 0; i < reader.FieldCount; i++)
                {
                    if (reader.GetName(i) == "name" && !reader.IsDBNull(i))
                    {
                        name = (string)reader[i];
                        break;
                    }
                }
                int level = 0;
                for (int i = 0; i < reader.FieldCount; i++)
                {
                    if (reader.GetName(i) == "level" && !reader.IsDBNull(i))
                    {
                        level = Convert.ToInt32(reader[i]);
                        break;
                    }
                }

                BaseStat unit = null;

                switch (type)
                {
                    case CardType.troop:
                        float hp = Convert.ToInt32(reader["hitpoints"]);
                        float damage = Convert.ToSingle(reader["damage"]);
                        float hit_speed = Convert.ToSingle(reader["hit_speed"]);
                        float move_speed = Convert.ToSingle(reader["move_speed"]);
                        float rotation_speed = Convert.ToSingle(reader["rotation_speed"]);
                        float range = Convert.ToSingle(reader["range"]);
                        float capture_range = Convert.ToSingle(reader["capture_range"]);
                        string troopCaptureTagString = (string)reader["capture_tag"];

                        var troopCaptureTagArray = JArray.Parse(troopCaptureTagString);
                        List<CaptureTag> targetTag = new List<CaptureTag>();

                        for (int i = 0; i < troopCaptureTagArray.Count; i++)
                        {
                            JToken token = troopCaptureTagArray[i];
                            if (String.Equals((string)token, CaptureTag.ground_unit.ToString()))
                            {
                                targetTag.Add(CaptureTag.ground_unit);
                                continue;
                            }
                            if (String.Equals((string)token, CaptureTag.air_unit.ToString()))
                            {
                                targetTag.Add(CaptureTag.air_unit);
                                continue;
                            }
                            if (String.Equals((string)token, CaptureTag.building.ToString()))
                            {
                                targetTag.Add(CaptureTag.building);
                                continue;
                            }
                        }
                        UnitStat troop = new UnitStat(unitID, level, name, hp, targetTag, damage, hit_speed, range, capture_range);
                        unit = troop;
                        break;
                    case CardType.building:
                        int? attackbuildingID = reader["attack_building_id"] is DBNull ? (int?)null : Convert.ToInt32(reader["attack_building_id"]);
                        int? spawnbuildingID = reader["spawn_building_id"] is DBNull ? (int?)null : Convert.ToInt32(reader["spawn_building_id"]);

                        float hitpoints = 0.0f;
                        for (int i = 0; i < reader.FieldCount; i++)
                        {
                            if (reader.GetName(i) == "hitpoints" && !reader.IsDBNull(i))
                            {
                                hitpoints = Convert.ToSingle(reader[i]);
                                break;
                            }
                        }
                        float life_time = 0.0f;
                        for (int i = 0; i < reader.FieldCount; i++)
                        {
                            if (reader.GetName(i) == "life_time" && !reader.IsDBNull(i))
                            {
                                life_time = Convert.ToSingle(reader[i]);
                                break;
                            }
                        }
                        if (attackbuildingID != null)
                        {
                            damage = Convert.ToSingle(reader["damage"]);
                            float? nullable_life_time = Convert.ToSingle(reader["life_time"]);
                            hit_speed = Convert.ToSingle(reader["hit_speed"]);
                            range = Convert.ToSingle(reader["range"]);
                            capture_range = Convert.ToSingle(reader["capture_range"]);
                            string attackBuildingCaptureTagString = (string)reader["capture_tag"];
                            targetTag = new List<CaptureTag>();
                            var attackBuildingCaptureTagArray = JArray.Parse(attackBuildingCaptureTagString);

                            for (int i = 0; i < attackBuildingCaptureTagArray.Count; i++)
                            {
                                JToken token = attackBuildingCaptureTagArray[i];
                                if (String.Equals((string)token, CaptureTag.ground_unit.ToString()))
                                {
                                    targetTag.Add(CaptureTag.ground_unit);
                                    continue;
                                }
                                if (String.Equals((string)token, CaptureTag.air_unit.ToString()))
                                {
                                    targetTag.Add(CaptureTag.air_unit);
                                    continue;
                                }
                                if (String.Equals((string)token, CaptureTag.building.ToString()))
                                {
                                    targetTag.Add(CaptureTag.building);
                                    continue;
                                }
                            }
                            AttackBuildingStat attackBuilding = null;
                            if (nullable_life_time != null)
                            {
                                attackBuilding = new LivingOutAttackBuildingStat(unitID, level, name, hitpoints, targetTag, damage, hit_speed, range, capture_range, (float)nullable_life_time);
                            }
                            else
                            {
                                attackBuilding = new AttackBuildingStat(unitID, level, name, hitpoints, targetTag, damage, hit_speed, range, capture_range);
                            }
                            unit = attackBuilding;
                            break;
                        }
                        else if (spawnbuildingID != null)
                        {
                            int spawn_unit_id = Convert.ToInt32(reader["spawn_unit_id"]);
                            int spawn_unit_count = Convert.ToInt32(reader["spawn_unit_count"]);
                            float spawn_delay = Convert.ToSingle(reader["spawn_delay"]);
                            string jsonSpawnOffset = (string)reader["spawn_offset"];

                            var jArray = JArray.Parse(jsonSpawnOffset);
                            List<Vector2> spawnOffset = new List<Vector2>();
                            for (int i = 0; i < jArray.Count; i++)
                            {
                                JToken token = jArray[i];
                                int x = (int)token[0];
                                int y = (int)token[1];
                                spawnOffset.Add(new Vector2(x, y));
                            }
                            LivingOutSpawnBuildingStat spawnBuilding = new LivingOutSpawnBuildingStat(unitID, level, name, hitpoints, life_time, spawn_unit_id, spawn_delay, spawn_unit_count, spawnOffset);
                            unit = spawnBuilding;
                            break;
                        }
                        break;
                    case CardType.spell:
                        float spell_damage = Convert.ToSingle(reader["damage"]);
                        float delay = Convert.ToSingle(reader["delay"]);
                        range = Convert.ToSingle(reader["range"]);
                        float speed = Convert.ToSingle(reader["speed"]);
                        ProjectileMagicStat spell = new ProjectileMagicStat(unitID, level, name, range, delay, spell_damage, speed);
                        unit = spell;
                        break;
                    default:
                        unit = null;
                        break;
                }

                reader.Close();
                return unit;
            }
            reader.Close();
            return null;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public GridSize FindGridSize(GridSizeType type)
        {
            string query = "SELECT * FROM grid_size WHERE grid_size_type = '" + type.ToString() + "'";
            MySqlCommand cmd = new MySqlCommand(query, _mySqlConnection);

            MySqlDataReader reader = cmd.ExecuteReader();

            if (reader.Read())
            {
                GridSize gridSize = new GridSize();
                gridSize.type = type;
                gridSize.top = Convert.ToInt32(reader["top"]);
                gridSize.down = Convert.ToInt32(reader["down"]);
                gridSize.left = Convert.ToInt32(reader["left"]);
                gridSize.right = Convert.ToInt32(reader["right"]);

                reader.Close();
                return gridSize;
            }
            reader.Close();
            return null;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public SpawnData FindSpawnData(SpawnType type)
        {
            string query = "SELECT * FROM spawn WHERE spawn_type = '" + type.ToString() + "'";
            MySqlCommand cmd = new MySqlCommand(query, _mySqlConnection);

            MySqlDataReader reader = cmd.ExecuteReader();

            if (reader.Read())
            {
                string jsonSpawnOffset = (string)reader["spawn_offset"];

                var jArray = JArray.Parse(jsonSpawnOffset);
                SpawnData spawnData = new SpawnData(jArray.Count);
                spawnData.type = type;
                for (int i = 0; i < jArray.Count; i++)
                {
                    JToken token = jArray[i];
                    int x = (int)token[0];
                    int y = (int)token[1];
                    spawnData.spawnOffset[i] = new SerializableVector2(x, y);
                }

                reader.Close();
                return spawnData;
            }
            reader.Close();
            return null;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public void AddAccount(string username, string Password)
        {
            string storedProcedureName = "AddAccount";

            using (MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection))
            {
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.AddWithValue("@in_username", username);
                cmd.Parameters.AddWithValue("@in_Password", Password);

                try
                {
                    cmd.ExecuteNonQuery();
                }
                catch (Exception e)
                {
                    Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.Message);
                    Console.WriteLine(e.Message);
                }
            }
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public void AddTowers(string username)
        {
            int accountID = FindAcountID(username);
            int king_towerID = 0;
            int princessID = 0;
            for (int i = 0; i < 2; i++)
            {
                switch (i)
                {
                    case 0:
                        king_towerID = FindTowerID("king_tower", 1);
                        break;
                    case 1:
                        princessID = FindTowerID("princess_tower", 1);
                        break;
                    default:
                        break;
                }
            }


            string storedProcedureName = "AddTowers";

            using (MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection))
            {
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.AddWithValue("@in_account_id", accountID);
                cmd.Parameters.AddWithValue("@in_king_tower_id", king_towerID);
                cmd.Parameters.AddWithValue("@in_princess_tower_id", princessID);

                try
                {
                    cmd.ExecuteNonQuery();
                }
                catch (Exception e)
                {
                    Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.Message);
                    Console.WriteLine(e.Message);
                }
            }
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public void AddCards(string username)
        {
            int accountID = FindAcountID(username);

            for (int i = 0; i < Constants.MAXIMUM_INITIAL_CARD_COUNT; i++)
            {
                /*string query = "INSERT INTO card_instance (account_id, card_id) VALUES('" +
                            accountID + "','" +
                            (CARD_ID.card_ids[i]) + "')";

                try
                {
                    MySqlCommand cmd = new MySqlCommand(query, _mySqlConnection);
                    cmd.ExecuteNonQuery();
                }
                catch (Exception e)
                {
                    Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.Message);
                    Console.WriteLine(e.Message);
                }*/
                for (int j = 0; j < 10; j++)
                {
                    string query = "INSERT INTO card_instance (account_id, card_id) VALUES('" +
                            accountID + "','" +
                            (CARD_ID.card_ids[i] + j) + "')";

                    try
                    {
                        MySqlCommand cmd = new MySqlCommand(query, _mySqlConnection);
                        cmd.ExecuteNonQuery();
                    }
                    catch (Exception e)
                    {
                        Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.Message);
                        Console.WriteLine(e.Message);
                    }
                }
            }
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public void AddDecks(string username)
        {
            int accountID = FindAcountID(username);

            string storedProcedureName = "AddDeck";

            MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection);

            for (int i = 0; i < Constants.MAXIMUM_DECKS; i++)
            {
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Clear();
                cmd.Parameters.AddWithValue("@in_account_id", accountID);
                cmd.Parameters.AddWithValue("@in_deck_id", (i + 1));

                for (int j = 1; j <= Constants.MAXIMUM_CARDS_IN_DECK; j++)
                {
                    cmd.Parameters.AddWithValue("@in_card_id_" + j, CARD_ID.card_ids[j - 1]);
                    //cmd.Parameters.AddWithValue("@in_card_id_" + j, DBNull.Value);
                }
                try
                {
                    cmd.ExecuteNonQuery();
                }
                catch (Exception e)
                {
                    Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.Message);
                    Console.WriteLine(e.Message);
                }
            }
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public ClientAccount LoadAccount(int clientID, string username)
        {
            string storedProcedureName = "GetAccount";
            MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection);
            cmd.CommandType = CommandType.StoredProcedure;

            cmd.Parameters.AddWithValue("@in_username", username);

            MySqlDataReader reader = cmd.ExecuteReader();

            if (reader.Read())
            {
                string password = Convert.ToString(reader["Password"]);
                int gold = Convert.ToInt32(reader["Gold"]);
                int level = Convert.ToInt32(reader["Level"]);
                int exp = Convert.ToInt32(reader["Exp"]);
                int trophy = Convert.ToInt32(reader["trophy"]);

                ServerTCP.Instance.Clients[clientID].AccountInfo.Username = username;
                ServerTCP.Instance.Clients[clientID].AccountInfo.Password = password;
                ServerTCP.Instance.Clients[clientID].AccountInfo.Gold = gold;
                ServerTCP.Instance.Clients[clientID].AccountInfo.Level = level;
                ServerTCP.Instance.Clients[clientID].AccountInfo.Exp = exp;
                ServerTCP.Instance.Clients[clientID].AccountInfo.Trophy = trophy;
            }

            reader.Close();
            return ServerTCP.Instance.Clients[clientID].AccountInfo;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public Towers LoadTowers(int clientID, string username)
        {
            int id = FindAcountID(username);

            string storedProcedureName = "GetTowers";
            MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection);
            cmd.CommandType = CommandType.StoredProcedure;

            cmd.Parameters.AddWithValue("@in_account_id", id);

            MySqlDataReader reader = cmd.ExecuteReader();

            for (int i = 0; i < 2; i++)
            {
                if (reader.Read())
                {
                    int building_id = Convert.ToInt32(reader["building_id"]);
                    string name = (string)reader["name"];
                    int Level = Convert.ToInt32(reader["Level"]);
                    float hitpoints = Convert.ToInt32(reader["hitpoints"]);
                    float damage = Convert.ToInt32(reader["damage"]);
                    float hit_speed = Convert.ToInt32(reader["hit_speed"]);
                    float range = Convert.ToInt32(reader["range"]);
                    float capture_range = Convert.ToInt32(reader["capture_range"]);
                    string captureTagString = (string)reader["capture_tag"];
                    var captureTagArray = JArray.Parse(captureTagString);
                    List<CaptureTag> targetTag = new List<CaptureTag>();
                    for (int j = 0; j < captureTagArray.Count; j++)
                    {
                        JToken token = captureTagArray[j];
                        if (String.Equals((string)token, CaptureTag.ground_unit.ToString()))
                        {
                            targetTag.Add(CaptureTag.ground_unit);
                            continue;
                        }
                        if (String.Equals((string)token, CaptureTag.air_unit.ToString()))
                        {
                            targetTag.Add(CaptureTag.air_unit);
                            continue;
                        }
                        if (String.Equals((string)token, CaptureTag.building.ToString()))
                        {
                            targetTag.Add(CaptureTag.building);
                            continue;
                        }
                    }
                    AttackBuildingStat towerUnit = new AttackBuildingStat(building_id, Level, name, hitpoints, targetTag, damage, hit_speed, range, capture_range);
                    Tower tower = new Tower(towerUnit);

                    switch (i)
                    {
                        case 0:
                            ServerTCP.Instance.Clients[clientID].Towers.kingTower = tower;
                            break;
                        case 1:
                            ServerTCP.Instance.Clients[clientID].Towers.leftPrincessTower = tower;
                            ServerTCP.Instance.Clients[clientID].Towers.rightPrincessTower = tower;
                            break;
                        default:
                            break;
                    }
                }
            }
            reader.Close();
            return ServerTCP.Instance.Clients[clientID].Towers;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public Decks LoadDecks(int clientID, string username)
        {
            int id = FindAcountID(username);

            Decks decks = new Decks();
            int?[] cards_ids = new int?[Constants.MAXIMUM_CARDS_IN_DECK];
            int deckId = 0;

            for (int i = 0; i < Constants.MAXIMUM_DECKS; i++)
            {
                string query = "SELECT * FROM deck_instance WHERE account_id = '" + id + "' AND deck_id = '" + (i + 1) + "'";
                MySqlCommand cmd = new MySqlCommand(query, _mySqlConnection);

                MySqlDataReader reader = cmd.ExecuteReader();

                if (reader.Read())
                {
                    deckId = Convert.ToInt32(reader["deck_id"]);
                    for (int j = 0; j < cards_ids.Length; j++)
                    {
                        cards_ids[j] = reader["card_id_" + (j + 1)] is DBNull ? (int?)null : Convert.ToInt32(reader["card_id_" + (j + 1)]);
                    }
                }
                reader.Close();

                Deck deck = new Deck(deckId);
                for (int j = 0; j < cards_ids.Length; j++)
                {
                    if (cards_ids[j] == null)
                    {
                        deck.AddCard(new Card());
                    }
                    else
                    {
                        deck.AddCard(FindCard((int)cards_ids[j]));
                    }
                }

                decks.AddDeck(deck);
            }
            ServerTCP.Instance.Clients[clientID].Decks = decks;
            return ServerTCP.Instance.Clients[clientID].Decks;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public Cards LoadCardInstances(int clientID, string username)
        {
            int id = FindAcountID(username);
            int?[] card_ids = new int?[Constants.MAXIMUM_CARDS];
            int index = 0;

            string query = "SELECT * FROM card_instance WHERE account_id = '" + id + "'";
            MySqlCommand cmd = new MySqlCommand(query, _mySqlConnection);

            MySqlDataReader reader = cmd.ExecuteReader();

            while (reader.Read())
            {
                card_ids[index] = reader["card_id"] is DBNull ? (int?)null : Convert.ToInt32(reader["card_id"]);
                ++index;
            }
            reader.Close();

            for (int i = 0; i < index; i++)
            {
                if (card_ids[i] == null)
                    continue;
                ServerTCP.Instance.Clients[clientID].Cards.AddCard(FindCard((int)card_ids[i]));
            }
            return ServerTCP.Instance.Clients[clientID].Cards;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public CardCollection LoadCardCollection()
        {
            CardCollection cardCollection = new CardCollection();

            string query = "SELECT * FROM card";
            MySqlCommand cmd = new MySqlCommand(query, _mySqlConnection);
            MySqlDataReader reader = cmd.ExecuteReader();

            int[] card_ids = new int[Constants.MAXIMUM_CARDS];

            for (int i = 0; i < Constants.MAXIMUM_CARDS; i++)
            {
                if (reader.Read())
                {
                    int cardID = Convert.ToInt32(reader["card_id"]);
                    card_ids[i] = cardID;
                }
            }
            reader.Close();

            for (int i = 0; i < Constants.MAXIMUM_CARDS; i++)
            {
                cardCollection.AddCard(FindCard(card_ids[i]));
            }

            return cardCollection;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public bool CheckUsernameExists(string username)
        {

            string storedProcedureName = "CheckUsernameExists";
            MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection);
            cmd.CommandType = CommandType.StoredProcedure;

            cmd.Parameters.AddWithValue("@in_username", username);

            MySqlDataReader reader = cmd.ExecuteReader();
            if (reader.HasRows)
            {
                reader.Close();
                return true;
            }
            reader.Close();
            return false;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public bool CheckAccountExists(string username, string Password)
        {
            string storedProcedureName = "CheckAccountExists";
            MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection);
            cmd.CommandType = CommandType.StoredProcedure;

            cmd.Parameters.AddWithValue("@in_username", username);
            cmd.Parameters.AddWithValue("@in_Password", Password);


            MySqlDataReader reader = cmd.ExecuteReader();

            if (reader.HasRows)
            {
                reader.Close();
                return true;
            }
            reader.Close();
            return false;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public void SetAccount(ClientAccount account)
        {
            string storedProcedureName = "SetAccount";

            using (MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection))
            {
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.AddWithValue("@in_username", account.Username);
                cmd.Parameters.AddWithValue("@in_gold", account.Gold);
                cmd.Parameters.AddWithValue("@in_level", account.Level);
                cmd.Parameters.AddWithValue("@in_exp", account.Exp);
                cmd.Parameters.AddWithValue("@in_trophy", account.Trophy);

                try
                {
                    cmd.ExecuteNonQuery();
                }
                catch (Exception e)
                {
                    Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.Message);
                    Console.WriteLine(e.Message);
                }
            }
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public void SetDecks(string username, Decks decks)
        {
            int id = FindAcountID(username);

            string storedProcedureName = "SetDeck";

            using (MySqlCommand cmd = new MySqlCommand(storedProcedureName, _mySqlConnection))
            {
                cmd.CommandType = CommandType.StoredProcedure;
                for (int i = 0; i < Constants.MAXIMUM_DECKS; i++)
                {
                    cmd.Parameters.Clear();

                    cmd.Parameters.AddWithValue("@in_account_id", id);
                    cmd.Parameters.AddWithValue("@in_deck_id", decks.decks[i].id);
                    for (int j = 0; j < Constants.MAXIMUM_CARDS_IN_DECK; j++)
                    {
                        Card card = decks.decks[i].cards[j];
                        if (card == null || card.id == 0)
                        {
                            cmd.Parameters.AddWithValue("@in_card_id_" + (j + 1), DBNull.Value);
                        }
                        else
                        {
                            cmd.Parameters.AddWithValue("@in_card_id_" + (j + 1), card.id);
                        }
                    }

                    try
                    {
                        cmd.ExecuteNonQuery();
                    }
                    catch (Exception e)
                    {
                        Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.Message);
                        Console.WriteLine(e.Message);
                    }
                }
            }
        }
    }
}
