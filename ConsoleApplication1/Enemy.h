#pragma once

#include "Character.h"

// 前方宣言。「EnemyData という型がある」と伝える。
// 参照を引数にするだけなら、ここでは中身まで読む必要がない。
struct EnemyData;

class Enemy : public Character
{
public:
    // 最初は待機用の値。プールから取り出すときに実際の敵の値にする。
    Enemy();

    // 既にある Enemy にデータを設定し直すことで、同じ物を再利用する。
    void Reset(const EnemyData& data);
};
