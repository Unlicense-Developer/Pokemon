#include "Framework.h"
#include "PokemonScene.h"

PokemonScene::PokemonScene()
{
    GameTileMapManager::Get();
    PokemonDataManager::Get();

    encounterGlow = new Quad(L"Textures/EncounterWipe/EncounterGlow.png");
    encounterGlow->Scale() *= 5.0f;
    encounterGlow->SetActive(false);

    encounterWipe = new AnimObject();
    encounterWipe->Scale() *= 5.0f;
    encounterWipe->SetActive(false);
    
    playerCoord = { 5, 5 };

    CreateSounds();
    CreateWipeClips();
}

PokemonScene::~PokemonScene()
{
    GameTileMapManager::Delete();
    PokemonDataManager::Delete();
}

void PokemonScene::Update()
{
   CAMSetting();

   UIManager::Get()->Update();
   EnemyManager::Get()->Update();
   PlayerManager::Get()->Update();
   CheckEncounter();
}

void PokemonScene::Render()
{
   GameTileMapManager::Get()->Render();
   encounterGlow->Render();
   
   PlayerManager::Get()->Render();
   EnemyManager::Get()->Render();
   encounterWipe->Render();
   UIManager::Get()->Render();
}

void PokemonScene::PreRender()
{
    CAM->SetView();
}

void PokemonScene::PostRender()
{
}

void PokemonScene::Start()
{
    PlayerManager::Get()->SetPlayerCoord(playerCoord);
    EnemyManager::Get();
}

void PokemonScene::End()
{
}

void PokemonScene::CAMSetting()
{
    Vector2 leftBottom = Vector2(120.0f, 120.0f);
    Vector2 rightTop = Vector2((GameTileMapManager::Get()->GetTileMap()->GetWidth() - 1) * 80.0f + 40.0f, (GameTileMapManager::Get()->GetTileMap()->GetHeight() - 1) * 80.0f + 40.0f);

    CAM->SetTarget(PlayerManager::Get()->GetPlayer());
    CAM->SetOffset(CENTER_X, CENTER_Y);
    CAM->SetLeftBottom(leftBottom.x, leftBottom.y);
    CAM->SetRightTop(rightTop.x, rightTop.y);
    CAM->SetDeviceSize(WIN_WIDTH, WIN_HEIGHT);

    CAM->Update();
}

void PokemonScene::CreateSounds()
{
    //Effect
    Audio::Get()->Add("OBJ_Bump", "Sounds/OBJ_Bump.mp3");
    Audio::Get()->Add("Jump", "Sounds/Jump.wav");

    //BGM
    Audio::Get()->Add("NewBarkTown", "Sounds/NewBarkTown.mp3", true);
    Audio::Get()->Add("Route29", "Sounds/Route29.mp3", true);
    Audio::Get()->Add("Route30", "Sounds/Route30.mp3", true);
    Audio::Get()->Add("VioletCity", "Sounds/VioletCity.mp3", true);
    Audio::Get()->Add("PokemonGym", "Sounds/PokemonGym.mp3", true);
    Audio::Get()->Add("Wild_Pokemon_Appear", "Sounds/Wild_Pokemon_Appear.mp3");
    Audio::Get()->Add("Trainer_Encounter", "Sounds/Lance_Intro.wav");
}

void PokemonScene::CreateWipeClips()
{
    string path = "Textures/EncounterWipe/";
    string file = "EncounterWipe1.xml";

    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    document->LoadFile((path + file).c_str());

    tinyxml2::XMLElement* atlas = document->FirstChildElement();
    string textureFile = path + atlas->Attribute("imagePath");

    vector<Frame*> frames;
    tinyxml2::XMLElement* sprite = atlas->FirstChildElement();

    while (sprite != nullptr)
    {
        float x, y, w, h;
        x = sprite->FloatAttribute("x");
        y = sprite->FloatAttribute("y");
        w = sprite->FloatAttribute("w");
        h = sprite->FloatAttribute("h");

        frames.push_back(new Frame(ToWString(textureFile), x, y, w, h));

        sprite = sprite->NextSiblingElement();
    }

    Clip* clip = new Clip(frames, false, 2.0f);
    encounterWipe->AddClip("Wipe1", clip);

    frames.clear();

    path = "Textures/EncounterWipe/";
    file = "EncounterWipe2.xml";

    document = new tinyxml2::XMLDocument();
    document->LoadFile((path + file).c_str());

    atlas = document->FirstChildElement();
    textureFile = path + atlas->Attribute("imagePath");

    sprite = atlas->FirstChildElement();

    while (sprite != nullptr)
    {
        float x, y, w, h;
        x = sprite->FloatAttribute("x");
        y = sprite->FloatAttribute("y");
        w = sprite->FloatAttribute("w");
        h = sprite->FloatAttribute("h");

        frames.push_back(new Frame(ToWString(textureFile), x, y, w, h));

        sprite = sprite->NextSiblingElement();
    }

    clip = new Clip(frames, false, 2.0f);
    encounterWipe->AddClip("Wipe2", clip);

    delete document;
}

void PokemonScene::CheckEncounter()
{
    encounterWipe->Update();
    encounterWipe->Pos() = CAM->GlobalPos() + Vector2(CENTER_X, CENTER_Y);
    encounterGlow->Pos() = CAM->GlobalPos() + Vector2( CENTER_X, CENTER_Y);

    if (PlayerManager::Get()->GetPlayer()->GetIsEncounter())
    {
        encounterGlow->SetActive(true);

        if (encounterGlow->GetColor().x >= ENCOUNTER_COLOR.x &&
            encounterGlow->GetColor().y >= ENCOUNTER_COLOR.y)
        {
            valueCount++;
            redValue = -ENCOUNTER_RED_VALUE;
            greenValue = -ENCOUNTER_GREEN_VALUE;
        }
        else if (encounterGlow->GetColor().x <= 0.0f &&
            encounterGlow->GetColor().y <= 0.0f)
        {
            redValue = ENCOUNTER_RED_VALUE;
            greenValue = ENCOUNTER_GREEN_VALUE;
        }

        encounterGlow->GetColor().x += redValue * DELTA ;
        encounterGlow->GetColor().y += greenValue * DELTA;
        
    }
    else
    {
        encounterGlow->GetColor() = ENCOUNTER_COLOR;
    }

    if (valueCount == 4)
    {
        encounterGlow->SetActive(false);
        PlayerManager::Get()->GetPlayer()->SetEncounter(false);
        encounterWipe->SetActive(true);

        int wipePattern = Random(1, 3);

        if(wipePattern == 1)
            encounterWipe->SetClip("Wipe1");
        else if (wipePattern == 2)
            encounterWipe->SetClip("Wipe2");

        valueCount = 0;
    }

    if (encounterWipe->GetCurClip() && !encounterWipe->GetCurClip()->IsPlay() &&
        encounterWipe->Active())
    {
        encounterWipe->GetCurClip() = nullptr;
        encounterWipe->SetActive(false);
        playerCoord = PlayerManager::Get()->GetPlayer()->GetCurCoord();
        SceneManager::Get()->ChangeScene("Battle");
    }


    encounterGlow->UpdateWorld();
    encounterWipe->UpdateWorld();
}
