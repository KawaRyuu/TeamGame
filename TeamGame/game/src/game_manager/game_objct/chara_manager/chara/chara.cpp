#include "chara.h"
#include "contol.h"
#include "chara_state.h"
#include "../../AttackManager/AttackManager.h"
//#include <cstdio>
#include <DxLib.h>

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
	if (contol::InputStick(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::UP)
		|| contol::InputStick(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::DOWN)
		|| contol::InputStick(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::LEFT)
		|| contol::InputStick(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::RIGHT))
		m_CharaState = CHARA_STATE::MOVE;
	else m_CharaState = CHARA_STATE::WAIT;

	if (contol::InputStart(contol::PLAYER_ID::ALL, contol::BUTTON_ID::ACTION_DOWN))
		m_CharaState = CHARA_STATE::NOMAL_ATTACK;
	
	this->StateChange(m_CharaState);

	if (m_DebugAttackTimer > 0)--m_DebugAttackTimer;
}

void IChara::ApplyHit(int damage, const vivid::Vector2& knockback, CHARA_ID attackerId)
{
	m_CurrentHp -= damage;
	m_Velocity += knockback;
	std::printf("ApplyHit: target=%d attacker=%d dmg=%d hp=%d\n",
		(int)m_CharaID, (int)attackerId, damage, m_CurrentHp);

	if (m_CurrentHp <= 0)
	{
		m_CurrentHp = 0;
		m_Active = false;
	}
}

void IChara::Draw()
{
	vivid::DrawTexture(m_DrawData, m_Position, m_Color);
	vivid::Vector2 origin = m_Position + vivid::Vector2((float)m_Width / 2.0f, (float)m_Height / 2.0f);
	vivid::DrawText(50, std::to_string(origin.x) + std::to_string(origin.y), vivid::Vector2::ZERO);

	//被弾判定の表示
	vivid::Rect hb = GetHurtbox();
	int hl = (int)hb.left;
	int ht = (int)hb.top;
	int hr = (int)hb.right;
	int hb_b = (int)hb.bottom;

	DrawBox(hl, ht, hr, hb_b, GetColor(255, 0, 0), TRUE);//赤(塗りつぶし)
	DrawBox(hl, ht, hr, hb_b, GetColor(255, 200, 200), FALSE);//薄め赤(枠)

	if (m_DebugAttackTimer > 0) {
		int left	= (int)std::floor(m_DebugAttackCenter.x - m_DebugAttackRadius);
		int top		= (int)std::floor(m_DebugAttackCenter.y - m_DebugAttackRadius);
		int right	= (int)std::ceil(m_DebugAttackCenter.x + m_DebugAttackRadius);
		int bottom	= (int)std::ceil(m_DebugAttackCenter.y + m_DebugAttackRadius);

		// 塗り青（見やすさ重視で不透明）
		DrawBox(left, top, right, bottom, GetColor(0, 0, 200), TRUE);
		DrawBox(left, top, right, bottom, GetColor(150, 150, 255), FALSE);
	}
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

vivid::Rect IChara::GetHurtbox() const
{
	vivid::Rect box;
	box.left = (int)m_Position.x;
	box.top = (int)m_Position.y;
	box.right = (int)m_Position.x + m_Width;
	box.bottom = (int)m_Position.y + m_Height;
	return box;
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
	m_Velocity = vivid::Vector2::ZERO;
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
	//通常攻撃のデータを作成（仮）
	Attack atk;
	atk.damage = m_BaseAttack;
	atk.startUpFrames = 1;
	atk.activeFrames = 3;
	atk.recoveryFrames = 6;
	atk.hitCenterLocal = vivid::Vector2(12.0f, 0.0f);//キャラ中心の右に出す（仮）
	atk.hitRadius = 12.0f;//半径
	atk.knockback = vivid::Vector2(150.0f, -60.0f);
	atk.oneHitPerTarget = true;

	vivid::Vector2 origin = m_Position + vivid::Vector2((float)m_Width / 2.0f, (float)m_Height / 2.0f);
	AttackManager::Instance().Spawn(atk, this->GetCharaID(), origin);

	float dir = IsFacingRight() ? 1.0f : -1.0f;
	m_DebugAttackCenter = origin + vivid::Vector2(atk.hitCenterLocal.x * dir, atk.hitCenterLocal.y);
	m_DebugAttackRadius = atk.hitRadius;
	m_DebugAttackTimer = atk.startUpFrames + atk.activeFrames + atk.recoveryFrames;
}

void IChara::SureKill()
{
	// 必殺技処理
}


void IChara::Resurrection()
{
	// 復活処理
}

