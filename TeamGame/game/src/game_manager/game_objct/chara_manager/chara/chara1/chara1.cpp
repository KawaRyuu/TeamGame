#include "chara1.h"

const int	CChara1::m_max_hp = 100;
const int	CChara1::m_base_attack = 20;
const float	CChara1::m_defence = 1.0f;
const float	CChara1::m_speed = 5.0f;

CChara1::CChara1()
	: IChara(32, 32, "data\\ball.png")
{
	this->SetParameter(m_max_hp, m_base_attack, m_defence, m_speed);
}

void CChara1::Initialize(const vivid::Vector2& pos, CHARA_ID id)
{
	// ‰Šú‰»
	IChara::Initialize(pos, id);
}

void CChara1::Update()
{
	IChara::Update();
	m_Position += m_Velocity;
}

void CChara1::Draw()
{
	IChara::Draw();
}
