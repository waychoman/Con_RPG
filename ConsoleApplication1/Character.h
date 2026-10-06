#pragma once

#include <string>

// .h は「このクラスが持つ機能」の宣言、.cpp は処理の中身を書く場所。
// 勇者と敵に共通する名前・HP・攻撃力をまとめる。
class Character
{
public:
    // 作るときに名前・最大 HP・攻撃力を渡す。HP は最大 HP から始まる。
    Character(std::string name, int maxHp, int attack);

    // const は「この関数はキャラクターの状態を変更しない」という約束。
    // private の値を読むための関数を getter（ゲッター）と呼ぶ。
    const std::string& GetName() const;
    int GetHp() const;
    int GetMaxHp() const;
    int GetAttack() const;
    bool IsAlive() const;

    // 実際に減った HP / 回復した HP を返す。
    int TakeDamage(int damage);
    int Heal(int amount);

protected:
    // protected はこのクラスと、継承した Player / Enemy から使える。
    // プールで敵を再利用するときなどに、能力と HP を設定し直す。
    void ResetStats(std::string name, int maxHp, int attack);

private:
    // 外から hp_ を直接変更させず、TakeDamage / Heal で範囲を守る。
    std::string name_;
    int hp_;
    int maxHp_;
    int attack_;
};
