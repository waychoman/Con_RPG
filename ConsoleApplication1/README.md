# コンソールRPGの作業ガイド

自分で実装を進めるためのひな形です。ゲームの処理はまだ実装していません。
ヘッダー（.h）にクラスや関数を宣言し、ソース（.cpp）に関数の中身を定義します。

| ファイル | 役割・課題の技術 |
| --- | --- |
| RPG.cpp | main 関数。最初はキャラクターの生成と表示を試し、最後に Game を起動する |
| Character.h / Character.cpp | 共通の名前・HP・攻撃力、コンストラクタ、ダメージ処理 |
| Player.h / Player.cpp | 勇者と回復アイテム |
| Enemy.h / Enemy.cpp | 敵と再利用時のリセット処理 |
| EnemyDataTable.h / EnemyDataTable.cpp | 敵データの一覧（データテーブル）と共有管理（シングルトン） |
| EnemyFactory.h / EnemyFactory.cpp | 敵のIDから敵を用意する（ファクトリーパターン） |
| EnemyPool.h / EnemyPool.cpp | 敵の取得・返却・再利用（オブジェクトプール） |
| GameState.h | ゲームの状態を列挙する |
| Game.h / Game.cpp | ゲームループ、戦闘、状態の切り替え（有限状態機械） |

## 実装する順番

1. Character のコンストラクタを作り、RPG.cpp で名前とHPを表示する。
2. ダメージ処理と生存判定を作り、Player と Enemy を用意する。
3. 勇者と敵が交互に攻撃する戦闘を作る。
4. EnemyDataTable に敵の種類を登録し、シングルトンとして取得する。
5. EnemyFactory からIDを指定して敵を用意する。
6. GameState と Game でタイトル・戦闘・結果・終了を切り替える。
7. EnemyPool を追加し、ファクトリーを通して敵を取得・返却する。

最初の目標は「敵を3体倒したらクリア、勇者のHPが0ならゲームオーバー」です。
追加の仕様は、基本の戦闘が動いてから決めましょう。

## 実行

Visual Studio で既存の ConsoleApplication1 プロジェクトを開き、Ctrl + F5 で実行します。
プロジェクト名はそのまま、main 関数のあるファイル名を RPG.cpp に変更しています。
追加した各ファイルは、ソリューション エクスプローラーにも登録しています。
