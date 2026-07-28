#include "chara.h"
#include "contol.h"

IChara::IChara(int width, int height, std::string data)
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
	, m_CharaID(CHARA_ID::MAX)
	, m_Velocity(vivid::Vector2::ZERO)
{
}

void IChara::Initialize(const vivid::Vector2& pos, CHARA_ID id)
{
	m_Position = pos - vivid::Vector2((float)m_Width / 2.0f, (float)m_Height / 2.0f);
	m_CharaID = id;

	m_Active = true;
}

void IChara::Update()
{
	this->Move();
}

void IChara::Draw()
{
	vivid::DrawTexture(m_DrawData, m_Position, m_Color);
}

void IChara::Finalize()
{
}

bool IChara::GetActive()
{
	return m_Active;
}

void IChara::SetActive(bool active)
{
	m_Active = active;
}

CHARA_ID IChara::GetCharaID()
{
	return m_CharaID;
}

void IChara::Move()
{
	m_Velocity = vivid::Vector2(contol::GetStickValue(contol::PLAYER_ID::ALL,
		contol::LR_ID::L, contol::XY_ID::X) * 10.0f,
		contol::GetStickValue(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::XY_ID::Y) * 10.0f);
}

