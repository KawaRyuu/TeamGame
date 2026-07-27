#include "chara1.h"


CChara1::CChara1()
	: IChara(32, 32, "data\\ball.png")
{
}

void CChara1::Initialize(const vivid::Vector2& pos, CHARA_ID id)
{
	// èâä˙âª
	IChara::Initialize(pos, id);
}

void CChara1::Update()
{
	IChara::Update();
	m_Position.x += 1.0f;
}

void CChara1::Draw()
{
	IChara::Draw();
}
