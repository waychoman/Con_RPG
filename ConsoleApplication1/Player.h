#pragma once

#include "Character.h"

// 勇者は Character のHP管理を継承し、回復薬と気力を追加する。
class Player : public Character
{
public:
    Player();

    int GetPotionCount() const;
    int GetPotionHealingAmount() const;
    int GetEnergy() const;
    int GetMaxEnergy() const;
    int GetPowerAttackCost() const;

    int UsePotion();
    bool TryUsePowerAttack();
    void RestoreEnergy();
    void Reset();

private:
    int potionCount_;
    int energy_;
};
