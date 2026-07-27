#include "test_button_ui.h"

CTestButtonUi::CTestButtonUi()
	: CButtonUi(500, 500, "data/ui/button.png")
{
}

void CTestButtonUi::Initialize(
	const vivid::Vector2& pos, 
	UI_ID id, 
	int width, int height, 
	std::string image_data,
	float image_scale,
	unsigned int color, 
	UI_DRAW_POS ui_draw, 
	contol::PLAYER_ID player_id,
	std::function<void()> on_click)
{
	CButtonUi::Initialize(pos, id, width, height, image_data, image_scale,
		color, ui_draw, player_id, on_click);
}

void CTestButtonUi::Update()
{
	CButtonUi::Update();
}

void CTestButtonUi::Draw()
{
	CButtonUi::Draw();
}
