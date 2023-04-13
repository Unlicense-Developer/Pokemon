#include "Framework.h"
#include "EditTileScene.h"

EditTileScene::EditTileScene()
{
    CreateSample();

    editTileMap = new EditTileMap(42, 38);
    editTileMap->Pos() = { 80, 80 };

    selectSample = new Quad(Vector2(SAMPLE_SIZE, SAMPLE_SIZE));

    char path[128];
    GetCurrentDirectoryA(sizeof(path), path);
    projectPath = path;
}

EditTileScene::~EditTileScene()
{
    for (Button* btn : sampleBtns)
        delete btn;

    delete editTileMap;
    delete selectSample;
}

void EditTileScene::Update()
{
    if (KEY_PRESS(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
    {
        if (selectSample->GetTexture())
        {
            editTileMap->ClickTile(selectSample, selectType);
        }
    }

    if (KEY_DOWN(VK_RBUTTON) && !ImGui::GetIO().WantCaptureMouse)
    {
        editTileMap->DeleteObjTile();
        editTileMap->DeletePortalTile();
        editTileMap->DeleteLedgeTile();
    }

    if (KEY_DOWN(VK_F1))
    {
        editTileMap->Save("TextData/Test.map");
    }

    if (KEY_DOWN(VK_F2))
    {
        editTileMap->Load("TextData/Test.map");
    }

    for (Button* btn : sampleBtns)
        btn->Update();

    CAM->Update();
    editTileMap->Update();

    if (selectSample->GetTexture())
    {
        if (mouseWheel > 0.0f)
        {
            selectSample->Rot().z += XM_PIDIV2;

            if (selectSample->Rot().z > XM_2PI)
                selectSample->Rot().z = XM_2PI;
        }
        else if (mouseWheel < 0.0f)
        {
            selectSample->Rot().z -= XM_PIDIV2;

            if (selectSample->Rot().z < 0)
                selectSample->Rot().z = 0.0f;
        }

        //selectSample->Pos() = CAM->ScreenToWorld(mousePos);
        selectSample->Pos() = mousePos;
        selectSample->UpdateWorld();
    }
}

void EditTileScene::PreRender()
{
    CAM->SetView();
}

void EditTileScene::Render()
{
    editTileMap->Render();
}

void EditTileScene::PostRender()
{
    for (Button* btn : sampleBtns)
        btn->Render();

    if (selectSample->GetTexture())
    {
        selectSample->Render();
    }

    const char* list[] = { "BG", "OBJ", "PORTAL", "LEDGE"};
    ImGui::ListBox("Type", (int*)&selectType, list, 4);

    Save();
    Load();
}

void EditTileScene::ClickSampleBtn(void* sampleBtn)
{
    Button* button = (Button*)sampleBtn;

    selectSample->SetTexture(button->GetTexture());
}

void EditTileScene::CreateSample()
{
    WIN32_FIND_DATA findData;

    HANDLE handle = FindFirstFile(L"Textures/Tile/VioletCity/*.png", &findData);

    bool result = true;
    wstring fileName;

    UINT width = 10;
    Vector2 size(SAMPLE_SIZE, SAMPLE_SIZE);

    Vector2 startPos(WIN_WIDTH - size.x, WIN_HEIGHT - size.y);

    while (result)
    {
        fileName = L"Textures/Tile/VioletCity/";
        fileName += findData.cFileName;
        result = FindNextFile(handle, &findData);

        UINT index = sampleBtns.size();

        Vector2 pos(index % width, index / width);

        Button* button = new Button(size);
        button->SetTexture(fileName);
        button->Pos() = startPos - (pos * size);
        button->SetParamEvent(bind(&EditTileScene::ClickSampleBtn, this, placeholders::_1));
        button->SetObject(button);

        sampleBtns.push_back(button);
    }
}

void EditTileScene::Save()
{
    if (ImGui::Button("Save"))
        DIALOG->OpenDialog("Save", "SaveFile", ".map", ".");

    if (DIALOG->Display("Save"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            file = file.substr(projectPath.length() + 1);

            editTileMap->Save(file);
        }

        DIALOG->Close();
    }
}

void EditTileScene::Load()
{
    if (ImGui::Button("Load"))
        DIALOG->OpenDialog("Load", "LoadFile", ".map", ".");

    if (DIALOG->Display("Load"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            file = file.substr(projectPath.length() + 1);

            editTileMap->Load(file);
        }

        DIALOG->Close();
    }
}
