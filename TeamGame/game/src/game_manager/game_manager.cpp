#include "game_manager.h"

#include "game_objct/scene_manager/scene_manager.h"
#include "game_objct/effect_manager/effect_manager.h"
#include "game_objct/ui_manager/ui_manager.h"
#include "game_objct/sound_manager/sound_manager.h"

// インスタンスの取得
CGameManager& CGameManager::
GetInstance(void)
{
    static CGameManager instance;

    return instance;
}

void CGameManager::Initialize()
{
    EFFECTS.Initialize();
    UIS.Initialize();
    SCENES.Initialize();
    SOUNDS.Initialize();

    //contol::AutoBindPlayer();
    contol::SetActiveCount((int)contol::PLAYER_ID::MAX);
}

void CGameManager::Update()
{
    SCENES.Update();
    EFFECTS.Update();
    UIS.Update();

    SCENES.CheckSceneChange();

}

void CGameManager::Draw()
{
    SCENES.Draw();
    UIS.DrawBack();
    EFFECTS.Draw();
    UIS.DrawFront();

    SCENES.DrawFade();

}

void CGameManager::Finalize()
{
    SCENES.Finalize();
    EFFECTS.Finalize();
    UIS.Finalize();
}
