#pragma once

#include <array>
#include <cstddef>

#include "Enemy.h"

// オブジェクトプール: 敵を先に2体作り、必要なときに貸し出して再利用する。
// 同時に戦うのは1体だが、複数の貸し出しと満杯を扱えるよう2枠用意する。
class EnemyPool
{
public:
    // = default はコンパイラの標準のコンストラクタを使うという指定。
    EnemyPool() = default;

    // 空いているEnemyを貸す。すべて使用中なら nullptr。
    Enemy* Acquire();

    // 自分が貸し出したEnemyを返してもらう。無効な返却なら false。
    // 返却してもEnemy自体は消えない。次の貸し出しで再利用する。
    bool Release(Enemy* enemy);

    std::size_t GetInUseCount() const;

    // 貸し出し中のアドレスを安定させるため、プールをコピー・移動させない。
    // コピー操作を宣言して削除すると、移動操作も自動生成されない。
    EnemyPool(const EnemyPool&) = delete;
    EnemyPool& operator=(const EnemyPool&) = delete;

private:
    // Enemyの所有者はプール。借りたポインタに delete を使ってはいけない。
    // ポインタを使えるのは貸し出し中かつプールが生きている間だけ。
    std::array<Enemy, 2> enemies_;

    // {} により最初は2枠とも false（未使用）になる。
    std::array<bool, 2> inUse_{};
};