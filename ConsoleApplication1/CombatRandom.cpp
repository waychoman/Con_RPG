#include "CombatRandom.h"

#include <algorithm>
#include <limits>

CombatRandom::CombatRandom(unsigned int seed)
    : engine_(seed)
{
}

int CombatRandom::Between(int minimum, int maximum)
{
    std::uniform_int_distribution<int> distribution(minimum, maximum);
    return distribution(engine_);
}

AttackRoll CombatRandom::RollAttack(int attack, int strengthPercent)
{
    if (attack <= 0 || strengthPercent <= 0)
    {
        return { 0, false };
    }

    // 基準の80～120%。強攻撃などの倍率を先に適用する。
    // long long と上限処理で、大きな設定値でも整数のあふれを防ぐ。
    const long long maximum = (std::numeric_limits<int>::max)();
    const long long scaled = std::min(maximum,
        static_cast<long long>(attack) * strengthPercent / 100);
    long long damage = std::min(maximum, scaled * Between(80, 120) / 100);
    const bool critical = Between(1, 100) <= 10;
    if (critical)
    {
        damage = std::min(maximum, damage * 150 / 100);
    }
    return { static_cast<int>(std::max(1LL, damage)), critical };
}
