#include "record_score_ui.h"

CRecordScoreUi::CRecordScoreUi()
	: CNumberUi()
	, m_ScoreDigit(0)
{
}

void CRecordScoreUi::Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, int size, int digit, contol::PLAYER_ID player_id)
{
	// CNumberUiの初期化
	CNumberUi::Initialize(pos, id, ui_draw, size, digit, player_id);
	m_Anchor = vivid::Vector2::ZERO;
}

void CRecordScoreUi::Update()
{
}

void CRecordScoreUi::Draw()
{
	int score = m_Value;

	for (int i = m_Digit - 1; i >= 0; i--)
	{
		// 頭の0は省く
		if (m_ScoreDigit <= m_Digit - 1 - i)	continue;

		// 数字の描画範囲を求める
		vivid::Rect	rect = { 0,0,0,m_Height };
		rect.left = (score % 10) * m_OneDigitWidth;
		rect.right = rect.left + m_OneDigitWidth;

		// スコアを10で割り、桁を1つずらす
		score /= 10;

		// 描画
		vivid::DrawTexture(m_DrawData, m_Position + vivid::Vector2((float)(i * m_DrawOneDigitWidth), 0.0f),
			0xffffffff, rect, m_Anchor, vivid::Vector2(m_DefaultScale, m_DefaultScale));
	}
}

void CRecordScoreUi::SetScore(int score)
{
	m_Value = score;

	// 桁数を求める
	if (m_Value <= 0)	m_ScoreDigit = 0;
	else				m_ScoreDigit = (int)log10(m_Value) + 1;

	// 表示できるスコアの最大値を超えたら、最大値に設定
	if (m_Value > m_MaxValue)
		m_Value = m_MaxValue;
}
