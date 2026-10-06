#include "Game.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <random>
#include <string>

namespace
{
    const std::array<EnemyId, 3> EncounterOrder =
    {
        EnemyId::Slime, EnemyId::Goblin, EnemyId::Dragon
    };

    // 倍率を整数の百分率で表す。25なら25%、200なら2倍。
    int Scale(int value, int percent)
    {
        const long long scaled = static_cast<long long>(value) * percent / 100;
        return static_cast<int>(std::min(scaled,
            static_cast<long long>((std::numeric_limits<int>::max)())));
    }

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

// 通常の起動では実行ごとに乱数の開始値を変える。
Game::Game()
    : Game(std::random_device{}())
{
}

Game::Game(unsigned int seed)
    : enemyFactory_(enemyPool_), random_(seed), currentEnemy_(nullptr),
      state_(GameState::Title), result_(GameResult::None),
      enemyIntent_(EnemyIntent::Attack), defeatedCount_(0)
{
}

void Game::Run(std::istream& input, std::ostream& output)
{
    while (state_ != GameState::Exit)
    {
        switch (state_)
        {
        case GameState::Title: HandleTitle(input, output); break;
        case GameState::Battle: HandleBattle(input, output); break;
        case GameState::Result: HandleResult(input, output); break;
        case GameState::Exit: break;
        }
    }
    ReleaseEnemy();
    output << "\nゲームを終了しました。\n";
}

void Game::HandleTitle(std::istream& input, std::ostream& output)
{
    output << "\n=== 小さなコンソールRPG ===\n"
        << "スライム、ゴブリン、ドラゴンを倒せばクリア！\n"
        << "勇者: HP" << player_.GetMaxHp() << " / 攻撃力"
        << player_.GetAttack() << " / 回復薬" << player_.GetPotionCount()
        << "個（HP" << player_.GetPotionHealingAmount() << "回復）\n"
        << "攻撃は80～120%に変動し、10%で会心（1.5倍）。敵HPも毎回変化。\n"
        << "敵の予告を見よう！ 大技は防御し、息切れ中に攻撃や回復。\n"
        << "防御は被害を75%軽減。通常攻撃と防御で気力が1回復。\n"
        << "強攻撃は気力2で威力1.75倍、敵の防御を無視。\n"
        << "1: はじめる\n0: 終了\n";
    if (ReadChoice(input, output, "10") == 1) { StartNewGame(output); }
    else { ExitGame(); }
}

void Game::StartNewGame(std::ostream& output)
{
    ReleaseEnemy();
    player_.Reset();
    defeatedCount_ = 0;
    result_ = GameResult::None;
    // 乱数はリセットしないので、再挑戦も前回の戦闘の繰り返しにならない。
    if (SpawnEnemy(output)) { state_ = GameState::Battle; }
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

    // テーブルの基準HPを90～110%に変える。設定表自体は変更しない。
    const int battleHp = std::max(1,
        Scale(currentEnemy_->GetMaxHp(), random_.Between(90, 110)));
    currentEnemy_->SetBattleHp(battleHp);
    enemyIntent_ = EnemyIntent::Attack;
    PrepareEnemyIntent();
    output << "\n" << currentEnemy_->GetName() << "が現れた！\n";
    return true;
}

void Game::PrepareEnemyIntent()
{
    // 大技の後は必ず1ターン息切れ。これが読み合いのチャンスになる。
    if (enemyIntent_ == EnemyIntent::HeavyAttack)
    {
        enemyIntent_ = EnemyIntent::Recover;
        return;
    }
    const int roll = random_.Between(1, 100);
    const int heavyChance = currentEnemy_->GetHeavyAttackChance();
    if (roll <= heavyChance) { enemyIntent_ = EnemyIntent::HeavyAttack; }
    else if (roll <= heavyChance + currentEnemy_->GetGuardChance())
    {
        enemyIntent_ = EnemyIntent::Guard;
    }
    else { enemyIntent_ = EnemyIntent::Attack; }
}

void Game::ShowEnemyIntent(std::ostream& output) const
{
    output << "敵の予告: ";
    switch (enemyIntent_)
    {
    case EnemyIntent::Attack:
        output << "通常攻撃（反撃あり）\n";
        break;
    case EnemyIntent::HeavyAttack:
        output << "大技（威力2倍！ 防御が有効）\n";
        break;
    case EnemyIntent::Guard:
        output << "防御（通常攻撃の被害75%軽減・反撃なし）\n";
        break;
    case EnemyIntent::Recover:
        output << "息切れ（反撃なし・こちらの攻撃が25%強化）\n";
        break;
    }
}

void Game::AttackEnemy(int strengthPercent, bool ignoreGuard, std::ostream& output)
{
    if (enemyIntent_ == EnemyIntent::Recover)
    {
        strengthPercent = Scale(strengthPercent, 125);
    }
    const AttackRoll roll = random_.RollAttack(player_.GetAttack(), strengthPercent);
    int damage = roll.damage;
    if (enemyIntent_ == EnemyIntent::Guard && !ignoreGuard)
    {
        damage = std::max(1, Scale(damage, 25));
        output << "敵の防御に阻まれた！\n";
    }
    if (enemyIntent_ == EnemyIntent::Guard && ignoreGuard)
    {
        output << "強攻撃が敵の防御を突破！\n";
    }
    if (roll.critical) { output << "会心の一撃！ "; }
    output << player_.GetName() << "の" << (ignoreGuard ? "強攻撃" : "攻撃")
        << "！ " << currentEnemy_->GetName() << "に"
        << currentEnemy_->TakeDamage(damage) << "ダメージ。\n";
}

void Game::ResolveEnemyTurn(bool defending, std::ostream& output)
{
    if (enemyIntent_ == EnemyIntent::Guard)
    {
        output << "敵は守りを固めている。反撃はない。\n";
        return;
    }
    if (enemyIntent_ == EnemyIntent::Recover)
    {
        output << "敵は息切れしている。反撃はない。\n";
        return;
    }

    const bool heavy = enemyIntent_ == EnemyIntent::HeavyAttack;
    const AttackRoll roll = random_.RollAttack(currentEnemy_->GetAttack(), heavy ? 200 : 100);
    int damage = roll.damage;
    if (defending)
    {
        damage = std::max(1, Scale(damage, 25));
        output << "防御でダメージを75%軽減！\n";
    }
    if (roll.critical) { output << "敵の会心！ "; }
    output << currentEnemy_->GetName() << "の" << (heavy ? "大技" : "反撃")
        << "！ " << player_.GetName() << "に" << player_.TakeDamage(damage)
        << "ダメージ。\n";
}

void Game::HandleBattle(std::istream& input, std::ostream& output)
{
    if (currentEnemy_ == nullptr) { ExitGame(); return; }
    output << "\n--- 戦闘（討伐 " << defeatedCount_ << "/"
        << EncounterOrder.size() << "）---\n"
        << player_.GetName() << " HP: " << player_.GetHp() << "/"
        << player_.GetMaxHp() << "  攻撃力: " << player_.GetAttack()
        << "  回復薬: " << player_.GetPotionCount() << "個"
        << "  気力: " << player_.GetEnergy() << "/" << player_.GetMaxEnergy() << "\n"
        << currentEnemy_->GetName() << " HP: " << currentEnemy_->GetHp()
        << "/" << currentEnemy_->GetMaxHp()
        << "  攻撃力: " << currentEnemy_->GetAttack() << "\n";
    ShowEnemyIntent(output);
    output << "1: 通常攻撃（気力+1）\n2: 回復薬（HP"
        << player_.GetPotionHealingAmount() << "回復・手番消費）\n"
        << "3: 防御（被害75%軽減・気力+1）\n4: 強攻撃（気力"
        << player_.GetPowerAttackCost() << "消費・防御無視）\n0: 冒険をやめる\n";

    const int choice = ReadChoice(input, output, "12340");
    if (choice == -1) { ExitGame(); return; }
    if (choice == 0)
    {
        result_ = GameResult::Retired;
        ReleaseEnemy();
        state_ = GameState::Result;
        return;
    }

    if (choice == 1)
    {
        player_.RestoreEnergy();
        AttackEnemy(100, false, output);
    }
    else if (choice == 2)
    {
        const int recovered = player_.UsePotion();
        if (recovered == 0)
        {
            output << (player_.GetPotionCount() == 0
                ? "回復薬がありません。\n" : "HPは満タンです。\n");
            return;
        }
        output << "回復薬を使用！ HPが" << recovered << "回復した。\n";
    }
    else if (choice == 3)
    {
        player_.RestoreEnergy();
        output << "勇者は防御した。気力が1回復（上限"
            << player_.GetMaxEnergy() << "）。\n";
    }
    else if (choice == 4)
    {
        if (!player_.TryUsePowerAttack())
        {
            output << "気力が足りません。通常攻撃か防御で気力を回復しよう。\n";
            return;
        }
        AttackEnemy(175, true, output);
    }

    if (!currentEnemy_->IsAlive())
    {
        output << currentEnemy_->GetName() << "を倒した！\n";
        ReleaseEnemy();
        ++defeatedCount_;
        if (defeatedCount_ == EncounterOrder.size())
        {
            result_ = GameResult::Clear;
            state_ = GameState::Result;
        }
        else { SpawnEnemy(output); }
        return;
    }

    ResolveEnemyTurn(choice == 3, output);
    if (!player_.IsAlive())
    {
        result_ = GameResult::GameOver;
        ReleaseEnemy();
        state_ = GameState::Result;
    }
    else
    {
        // 有効な行動が終わった時だけ次の予告を決める。
        // 入力ミスや使えない薬・気力不足では、予告も乱数も進まない。
        PrepareEnemyIntent();
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
    case GameResult::None: break;
    }
    output << "討伐数: " << defeatedCount_ << "/" << EncounterOrder.size()
        << "  残りHP: " << player_.GetHp() << "/" << player_.GetMaxHp()
        << "\n1: もう一度遊ぶ\n0: 終了\n";
    if (ReadChoice(input, output, "10") == 1) { StartNewGame(output); }
    else { ExitGame(); }
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
