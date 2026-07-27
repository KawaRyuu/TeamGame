#include "game_main.h"

#include "contol.h"
#include "../../../effect_manager/effect_manager.h"
#include "../../../ui_manager/ui_manager.h"
#include "../../../sound_manager/sound_manager.h"
#include "../../../chara_manager/chara_manager.h"

const int	CGameMain::m_view_winner_time = 8 * 60;

CGameMain::CGameMain()
	: m_GameSetFlag(false)
	, m_ViewWinnerTimer(0)
{
}

void CGameMain::Initialize()
{
	//PLAYS.InitializeGameData();

	CHARAS.Initialize();

	// BGMの再生
	//SOUNDS.Play(SOUND_ID::GAMEMAIN_BGM, true);

}

void CGameMain::Update()
{
	//PLAYS.Update();
	CHARAS.Update();

	if (m_GameSetFlag)
	{
		if (m_ViewWinnerTimer++ > m_view_winner_time)
		{
			// 今は1試合終わったらモード選択へ戻る
			SCENES.SetNextScene(SCENE_ID::MODE);


		}
	}

#ifdef VIVID_DEBUG

	if (contol::InputStart(contol::PLAYER_ID::ALL,contol::BUTTON_ID::START))
	{
		SCENES.SetNextScene(SCENE_ID::MODE);
	}

#endif // DEBUG
}

void CGameMain::Draw()
{
	CHARAS.Draw();
#ifdef VIVID_DEBUG
	vivid::DrawText(30, "GameMain", vivid::Vector2::ZERO);
#endif // DEBUG

}

void CGameMain::Finalize()
{
	// BGMの停止
	//SOUNDS.Stop(SOUND_ID::GAMEMAIN_BGM);
	CHARAS.Finalize();
	EFFECTS.Finalize();
}
