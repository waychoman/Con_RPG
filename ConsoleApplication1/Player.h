#pragma once

#include "Character.h"

// public Character は継承。名前・HP・攻撃力の処理を Character から引き継ぐ。
// Player では「勇者だけの機能」である回復薬の管理を追加する。
class Player : public Character
{
public:
    Player();

    int GetPotionCount() const;
    int UsePotion();
    void Reset();

private:
    int potionCount_;
};
