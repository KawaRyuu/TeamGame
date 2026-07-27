/**
 * @file	sound_id.h
 * @brief	サウンドID
 * @author	Kouki Kawakami
 * @date
 */
#pragma once

 /**
 *	@brief	サウンドID
 */
enum class SOUND_ID
{
	// BGM
	TITLE_BGM,		// タイトルBGM

	//SE
	TEST_SE,		// SEのテスト

	MAX,
};

/**
*	@brief	サウンドの種類
*/
enum class SOUND_TYPE
{
	BGM,		// BGM
	SE,			// 効果音

	MAX,
};