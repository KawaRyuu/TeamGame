#include "button_ui.h"
#include "contol.h"
#include "../../ui_manager.h"
#include "../../../sound_manager/sound_manager.h"

CButtonUi::CButtonUi(int width, int height, std::string data)
	: IUi(width, height, data)
	, m_ButtonWidth(0)
	, m_ButtonHeight(0)
	, m_ButtonImageData("")
	, m_ImagePosition(vivid::Vector2::ZERO)
	, m_ImageScale(vivid::Vector2::ZERO)
	, m_ImageScaleRate(0.0f)
	, m_OnClick(nullptr)
	, m_Selecting(false)
	, m_Left(nullptr)
	, m_Up(nullptr)
	, m_Right(nullptr)
	, m_Down(nullptr)
{
}

void CButtonUi::Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id)
{
	// IUiの初期化
	IUi::Initialize(pos, id, ui_draw, player_id);
}

void CButtonUi::Initialize(
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
	// 変数の初期化
	m_Color = color;
	m_ButtonWidth = width;
	m_ButtonHeight = height;
	m_ButtonImageData = image_data;
	m_ImageScaleRate = image_scale;
	m_OnClick = on_click;
	m_Anchor = vivid::Vector2::ZERO;

	// CButtonUiの初期化
	this->Initialize(pos, id, ui_draw, player_id);

	// サイズの変更
	this->SetButtonSize();
	this->SetImageSize();
}

void CButtonUi::Update()
{
	if (!m_Selecting)	return;

	if (!UIS.GetButtonCursorMoved())
	{
		this->MoveSelect();
	}

	if (this->IsClicked()) 
	{ 
		if (m_OnClick)	m_OnClick();
	}
}

void CButtonUi::Draw()
{
	unsigned int color;
	if (m_Selecting)	color = m_Color;
	else				color = this->MultiplyColor(m_Color, 0.8f);

	vivid::DrawTexture(m_DrawData, m_Position, color, m_Rect, m_Anchor, m_Scale);

	if (m_Selecting)	color = 0xffffffff;
	else				color = this->MultiplyColor(0xffffffff, 0.8f);

	if (m_ButtonImageData != "")
		vivid::DrawTexture(m_ButtonImageData, m_ImagePosition, color, m_Rect, m_Anchor, m_ImageScale);
}

void CButtonUi::SetNeighbor(CButtonUi* left, CButtonUi* up, CButtonUi* right, CButtonUi* down)
{
	m_Left = left;
	m_Up = up;
	m_Right = right;
	m_Down = down;
}

void CButtonUi::SetButtonActive(bool flag)
{
	m_Selecting = flag;
}

bool CButtonUi::IsClicked()
{
	return contol::InputEnd(contol::PLAYER_ID::ALL, contol::BUTTON_ID::ACTION_DOWN);
}

void CButtonUi::MoveSelect()
{
	if ((contol::InputDPadStart(contol::PLAYER_ID::ALL, contol::D_PAD_ID::LEFT)
		|| contol::InputStickStart(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::LEFT))
		&& m_Left)
	{
		UIS.SetButtonCursorMoved();
		m_Left->SetButtonActive();
		this->SetButtonActive(false);

		// SEの再生
		//SOUNDS.Play(SOUND_ID::MOVE_CURSOR);
	}
	else if ((contol::InputDPadStart(contol::PLAYER_ID::ALL, contol::D_PAD_ID::UP)
		|| contol::InputStickStart(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::UP))
		&& m_Up)
	{
		UIS.SetButtonCursorMoved();
		m_Up->SetButtonActive();
		this->SetButtonActive(false);

		// SEの再生
		//SOUNDS.Play(SOUND_ID::MOVE_CURSOR);
	}
	else if ((contol::InputDPadStart(contol::PLAYER_ID::ALL, contol::D_PAD_ID::RIGHT)
		|| contol::InputStickStart(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::RIGHT))
		&& m_Right)
	{
		UIS.SetButtonCursorMoved();
		m_Right->SetButtonActive();
		this->SetButtonActive(false);

		// SEの再生
		//SOUNDS.Play(SOUND_ID::MOVE_CURSOR);
	}
	else if ((contol::InputDPadStart(contol::PLAYER_ID::ALL, contol::D_PAD_ID::DOWN)
		|| contol::InputStickStart(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::DOWN))
		&& m_Down)
	{
		UIS.SetButtonCursorMoved();
		m_Down->SetButtonActive();
		this->SetButtonActive(false);

		// SEの再生
		//SOUNDS.Play(SOUND_ID::MOVE_CURSOR);
	}
}

unsigned int CButtonUi::MultiplyColor(unsigned int color, float rate)
{
	// 各成分取り出し
	unsigned int a = (color >> 24) & 0xff;
	unsigned int r = (color >> 16) & 0xff;
	unsigned int g = (color >> 8) & 0xff;
	unsigned int b = (color) & 0xff;

	// 乗算
	r = (unsigned int)(r * rate);
	g = (unsigned int)(g * rate);
	b = (unsigned int)(b * rate);

	// 上限チェック
	r = (r > 255) ? 255 : r;
	g = (g > 255) ? 255 : g;
	b = (b > 255) ? 255 : b;

	// 合成
	return (a << 24) | (r << 16) | (g << 8) | b;
}

void CButtonUi::SetButtonSize()
{
	m_Scale.x = (float)m_ButtonWidth / (float)m_Width;
	m_Scale.y = (float)m_ButtonHeight / (float)m_Height;
}

void CButtonUi::SetImageSize()
{
	// 表示画像データ無しの場合は処理しない
	if (m_ButtonImageData == "")	return;

	// 表示画像の長辺を求める
	vivid::LoadTexture(m_ButtonImageData);
	int image_long_side = vivid::GetTextureWidth(m_ButtonImageData) > vivid::GetTextureHeight(m_ButtonImageData) 
		? vivid::GetTextureWidth(m_ButtonImageData) : vivid::GetTextureHeight(m_ButtonImageData);

	// ボタンの短辺を求める
	int button_short_side = m_ButtonWidth < m_ButtonHeight ? m_ButtonWidth : m_ButtonHeight;

	// 画像のスケール値に反映させる
	m_ImageScale.x = m_ImageScale.y = ((float)button_short_side / (float)image_long_side) * m_ImageScaleRate;

	// ボタンの中心座標
	vivid::Vector2 center_pos = m_Position + vivid::Vector2((float)m_ButtonWidth / 2.0f, (float)m_ButtonHeight / 2.0f);
	m_ImagePosition = center_pos - vivid::Vector2((float)vivid::GetTextureWidth(m_ButtonImageData) / 2.0f, 
		(float)vivid::GetTextureHeight(m_ButtonImageData) / 2.0f) * m_ImageScale;
}
