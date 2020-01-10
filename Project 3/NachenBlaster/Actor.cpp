#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

/*=========================================================================================
 ==============>> ACTOR IMPLEMENTATION <<==================================================
 ========================================================================================*/
Actor::Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw)   // Constructor
: GraphObject(imageID, startX, startY, dir, size, depth), m_world(sw)
{
    m_alive = true;
    m_identifier = imageID;
}

Actor::~Actor()     // Destructor
{ }

int Actor::getIdentifier() const
{ return m_identifier; }

bool Actor::isAlien() const
{ return false; }

bool Actor::isProjectile() const
{ return false; }

bool Actor::isGoodie() const
{ return false; }

bool Actor::isAlive() const
{ return m_alive; }

int Actor::getDmg() const
{ return 0; }

int Actor::getHealthScore() const
{ return 0; }

int Actor::getCabbage() const
{ return 0; }

int Actor::numTorpedo() const
{ return 0; }

void Actor::getHP()
{ }

void Actor::getTorpedo()
{ }

bool Actor::flownOffScreen(int x) const
{
    if ( x < 0 || x >= VIEW_WIDTH)
        return true;
    
    return false;
}

void Actor::dropGoodie(double x, double y)
{ }

void Actor::setDead()
{ m_alive = false; }


StudentWorld* Actor::getWorld()
{ return m_world; }


void Actor::doSomething()
{
    int id = getIdentifier();
    if (id == IID_NACHENBLASTER || id == IID_STAR || isGoodie())
    {
        doDifferent();
        return;
    }
    
    else
    {
        if (! isAlive())
            return;
    
        if (flownOffScreen(getX()))
        {
            setDead();
            return;
        }
    }
    
    doDifferent();
}


/*=========================================================================================
 ==============>> STAR IMPLEMENTATION <<===================================================
 ========================================================================================*/
Star::Star(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw)     // Constructor
: Actor(imageID, startX, startY, dir, size, depth, sw)
{ }

Star::~Star()       // Destructor
{ }

void Star::doDifferent()
{
    moveTo(getX()-1, getY());   // Move the star 1 pixel to the left
    if (flownOffScreen(getX())) // If it moves off the screen (i.e x = -1) then set it as dead
        setDead();
}

/*=========================================================================================
 ==============>> NACHENBLASTER IMPLEMENTATION <<==========================================
 ========================================================================================*/
NachenBlaster::NachenBlaster(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw)   // Constructor
: Actor(imageID, startX, startY, dir, size, depth, sw)
{
    m_hp = NB_MAX_HP;
    m_ep = MAX_CABBAGES;
    m_ftorpedoes = 0;
}

NachenBlaster::~NachenBlaster()     // Destructor
{ }

int NachenBlaster::getHealthScore() const
{ return m_hp; }

int NachenBlaster::getCabbage() const
{ return m_ep; }

int NachenBlaster::numTorpedo() const
{ return m_ftorpedoes; }

void NachenBlaster::getTorpedo()
{
    m_ftorpedoes += 5;
}

void NachenBlaster::getHP()
{
    // If the hp is less than 40, we can simply add 10 to reach max health
    if (m_hp <= NB_MAX_HP - 10)
        m_hp += 10;
    // Otherwise, hp is greater than 40 and we cannot go over max, so we set it to max
    else
        m_hp = NB_MAX_HP;
}

void NachenBlaster::takeDamage(int dmg)
{
    m_hp -= dmg;
}

void NachenBlaster::displayCollision(double x, double y, Actor* obj)
{
    takeDamage(obj->getDmg());   // Take dmg according to the collided object
    obj->setDead();      // Set collided object to dead
    
    if (obj->isAlien()) // if the collided object was an Alien
    {
        if (obj->getIdentifier() == IID_SMALLGON || obj->getIdentifier() == IID_SMOREGON)
            getWorld()->increaseScore(250);
        else
            getWorld()->increaseScore(1000);
        
        getWorld()->destroyedAlien();   // Decrement the number of aliens needed to clear level
        getWorld()->playSound(SOUND_DEATH);
        getWorld()->generateExplosion(IID_EXPLOSION, x, y); // Generate the explosion
        obj->dropGoodie(obj->getX(), obj->getY());  // Check probability of dropping a goodie
    }
    
    else if (obj->isProjectile())   // if the collided object was a projectile
    {
        getWorld()->playSound(SOUND_BLAST);
    }
}

void NachenBlaster::doDifferent()
{
    if (! isAlive())    // If NachenBlaster is not alive, do nothing
        return;
    
    int key;
    if (getWorld()->getKey(key))
    {
        switch(key)
        {
            case KEY_PRESS_UP:
                if (getY() < VIEW_HEIGHT-1)
                    moveTo(getX(), getY()+6);
                break;
                
            case KEY_PRESS_DOWN:
                if (getY() > 0)
                    moveTo(getX(), getY()-6);
                break;
                
            case KEY_PRESS_LEFT:
                if (getX() > 0)
                    moveTo(getX()-6, getY());
                break;
                
            case KEY_PRESS_RIGHT:
                if (getX() < VIEW_WIDTH-1)
                    moveTo(getX()+6, getY());
                break;
                
            case KEY_PRESS_TAB:
                if (m_ftorpedoes > 0)
                {
                    getWorld()->generateProjectile(IID_TORPEDO, getX()+12, getY(), DEFAULT_DIRECTION);
                    m_ftorpedoes--;
                    getWorld()->playSound(SOUND_TORPEDO);
                }
                break;
                
            case KEY_PRESS_SPACE:
                if (m_ep >= MIN_CABBAGES)
                {
                    getWorld()->generateProjectile(IID_CABBAGE, getX()+12, getY(), DEFAULT_DIRECTION);
                    m_ep -= 5;
                    getWorld()->playSound(SOUND_PLAYER_SHOOT);
                }
                break;
        }
    }
    
    // Check for collisions
    Actor* collidedObject = getWorld()->checkCollision(this);
    if (collidedObject != nullptr)
        displayCollision(collidedObject->getX(), collidedObject->getY(), collidedObject);
    
    // If the NachenBlaster has 0 or less health, set it to be dead
    if (getHealthScore() <= 0)
        setDead();
    
    // Generate 1 cabbage energy point per tick of the game up to max 30 cabbages
    if (m_ep < MAX_CABBAGES)
        m_ep++;
}

/*=========================================================================================
 ==============>> EXPLOSION IMPLEMENTATION <<==============================================
 ========================================================================================*/
Explosion::Explosion(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw)  // Constructor
: Actor(imageID, startX, startY, dir, size, depth, sw)
{
    m_life = 0;
}

Explosion::~Explosion()
{ }

void Explosion::doDifferent()
{
    // Increase the size of the explosion over the course of its 4 tick life
    if (m_life < 3)
    {
        double newSize = getSize() * 1.5;
        setSize(newSize);
        m_life++;
    }
    // Once the explosion has lasted 4 ticks, set it to be dead
    else
        setDead();
}

/*=========================================================================================
 ==============>> ALIEN IMPLEMENTATION <<==================================================
 ========================================================================================*/
Alien::Alien(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw, int hp, int fplen, double tspeed, int score)   // Constructor
: Actor(imageID, startX, startY, dir, size, depth, sw)
{
    m_score = score;
    m_hp = hp;
    m_fplen = fplen;
    m_tspeed = tspeed;
}

Alien::~Alien()     // Destructor
{ }

bool Alien::isAlien() const
{ return true; }

int Alien::getFPLength() const
{ return m_fplen; }

double Alien::getTravelSpeed() const
{ return m_tspeed; }

int Alien::getDmg() const
{ return m_dmg; }

int Alien::getHealthScore() const
{ return m_hp; }

void Alien::takeDamage(int dmg)
{
    m_hp -= dmg;
}

void Alien::setDmg(int dmg)
{ m_dmg = dmg; }


int Alien::newFlightPlan() const
{
    if (getFPLength() == 0) // If the flight plan length is 0, return 0
        return 0;
    
    if (getY() <= 0) // If Alien is at bottom, return -1
        return -1;
    
    if (getY() >= VIEW_HEIGHT-1) // If Alien is at top, return 1
        return 1;
    
    // Otherwise, if Alien doesn't need a new flight plan, return 2
    return 2;
}

void Alien::setNewFlightPlan(int fp)
{
    // Based on the return of newFlightPlan, we set the direction accordingly
    if (fp == 0)
        setDirection(getRandDir());
    
    else if (fp == -1)
        setDirection(UP_AND_LEFT);
    
    else if (fp == 1)
        setDirection(DOWN_AND_LEFT);
    
    // Generate a random flight plan length
    setRandFPLength();
}

int Alien::getRandDir() const
{
    // Generates a random int and uses it to determine a random direction
    int num = randInt(0,2);
    
    if (num == 0)
        return LEFT;
    else if (num == 1)
        return UP_AND_LEFT;
    else
        return DOWN_AND_LEFT;
}

void Alien::setRandFPLength()
{ m_fplen = randInt(1, 32); }


void Alien::moveInDir(int dir)
{
    double speed = getTravelSpeed();
    
    if (dir == LEFT)
        moveTo(getX()-speed, getY());
    
    else if (dir == UP_AND_LEFT)
        moveTo(getX()-speed, getY()+speed);

    else
        moveTo(getX()-speed, getY()-speed);
    
    m_fplen--;
}

void Alien::displayCollision(double x, double y, Actor* proj)
{
    takeDamage(proj->getDmg());   // Take dmg according to the object
    proj->setDead();      // Set object to dead
 
    // If the Alien is killed by the NachenBlaster
    if (m_hp <= 0)
    {
        if (proj->isProjectile())
        {
            if (getIdentifier() == IID_SMALLGON || getIdentifier() == IID_SMOREGON)
                getWorld()->increaseScore(250);
            else
                getWorld()->increaseScore(1000);
        }
        
        getWorld()->destroyedAlien();
        setDead();
        getWorld()->playSound(SOUND_DEATH);
        getWorld()->generateExplosion(IID_EXPLOSION, getX(), getY());
    }
    else
        getWorld()->playSound(SOUND_BLAST);
}

void Alien::doDifferent()
{
    // Check for collision with Projectiles
    checkCollision();
    
    // Change flight plan and direction if necessary
    int fp = newFlightPlan();
    if (fp != 2)
        setNewFlightPlan(fp);
    
    // Check if NachenBlaster is to left of Smallgon ***********
    
    // Move in its current direction
    moveInDir(getDirection());
}

//=====================================================//
/*------------>> SMALLGON IMPLEMENTATION <<------------*/
//=====================================================//
Smallgon::Smallgon(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw, int hp, int fplen, double tspeed, int score) // Constructor
: Alien(imageID, startX, startY, dir, ALIEN_SIZE, ALIEN_DEPTH, sw, hp, fplen, tspeed, score)
{
    setDmg(SMALLGON_COLLISION_DMG);
}

Smallgon::~Smallgon()
{ }

void Smallgon::checkCollision()
{
    Actor* collidedProj = getWorld()->checkCollision(this);
    if (collidedProj != nullptr)
        displayCollision(collidedProj->getX(), collidedProj->getY(), collidedProj);
}

//=====================================================//
/*------------>> SMOREGON IMPLEMENTATION <<------------*/
//=====================================================//
Smoregon::Smoregon(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw, int hp, int fplen, double tspeed, int score) // Constructor
: Alien(imageID, startX, startY, dir, ALIEN_SIZE, ALIEN_DEPTH, sw, hp, fplen, tspeed, score)
{
    setDmg(SMOREGON_COLLISION_DMG);
}

Smoregon::~Smoregon()
{ }

void Smoregon::dropGoodie(double x, double y)
{
    int chance1 = randInt(1, 3);    // 1 in 3 chance to drop a goodie
    int chance2 = randInt(1, 2);    // 50/50 chance it will be a torpedo or repair goodie
    if (chance1 == 1)
    {
        if (chance2 == 1)
            getWorld()->generateGoodies(IID_TORPEDO_GOODIE, x, y);
        else
            getWorld()->generateGoodies(IID_REPAIR_GOODIE, x, y);
    }
}

void Smoregon::checkCollision()
{
    // Check for collision with projectiles
    Actor* collidedProj = getWorld()->checkCollision(this);
    if (collidedProj != nullptr)
    {
        displayCollision(collidedProj->getX(), collidedProj->getY(), collidedProj);
        if (! isAlive())
            dropGoodie(getX(), getY());
    }
}

//=====================================================//
/*----------->> SNAGGLEGON IMPLEMENTATION <<-----------*/
//=====================================================//
Snagglegon::Snagglegon(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw, int hp, int fplen, double tspeed, int score) // Constructor
: Alien(imageID, startX, startY, dir, ALIEN_SIZE, ALIEN_DEPTH, sw, hp, fplen, tspeed, score)
{
    setDmg(SNAGGLEGON_COLLISION_DMG);
}

Snagglegon::~Snagglegon()
{ }

void Snagglegon::dropGoodie(double x, double y)
{
    int chance1 = randInt(1, 6);    // 1 in 6 chance to drop a life goodie
    if (chance1 == 1)
        getWorld()->generateGoodies(IID_LIFE_GOODIE, x, y);
}


void Snagglegon::checkCollision()
{
    // Check for collision with projectiles
    Actor* collidedProj = getWorld()->checkCollision(this);
    if (collidedProj != nullptr)
    {
        displayCollision(collidedProj->getX(), collidedProj->getY(), collidedProj);
        if (! isAlive())
            dropGoodie(getX(), getY());
    }
}


/*=========================================================================================
 ==============>> PROJECTILE IMPLEMENTATION <<=============================================
 ========================================================================================*/
Projectile::Projectile(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw)    // Constructor
: Actor(imageID, startX, startY, dir, size, depth, sw)
{ }

Projectile::~Projectile()
{ }

bool Projectile::isProjectile() const
{ return true; }

int Projectile::getDmg() const
{ return m_dmg; }

void Projectile::setDmg(int dmg)
{ m_dmg = dmg; }


//=====================================================//
/*------------->> CABBAGE IMPLEMENTATION <<------------*/
//=====================================================//
Cabbage::Cabbage(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw)  // Constructor
: Projectile(imageID, startX, startY, dir, size, depth, sw)
{
    setDmg(CABBAGE_DMG);
}

Cabbage::~Cabbage()
{ }

void Cabbage::doDifferent()
{
    // Cabbage must move 8 pixels to the right and rotate 20 degrees CCW
    moveTo(getX()+8, getY());
    setDirection(getDirection() + 20);
}


//=====================================================//
/*------------->> TURNIP IMPLEMENTATION <<-------------*/
//=====================================================//
Turnip::Turnip(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld *sw) // Constructor
: Projectile(imageID, startX, startY, dir, size, depth, sw)
{
    setDmg(TURNIP_DMG);
}

Turnip::~Turnip()
{ }

void Turnip::doDifferent()
{
    // Turnip must move 6 pixels left and rotate 20 degrees CCW
    moveTo(getX()-6, getY());
    setDirection(getDirection()+20);
}


//=====================================================//
/*---------->> FLAT_TORPEDO IMPLEMENTATION <<----------*/
//=====================================================//
Flat_Torpedo::Flat_Torpedo(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw)    // Constructor
: Projectile(imageID, startX, startY, dir, size, depth, sw)
{
    setDmg(TORPEDO_DMG);
}

Flat_Torpedo::~Flat_Torpedo()
{ }

void Flat_Torpedo::doDifferent()
{
    // Flat_Torpedoes must move 8 pixels left or right according to Alien or NachenBlaster
    if (isAlien())
        moveTo(getX()-8, getY());
    else
        moveTo(getX()+8, getY());

}

/*========================================================================================
===============>> GOODIES IMPLEMENTATION <<===============================================
========================================================================================*/
Goodies::Goodies(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw)
: Actor(imageID, startX, startY, dir, size, depth, sw)
{ }

Goodies::~Goodies()
{ }

bool Goodies::isGoodie() const
{ return true; }

void Goodies::doDifferent()
{
    // Check for collision with NachenBlaster
    getWorld()->checkCollision(this);

    moveTo(getX() - GOODIE_STEP, getY() - GOODIE_STEP);
}

//=====================================================//
/*-------->> EXTRA LIFE GOODIE IMPLEMENTATION <<-------*/
//=====================================================//
ExtraLifeGoodie::ExtraLifeGoodie(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw)
: Goodies(imageID, startX, startY, dir, size, depth, sw)
{ }

ExtraLifeGoodie::~ExtraLifeGoodie()
{ }


//=====================================================//
/*---------->> REPAIR GOODIE IMPLEMENTATION <<---------*/
//=====================================================//
RepairGoodie::RepairGoodie(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw)
: Goodies(imageID, startX, startY, dir, size, depth, sw)
{ }

RepairGoodie::~RepairGoodie()
{ }


//=====================================================//
/*---------->> FTORPEDO GOODIE IMPLEMENTATION <<-------*/
//=====================================================//
FTorpedoGoodie::FTorpedoGoodie(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw)
: Goodies(imageID, startX, startY, dir, size, depth, sw)
{ }

FTorpedoGoodie::~FTorpedoGoodie()
{ }






