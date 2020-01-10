#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

//================ Global Constants =================//
const int DEFAULT_DIRECTION = 0;
const int UP_AND_LEFT = 135;
const int DOWN_AND_LEFT = 225;
const int LEFT = 180;
const double EXPLOSION_SIZE = 1.0;
const int EXPLOSION_DEPTH = 0;

// NachenBlaster Constants
const int NB_MAX_HP = 50;
const int MAX_CABBAGES = 30;
const int MIN_CABBAGES = 5;
const int NB_DEPTH = 0;
const double NB_START_Y = 128;

// Star Constants
const int MAX_STARS = 30;
const int STAR_DEPTH = 3;

// Alien Constants
const double ALIEN_SIZE = 1.5;              const int SMALLGON_COLLISION_DMG = 5;
const int ALIEN_DEPTH = 1;                  const int SMOREGON_COLLISION_DMG = 5;
const int ALIEN_FPLEN = 0;                  const int SNAGGLEGON_COLLISION_DMG = 15;
const double SMALLGON_TSPEED = 2.0;         const int SMALLGON_SCORE_VAL = 250;
const double SMOREGON_TSPEED = 2.0;         const int SMOREGON_SCORE_VAL = 250;
const double SNAGGLEGON_TSPEED = 1.75;      const int SNAGGLEGON_SCORE_VAL = 1000;

// Projectile Constants
const double PROJ_SIZE = 0.5;
const int PROJ_DEPTH = 1;
const int CABBAGE_DMG = 2;
const int TURNIP_DMG = 2;
const int TORPEDO_DMG = 8;

// Goodie Constants
const double GOODIE_SIZE = 0.5;
const int GOODIE_DEPTH = 1;
const double GOODIE_STEP = 0.75;
const int GOODIE_SCORE_VAL = 100;

/*=========================================================================================
 ==============>> BASE: ACTOR CLASS <<=====================================================
 ========================================================================================*/
class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw);  // Constructor
    virtual ~Actor();        // Destructor
    
    void doSomething();
    
    int getIdentifier() const;
    virtual bool isAlien() const;
    virtual bool isProjectile() const;
    virtual bool isGoodie() const;
    bool isAlive() const;
    virtual int getHealthScore() const;
    virtual int getCabbage() const;
    virtual int numTorpedo() const;
    
    bool flownOffScreen(int x) const;
    virtual int getDmg() const;
    virtual void getHP();
    virtual void getTorpedo();
    virtual void dropGoodie(double x, double y);
    
    void setDead();
    StudentWorld* getWorld();
    
private:
    virtual void doDifferent() = 0;
    
    StudentWorld* m_world;
    bool m_alive;
    int m_identifier;
};


/*=========================================================================================
 ==============>> DERIVED: STAR CLASS <<===================================================
 ========================================================================================*/
class Star : public Actor
{
public:
    Star(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw);        // Constructor
    virtual ~Star();        // Destructor
    
private:
    virtual void doDifferent();
};


/*=========================================================================================
 ==============>> DERIVED: NACHENBLASTER CLASS <<==========================================
 ========================================================================================*/
class NachenBlaster : public Actor
{
public:
    NachenBlaster(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw);  // Constructor
    virtual ~NachenBlaster();   // Destructor

    virtual int getHealthScore() const;
    virtual int getCabbage() const;
    virtual int numTorpedo() const;
    
    virtual void getTorpedo();
    virtual void getHP();
    void takeDamage(int dmg);
    void displayCollision(double x, double y, Actor* obj);
    
private:
    virtual void doDifferent();
    int m_hp;
    int m_ep;
    int m_ftorpedoes;
};


/*=========================================================================================
 ==============>> DERIVED: EXPLOSION CLASS <<==============================================
 ========================================================================================*/
class Explosion : public Actor
{
public:
    Explosion(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw);    // Constructor
    virtual ~Explosion();   // Destructor
    
private:
    virtual void doDifferent();
    int m_life;
};


// FIX ALIEN ORIENTATIONS & IMPLEMENT CHECKS FOR NACHENBLASTER TO LEFT!!!!!!!!!!!!!
/*========================================================================================
===============>> DERIVED BASE: ALIEN CLASS <<============================================
========================================================================================*/
class Alien : public Actor
{
public:
    Alien(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw, int hp, int fplen, double tspeed, int score);     // Constructor
    virtual ~Alien();           // Destructor
    
    virtual bool isAlien() const;
    int getFPLength() const;
    double getTravelSpeed() const;
    int newFlightPlan() const;
    int getRandDir() const;
    virtual int getDmg() const;
    virtual int getHealthScore() const;
    
    void takeDamage(int dmg);
    void setDmg(int dmg);
    void setNewFlightPlan(int fp);
    void setRandFPLength();
    void moveInDir(int dir);
    void displayCollision(double x, double y, Actor* proj);
    virtual void doDifferent();
    virtual void checkCollision() = 0;
    
private:
    int m_score;
    int m_hp;
    int m_fplen;
    double m_tspeed;
    int m_dmg;
};


//=====================================================//
/*--------------->> DERIVED: SMALLGON <<---------------*/
//=====================================================//
class Smallgon : public Alien
{
public:
    Smallgon(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw, int hp, int fplen, double tspeed, int score);    // Constructor
    virtual ~Smallgon();    // Destructor
    
private:
    virtual void checkCollision();
};

//=====================================================//
/*--------------->> DERIVED: SMOREGON <<---------------*/
//=====================================================//
class Smoregon : public Alien
{
public:
    Smoregon(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw, int hp, int fplen, double tspeed, int score);    // Constructor
    virtual ~Smoregon();    // Destructor
    
    virtual void dropGoodie(double x, double y);
    
private:
    virtual void checkCollision();
};

//=====================================================//
/*-------------->> DERIVED: SNAGGLEGON <<--------------*/
//=====================================================//
class Snagglegon : public Alien
{
public:
    Snagglegon(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw, int hp, int fplen, double tspeed, int score);    // Constructor
    virtual ~Snagglegon();    // Destructor
    
    virtual void dropGoodie(double x, double y);
    
private:
    virtual void checkCollision();
};

/*=========================================================================================
===============>> DERIVED BASE: PROJECTILES CLASS <<=======================================
=========================================================================================*/
class Projectile : public Actor
{
public:
    Projectile(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw);   // Constructor
    virtual ~Projectile();
    
    virtual bool isProjectile() const;
    void setDmg(int dmg);
    virtual int getDmg() const;
    
private:
    int m_dmg;
};

//=====================================================//
/*--------------->> DERIVED: CABBAGES <<---------------*/
//=====================================================//
class Cabbage : public Projectile
{
public:
    Cabbage(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw);  // Constructor
    virtual ~Cabbage();     // Destructor
    
private:
    virtual void doDifferent();
};

//=====================================================//
/*--------------->> DERIVED: TURNIPS <<----------------*/
//=====================================================//
class Turnip : public Projectile
{
public:
    Turnip(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw);   // Constructor
    virtual ~Turnip();  // Destructor
    
private:
    virtual void doDifferent();
};

//=====================================================//
/*--------->> DERIVED: FLATULENCE TORPEDOES <<---------*/
//=====================================================//
class Flat_Torpedo : public Projectile
{
public:
    Flat_Torpedo(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw); // Constructor
    virtual ~Flat_Torpedo();    // Destructor
    
private:
    virtual void doDifferent();
};

/*=========================================================================================
===============>> DERIVED BASE: GOODIES CLASS <<===========================================
=========================================================================================*/
class Goodies : public Actor
{
public:
    Goodies(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw);  // Constructor
    virtual ~Goodies();     // Destructor
    
    virtual bool isGoodie() const;
    
protected:
    virtual void doDifferent();
};

//=====================================================//
/*---------->> DERIVED: EXTRA LIFE GOODIE <<-----------*/
//=====================================================//
class ExtraLifeGoodie : public Goodies
{
public:
    ExtraLifeGoodie(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw);   // Constructor
    virtual ~ExtraLifeGoodie();  // Destructor
    
private:
};


//=====================================================//
/*----------->> DERIVED: REPAIR GOODIE <<--------------*/
//=====================================================//
class RepairGoodie : public Goodies
{
public:
    RepairGoodie(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw);     // Constructor
    virtual ~RepairGoodie();    // Destructor
    
private:
};


//=====================================================//
/*------>> DERIVED: FLATULENCE TORPEDO GOODIES <<------*/
//=====================================================//
class FTorpedoGoodie : public Goodies
{
public:
    FTorpedoGoodie(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw);   // Constructor
    virtual ~FTorpedoGoodie();    // Destructor
    
private:
};


#endif // ACTOR_H_


