
/*!
 *  @file       vivid.h
 *  @brief      Vivid2D Library
 *  @author     Kazuya Maruyama
 *              Shinya Kosugi
 *              Toshihito Ohashi
 *  @date       2020/11/25
 *  @version    2.8
 *
 *  Copyright (c) 2013-2023, Kazuya Maruyama. Shinya Kosugi. Toshihito ohashi. All rights reserved.
 */

#pragma once

#include <DxLib.h>
#include <string>
#include <cmath>
#include <ctime>
#include <tuple>

/*!
 *  @brief  WindowsAPIで定義されているマクロを無効化
 */
#undef CreateFont
#undef DrawText
#undef PlaySound

#ifdef  _DEBUG
/*!
 *  @brief  デバッグ実行時マクロ<br>
 *          このマクロが有効な時、Debug用プロセスでプログラムを実行します。
 */
#define VIVID_DEBUG
#else
/*!
 *  @brief  リリリース実行時マクロ<br>
 *          このマクロが有効な時、Release用プロセスでプログラムを実行します。
 */
#define VIVID_RELEASE
#endif

/*!
 *  @brief      ポインタ変数解放し、NULLを設定
 *
 *  @param[in]  p   解放するポインタ
 */
#define VIVID_SAFE_DELETE( p )          if( ( p ) ) { delete ( p ); ( p ) = nullptr; }

/*!
 *  @brief      ポインタ変数解放し、NULLを設定
 *
 *  @param[in]  p   解放するポインタ
 */
#define VIVID_SAFE_DELETE_ARRAY( p )    if( ( p ) ) { delete[] ( p ); ( p ) = nullptr; }

/*!
 *  @brief      インターフェースクラスを解放し、NULLを設定
 *
 *  @param[in]  p   解放するインターフェースポインタ
 */
#define VIVID_SAFE_RELEASE( p )         if( ( p ) ) { ( p )->Release( ); ( p ) = nullptr; }

/*!
 *  @brief      DXLibエラー値
 */
#define VIVID_DX_ERROR          ( -1 )

/*!
 *  @brief      使用していないハンドル値
 */
#define VIVID_UNUSED_HANDLE     ( -1 )

#ifdef  VIVID_DEBUG
/*!
 *  @brief      コンソールに文字や値を出力する<br>
 *              C言語におけるprintf関数と同様の使い方。
 *
 *  @param[in]  format  書式指定文字列
 *  @param[in]  ...     文字列や変数など
 */
#define dprintf( format, ... )          printf( format, __VA_ARGS__ )

/*!
 *  @brief      コンソールに現在のコードラインを表示する。
 */
#define dprint_line                     dprintf( "Line - %d\n", __LINE__ )

/*!
 *  @brief      コンソールに現在のファイル名を表示する。
 */
#define dprint_file                     dprintf( "File - %s\n", __FILE__ )

/*!
 *  @brief      コンソールに文字列を表示する。
 *
 *  @param[in]  x   文字列
 */
#define VIVID_OUT_MESSAGE( x )          dprintf( x )

/*!
 *  @brief      コンソールに文字や値を出力する<br>
 *              C言語におけるprintf関数と同様の使い方。
 *
 *  @param[in]  format  書式指定文字列
 *  @param[in]  ...     文字列や変数など
 */
#define VIVID_TRACE( format, ... )      dprintf( format, __VA_ARGS__ )

/*!
 *  @brief      アサート処理
 *              ゲーム画面出力用
 *
 *  @param[in]  x       条件式
 *  @param[in]  message エラーメッセージ
 */
#define VIVID_ASSERT( x, message )      Assert( ( x ), ( message ), __FILE__, __LINE__ )

/*!
 *  @brief      アサート処理
 *              ゲーム画面出力用
 *
 *  @param[in]  x       条件式
 *  @param[in]  message エラーメッセージ
 */
#define VIVID_DX_ASSERT( x, message )   Assert( ( x ) != VIVID_DX_ERROR, ( message ), __FILE__, __LINE__ )

/*!
 *  @brief      メモリリークチェック
 */
#define VIVID_CHECK_MEMORY_LEAK         _CrtSetDbgFlag( _CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF )

/*!
 *  @brief      メモリリーク時にファイル名と行番号を表示する
 */
#define VIVID_NEW                       new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#else
#define dprintf( format, ... )
#define dprint_line
#define dprint_file
#define VIVID_OUT_MESSAGE( x )
#define VIVID_TRACE( format, ... )
#define VIVID_ASSERT( x, type )
#define VIVID_DX_ASSERT( x, type )
#define VIVID_CHECK_MEMORY_LEAK
#define VIVID_NEW                   new
#endif

/*!
 *  @brief      vivid名前空間
 */
namespace vivid
{
    const int WINDOW_WIDTH  = 1920;     //! ウィンドウの幅
    const int WINDOW_HEIGHT = 1080;      //! ウィンドウの高さ

    /*!
     *  @brief      vivid::keyboard名前空間
     */
    namespace keyboard
    {
        /*!
         *  @brief      キーID<br>
         *              キーボード判定で使用可能なキーID
         */
        enum class KEY_ID
        {
              DUMMY         = 0x00  //! Dummy
            , ESCAPE        = 0x01  //! Escape
            , ONE           = 0x02  //! 1
            , TWO           = 0x03  //! 2
            , THREE         = 0x04  //! 3
            , FOUR          = 0x05  //! 4
            , FIVE          = 0x06  //! 5
            , SIX           = 0x07  //! 6
            , SEVEN         = 0x08  //! 7
            , EIGHT         = 0x09  //! 8
            , NINE          = 0x0A  //! 9
            , ZERO          = 0x0B  //! 0
            //, MINUS         = 0x0C  //! -
            //, EQUALS        = 0x0D  //! =
            , BACK          = 0x0E  //! BackSpace
            , TAB           = 0x0F  //! Tab
            , Q             = 0x10  //! Q
            , W             = 0x11  //! W
            , E             = 0x12  //! E
            , R             = 0x13  //! R
            , T             = 0x14  //! T
            , Y             = 0x15  //! Y
            , U             = 0x16  //! U
            , I             = 0x17  //! I
            , O             = 0x18  //! O
            , P             = 0x19  //! P
            //, LBRACKET      = 0x1A  //! [
            //, RBRACKET      = 0x1B  //! ]
            , RETURN        = 0x1C  //! Enter
            , LCONTROL      = 0x1D  //! Ctrl(左側)
            , A             = 0x1E  //! A
            , S             = 0x1F  //! S
            , D             = 0x20  //! D
            , F             = 0x21  //! F
            , G             = 0x22  //! G
            , H             = 0x23  //! H
            , J             = 0x24  //! J
            , K             = 0x25  //! K
            , L             = 0x26  //! L
            //, SEMICOLON     = 0x27  //! ;(セミコロン)
            //, APOSTROPHE    = 0x28  //! '(シングルクォーテーション)
            //, GRAVE         = 0x29  //! `
            , LSHIFT        = 0x2A  //! Shift(左側)
            , BACKSLASH     = 0x2B  //! '\'(バックスラッシュまたは円マーク)
            , Z             = 0x2C  //! Z
            , X             = 0x2D  //! X
            , C             = 0x2E  //! C
            , V             = 0x2F  //! V
            , B             = 0x30  //! B
            , N             = 0x31  //! N
            , M             = 0x32  //! M
            //, COMMA         = 0x33  //! ,(カンマ)
            //, PERILD        = 0x34  //! .(ピリオド)
            //, SLASH         = 0x35  //! '/'(スラッシュ)
            //, RSHIFT        = 0x36  //! シフト(右側)
            //, MULTIPLY      = 0x37  //! *(NumPad)
            //, LMENU         = 0x38  //! Alt(左側)
            , SPACE         = 0x39  //! Space
            //, CAPIRAL       = 0x3A  //! CapsLock
            , F1            = 0x3B  //! F1
            , F2            = 0x3C  //! F2
            , F3            = 0x3D  //! F3
            , F4            = 0x3E  //! F4
            , F5            = 0x3F  //! F5
            , F6            = 0x40  //! F6
            , F7            = 0x41  //! F7
            , F8            = 0x42  //! F8
            , F9            = 0x43  //! F9
            , F10           = 0x44  //! F10
            //, NUMLOCK       = 0x45  //! NumLock
            //, SCROLL        = 0x46  //! ScrollLock
            , NUMPAD7       = 0x47  //! 7(NumPad)
            , NUMPAD8       = 0x48  //! 8(NumPad)
            , NUMPAD9       = 0x49  //! 9(NumPad)
            , SUBTRACT      = 0x4A  //! -(NumPad)
            , NUMPAD4       = 0x4B  //! 4(NumPad)
            , NUMPAD5       = 0x4C  //! 5(NumPad)
            , NUMPAD6       = 0x4D  //! 6(NumPad)
            , ADD           = 0x4E  //! +(NumPad)
            , NUMPAD1       = 0x4F  //! 1(NumPad)
            , NUMPAD2       = 0x50  //! 2(NumPad)
            , NUMPAD3       = 0x51  //! 3(NumPad)
            , NUMPAD0       = 0x52  //! 0(NumPad)
            , NUMPADDECIMAL = 0x53  //! .(NumPad)
            , F11           = 0x57  //! F11
            , F12           = 0x58  //! F12
            , F13           = 0x64  //! F13
            , F14           = 0x65  //! F14
            , F15           = 0x66  //! F15
            //, KANA          = 0x70  //! カナ
            //, CONVERT       = 0x79  //! 変換
            //, NOCONVERT     = 0x7B  //! 無変換
            //, YEN           = 0x7D  //! '\'
            //, NUMPADEQUALS  = 0x8D  //! =(NumPad)
            //, CIRCUMFLEX    = 0x90  //! ^(ハット)
            //, AT            = 0x91  //! @(アットマーク)
            //, COLON         = 0x92  //! :(コロン)
            //, UNDERLINE     = 0x93  //! _(アンダーライン)
            //, KANJI         = 0x94  //! 漢字
            //, STOP          = 0x95  //! Stop
            //, AX            = 0x96  //! JapanAX
            //, UNLABELED     = 0x97  //! J3100
            , NUMPADENTER   = 0x9C  //! Enter(NumPad)
            , RCONTROL      = 0x9D  //! Ctrl(右側)
            //, NUMPADCOMMA   = 0xB3  //! ,(NumPad)
            //, DIVIDE        = 0xB5  //! /(NumPad)
            //, SYSRQ         = 0xB7  //! SysRq
            //, RMENU         = 0xB8  //! Alt(右側)
            //, PAUSE         = 0xC5  //! Pause
            //, HOME          = 0xC7  //! Home
            , UP            = 0xC8  //! ↑
            //, PRIOR         = 0xC9  //! PageUp
            , LEFT          = 0xCB  //! ←
            , RIGHT         = 0xCD  //! →
            //, END           = 0xCF  //! End
            , DOWN          = 0xD0  //! ↓
            //, NEXT          = 0xD1  //! PageDown
            //, INSERT        = 0xD2  //! Insert
            //, DELETE        = 0xD3  //! Delete
            //, LWIN          = 0xDB  //! Windows
            //, RWIN          = 0xDC  //! Windows
            //, APPS          = 0xDD  //! Menu
            //, POWER         = 0xDE  //! Power
            //, SLEEP         = 0xDE  //! Windows
        };
    }

    /*!
     *  @brief      vivid::mouse名前空間
     */
    namespace mouse
    {
        /*!
         *  @brief      マウスボタンID<br>
         *              クリック判定で使用可能なボタンID
         */
        enum class BUTTON_ID
        {
              LEFT    = MOUSE_INPUT_LEFT      //! 左ボタン
            , RIGHT   = MOUSE_INPUT_RIGHT     //! 右ボタン
            , MIDDLE  = MOUSE_INPUT_MIDDLE    //! 中ボタン
        };
    }

    /*!
     *  @brief      vivid::controller名前空間
     */
    namespace controller
    {
        /*!
         *  @brief      デバイスID
         */
        enum class DEVICE_ID
        {
              PLAYER1       //! 1Pジョイパッド
            , PLAYER2       //! 2Pジョイパッド
            , PLAYER3       //! 3Pジョイパッド
            , PLAYER4       //! 4Pジョイパッド
            , MAX           //! 最大デバイス数
        };

        /*!
         *  @brief      ボタンID
         */
        enum class BUTTON_ID
        {
              UP              = XINPUT_BUTTON_DPAD_UP         //! ボタン上
            , DOWN            = XINPUT_BUTTON_DPAD_DOWN       //! ボタン下
            , LEFT            = XINPUT_BUTTON_DPAD_LEFT       //! ボタン左
            , RIGHT           = XINPUT_BUTTON_DPAD_RIGHT      //! ボタン右
            , START           = XINPUT_BUTTON_START           //! STARTボタン
            , BACK            = XINPUT_BUTTON_BACK            //! BACKボタン
            , LEFT_THUMB      = XINPUT_BUTTON_LEFT_THUMB      //! 左スティック押し込み
            , RIGHT_THUMB     = XINPUT_BUTTON_RIGHT_THUMB     //! 右スティック押し込み
            , LEFT_SHOULDER   = XINPUT_BUTTON_LEFT_SHOULDER   //! LBボタン
            , RIGHT_SHOULDER  = XINPUT_BUTTON_RIGHT_SHOULDER  //! RBボタン
            , A               = XINPUT_BUTTON_A               //! Aボタン
            , B               = XINPUT_BUTTON_B               //! Bボタン
            , X               = XINPUT_BUTTON_X               //! Xボタン
            , Y               = XINPUT_BUTTON_Y               //! Yボタン
        };
    }
    /*!
     *  @brief      アルファブレンドID定義
     */
    enum class ALPHABLEND
    {
        NOBELEND = DX_BLENDMODE_NOBLEND,     //! ブレンドなし
        ALPHA    = DX_BLENDMODE_ALPHA,       //! 半透明
        ADD      = DX_BLENDMODE_ADD,         //! 加算合成
        SUB      = DX_BLENDMODE_SUB,         //! 減算合成
        INVSRC   = DX_BLENDMODE_INVSRC,      //! 反転合成
    };

    /*!
     *  @brief      描画用関数ポインタ型
     */
//    typedef void(*DISPLAY_FUNC)(void);
    using DISPLAY_FUNC = void (*)(void);

    /*!
     *  @brief      二次元ベクトルクラス
     */
    class Vector2
    {
    public:
        float x;    //! X成分
        float y;    //! Y成分

        /*!
         *  @brief  コンストラクタ
         */
        Vector2(void);

        /*!
         *  @brief  コンストラクタ
         *
         *  @param[in]  x   X成分
         *  @param[in]  y   Y成分
         */
        Vector2(float x, float y);

        /*!
         *  @brief  コンストラクタ
         *
         *  @param[in]  v  ベクトル
         */
        Vector2(const Vector2& v);

        /*!
         *  @brief  デストラクタ
         */
        ~Vector2(void);

        /*!
         *  @brief      ベクトルの正規化<br>
         *              自身のベクトルも正規化される
         *
         *  @return     正規化されたベクトル
         */
        Vector2            Normalize(void);

        /*!
         *  @brief      ベクトルの正規化
         *
         *  @param[in]  v   ベクトルクラス
         *
         *  @return     正規化されたベクトル
         */
        static Vector2     Normalize(const Vector2& v);

        /*!
         *  @brief      ベクトルの大きさ取得
         *
         *  @return     ベクトルの大きさ
         */
        float               Length(void);

        /*!
         *  @brief      ベクトルの大きさ取得
         *
         *  @param[in]  v   ベクトルクラス
         *
         *  @return     ベクトルの大きさ
         */
        static float        Length(const Vector2& v);

        /*!
         *  @brief      2つのベクトルの内積<br>
         *              dot = vA・vB
         *
         *  @param[in]  vA  ベクトルクラス
         *  @param[in]  vB  ベクトルクラス
         *
         *  @return     内積
         */
        static float        Dot(const Vector2& vA, const Vector2& vB);

        /*!
         *  @brief      2つのベクトルの外積<br>
         *              cross = vA×vB
         *
         *  @param[in]  vA  ベクトルクラス
         *  @param[in]  vB  ベクトルクラス
         *
         *  @return     外積
         */
        static float        Cross(const Vector2& vA, const Vector2& vB);

        /*!
         *  @brief      ベクトルの回転（Z軸、転置
         *
         *  @param[in]  v       ベクトルクラス
         *  @param[in]  angle   回転角（ラジアン）
         *
         *  @return     ベクトルクラス
         */
        static Vector2      Rotate(const Vector2& v, float angle);

        /*!
         *  @brief      代入演算子のオーバーロード
         *
         *  @param[in]  v   ベクトルクラス
         *
         *  @return     ベクトルクラス
         */
        Vector2&           operator=(const Vector2& v);

        /*!
         *  @brief      加算演算子のオーバーロード
         *
         *  @param[in]  v   ベクトルクラス
         *
         *  @return     ベクトルクラス
         */
        Vector2&           operator+=(const Vector2& v);

        /*!
         *  @brief      減算演算子のオーバーロード
         *
         *  @param[in]  v   ベクトルクラス
         *
         *  @return     ベクトルクラス
         */
        Vector2&           operator-=(const Vector2& v);

        /*!
         *  @brief      乗算演算子のオーバーロード
         *
         *  @param[in]  scalar  スカラー値
         *
         *  @return     ベクトルクラス
         */
        Vector2&           operator*=(float scalar);

        /*!
         *  @brief      等価演算子のオーバーロード
         *
         *  @param[in]  v   ベクトルクラス
         *
         *  @return     二つのベクトルが等しい
         *  @retval     true    等しい
         *  @retval     false   等しくない
         */
        bool                operator==(const Vector2& v) const;

        /*!
         *  @brief      不等演算子のオーバーロード
         *
         *  @param[in]  v   ベクトルクラス
         *
         *  @return     二つのベクトルが等しくない
         *  @retval     true    等しくない
         *  @retval     false   等しい
         */
        bool                operator!=(const Vector2& v) const;

        /*!
         *  @brief      正符号演算子のオーバーロード
         *
         *  @param[in]  v   ベクトルクラス
         *
         *  @return     ベクトルクラス
         */
        friend Vector2     operator+(const Vector2& v);

        /*!
         *  @brief      負符号演算子のオーバーロード
         *
         *  @param[in]  v   ベクトルクラス
         *
         *  @return     ベクトルクラス
         */
        friend Vector2     operator-(const Vector2& v);

        /*!
         *  @brief      加算演算子のオーバーロード
         *
         *  @param[in]  vA  ベクトルクラス
         *  @param[in]  vB  ベクトルクラス
         *
         *  @return     ベクトルクラス
         */
        friend Vector2     operator+(const Vector2& vA, const Vector2& vB);

        /*!
         *  @brief      減算演算子のオーバーロード
         *
         *  @param[in]  vA  ベクトルクラス
         *  @param[in]  vB  ベクトルクラス
         *
         *  @return     ベクトルクラス
         */
        friend Vector2     operator-(const Vector2& vA, const Vector2& vB);

        /*!
         *  @brief      乗算演算子のオーバーロード
         *
         *  @param[in]  vA  ベクトルクラス
         *  @param[in]  vB  ベクトルクラス
         *
         *  @return     ベクトルクラス
         */
        friend Vector2     operator*(const Vector2& vA, const Vector2& vB);

        /*!
         *  @brief      乗算演算子のオーバーロード
         *
         *  @param[in]  v       ベクトルクラス
         *  @param[in]  scalar  スカラー値
         *
         *  @return     ベクトルクラス
         */
        friend Vector2     operator*(const Vector2& v, float scalar);

        /*!
         *  @brief      乗算演算子のオーバーロード
         *
         *  @param[in]  scalar  スカラー値
         *  @param[in]  v       ベクトルクラス
         *
         *  @return     ベクトルクラス
         */
        friend Vector2     operator*(float scalar, const Vector2& v);

        static const Vector2   ZERO;   //! 成分がすべて0のベクトル
        static const Vector2   ONE;    //! 成分がすべて1のベクトル
    };

    /*!
     *  @brief      正符号演算子のオーバーロード
     *
     *  @param[in]  v   ベクトルクラス
     *
     *  @return     ベクトルクラス
     */
    Vector2     operator+(const Vector2& v);

    /*!
     *  @brief      負符号演算子のオーバーロード
     *
     *  @param[in]  v   ベクトルクラス
     *
     *  @return     ベクトルクラス
     */
    Vector2     operator-(const Vector2& v);

    /*!
     *  @brief      加算演算子のオーバーロード
     *
     *  @param[in]  vA  ベクトルクラス
     *  @param[in]  vB  ベクトルクラス
     *
     *  @return     ベクトルクラス
     */
    Vector2     operator+(const Vector2& vA, const Vector2& vB);

    /*!
     *  @brief      減算演算子のオーバーロード
     *
     *  @param[in]  vA  ベクトルクラス
     *  @param[in]  vB  ベクトルクラス
     *
     *  @return     ベクトルクラス
     */
    Vector2     operator-(const Vector2& vA, const Vector2& vB);

    /*!
     *  @brief      乗算演算子のオーバーロード
     *
     *  @param[in]  vA  ベクトルクラス
     *  @param[in]  vB  ベクトルクラス
     *
     *  @return     ベクトルクラス
     */
    Vector2     operator*(const Vector2& vA, const Vector2& vB);

    /*!
     *  @brief      乗算演算子のオーバーロード
     *
     *  @param[in]  v       ベクトルクラス
     *  @param[in]  scalar  スカラー値
     *
     *  @return     ベクトルクラス
     */
    Vector2     operator*(const Vector2& v, float scalar);

    /*!
     *  @brief      乗算演算子のオーバーロード
     *
     *  @param[in]  scalar  スカラー値
     *  @param[in]  v       ベクトルクラス
     *
     *  @return     ベクトルクラス
     */
    Vector2     operator*(float scalar, const Vector2& v);

    /*!
     *  @brief      ポイント構造体
     */
    struct Point
    {
        int x;          //! X座標
        int y;          //! Y座標
    };

    /*!
     *  @brief      矩形構造体
     */
    struct Rect
    {
        int left;       //! 左端
        int top;        //! 上端
        int right;      //! 右端
        int bottom;     //! 下端
    };
    /*!
     *  @brief      再生エフェクト構造体 s.kosugi
     */
    struct PLAYEFFECT_DATA
    {
        int handle;             //! 再生ハンドル
        vivid::Vector2 pos;     //! エフェクト位置

        /*!
         *  @brief  コンストラクタ
         */
        PLAYEFFECT_DATA(void);
    };

    /*!
     *  @brief      初期化
     *
     *  @param[in]  hInst       インスタンスハンドル
     */
    void    Initialize(HINSTANCE hInst);

    /*!
     *  @brief      描画関数登録
     *
     *  @param[in]  display     描画用関数
     */
    void    DisplayFunction(DISPLAY_FUNC display);

    /*!
     *  @brief      画面転送
     */
    void    MainLoop(void);

    /*!
     *  @brief      解放
     */
    void    Finalize(void);

    /*!
     *  @brief      ウィンドウの横幅取得
     *
     *  @return     ウィンドウの横幅
     */
    int     GetWindowWidth(void);

    /*!
     *  @brief      ウィンドウの縦幅取得
     *
     *  @return     ウィンドウの縦幅
     */
    int     GetWindowHeight(void);

    /*!
     *  @brief      デルタタイム取得
     *              デルタタイムは前回の更新からどれくらいの時間が経過したかの値
     *
     *  @return     デルタタイム
     */
    float   GetDeltaTime(void);

    /*!
     *  @brief      デルタタイム(タイムスケールなし)取得
     *              デルタタイムは前回の更新からどれくらいの時間が経過したかの値
     *
     *  @return     デルタタイム
     */
    float   GetUnscaledDeltaTime(void);

    /*!
     *  @brief      デルタタイム倍率取得
     *
     *  @return     デルタタイム倍率
     */
    float   GetDeltaTimeScale(void);

    /*!
     *  @brief      デルタタイム倍率設定
     *
     *  @param[in]  scale   デルタタイム倍率
     */
    void    SetDeltaTimeScale(float scale);

    /*!
     *  @brief      現在のFPS取得
     *
     *  @return     現在のFPS
     */
    float   GetFPS(void);

    /*!
     *  @brief      フレームレートの設定
     *
     *  @param[int] frame_rate  フレームレート
     */
    void    SetFrameRate(int fram_rate);

    /*!
     *  @brief      テクスチャ読み込み
     *
     *  @param[in]  file_name   ファイル名
     */
    void    LoadTexture(const std::string& file_name);

    /*!
     *  @brief      テクスチャ描画
     *
     *  @param[in]  file_name   ファイル名
     *  @param[in]  position    表示位置
     */
    void    DrawTexture(const std::string& file_name, const Vector2& position);

    /*!
     *  @brief      テクスチャ描画
     *
     *  @param[in]  file_name   ファイル名
     *  @param[in]  position    表示位置
     *  @param[in]  color       色
     */
    void    DrawTexture(const std::string& file_name, const Vector2& position, unsigned int color);

    /*!
     *  @brief      テクスチャ描画
     *
     *  @param[in]  file_name   ファイル名
     *  @param[in]  position    表示位置
     *  @param[in]  color       色
     *  @param[in]  rect        読み込み範囲
     */
    void    DrawTexture(const std::string& file_name, const Vector2& position, unsigned int color, const Rect& rect);

    /*!
     *  @brief      テクスチャ描画
     *
     *  @param[in]  file_name   ファイル名
     *  @param[in]  position    表示位置
     *  @param[in]  color       色
     *  @param[in]  rect        読み込み範囲
     *  @param[in]  anchor      拡大・回転の基準点
     *  @param[in]  scale       拡大率
     */
    void    DrawTexture(const std::string& file_name, const Vector2& position, unsigned int color, const Rect& rect, const Vector2& anchor, const Vector2& scale);

    /*!
     *  @brief      テクスチャ描画
     *
     *  @param[in]  file_name   ファイル名
     *  @param[in]  position    表示位置
     *  @param[in]  color       色
     *  @param[in]  rect        読み込み範囲
     *  @param[in]  anchor      拡大・回転の基準点
     *  @param[in]  rotation    回転値(ラジアン)
     */
    void    DrawTexture(const std::string& file_name, const Vector2& position, unsigned int color, const Rect& rect, const Vector2& anchor, float rotation);

    /*!
     *  @brief      テクスチャ描画
     *
     *  @param[in]  file_name   ファイル名
     *  @param[in]  position    表示位置
     *  @param[in]  color       色
     *  @param[in]  rect        読み込み範囲
     *  @param[in]  anchor      拡大・回転の基準点
     *  @param[in]  scale       拡大率
     *  @param[in]  rotation    回転値(ラジアン)
     */
    void    DrawTexture(const std::string& file_name, const Vector2& position, unsigned int color, const Rect& rect, const Vector2& anchor, const Vector2& scale, float rotation);

    /*!
     *  @brief      テクスチャ描画
     *
     *  @param[in]  file_name   ファイル名
     *  @param[in]  position    表示位置
     *  @param[in]  color       色
     *  @param[in]  rect        読み込み範囲
     *  @param[in]  anchor      拡大・回転の基準点
     *  @param[in]  scale       拡大率
     *  @param[in]  rotation    回転値(ラジアン)
     *  @param[in]  blend_mode  アルファブレンドモード
     */
    void    DrawTexture(const std::string& file_name, const Vector2& position, unsigned int color, const Rect& rect, const Vector2& anchor, const Vector2& scale, float rotation, ALPHABLEND blend_mode);

    /*!
     *  @brief      テクスチャの幅を取得
     *
     *  @param[in]  file_name   ファイル名
     */
    int     GetTextureWidth(const std::string& file_name);

    /*!
     *  @brief      テクスチャの高さを取得
     *
     *  @param[in]  file_name   ファイル名
     */
    int     GetTextureHeight(const std::string& file_name);

    /*!
     *  @brief      フォントの生成
     *
     *  @param[in]  size            フォントサイズ
     */
    void    CreateFont(int size);

    /*!
     *  @brief      フォントの生成
     *
     *  @param[in]  size            フォントサイズ
     *  @param[in]  edge_size       エッジサイズ
     */
    void    CreateFont(int size, int edge_size);

    /*!
     *  @brief      テキスト描画
     *
     *  @param[in]  size        フォントサイズ
     *  @param[in]  text        テキスト
     *  @param[in]  position    表示位置
     */
    void    DrawText(int size, const std::string& text, const Vector2& position);

    /*!
     *  @brief      テキスト描画
     *
     *  @param[in]  size        フォントサイズ
     *  @param[in]  text        テキスト
     *  @param[in]  position    表示位置
     *  @param[in]  color       文字色
     */
    void    DrawText(int size, const std::string& text, const Vector2& position, unsigned int color);

    /*!
     *  @brief      テキスト描画
     *
     *  @param[in]  size        フォントサイズ
     *  @param[in]  text        テキスト
     *  @param[in]  position    表示位置
     *  @param[in]  color       文字色
     *  @param[in]  edge_color  エッジの色
     */
    void    DrawText(int size, const std::string& text, const Vector2& position, unsigned int color, unsigned int edge_color);

    /*!
     *  @brief      テキスト描画
     *
     *  @param[in]  size        フォントサイズ
     *  @param[in]  text        テキスト
     *  @param[in]  position    表示位置
     *  @param[in]  color       文字色
     *  @param[in]  blend_mode  アルファブレンドモード
     */
    void    DrawText(int size, const std::string& text, const Vector2& position, unsigned int color, ALPHABLEND blend_mode);

    /*!
     *  @brief      テキスト描画
     *
     *  @param[in]  size        フォントサイズ
     *  @param[in]  text        テキスト
     *  @param[in]  position    表示位置
     *  @param[in]  color       文字色
     *  @param[in]  edge_color  エッジの色
     *  @param[in]  blend_mode  アルファブレンドモード
     */
    void    DrawText(int size, const std::string& text, const Vector2& position, unsigned int color, unsigned int edge_color, ALPHABLEND blend_mode);

    /*!
     *  @brief      テキストの幅を取得
     *
     *  @param[in]  size        フォントサイズ
     *  @param[in]  text        テキスト
     */
    int     GetTextWidth(int size, const std::string& text);

    /*!
     *  @brief      ライン描画
     *
     *  @param[in]  start_pos   始点位置
     *  @param[in]  end_pos     終点位置
     *  @param[in]  color       文字色
     *  @param[in]  thickness   幅
     *  @param[in]  blend_mode  アルファブレンドモード
     */
    void DrawLine(const Vector2& start_pos, const Vector2& end_pos, unsigned int color, float thickness = 1.0f, ALPHABLEND blend_mode = ALPHABLEND::NOBELEND);

    /*!
     *  @brief      vivid::keyboard名前空間
     */
    namespace keyboard
    {
        /*!
         *  @brief      ボタン入力判定
         *
         *  @param[in]  key  キーID
         *
         *  @return     押されている間trueを返す
         */
        bool    Button(KEY_ID key);

        /*!
         *  @brief      トリガー入力判定
         *
         *  @param[in]  key  キーID
         *
         *  @return     押された瞬間だけtrueを返す
         */
        bool    Trigger(KEY_ID key);

        /*!
         *  @brief      リリースド入力判定
         *
         *  @param[in]  key  キーID
         *
         *  @return     離された瞬間だけtrueを返す
         */
        bool    Released(KEY_ID key);
    }

    /*!
     *  @brief      vivid::mouse名前空間
     */
    namespace mouse
    {
        /*!
         *  @brief      ボタン入力判定
         *
         *  @param[in]  button  ボタンID
         *
         *  @return     押されている間trueを返す
         */
        bool    Button(BUTTON_ID button);

        /*!
         *  @brief      トリガー入力判定
         *
         *  @param[in]  button  ボタンID
         *
         *  @return     押された瞬間だけtrueを返す
         */
        bool    Trigger(BUTTON_ID button);

        /*!
         *  @brief      リリースド入力判定
         *
         *  @param[in]  button  ボタンID
         *
         *  @return     離された瞬間だけtrueを返す
         */
        bool    Released(BUTTON_ID button);

        /*!
         *  @brief      マウスカーソルの座標取得
         *
         *  @return     マウスカーソル座標
         */
        Point   GetCursorPos(void);

        /*!
         *  @brief      マウスホイール値取得
         *
         *  @return     ホイール値
         *  @retval     正の数     奥にホイールされた
         *  @retval     負の数     手前にホイールされた
         *  @retval     0          ホイールされていない
         */
        int     GetWheel(void);
    }

    /*!
     *  @brief      vivid::controller名前空間
     */
    namespace controller
    {
        /*!
         *  @brief      ボタン入力判定
         *
         *  @param[in]  device  デバイスID
         *  @param[in]  button  ボタンID
         *
         *  @return     押されている間trueを返す
         */
        bool    Button(DEVICE_ID device, BUTTON_ID button );

        /*!
         *  @brief      トリガー入力判定
         *
         *  @param[in]  device  デバイスID
         *  @param[in]  button  ボタンID
         *
         *  @return     押された瞬間だけtrueを返す
         */
        bool    Trigger(DEVICE_ID device, BUTTON_ID button);

        /*!
         *  @brief      リリースド入力判定
         *
         *  @param[in]  device  デバイスID
         *  @param[in]  button  ボタンID
         *
         *  @return     離された瞬間だけtrueを返す
         */
        bool    Released(DEVICE_ID device, BUTTON_ID button);

        /*!
         *  @brief      左アナログスティック取得
         *
         *  @param[in]  device      デバイスID
         *
         *  @return     左アナログスティックの状態(-1.0f～1.0f)
         */
        vivid::Vector2  GetAnalogStickLeft(DEVICE_ID device);

        /*!
         *  @brief      右アナログスティック取得
         *
         *  @param[in]  device      デバイスID
         *
         *  @return     右アナログスティックの状態(-1.0f～1.0f)
         */
        vivid::Vector2  GetAnalogStickRight(DEVICE_ID device);

        /*!
         *  @brief      左トリガー取得
         *
         *  @param[in]  device      デバイスID
         *
         *  @return     左トリガーの状態(0.0f～1.0f)
         */
        float           GetTriggerLeft(DEVICE_ID device);

        /*!
         *  @brief      右トリガー取得
         *
         *  @param[in]  device      デバイスID
         *
         *  @return     右トリガーの状態(0.0f～1.0f)
         */
        float           GetTriggerRight(DEVICE_ID device);

        /*!
         *  @brief      バイブレーションの開始
         *
         *  @param[in]  device      デバイスID
         *  @param[in]  power       振動の強さ(0～1000)
         *  @param[in]  time        振動する時間(秒単位)
         */
        void            StartVibration(DEVICE_ID device, int power, float time);

        /*!
         *  @brief      接続数取得
         *
         *  @return     接続数
         */
        int             GetConnectCount(void);
    }

    /*!
     *  @brief      サウンドファイルの読み込み
     *
     *  @param[in]  file_name   ファイル名
     */
    void    LoadSound(const std::string& file_name);

    /*!
     *  @brief      サウンド再生
     *
     *  @param[in]  file_name   ファイル名
     *  @param[in]  loop        ループ再生
     */
    void    PlaySound(const std::string& file_name, bool loop);

    /*!
     *  @brief      サウンド停止
     *
     *  @param[in]  file_name   ファイル名
     */
    void    StopSound(const std::string& file_name);

    /*!
     *  @brief      vivid::core名前空間
     */
    namespace core
    {
        /*!
         *  @brief      更新するフレームカウントの計算
         */
        void    CalcFrameCount(void);

        /*!
         *  @brief      更新フレームの待機処理
         */
        void    FrameSync(void);

        /*!
         *  @brief      スクリーンショットの撮影
         */
        void    ScreenShot(void);

        /*!
         *  @brief      アサート処理
         *              ゲーム画面出力用
         *
         *  @param[in]  result      条件や結果を渡す。falseになるとアサートが発生する
         *  @param[in]  message     エラーメッセージ
         *  @param[in]  file        ファイル名
         *  @param[in]  line        行番号
         */
        void    Assert(bool result, const std::string& message, const std::string& file, int line);

        /*!
         *  @brief      読み込み済みのテクスチャ検索
         *
         *  @param[in]  file_name   ファイル名
         */
        int     FindLoadedTexture(const std::string& file_name);

        /*!
         *  @brief      読み込み済みのサウンド検索
         *
         *  @param[in]  file_name   ファイル名
         */
        int     FindLoadedSound(const std::string& file_name);

        /*!
         *  @brief      生成済みのフォント検索
         *
         *  @param[in]  size        フォントサイズ
         */
        int     FindCreatedFont(int size);
    }

    /*!
     *  @brief      vivid::effekseer名前空間
     *  @author     s.kosugi
     */
    namespace effekseer
    {
        /*!
         *  @brief      エフェクシアライブラリの初期化
         *
         *  @return     正常終了:0  エラー:-1
         */
        int InitEffekseer(void);

        /*!
         *  @brief      エフェクトファイル読み込み
         *
         *  @param[in]  file_name   ファイル名
         *
         *  @return     正常終了:0  エラー:-1
         */
        int LoadEffect(const std::string& file_name);

        /*!
         *  @brief      エフェクトの再生開始
         *              成功するとエフェクトハンドルが返る
         *
         *  @param[in]  file_name   ファイル名
         *  @param[in]  pos         再生位置
         *
         *  @return     エラー:-1   正常終了:それ以外
         */
        int StartEffect(const std::string& file_name, const vivid::Vector2& pos);

        /*!
         *  @brief      エフェクトの再生開始
         *              成功するとエフェクトハンドルが返る
         *
         *  @param[in]  file_name   ファイル名
         *  @param[in]  pos         再生位置
         *  @param[in]  scale       拡大率
         *
         *  @return     エラー:-1   正常終了:それ以外
         */
        int StartEffect(const std::string& file_name, const vivid::Vector2& pos,float scale);

        /*!
         *  @brief      エフェクトの描画
         *
         *  @param[in]  handle      エフェクトの再生ハンドル
         *  @param[in]  pos         エフェクトの再生位置
         *
         *  @return     描画成功:true  再生終了済:false
         */
        bool DrawEffect(const int handle, const vivid::Vector2& pos);

        /*!
         *  @brief      エフェクトの再生終了チェック
         *
         *  @param[in]  handle      エフェクトの再生ハンドル
         *
         *  @return     再生中:true  再生終了:false
         */
        bool IsEffectPlaying(const int handle);

        /*!
         *  @brief      再生リストからエフェクト全描画
         *
         */
        void DrawEffectList(void);

        /*!
         *  @brief      エフェクトリストから全描画をするかどうか
         *
         *  @param[in]  flag        描画する : true  描画しない : false
         *
         */
        void SetUseEffectListFlag(bool flag);
    }
}
