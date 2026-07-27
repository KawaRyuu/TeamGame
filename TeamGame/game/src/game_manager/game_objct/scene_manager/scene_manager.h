/**
*	@file	scene_manager.h
*	@brief	生成するシーンのマネージャー
*	@author	K.Kawakami
*/
#pragma once

#include "vivid.h"
#include "scene/scene_id.h"
#include <list>
#include <map>
#include <functional>

class IScene;

class CSceneManager
{
public:
	// インスタンスの取得
	static CSceneManager& GetInstance();

	/**
	*	@brief		初期化
	*/
	void Initialize();

	/**
	*	@brief		更新
	*/
	void Update();

	/**
	*	@brief		描画
	*/
	void Draw();

	/**
	*	@brief		フェードの描画
	*/
	void DrawFade();

	/**
	*	@brief		解放
	*/
	void Finalize();

	/**
	*	@brief		次のシーンのセット
	*	
	*	@param[in]	次に切り替えたいシーンのID
	*/
	void SetNextScene(SCENE_ID id);

	/**
	*	@brief		シーンの切り替えがあるかのチェック（ある場合はシーンを切り替える）
	*/
	void CheckSceneChange();

	/**
	*	@brief		フェード状態の取得
	* 
	*	@return		フェード中の場合はtrue,未フェーズ状態の場合はfalseを返す
	*/
	bool GetIsFading();


private:
	//// ID ////

	/**
	*	@brief		シーンの状態
	*/
	enum class SCENE_STATE
	{
		MAIN,	// 通常
		F_IN,	// フェードイン
		F_OUT,	// フェードアウト
		CHANGE,	// シーン変更
	};


	//// 関数 ////

	CSceneManager() = default;									// コンストラクタ
	~CSceneManager() = default;									// デストラクタ
	CSceneManager(const CSceneManager&) = delete;				// コピーコンストラクタ禁止
	CSceneManager& operator=(const CSceneManager&) = delete;	// コピー代入禁止
	CSceneManager(CSceneManager&&) = delete;					// ムーブコンストラクタ禁止
	CSceneManager& operator=(CSceneManager&&) = delete;			// ムーブ代入禁止

	/**
	*	@brief		シーンの切り替え
	*/
	void ChangeScene();

	/**
	*	@brief		シーン状態の設定
	*	
	*	@param[in]	state	設定したいシーン状態のID
	*/
	void SetState(SCENE_STATE state);

	/**
	*	@brief		通常のシーン更新
	*/
	void UpdateMain();

	/**
	*	@brief		フェードインの更新
	*/
	void UpdateFadeIn();

	/**
	*	@brief		フェードアウトの更新
	*/
	void UpdateFadeOut();

	/**
	*	@brief		シーン変更の更新
	*/
	void UpdateChange();


	//// 定数 ////

	static const unsigned int	m_fade_default_color;	// フェードのデフォルトカラー
	static const int			m_fade_max_alpha;		// 透明度の最大値
	static const int			m_fade_min_alpha;		// 透明度の最小値
	static const int			m_fade_speed;			// フェード速度


	//// 変数 ////

	using SCENE_LIST = std::list<IScene*>;
	SCENE_LIST	m_SceneList;
	std::map<SCENE_ID, std::function<IScene* ()>>	m_Generator;

	SCENE_ID	m_SceneID;
	SCENE_ID	m_NextSceneID;
	IScene*		m_Scene;

	SCENE_STATE		m_SceneState;		// フェードの状態
	unsigned int	m_FadeColor;		// フェード状態の色
	bool			m_SceneChangeFlag;	// シーン変更フラグ

};

#define SCENES CSceneManager::GetInstance()
