#include "number_ui.h"

CNumberUi::CNumberUi()
	: IUi(960, 128, "data/ui/number.png")
	, m_Value(0)
	, m_MaxValue(0)
	, m_Digit(0)
	, m_OneDigitWidth(0)
	, m_DrawOneDigitWidth(0)
	, m_Size(0)
	, m_DefaultScale(0.0f)
{
}

void CNumberUi::Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id)
{
	// IUiの初期化
	IUi::Initialize(pos, id, ui_draw, player_id);

	// 表示する最大値を求める（99…9 に設定）
	m_MaxValue = (int)std::pow(10, m_Digit) - 1;
	// 一桁当たりの横幅
	m_OneDigitWidth = m_Width / 10;
	// 中心座標は一桁の真ん中
	m_Anchor = vivid::Vector2((float)m_OneDigitWidth / 2, (float)m_Height / 2);
}

void CNumberUi::Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, int size, int digit, contol::PLAYER_ID player_id)
{
	// 変数の初期化
	m_Size = size;
	m_Digit = digit;

	// CNumberUiの初期化
	this->Initialize(pos, id, ui_draw, player_id);

	// デフォルトの拡縮率
	m_DefaultScale = (float)size / (float)m_Height;
	// 表示上の横幅
	m_DrawOneDigitWidth = (int)((float)m_OneDigitWidth * m_DefaultScale);
}

void CNumberUi::Update()
{

}

void CNumberUi::Draw()
{
	int value = m_Value;

	for (int i = m_Digit - 1; i >= 0; i--)
	{
		// 数字の描画範囲を求める
		vivid::Rect	rect = { 0,0,0,m_Height };
		rect.left = (value % 10) * m_OneDigitWidth;
		rect.right = rect.left + m_OneDigitWidth;

		// スコアを10で割り、桁を1つずらす
		value /= 10;

		// 描画
		vivid::DrawTexture(m_DrawData, m_Position + vivid::Vector2((float)(i * m_DrawOneDigitWidth), 0.0f));
	}
}
