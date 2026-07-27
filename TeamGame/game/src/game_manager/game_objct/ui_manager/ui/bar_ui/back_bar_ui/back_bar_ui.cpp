#include "back_bar_ui.h"

const std::string CBackBarUi::m_bar_data = "data/ui/bar/back_bar/bar.png";
const std::string CBackBarUi::m_bar_frame_data = "data/ui/bar/back_bar/bar_frame.png";

CBackBarUi::CBackBarUi()
	: CBarUi(256, 64, m_bar_data)
{
}

void CBackBarUi::Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id)
{
	CBarUi::Initialize(pos, id, ui_draw, player_id);
	m_Rect = { 0,0,0,m_Height };
}

void CBackBarUi::Update()
{
	CBarUi::Update();
}

void CBackBarUi::Draw()
{
	// ÉoÅ[ÇÃï`âÊ
	vivid::DrawTexture(m_bar_data, m_Position, 0xffa0a0a0);
	vivid::DrawTexture(m_bar_data, m_Position, 0xffffff80, m_Rect);
	vivid::DrawTexture(m_bar_frame_data, m_Position);
}
