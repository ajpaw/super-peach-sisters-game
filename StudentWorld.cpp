#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;
class GraphObject;
class Actor;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

 
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    flagdied = false;
    ppointer = nullptr;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    Level a (assetPath());
 
    ostringstream oss;
    
    flagdied = false;
    
    if (getLevel()<=9) oss<<"level"<<"0"<<getLevel()<<".txt";
    else oss<<"level"<<getLevel()<<".txt";
 
    string level_file = oss.str();
    Level::LoadResult result = a.loadLevel(level_file);
 
     if (result == Level::load_fail_file_not_found)
        {
            cerr << "Could not find "<< level_file<< " data file " << endl;
            return GWSTATUS_LEVEL_ERROR;
        }
    
    
    else if (result == Level::load_fail_bad_format)
        {
            cerr << level_file<< " is improperly formatted" << endl;
            return GWSTATUS_LEVEL_ERROR;
        }
    
    
    else if (result == Level::load_success)
    {
        Level::GridEntry ge; int x = 0 ; int y = 0;

        for (int gx = 0; gx<GRID_WIDTH; gx++)
        {
            for (int gy = 0; gy<GRID_HEIGHT; gy++)
            {
                ge = a.getContentsOf(gx, gy);
                x = gx * SPRITE_WIDTH ;
                y = gy * SPRITE_HEIGHT ;
                
                switch(ge)
                {
                    
                    case Level::block:
                        elements.push_back(new Block(this, x, y, "nothing"));
                        break;
                    
                    case Level::peach:
                        ppointer = new Peach(this, x,y);
                        break;
                        
                     case Level::pipe:
                        elements.push_back(new Pipe(this, x, y ));
                        break;
                     
                     case Level::flag:
                        elements.push_back(new Flag(this, x, y ));
                        break;
                        
                      case Level::mario:
                        elements.push_back(new Mario(this, x, y ));
                        break;
                      
                      case Level::flower_goodie_block:
                        elements.push_back(new Block(this, x, y, "flower"));
                        break;
                        
                      case Level::mushroom_goodie_block:
                        elements.push_back(new Block(this, x, y, "mushroom"));
                        break;
                        
                      case Level::star_goodie_block:
                        elements.push_back(new Block(this, x, y, "star"));
                        break;
                        
                      case Level::goomba:
                       elements.push_back(new Goomba(this, x, y, 180*randInt(0, 1)));
                        break;
                        
                      case Level::koopa:
                        elements.push_back(new Koopa(this,x, y, 180*randInt(0, 1)));
                        break;
                        
                       case Level::piranha:
                        elements.push_back(new Piranha(this,x, y, 180*randInt(0, 1)));
                        break;
    
                      default: ;
                }
                
            }
            
        }
        
        
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
     
    if (ppointer!=nullptr && ppointer->isAlive())
    {
        ppointer->doSomething();
        
    }
    
      
    
     for (vector<Actor*>::iterator it = elements.begin() ; it != elements.end(); ++it)
     {
          
         if ((*it)!=nullptr &&  (*it)->isAlive())
         {
             (*it)->doSomething();
            
             //Checking if a Goal has died right now
             
             if ((*it)->isGoal() &&  (*it)->isAlive() == false )
                {
                    playSound((*it)->mySounds());
                    return (*it)->effectOnGame();
                    break;
                }
             
             //Checking if Peach has died right now
             
             if (ppointer->isAlive() == false)
                {
                    playSound(SOUND_PLAYER_DIE);
                    decLives();
                    return GWSTATUS_PLAYER_DIED;
                }
         }
     }

    
//----------DELETION OF DEAD PEOPLE------------
    
    for (vector<Actor*>::iterator it = elements.begin() ; it != elements.end(); )
    {
        if ((*it)!=nullptr && (*it)->isAlive() == false)
        {
            delete (*it);
            it = elements.erase(it);
        }
        
        else it++;
        
    }

     ostringstream oss2;
     oss2 <<"Lives: " << getLives() << " Level: " << getLevel() << " Points: " << getScore();
     if (ppointer->hasStarPower())
         oss2<<" StarPower!";
     if (ppointer->hasJumpPower())
        oss2<<" JumpPower!";
     if (ppointer->hasShootPower())
        oss2<<" ShootPower!";
    
     setGameStatText(oss2.str());
     return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
    for (vector<Actor*>::iterator it = elements.begin() ; it != elements.end(); )
    {
        if (*it!=nullptr) delete *it;
        it = elements.erase(it);
    }
    
    if (ppointer != nullptr) delete ppointer;
 
}


StudentWorld* StudentWorld::getWorld()
{
    return this;
}

 
 
bool StudentWorld::overlap(Actor* a, Actor* &whatoverlaps)
{
    if (a == nullptr) return false;
    double anowx = a->getX();
    double anowy = a->getY();
    
    double bnowx = 0;
    double bnowy = 0;
    
    for (vector<Actor*>::iterator it = elements.begin() ; it != elements.end(); ++it)
    {
      if (a == (*it)) continue;                     // do not check overlap with myself
      if ((*it)==nullptr) continue;
      if (!(*it)->isAlive()) continue;
      if ((*it)->pathExists() == false) continue;  //overlap only checks for non-solid obj
        
      bnowx = (*it)->getX();
      bnowy = (*it)->getY();
        
        if (anowx < bnowx && bnowx <= anowx+SPRITE_WIDTH-1 &&
            bnowy == anowy)
        {
             
            whatoverlaps = *it;
            return true;
        }
        
        
        if ( anowx+SPRITE_WIDTH-1 > bnowx+SPRITE_WIDTH-1 &&
             anowx <= bnowx+SPRITE_WIDTH-1 &&
             bnowy == anowy)
        {
            
            whatoverlaps = *it;
            return true;
                
        }
        
        if (anowy < bnowy && bnowy <= anowy+SPRITE_HEIGHT-1 &&
            bnowx == anowx)
        {
              
            whatoverlaps = *it;
            return true;
        }
        
        if ( anowy+SPRITE_HEIGHT-1 > bnowy+SPRITE_HEIGHT-1 &&
             anowy <= bnowy+SPRITE_HEIGHT-1 &&
             bnowx == anowx)
        {
             
            whatoverlaps = *it; return true;
                
        }
              
    }
    
    
     //------THIS IS FOR CHECKING FOR PEACH SPECIFICALLY as she is not in vector----
    
    if (a == ppointer) return false;
 
    bnowx = ppointer->getX();
    bnowy = ppointer->getY();
 
      if (anowx <=  bnowx && bnowx <= anowx+SPRITE_WIDTH-1 &&
          bnowy == anowy)
      {
          whatoverlaps = ppointer;
          return true;
      }
      
      
      if ( anowx+SPRITE_WIDTH-1 > bnowx+SPRITE_WIDTH-1 &&
           anowx <= bnowx+SPRITE_WIDTH-1 &&
           bnowy == anowy)
      {
          whatoverlaps = ppointer;
          return true;
              
      }
      
      if (anowy < bnowy && bnowy <= anowy+SPRITE_HEIGHT-1 &&
          bnowx == anowx)
      {
          whatoverlaps = ppointer;
          return true;
      }
      
      if ( anowy+SPRITE_HEIGHT-1 > bnowy+SPRITE_HEIGHT-1 &&
           anowy <= bnowy+SPRITE_HEIGHT-1 &&
           bnowx == anowx)
      {
          whatoverlaps = ppointer;
          return true;
              
      }
      return false;
}

 
void StudentWorld::damage(Actor* victim)
{
    if (victim == ppointer) ppointer->bonk();
    
    else victim->damage();
}

 
bool StudentWorld::isBlockingObjectAt( int x, int y, Actor* hitter)
{
   for (vector<Actor*>::iterator it = elements.begin() ; it != elements.end(); ++it)
   {
       if ((*it)==nullptr) continue;
       
       if ((*it)->pathExists() == false &&
           x < (*it)->getX() &&
           x+SPRITE_WIDTH-1  >=  (*it)->getX() &&
           ( ( (*it)->getY() >= y && (*it)->getY() <= y+SPRITE_HEIGHT-1) ||
             ( (*it)->getY()+SPRITE_HEIGHT-1 >= y && (*it)->getY()+SPRITE_HEIGHT-1 <= y+SPRITE_HEIGHT-1) )
           )
       {
           if (hitter == ppointer)
           (*it)->bonk();        //this means block is to Peachs right
            
           return true;
           
       }
       
       if ((*it)->pathExists() == false &&
           x+SPRITE_WIDTH-1 > (*it)->getX()+SPRITE_WIDTH-1 &&
           x <= (*it)->getX()+SPRITE_WIDTH-1 &&
           ( ( (*it)->getY() >= y && (*it)->getY() <= y+SPRITE_HEIGHT-1) ||
             ( (*it)->getY()+SPRITE_HEIGHT-1 > y && (*it)->getY()+SPRITE_HEIGHT-1 <= y+SPRITE_HEIGHT-1) )
           )
        {
            if (hitter == ppointer)
            (*it)->bonk();          //this means block is to Peachs left
            
            return true;
                
        }
       
       
       if ((*it)->pathExists() == false &&
           y < (*it)->getY() &&
           (*it)->getY() <=  y+SPRITE_HEIGHT-1 &&
           ( (x <= (*it)->getX() && (*it)->getX() <= x+SPRITE_WIDTH-1) ||
             (x <= (*it)->getX()+SPRITE_WIDTH-1 && (*it)->getX()+SPRITE_WIDTH-1 <= x+SPRITE_WIDTH-1) )
           )
       {
           
            if (hitter == ppointer)
                    (*it)->bonk();      //this is when peach hits her head on the block
              
            return true;
       }
       
       if ( (*it)->pathExists() == false &&
            y+SPRITE_HEIGHT-1 > (*it)->getY()+SPRITE_HEIGHT-1 &&
            y <= (*it)->getY()+SPRITE_HEIGHT-1 &&
           ( (x <= (*it)->getX() && (*it)->getX() <= x+SPRITE_WIDTH-1) ||
             (x <= (*it)->getX()+SPRITE_WIDTH-1 && (*it)->getX()+SPRITE_WIDTH-1 <= x+SPRITE_WIDTH-1) )
           )
       {
            //this is when peach is above the block, note there is no bonking
            return true;
               
       }
   }
    
   return false;
}
