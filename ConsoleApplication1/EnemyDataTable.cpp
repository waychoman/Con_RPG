#include "EnemyDataTable.h"

EnemyDataTable::EnemyDataTable()
    : data_{{
        { EnemyId::Slime, "スライム", 30, 5 },
        { EnemyId::Goblin, "ゴブリン", 45, 9 },
        { EnemyId::Dragon, "ドラゴン", 70, 14 }
    }}
{
}

const EnemyDataTable& EnemyDataTable::Instance()
{
    // 関数内の static は最初の呼び出しで1度だけ作られ、以後同じ実体が残る。
    // 通常のローカル変数と違い、関数を抜けても消えない。
    static const EnemyDataTable instance;
    return instance;
}

const EnemyData* EnemyDataTable::Find(EnemyId id) const
{
    // const auto& は各データをコピーせず、読み取り専用の参照で見る書き方。
    for (const auto& data : data_)
    {
        if (data.id == id)
        {
            // &data はデータのアドレス。データの所有者はテーブルのまま。
            return &data;
        }
    }

    // nullptr は「何も指していないポインタ」。不明なIDは配列の外を読まない。
    return nullptr;
}