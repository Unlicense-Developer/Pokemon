#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define WIN_START_X 1200
#define WIN_START_Y 100

#define CENTER_X (WIN_WIDTH * 0.5f)
#define CENTER_Y (WIN_HEIGHT * 0.5f)

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define KEY_DOWN(k) Keyboard::Get()->Down(k)
#define KEY_UP(k) Keyboard::Get()->Up(k)
#define KEY_PRESS(k) Keyboard::Get()->Press(k)

#define DELTA Timer::Get()->GetElapsedTime()

#define CAM Environment::Get()->GetMainCamera()

#define DIALOG ImGuiFileDialog::Instance()

#define CUR_SCENE SceneManager::Get()->GetCurScene()
#define CUR_TILEDATA GameTileMapManager::Get()->GetTileMap()->GetCoordToTile(curCoord)->GetData()
#define DEST_TILEDATA GameTileMapManager::Get()->GetTileMap()->GetCoordToTile(destCoord)->GetData()
#define MY_TRAINER PlayerManager::Get()->GetPlayer()
#define MY_POKEMONS PlayerManager::Get()->GetMyPokemons()
#define MY_FIRST_POKEMON PlayerManager::Get()->GetMyPokemons()[0]
#define ENEMY_POKEMON BattleManager::Get()->GetEnemy()
#define WILD_POKEMON EnemyManager::Get()->GetWildPokemon()
#define ENEMY_TRAINER EnemyManager::Get()->GetEnemyTrainer()
#define CUR_PANEL frontPanel.top()
#define FRONT_PANEL(K) frontPanel.top() == totalPaneldatas.at(K)
#define FRONT_CURSOR_INDEX frontPanel.top()->GetCursor()->GetCurIndex()
#define FRONT_INDEX_SIZE frontPanel.top()->GetCursor()->GetIndexSize()

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <list>
#include <stack>
#include <unordered_set>
#include <functional>
#include <thread>
#include <mutex>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//DirectXTex
#include <DirectXTex.h>

#pragma comment(lib, "DirectXTex.lib")

//ImGui
#include <ImGui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <dirent/dirent.h>
#include <ImGuiFileDialog.h>
#include <ImGuiFileDialogConfig.h>

#pragma comment(lib, "ImGui.lib")

//DirectWrite
#include <d2d1_2.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace std;
using namespace DirectX;

typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMMATRIX Matrix;

typedef function<void()> Event;
typedef function<void(void*)> ParamEvent;
typedef function<void(int)> IntParamEvent;

//FMOD Library
#include "Libraries/inc/fmod.hpp"
#pragma comment(lib, "Libraries/lib/fmod_vc.lib")

//Framework Header

#include "Framework/Utilities/Singleton.h"
#include "Framework/Utilities/Keyboard.h"
#include "Framework/Utilities/Timer.h"
#include "Framework/Utilities/Utility.h"
#include "Framework/Utilities/Observer.h"
#include "Framework/Utilities/tinyxml2.h"
#include "Framework/Utilities/Font.h"
#include "Framework/Utilities/Audio.h"
#include "Framework/Utilities/BinaryReader.h"
#include "Framework/Utilities/BinaryWriter.h"

using namespace Utility;

#include "Framework/Math/Vector2.h"
#include "Framework/Math/GameMath.h"
#include "Framework/Math/Transform.h"

using namespace GameMath;

#include "Framework/System/Device.h"

#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"

#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/ConstBuffer.h"
#include "Framework/Buffer/VertexLayouts.h"
#include "Framework/Buffer/GlobalBuffer.h"

#include "Framework/Render/Texture.h"
#include "Framework/Render/Frame.h"
#include "Framework/Render/Clip.h"
#include "Framework/Render/RenderTarget.h"

#include "Framework/Collision/Collider.h"
#include "Framework/Collision/RectCollider.h"
#include "Framework/Collision/CircleCollider.h"
#include "Framework/Collision/LineCollider.h"

#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Environment.h"

//Object Header
#include "Objects/Basic/GameObject.h"
#include "Objects/Basic/Quad.h"
#include "Objects/Basic/Action.h"
#include "Objects/Basic/Effect.h"
#include "Objects/Basic/AnimObject.h"

#include "Objects/UI/Button.h"
#include "Objects/UI/UICursor.h"
#include "Objects/UI/Panel.h"
#include "Objects/UI/ProgressBar.h"

#include "Objects/Character/Pokemon/Pokemon.h"
#include "Objects/Character/Trainer/Trainer.h"
#include "Objects/Character/NPC/NPC.h"
#include "Objects/Character/NPC/EnemyTrainer.h"

#include "Objects/Tile/Tile.h"
#include "Objects/Tile/EditTileMap.h"
#include "Objects/Tile/GameTileMap.h"


#include "Objects/Manager/TextBoxManager.h"
#include "Objects/Manager/PlayerManager.h"
#include "Objects/Manager/EnemyManager.h"
#include "Objects/Manager/EffectManager.h"
#include "Objects/Manager/UIManager.h"
#include "Objects/Manager/GameTileMapManager.h"
#include "Objects/Manager/PokemonDataManager.h"
#include "Objects/Manager/BattleManager.h"

//Scene Header
#include "Scenes/Scene.h"
#include "Manager/GameManager.h"
#include "Manager/SceneManager.h"

extern HWND hWnd;
extern Vector2 mousePos;
extern float mouseWheel;