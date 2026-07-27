#include "bar_ui.h"
#include "contol.h"
#include "../../ui_manager.h"

CBarUi::CBarUi(int width, int height, std::string data)
	: IUi(width, height, data)
	, m_Rate(0.0f)
{
}

void CBarUi::Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id)
{
	// IUi‚Ì‰Šú‰»
	IUi::Initialize(pos, id, ui_draw, player_id);
}

void CBarUi::Update()
{
	// •`‰æ”ÍˆÍ‚Ìİ’è
	m_Rect.left = 0;
	m_Rect.right = (int)((float)m_Width * m_Rate);
}

void CBarUi::Draw()
{
}

void CBarUi::SetBarRate(float rate)
{
	// Š„‡‚Ì•ÏX
	m_Rate = rate;
}
