#include "chara.h"
#include "contol.h"
#include "chara_state.h"

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
	, m_CharaState(CHARA_STATE::WAIT)
{
	this->SetParameter(0, 0, 0.0f, 0.0f);
}

void IChara::Initialize(const vivid::Vector2& pos, CHARA_ID id)
{
	m_Position = pos - vivid::Vector2((float)m_Width / 2.0f, (float)m_Height / 2.0f);
	m_CharaID = id;

	m_Active = true;
}

void IChara::Update()
{
	this->StateChange(m_CharaState);
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

void IChara::SetParameter(int max_hp, int attack, float defence, float speed)
{
	m_MaxHp = max_hp;
	m_CurrentHp = m_MaxHp;
	m_BaseAttack = attack;
	m_Defence = defence;
	m_Speed = speed;
}

void IChara::StateChange(CHARA_STATE state)
{
	switch (state)
	{
	case CHARA_STATE::WAIT:
		this->Wait();
		break;
	case CHARA_STATE::MOVE:
		this->Move();
		break;
	case CHARA_STATE::GUARD:
		this->Guard();
		break;
	case CHARA_STATE::SKILL:
		this->Skill();
		break;
	case CHARA_STATE::USE_ITEM:
		this->UseItem();
		break;
	case CHARA_STATE::NOMAL_ATTACK:
		this->NormalAttack();
		break;
	case CHARA_STATE::SURE_KILL:
		this->SureKill();
	case CHARA_STATE::RESURRECTION:
		this->Resurrection();
		break;
	}
}

void IChara::Wait()
{
	// 待機処理
}

void IChara::Move()
{
	m_Velocity = vivid::Vector2(contol::GetStickValue(contol::PLAYER_ID::ALL,
		contol::LR_ID::L, contol::XY_ID::X) * m_Speed,
		contol::GetStickValue(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::XY_ID::Y) * m_Speed);
}

void IChara::Guard()
{
	// ガード処理
}

void IChara::Skill()
{
	// 特性処理
}

void IChara::UseItem()
{
	//アイテムを使用する処理
}

void IChara::NormalAttack()
{
	// 通常攻撃処理
}

void IChara::SureKill()
{
	// 必殺技処理
}


void IChara::Resurrection()
{
	// 復活処理
}

