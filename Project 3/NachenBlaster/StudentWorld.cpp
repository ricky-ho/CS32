#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <random>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp


// Calculate the distance between two actors
double StudentWorld::euclidian_dist(double x1, double y1, double x2, double y2)
{
    return (sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
}

// Check for collision with collideable actors
Actor* StudentWorld::checkCollision(Actor* act)
{
    double x1 = act->getX();
    double y1 = act->getY();
    double r1 = act->getRadius();
    
    vector<Actor*>::iterator it = m_actor.begin();
    double x2, y2, r2;
    
    if (act->getIdentifier() == IID_NACHENBLASTER) // NachenBlaster can only collide with aliens and projectiles and goodies
    {
        while (it != m_actor.end())     // Loop through every actor to check for collision
        {
            if (((*it)->isAlien() || (*it)->isProjectile()) && (*it)->isAlive())
            {
                x2 = (*it)->getX();
                y2 = (*it)->getY();
                r2 = (*it)->getRadius();
            
                if (euclidian_dist(x1, y1, x2, y2) < (0.75 * (r1 + r2)))    // If there is a collision, return ptr to that actor
                    return (*it);
            }
            it++;
        }
    }
        
    else if (act->isAlien())   // Aliens check for collision with projectiles
    {
        while (it != m_actor.end())
        {
            if ((*it)->isProjectile() && (*it)->isAlive())
            {
                x2 = (*it)->getX();
                y2 = (*it)->getY();
                r2 = (*it)->getRadius();
            
                if (euclidian_dist(x1, y1, x2, y2) < (0.75 * (r1 + r2)))
                    return (*it);
            }
            it++;
        }
    }
        
    else if (act->isGoodie())   // Goodies check for collision with NachenBlaster
    {
        while (it != m_actor.end())
        {
            if ((*it)->getIdentifier() == IID_NACHENBLASTER)
            {
                x2 = (*it)->getX();
                y2 = (*it)->getY();
                r2 = (*it)->getRadius();
                
                if (euclidian_dist(x1, y1, x2, y2) < (0.75 * (r1 + r2)))
                    giveGoodie(act, (*it));
            }
            it++;
        }
    }
    
    return nullptr;
}



void StudentWorld::destroyedAlien()
{
    m_remaining--;
}


// 1 in 15 Chance to Generate a Star
void StudentWorld::generateStars()
{
    int n = randInt(0, 14);
    if (n == 14)
    {
        // Generate a random starting Y and random size for star
        double randsize = (randInt(5, 50)) / 100.0;
        int randy = randInt(0, VIEW_HEIGHT-1);
        
        Actor* newStar = new Star(IID_STAR, VIEW_WIDTH-1, randy, DEFAULT_DIRECTION, randsize, STAR_DEPTH, this);   // Create a new star to replace the deleted star
        m_actor.push_back(newStar); // <----ITERATOR INVALIDATED
    }
}


// Generate Aliens                      // FIX INVERTED SNAGGLEGON AND DIRECTIONS
void StudentWorld::generateAliens()
{
    if (m_aliensOnScreen == m_maxAliens)    // There can only be m_maxAliens on screen at one time
        return;
    
    int level = getLevel();
    
    int S1 = 60;
    int S2 = 20 + (level * 5);
    int S3 = 5 + (level * 10);
    int S = S1 + S2 + S3;
    
    Actor* newAlien;
    int hp;
    int randy = randInt(0, VIEW_HEIGHT-1);
    
    int random = randInt(1, S);
    if (random <= S1)       // Generate Smallgon with S1/S probability
    {
        hp = 5 * (1 + (level - 1) * .1);
        newAlien = new Smallgon(IID_SMALLGON, VIEW_WIDTH-1, randy, DEFAULT_DIRECTION, ALIEN_SIZE, ALIEN_DEPTH, this, hp, ALIEN_FPLEN, SMALLGON_TSPEED, SMALLGON_SCORE_VAL);
    }
    else if (random > S1 && random <= (S1 + S2))        // Generate Smoregon with S2/S probability
    {
        hp = 5 * (1 + (level - 1) * .1);
        newAlien = new Smoregon(IID_SMOREGON, VIEW_WIDTH-1, randy, DEFAULT_DIRECTION, ALIEN_SIZE, ALIEN_DEPTH, this, hp, ALIEN_FPLEN, SMOREGON_TSPEED, SMOREGON_SCORE_VAL);
    }
    else if (random > (S1 + S2) && random <= S)         // Generate Snagglegon with S3/S probability
    {
        hp = 10 * (1 + (level - 1) * .1);
        newAlien = new Snagglegon(IID_SNAGGLEGON, VIEW_WIDTH-1, randy, DOWN_AND_LEFT, ALIEN_SIZE, ALIEN_DEPTH, this, hp, ALIEN_FPLEN, SNAGGLEGON_TSPEED, SNAGGLEGON_SCORE_VAL);
    }
    
    m_actor.push_back(newAlien);
    
    m_aliensOnScreen++;
}


// Generate projectiles
void StudentWorld::generateProjectile(int imageID, double x, double y, int dir)
{
    Actor* newProjectile;
    
    if (imageID == IID_CABBAGE)
    {
        newProjectile = new Cabbage(imageID, x, y, dir, PROJ_SIZE, PROJ_DEPTH, this);
        m_actor.push_back(newProjectile);
    }
    
    else if (imageID == IID_TURNIP)
    {
        newProjectile = new Turnip(imageID, x, y, dir, PROJ_SIZE, PROJ_DEPTH, this);
        m_actor.push_back(newProjectile);
    }
    
    else
    {
        newProjectile = new Flat_Torpedo(imageID, x, y, dir, PROJ_SIZE, PROJ_DEPTH, this);
        m_actor.push_back(newProjectile);
    }
}

void StudentWorld::generateExplosion(int imageID, double x, double y)
{
    Actor* newExplosion = new Explosion(imageID, x, y, DEFAULT_DIRECTION, EXPLOSION_SIZE, EXPLOSION_DEPTH, this);
    m_actor.push_back(newExplosion);
}

void StudentWorld::generateGoodies(int imageID, double x, double y)
{
    if (imageID == IID_LIFE_GOODIE)
    {
        Actor* newGoodie = new ExtraLifeGoodie(imageID, x, y, DEFAULT_DIRECTION, GOODIE_SIZE, GOODIE_DEPTH, this);
        m_actor.push_back(newGoodie);
    }
    else if (imageID == IID_REPAIR_GOODIE)
    {
        Actor* newGoodie = new RepairGoodie(imageID, x, y, DEFAULT_DIRECTION, GOODIE_SIZE, GOODIE_DEPTH, this);
        m_actor.push_back(newGoodie);
    }
    else
    {
        Actor* newGoodie = new FTorpedoGoodie(imageID, x, y, DEFAULT_DIRECTION, GOODIE_SIZE, GOODIE_DEPTH, this);
        m_actor.push_back(newGoodie);
    }
    
}

void StudentWorld::giveGoodie(Actor* gd, Actor* nb)
{
    increaseScore(GOODIE_SCORE_VAL);
    gd->setDead();
    playSound(SOUND_GOODIE);
    
    int identity = gd->getIdentifier();
    if (identity == IID_LIFE_GOODIE)
        incLives();
    else if (identity == IID_REPAIR_GOODIE)
        nb->getHP();
    else if (identity == IID_TORPEDO_GOODIE)
        nb->getTorpedo();
}

// Deletes and erases actors that are dead
void StudentWorld::removeDeadActors()
{
    vector<Actor*>::iterator it = m_actor.begin();
    
    while (it != m_actor.end())
    {
        if (! (*it)->isAlive())     // If the actor is no longer alive, then delete it
        {
            if ((*it)->isAlien())   // Decrement the # of aliens on screen
                m_aliensOnScreen--;
            
            delete(*it);
            it = m_actor.erase(it);
        }
        else          // Otherwise, we move onto the next actor
            it++;
    }
}

// Update the text displayed at the top of the screen
void StudentWorld::updateText(Actor* nb)
{
    int health = nb->getHealthScore()*100.0;
    int cabbage = nb->getCabbage()*100.0;
    int torpedo = nb->numTorpedo();
    
    ostringstream oss;
    oss << "Lives: " << getLives() << "  " << "Health:  " << health/NB_MAX_HP << "%" << "  " << "Score: " << getScore() << "  "
        << "Level: " << getLevel() << "  " << "Cabbages: " << cabbage/MAX_CABBAGES << "%" << "  " << "Torpedoes: " << torpedo;
    string text = oss.str();
    setGameStatText(text);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

StudentWorld::StudentWorld(string assetDir) // Constructor
: GameWorld(assetDir)
{
    m_aliensOnScreen = 0;
    m_maxAliens = 4 + (0.5 * getLevel());
    m_remaining = 6 + (4 * getLevel());
}

StudentWorld::~StudentWorld()               // Destructor
{ cleanUp(); }

int StudentWorld::init()
{
    for (int k = 0; k < MAX_STARS; k++)    // Create 30 stars at random x,y locations
    {
        // Generate a random starting X and starting Y, and a random double between .05 and .50 for star's size
        double randsize = (randInt(5, 50)) / 100.0;
        int randx = randInt(0, VIEW_WIDTH-1);
        int randy = randInt(0, VIEW_HEIGHT-1);
        
        Actor* a = new Star(IID_STAR, randx, randy, DEFAULT_DIRECTION, randsize, STAR_DEPTH, this);
        m_actor.push_back(a);
    }
    
    // Create NachenBlaster
    Actor* newNB = new NachenBlaster(IID_NACHENBLASTER, 0, NB_START_Y, DEFAULT_DIRECTION, 1.0, NB_DEPTH, this);
    m_actor.push_back(newNB);
    
    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
    vector<Actor*>::iterator it = m_actor.begin();
    Actor* NB = nullptr;
    
    // Call all of the the actors to doSomething
    while (it != m_actor.end())
    {
        if ((*it)->getIdentifier() == IID_NACHENBLASTER)
            NB = (*it);
        
        (*it)->doSomething();
        it++;
    }
    
    // Check to see if we killed enough aliens to advance to next level
    if (m_remaining == 0)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    // Check to see if the NachenBlaster is dead
    if (NB != nullptr && ! NB->isAlive())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    // Once all actors have gotten a chance to move, check to delete any actors who are dead
    removeDeadActors();
    
    // Update game stat text
    updateText(NB);
    
    // 1/15 Chance to Generate a Star
    generateStars();
    
    // Generate aliens onto screen
    if (m_aliensOnScreen < min(m_maxAliens, m_remaining))
        generateAliens();
    
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
    m_aliensOnScreen = 0;
    m_maxAliens = 4 + (0.5 * getLevel());
    m_remaining = 6 + (4 * getLevel());
    
    // Delete all other actors
    vector<Actor*>::iterator it = m_actor.begin();
    while (it != m_actor.end())
    {
        delete (*it);
        it = m_actor.erase(it);
    }
}
