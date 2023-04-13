#include "Framework.h"

#include "Scenes/IntroScene.h"
#include "Scenes/EditTileScene.h"
#include "Scenes/PokemonScene.h"
#include "Scenes/BattleScene.h"

GameManager::GameManager()
{
    srand(time(NULL));
    Create();

    uiViewBuffer = new MatrixBuffer();

    SceneManager::Get()->Add("Intro", new IntroScene());
    SceneManager::Get()->Add("TileEdit", new EditTileScene());
    SceneManager::Get()->Add("Pokemon", new PokemonScene());
    SceneManager::Get()->Add("Battle", new BattleScene());

    SceneManager::Get()->ChangeScene("Pokemon");
}

GameManager::~GameManager()
{
    delete uiViewBuffer;

    Delete();
    SceneManager::Delete();
}

void GameManager::Update()
{
    Keyboard::Get()->Update();
    Timer::Get()->Update();

    SceneManager::Get()->Update();

    mouseWheel = 0.0f;
}

void GameManager::Render()
{
    SceneManager::Get()->PreRender();
    
    Device::Get()->Clear();
    Font::Get()->GetDC()->BeginDraw();

    Environment::Get()->SetViewport();
    Environment::Get()->SetProjection();

    SceneManager::Get()->Render();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

   /* string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
    Font::Get()->RenderText(fps, { 200, WIN_HEIGHT - 10 });*/

    //CAM->RenderUI();

    uiViewBuffer->SetVS(1);
    SceneManager::Get()->PostRender();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    Font::Get()->GetDC()->EndDraw();

    Device::Get()->Present();
}

void GameManager::Create()
{
    Keyboard::Get();
    Timer::Get();
    Device::Get();
    Environment::Get();
    Observer::Get();
    PlayerManager::Get();
    TextBoxManager::Get();
    UIManager::Get();

    Font::Get()->AddColor("White", 1, 1, 1);
    Font::Get()->AddColor("Black", 0, 0, 0);
    Font::Get()->AddColor("Magenta", 1, 0, 1);
    Font::Get()->AddStyle("PokemonFont_40", L"PokemonGSK", 40.0f);
    Font::Get()->AddStyle("PokemonFont_50", L"PokemonGSK", 50.0f);
    Font::Get()->AddStyle("PokemonFont_55", L"PokemonGSK", 55.0f);
    Font::Get()->AddStyle("PokemonFont_60", L"PokemonGSK", 60.0f);
    Font::Get()->AddStyle("PokemonFont_65", L"PokemonGSK", 65.0f);
    Font::Get()->AddStyle("PokemonFont_70", L"PokemonGSK", 70.0f);
    Font::Get()->AddStyle("PokemonFont_75", L"PokemonGSK", 75.0f);
    Font::Get()->AddStyle("PokemonFont_80", L"PokemonGSK", 80.0f);
    Font::Get()->AddStyle("PokemonFont_90", L"PokemonGSK", 90.0f);

    Font::Get()->SetColor("White");
    Font::Get()->SetStyle("PokemonFont_40");

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE, DC);
}

void GameManager::Delete()
{
    Keyboard::Delete();
    Timer::Delete();
    Device::Delete();
    Shader::Delete();
    Texture::Delete();
    Environment::Delete();    
    Observer::Delete();
    PlayerManager::Delete();
    TextBoxManager::Delete();
    UIManager::Delete();
    Font::Delete();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
}
