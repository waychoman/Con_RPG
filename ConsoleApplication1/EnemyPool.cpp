#include "EnemyPool.h"

Enemy* EnemyPool::Acquire()
{
    // size_t は配列の要素数・添字などに使う、負の値を持たない整数型。
    for (std::size_t index = 0; index < enemies_.size(); ++index)
    {
        if (!inUse_[index])
        {
            inUse_[index] = true;
            return &enemies_[index];
        }
    }

    return nullptr;
}

bool EnemyPool::Release(Enemy* enemy)
{
    for (std::size_t index = 0; index < enemies_.size(); ++index)
    {
        // アドレスが一致するかだけを見るので、未知のポインタを読み取らない。
        // 使用中も確認し、二重返却を受け付けない。
        if (enemy == &enemies_[index] && inUse_[index])
        {
            inUse_[index] = false;
            return true;
        }
    }

    return false;
}

std::size_t EnemyPool::GetInUseCount() const
{
    std::size_t count = 0;
    for (bool inUse : inUse_)
    {
        if (inUse)
        {
            ++count;
        }
    }

    return count;
}