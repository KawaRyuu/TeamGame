/**
*	@file	button_ui.h
*	@brief	ボタンUIのベースクラス
*	@author	K.Kawakami
*/
#pragma once

#include "vivid.h"
#include "../ui.h"
#include <functional>

class CButtonUi : public IUi
{
public:
	//// 関数 //// 

	/**
	*	@brief		コンストラクタ
	*/
	CButtonUi(int width, int height, std::string data);
	CButtonUi() = delete;

	/**
	*	@brief		デストラクタ
	*/
	~CButtonUi() = default;

	/**
	*	@brief		初期化
	*
	*	@param[in]	pos			位置
	*	@param[in]	id			生成したUIのID
	*	@param[in]	ui_draw		UIを描画する位置（手前か奥か）
	*	@param[in]	playre_id	UIを生成したプレイヤーのID（誰のUIか）
	*/
	void Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id) override;
	/**
	*	@brief		初期化
	*
	*	@param[in]	pos			位置
	*	@param[in]	id			生成したUIのID
	*	@param[in]	width		横幅
	*	@param[in]	height		縦幅
	*	@param[in]	image_data	表示画像名
	*	@param[in]	image_scale	表示画像のデフォルトサイズ
	*	@param[in]	color		色
	*	@param[in]	ui_draw		UIを描画する位置（手前か奥か）
	*	@param[in]	playre_id	UIを生成したプレイヤーのID（誰のUIか）
	*	@param[in]	on_click	決定ボタンを押した時の処理
	*/
	virtual void Initialize(const vivid::Vector2& pos, UI_ID id, int width, int height,
		std::string image_data, float image_scale, unsigned int color, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id,
		std::function<void()> on_click);

	/**
	*	@brief		更新
	*/
	void Update() override;

	/**
	*	@brief		描画
	*/
	void Draw() override;

	/**
	*	@brief		隣接するボタンの設定
	* 
	*	@param[in]	left	左隣のボタンオブジェクト
	*	@param[in]	up		上隣のボタンオブジェクト
	*	@param[in]	right	右隣のボタンオブジェクト
	*	@param[in]	down	下隣のボタンオブジェクト
	*/
	void SetNeighbor(CButtonUi* left, CButtonUi* up, CButtonUi* right, CButtonUi* down);

	/**
	*	@brief		ボタンアクティブフラグの設定
	*
	*	@param[in]	flag	フラグ
	*/
	void SetButtonActive(bool flag = true);


protected:
	//// 関数 ////

	/**
	*	@brief		決定キー入力チェック
	*	@return		決定キーが押されたらtrueを返す
	*/
	bool IsClicked();

	/**
	*	@brief		ボタン選択カーソルの移動
	*/
	void MoveSelect();

	/**
	*	@brief		色の乗算(明暗)
	* 
	*	@param[in]	color	元の色
	*	@param[in]	rate	明暗の倍率
	*	@return		計算後の色を返す
	*/
	unsigned int MultiplyColor(unsigned int color, float rate);

	/**
	*	@brief		ボタンサイズの設定
	*/
	void SetButtonSize();

	/**
	*	@brief		表示画像サイズの設定
	*/
	void SetImageSize();


	//// 変数 ////

	int						m_ButtonWidth;		// ボタンの横幅
	int						m_ButtonHeight;		// ボタンの縦幅
	std::string				m_ButtonImageData;	// ボタン内の描画画像のファイル名
	vivid::Vector2			m_ImagePosition;	// 表示画像の表示位置
	vivid::Vector2			m_ImageScale;		// 表示画像のスケール
	float					m_ImageScaleRate;	// 表示画像のデフォルト拡縮率
	std::function<void()>	m_OnClick;			// ボタンクリック時の処理
	bool					m_Selecting;		// 選択中フラグ
	
    CButtonUi*	m_Left;		// 左入力時の遷移先ボタン
    CButtonUi*	m_Up;		// 上入力時の遷移先ボタン
    CButtonUi*	m_Right;	// 右入力時の遷移先ボタン
    CButtonUi*	m_Down;		// 下入力時の遷移先ボタン

};