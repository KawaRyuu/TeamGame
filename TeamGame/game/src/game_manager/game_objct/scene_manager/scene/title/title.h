/**
*	@file	title.h
*	@brief	タイトルシーン
*	@author	K.Kawakami
*/
#pragma once

#include "../scene.h"

class CTitle : public IScene
{
public:
	//// 関数 ////

	/**
	*	@brief		コンストラクタ
	*/
	CTitle();

	/**
	*	@brief		デストラクタ
	*/
	~CTitle() = default;

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

};
