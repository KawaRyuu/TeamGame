#include "effect_manager.h"
#include "effect/block_vanish_effect/block_vanish_effect.h"


CEffectManager& CEffectManager::GetInstance()
{
	static CEffectManager instance;

	return instance;
}

void CEffectManager::Initialize()
{
	m_Generator[EFFECT_ID::BLOCK_VANISH]	= []() { return new CBlockVanishEffect(); };
	m_Generator[EFFECT_ID::DUMMY]			= []() { return nullptr; };
	m_EffectList.clear();
}

void CEffectManager::Update()
{
	for (auto it = m_EffectList.begin(); it != m_EffectList.end(); )
	{
		(*it)->Update();

		if (!(*it)->GetActive())
		{
			(*it)->Finalize();

			delete (*it);

			it = m_EffectList.erase(it);

			continue;
		}

		++it;
	}
}

void CEffectManager::Draw()
{
	for (auto it = m_EffectList.begin(); it != m_EffectList.end(); )
	{
		(*it)->Draw();

		++it;
	}
}

void CEffectManager::Finalize()
{
	for (auto it = m_EffectList.begin(); it != m_EffectList.end(); )
	{
		(*it)->Finalize();

		delete (*it);

		it = m_EffectList.erase(it);
	}

	m_EffectList.clear();
}

void CEffectManager::Create(EFFECT_ID id, const vivid::Vector2& pos, unsigned int color)
{
	IEffect* effect = nullptr;

	effect = m_Generator[id]();

	if (effect != nullptr)
	{
		effect->Initialize(pos, id, color);

		m_EffectList.push_back(effect);
	}
}

void CEffectManager::DeleteEffect(EFFECT_ID id)
{
	for (auto it = m_EffectList.begin(); it != m_EffectList.end(); )
	{
		if ((*it)->GetEffectID() == id)
		{
			(*it)->Finalize();

			delete (*it);

			it = m_EffectList.erase(it);

			continue;
		}
		++it;
	}
}
