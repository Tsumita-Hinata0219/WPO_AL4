#pragma once
#include "MyMath.h"
#include "Model.h"
#include "Player/Player.h"
#include "Enemy/Bullet/EnemyBullet.h"
#include "OBBCollider.h"

class GameScene;

    /// <summary>
    /// 敵のパラメーター（レベルによって上下する値）
    /// </summary>
    /// <param name="hp : 体力"></param>
    /// <param name="power : 攻撃力"></param>
    /// <param name="speed : 速度"></param>
struct Parameter
{
    int hp;
    int power;
    float speed;
    float searchDistance;
    int pollution;
};

class IEnemy {
public:

    IEnemy() {};
    virtual ~IEnemy() {};

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="player : プレイヤーのポインタ"></param>
    /// <param name="position : 初期位置"></param>
    /// <param name="level : レベル(これによってパラメーターが変化する)"></param>
    virtual void Initialize(Player* player, Vector3 position, int level) = 0;
    /// <summary>
    /// 更新処理
    /// </summary>
    virtual void Update() = 0;
    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="view"></param>
    virtual void Draw(Camera* camera) = 0;

    virtual OBBCollider* GetOBBCollider() = 0;

    bool IsAlive() { return this->isAlive_; }

    void SetGameScene(GameScene* scene) { this->gameScene = scene; };

    void SetLevel(int level) { level_ = level; }

    std::list<EnemyBullet*>* GetEnemyBullets() { return &bullet_; }

    virtual void OnCollisionWithPlayer(Player* player) = 0;

    virtual void OnCollisionWithPlayerBullet(IPlayerBullet* bullet) = 0;

protected:
    //player
    Player* player_;
    //worldTransform
    WorldTransform worldTransform_;
    //速度
    Vector3 velocity_;
    //モデル
    Model model_;
    //生存しているかのフラグ
    bool isAlive_;
    bool isFeed_;
    //Enemyの持つ弾を可変長配列で管理
    std::list<EnemyBullet*> bullet_;
    //playerとの距離
    float distanceToPlayer_;
    
    int level_;

    Parameter param{};

    GameScene* gameScene = nullptr;
};