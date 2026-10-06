// RPG.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include "Character.h"

int main()
{
    Character player("勇者", 100);
    Character enemy("スライム", 30);

    std::cout << player.name << " HP: " << player.hp << '\n';
    std::cout << enemy.name << " HP: " << enemy.hp << '\n';

    enemy.TakeDamage(10);

    std::cout << enemy.name << " HP: " << enemy.hp << '\n';
}
