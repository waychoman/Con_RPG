#pragma once

#include "EnemyDataTable.h"
#include "EnemyPool.h"

// 生成の手順をまとめる窓口。利用側は敵のIDだけを指定すればよい。
// ここではファクトリーパターンを簡単な形（シンプルファクトリー）で実装する。
// 継承で生成方法を差し替える Factory Method の形にはしていない。
class EnemyFactory
{
public:
    // 参照（&）で既存のプールを受け取り、共有する。プールをコピーしない。
    // explicit は、引数からの意図しない自動変換を禁止する。
    explicit EnemyFactory(EnemyPool& pool);

    // 設定の検索→空き枠の取得→初期値の設定をまとめて行う。
    // 不明なIDやプールが満杯の場合は nullptr を返す。
    Enemy* Create(EnemyId id);

private:
    // 参照先のプールは、このファクトリーより長く生きている必要がある。
    EnemyPool& pool_;
};