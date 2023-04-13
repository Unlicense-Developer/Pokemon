#include "Framework.h"

Trainer::Trainer()
    :Quad(Vector2(TILE_SIZE,TILE_SIZE))
{
    tag = "Trainer";

    trainer = new AnimObject();
    trainer->Pos() = Pos() + Vector2(0, 20);
    trainer->SetParent(this);
    trainer->Scale() *= 5.0f;

    grassMove = new AnimObject();
    grassMove->Pos() = Pos() + Vector2(0, 20);
    grassMove->SetParent(this);
    grassMove->UpdateWorld();

    shadow = new Quad(L"Textures/Character/Trainer/Shadow.png");
    shadow->SetActive(false);

    battleImage = new Quad(L"Textures/Battle/Player.png");
    battleImage->Scale() *= 5.0f; 
    battleImage->SetActive(false);

    CreateClips();

    trainer->SetClip("IDLE_DOWN");
}

Trainer::~Trainer()
{
    delete trainer;
    delete grassMove;
    delete shadow;
    delete battleImage;
}

void Trainer::Update()
{
    SetDirection();
    UpdateGrassMove();
    CheckTileType();
    SetAnimation();
    Move();
    Jump();

    trainer->Update();
    grassMove->Update();
    UpdateShadowPos();

    UpdateWorld();
}

void Trainer::Render()
{
    shadow->Render();
    trainer->Render();
    grassMove->Render();
}

void Trainer::SetCurCoord(Vector2 coord)
{
    curCoord = coord;
    Pos() = curCoord * 80.0f;
}

void Trainer::CheckTileType()
{
    if (DEST_TILEDATA.type == Tile::OBJ)
    {
        isMove = false;

        if (!Audio::Get()->IsPlaySound("OBJ_Bump"))
            Audio::Get()->Play("OBJ_Bump", 0.5f);
    }
    else if (DEST_TILEDATA.type == Tile::PORTAL)
    {
        if (DEST_TILEDATA.isReverse)
        {
            SetCurCoord(DEST_TILEDATA.destCoord);
            GameTileMapManager::Get()->Load(DEST_TILEDATA.dest);
            destCoord = curCoord + Vector2(0, -1.0f);
        }
        else if (!DEST_TILEDATA.isReverse)
        {
            SetCurCoord(DEST_TILEDATA.destCoord);
            GameTileMapManager::Get()->Load(DEST_TILEDATA.dest);
            isMove = false;
        }

    }
    else if (DEST_TILEDATA.type == Tile::LEDGE)
    {
        isMove = false;

        if (!Audio::Get()->IsPlaySound("OBJ_Bump"))
            Audio::Get()->Play("OBJ_Bump", 0.5f);

    }

}

void Trainer::Move()
{
    if (!isMove) return;
    if (isBattle) return;

    direction = destCoord - curCoord;

    Pos() += Vector2(SPEED, SPEED) * direction * DELTA;

    if (direction.x > 0 && destCoord.x * TILE_SIZE < Pos().x ||
    direction.x < 0 && destCoord.x * TILE_SIZE > Pos().x ||
    direction.y > 0 && destCoord.y * TILE_SIZE < Pos().y ||
    direction.y < 0 && destCoord.y * TILE_SIZE > Pos().y)
    {
        //curCoord = destCoord;
        SetCurCoord(destCoord);
        isMove = false;
        isJumpfall = false;
        jumpTime = 0.0f;
        shadow->SetActive(false);
        
        if (ToString(CUR_TILEDATA.textureFile.substr(CUR_TILEDATA.textureFile.length() - 9)) == "grass.png")
        {
            if (WILD_POKEMON_APPEAR >= Random(0.0f, 1.0f)) // 야생 포켓몬 만남
            {
                Audio::Get()->Stop(GameTileMapManager::Get()->GetCurBGMName());
                Audio::Get()->Play("Wild_Pokemon_Appear", 0.5f);
                BattleManager::Get()->SetBattleType(WILD_BATTLE);
                grassMove->SetActive(false);
                isMove = false;
                isBattle = true;
                isEncounter = true;
            }
            else
            {
                isBattle = false;
            }
        }
    }  
}

void Trainer::Jump()
{
    if (!isMove) return;

    if (jumpTime > JUMP_FALL_TIME)
    {
        isJumpfall = true;
    }

    if (!isJumpfall)
    {
        if (abs(direction.x) == 2.0f)
        {
            jumpTime += DELTA;
            Pos().y += JUMP_POWER * DELTA;
        }
        else if (direction.y == -2.0f)
        {
            jumpTime += DELTA;
            Pos().y += (JUMP_DOWN_POWER - jumpTime * 60.0f )* DELTA;
        }
    }
    else
        Pos().y -= JUMP_POWER * DELTA;
}

void Trainer::UpdateShadowPos()
{
    if (shadow->Active())
    {
        if (direction.y < 0) // down
            shadow->Pos() = { GlobalPos().x, shadow->Pos().y - 295.0f * DELTA};
        else // left, right
            shadow->Pos() = { GlobalPos().x, destCoord.y * TILE_SIZE - shadow->GetSize().y * 0.35f};
    }
    else
        shadow->Pos() = { GlobalPos().x, GlobalPos().y - shadow->GetSize().y * 0.35f };

    shadow->UpdateWorld();
}

void Trainer::UpdateGrassMove()
{
    if (isBattle) return;

    grassMove->Update();

    string grassFile = ToString(GameTileMapManager::Get()->GetTileMap()->GetCoordToTile(destCoord)->GetData().textureFile);

    if (grassFile.substr(grassFile.length() - 9) == "grass.png")
        grassMove->SetActive(true);
    else
        grassMove->SetActive(false);

    if (grassMove->Active())
    {
        if (destCoord.x != curCoord.x ||
           destCoord.y != curCoord.y)
           grassMove->SetClip("GRASS_MOVE");
       else if (destCoord.x == curCoord.x &&
           destCoord.y == curCoord.y)
           grassMove->SetClip("GRASS_IDLE");
    }
}

void Trainer::SetDirection()
{
    if (isBattle) return;
    if (isMove) return;
    if (UIManager::Get()->IsPanelListActive()) return;

    destCoord = curCoord;

    if (KEY_PRESS(VK_RIGHT))
    {
        destCoord.x++;
        isMove = true;

        if (DEST_TILEDATA.type == Tile::LEDGE)
        {
            if (DEST_TILEDATA.ledgeDir == "right")
            {
                destCoord.x++;
                shadow->SetActive(true);

                if (!Audio::Get()->IsPlaySound("Jump"))
                    Audio::Get()->Play("Jump", 0.5f);
            }
        }
    }
    else if (KEY_PRESS(VK_LEFT))
    {
        destCoord.x--;
        isMove = true;

        if (DEST_TILEDATA.type == Tile::LEDGE)
        {
            if (DEST_TILEDATA.ledgeDir == "left")
            {
                destCoord.x--;
                shadow->SetActive(true);

                if (!Audio::Get()->IsPlaySound("Jump"))
                    Audio::Get()->Play("Jump", 0.5f);
            }
        }
    }
    else if (KEY_PRESS(VK_UP))
    {
        destCoord.y++;
        isMove = true;
    }
    else if (KEY_PRESS(VK_DOWN))
    {
        destCoord.y--;
        isMove = true;

        if (DEST_TILEDATA.type == Tile::LEDGE)
        {
            if (DEST_TILEDATA.ledgeDir == "down")
            {
                destCoord.y--;
                shadow->SetActive(true);

                if (!Audio::Get()->IsPlaySound("Jump"))
                    Audio::Get()->Play("Jump", 0.5f);
            }
        }
    }
}

void Trainer::SetAnimation()
{
    if (isMove)
    {
        Vector2 direction = destCoord - curCoord;

        if (direction.x > 0 && direction.y == 0)
        {
            trainer->SetClip("MOVE_LR");
            trainer->Rot().y = 0.0f;
        }
        else if (direction.x < 0 && direction.y == 0)
        {
            trainer->SetClip("MOVE_LR");
            trainer->Rot().y = XM_PI;
        }
        else if (direction.x == 0 && direction.y > 0)
        {
            trainer->SetClip("MOVE_UP");
        }
        else if (direction.x == 0 && direction.y < 0)
        {
            trainer->SetClip("MOVE_DOWN");
        }
    }
    else
    {
        if(direction.x > 0 && direction.y == 0)
        {
            trainer->SetClip("IDLE_LR");
            trainer->Rot().y = 0.0f;
        }
        else if (direction.x < 0 && direction.y == 0)
        {
            trainer->SetClip("IDLE_LR");
            trainer->Rot().y = XM_PI;
        }
        else if (direction.x == 0 && direction.y > 0)
        {
            trainer->SetClip("IDLE_UP");
        }
        else if (direction.x == 0 && direction.y < 0)
        {
            trainer->SetClip("IDLE_DOWN");
        }
    }
}

void Trainer::CreateClips()
{
    vector<Frame*> frames;
    
    //Trainer
    wstring textureFile = L"Textures/Character/Trainer/Trainer_IDLE_DOWN.png";
    Vector2 cutSize = Texture::Add(textureFile)->GetSize() / Vector2(1, 1);

    frames.push_back(new Frame(textureFile, 0, 0, cutSize.x, cutSize.y));
    trainer->AddClip("IDLE_DOWN", new Clip(frames));
    frames.clear();

    textureFile = L"Textures/Character/Trainer/Trainer_IDLE_UP.png";
    cutSize = Texture::Add(textureFile)->GetSize() / Vector2(1, 1);

    frames.push_back(new Frame(textureFile, 0, 0, cutSize.x, cutSize.y));
    trainer->AddClip("IDLE_UP", new Clip(frames));
    frames.clear();

    textureFile = L"Textures/Character/Trainer/Trainer_IDLE_LR.png";
    cutSize = Texture::Add(textureFile)->GetSize() / Vector2(1, 1);

    frames.push_back(new Frame(textureFile, 0, 0, cutSize.x, cutSize.y));
    trainer->AddClip("IDLE_LR", new Clip(frames));
    frames.clear();

    textureFile = L"Textures/Character/Trainer/Trainer_RUN_LR.png";
    cutSize = Texture::Add(textureFile)->GetSize() / Vector2(2, 1);

    frames.push_back(new Frame(textureFile, cutSize.x * 0, 0, cutSize.x, cutSize.y));
    frames.push_back(new Frame(textureFile, cutSize.x * 1, 0, cutSize.x, cutSize.y));
    trainer->AddClip("MOVE_LR", new Clip(frames, true, 0.7f));
    frames.clear();

    textureFile = L"Textures/Character/Trainer/Trainer_RUN_UP.png";
    cutSize = Texture::Add(textureFile)->GetSize() / Vector2(4, 1);

    frames.push_back(new Frame(textureFile, cutSize.x * 0, 0, cutSize.x, cutSize.y));
    frames.push_back(new Frame(textureFile, cutSize.x * 1, 0, cutSize.x, cutSize.y));
    frames.push_back(new Frame(textureFile, cutSize.x * 2, 0, cutSize.x, cutSize.y));
    frames.push_back(new Frame(textureFile, cutSize.x * 3, 0, cutSize.x, cutSize.y));
    trainer->AddClip("MOVE_UP", new Clip(frames, true, 0.7f));
    frames.clear();

    textureFile = L"Textures/Character/Trainer/Trainer_RUN_DOWN.png";
    cutSize = Texture::Add(textureFile)->GetSize() / Vector2(4, 1);

    frames.push_back(new Frame(textureFile, cutSize.x * 0, 0, cutSize.x, cutSize.y));
    frames.push_back(new Frame(textureFile, cutSize.x * 1, 0, cutSize.x, cutSize.y));
    frames.push_back(new Frame(textureFile, cutSize.x * 2, 0, cutSize.x, cutSize.y));
    frames.push_back(new Frame(textureFile, cutSize.x * 3, 0, cutSize.x, cutSize.y));
    trainer->AddClip("MOVE_DOWN", new Clip(frames, true, 0.7f));
    frames.clear();

    //grassMove
    textureFile = L"Textures/Character/Trainer/grassMove.png";
    cutSize = Texture::Add(textureFile)->GetSize() / Vector2(2, 1);

    frames.push_back(new Frame(textureFile, cutSize.x * 0, 0, cutSize.x, cutSize.y));
    grassMove->AddClip("GRASS_IDLE", new Clip(frames));
    frames.clear();

    frames.push_back(new Frame(textureFile, cutSize.x * 0, 0, cutSize.x, cutSize.y));
    frames.push_back(new Frame(textureFile, cutSize.x * 1, 0, cutSize.x, cutSize.y));
    grassMove->AddClip("GRASS_MOVE", new Clip(frames, true, 0.4f));
    frames.clear();
}
