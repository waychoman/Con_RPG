#pragma once

#include "Character.h"

struct EnemyData;

class Enemy : public Character
{
public:
    Enemy();
    void Reset(const EnemyData& data);
    void SetBattleHp(int maxHp);

    int GetHeavyAttackChance() const;
    int GetGuardChance() const;

private:
    int heavyAttackChance_;
    int guardChance_;
};
