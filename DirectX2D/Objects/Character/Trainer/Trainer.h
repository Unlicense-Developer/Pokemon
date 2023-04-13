#pragma once

class Trainer : public Quad
{
private:
    const Vector2 TILE_VECTOR = { 80.0f, 80.0f };
    const float TILE_SIZE = 80.0f;
    const float SPEED = 200.0f;
    const float JUMP_POWER = 310.0F;
    const float JUMP_DOWN_POWER = 500.0f;
    const float JUMP_FALL_TIME = 0.195f;
    const float WILD_POKEMON_APPEAR = 0.2f;
    
public:
    Trainer();
    ~Trainer();

    void Update();
    void Render();

    void CheckTileType();
    void SetCurCoord(Vector2 coord);
    void SetEncounter(bool isEncounter) {  this->isEncounter = isEncounter; }
    void SetIsBattle(bool isBattle) {  this->isBattle = isBattle; }
    void SetIsMove(bool isMove) {  this->isMove = isMove; }

    Quad* GetBattleImage() { return battleImage; }
    Vector2 GetCurCoord() { return curCoord; }
    bool GetIsEncounter() { return isEncounter; }
    bool GetIsBattle() { return isBattle; }
    bool GetIsMove() { return isMove; }

private:
    void Move();
    void Jump();
    void UpdateShadowPos();
    void UpdateGrassMove();
    void SetDirection();
    void SetAnimation();
    void CreateClips();

private:
    AnimObject* trainer;
    AnimObject* grassMove; 
    Quad* shadow;
    Quad* battleImage;

    float jumpTime = 0.0f;

    bool isMove = false;
    bool isJump = false;
    bool isJumpfall = false;
    bool isEncounter = false;
    bool isBattle = false;

    Vector2 direction;
    Vector2 curCoord;
    Vector2 destCoord;
};
