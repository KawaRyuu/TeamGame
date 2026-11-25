#pragma once
#include <vector>
#include <unordered_set>
#include "vivid.h"
#include "../chara_manager/chara/chara.h"

/**
*	@brief	攻撃の情報を管理する構造体
*	@param		damage			攻撃力
*	@param		startUpFrames	攻撃の発生フレーム数
* 	@param		activeFrames	当たり判定の有効フレーム数
* 	@param		recoveryFrames	回復フレーム数
*/
struct Attack {
	int damage = 10;
	int startUpFrames = 2;
	int activeFrames = 3;
	int recoveryFrames = 5;
	vivid::Vector2 hitCenterLocal = vivid::Vector2(12.0f, 0.0f);// 当たり判定の中心位置（ローカル座標x,y）
	float hitRadius = 10.0f; // 当たり判定の半径
	vivid::Vector2 knockback = vivid::Vector2(100.0f, -50.0f);
	bool oneHitPerTarget = true;
};

struct AttackInstance {
	Attack atk;
	CHARA_ID owner = CHARA_ID::MAX;
	vivid::Vector2 origin = vivid::Vector2::ZERO; // ワールド基準の原点(通常はキャラ中心)
	int frame = 0;
	std::unordered_set<int> alreadyHit;

	bool isActiveFrame() const {
		return frame >= atk.startUpFrames && frame < (atk.startUpFrames + atk.activeFrames);
	}

	bool finished() const {
		return frame >= (atk.startUpFrames + atk.activeFrames + atk.recoveryFrames);
	}

	vivid::Vector2 getWorldCenter(bool facinRight)const {
		if (facinRight) return vivid::Vector2(origin.x + atk.hitCenterLocal.x,
			origin.y + atk.hitCenterLocal.y);
		return vivid::Vector2(origin.x - atk.hitCenterLocal.x, origin.y + atk.hitCenterLocal.y);
	}
	float getRadius()const { return atk.hitRadius; }
};

class AttackManager
{
public:
	static AttackManager& Instance() {
		static AttackManager s;
		return s;
	}

	void Update(const std::vector<IChara*>& targets);

	void Spawn(const Attack& atk, CHARA_ID owner, const vivid::Vector2& origin) {
		AttackInstance inst;
		inst.atk = atk;
		inst.owner = owner;
		inst.origin = origin;
		inst.frame = 0;
		m_active.push_back(std::move(inst));
	}
	
	const std::vector<AttackInstance>& GetActive() const { return m_active; }

private:
	AttackManager() = default;
	std::vector<AttackInstance> m_active;
};
