#include "chara_manager.h"
#include "chara/chara1/chara1.h"


CCharaManager& CCharaManager::GetInstance()
{
	static CCharaManager instance;

	return instance;
}

void CCharaManager::Initialize()
{
	m_Generator[CHARA_ID::CHARA_1] = []() { return new CChara1(); };
	m_Generator[CHARA_ID::MAX] = []() { return nullptr; };
	m_CharaList.clear();
}

void CCharaManager::Update()
{
	for (auto it = m_CharaList.begin(); it != m_CharaList.end(); )
	{
		(*it)->Update();

		if (!(*it)->GetActive())
		{
			(*it)->Finalize();

			delete (*it);

			it = m_CharaList.erase(it);

			continue;
		}

		++it;
	}
}

void CCharaManager::Draw()
{
	for (auto it = m_CharaList.begin(); it != m_CharaList.end(); )
	{
		(*it)->Draw();

		++it;
	}
}

void CCharaManager::Finalize()
{
	for (auto it = m_CharaList.begin(); it != m_CharaList.end(); )
	{
		(*it)->Finalize();

		delete (*it);

		it = m_CharaList.erase(it);
	}

	m_CharaList.clear();
}

void CCharaManager::Create(CHARA_ID id, const vivid::Vector2& pos)
{
	IChara* chara = nullptr;

	chara = m_Generator[id]();

	if (chara != nullptr)
	{
		chara->Initialize(pos, id);

		m_CharaList.push_back(chara);
	}
}
