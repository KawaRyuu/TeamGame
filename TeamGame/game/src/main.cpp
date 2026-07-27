
/*!
 *  @file       main.cpp
 *  @brief      エントリーポイント
 *  @author     Kazuya Maruyama
 *				Toshihito Ohashi
 *  @date       2023/05/08
 *  @version    1.0
 *
 *  Copyright (c) 2013-2023, Toshihito Ohashi. All rights reserved.
 */

#include "vivid.h"
#include "contol.h"
#include "game_manager/game_manager.h"

static const char* g_window_name = "OneStroke Link";        // ウィンドウ名

/**
*   @brief      描画関数
*/
void Display()
{
    // contolライブラリ更新
    contol::Update();

    // ゲームマネージャー更新
    GAMES.Update();

    // ゲームマネージャー描画
    GAMES.Draw();

}

/**
*   @brief      ウィンドウ情報の設定
*/
void SetWindowInfo()
{
    // ウィンドウ非アクティブ時の更新の停止
    DxLib::SetAlwaysRunFlag(FALSE);

    // ウィンドウテキストの変更
    SetMainWindowText(g_window_name);

    // マウスの表示
    DxLib::SetMouseDispFlag(TRUE);
}

/*!
 *  @brief      メイン関数
 *
 *  @param[in]  hInst       インスタンスハンドル
 *  @param[in]  hPrevInst   Win16時代の遺物
 *  @param[in]  lpCmdLine   コマンドライン文字列
 *  @param[in]  nCmdShow    ウィンドの表示方法
 *
 *  @return     メイン関数の成否
 */
int WINAPI
WinMain( _In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{
    (void)hPrevInst;
    (void)lpCmdLine;
    (void)nCmdShow;

    // vividライブラリ初期化
    vivid::Initialize(hInst);

    // ウィンドウ情報の設定
    SetWindowInfo();

    // contolライブラリ初期化
    contol::Initialize();

    // ゲームマネージャー初期化
    GAMES.Initialize();

    // 更新/描画関数登録
    vivid::DisplayFunction(Display);

    // ゲームループ
    vivid::MainLoop();

    // ゲームマネージャー解放
    GAMES.Finalize();

    // vividライブラリ解放
    vivid::Finalize();
}
