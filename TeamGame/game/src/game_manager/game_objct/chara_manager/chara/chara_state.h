/**
*	@file	chara_state.h
*	@brief	キャラクターの状態
*	@author	Ryuuki.Kawakami
*/
#pragma once

enum class CHARA_STATE
{
	WAIT,			//待機
	MOVE,			//移動
	GUARD,			//防御
	SKILL,			//特性
	USE_ITEM,		//アイテム使用
	NOMAL_ATTACK,	//通常攻撃
	SURE_KILL,		//必殺技
	RESURRECTION	//復活
};