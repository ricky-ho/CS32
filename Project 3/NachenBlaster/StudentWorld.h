#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);     // Constructor
    ~StudentWorld();                        // Destructor
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    double euclidian_dist(double x1, double y1, double x2, double y2);
    Actor* checkCollision(Actor* act);
    
    void destroyedAlien();
    void generateStars();
    void generateAliens();
    void generateProjectile(int imageID, double x, double y, int dir);
    void generateExplosion(int imageID, double x, double y);
    void generateGoodies(int imageID, double x, double y);
    
    void giveGoodie(Actor* gd, Actor* nb);
    void removeDeadActors();
    void updateText(Actor* nb);
    
private:
    std::vector<Actor*> m_actor;
    int m_aliensOnScreen;
    int m_remaining;
    int m_maxAliens;
};

#endif // STUDENTWORLD_H_

