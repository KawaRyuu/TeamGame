#include "title.h"
#include "contol.h"
#include "../../../sound_manager/sound_manager.h"

CTitle::CTitle()
{
}

void CTitle::Initialize()
{
	// BGMÇÃçƒê∂
	SOUNDS.Play(SOUND_ID::TITLE_BGM, true);
}

void CTitle::Update()
{
	if (contol::InputAnyKey(contol::PLAYER_ID::ALL))
	{
		SCENES.SetNextScene(SCENE_ID::MODE);

		// SEÇÃçƒê∂
		SOUNDS.Play(SOUND_ID::TEST_SE);
	}

	contol::SetPlayerDevice();

#ifdef VIVID_DEBUG
#endif // DEBUG
}

void CTitle::Draw()
{
	vivid::DrawText(150, "Ç–Ç∆Ç”Ç≈ÉäÉìÉN", { 400.0f,300.0f });
	vivid::DrawText(50, "[ L + R ] Ç≈énÇﬂÇÈ", { 700.0f,700.0f });

#ifdef VIVID_DEBUG
	vivid::DrawText(30, "Title", vivid::Vector2::ZERO);
#endif // DEBUG

}

void CTitle::Finalize()
{
	// BGMÇÃí‚é~
	SOUNDS.Stop(SOUND_ID::TITLE_BGM);
}
