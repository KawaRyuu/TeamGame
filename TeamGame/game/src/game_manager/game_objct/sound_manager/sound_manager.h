/**
 * @file    sound_manager.h
 * @brief   サウンドマネージャー
 * @author	Kouki Kawakami
 */

#pragma once

#include "sound_id.h"
#include <DxLib.h>
#include <map>
#include <string>

class CSoundManager
{
public:
    //// 関数 ////

    /**
    *   @brief  インスタンスの取得
    */
    static CSoundManager& GetInstance();

    /**
    *	@brief  初期化
    */
    void Initialize();

    /**
    *	@brief      サウンドのロード
    * 
    *   @param[in]  id  ロードするサウンドのID
    */
    void Load(SOUND_ID id);

    /**
    *	@brief      サウンドの再生
    * 
    *   @param[in]  id      再生するサウンドのID
    *   @param[in]  loop    ループ再生（default = false）
    */
    void Play(SOUND_ID id, bool loop = false);

    /**
    *	@brief      サウンドの再生状態の確認
    *
    *   @param[in]  id      確認するサウンドのID
    *   @return             指定サウンドが再生中の場合はtrue、そうでない場合はfalseを返す
    */
    bool CheckPlaying(SOUND_ID id);

    /**
    *	@brief      音量の変更(種類)
    * 
    *   @param[in]  sounds  音量変更するサウンドの種類
    *   @param[in]  volume  変更後の音量（0～10）
    */
    void ChangeVolume(SOUND_TYPE sounds, int volume);
    /**
    *	@brief      音量の変更(ID指定)
    * 
    *   @param[in]  id      音量変更するサウンドのID
    *   @param[in]  volume  変更後の音量（0～10）
    */
    void ChangeVolume(SOUND_ID id, int volume);

    /**
    *	@brief      サウンドの停止
    * 
    *   @param[in]  id      再生を停止するサウンドのID
    */
    void Stop(SOUND_ID id);

    /**
    *	@brief  サウンドの全停止
    */
    void StopAll();

    /**
    *	@brief  音量の初期化
    */
    void SetDefaultVolume();

    /**
    *   @brief      種類ごとの音量の取得
    * 
    *   @param[in]  sounds  音の種類
    *   @return     音量
    */
    inline int GetSoundsVolume(SOUND_TYPE sounds) const { return m_SoundsRate[(int)sounds]; }


private:
    //// 関数 ////

    CSoundManager() = default;									// コンストラクタ
    ~CSoundManager() = default;								    // デストラクタ
    CSoundManager(const CSoundManager&) = delete;				// コピーコンストラクタ禁止
    CSoundManager& operator=(const CSoundManager&) = delete;	// コピー代入禁止
    CSoundManager(CSoundManager&&) = delete;					// ムーブコンストラクタ禁止
    CSoundManager& operator=(CSoundManager&&) = delete;		    // ムーブ代入禁止

    /**
    *   @brief      ロード済みのサウンドを探す
    *   
    *   @param[in]  file_name  サウンドファイル名
    *   @return     サウンドハンドル(エラー時は-1)を返す
    */
    int FindLoaded(const std::string& file_name);


    //// 定数 ////

    static const char*  m_sound_file_name[(int)SOUND_ID::MAX];          // サウンドのファイル名
    static const int    m_default_sound_volume[(int)SOUND_ID::MAX];      // デフォルトの音量
    static const int    m_sounds_id_num[(int)SOUND_TYPE::MAX + 1];       // 各サウンドの最初のID

        
    //// 変数 ////

    int     m_VolumeRate[(int)SOUND_ID::MAX];       // 音量(10段階)
    int     m_SoundsRate[(int)SOUND_TYPE::MAX];     // 種類ごとの音量
    std::map<std::string, int>  m_SoundList;        // サウンドリスト

};

#define SOUNDS CSoundManager::GetInstance()