#pragma once

#include <array>
#include <string>

// 数字の代わりに名前で敵の種類を指定する。
// enum class なので EnemyId::Slime のように書く。
enum class EnemyId
{
    Slime,
    Goblin,
    Dragon
};

// 敵1種類の設定。実際の戦闘中に減るHPとは別の、初期値のまとまり。
struct EnemyData
{
    EnemyId id;
    std::string name;
    int maxHp;
    int attack;
    int heavyAttackChance; // 大技の確率（%）。大技の直後は0%になる。
    int guardChance;       // 防御の確率（%）。残りが通常攻撃になる。
};

// データテーブル: 敵の設定を1か所に集め、IDから取り出せるようにする。
// シングルトン: このクラスの実体を1つだけ用意し、全員で共有する。
class EnemyDataTable
{
public:
    // static なので、オブジェクトを先に作らずクラス名から呼び出せる。
    // const の参照を返し、共有している設定を呼び出し側が変更するのを防ぐ。
    static const EnemyDataTable& Instance();

    // データを探すだけなので末尾の const を付ける。
    // 見つかれば共有データへのポインタ、見つからなければ nullptr を返す。
    const EnemyData* Find(EnemyId id) const;

    // = delete は「この操作は使えない」という宣言。共有の実体をコピーさせない。
    EnemyDataTable(const EnemyDataTable&) = delete;
    EnemyDataTable& operator=(const EnemyDataTable&) = delete;

private:
    // 外から自由に作らせず、Instance() の中だけで作る。
    EnemyDataTable();

    // std::array は要素数が決まった配列。ここでは敵3種類を保持する。
    std::array<EnemyData, 3> data_;
};
