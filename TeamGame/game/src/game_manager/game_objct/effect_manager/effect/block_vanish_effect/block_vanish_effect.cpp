#include "block_vanish_effect.h"

const float CBlockVanishEffect::m_speed = 6.0f;
const float CBlockVanishEffect::m_decelerator = 0.9f;
const float CBlockVanishEffect::m_scale_rate = 0.995f;

CBlockVanishEffect::CBlockVanishEffect()
	: IEffect(32, 32, "data\\ball.png")
{
}

void CBlockVanishEffect::Initialize(const vivid::Vector2& pos, EFFECT_ID id, unsigned int color)
{
	// 初期化
	IEffect::Initialize(pos, id, color);
	m_AutoDeleteTimer = 30;

	for (int i = 0; i < m_max_particle; i++)
	{
		// 位置の初期化
		m_Positions[i] = vivid::Vector2::ZERO;

		// 8方向に飛ぶよう速度を設定
		float angle = (float)(i * 45) * 3.14f / 180.0f;
		m_Velocitys[i] = vivid::Vector2(cos(angle) * m_speed, sin(angle) * m_speed);
	}
}

void CBlockVanishEffect::Update()
{
	IEffect::Update();

	for (int i = 0; i < m_max_particle; i++)
	{
		// 速度の更新（減速）
		m_Velocitys[i].x *= m_decelerator;
		m_Velocitys[i].y *= m_decelerator;
		// 位置の更新
		m_Positions[i] += m_Velocitys[i];

		// スケールの更新（縮小）
		m_Scale.x *= m_scale_rate;
		m_Scale.y *= m_scale_rate;
	}
}

void CBlockVanishEffect::Draw()
{
	for (int i = 0; i < m_max_particle; i++)
	{
		vivid::DrawTexture(m_DrawData, m_Position + m_Positions[i], m_Color, m_Rect, m_Anchor, m_Scale);
	}
}
