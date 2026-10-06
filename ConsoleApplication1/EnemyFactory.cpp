#include "EnemyFactory.h"

EnemyFactory::EnemyFactory(EnemyPool& pool)
    : pool_(pool)
{
}

Enemy* EnemyFactory::Create(EnemyId id)
{
    // 先に設定を確認し、不明なIDでプールの枠を消費しない。
    const EnemyData* data = EnemyDataTable::Instance().Find(id);
    if (data == nullptr)
    {
        return nullptr;
    }

    Enemy* enemy = pool_.Acquire();
    if (enemy == nullptr)
    {
        return nullptr;
    }

    // -> はポインタが指すオブジェクトのメンバにアクセスする記号。
    // *data はポインタが指すデータそのもの。ResetはHPなどを初期値に戻す。
    // 再利用した敵に前の戦闘のHPが残らないよう、毎回設定し直す。
    enemy->Reset(*data);
    return enemy;
}