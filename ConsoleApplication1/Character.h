#pragma once
#include <string>
// 勇者と敵に共通する名前・HP・攻撃力を持つクラス。
// TODO: まず Character クラスとコンストラクタをここに宣言する。
// TODO: ダメージ処理と生存判定を追加する。
class Character
{
public:
    std::string name;
    int hp;

    Character(std::string initialName, int initialHp)
        : name(initialName), hp(initialHp)
    {
    }
    void TakeDamage(int damage)
    {
        hp -= damage;

        if (hp < 0)
        {
            hp = 0;
        }
    }
    bool IsAlive() const
    {
        return hp > 0;
	}
};