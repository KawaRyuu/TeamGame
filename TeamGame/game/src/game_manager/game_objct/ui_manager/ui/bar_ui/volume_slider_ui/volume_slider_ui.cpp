#include "volume_slider_ui.h"

const std::string	CVolumeSliderUi::m_bar_data = "data/ui/bar/volume_slider/bar.png";
const std::string	CVolumeSliderUi::m_bar_frame_data = "data/ui/bar/volume_slider/bar_frame.png";
const std::string	CVolumeSliderUi::m_bar_point_data = "data/ui/bar/volume_slider/bar_point.png";
const std::string	CVolumeSliderUi::m_bar_point_back_data = "data/ui/bar/volume_slider/bar_point_back.png";
const unsigned int	CVolumeSliderUi::m_bar_point_color[] = { 0xffffffff, 0xffffffc0 };
const int			CVolumeSliderUi::m_bar_point_size = 64;

CVolumeSliderUi::CVolumeSliderUi()
	: CBarUi(1080, 32, m_bar_data)
	, m_SoundType(SOUND_TYPE::MAX)
	, m_SliderActive(false)
	, m_BarPointColor(0xffffffff)
	, m_BarPointPos(vivid::Vector2::ZERO)
{
}

void CVolumeSliderUi::Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id)
{
	CBarUi::Initialize(pos, id, ui_draw, player_id);
	m_Rect = { 0,0,0,m_Height };
	m_BarPointPos = { m_Position.x + (int)m_Rect.right - (float)m_bar_point_size / 2.0f, 
		m_Position.y + (float)m_Height / 2.0f - (float)m_bar_point_size / 2.0f };
}

void CVolumeSliderUi::Update()
{
	CBarUi::Update();
}

void CVolumeSliderUi::Draw()
{
	// バーの描画
	vivid::DrawTexture(m_bar_data, m_Position, 0xffa0a0a0);
	vivid::DrawTexture(m_bar_data, m_Position, m_Color, m_Rect);
	vivid::DrawTexture(m_bar_frame_data, m_Position);
	vivid::DrawTexture(m_bar_point_back_data, m_BarPointPos);
	vivid::DrawTexture(m_bar_point_data, m_BarPointPos, m_BarPointColor);

	// 仮文字表示
	std::string str = "";

	if (m_SliderActive) str = "〇";
	else				str = "・";

	switch (m_SoundType)
	{
	case SOUND_TYPE::BGM:	str += "BGM";	break;
	case SOUND_TYPE::SE:	str += "SE";	break;
	}
	vivid::DrawText((int)((float)m_Height * 1.5f), str, m_Position + vivid::Vector2(0.0f, (float)(-m_Height) * 2.0f));
}

void CVolumeSliderUi::SetBarRate(float rate)
{
	// 割合の変更
	m_Rate = rate;

	// 描画範囲の設定
	m_Rect.left = 0;
	m_Rect.right = (int)((float)m_Width * m_Rate);

	// バー点の位置の設定
	m_BarPointPos = { m_Position.x + (int)m_Rect.right - (float)m_bar_point_size / 2.0f,
		m_Position.y + (float)m_Height / 2.0f - (float)m_bar_point_size / 2.0f };
}

void CVolumeSliderUi::SetSoundType(SOUND_TYPE sounds)
{
	m_SoundType = sounds;
}

void CVolumeSliderUi::SetSliderColor(unsigned int color)
{
	m_Color = color;
}

void CVolumeSliderUi::SetSliderActive(bool flag)
{
	m_SliderActive = flag;

	// バー点の色の設定
	m_BarPointColor = m_bar_point_color[(int)m_SliderActive];
}
