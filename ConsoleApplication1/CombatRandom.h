#pragma once

#include <random>

// 乱数の結果と、その攻撃が会心だったかを一組で返す。
struct AttackRoll
{
    int damage;
    bool critical;
};

// ランダムな値を決める部分を、HPを減らす Character の処理から分離する。
class CombatRandom
{
public:
    explicit CombatRandom(unsigned int seed);
    int Between(int minimum, int maximum);
    AttackRoll RollAttack(int attack, int strengthPercent = 100);

private:
    std::mt19937 engine_;
};
