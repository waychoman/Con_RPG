#include "Enemy.h"
#include "EnemyDataTable.h"

#include <algorithm>

Enemy::Enemy()
    : Character("待機中", 1, 0), heavyAttackChance_(0), guardChance_(0)
{
}

void Enemy::Reset(const EnemyData& data)
{
    ResetStats(data.name, data.maxHp, data.attack);
    heavyAttackChance_ = std::max(0, std::min(100, data.heavyAttackChance));
    guardChance_ = std::max(0, std::min(100 - heavyAttackChance_, data.guardChance));
}

void Enemy::SetBattleHp(int maxHp)
{
    // 設定表を変更せず、この戦闘の敵だけHPを設定し直す。
    ResetStats(GetName(), maxHp, GetAttack());
}

int Enemy::GetHeavyAttackChance() const { return heavyAttackChance_; }
int Enemy::GetGuardChance() const { return guardChance_; }
