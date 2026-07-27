#include "game_main.h"

#include "contol.h"
#include "../../../effect_manager/effect_manager.h"
#include "../../../ui_manager/ui_manager.h"
#include "../../../sound_manager/sound_manager.h"

const int	CGameMain::m_view_winner_time = 8 * 60;

CGameMain::CGameMain()
	: m_GameSetFlag(false)
	, m_ViewWinnerTimer(0)
{
}

void CGameMain::Initialize()
{
	//PLAYS.InitializeGameData();

	// BGMの再生
	//SOUNDS.Play(SOUND_ID::GAMEMAIN_BGM, true);


	/*	8人用テスト
	FIELDS.Create(FIELD_ID::NORMAL, { 100.0f, 50.0f }, contol::PLAYER_ID::P1);
	FIELDS.Create(FIELD_ID::NORMAL, { 100.0f + 40.0f * 11.0f, 50.0f }, contol::PLAYER_ID::P2);
	FIELDS.Create(FIELD_ID::NORMAL, { 100.0f + 40.0f * 11.0f * 2, 50.0f }, contol::PLAYER_ID::P3);
	FIELDS.Create(FIELD_ID::NORMAL, { 100.0f + 40.0f * 11.0f * 3, 50.0f }, contol::PLAYER_ID::P4);
	FIELDS.Create(FIELD_ID::NORMAL, { 100.0f, 590.0f }, contol::PLAYER_ID::P5);
	FIELDS.Create(FIELD_ID::NORMAL, { 100.0f + 40.0f * 11.0f, 590.0f }, contol::PLAYER_ID::P6);
	FIELDS.Create(FIELD_ID::NORMAL, { 100.0f + 40.0f * 11.0f * 2, 590.0f }, contol::PLAYER_ID::P7);
	FIELDS.Create(FIELD_ID::NORMAL, { 100.0f + 40.0f * 11.0f * 3, 590.0f }, contol::PLAYER_ID::P8);
	*/

}

void CGameMain::Update()
{
	//PLAYS.Update();

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

#ifdef VIVID_DEBUG
	vivid::DrawText(30, "GameMain", vivid::Vector2::ZERO);
#endif // DEBUG

}

void CGameMain::Finalize()
{
	// BGMの停止
	//SOUNDS.Stop(SOUND_ID::GAMEMAIN_BGM);

	EFFECTS.Finalize();
}
