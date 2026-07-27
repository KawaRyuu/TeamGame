
/*!
 *  @file       vivid.cpp
 *  @brief      Vivid2D Library
 *  @author     Kazuya Maruyama
 *              Shinya Kosugi
 *              Toshihito Ohashi
 *  @date       2020/11/25
 *  @version    2.8
 *
 *  Copyright (c) 2013-2023, Kazuya Maruyama. Shinya Kosugi. Toshihito Ohashi. All rights reserved.
 */

#pragma comment( lib, "imm32.lib" )
#pragma comment( lib, "Shlwapi.lib" )

#include "vivid.h"
#include <imm.h>
#include <shlwapi.h>
#include <map>
#include <list>
#include <iomanip>
#include <sstream>

#pragma warning(disable: 4100)      // effekseer.hの警告回避 s.kosugi
#include <EffekseerForDXLib.h>      // effekseerライブラリの読み込み s.kosugi

 /*!
  *  @brief      vivid名前空間
  */
namespace vivid
{
    /*!
     *  @brief      vivid::core名前空間
     */
    namespace core
    {
        static const int            g_window_width                  = WINDOW_WIDTH;                         //!< ウィンドウの横幅
        static const int            g_window_height                 = WINDOW_HEIGHT;                        //!< ウィンドウの縦幅
        static const char*          g_class_name                    = "vivid2D 2.8 with Effekseer";         //!< クラスネーム
        static const int            g_color_bit                     = 32;                                   //!< カラービット
        static const unsigned int   g_button_state_num              = 9;                                    //!< 入力判定用ボタン数
        static const unsigned int   g_max_key_count                 = 256;                                  //!< ボタンの最大数
        static const float          g_one_millisecond               = 1000.0f;                              //!< 1ミリ秒
        static const float          g_max_delta_time                = 0.16f;                                //!< 最大デルタタイム
        static const float          g_default_delta_time_scale      = 1.0f;                                 //!< 初期のデルタタイムスケール
        static const int            g_default_frame_rate            = 60;                                   //!< デフォルトのフレームレート
        static const int            g_calc_frame_count              = 60;                                   //!< 平均を計算するフレームの数
        static const int            g_limit_cant_analog             = 30000;                                //!< アナログスティック入力の限界値
        static const int            g_max_trigger                   = 255;                                  //!< トリガー入力の最大値
        static const int            g_analog_dead_zone              = 15000;                                //!< アナログスティックの向こう範囲
        HINSTANCE                   g_InstanceHandle                = NULL;                                 //!< インスタンスハンドル
        DISPLAY_FUNC                g_DisplayFunction               = nullptr;                              //!< 更新用関数
        int                         g_FrameRate                     = g_default_frame_rate;                 //!< フレームレート
        float                       g_Fps                           = 0.0f;                                 //!< 現在のFPS
        int                         g_FrameCount                    = 0;                                    //!< フレームカウント
        int                         g_StartTime                     = 0;                                    //!< フレーム計測開始時間
        int                         g_PrevTime                      = 0;                                    //!< 1フレーム前の時間
        float                       g_DeltaTime                     = 0.0f;                                 //!< ゲーム更新用デルタタイム
        float                       g_DeltaTimeScale                = g_default_delta_time_scale;           //!< デルタタイム倍率
        std::map<std::string, int>  g_TextureList;                                                          //!< テクスチャリスト
        std::map<std::string, int>  g_SoundList;                                                            //!< サウンドリスト
        std::map<int, int>          g_FontList;                                                             //!< フォントリスト
        char                        g_KeyState[g_max_key_count]     = { 0 };                                //!< 入力データ
        char                        g_PrevKeyState[g_max_key_count] = { 0 };                                //!< 前回の入力データ
        int                         g_MouseState                    = 0;                                    //!< マウスの入力状態
        int                         g_PrevMouseState                = 0;                                    //!< 前のマウスの入力状態
        std::map<std::string, int>  g_EffekseerList;                                                        //!< エフェクト読み込みリスト s.kosugi
        std::list<PLAYEFFECT_DATA>  g_EffectPlayList;                                                       //!< エフェクト全再生リスト s.kosugi
        bool                        g_UseEffectListFlag             = true;                                 //!< エフェクトリストの全再生を行うかどうか s.kosugi
        XINPUT_STATE                g_ControllerState[static_cast<int>(controller::DEVICE_ID::MAX)];        //!< コントローラーの入力状態
        XINPUT_STATE                g_PrevControllerState[static_cast<int>(controller::DEVICE_ID::MAX)];    //!< 1フレーム前の入力状態

#ifdef VIVID_DEBUG
        bool                        g_DebugShowFps                  = true;                                 //!< デバッグ用FPS表示フラグ
#endif
    }
};

const vivid::Vector2 vivid::Vector2::ZERO = Vector2(0.0f, 0.0f);
const vivid::Vector2 vivid::Vector2::ONE  = Vector2(1.0f, 1.0f);

/*
 *  コンストラクタ
 */
vivid::Vector2::
Vector2(void)
    : x(0.0f), y(0.0f)
{
}

/*
 *  コンストラクタ
 */
vivid::Vector2::
Vector2(float x, float y)
    : x(x), y(y)
{
}

/*
 *  コンストラクタ
 */
vivid::Vector2::
Vector2(const Vector2& v)
    : x(v.x), y(v.y)
{
}

/*
 *  デストラクタ
 */
vivid::Vector2::
~Vector2(void)
{
}

/*
 *  正規化
 */
vivid::Vector2
vivid::Vector2::
Normalize(void)
{
    return Normalize(*this);
}

/*
 *  正規化
 */
vivid::Vector2
vivid::Vector2::
Normalize(const Vector2& v)
{
    Vector2 t = v;

    float inv = 1.0f / t.Length();

    t.x *= inv;
    t.y *= inv;

    return t;
}

/*
 *  ベクトルの大きさ取得
 */
float
vivid::Vector2::
Length(void)
{
    return Length(*this);
}

/*
 *  ベクトルの大きさ取得
 */
float
vivid::Vector2::
Length(const Vector2& v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

/*
 *  内積
 */
float
vivid::Vector2::
Dot(const Vector2& vA, const Vector2& vB)
{
    return (vA.x * vB.x + vA.y * vB.y);
}

/*
 *  外積
 */
float
vivid::Vector2::
Cross(const Vector2& vA, const Vector2& vB)
{
    return (vA.x * vB.y) - (vA.y * vB.x);
}

/*
 *  ベクトルの回転
 */
vivid::Vector2
vivid::Vector2::
Rotate(const Vector2& v, float angle)
{
    Vector2 result;
    const float _cos = cos(angle);
    const float _sin = sin(angle);

    result.x = v.x * _cos + v.y * -_sin;
    result.y = v.x * _sin + v.y *  _cos;

    return result;
}

/*
 *  代入演算子のオーバーロード
 */
vivid::Vector2&
vivid::Vector2::
operator=(const Vector2& v)
{
    x = v.x;
    y = v.y;

    return *this;
}

/*
 *  加算演算子のオーバーロード
 */
vivid::Vector2&
vivid::Vector2::
operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;

    return *this;
}

/*
 *  減算演算子のオーバーロード
 */
vivid::Vector2&
vivid::Vector2::
operator-=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;

    return *this;
}

/*
 *  乗算演算子のオーバーロード
 */
vivid::Vector2&
vivid::Vector2::
operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;

    return *this;
}

/*
 *  等価演算子のオーバーロード
 */
bool
vivid::Vector2::
operator==(const Vector2& v) const
{
    return (x == v.x && y == v.y);
}

/*
 *  不等演算子のオーバーロード
 */
bool
vivid::Vector2::
operator!=(const Vector2& v) const
{
    return (x != v.x || y != v.y);
}

/*
 *  正符号演算子のオーバーロード
 */
vivid::Vector2
vivid::operator+(const vivid::Vector2& v)
{
    return v;
}

/*
 *  負符号演算子のオーバーロード
 */
vivid::Vector2
vivid::operator-(const vivid::Vector2& v)
{
    return vivid::Vector2(-v.x, -v.y);
}

/*
 *  加算演算子のオーバーロード
 */
vivid::Vector2
vivid::operator+(const vivid::Vector2& vA, const vivid::Vector2& vB)
{
    return vivid::Vector2(vA.x + vB.x, vA.y + vB.y);
}

/*
 *  減算演算子のオーバーロード
 */
vivid::Vector2
vivid::operator-(const vivid::Vector2& vA, const vivid::Vector2& vB)
{
    return vivid::Vector2(vA.x - vB.x, vA.y - vB.y);
}

/*
 *  乗算演算子のオーバーロード
 */
vivid::Vector2
vivid::operator*(const vivid::Vector2& vA, const vivid::Vector2& vB)
{
    return vivid::Vector2(vA.x * vB.x, vA.y * vB.y);
}

/*
 *  乗算演算子のオーバーロード
 */
vivid::Vector2
vivid::operator*(const vivid::Vector2& v, float scalar)
{
    return vivid::Vector2(v.x * scalar, v.y * scalar);
}

/*
 *  乗算演算子のオーバーロード
 */
vivid::Vector2
vivid::operator*(float scalar, const vivid::Vector2& v)
{
    return vivid::Vector2(v.x * scalar, v.y * scalar);
}

/*
 *  コンストラクタ
 */
vivid::PLAYEFFECT_DATA::
PLAYEFFECT_DATA(void)
    : handle(0)
{
}

using namespace vivid::core;

/*
 * 初期化
 */
void
vivid::
Initialize(HINSTANCE hInst)
{
    // インスタンスハンドル
    g_InstanceHandle = hInst;

    // IMEを禁止する
    ImmDisableIME(0xffffffff);

    // 乱数の初期化
    srand((unsigned int)time(NULL));

    // Log.txtを生成しない
    SetOutApplicationLogValidFlag(FALSE);

    // ウィンドウが非アクティブ時も更新する
    SetAlwaysRunFlag(TRUE);

    // ウィンドウテキストの変更
    SetMainWindowText(g_class_name);

    // バックバッファサイズ指定
    SetGraphMode(g_window_width, g_window_height, g_color_bit);

    // ウィンドウサイズ指定
    SetWindowSize(g_window_width, g_window_height);

#ifdef VIVID_DEBUG
    // ウィンドウモードで起動
    ChangeWindowMode(TRUE);
#else
    // フルスクリーンモードで起動
    ChangeWindowMode(FALSE);

    // ゲーム画面の解像度に合わせてフルスクリーンにする
    SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
#endif

    // DirectX11のバージョンを指定する
    if (SetUseDirect3DVersion(DX_DIRECT3D_11) == VIVID_DX_ERROR)
        return;

    // DXライブラリ初期化
    if (DxLib_Init() == VIVID_DX_ERROR)
        return;

    // バックバッファに書き込む
    SetDrawScreen(DX_SCREEN_BACK);

    // デフォルトフォントをエッジ付きに設定する
    ChangeFontType(DX_FONTTYPE_EDGE);

    // 透過色指定(マゼンタ)
    SetTransColor(0xff, 0x00, 0xff);

    // テクスチャリストをクリア
    g_TextureList.clear();

    // サウンドリストをクリア
    g_SoundList.clear();

    // フォントリストをクリア
    g_FontList.clear();

    // コントローラーのステートをクリア
    ZeroMemory(g_ControllerState, sizeof(XINPUT_STATE) * static_cast<int>(controller::DEVICE_ID::MAX));
    ZeroMemory(g_PrevControllerState, sizeof(XINPUT_STATE) * static_cast<int>(controller::DEVICE_ID::MAX));

    // バイブレーションを使用する
    SetUseJoypadVibrationFlag(TRUE);

    // エフェクシアの初期化 s.kosugi
    effekseer::InitEffekseer();

    // エフェクトリストをクリア s.kosugi
    g_EffekseerList.clear();
    g_EffectPlayList.clear();
}

/*
 *  描画関数登録
 */
void
vivid::
DisplayFunction(DISPLAY_FUNC display)
{
    // 関数ポインタを保存
    g_DisplayFunction = display;
}

/*
 *  メインループ
 */
void
vivid::
MainLoop(void)
{
    // Windowsからの命令を処理
    while (ProcessMessage() != VIVID_DX_ERROR)
    {
        // フレームカウント算出
        CalcFrameCount();

        // 入力処理更新
        // 前回のデータコピー
        memcpy(g_PrevKeyState, g_KeyState, g_max_key_count);

        // 現在の入力情報取得
        GetHitKeyStateAll(g_KeyState);

        // 前のフレームの入力状態を保存
        g_PrevMouseState = g_MouseState;

        // マウスの入力情報取得
        g_MouseState = GetMouseInput();

        // 接続されている台数分だけ、コントローラーの入力情報取得
        /*for (int i = 0; i < controller::GetConnectCount(); ++i)
        {
            // 1フレーム前の状態を保存
            g_PrevControllerState[i] = g_ControllerState[i];

            // 現在の入力状態を取得
            GetJoypadXInputState(i + DX_INPUT_PAD1, &g_ControllerState[i]);
        }*/

        // 画面の背景色を設定
        SetBackgroundColor(0x80, 0x80, 0x80);

        // 画面のクリア
        ClearDrawScreen();

        // 更新/描画
        g_DisplayFunction();

        // effekseerライブラリの更新  s.kosugi
        UpdateEffekseer2D();
        // エフェクトの描画  s.kosugi
        if (g_UseEffectListFlag)
            effekseer::DrawEffectList();

        // スクリーンショットの撮影
        if (keyboard::Trigger(vivid::keyboard::KEY_ID::F9))
            ScreenShot();

#ifdef VIVID_DEBUG
        // FPSの表示/非表示切り替え
        if (keyboard::Trigger(vivid::keyboard::KEY_ID::F6))
            g_DebugShowFps = !g_DebugShowFps;

        if (g_DebugShowFps)
        {
            std::stringstream fps;

            fps << "[FPS]" << std::fixed << std::setprecision(2) << GetFPS();

            int x = g_window_width - GetDrawStringWidth(fps.str().c_str(), (int)fps.str().length());

            DrawString(x, g_window_height - GetFontSize(), fps.str().c_str(), 0xff00ff00, 0xff004000);
        }
#endif
        // バックバッファの転送
        ScreenFlip();

        // フレーム同期
        FrameSync();

        // Escキーで終了
        if (keyboard::Trigger(vivid::keyboard::KEY_ID::ESCAPE))
            break;
    }
}

/*
 *  解放
 */
void
vivid::
Finalize(void)
{
    // すべてのグラフィックスを解放
    InitGraph();

    // すべてのフォントを解放する
    InitFontToHandle();

    // すべてのサウンドを解放
    InitSoundMem();

    // エフェクシアライブラリの終了 s.kosugi
    Effkseer_End();

    // DXライブラリ解放
    DxLib_End();
}

/*
 *  ウィンドウの横幅取得
 */
int
vivid::
GetWindowWidth(void)
{
    return g_window_width;
}

/*
 *  ウィンドウの縦幅取得
 */
int
vivid::
GetWindowHeight(void)
{
    return g_window_height;
}

/*
 *  デルタタイム取得
 */
float
vivid::
GetDeltaTime(void)
{
    return g_DeltaTime * g_DeltaTimeScale;
}

/*
 *  デルタタイム(倍率なし)取得
 */
float
vivid::
GetUnscaledDeltaTime(void)
{
    return g_DeltaTime;
}

/*
 *  デルタタイム倍率取得
 */
float
vivid::
GetDeltaTimeScale(void)
{
    return g_DeltaTimeScale;
}

/*
 *  デルタタイム倍率設定
 */
void
vivid::
SetDeltaTimeScale(float scale)
{
    g_DeltaTimeScale = scale;
}

/*
 *  現在のFPS取得
 */
float
vivid::
GetFPS(void)
{
    return g_Fps;
}

/*
 *  フレームレート設定
 */
void
vivid::
SetFrameRate(int frame_rate)
{
    g_FrameRate = frame_rate;
}

/*
 *  テクスチャ読み込み
 */
void
vivid::
LoadTexture(const std::string& file_name)
{
    // ロード済みのテクスチャ検索
    int texture = FindLoadedTexture(file_name);

    // テクスチャが見つかった
    if (texture != VIVID_DX_ERROR)
        return;

    // テクスチャの読み込み
    texture = LoadGraph(file_name.c_str());

    VIVID_DX_ASSERT(texture, file_name + "の読み込みに失敗しました。");

    // テクスチャを登録
    g_TextureList.insert(std::map<std::string, int>::value_type(file_name, texture));
}

/*
 *  テクスチャ描画
 */
void
vivid::
DrawTexture(const std::string& file_name, const Vector2& position)
{
    DrawTexture(file_name, position, 0xffffffff);
}

/*
 *  テクスチャ描画
 */
void
vivid::
DrawTexture(const std::string& file_name, const Vector2& position, unsigned int color)
{
    // テクスチャを検索
    int texture = FindLoadedTexture(file_name);

    // テクスチャがないければロードする
    if (texture == VIVID_DX_ERROR)
    {
        LoadTexture(file_name);

        // ロード済みのテクスチャを再度検索
        texture = FindLoadedTexture(file_name);
    }

    int width = 0, height = 0;

    // テクスチャのサイズ取得
    GetGraphSize(texture, &width, &height);

    vivid::Rect rect = { 0, 0, width, height };

    DrawTexture(file_name, position, color, rect);
}

/*
 *  テクスチャ描画
 */
void
vivid::
DrawTexture(const std::string& file_name, const Vector2& position, unsigned int color, const Rect& rect)
{
    vivid::Vector2 anchor = { 0.0f, 0.0f };
    vivid::Vector2 scale = { 1.0f, 1.0f };

    DrawTexture(file_name, position, color, rect, anchor, scale);
}

/*
 *  テクスチャ描画
 */
void
vivid::
DrawTexture(const std::string& file_name, const Vector2& position, unsigned int color, const Rect& rect, const Vector2& anchor, const Vector2& scale)
{
    DrawTexture(file_name, position, color, rect, anchor, scale, 0.0f);
}

/*
 *  テクスチャ描画
 */
void
vivid::
DrawTexture(const std::string& file_name, const Vector2& position, unsigned int color, const Rect& rect, const Vector2& anchor, float rotation)
{
    vivid::Vector2 scale = { 1.0f, 1.0f };

    DrawTexture(file_name, position, color, rect, anchor, scale, rotation);
}

/*
 *  テクスチャ描画
 */
void
vivid::
DrawTexture(const std::string& file_name, const Vector2& position, unsigned int color, const Rect& rect, const Vector2& anchor, const Vector2& scale, float rotation)
{
    DrawTexture(file_name, position, color, rect, anchor, scale, rotation, vivid::ALPHABLEND::ALPHA);
}

/*
 *  テクスチャ描画
 */
void
vivid::
DrawTexture(const std::string& file_name, const Vector2& position, unsigned int color, const Rect& rect, const Vector2& anchor, const Vector2& scale, float rotation, ALPHABLEND blend_mode)
{
    // ロード済みのテクスチャを検索
    int texture = FindLoadedTexture(file_name);

    // テクスチャがなければロードする
    if (texture == VIVID_DX_ERROR)
    {
        LoadTexture(file_name);

        texture = FindLoadedTexture(file_name);
    }

    // 滑らかに描画
    SetDrawMode(DX_DRAWMODE_BILINEAR);

    // アルファブレンド設定
    SetDrawBlendMode(static_cast<int>(blend_mode), ((color & 0xff000000) >> 24));

    // 輝度設定
    SetDrawBright(((color & 0x00ff0000) >> 16), ((color & 0x0000ff00) >> 8), (color & 0x000000ff));

    // 描画
    DrawRectRotaGraph3F(position.x + anchor.x, position.y + anchor.y, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, anchor.x, anchor.y, scale.x, scale.y, rotation, texture, TRUE);

    // 輝度をデフォルトに戻す
    SetDrawBright(0xff, 0xff, 0xff);

    // アルファブレンドモードをデフォルトに戻す
    SetDrawBlendMode(static_cast<int>(vivid::ALPHABLEND::ALPHA), 0xff);
}

/*
 *  テクスチャの幅を取得
 */
int
vivid::
GetTextureWidth(const std::string& file_name)
{
    // テクスチャを検索
    int texture = FindLoadedTexture(file_name);

    // テクスチャが見つからなかった
    if (texture == VIVID_DX_ERROR)
        return 0;

    int width = 0, height = 0;

    // グラフィックサイズ取得
    GetGraphSize(texture, &width, &height);

    return width;
}

/*
 *  テクスチャの高さを取得
 */
int
vivid::
GetTextureHeight(const std::string& file_name)
{
    // テクスチャを検索
    int texture = FindLoadedTexture(file_name);

    // テクスチャが見つからなかった
    if (texture == VIVID_DX_ERROR)
        return 0;

    int width = 0, height = 0;

    // グラフィックサイズ取得
    GetGraphSize(texture, &width, &height);

    return height;
}

/*
 *  フォントの生成
 */
void
vivid::
CreateFont(int size)
{
    CreateFont(size, 0);
}

/*
 *  フォントの生成
 */
void
vivid::
CreateFont(int size, int edge_size)
{
    // フォントを検索
    int font = FindCreatedFont(size);

    // フォントが見つかった
    if (font != VIVID_DX_ERROR)
        return;

    // フォント生成
    font = CreateFontToHandle("メイリオ", size, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, edge_size);

    VIVID_DX_ASSERT(font, "フォントの生成に失敗しました。");

    // フォントを登録
    g_FontList.insert(std::map<int, int>::value_type(size, font));
}

/*
 *  テキスト描画
 */
void
vivid::
DrawText(int size, const std::string& text, const Vector2& position)
{
    DrawText(size, text, position, 0xffffffff);
}

/*
 *  テキスト描画
 */
void
vivid::
DrawText(int size, const std::string& text, const Vector2& position, unsigned int color)
{
    DrawText(size, text, position, color, 0xff000000);
}

/*
 *  テキスト描画
 */
void
vivid::
DrawText(int size, const std::string& text, const Vector2& position, unsigned int color, unsigned int edge_color)
{
    DrawText(size, text, position, color, edge_color, vivid::ALPHABLEND::ALPHA);
}

/*
 *  テキスト描画
 */
void
vivid::
DrawText(int size, const std::string& text, const Vector2& position, unsigned int color, ALPHABLEND blend_mode)
{
    DrawText(size, text, position, color, 0xff000000, blend_mode);
}

/*
 *  テキスト描画
 */
void
vivid::
DrawText(int size, const std::string& text, const Vector2& position, unsigned int color, unsigned int edge_color, ALPHABLEND blend_mode)
{
    // フォントを検索
    int font = FindCreatedFont(size);

    // フォントが見つからなかった
    if (font == VIVID_DX_ERROR)
    {
        CreateFont(size);

        font = FindCreatedFont(size);
    }

    // 滑らかに描画
    SetDrawMode(DX_DRAWMODE_BILINEAR);

    // アルファブレンド設定
    SetDrawBlendMode(static_cast<int>(blend_mode), (color & 0xff000000) >> 24);

    // 文字列描画
    DrawStringToHandle((int)position.x, (int)position.y, text.c_str(), color, font, edge_color);

    // アルファブレンドモードをデフォルトに戻す
    SetDrawBlendMode(static_cast<int>(ALPHABLEND::ALPHA), 0xff);
}

/*
 *  文字列の幅を取得
 */
int
vivid::
GetTextWidth(int size, const std::string& text)
{
    // フォントを検索
    int font = FindCreatedFont(size);

    // フォントが見つからなかった
    if (font == VIVID_DX_ERROR)
        return 0;

    return GetDrawStringWidthToHandle(text.c_str(), (int)text.length(), font);
}

/*
 *  ラインの描画
 */
void
vivid::
DrawLine(const Vector2& start_pos, const Vector2& end_pos, unsigned int color, float thickness, ALPHABLEND blend_mode)
{
    // アルファブレンド設定
    SetDrawBlendMode(static_cast<int>(blend_mode), (color & 0xff000000) >> 24);

    DxLib::DrawLineAA(start_pos.x, start_pos.y, end_pos.x, end_pos.y, color, thickness);

    // アルファブレンドモードをデフォルトに戻す
    SetDrawBlendMode(static_cast<int>(ALPHABLEND::ALPHA), 0xff);
}

/*
 *  ボタン判定
 */
bool
vivid::keyboard::
Button(vivid::keyboard::KEY_ID key)
{
    return g_KeyState[static_cast<int>(key)];
}

/*
 *  トリガー判定
 */
bool
vivid::keyboard::
Trigger(vivid::keyboard::KEY_ID key)
{
    return (g_KeyState[static_cast<int>(key)] && !g_PrevKeyState[static_cast<int>(key)]);
}

/*
 *  リリース判定
 */
bool
vivid::keyboard::
Released(vivid::keyboard::KEY_ID key)
{
    return (!g_KeyState[static_cast<int>(key)] && g_PrevKeyState[static_cast<int>(key)]);
}

/*
 *  ボタン判定
 */
bool
vivid::mouse::
Button(vivid::mouse::BUTTON_ID button)
{
    return (g_MouseState & static_cast<int>(button));
}

/*
 *  トリガー判定
 */
bool
vivid::mouse::
Trigger(vivid::mouse::BUTTON_ID button)
{
    return (g_MouseState & static_cast<int>(button)) && !(g_PrevMouseState & static_cast<int>(button));
}

/*
 *  リリース判定
 */
bool
vivid::mouse::
Released(vivid::mouse::BUTTON_ID button)
{
    return !(g_MouseState & static_cast<int>(button)) && (g_PrevMouseState & static_cast<int>(button));
}

/*
 *  マウスカーソルの位置取得
 */
vivid::Point
vivid::mouse::
GetCursorPos(void)
{
    vivid::Point t;

    // カーソル位置取得
    GetMousePoint(&t.x, &t.y);

    return t;
}

/*
 *  マウスホイール値取得
 */
int
vivid::mouse::
GetWheel(void)
{
    return GetMouseWheelRotVol();
}

/*
 *  ボタン判定
 */
bool
vivid::controller::
Button(vivid::controller::DEVICE_ID device, vivid::controller::BUTTON_ID button)
{
    return (g_ControllerState[static_cast<int>(device)].Buttons[static_cast<int>(button)]);
}

/*
 *  トリガー判定
 */
bool
vivid::controller::
Trigger(vivid::controller::DEVICE_ID device, vivid::controller::BUTTON_ID button)
{
    return (g_ControllerState[static_cast<int>(device)].Buttons[static_cast<int>(button)]) && !(g_PrevControllerState[static_cast<int>(device)].Buttons[static_cast<int>(button)]);
}

/*
 *  リリース判定
 */
bool
vivid::controller::
Released(vivid::controller::DEVICE_ID device, vivid::controller::BUTTON_ID button)
{
    return !(g_ControllerState[static_cast<int>(device)].Buttons[static_cast<int>(button)]) && (g_PrevControllerState[static_cast<int>(device)].Buttons[static_cast<int>(button)]);
}

/*
 *  左アナログスティック取得
 */
vivid::Vector2
vivid::controller::
GetAnalogStickLeft(vivid::controller::DEVICE_ID device)
{
    int x = g_ControllerState[static_cast<int>(device)].ThumbLX;
    int y = g_ControllerState[static_cast<int>(device)].ThumbLY;

    float ax = (abs(x) < g_analog_dead_zone ? 0.0f : x );
    float ay = (abs(y) < g_analog_dead_zone ? 0.0f : y );

    if (x < 0.0f)
        ax = max((float)x, -g_limit_cant_analog);
    else if (x > 0.0f)
        ax = min((float)x, g_limit_cant_analog);

    if (y < 0.0f)
        ay = max((float)y, -g_limit_cant_analog);
    else if (y > 0.0f)
        ay = min((float)y, g_limit_cant_analog);

    // Y軸だけを反転
    return vivid::Vector2(ax, -ay) * ( 1.0f / (float)g_limit_cant_analog );
}

/*
 *  右アナログスティック取得
 */
vivid::Vector2
vivid::controller::
GetAnalogStickRight(vivid::controller::DEVICE_ID device)
{
    int x = g_ControllerState[static_cast<int>(device)].ThumbRX;
    int y = g_ControllerState[static_cast<int>(device)].ThumbRY;

    float ax = (abs(x) < g_analog_dead_zone ? 0.0f : x);
    float ay = (abs(y) < g_analog_dead_zone ? 0.0f : y);

    if (x < 0.0f)
        ax = max((float)x, -g_limit_cant_analog);
    else if (x > 0.0f)
        ax = min((float)x, g_limit_cant_analog);

    if (y < 0.0f)
        ay = max((float)y, -g_limit_cant_analog);
    else if (y > 0.0f)
        ay = min((float)y, g_limit_cant_analog);

    // Y軸だけを反転
    return vivid::Vector2(ax, -ay) * (1.0f / (float)g_limit_cant_analog);
}

/*
 *  左トリガー取得
 */
float
vivid::controller::
GetTriggerLeft(vivid::controller::DEVICE_ID device)
{
    return (float)g_ControllerState[static_cast<int>(device)].LeftTrigger / (float)g_max_trigger;
}

/*
 *  右トリガー取得
 */
float
vivid::controller::
GetTriggerRight(vivid::controller::DEVICE_ID device)
{
    return (float)g_ControllerState[static_cast<int>(device)].RightTrigger / (float)g_max_trigger;
}

/*
 *  バイブレーションの開始
 */
void
vivid::controller::
StartVibration(vivid::controller::DEVICE_ID device, int power, float time)
{
    StartJoypadVibration(static_cast<int>(device) + 1, power, (int)(time * 1000.0f));
}

/*
 *  接続数取得
 */
int
vivid::controller::
GetConnectCount(void)
{
    return GetJoypadNum();
}

/*
 *  サウンドファイルの読み込み
 */
void
vivid::
LoadSound(const std::string& file_name)
{
    // ロード済みのサウンド検索
    int sound = FindLoadedSound(file_name);

    // サウンドが見つかった
    if (sound != VIVID_DX_ERROR)
        return;

    // サウンドの読み込み
    sound = LoadSoundMem(file_name.c_str());

    VIVID_DX_ASSERT(sound, file_name + "の読み込みに失敗しました。");

    // サウンドを登録
    g_SoundList.insert(std::map<std::string, int>::value_type(file_name, sound));
}

/*
 *  サウンド再生
 */
void
vivid::
PlaySound(const std::string& file_name, bool loop)
{
    // ロード済みのサウンド検索
    int sound = FindLoadedSound(file_name);

    if (sound == VIVID_DX_ERROR)
        return;

    PlaySoundMem(sound, (loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK));
}

/*
 *  サウンド停止
 */
void
vivid::
StopSound(const std::string& file_name)
{
    // ロード済みのサウンド検索
    int sound = FindLoadedSound(file_name);

    if (sound == VIVID_DX_ERROR)
        return;

    StopSoundMem(sound);
}

/*
 *  更新するフレームカウントの計算
 */
void
vivid::core::
CalcFrameCount(void)
{
    // 現在の時間を取得
    int now_time = GetNowCount();

    // 計算開始時
    if (g_FrameCount == 0)
    {
        g_StartTime = now_time;
    }
    // 指定されたフレームカウント時
    else if (g_FrameCount == g_calc_frame_count)
    {
        // フレームの平均を算出してFPSを求める
        g_Fps = g_one_millisecond / ((now_time - g_StartTime) / (float)g_calc_frame_count);

        g_FrameCount = 0;

        g_StartTime = now_time;
    }

    // フレームカウントを更新
    ++g_FrameCount;

    // 前の処理との時間の差
    int sub_time = now_time - g_PrevTime;

    // デルタタイムを算出
    g_DeltaTime = (float)sub_time / g_one_millisecond;

    // 最大デルタタイムで制限する
    if (g_DeltaTime > g_max_delta_time)
        g_DeltaTime = g_max_delta_time;

    // 今の時間を保存
    g_PrevTime = now_time;
}

/*
 *  更新フレームの待機処理
 */
void
vivid::core::
FrameSync(void)
{
    // 今とフレーム計測開始時の差を算出
    int tookTime = GetNowCount() - g_StartTime;

    // 待機時間を算出
    int waitTime = g_FrameCount * (int)g_one_millisecond / g_FrameRate - tookTime;

    // 待機時間があればその分待って同期する
    if (waitTime > 0)
        WaitTimer(waitTime);
}

/*
 *  スクリーンショットの撮影
 */
void
vivid::core::
ScreenShot(void)
{
    DATEDATA dd;

    // 現在の時間取得
    GetDateTime(&dd);

    // フォルダの有無チェック
    // なければ作成
    if (!PathIsDirectory("capture"))
        CreateDirectory("capture", NULL);

    // 日付をファイル名にする
    std::string file_name = "capture\\";

    file_name += std::to_string(dd.Year) + std::to_string(dd.Mon) + std::to_string(dd.Day) + "_";

    file_name += std::to_string(dd.Hour) + std::to_string(dd.Min) + std::to_string(dd.Sec) + ".bmp";

    // スクリーンショットの書き出し
    SaveDrawScreenToBMP(0, 0, g_window_width, g_window_height, file_name.c_str());
}

/*
 *  アサート処理
 */
void
vivid::core::
Assert(bool result, const std::string& message, const std::string& file, int line)
{
    if (result) return;

    // アルファブレンド設定
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0xee);

    // 背景描画
    DrawBox(0, 0, g_window_width, g_window_height, 0xff000000, TRUE);

    // アルファブレンドを戻す
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // 描画メッセージの作成
    std::string t = "[ASSERT] " + message + "\n[ FILE ] " + file.substr(file.rfind("\\") + 1) + "\n[ LINE ] " + std::to_string(line);

    // エラーメッセージの描画
    DrawString(0, 0, t.c_str(), 0xff00ff00, 0xff004000);

    // 画面を更新
    ScreenFlip();

    // メッセージを検知
    while (ProcessMessage() != VIVID_DX_ERROR)
    {
        // Escキーで終了
        if (CheckHitKey(KEY_INPUT_ESCAPE) == TRUE)
            break;
    }

    // ライブラリの解放
    Finalize();

    // アプリケーションの終了
    exit(-1);
}

/*
 *  ロード済みのテクスチャ検索
 */
int
vivid::core::
FindLoadedTexture(const std::string& file_name)
{
    int  texture = VIVID_UNUSED_HANDLE;

    // テクスチャリストが空でない
    if (!g_TextureList.empty())
    {
        std::map<std::string, int>::iterator it = g_TextureList.begin();
        std::map<std::string, int>::iterator end = g_TextureList.end();

        for (; it != end; ++it)
        {
            // 一致するファイル名があるか検索
            if (it->first == file_name)
            {
                // 同じものがあれば読み込み済みのテクスチャを登録
                texture = it->second;

                break;
            }
        }
    }

    return texture;
}

/*
 *  ロード済みのサウンド検索
 */
int
vivid::core::
FindLoadedSound(const std::string& file_name)
{
    int sound = VIVID_UNUSED_HANDLE;

    // サウンドリストが空でない
    if (!g_SoundList.empty())
    {
        std::map<std::string, int>::iterator it = g_SoundList.begin();
        std::map<std::string, int>::iterator end = g_SoundList.end();

        for (; it != end; ++it)
        {
            // 一致するファイル名があるか検索
            if (it->first == file_name)
            {
                // 同じものがあれば読み込み済みのサウンドを登録
                sound = it->second;

                break;
            }
        }
    }

    return sound;
}

/*
 *  生成済みのフォント検索
 */
int
vivid::core::
FindCreatedFont(int size)
{
    int font = VIVID_UNUSED_HANDLE;

    // フォントリストが空でない
    if (!g_FontList.empty())
    {
        std::map<int, int>::iterator it = g_FontList.begin();
        std::map<int, int>::iterator end = g_FontList.end();

        for (; it != end; ++it)
        {
            // 一致するフォントがあるか検索
            if (it->first == size)
            {
                // 同じものがあれば生成済みのフォントを登録
                font = it->second;

                break;
            }
        }
    }

    return font;
}
/*!
 *  @brief      vivid名前空間
 */
namespace vivid
{
    /*!
     *  @brief      vivid::effekseer名前空間
     */
    namespace effekseer
    {
        static const float g_DefaultEffectScale = 25.0f;
    }
}
/*
 *  エフェクシアライブラリの初期化
 *  s.kosugi
 */
int vivid::effekseer::InitEffekseer(void)
{
    // Effekseerを初期化する。
    // 引数には画面に表示する最大パーティクル数を設定する。
    if (Effkseer_Init(8000) == -1)
    {
        return -1;
    }

    // フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
    // Effekseerを使用する場合は必ず設定する。
    SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

    // DXライブラリのデバイスロストした時のコールバックを設定する。
    // ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
    // ただし、DirectX11を使用する場合は実行する必要はない。
    Effekseer_SetGraphicsDeviceLostCallbackFunctions();

    // Effekseerに2D描画の設定をする。
    Effekseer_Set2DSetting(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Zバッファを有効にする。
    // Effekseerを使用する場合、2DゲームでもZバッファを使用する。
    SetUseZBuffer3D(TRUE);

    // Zバッファへの書き込みを有効にする。
    // Effekseerを使用する場合、2DゲームでもZバッファを使用する。
    SetWriteZBuffer3D(TRUE);

    return 0;
}

/*
 *  エフェクト読み込み
 *  s.kosugi
 */
int vivid::effekseer::LoadEffect(const std::string& file_name)
{
    // 読み込み済み
    if (g_EffekseerList.end() != g_EffekseerList.find(file_name))
        return 0;

    int handle = LoadEffekseerEffect(file_name.c_str());

    // 読み込み失敗
    if (-1 == handle) return -1;

    // 読み込み済みリストに追加
    g_EffekseerList[file_name] = handle;

    return 0;
}

/*
 *  エフェクト再生開始
 *  s.kosugi
 */
int vivid::effekseer::StartEffect(const std::string& file_name, const vivid::Vector2& pos)
{
    return StartEffect(file_name,pos, g_DefaultEffectScale);
}
/*
 *  エフェクト再生開始
 *  s.kosugi
 */
int vivid::effekseer::StartEffect(const std::string& file_name, const vivid::Vector2& pos,const float scale)
{
    // エフェクト読み込み
    if (-1 == LoadEffect(file_name))
        return -1;
    int PlayHandle = PlayEffekseer2DEffect(g_EffekseerList[file_name]);

    // エフェクトの拡大率を指定
    SetScalePlayingEffekseer2DEffect(PlayHandle, scale, scale, scale);

    PLAYEFFECT_DATA effect;
    effect.handle = PlayHandle;
    effect.pos = pos;
    // エフェクト再生リストに追加
    g_EffectPlayList.push_back(effect);

    return PlayHandle;
}
/*
 *  エフェクト描画
 *  s.kosugi
 */
bool vivid::effekseer::DrawEffect(const int handle, const vivid::Vector2& pos)
{
    if (IsEffekseer2DEffectPlaying(handle)) return false;

    // エフェクトの位置を変更する
    SetPosPlayingEffekseer2DEffect(handle, pos.x, pos.y, 0.0f);
    // エフェクトを描画する
    DrawEffekseer2D_Begin();
    DrawEffekseer2D_Draw(handle);
    DrawEffekseer2D_End();

    return true;
}
/*
 *  エフェクト再生終了チェック
 *  s.kosugi
 */
bool vivid::effekseer::IsEffectPlaying(const int handle)
{
    if (IsEffekseer2DEffectPlaying(handle)) return false;
    return true;
}
/*
 *  再生リストからエフェクト全描画
 *  s.kosugi
 */
void vivid::effekseer::DrawEffectList(void)
{
    if (g_EffectPlayList.empty()) return;

    auto it = g_EffectPlayList.begin();
    while (it != g_EffectPlayList.end())
    {
        PLAYEFFECT_DATA effect = (PLAYEFFECT_DATA)(*it);
        if (!DrawEffect(effect.handle, effect.pos))
        {
            // エフェクト再生終了済みのため削除
            it = g_EffectPlayList.erase(it);
            continue;
        }
        it++;
    }
}
/*
*  エフェクトリストから全描画をするかどうか
*  s.kosugi
*/
void vivid::effekseer::SetUseEffectListFlag(bool flag)
{
    g_UseEffectListFlag = flag;
}
