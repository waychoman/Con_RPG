#include "Game.h"
#include <iostream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif

int main()
{
#ifdef _WIN32
    // プロジェクトの /utf-8 設定と合わせ、日本語をUTF-8で表示する。
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // main は入り口。ゲームの具体的な処理は Game に任せる。
    Game game;
    game.Run(std::cin, std::cout);
    return 0;
}
