#include "scene_manager.h"
#include "scene/load/load.h"
#include "scene/title/title.h"
#include "scene/mode/mode.h"
#include "scene/entry/entry.h"
#include "scene/game_main/game_main.h"
#include "scene/option/option.h"
#include "../ui_manager/ui_manager.h"


const unsigned int CSceneManager::m_fade_default_color = 0xff000000;
const int CSceneManager::m_fade_max_alpha = 255;
const int CSceneManager::m_fade_min_alpha = 0;
const int CSceneManager::m_fade_speed = 10;


CSceneManager& CSceneManager::GetInstance()
{
	static CSceneManager instance;

	return instance;
}

void CSceneManager::Initialize()
{
	m_SceneID = SCENE_ID::DUMMY;
    m_NextSceneID = SCENE_ID::LOAD;

    m_Generator[SCENE_ID::LOAD]         = []() { return new CLoad(); };
    m_Generator[SCENE_ID::TITLE]        = []() { return new CTitle(); };
    m_Generator[SCENE_ID::OPTION]       = []() { return new COption(); };
    m_Generator[SCENE_ID::MODE]         = []() { return new CMode(); };
    m_Generator[SCENE_ID::ENTRY]        = []() { return new CEntry(); };
    m_Generator[SCENE_ID::GAMEMAIN]     = []() { return new CGameMain(); };
    m_Generator[SCENE_ID::DUMMY]        = []() { return nullptr; };

    this->SetState(SCENE_STATE::CHANGE);
}

void CSceneManager::Update()
{
    switch (m_SceneState)
    {
    case SCENE_STATE::MAIN:     this->UpdateMain();     break;
    case SCENE_STATE::F_IN:     this->UpdateFadeIn();   break;
    case SCENE_STATE::F_OUT:    this->UpdateFadeOut();  break;
    case SCENE_STATE::CHANGE:   this->UpdateChange();   break;
    }
}

void CSceneManager::Draw()
{
    // シーンがない時は処理しない
    if (!m_Scene)		return;

    // シーンの描画
    m_Scene->Draw();
}

void CSceneManager::DrawFade()
{
    // 通常状態の時は処理しない
    if (m_SceneState == SCENE_STATE::MAIN) return;

    vivid::DrawTexture("data\\white.png", vivid::Vector2::ZERO, m_FadeColor);
}

void CSceneManager::Finalize()
{
    // シーンがない時は処理しない
    if (!m_Scene)		return;

    // シーンの解放
	m_Scene->Finalize();

    // m_Sceneの削除
	delete m_Scene;
	m_Scene = nullptr;
}

void CSceneManager::SetNextScene(SCENE_ID id)
{
    m_NextSceneID = id;
}

void CSceneManager::CheckSceneChange()
{
    // 次のシーンIDがDUMMY(シーン変更なし)の時は処理しない
    if (m_NextSceneID == SCENE_ID::DUMMY)   return;

    // シーン変更フラグがfalseなら
    if (!m_SceneChangeFlag)
    {
        // 状態をフェードアウトにする
        this->SetState(SCENE_STATE::F_OUT);

        // フラグをセットする
        m_SceneChangeFlag = true;
    }
}

bool CSceneManager::GetIsFading()
{
    if (m_SceneState == SCENE_STATE::MAIN)  return false;

    return true;
}

void CSceneManager::ChangeScene()
{
    // 解放
    this->Finalize();

    // 毎シーン使っているオブジェクトはシーン切り替えごとに解放
    // （前のシーンの物は基本使わないため）
    UIS.Finalize();

    // シーンIDの更新
    m_SceneID = m_NextSceneID;
    m_NextSceneID = SCENE_ID::DUMMY;

    // m_Sceneに新しいシーンを入れる
    m_Scene = m_Generator[m_SceneID]();

    // 新しいシーンの初期化
    m_Scene->Initialize();
}

void CSceneManager::SetState(SCENE_STATE state)
{
    m_SceneState = state;

    if (state == SCENE_STATE::F_OUT)
    {
        m_FadeColor = 0x00000000;
    }
    if (state == SCENE_STATE::F_IN)
    {
        m_FadeColor = m_fade_default_color;
    }
}

void CSceneManager::UpdateMain()
{
    // シーンがない時は処理しない
    if (!m_Scene)		return;

    // シーンの更新
    m_Scene->Update();
}

void CSceneManager::UpdateFadeIn()
{
    // 透明度を取り出す
    int alpha = (m_FadeColor & 0xff000000) >> 24;

    // 減算
    alpha -= m_fade_speed;

    // 最小値を超えた場合の補正
    if (alpha < m_fade_min_alpha)
    {
        alpha = m_fade_min_alpha;
    }

    // 透明度を適用
    m_FadeColor = (alpha << 24) | (m_FadeColor & 0x00ffffff);

    // フェードインが完了したら状態をメインにする
    if (alpha == m_fade_min_alpha)
    {
        this->SetState(SCENE_STATE::MAIN);

        // シーン変更フラグをfalseに戻す
        m_SceneChangeFlag = false;
    }
}

void CSceneManager::UpdateFadeOut()
{
    // 透明度を取り出す
    int alpha = (m_FadeColor & 0xff000000) >> 24;

    // 加算
    alpha += m_fade_speed;

    // 最大値を超えた場合の補正
    if (alpha > m_fade_max_alpha)
    {
        alpha = m_fade_max_alpha;
    }

    // 透明度を適用
    m_FadeColor = (alpha << 24) | (m_FadeColor & 0x00ffffff);

    // フェードアウトが完了したら、シーン変更状態にする
    if (alpha == m_fade_max_alpha)
    {
        this->SetState(SCENE_STATE::CHANGE);
    }
}

void CSceneManager::UpdateChange()
{
    // シーンを変更
    this->ChangeScene();

    // 状態をフェードインにする
    this->SetState(SCENE_STATE::F_IN);
}
