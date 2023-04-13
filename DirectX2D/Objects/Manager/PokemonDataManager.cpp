#include "Framework.h"

PokemonDataManager::PokemonDataManager()
{
    t1 = new thread(&PokemonDataManager::LoadPokemonStat, this);
    t2 = new thread(&PokemonDataManager::LoadPokemonSkill, this);
    t3 = new thread(&PokemonDataManager::LoadPokemonMatchType, this);

    t1->join();
    t2->join();
    t3->join();
}

PokemonDataManager::~PokemonDataManager()
{
    delete t1;
    delete t2;
    delete t3;
}

void PokemonDataManager::LoadPokemonStat()
{
    fstream loadFile("TextData/PokemonDataTable.csv");

    string temp;

    char text[256];

    bool isFirstLine = true;

    loadMutex1.lock();

    while (true)
    {
        loadFile >> temp;

        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        vector<string> datas = SplitString(temp, ",");

        PokemonStat data;
        data.pokemonNum = stoi(datas[0]);
        data.pokemonName = datas[1];
        data.type1 = datas[2];
        data.type2 = datas[3];
        data.h = stof(datas[4]);
        data.a = stof(datas[5]);
        data.b = stof(datas[6]);
        data.c = stof(datas[7]);
        data.d = stof(datas[8]);
        data.s = stof(datas[9]);
        data.frontImage = ToWString(datas[10]);
        data.backImage = ToWString(datas[11]);
        data.iconImage = ToWString(datas[12]);

        Audio::Get()->Add(data.pokemonName, datas[13]);

        pokemonStats[data.pokemonNum] = data;

        if (loadFile.eof())
            break;
    }

    loadMutex1.unlock();
}

void PokemonDataManager::LoadPokemonSkill()
{
    fstream loadFile("TextData/PokemonSkillTable.csv");

    string temp;

    char text[256];

    bool isFirstLine = true;

    loadMutex2.lock();

    while (true)
    {
        loadFile >> temp;

        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        vector<string> datas = SplitString(temp, ",");

        PokemonSkill data;
        data.pokemonNum = stoi(datas[0]);

        data.skill1Name = datas[1];
        data.skill1Type = datas[2];
        data.skill1AttackType = datas[3];
        data.skill1Power = stoi(datas[4]);
        data.skill1Acc = stof(datas[5]);
        data.skill1PP = stoi(datas[6]);

        data.skill2Name = datas[7];
        data.skill2Type = datas[8];
        data.skill2AttackType = datas[9];
        data.skill2Power = stoi(datas[10]);
        data.skill2Acc = stof(datas[11]);
        data.skill2PP = stoi(datas[12]);

        data.skill3Name = datas[13];
        data.skill3Type = datas[14];
        data.skill3AttackType = datas[15];
        data.skill3Power = stoi(datas[16]);
        data.skill3Acc = stof(datas[17]);
        data.skill3PP = stoi(datas[18]);

        data.skill4Name = datas[19];
        data.skill4Type = datas[20];
        data.skill4AttackType = datas[21];
        data.skill4Power = stoi(datas[22]);
        data.skill4Acc = stof(datas[23]);
        data.skill4PP = stoi(datas[24]);


        pokemonSkills[data.pokemonNum] = data;

        if (loadFile.eof())
            break;
    }

    loadMutex2.unlock();
}

void PokemonDataManager::LoadPokemonMatchType()
{
    fstream loadFile("TextData/PokemonMatchTypeTable.csv");

    string temp;

    char text[256];

    loadMutex3.lock();

    for (int i = 0; i < MAX_TYPE; i++)
    {
        loadFile >> temp;
        vector<string> datas = SplitString(temp, ",");

        for (int j = 0; j < MAX_TYPE; j++)
        {
            matchTypes[i][j] = stof(datas[j]);
        }
    }

    loadMutex3.unlock();
}

PokemonStat PokemonDataManager::GetPokemonStat(int pokeNum)
{
    return pokemonStats.at(pokeNum);
}

PokemonSkill PokemonDataManager::GetPokemonSkill(int pokeNum)
{
    return pokemonSkills.at(pokeNum);
}

float PokemonDataManager::GetMatchTypeRate(string attackType, string defenseType)
{
    if (defenseType == "NONE")
        return 1.0f;

    int attackIndex = ChangeTypeToIndex(attackType);
    int defenseIndex = ChangeTypeToIndex(defenseType);

    return matchTypes[attackIndex][defenseIndex];
}

int PokemonDataManager::ChangeTypeToIndex(string type)
{
    int index = 0;

    if (type == "노말")
        index = 0;
    else if (type == "불꽃")
        index = 1;
    else if (type == "물")
        index = 2;
    else if (type == "전기")
        index = 3;
    else if (type == "풀")
        index = 4;
    else if (type == "얼음")
        index = 5;
    else if (type == "격투")
        index = 6;
    else if (type == "독")
        index = 7;
    else if (type == "땅")
        index = 8;
    else if (type == "비행")
        index = 9;
    else if (type == "에스퍼")
        index = 10;
    else if (type == "벌레")
        index = 11;
    else if (type == "바위")
        index = 12;
    else if (type == "고스트")
        index = 13;
    else if (type == "드래곤")
        index = 14;
    else if (type == "악")
        index = 15;
    else if (type == "강철")
        index = 16;
    else if (type == "페어리")
        index = 17;

    return index;
}

