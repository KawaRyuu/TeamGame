/**
*	@file	ui_manager.h
*	@brief	生成するUIのマネージャー
*	@author	K.Kawakami
*/

#pragma once

#include "contol.h"
#include "vivid.h"
#include "ui/ui_id.h"
#include "ui/button_ui/button_ui.h"
#include "ui/bar_ui/bar_ui.h"
#include "ui/number_ui/number_ui.h"
#include <list>
#include <map>
#include <functional>

class IUi;

class CUiManager
{
public:
	//// 関数 //// 

	/**
	*	@brief		インスタンスの取得
	*
	*	@return		インスタンス
	*/
	static CUiManager& GetInstance();

	/**
	*	@brief		初期化
	*/
	void Initialize();

	/**
	*	@brief		更新
	*/
	void Update();

	/**
	*	@brief		描画（手前）
	*/
	void DrawFront();

	/**
	*	@brief		描画（中間）
	*/
	void DrawMiddle();

	/**
	*	@brief		描画（奥）
	*/
	void DrawBack();

	/**
	*	@brief		解放
	*/
	void Finalize();

	/**
	*	@brief		UIの生成
	*
	*	@param[in]	id			UIID
	*	@param[in]	pos			位置
	*	@param[in]	ui_draw		UIの表示位置（default = FRONT）
	*	@param[in]	player_id	誰のUIかの識別子（default = ALL）
	*	@return		生成したUIオブジェクトを返す
	*/
	IUi* Create(UI_ID id, const vivid::Vector2& pos,
		UI_DRAW_POS ui_draw = UI_DRAW_POS::FRONT, contol::PLAYER_ID player_id = contol::PLAYER_ID::ALL);

	/**
	*	@brief		数字UIの生成
	*
	*	@param[in]	id			UIID
	*	@param[in]	pos			位置
	*	@param[in]	size		1文字当たりの縦幅
	*	@param[in]	digit		表示桁数
	*	@param[in]	ui_draw		UIの表示位置（default = FRONT）
	*	@param[in]	playre_id	誰のUIかの識別子（default = ALL）
	*	@return		生成した数字UIオブジェクトを返す
	*/
	CNumberUi* CreateNumber(UI_ID id, const vivid::Vector2& pos, int size, int digit,
		UI_DRAW_POS ui_draw = UI_DRAW_POS::FRONT, contol::PLAYER_ID player_id = contol::PLAYER_ID::ALL);

	/**
	*	@brief		ボタンUIの生成
	*
	*	@param[in]	id			UIID
	*	@param[in]	pos			位置
	*	@param[in]	width		横幅
	*	@param[in]	height		縦幅
	*	@param[in]	on_click	決定ボタンを押した時の処理
	*	@param[in]	color		色（default = 0xffffffff）
	*	@param[in]	image_data	表示画像名（default = ""）
	*	@param[in]	image_scale	表示画像のデフォルトサイズ（default = 1.0f）
	*	@param[in]	ui_draw		UIの表示位置（default = FRONT）
	*	@param[in]	player_id	誰のUIかの識別子（default = ALL）
	*	@return		生成したボタンオブジェクトを返す
	*/
	CButtonUi* CreateButton(UI_ID id, const vivid::Vector2& pos, int width, int height,
		std::function<void()> on_click, unsigned int color = 0xffffffff, std::string image_data = "",
		float image_scale = 1.0f, UI_DRAW_POS ui_draw = UI_DRAW_POS::FRONT, contol::PLAYER_ID player_id = contol::PLAYER_ID::ALL);

	/**
	*	@brief		UIの生成
	*
	*	@param[in]	id			UIID
	*	@param[in]	pos			位置
	*	@param[in]	ui_draw		UIの表示位置（default = FRONT）
	*	@param[in]	player_id	誰のUIかの識別子（default = ALL）
	*/
	CBarUi* CreateBar(UI_ID id, const vivid::Vector2& pos,
		UI_DRAW_POS ui_draw = UI_DRAW_POS::FRONT, contol::PLAYER_ID player_id = contol::PLAYER_ID::ALL);

	/**
	*	@brief		UIの削除
	*
	*	@param[in]	id		UIID
	*/
	void DeleteUi(UI_ID id);

	/**
	*	@brief		ボタンカーソル移動フラグの取得
	* 
	*	@return		ボタンカーソルが移動した時にtrueを返す
	*/
	inline bool GetButtonCursorMoved() const { return m_ButtonMoved; };

	/**
	*	@brief		ボタンカーソル移動フラグの設定
	*
	*	@param[in]	flag 移動時true（default = true）
	*/
	void SetButtonCursorMoved(bool flag = true);

	/**
	*	@brief		モードセレクトシーンのUI移動の設定
	*/
	void SetModeSceneUiMove();

	/**
	*	@brief		モードセレクトシーンのUI移動の設定
	* 
	*	@return		描画基準点を返す
	*/
	inline float GetModeSceneUiMove() const { return m_ModeUiDrawPos; };

	/**
	*	@brief		モードセレクトシーンのUI移動の中間タイミングの取得
	* 
	*	@return		演出時間の半分の時間が過ぎた時にtrueを返す
	*/
	inline bool GetModeSceneUiHalfTime() const { return (m_ModeUiDrawAngle > 270); };


private:
	//// 関数 //// 

	CUiManager() = default;								// コンストラクタ
	~CUiManager() = default;							// デストラクタ
	CUiManager(const CUiManager&) = delete;				// コピーコンストラクタ禁止
	CUiManager& operator=(const CUiManager&) = delete;	// コピー代入禁止
	CUiManager(CUiManager&&) = delete;					// ムーブコンストラクタ禁止
	CUiManager& operator=(CUiManager&&) = delete;		// ムーブ代入禁止


	//// 変数 ////

	using UI_LIST = std::list<IUi*>;

	UI_LIST	m_UiList;

	std::map<UI_ID, std::function<IUi* ()>>	m_Generator;

	bool	m_ButtonMoved;		// ボタンカーソルの移動フラグ
	float	m_ModeUiDrawPos;	// モードセレクトシーン内UIの描画基準位置
	int		m_ModeUiDrawAngle;	// モードセレクトシーン内UIの描画位置のSIN回転値

};

#define UIS CUiManager::GetInstance()