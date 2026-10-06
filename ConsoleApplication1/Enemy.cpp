#include "Enemy.h"

#include "EnemyDataTable.h"

Enemy::Enemy()
    : Character("待機中", 1, 0)
{
}

void Enemy::Reset(const EnemyData& data)
{
    // データの中身を読む .cpp では、EnemyData の定義が必要。
    // 親クラスの protected 関数で名前・能力・HP をまとめて設定する。
    ResetStats(data.name, data.maxHp, data.attack);
}
