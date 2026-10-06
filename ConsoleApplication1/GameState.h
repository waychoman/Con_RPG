#pragma once

// 「今、どの画面を処理しているか」を表す有限個の状態。
// Game.cpp がこの値に応じて処理を選び、次の状態へ切り替える。
enum class GameState
{
    Title,
    Battle,
    Result,
    Exit
};

// 結果画面に来た理由。画面の状態と勝敗は別々に管理する。
enum class GameResult
{
    None,
    Clear,
    GameOver,
    Retired
};
