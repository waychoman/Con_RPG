#pragma once

#include "GameState.h"
#include "Player.h"
#include "EnemyPool.h"
#include "EnemyFactory.h"
#include "CombatRandom.h"

#include <cstddef>
#include <iosfwd>

// 画面の状態とは別に、次の敵の行動を記録する。
enum class EnemyIntent
{
    Attack,
    HeavyAttack,
    Guard
};

class Game
{
public:
    Game();
    // 検証では開始値（シード）を固定し、同じ戦闘を再現できる。
    explicit Game(unsigned int seed);
    void Run(std::istream& input, std::ostream& output);

private:
    void HandleTitle(std::istream& input, std::ostream& output);
    void HandleBattle(std::istream& input, std::ostream& output);
    void HandleResult(std::istream& input, std::ostream& output);
    void StartNewGame(std::ostream& output);
    bool SpawnEnemy(std::ostream& output);
    void PrepareEnemyIntent();
    void ShowEnemyIntent(std::ostream& output) const;
    void AttackEnemy(int strengthPercent, bool ignoreGuard, std::ostream& output);
    void ResolveEnemyTurn(bool defending, std::ostream& output);
    void ReleaseEnemy();
    void ExitGame();

    // 宣言順に構築される。ファクトリーより先にプールを用意する。
    Player player_;
    EnemyPool enemyPool_;
    EnemyFactory enemyFactory_;
    CombatRandom random_;

    // プールから借りた敵。Game はこのポインターを delete しない。
    Enemy* currentEnemy_;
    GameState state_;
    GameResult result_;
    EnemyIntent enemyIntent_;
    // 前の有効な手番が防御なら、次の手番は別の行動を選ぶ。
    bool defendedLastTurn_;
    std::size_t defeatedCount_;
};
