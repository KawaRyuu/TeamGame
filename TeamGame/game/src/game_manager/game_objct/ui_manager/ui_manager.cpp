#include "ui_manager.h"
#include "../scene_manager/scene_manager.h"
#include "ui/number_ui/number_ui.h"
#include "ui/button_ui/button_ui.h"
#include "ui/number_ui/score_ui/score_ui.h"
#include "ui/button_ui/test_button_ui/test_button_ui.h"
#include "ui/button_ui/test_button_ui2/test_button_ui2.h"
#include "ui/bar_ui/load_progress_ui/load_progress_ui.h"
#include "ui/bar_ui/back_bar_ui/back_bar_ui.h"
#include "ui/bar_ui/volume_slider_ui/volume_slider_ui.h"
#include "ui/alphanumeric_ui/alphanumeric_ui.h"
#include "ui/number_ui/record_score_ui/record_score_ui.h"


CUiManager& CUiManager::GetInstance()
{
	static CUiManager instance;

	return instance;
}

void CUiManager::Initialize()
{
	m_Generator[UI_ID::ALPHANUMERIC_UI]		= []() { return new CAlphanumericUi(); };
	m_Generator[UI_ID::SCORE_UI]			= []() { return new CScoreUi(); };
	m_Generator[UI_ID::RECORD_SCORE_UI]		= []() { return new CRecordScoreUi(); };
	m_Generator[UI_ID::TEST_BUTTON_UI]		= []() { return new CTestButtonUi(); };
	m_Generator[UI_ID::TEST_BUTTON_UI2]		= []() { return new CTestButtonUi2(); };
	m_Generator[UI_ID::LOAD_PROGRESS_UI]	= []() { return new CLoadProgressUi(); };
	m_Generator[UI_ID::BACK_BAR_UI]			= []() { return new CBackBarUi(); };
	m_Generator[UI_ID::VOLUME_SLIDER_UI]	= []() { return new CVolumeSliderUi(); };
	m_Generator[UI_ID::DUMMY]				= []() { return nullptr; };
	m_UiList.clear();

	m_ButtonMoved = false;
	m_ModeUiDrawPos = 0.0f;
	m_ModeUiDrawAngle = 360;
}

void CUiManager::Update()
{
	if (m_ModeUiDrawAngle < 360)
	{
		m_ModeUiDrawAngle += 4;
		float test = std::sin((float)m_ModeUiDrawAngle * 3.14f / 180.0f);
		m_ModeUiDrawPos = (float)vivid::WINDOW_HEIGHT * test;

		if (m_ModeUiDrawAngle >= 360)
		{
			m_ModeUiDrawPos = 0.0f;
			m_ModeUiDrawAngle = 360;
		}

		return;
	}

	// 画面フェーズ中の場合、UIの更新はしない
	if (SCENES.GetIsFading())	return;

	// フレーム初めにボタン移動フラグをfalseに設定
	this->SetButtonCursorMoved(false);

	for (auto it = m_UiList.begin(); it != m_UiList.end(); )
	{
		(*it)->Update();

		if (!(*it)->GetActive())
		{
			(*it)->Finalize();

			delete (*it);

			it = m_UiList.erase(it);

			continue;
		}

		++it;
	}
}

void CUiManager::DrawFront()
{
	for (auto it = m_UiList.begin(); it != m_UiList.end(); )
	{
		if ((*it)->GetUiDrawPos() == UI_DRAW_POS::FRONT)
			(*it)->Draw();

		++it;
	}
}

void CUiManager::DrawMiddle()
{
	for (auto it = m_UiList.begin(); it != m_UiList.end(); )
	{
		if ((*it)->GetUiDrawPos() == UI_DRAW_POS::MIDDLE)
			(*it)->Draw();

		++it;
	}
}

void CUiManager::DrawBack()
{
	for (auto it = m_UiList.begin(); it != m_UiList.end(); )
	{
		if ((*it)->GetUiDrawPos() == UI_DRAW_POS::BACK)
			(*it)->Draw();

		++it;
	}
}

void CUiManager::Finalize()
{
	for (auto it = m_UiList.begin(); it != m_UiList.end(); )
	{
		(*it)->Finalize();

		delete (*it);

		it = m_UiList.erase(it);
	}

	m_UiList.clear();
}

IUi* CUiManager::Create(UI_ID id, const vivid::Vector2& pos, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id)
{
	IUi* ui = nullptr;

	ui = m_Generator[id]();

	if (ui != nullptr)
	{
		ui->Initialize(pos, id, ui_draw, player_id);

		m_UiList.push_back(ui);

		return ui;
	}

	return nullptr;
}

CNumberUi* CUiManager::CreateNumber(
	UI_ID id, 
	const vivid::Vector2& pos, 
	int size, 
	int digit, 
	UI_DRAW_POS ui_draw, 
	contol::PLAYER_ID player_id)
{
	IUi* ui = m_Generator[id]();

	if (ui != nullptr)
	{
		// uiをCNumberUiにキャストして初期化
		if (auto number_ui = dynamic_cast<CNumberUi*>(ui))
		{
			number_ui->Initialize(pos, id, ui_draw, size, digit, player_id);
			m_UiList.push_back(ui);

			return number_ui;
		}
		else
		{
			ui->Initialize(pos, id, ui_draw, player_id);
			m_UiList.push_back(ui);

			return number_ui;
		}
	}

	return nullptr;
}

CButtonUi* CUiManager::CreateButton(
	UI_ID id, 
	const vivid::Vector2& pos, 
	int width, int height, 
	std::function<void()> on_click,
	unsigned int color,
	std::string image_data,
	float image_scale,
	UI_DRAW_POS ui_draw, 
	contol::PLAYER_ID player_id)
{
	IUi* ui = m_Generator[id]();

	if (ui != nullptr)
	{
		// uiをCButtonUiにキャストして初期化
		if (auto button_ui = dynamic_cast<CButtonUi*>(ui))
		{
			button_ui->Initialize(pos, id, width, height, image_data, image_scale,
				color, ui_draw, player_id, on_click);
			m_UiList.push_back(ui);

			return button_ui;
		}
		else
		{
			ui->Initialize(pos, id, ui_draw, player_id);
			m_UiList.push_back(ui);

			return button_ui;
		}
	}

	return nullptr;
}

CBarUi* CUiManager::CreateBar(UI_ID id, const vivid::Vector2& pos, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id)
{
	IUi* ui = m_Generator[id]();

	if (ui != nullptr)
	{
		// uiをCBarUiにキャストして初期化
		if (auto bar_ui = dynamic_cast<CBarUi*>(ui))
		{
			bar_ui->Initialize(pos, id, ui_draw, player_id);
			m_UiList.push_back(ui);

			return bar_ui;
		}
		else
		{
			ui->Initialize(pos, id, ui_draw, player_id);
			m_UiList.push_back(ui);

			return bar_ui;
		}
	}

	return nullptr;
}

void CUiManager::DeleteUi(UI_ID id)
{
	for (auto it = m_UiList.begin(); it != m_UiList.end(); )
	{
		if ((*it)->GetUiID() == id)
		{
			(*it)->Finalize();

			delete (*it);

			it = m_UiList.erase(it);

			continue;
		}
		++it;
	}
}

void CUiManager::SetButtonCursorMoved(bool flag)
{
	m_ButtonMoved = flag;
}

void CUiManager::SetModeSceneUiMove()
{
	m_ModeUiDrawAngle = 180;
}
