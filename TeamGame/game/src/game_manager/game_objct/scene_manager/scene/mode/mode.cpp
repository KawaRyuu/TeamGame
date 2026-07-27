#include "mode.h"
#include "contol.h"
#include "../../../sound_manager/sound_manager.h"

CMode::CMode()
	: m_ModeState(MODE_STATE::MAIN)
	, m_ToMainMenu(false)
	, m_ToSoloMenu(false)
	, m_ToEveryMenu(false)
	, m_PlayerCount(0)
	, m_BackButton(nullptr)
{
}

void CMode::Initialize()
{
	//m_PlayerCount = PLAYS.GetPlayerCount();
	m_PlayerCount = 2;

	// 戻るボタンの生成
	m_BackButton = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ 100.0f,(float)vivid::WINDOW_HEIGHT - 200.0f },
		100, 100,
		[this]()
		{
			this->InputBackKey();
		},
		0xff80ffff,
		"data/ui/mode_select/back.png", 0.75f);

	// メインメニューボタン群の生成
	this->CreateMainMenuButton();

	// BGMの再生
	////SOUNDS.Play(SOUND_ID::MODE_BGM, true);

	// コントローラー未接続時の自動割り当て
	contol::AutoBindPlayer1();
}

void CMode::Update()
{
	// 各状態の切り替わりフラグがtrueになったら、新しいUIを生成して状態を変更する
	if (m_ToMainMenu && UIS.GetModeSceneUiHalfTime())
	{
		UIS.DeleteUi(UI_ID::TEST_BUTTON_UI);
		this->CreateMainMenuButton();
		m_ToMainMenu = false;
		m_ModeState = MODE_STATE::MAIN;
	}
	if (m_ToSoloMenu && UIS.GetModeSceneUiHalfTime())
	{
		UIS.DeleteUi(UI_ID::TEST_BUTTON_UI);
		this->CreateSoloMenuButton();
		m_ToSoloMenu = false;
		m_ModeState = MODE_STATE::SOLO;
	}
	if (m_ToEveryMenu && UIS.GetModeSceneUiHalfTime())
	{
		UIS.DeleteUi(UI_ID::TEST_BUTTON_UI);
		this->CreateEveryMenuButton();
		m_ToEveryMenu = false;
		m_ModeState = MODE_STATE::EVERY;
	}

	// 戻るボタン入力時
	if (contol::InputEnd(contol::PLAYER_ID::ALL, contol::BUTTON_ID::ACTION_RIGHT)
		&& UIS.GetModeSceneUiHalfTime())
	{
		this->InputBackKey();
	}

	this->ChangePlayerCount();
	// モード選択シーンへ戻る
	if (contol::InputEnd(contol::PLAYER_ID::P1, contol::BUTTON_ID::START))
	{
		//SCENES.SetNextScene(SCENE_ID::SHOW_RECORD);
	}

#ifdef VIVID_DEBUG
	
	// テストでゲームメインへ
	if (contol::Input(contol::PLAYER_ID::ALL, contol::BUTTON_ID::ACTION_UP)
		&& contol::InputDPad(contol::PLAYER_ID::ALL, contol::D_PAD_ID::UP))
	{
		SCENES.SetNextScene(SCENE_ID::GAMEMAIN);
	}

#endif // DEBUG
}

void CMode::Draw()
{
	if (m_ModeState == MODE_STATE::EVERY)
	{
		vivid::DrawText(40, "[L] [R] で参加人数を変更：" + std::to_string(m_PlayerCount) + "人プレイ", { 10.0f,10.0f });
	}

#ifdef VIVID_DEBUG
	vivid::DrawText(30, "Mode", vivid::Vector2::ZERO);
	vivid::DrawText(30, std::to_string(m_PlayerCount), vivid::Vector2(0.0f, 30.0f));
#endif // DEBUG

}

void CMode::Finalize()
{
	// BGMの停止
	////SOUNDS.Stop(SOUND_ID::MODE_BGM);
}

void CMode::InputBackKey()
{
	// メインメニューならタイトルに戻り、それ以外ならメインメニューに戻る
	switch (m_ModeState)
	{
	case CMode::MODE_STATE::MAIN:
		SCENES.SetNextScene(SCENE_ID::TITLE);
		break;

	case CMode::MODE_STATE::SOLO:
		m_ToMainMenu = true;
		UIS.SetModeSceneUiMove();
		break;

	case CMode::MODE_STATE::EVERY:
		m_ToMainMenu = true;
		UIS.SetModeSceneUiMove();
		break;
	}

	// SEの再生
	////SOUNDS.Play(SOUND_ID::CANCEL);
}

void CMode::CreateMainMenuButton()
{
	// 「ひとりで」ボタン
	auto btn_solo = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ 100.0f,150.0f },
		625, 500,
		[this]()
		{
			m_ToSoloMenu = true;
			UIS.SetModeSceneUiMove();

			// SEの再生
			////SOUNDS.Play(SOUND_ID::DECIDE);
		},
		0xffffa0ff,
		"data/ui/mode_select/solo.png", 0.9f);
	// 「みんなで」ボタン
	auto btn_every = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ (float)vivid::WINDOW_WIDTH - 625.0f - 100.0f,150.0f },
		625, 500,
		[this]()
		{
			m_ToEveryMenu = true;
			UIS.SetModeSceneUiMove();

			// SEの再生
			////SOUNDS.Play(SOUND_ID::DECIDE);
		},
		0xffc0ffc0,
		"data/ui/mode_select/every.png", 0.9f);
	// 「設定」ボタン
	auto btn_option = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ 325.0f,(float)vivid::WINDOW_HEIGHT - 300.0f },
		300, 200,
		[this]()
		{
			SCENES.SetNextScene(SCENE_ID::OPTION);

			// SEの再生
			//SOUNDS.Play(SOUND_ID::DECIDE);
		},
		0xffa0c0ff,
		"data/ui/mode_select/option.png", 1.0f);

	// 隣接するボタンの設定
	btn_solo->SetNeighbor(nullptr, nullptr, btn_every, btn_option);
	btn_every->SetNeighbor(btn_solo, nullptr, nullptr, btn_option);
	btn_option->SetNeighbor(m_BackButton, btn_solo, nullptr, nullptr);
	m_BackButton->SetNeighbor(nullptr, btn_solo, btn_option, nullptr);

	// ボタンアクティブ状態の設定
	btn_solo->SetButtonActive();
	m_BackButton->SetButtonActive(false);
}

void CMode::CreateSoloMenuButton()
{
	// 「スコアアタック」ボタン
	auto btn_score = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ 75.0f,100.0f },
		540, 540,
		[this]()
		{
			//PLAYS.SetPlayerCount(1);
			//PLAYS.SetGameInfo(1, CPlayManager::GAME_MODE::SOLO_SCORE);
			SCENES.SetNextScene(SCENE_ID::ENTRY);

			// SEの再生
			//SOUNDS.Play(SOUND_ID::DECIDE);
		},
		0xffffa0ff,
		"data/ui/mode_select/score.png", 1.0f);
	// 「エンドレス」ボタン
	auto btn_endless = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ 540.0f * 1.0f + 75.0f * 2.0f,100.0f },
		540, 300,
		[this]()
		{
			//PLAYS.SetPlayerCount(1);
			//PLAYS.SetGameInfo(1, CPlayManager::GAME_MODE::SOLO_ENDLESS);
			SCENES.SetNextScene(SCENE_ID::ENTRY);

			// SEの再生
			//SOUNDS.Play(SOUND_ID::DECIDE);
		},
		0xffffa0ff,
		"data/ui/mode_select/endless.png", 1.75f);
	// 「ミニエンドレス」ボタン
	auto btn_mini_endless = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ 540.0f * 2.0f + 75.0f * 3.0f,100.0f },
		540, 300,
		[this]()
		{
			// 実装予定

			// SEの再生
			//SOUNDS.Play(SOUND_ID::NO);
		},
		0xffff80ff,
		"data/ui/mode_select/coming_soon.png", 1.75f);
	// 「チャレンジ」ボタン
	auto btn_challenge = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ 540.0f * 1.0f + 75.0f * 2.0f,450.0f },
		540, 300,
		[this]()
		{
			// 実装予定

			// SEの再生
			//SOUNDS.Play(SOUND_ID::NO);
		},
		0xffffa0ff,
		"data/ui/mode_select/coming_soon.png", 1.75f);
	// 「レッスン」ボタン
	auto btn_lesson = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ 540.0f * 2.0f + 75.0f * 3.0f,450.0f },
		540, 300,
		[this]()
		{
			// 実装予定

			// SEの再生
			//SOUNDS.Play(SOUND_ID::NO);
		},
		0xffffa0ff,
		"data/ui/mode_select/coming_soon.png", 1.75f);

	// 隣接するボタンの設定
	btn_score->SetNeighbor(nullptr, nullptr, btn_endless, m_BackButton);
	btn_endless->SetNeighbor(btn_score, nullptr, btn_mini_endless, btn_challenge);
	btn_mini_endless->SetNeighbor(btn_endless, nullptr, nullptr, btn_lesson);
	btn_challenge->SetNeighbor(btn_score, btn_endless, btn_lesson, m_BackButton);
	btn_lesson->SetNeighbor(btn_challenge, btn_mini_endless, nullptr, m_BackButton);
	m_BackButton->SetNeighbor(nullptr, btn_score, nullptr, nullptr);

	// ボタンアクティブ状態の設定
	btn_score->SetButtonActive();
	m_BackButton->SetButtonActive(false);
}

void CMode::CreateEveryMenuButton()
{
	// 「VS」ボタン
	auto btn_vs = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ 75.0f,100.0f },
		540, 540,
		[this]()
		{
			if (m_PlayerCount <= 1 || m_PlayerCount > 4)
			{
				// SEの再生
				//SOUNDS.Play(SOUND_ID::CANCEL);

				return;
			}

			//PLAYS.SetGameInfo(m_PlayerCount, CPlayManager::GAME_MODE::EVERY_VS);
			SCENES.SetNextScene(SCENE_ID::ENTRY);

			// SEの再生
			//SOUNDS.Play(SOUND_ID::DECIDE);
		},
		0xffc0ffc0,
		"data/ui/mode_select/vs.png", 1.0f);
	// 「スコアアタック」ボタン
	auto btn_score = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ 540.0f * 1.0f + 75.0f * 2.0f,100.0f },
		540, 540,
		[this]()
		{
			if (m_PlayerCount <= 1 || m_PlayerCount > 4)
			{
				// SEの再生
				//SOUNDS.Play(SOUND_ID::CANCEL);

				return;
			}

			//PLAYS.SetGameInfo(m_PlayerCount, CPlayManager::GAME_MODE::EVERY_SCORE);
			SCENES.SetNextScene(SCENE_ID::ENTRY);

			// SEの再生
			//SOUNDS.Play(SOUND_ID::DECIDE);
		},
		0xffc0ffc0,
		"data/ui/mode_select/score.png", 1.0f);
	// 「ミニVS」ボタン
	auto btn_mini_vs = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ 540.0f * 2.0f + 75.0f * 3.0f,100.0f },
		540, 300,
		[this]()
		{
			// 実装予定

			// SEの再生
			//SOUNDS.Play(SOUND_ID::NO);
		},
		0xffc0ffc0,
		"data/ui/mode_select/coming_soon.png", 1.75f);
	// 「ペアでVS」ボタン
	auto btn_pair_vs = UIS.CreateButton(UI_ID::TEST_BUTTON_UI,
		{ 540.0f * 2.0f + 75.0f * 3.0f,450.0f },
		540, 300,
		[this]()
		{
			if (m_PlayerCount < 3 || m_PlayerCount % 2 != 0)
			{
				// SEの再生
				//SOUNDS.Play(SOUND_ID::CANCEL);

				return;
			}

			//PLAYS.SetGameInfo(m_PlayerCount, CPlayManager::GAME_MODE::EVERY_PAIR);
			SCENES.SetNextScene(SCENE_ID::ENTRY);

			// SEの再生
			//SOUNDS.Play(SOUND_ID::DECIDE);
		},
		0xffc0ffc0,
		"data/ui/mode_select/pair.png", 1.75f);

	// 隣接するボタンの設定
	btn_vs->SetNeighbor(nullptr, nullptr, btn_score, m_BackButton);
	btn_score->SetNeighbor(btn_vs, nullptr, btn_mini_vs, m_BackButton);
	btn_mini_vs->SetNeighbor(btn_score, nullptr, nullptr, btn_pair_vs);
	btn_pair_vs->SetNeighbor(btn_score, btn_mini_vs, nullptr, m_BackButton);
	m_BackButton->SetNeighbor(nullptr, btn_vs, nullptr, nullptr);

	// ボタンアクティブ状態の設定
	btn_vs->SetButtonActive();
	m_BackButton->SetButtonActive(false);
}

void CMode::ChangePlayerCount()
{
	// 「みんなで」以外の時は処理しない
	if (m_ModeState != MODE_STATE::EVERY)	return;

	// 減らす
	if (contol::InputStart(contol::PLAYER_ID::ALL, contol::BUTTON_ID::LB)
		&& m_PlayerCount > 2)
	{
		m_PlayerCount--;
		//PLAYS.SetPlayerCount(m_PlayerCount);
		contol::AutoBindPlayer();

		// SEの再生
		//SOUNDS.Play(SOUND_ID::DECIDE);
	}
	// 増やす
	if (contol::InputStart(contol::PLAYER_ID::ALL, contol::BUTTON_ID::RB)
		&& m_PlayerCount < (int)(contol::PLAYER_ID::MAX))
	{
		m_PlayerCount++;
		//PLAYS.SetPlayerCount(m_PlayerCount);
		contol::AutoBindPlayer();

		// SEの再生
		//SOUNDS.Play(SOUND_ID::DECIDE);
	}
}
