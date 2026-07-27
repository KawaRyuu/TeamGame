#include "test_button_ui2.h"

CTestButtonUi2::CTestButtonUi2()
	: CButtonUi(500, 500, "data/ui/button2.png")
{
}

void CTestButtonUi2::Initialize(
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

void CTestButtonUi2::Update()
{
	CButtonUi::Update();
}

void CTestButtonUi2::Draw()
{
	CButtonUi::Draw();
}
