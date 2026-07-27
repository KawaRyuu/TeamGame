/**
*	@file	scene.h
*	@brief	シーンのベースクラス
*	@author	K.Kawakami
*/
#pragma once

#include "vivid.h"
#include "../scene_manager.h"

class IScene
{
public:
	//// 関数 ////

	/**
	*	@brief		コンストラクタ
	*/
	IScene();

	/**
	*	@brief		デストラクタ
	*/
	~IScene() = default;

	/**
	*	@brief		初期化
	*/
	virtual void Initialize();

	/**
	*	@brief		更新
	*/
	virtual void Update();

	/**
	*	@brief		描画
	*/
	virtual void Draw();

	/**
	*	@brief		解放
	*/
	virtual void Finalize();

};
