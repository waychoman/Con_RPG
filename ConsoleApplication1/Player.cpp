#include "Player.h"

#include <algorithm>

namespace
{
    // 勇者の調整値をここに集める。再挑戦と初回は同じ設定を使う。
    const int StartingHp = 100;
    const int StartingAttack = 12;
    const int StartingPotions = 2;
    const int PotionHealing = 25;
    const int MaximumEnergy = 3;
    const int PowerAttackCost = 2;
}

Player::Player()
    : Character("勇者", StartingHp, StartingAttack),
      potionCount_(StartingPotions), energy_(MaximumEnergy)
{
}

int Player::GetPotionCount() const { return potionCount_; }
int Player::GetPotionHealingAmount() const { return PotionHealing; }
int Player::GetEnergy() const { return energy_; }
int Player::GetMaxEnergy() const { return MaximumEnergy; }
int Player::GetPowerAttackCost() const { return PowerAttackCost; }

int Player::UsePotion()
{
    if (potionCount_ <= 0)
    {
        return 0;
    }

    const int recoveredHp = Heal(PotionHealing);
    if (recoveredHp > 0)
    {
        --potionCount_;
    }
    return recoveredHp;
}

bool Player::TryUsePowerAttack()
{
    if (energy_ < PowerAttackCost)
    {
        return false;
    }
    energy_ -= PowerAttackCost;
    return true;
}

void Player::RestoreEnergy()
{
    energy_ = std::min(MaximumEnergy, energy_ + 1);
}

void Player::Reset()
{
    ResetStats("勇者", StartingHp, StartingAttack);
    potionCount_ = StartingPotions;
    energy_ = MaximumEnergy;
}
