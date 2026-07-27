/**
*	@file	chara1.h
*	@brief	キャラクターテスト１
*	@author	R.Kawakami
*/
#pragma once

#include "vivid.h"
#include "../chara.h"

class CChara1 : public IChara
{
public:
	//// 関数 //// 

	/**
	*	@brief		コンストラクタ
	*/
	CChara1();

	/**
	*	@brief		デストラクタ
	*/
	~CChara1() = default;

	/**
	*	@brief		初期化
	*
	*	@param[in]	pos			位置
	*	@param[in]	id			生成したキャラクターID
	*/
	void Initialize(const vivid::Vector2& pos, CHARA_ID id) override;

	/**
	*	@brief		更新
	*/
	void Update() override;

	/**
	*	@brief		描画
	*/
	void Draw() override;


private:
	//// 関数 ////


	//// 定数 //// 	



	//// 変数 //// 

};