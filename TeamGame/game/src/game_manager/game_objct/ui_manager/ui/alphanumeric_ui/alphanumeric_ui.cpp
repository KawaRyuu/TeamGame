#include "alphanumeric_ui.h"

CAlphanumericUi::CAlphanumericUi()
	: IUi(2560, 1280, "data/ui/alphanumeric.png")
	, m_AlphanumericNum(0)
{
}

void CAlphanumericUi::Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id)
{
	IUi::Initialize(pos, id, ui_draw, player_id);
	this->SetAlphanumericNum(0);
	m_Anchor = vivid::Vector2::ZERO;
}

void CAlphanumericUi::Update()
{
}

void CAlphanumericUi::Draw()
{
	vivid::DrawTexture("data/ui/text_bg.png", m_Position, m_Color, { 0,0,256,256 }, m_Anchor, m_Scale);
	vivid::DrawTexture(m_DrawData, m_Position, m_Color, m_Rect, m_Anchor, m_Scale);
}

void CAlphanumericUi::SetAlphanumericNum(int num)
{
	m_AlphanumericNum = num;

	// ï`âÊà íuÇÃïœçX
	m_Rect.top = (m_Height / 5) * (m_AlphanumericNum / 10);
	m_Rect.bottom = m_Rect.top + (m_Height / 5);
	m_Rect.left = (m_Width / 10) * (m_AlphanumericNum % 10);
	m_Rect.right = m_Rect.left + (m_Width / 10);
}

void CAlphanumericUi::SetFontSize(int size)
{
	float scale = (float)size / (float)(m_Height / 5);

	m_Scale = { scale, scale };
}

