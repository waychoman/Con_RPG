#include "Game.h"

#include <array>
#include <iostream>
#include <string>

namespace
{
    // 出現順。敵のステータスは EnemyDataTable.cpp にある。
    const std::array<EnemyId, 3> EncounterOrder =
    {
        EnemyId::Slime, EnemyId::Goblin, EnemyId::Dragon
    };

    // 1行全体を読み、前後の空白を除いて1文字の数字だけを受け付ける。
    // EOF（入力が終わった状態）は -1。入力失敗で無限ループさせない。
    int ReadChoice(std::istream& input, std::ostream& output,
        const std::string& allowedChoices)
    {
        std::string line;
        while (true)
        {
            output << "選択 > " << std::flush;
            if (!std::getline(input, line))
            {
                return -1;
            }

            const std::size_t first = line.find_first_not_of(" \t\r");
            if (first != std::string::npos)
            {
                const std::size_t last = line.find_last_not_of(" \t\r");
                const std::string choice = line.substr(first, last - first + 1);
                if (choice.size() == 1 &&
                    allowedChoices.find(choice[0]) != std::string::npos)
                {
                    return choice[0] - '0';
                }
            }

            output << "表示された数字を1つ入力してください。\n";
        }
    }
}

Game::Game()
    : enemyFactory_(enemyPool_), currentEnemy_(nullptr),
      state_(GameState::Title), result_(GameResult::None), defeatedCount_(0)
{
}

void Game::Run(std::istream& input, std::ostream& output)
{
    // 有限状態機械：各画面の処理が次の state_ を決める。
    while (state_ != GameState::Exit)
    {
        switch (state_)
        {
        case GameState::Title:
            HandleTitle(input, output);
            break;
        case GameState::Battle:
            HandleBattle(input, output);
            break;
        case GameState::Result:
            HandleResult(input, output);
            break;
        case GameState::Exit:
            break;
        }
    }

    ReleaseEnemy();
    output << "\nゲームを終了しました。\n";
}

void Game::HandleTitle(std::istream& input, std::ostream& output)
{
    output << "\n=== 小さなコンソールRPG ===\n"
        << "スライム、ゴブリン、ドラゴンを倒せばクリア！\n"
        << "勇者: HP100 / 攻撃力12 / 回復薬3個（HP35回復）\n"
        << "1: はじめる\n0: 終了\n";

    const int choice = ReadChoice(input, output, "10");
    if (choice == 1)
    {
        StartNewGame(output);
    }
    else
    {
        ExitGame();
    }
}

void Game::StartNewGame(std::ostream& output)
{
    // 再挑戦でも、前回借りていた敵と勇者の状態を引き継がない。
    ReleaseEnemy();
    player_.Reset();
    defeatedCount_ = 0;
    result_ = GameResult::None;
    if (SpawnEnemy(output))
    {
        state_ = GameState::Battle;
    }
}

bool Game::SpawnEnemy(std::ostream& output)
{
    if (defeatedCount_ >= EncounterOrder.size())
    {
        ExitGame();
        return false;
    }

    currentEnemy_ = enemyFactory_.Create(EncounterOrder[defeatedCount_]);
    if (currentEnemy_ == nullptr)
    {
        output << "敵を準備できませんでした。\n";
        ExitGame();
        return false;
    }

    output << "\n" << currentEnemy_->GetName() << "が現れた！\n";
    return true;
}

void Game::HandleBattle(std::istream& input, std::ostream& output)
{
    if (currentEnemy_ == nullptr)
    {
        ExitGame();
        return;
    }

    output << "\n--- 戦闘（討伐 " << defeatedCount_ << "/"
        << EncounterOrder.size() << "）---\n"
        << player_.GetName() << " HP: " << player_.GetHp() << "/"
        << player_.GetMaxHp() << "  攻撃力: " << player_.GetAttack()
        << "  回復薬: " << player_.GetPotionCount() << "個\n"
        << currentEnemy_->GetName() << " HP: " << currentEnemy_->GetHp()
        << "/" << currentEnemy_->GetMaxHp()
        << "  攻撃力: " << currentEnemy_->GetAttack() << "\n"
        << "1: 攻撃\n2: 回復薬\n0: 冒険をやめる\n";

    const int choice = ReadChoice(input, output, "120");
    if (choice == -1)
    {
        ExitGame();
        return;
    }
    if (choice == 0)
    {
        result_ = GameResult::Retired;
        ReleaseEnemy();
        state_ = GameState::Result;
        return;
    }

    if (choice == 1)
    {
        const int damage = currentEnemy_->TakeDamage(player_.GetAttack());
        output << player_.GetName() << "の攻撃！ "
            << currentEnemy_->GetName() << "に" << damage << "ダメージ。\n";
    }
    else if (choice == 2)
    {
        const int recovered = player_.UsePotion();
        if (recovered == 0)
        {
            output << (player_.GetPotionCount() == 0
                ? "回復薬がありません。\n" : "HPは満タンです。\n");
            // 使えなかった回復薬や無効な入力では敵のターンに進まない。
            return;
        }
        output << "回復薬を使用！ HPが" << recovered << "回復した。\n";
    }

    if (!currentEnemy_->IsAlive())
    {
        output << currentEnemy_->GetName() << "を倒した！\n";
        // 倒れた敵を返却してから、次の敵を同じプールで用意する。
        ReleaseEnemy();
        ++defeatedCount_;
        if (defeatedCount_ == EncounterOrder.size())
        {
            result_ = GameResult::Clear;
            state_ = GameState::Result;
        }
        else
        {
            SpawnEnemy(output);
        }
        return;
    }

    const int damage = player_.TakeDamage(currentEnemy_->GetAttack());
    output << currentEnemy_->GetName() << "の反撃！ "
        << player_.GetName() << "に" << damage << "ダメージ。\n";
    if (!player_.IsAlive())
    {
        result_ = GameResult::GameOver;
        ReleaseEnemy();
        state_ = GameState::Result;
    }
}

void Game::HandleResult(std::istream& input, std::ostream& output)
{
    output << "\n=== 結果 ===\n";
    switch (result_)
    {
    case GameResult::Clear:
        output << "ゲームクリア！ ドラゴンを倒して平和を取り戻した。\n";
        break;
    case GameResult::GameOver:
        output << "ゲームオーバー。勇者は倒れてしまった。\n";
        break;
    case GameResult::Retired:
        output << "冒険を中断しました。\n";
        break;
    case GameResult::None:
        break;
    }
    output << "討伐数: " << defeatedCount_ << "/" << EncounterOrder.size()
        << "  残りHP: " << player_.GetHp() << "/" << player_.GetMaxHp()
        << "\n1: もう一度遊ぶ\n0: 終了\n";

    if (ReadChoice(input, output, "10") == 1)
    {
        StartNewGame(output);
    }
    else
    {
        ExitGame();
    }
}

void Game::ReleaseEnemy()
{
    if (currentEnemy_ != nullptr)
    {
        enemyPool_.Release(currentEnemy_);
        currentEnemy_ = nullptr;
    }
}

void Game::ExitGame()
{
    ReleaseEnemy();
    state_ = GameState::Exit;
}
