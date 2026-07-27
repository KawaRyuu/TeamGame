#include "ui.h"

IUi::IUi(int width, int height, std::string data)
	: m_Width(width)
	, m_Height(height)
	, m_DrawData(data)
	, m_Position(vivid::Vector2::ZERO)
	, m_Color(0xffffffff)
	, m_Rect({ 0, 0, width, height })
	, m_Anchor(vivid::Vector2((float)width / 2.0f, (float)height / 2.0f))
	, m_Scale(vivid::Vector2::ONE)
	, m_Rotation(0.0f)
	, m_Active(true)
	, m_UiID(UI_ID::DUMMY)
	, m_Velocity(vivid::Vector2::ZERO)
	, m_OutOfRangeTimer(0)
	, m_UiDrawPos(UI_DRAW_POS::DUMMY)
	, m_PlayerID(contol::PLAYER_ID::MAX)
{
}

void IUi::Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id)
{
	m_Position = pos;
	m_UiID = id;
	m_UiDrawPos = ui_draw;
	m_PlayerID = player_id;

	m_Active = true;
}

void IUi::Update()
{
}

void IUi::Draw()
{
	vivid::DrawTexture(m_DrawData, m_Position);
}

void IUi::Finalize()
{
}

bool IUi::GetActive()
{
	return m_Active;
}

void IUi::SetActive(bool active)
{
	m_Active = active;
}

UI_ID IUi::GetUiID()
{
	return m_UiID;
}

UI_DRAW_POS IUi::GetUiDrawPos()
{
	return m_UiDrawPos;
}

