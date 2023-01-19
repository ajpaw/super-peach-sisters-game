#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
class StudentWorld;
 

class Actor : public GraphObject
{
    public:
        Actor(StudentWorld* g, int image, int startx, int starty, int actordir = 0, int actordepth = 0,
        double actorsize = 1.0);
        virtual ~Actor();
        virtual void doSomething() = 0; virtual void bonk() = 0;
        virtual bool isAlive() const;
    
        virtual bool isEnemy()          {return false;};
        virtual bool isWeapon()         {return false;};
        virtual bool isGoodies()        {return false;};
        virtual bool isHarmless()       {return false;};
        virtual bool isGoal()           {return false;};
        //virtual bool isFlag()           {return false;};
        virtual bool pathExists()       {return true;};
        virtual bool isOurPlayer()      {return false; };
        virtual void setAsDead()        {hitpoints = 0;};
        //virtual bool isMario()          {return false; };
        virtual void setHP(int n)       {hitpoints = n;};
        virtual int changeHPby (int n)  {hitpoints = hitpoints+ n; return hitpoints; };
        virtual bool canGetDam()        {return false;};
        StudentWorld* getWorld();
        virtual void damage()           { };
        virtual int  mySounds()         {return 0;};
        virtual int  effectOnGame()     {return 0;};
    
    
 
    private:
    
        int hitpoints;
        bool isStillAlive = true;
        StudentWorld* myworld;
    
 };

class Harmless : public Actor
{
    public:
        Harmless(StudentWorld* g, int image, int startx, int starty, int actordir = 0, int actordepth = 2,
        double actorsize = 1.0);
        virtual ~Harmless();
        bool isHarmless() {return true;};
        void doSomething() {};
        bool pathExists() {return false;};
        virtual void bonk() = 0;
        bool canGetDam() {return false;};
    
};

 

class Goodies : public Actor
{
    public:
        Goodies (StudentWorld* g, int image, int startx, int starty, int actordir = 0, int actordepth = 1, double actorsize = 1.0);
        virtual ~Goodies();
    
        bool isGoodies () {return true;};
        bool canGetDam()  {return false;};
        void bonk() {};
        virtual void doSomething();
        virtual void givePoints() = 0;
        virtual void givePower() = 0;
    
};

class Flower : public Goodies
{
    public:
          Flower  (StudentWorld* g, int startx, int starty);
          ~Flower ();
          
          void givePoints() ;
          void givePower() ;
        
    
};

class Mushroom : public Goodies
{
    public:
        Mushroom (StudentWorld* g, int startx, int starty);
        ~Mushroom();
 
          void givePoints();
          void givePower();
           
};

class Star : public Goodies
{
    public:
        Star (StudentWorld* g, int startx, int starty);
        ~Star();
 
          void givePoints();
          void givePower();
           
};

class Block : public Harmless
{
    
    public:
        Block(StudentWorld* g, int x, int y, std::string holdswhat = "nothing");
        ~Block();
        bool hasThrown ();
        void bonk();
    
    
    private:
        bool threwGoodie = false;
        std::string holdswhat;
};

class Pipe : public Harmless
{
    
    public:
        Pipe(StudentWorld* g, int x, int y);
        virtual ~Pipe();
         
        void bonk();
    
    
    private:
        
};


class Peach : public Actor
{
    public:
        Peach(StudentWorld* g, int x, int y);
        virtual ~Peach();
        void doSomething();
        void bonk();
        bool hasJumpPower()   { return hasJump; };
        bool hasShootPower()  { return hasShoot; };
        bool hasStarPower()   { return isStarInvinc; };
        bool isOurPlayer()    { return true; };
        void giveShoot()      { hasShoot = true;};
        void giveJump()       { hasJump = true;};
        void giveStar()       { ticks_remaining_starpower = 150; isStarInvinc = true; };
    
       bool canGetDam()        {return true;};
        
    private:
        int ticks_remaining_starpower;
        int ticks_for_temp_invinc;
        bool isTempInvinc;
        bool isStarInvinc;
        std::string mypower;
        bool hasJump;
        bool hasShoot;
        double time_to_recharge_before_next_fire;
        double remaining_jump_distance;
    
    
};

class Goal : public Actor
{
    public:
    Goal (StudentWorld* g, int image, int startx, int starty, int actordir = 0, int actordepth = 1,
          double actorsize = 1.0);
    ~Goal();
 
    void bonk()       { };
    void doSomething();
    bool isGoal ()     {return true;};
    virtual int  mySounds() { return 0; } ;
    virtual int  effectOnGame() { return 0 ;};
    
    
};

class Flag : public Goal
{
   public:
    Flag (StudentWorld* g, int x, int y);
    ~Flag ();
     
    void setAsDead();
     
    int mySounds ()   { return SOUND_FINISHED_LEVEL; };
    int effectOnGame ()   { return GWSTATUS_FINISHED_LEVEL;};
    
};


class Mario : public Goal
{
   public:
    Mario (StudentWorld* g, int x, int y);
    ~Mario ();
 
    int mySounds ()   { return SOUND_GAME_OVER; };
    int effectOnGame ()   { return GWSTATUS_PLAYER_WON;};
};

class Projectile : public Actor
{
  public:
    Projectile(StudentWorld* g, int image, int startx, int starty, int actordir, int actordepth = 1,
    double actorsize = 1.0);
    virtual ~Projectile();
    virtual void doSomething();
    bool pathExists() {return true;};
    void bonk() {};
    bool canGetDam() {return false;};
   
};


class PeachFireball: public Projectile
{
   public:
        PeachFireball (StudentWorld* g,  int startx, int starty, int actordir);
        ~PeachFireball();
        void bonk() {} ;
      
    
};

class PiranhaFireball: public Projectile
{
   public:
        PiranhaFireball (StudentWorld* g,  int startx, int starty, int actordir);
        ~PiranhaFireball();
        void bonk() {};
        void doSomething();
    
};

class Shell: public Projectile
{
    public:
        Shell (StudentWorld* g,  int startx, int starty, int actordir);
        ~Shell();
        void bonk()  {};
         
    
};

class Enemy: public Actor
{
    public:
        Enemy(StudentWorld* g, int image, int startx, int starty, int actordir,
              int actordepth = 0,double actorsize = 1.0);
        virtual ~Enemy();
        bool isEnemy ()  {return true;};
        bool canGetDam() {return true;};
        virtual void doSomething() ;
        virtual void damage();
        virtual void bonk();
    
};

class Goomba: public Enemy
{
    public:
        Goomba(StudentWorld* g, int startx, int starty, int actordir);
        ~Goomba ();
        
};

class Koopa: public Enemy
{
    public:
        Koopa(StudentWorld* g, int startx, int starty, int actordir);
        ~Koopa ();
        void bonk();
        void damage();
    
};

class Piranha: public Enemy
{
    public:
        Piranha(StudentWorld* g, int startx, int starty, int actordir);
        ~Piranha ();
        void doSomething();
 
    private:
        int mydelay;
    
};

#endif // ACTOR_H_
