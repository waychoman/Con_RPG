#include "Character.h"

#include <algorithm>

// Character:: は「Character クラスの関数」という意味。
// : の後はメンバ初期化リスト。メンバは .h で宣言した順に初期化される。
// std::max(a, b) は大きい方を返す。最大 HP は 1 以上、攻撃力は 0 以上にする。
Character::Character(std::string name, int maxHp, int attack)
    : name_(name), hp_(std::max(1, maxHp)), maxHp_(hp_), attack_(std::max(0, attack))
{
}

const std::string& Character::GetName() const
{
    // const 参照で返すので、文字列をコピーせずに読める。
    return name_;
}

int Character::GetHp() const
{
    return hp_;
}

int Character::GetMaxHp() const
{
    return maxHp_;
}

int Character::GetAttack() const
{
    return attack_;
}

bool Character::IsAlive() const
{
    return hp_ > 0;
}

int Character::TakeDamage(int damage)
{
    if (damage <= 0)
    {
        return 0;
    }

    // std::min(a, b) は小さい方を返す。残り HP より多くは減らさない。
    const int actualDamage = std::min(hp_, damage);
    hp_ -= actualDamage;
    return actualDamage;
}

int Character::Heal(int amount)
{
    // このゲームの回復薬では、HP が 0 のキャラクターを復活させない。
    if (amount <= 0 || !IsAlive())
    {
        return 0;
    }

    const int actualRecovery = std::min(maxHp_ - hp_, amount);
    hp_ += actualRecovery;
    return actualRecovery;
}

void Character::ResetStats(std::string name, int maxHp, int attack)
{
    name_ = name;
    maxHp_ = std::max(1, maxHp);
    hp_ = maxHp_;
    attack_ = std::max(0, attack);
}
