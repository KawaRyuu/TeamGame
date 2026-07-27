#include "score_ui.h"

CScoreUi::CScoreUi()
	: CNumberUi()
	, m_OldScore(0)
	, m_ScoreDigit(0)
	, m_ExpandFlag(false)
{
}

void CScoreUi::Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, int size, int digit, contol::PLAYER_ID player_id)
{
	// CNumberUiの初期化
	CNumberUi::Initialize(pos, id, ui_draw, size, digit, player_id);
}

void CScoreUi::Update()
{
	// スコアを受け取る
	//m_Value = FIELDS.GetScore(m_PlayerID);
	m_Value = 0;

	// 桁数を求める
	if (m_Value <= 0)	m_ScoreDigit = 0;
	else				m_ScoreDigit = (int)log10(m_Value) + 1;

	// 1F前と今で変化があった場合は、数字を少し拡大させる
	if (m_OldScore != m_Value)
	{
		m_OldScore = m_Value;
		m_ExpandFlag = true;
		m_Scale = vivid::Vector2(1.2f, 1.2f);
	}
	// 演出で少し段階を踏む
	else if (m_ExpandFlag)
	{
		m_ExpandFlag = false;
		m_Scale = vivid::Vector2(1.1f, 1.1f);
	}
	// 特に変化ない場合はデフォルトサイズ
	else
	{
		m_Scale = vivid::Vector2::ONE;
	}

	// 表示できるスコアの最大値を超えたら、最大値に設定
	if (m_Value > m_MaxValue)
		m_Value = m_MaxValue;
}

void CScoreUi::Draw()
{
	int score = m_Value;

	for (int i = m_Digit - 1; i >= 0; i--)
	{
		// 演出でサイズを拡大したりする
		vivid::Vector2 scale = vivid::Vector2::ONE;
		// 桁数のみ演出（左部の000…は拡大させない）
		if (m_ScoreDigit > m_Digit - 1 - i)
			scale = m_Scale;

		// 数字の描画範囲を求める
		vivid::Rect	rect = { 0,0,0,m_Height };
		rect.left = (score % 10) * m_OneDigitWidth;
		rect.right = rect.left + m_OneDigitWidth;

		// スコアを10で割り、桁を1つずらす
		score /= 10;

		// 描画
		vivid::DrawTexture(m_DrawData, m_Position - vivid::Vector2
			((float)m_DrawOneDigitWidth / 2.0f,(float)m_Size / 2.0f) + vivid::Vector2
			((float)(i * m_DrawOneDigitWidth),0.0f),
			0xffffffff, rect, m_Anchor, scale * m_DefaultScale);
	}
}
