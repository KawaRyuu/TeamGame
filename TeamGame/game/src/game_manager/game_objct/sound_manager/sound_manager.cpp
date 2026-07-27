#include "sound_manager.h"

const char* CSoundManager::m_sound_file_name[] =
{
    // BGM
    "data/sound/bgm/title.mp3",

    // SE
    "data/sound/se/se_test.mp3",

};
const int CSoundManager::m_default_sound_volume[] =
{
    // BGM
    50,

    // SE
    50,

};
const int CSoundManager::m_sounds_id_num[] =
{
    (int)SOUND_ID::TITLE_BGM,
    (int)SOUND_ID::TEST_SE,
    (int)SOUND_ID::MAX,
};

// インスタンスの取得
CSoundManager& CSoundManager::
GetInstance(void)
{
    static CSoundManager instance;

    return instance;
}

void CSoundManager::Initialize()
{
    // 種類ごとの音量の初期化
    for (int i = 0; i < (int)SOUND_TYPE::MAX; i++)
    {
        m_SoundsRate[i] = 5;
    }

    // 各サウンドの音量の初期化
    for (int i = 0; i < (int)SOUND_ID::MAX; i++)
    {
        m_VolumeRate[i] = 5;
    }
    this->SetDefaultVolume();

    // サウンドリストの初期化
    m_SoundList.clear();
}

void CSoundManager::Load(SOUND_ID id)
{
    // ロード済みのサウンド検索
    int sound = this->FindLoaded(m_sound_file_name[(int)id]);

    // サウンドが見つかった場合は処理しない
    if (sound != -1)    return;

    // サウンドの読み込み
    sound = DxLib::LoadSoundMem(m_sound_file_name[(int)id]);

    // サウンドリストに追加
    m_SoundList.insert(std::map<std::string, int>::value_type(m_sound_file_name[(int)id], sound));
}

void CSoundManager::Play(SOUND_ID id, bool loop)
{
    // ロード済みのサウンド検索
    int sound = this->FindLoaded(m_sound_file_name[(int)id]);

    // エラー時は処理しない
    if (sound == -1)    return;

    // 音量を設定する
    DxLib::ChangeVolumeSoundMem(255 * (int)((float)m_default_sound_volume[(int)id] * ((float)m_VolumeRate[(int)id] / 5.0f)) / 100, sound);

    // 再生する
    DxLib::PlaySoundMem(sound, (loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK));
}

bool CSoundManager::CheckPlaying(SOUND_ID id)
{
    // ロード済みのサウンド検索
    int sound = this->FindLoaded(m_sound_file_name[(int)id]);

    // エラー時は処理しない
    if (sound == -1)    return false;

    // 再生中の場合
    if (DxLib::CheckSoundMem(sound) == 1)   return true;

    return false;
}

void CSoundManager::ChangeVolume(SOUND_TYPE sounds, int volume)
{
    // 関係無いIDは処理しない
    if (sounds == SOUND_TYPE::MAX)    return;

    // その種類の音量を変更する
    for (int i = m_sounds_id_num[(int)sounds]; i < m_sounds_id_num[(int)sounds + 1]; i++)
    {
        this->ChangeVolume((SOUND_ID)i, volume);
    }
    // 種類ごとの音量の値も変更
    m_SoundsRate[(int)sounds] = volume;
}

void CSoundManager::ChangeVolume(SOUND_ID id, int volume)
{
    // 音量を変更する
    m_VolumeRate[(int)id] = volume;

    // ロード済みのサウンド検索
    int sound = this->FindLoaded(m_sound_file_name[(int)id]);

    // エラー時は処理しない
    if (sound == -1)    return;

    // サウンドの音量を変更する
    DxLib::ChangeVolumeSoundMem(255 * (int)((float)m_default_sound_volume[(int)id] * ((float)m_VolumeRate[(int)id] / 5.0f)) / 100, sound);
}

void CSoundManager::Stop(SOUND_ID id)
{
    // ロード済みのサウンド検索
    int sound = this->FindLoaded(m_sound_file_name[(int)id]);

    // エラー時は処理しない
    if (sound == -1)    return;

    // サウンドの停止
    DxLib::StopSoundMem(sound);
}

void CSoundManager::StopAll()
{
    // 順番に全てのサウンドを止める
    for (int i = 0; i < (int)SOUND_ID::MAX; i++)
    {
        this->Stop((SOUND_ID)i);
    }
}

void CSoundManager::SetDefaultVolume()
{
    // 全てのサウンドの音量を戻す
    for (int i = 0; i < (int)SOUND_ID::MAX; i++)
    {
        m_VolumeRate[i] = 5;
    }
}

int CSoundManager::FindLoaded(const std::string& file_name)
{
    // サウンドハンドル
    int sound = -1;

    // サウンドリストが空でない時
    if (!m_SoundList.empty())
    {
        // サウンドリストを探索
        for (auto it = m_SoundList.begin(); it != m_SoundList.end(); )
        {
            // 同じファイル名のものを検索
            if (it->first == file_name)
            {
                // 同じものがあれば読み込み済みのサウンドを登録
                sound = it->second;

                break;
            }
            ++it;
        }
    }

    // ハンドルを返す
    return sound;
}
