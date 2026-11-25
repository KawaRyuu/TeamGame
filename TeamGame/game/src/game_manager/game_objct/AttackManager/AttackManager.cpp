#include "../AttackManager/AttackManager.h"
#include <cmath>

static bool CircleIntersectsRect(const vivid::Vector2& center, float radius, const vivid::Rect& rect) {
	float closestX = center.x;
	float closestY = center.y;

	if (center.x < rect.left) closestX = static_cast<float>(rect.left);
	else if (center.x > rect.right) closestX = static_cast<float> (rect.right);

	if (center.y < rect.top) closestY = static_cast<float>(rect.top);
	else if (center.y > rect.bottom) closestY = static_cast<float>(rect.bottom);

	float dx = center.x - closestX;
	float dy = center.y - closestY;
	float distSq = dx * dx + dy * dy;
	return distSq <= radius * radius;
}

void AttackManager::Update(const std::vector<IChara*>& targets) {
	for (int i = (int)m_active.size() - 1; i >= 0; --i) {
		AttackInstance& inst = m_active[i];

		//UŒ‚ŽÒ‚ÌŒü‚«‚ð’T‚·(Œ©‚Â‚©‚ç‚È‚¯‚ê‚Î‰EŒü‚«)
		bool facingRight = true;
		for (IChara* c : targets) {
			if (!c) continue;
			if (c->GetCharaID() == inst.owner) {
				facingRight = c->IsFacingRight();
				break;
			}
		}

		if (inst.isActiveFrame()) {
			vivid::Vector2 center = inst.getWorldCenter(facingRight);
			float r = inst.getRadius();

			for (IChara* t : targets) {
				if (!t) continue;
				if (t->GetCharaID() == inst.owner) continue; // UŒ‚ŽÒ‚Í–³Ž‹
				if (!t->GetActive()) continue;				 // ”ñƒAƒNƒeƒBƒu‚Í–³Ž‹
				if (t->IsInvulnerable()) continue;

				vivid::Rect hurt = t->GetHurtbox();
				if (CircleIntersectsRect(center, r, hurt)) {
					if (inst.atk.oneHitPerTarget) {
						if (inst.alreadyHit.find((int)t->GetCharaID()) != inst.alreadyHit.end()) continue;
						inst.alreadyHit.insert((int)t->GetCharaID());
					}
					t->ApplyHit(inst.atk.damage, inst.atk.knockback, inst.owner);
				}
			}
		}
		inst.frame++;
		if (inst.finished()) {
			m_active[i] = m_active.back();
			m_active.pop_back();
		}
	}
}