#include "load.h"
#include "../../../ui_manager/ui_manager.h"

const int	CLoad::m_load_data_count = (int)SOUND_ID::MAX;

CLoad::CLoad()
	: m_LoadDataNum(0)
	, m_LoadProgressUi(nullptr)
{
}

void CLoad::Initialize()
{
	// UIの生成
	m_LoadProgressUi = UIS.CreateBar(UI_ID::LOAD_PROGRESS_UI, 
		{(float)vivid::WINDOW_WIDTH / 2.0f - 540.0f,(float)vivid::WINDOW_HEIGHT / 4.0f * 3.0f});
}

void CLoad::Update()
{
	// 既にロードが終了している場合は処理しない
	if (m_LoadDataNum >= m_load_data_count)	return;

	// 音源をロードする
	SOUNDS.Load((SOUND_ID)m_LoadDataNum);

	// 番号カウントアップ
	m_LoadDataNum++;

	// 全てロードし終わったらタイトルシーンへ
	if (m_LoadDataNum >= m_load_data_count)
	{
		SCENES.SetNextScene(SCENE_ID::TITLE);
	}

	// プログレスバーの更新
	m_LoadProgressUi->SetBarRate((float)m_LoadDataNum / (float)m_load_data_count);
}

void CLoad::Draw()
{

#ifdef VIVID_DEBUG
	vivid::DrawText(30, "Load", vivid::Vector2::ZERO);
#endif // DEBUG

}

void CLoad::Finalize()
{
}
