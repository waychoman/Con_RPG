#pragma once

#include "GameState.h"
#include "Player.h"
#include "EnemyPool.h"
#include "EnemyFactory.h"

#include <cstddef>
#include <iosfwd>

// 戦闘ルールと画面の切り替えをまとめる。
// 敵の数値や作り方は、データテーブルとファクトリーに任せる。
class Game
{
public:
    Game();
    void Run(std::istream& input, std::ostream& output);

private:
    void HandleTitle(std::istream& input, std::ostream& output);
    void HandleBattle(std::istream& input, std::ostream& output);
    void HandleResult(std::istream& input, std::ostream& output);
    void StartNewGame(std::ostream& output);
    bool SpawnEnemy(std::ostream& output);
    void ReleaseEnemy();
    void ExitGame();

    // 宣言順に構築される。ファクトリーより先にプールを用意する。
    Player player_;
    EnemyPool enemyPool_;
    EnemyFactory enemyFactory_;

    // プールから借りた敵。Game はこのポインターを delete しない。
    Enemy* currentEnemy_;
    GameState state_;
    GameResult result_;
    std::size_t defeatedCount_;
};
