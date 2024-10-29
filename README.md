# 暴走鉄球  

プランナー２名 / プログラマー ２名 / CGデザイナー 5名 / キャラクターデザイナー 3名 / 声優 2名　　

計14名で制作したアクションゲームです。　　

制作期間 2023/11/24～2024/03/22　　

**概要説明**  
鉄球振り回しド派手アクション！！しゃべる鉄球と共にドンドン建物や敵をぶっ壊せ！！　　

# 開発環境
[DXライブラリ](https://dxlib.xsrv.jp/)  
[nlomann-json](https://github.com/nlohmann/json)  

# プロジェクト構成  
```
.
|── AppFrame(ゲームを作成する際に必要となる機能や便利な機能をまとめたライブラリ)
|   |
|   └── source(ソースファイル)
|       |
|       |── Application(実行に必要となる機能や、汎用性が高い機能をまとめたファイルを格納)
|       |   |
|       |   |── ApplicationBase(DxLibの初期化や全体のループを管理するクラス)
|       |   |
|       |   |── Global(グローバル変数で持っておきたいものを作成するクラス)
|       |   |
|       |   |── UtilMacro(よく使う機能をマクロとしてまとめたもの)
|       |   |
|       |   └── WinMain(ゲーム全体の処理を行うもの)
|       |
|       |── CFile(ファイルの入出力機能をまとめたもの)
|       |   |
|       |   └── CFile(ファイルの入出力機能をまとめたファイル)
|       |
|       |── Mode(ゲームを作成する際に必要なフレームワークをまとめたもの)
|       |   |
|       |   |── ModeBase(ゲームのシーンを作成する際の基底クラス)
|       |   |
|       |   └── ModeServer(Mode関連のクラスを管理するクラス)
|       |
|       |── System(様々な機能を種類別に分けたもの)
|       |   |
|       |   └── Source ・ Header(Systemとして使用するクラスのソースとヘッダーファイルをまとめたもの　フォルダは分かれているが中身としては同じ)
|       |       |
|       |       |── Collision(2Dや3Dの当たり判定に関する関数をまとめたフォルダ)
|       |       |   |
|       |       |   |── 2DCollision(2Dで使用する当たり判定関数をまとめたファイル)
|       |       |   |
|       |       |   |── 3DCollision(3Dで使用する当たり判定関数をまとめたファイル)
|       |       |   |
|       |       |   └── MyStructure(当たり判定用の構造体やクラスをまとめたファイル)
|       |       |
|       |       |── Effect(汎用性の高いエフェクトをまとめたフォルダ)
|       |       |   |
|       |       |   └── Effect(汎用性の高いエフェクトをまとめたクラス)
|       |       |
|       |       |── Function(計算系クラスや描画系クラスをまとめたフォルダ)
|       |       |   |
|       |       |   |── 3DDraw(3Dの当たり判定を描画する関数をまとめたファイル)
|       |       |   |
|       |       |   |── DrawGauge(指定した場所から時計回りに消えていく円形のゲージを描画するためのクラス)
|       |       |   |
|       |       |   |── Easing(イージング関連の機能をまとめたファイル)
|       |       |   |
|       |       |   |── Fps(FPSを60フレームで安定させるためのクラス)
|       |       |   |
|       |       |   |── mydraw(2Dで使用する描画関連の関数をまとめたクラス)
|       |       |   |
|       |       |   |── mymath(汎用性の高い計算系関数をまとめたクラス)
|       |       |   |
|       |       |   |── Timer(ゲームループの1周をマイクロ秒単位で取得するためのクラス)
|       |       |   |
|       |       |   |── Vector3D(自作のベクトルクラス)
|       |       |   |
|       |       |   └── Vibration(コントローラーの振動を管理するためのクラス)
|       |       |
|       |       |── Input(入力関連のクラスをまとめたフォルダ)
|       |       |   |
|       |       |   └── XInput(XInputのコントローラーを使用するために必要なクラス)
|       |       |
|       |       |── Resource(画像やモデルなどを管理するクラス)
|       |       |   |
|       |       |   └── ResourceServer(画像やモデルなどを管理するクラス)
|       |       |
|       |       └── Sound(SEやBGM、ボイスなどを一括で管理するクラス)
|       |           |
|       |           |── SoundItem(SEやBGMなどのサウンドアイテムのクラス集)
|       |           |
|       |           └── SoundServer(サウンドアイテムを再生や削除など管理するクラス)
|       |
|       └── appframe.h(AppFrame内のファイルのヘッダーを一つにまとめたもの)
|
|── DxLib(ライブラリ用ディレクトリ)
|
|── Game(ゲーム本体の Project)
|   |
|   |── Game
|   |   |
|   |   |── include
|   |   |   |
|   |   |   └── nlohmann(nlohmann-jsonのインクルードファイル)
|   |   |
|   |   └── source(ソースファイル)
|   |       |
|   |       |── Afterglow(鉄球の目に残光を発生させるクラス)
|   |       |
|   |       |── AfterImage(半透明にしたモデルを複数体表示することで残像を表現するクラス)
|   |       |
|   |       |── AnimationChain(リザルト画面で鎖のアニメーションを表示するクラス)
|   |       |
|   |       |── AnimationItem(キャラクターのアニメーション情報を保持するクラス)
|   |       |
|   |       |── AnimationManager(キャラクターのアニメーションを管理するクラス)
|   |       |
|   |       |── ApplicationMain(ApplicationBaseを継承し、ModeServerなどの処理を行うクラス)
|   |       |
|   |       |── BoardArrow(指定の方向に矢印を描画するクラス)
|   |       |
|   |       |── BoardParabola(指定の方向に放物線の矢印を描画するクラス)
|   |       |
|   |       |── BoardPolygon(板ポリを生成するためのクラス)
|   |       |
|   |       |── BoardPolygonDust(砂煙の板ポリを生成するのクラス)
|   |       |
|   |       |── bone(キャラクターの髪の毛やリボンなどをboneを物理演算で動かすためのクラス)
|   |       |
|   |       |── Boss(ボスの制御を行うクラス)
|   |       |
|   |       |── BossIronBall(ボス鉄球の制御を行うクラス)
|   |       |
|   |       |── BreakableBuilding(破壊可能な建物のクラス)
|   |       |
|   |       |── BreakModelParts(建物モデルをパーツごとに吹き飛ばす処理を行うクラス)
|   |       |
|   |       |── BuildingBase(建物クラスの基底クラス)
|   |       |
|   |       |── Camera(プレイヤーを追従する基本的なカメラクラス)
|   |       |
|   |       |── CameraBase(それぞれのカメラクラスで共通で使用する変数がある基底クラス)
|   |       |
|   |       |── Cell(当たり判定処理の管理を行うCollisionManagerクラスに情報を登録するためのクラス)
|   |       |
|   |       |── CharacterBase(ObjectBaseから派生したキャラクターのもととなる基底クラス)
|   |       |
|   |       |── ClassificationEffect(FrameDataからのコマンドを受け取り、それに応じたエフェクトやSEを再生するクラス)
|   |       |
|   |       |── CollisionManager(当たり判定処理の管理を行うクラス)
|   |       |
|   |       |── CrystarPattern1(一番弱いクライスターのパターン1(ガラス)のクラス 突進攻撃をします)
|   |       |
|   |       |── CrystarPattern2(クライスターのパターン2(岩)のクラス 頭突き攻撃をします。)
|   |       |
|   |       |── CrystarPattern3(一番強いクライスターのパターン3(鉄)のクラス 回転しながら追尾攻撃をします。)
|   |       |
|   |       |── CrystarRoof(クライスターの屋根部分を描画するためのクラス)
|   |       |
|   |       |── EffectBase(エフェクトを再生するための基底クラス)
|   |       |
|   |       |── EffectManager(全てのエフェクトを管理するためのクラス)
|   |       |
|   |       |── EffekseerBase(エフェクシアを再生するための基本的なクラス)
|   |       |
|   |       |── EffekseerPosSynchro(位置情報のあるアドレスを受け取り移動するエフェクシアを再生するクラス)
|   |       |
|   |       |── EffekseerRotation(回転攻撃のエフェクトを再生するクラス)
|   |       |
|   |       |── EnemyBase(エネミーの基本行動や共通の変数が書かれた基底クラス)
|   |       |
|   |       |── EnemyManager(エネミーの生成、管理を行うクラス)
|   |       |
|   |       |── EnemyStract(エネミーで使用する構造体を定義するヘッダーです。)
|   |       |
|   |       |── Floor(ゲームステージある床(タイル)を管理するクラス)
|   |       |
|   |       |── Fog(プレイヤーが決められた範囲外に出るとフォグが発生するクラス)
|   |       |
|   |       |── FrameData(ファイルから読み込んだキャラクターのモーションに合わせて行う処理)
|   |       |
|   |       |── Gate(プレイヤーが触れればクリアになるオブジェクトクラス)
|   |       |
|   |       |── Heart(プレイヤーが触れたらＨＰを回復するオブジェクトクラス)
|   |       |
|   |       |── IronBall( プレイヤーが持つ鉄球・鎖の制御・描画を行うクラス)
|   |       |
|   |       |── Light(ファイルからデータを読み込みライトを設定するクラス)
|   |       |
|   |       |── LoadingChain(loading時の鎖を制御・描画を行うクラス)
|   |       |
|   |       |── LoadingPlayer(loading時のキャラクターのを制御・描画を行うクラス)
|   |       |
|   |       |── ModeBossBattle(ボスバトルモードの制御を行うクラス)
|   |       |
|   |       |── ModeClear(クリア時にリザルトと次のステージへの遷移を管理するクラス)
|   |       |
|   |       |── ModeFade(ステージ遷移時にフェードイン、フェードアウトを行うためのクラス)
|   |       |
|   |       |── ModeFadeComeBack(ステージ遷移時にフェードインしてからフェードアウトを行うためのクラス)
|   |       |
|   |       |── ModeGame(ボスバトル以外のステージを管理するクラス)
|   |       |
|   |       |── ModeGameOver(ゲームオーバー時に遷移し、リトライかタイトルに戻るかを選択するモード)
|   |       |
|   |       |── ModeInstructions(ポーズ画面から遷移できる、操作説明画面を表示するクラス)
|   |       |
|   |       |── ModelColor(モデルの白点滅処理を行うクラス)
|   |       |
|   |       |── ModeLoading(loading時のキャラクターや鉄球などの処理・描画を行うクラス)
|   |       |
|   |       |── ModeMovie(ゲームクリア時にエンディングムービーを再生するクラス)
|   |       |
|   |       |── ModePause(音量設定や振動設定、操作説明画面、ゲーム終了を選択するポーズ画面のクラス)
|   |       |
|   |       |── ModeRotationCamera(ステージ開始時のカメラの回転処理を行うクラス)
|   |       |
|   |       |── ModeScenario(シナリオを再生するクラス)
|   |       |
|   |       |── ModeTitle(タイトル画面の処理を行うクラス)
|   |       |
|   |       |── ModeTutorial(ステージの最初にチュートリアル画面を表示するクラス)
|   |       |
|   |       |── ModeZoomCamera(ゲート表示時にカメラを移動させるクラス)
|   |       |
|   |       |── MotionList(各キャラクターのモーションリストを管理するクラス)
|   |       |
|   |       |── myJson(jsonファイルを読み込むクラス)
|   |       |
|   |       |── ObjectBase(当たり判定処理を行う全てのオブジェクトの基底クラス)
|   |       |
|   |       |── PlaneEffectBase( 板ポリゴンを表示するエフェクトの基底クラス)
|   |       |
|   |       |── Player(プレイヤーキャラクターの制御・描画を行うクラス)
|   |       |
|   |       |── RotationCamera(カメラを回転させる処理をするクラス)
|   |       |
|   |       |── ScreenVibration(画面上でのカメラが見ている映像の中心座標をずらす処理行うクラス)
|   |       |
|   |       |── SlaBlock(スラブロックの攻撃パターン1(岩)のクラス 1度だけプレイヤーの真上から落下攻撃を行います)
|   |       |
|   |       |── SlaBlockPattern2(スラブロックの攻撃パターン2(ガラス,鉄)のクラス 1度だけプレイヤーの真上から落下攻撃を行います)
|   |       |
|   |       |── Suppression(敵や建物を破壊した時に減少していく制圧ゲージを管理するクラス)
|   |       |
|   |       |── TargetZoomCamera(今のカメラの位置を指定の位置に移動する処理を行うクラス)
|   |       |
|   |       |── TimeLimit(制限時間の管理を行うクラス)
|   |       |
|   |       |── Tower(タワーオブジェクトの制御・描画を行うクラス)
|   |       |
|   |       |── TowerParts(タワーパーツの制御を行うクラス)
|   |       |
|   |       |── UIBase(UIの基底クラス)
|   |       |
|   |       |── UIBossHp(ボスのHPを表示するUIクラス)
|   |       |
|   |       |── UIExpPoint(経験値のUIを表示するクラス)
|   |       |
|   |       |── UIHeart(プレイヤーHPのUIを表示するクラス)
|   |       |
|   |       |── UISuppressionGauge(制圧ゲージを表示するクラス)
|   |       |
|   |       |── UITimeLimit(残り時間を表示するUIクラス)
|   |       |
|   |       └── UnbreakableBuilding(破壊不可能な建物のクラス)
|   |
|   └── Game.sln(コードを見る際は、こちらを起動して下さい)
|
|── .gitattributes
|
|── .gitignore
|
└── README.md
```

# 起動方法  
上記のプロジェクトをダウンロードし、解凍後「Game/Game.sln」を起動  

# AppFrameについて  
ゲームを作成する上で定形的に必要となる機能また今までの制作で汎用的に使える機能をまとめたフレームワーク  
Static Library の形式で提供(Project 設定)  

***ApplicationBase***  

DX ライブラリを用いた Windows アプリケーションを作成する基底クラス  
本クラスを継承したクラスを作成すれば最小のコードでアプリのメイン部分を記述出来る  

***ModeBase / ModeServer***  

本フレームワークはモードと呼ぶ単位でプログラミング可能になっており、モードを切り替える機能も提供します。  
これによりアプリ作成者は、モード単位の実装をする事でゲームのフローを構築出来ます。  

ModeBase を継承したクラス単位でタイトルやインゲーム、設定画面など好きな単位で構築できModeServer が各モードを切り替える機能を提供します。  
