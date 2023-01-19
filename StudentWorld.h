#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include <vector>
#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
using namespace std;

class StudentWorld : public GameWorld
{
  private:
    vector <Actor*> elements;
    Peach* ppointer;
    bool flagdied;
    
    
  public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  StudentWorld*  getWorld();
  bool isBlockingObjectAt(int x, int y, Actor* hitter);
  bool overlap(Actor*a, Actor* &whatoverlaps);
  void toggleflagdead() {if (flagdied) flagdied = false; else flagdied = true;} ;
  void giveShootPower()            { ppointer->giveShoot(); };
  void setPeachHP(int n)           { ppointer->setHP(n); };
  void addActor (Actor* addition)  { elements.push_back(addition);};
  void giveJumpPower()             { ppointer->giveJump(); };
  void giveStarPower()             { ppointer->giveStar(); };
  bool hasSheStarPower()           { return ppointer->hasStarPower();};
  void damage(Actor* victim) ;
  double getPeachX ()                {  return ppointer->getX(); } ;
  double getPeachY ()                {  return ppointer->getY(); } ;
    

    
};

#endif

// STUDENTWORLD_H_
