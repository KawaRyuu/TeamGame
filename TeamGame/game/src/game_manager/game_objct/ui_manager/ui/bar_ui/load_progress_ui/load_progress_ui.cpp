#include "load_progress_ui.h"

const std::string CLoadProgressUi::m_bar_data = "data/ui/bar/load_progress/bar.png";
const std::string CLoadProgressUi::m_bar_frame_data = "data/ui/bar/load_progress/bar_frame.png";

CLoadProgressUi::CLoadProgressUi()
	: CBarUi(1080, 64, m_bar_data)
{
}

void CLoadProgressUi::Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id)
{
	CBarUi::Initialize(pos, id, ui_draw, player_id);
	m_Rect = { 0,0,0,m_Height };
}

void CLoadProgressUi::Update()
{
	CBarUi::Update();
}

void CLoadProgressUi::Draw()
{
	// バーの描画
	vivid::DrawTexture(m_bar_data, m_Position, 0xffa0a0a0);
	vivid::DrawTexture(m_bar_data, m_Position, 0xffff80ff, m_Rect);
	vivid::DrawTexture(m_bar_frame_data, m_Position);

	// 仮パーセント表示
	std::string str = std::to_string((int)std::round(m_Rate * 100.0f)) + "%";

	// 仮「Now Loading...」表示
	vivid::DrawText(m_Height / 2, "Now Loading... : " + str, m_Position + vivid::Vector2(0.0f, (float)(-m_Height / 2)));
}
