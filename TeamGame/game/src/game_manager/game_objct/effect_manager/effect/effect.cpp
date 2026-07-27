#include "effect.h"

const int IEffect::m_out_of_range_time = 3 * 60;

IEffect::IEffect(int width, int height, std::string data)
	:m_Width(width)
	, m_Height(height)
	, m_DrawData(data)
	, m_Position(vivid::Vector2::ZERO)
	, m_Color(0xffffffff)
	, m_Rect({ 0, 0, width, height })
	, m_Anchor(vivid::Vector2((float)width / 2.0f, (float)height / 2.0f))
	, m_Scale(vivid::Vector2::ONE)
	, m_Rotation(0.0f)
	, m_Active(true)
	, m_EffectID(EFFECT_ID::DUMMY)
	, m_Velocity(vivid::Vector2::ZERO)
	, m_AutoDeleteTimer(10)
{
}

void IEffect::Initialize(const vivid::Vector2& pos, EFFECT_ID id, unsigned int color)
{
	m_Position = pos - vivid::Vector2((float)m_Width / 2.0f, (float)m_Height / 2.0f);
	m_EffectID = id;
	m_Color = color;

	m_Active = true;
}

void IEffect::Update()
{
	this->AutoDeleteEffect();
}

void IEffect::Draw()
{
	vivid::DrawTexture(m_DrawData, m_Position, m_Color);
}

void IEffect::Finalize()
{
}

bool IEffect::GetActive()
{
	return m_Active;
}

void IEffect::SetActive(bool active)
{
	m_Active = active;
}

EFFECT_ID IEffect::GetEffectID()
{
	return m_EffectID;
}

void IEffect::AutoDeleteEffect()
{
	// カウントダウン処理
	if (m_AutoDeleteTimer > 0)
	{
		m_AutoDeleteTimer--;
		return;
	}

	// タイマーが0以下なら削除
	this->SetActive(false);
}

float IEffect::GetRadius()
{
	float	w = (float)m_Width / 2.0f;
	float	h = (float)m_Height / 2.0f;

	return (w + h) / 2.0f;
}

