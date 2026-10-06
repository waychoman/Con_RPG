#include "Player.h"

// Player を作るとき、先に親クラス Character のコンストラクタを呼ぶ。
Player::Player()
    : Character("勇者", 100, 12), potionCount_(3)
{
}

int Player::GetPotionCount() const
{
    return potionCount_;
}

int Player::UsePotion()
{
    if (potionCount_ <= 0)
    {
        return 0;
    }

    // 継承した Heal を使う。HP 上限や生存判定は Character に任せる。
    const int recoveredHp = Heal(35);
    if (recoveredHp > 0)
    {
        --potionCount_;
    }

    // HP が満タン、または倒れている場合は 0 となり、薬も消費しない。
    return recoveredHp;
}

void Player::Reset()
{
    ResetStats("勇者", 100, 12);
    potionCount_ = 3;
}
