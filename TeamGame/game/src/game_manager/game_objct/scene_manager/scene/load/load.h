/**
*	@file	load.h
*	@brief	ロードシーン
*	@author	K.Kawakami
*/
#pragma once

#include "../scene.h"
#include "../../../sound_manager/sound_manager.h"
#include "../../../ui_manager/ui/bar_ui/bar_ui.h"

class CLoad : public IScene
{
public:
	//// 関数 ////

	/**
	*	@brief		コンストラクタ
	*/
	CLoad();

	/**
	*	@brief		デストラクタ
	*/
	~CLoad() = default;

	/**
	*	@brief		初期化
	*/
	void Initialize() override;

	/**
	*	@brief		更新
	*/
	void Update() override;

	/**
	*	@brief		描画
	*/
	void Draw() override;

	/**
	*	@brief		解放
	*/
	void Finalize() override;


private:
	//// 定数 ////

	static const int	m_load_data_count;	// ロードするデータ数


	//// 変数 ////

	int		m_LoadDataNum;		// ロードするデータ番号
	CBarUi* m_LoadProgressUi;	// プログレスバー

};
