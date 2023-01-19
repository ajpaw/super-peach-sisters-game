#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
 

Actor::Actor(StudentWorld* g, int image, int startx, int starty, int actordir, int actordepth, double actorsize)
        : GraphObject (image, startx, starty,actordir,actordepth,actorsize)
{
    hitpoints = 1; //each obj starts out alive
    myworld = g;
    
}

Actor::~Actor()
{

}

bool Actor::isAlive() const
{
    if (hitpoints>0) return true;
    return false; 
}

void Actor::doSomething()
{
    
    
}

StudentWorld* Actor::getWorld()
{
    return  myworld;
}

 
// ################ HARMLESS CLASS IMPLEMENTATIONS #################
// #################################################################

Harmless::Harmless(StudentWorld* g, int image, int stx, int sty, int actordir, int actordepth,
double actorsize) : Actor(g, image, stx, sty,0,2,1)
{}

Harmless::~Harmless()
{}

 
// ################ BLOCK CLASS IMPLEMENTATIONS #################
// #################################################################


Block::Block(StudentWorld* g, int x, int y, string holds)
    :Harmless(g, IID_BLOCK, x, y, 0,2,1.0)
{
    holdswhat = holds;
    threwGoodie = false;
    if (holds=="nothing") threwGoodie = true;
    
}

Block::~Block()
{
}

bool Block::hasThrown()
{
    
    if (threwGoodie) return true;
    return false;
}


void Block::bonk()
{
    if (hasThrown() || holdswhat=="nothing")
    { getWorld()->playSound(SOUND_PLAYER_BONK);
    }
    
    else if (holdswhat=="flower" && !hasThrown())   //not yet thrown its flower
    {
         
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        threwGoodie = true;
        Actor* addition = new Flower(getWorld(), getX(), getY()+8);
        getWorld()->addActor(addition);
    }
    
    else if (holdswhat=="mushroom" && !hasThrown()) //not yet thrown its shroom
    {
         
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        threwGoodie = true;
        Actor* addition = new Mushroom(getWorld(), getX(), getY()+8);
        getWorld()->addActor(addition);
        
    }
    
    else if (holdswhat=="star" && !hasThrown())  //not yet thrown its star
    {
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        threwGoodie = true;
        Actor* addition = new Star(getWorld(), getX(), getY()+8);
        getWorld()->addActor(addition);
        
    }
}
 
 
// ################ PIPE CLASS IMPLEMENTATIONS #################
// #############################################################

Pipe::Pipe (StudentWorld* g, int x, int y)
:Harmless(g, IID_PIPE, x, y)
{
}

Pipe:: ~Pipe() {}

void Pipe::bonk()
{}

// ################ PEACH CLASS IMPLEMENTATIONS #################
// #################################################################


Peach::Peach(StudentWorld* g, int x, int y) : Actor(g, IID_PEACH, x, y, 0,0,1)
{
    mypower = "nothing";
    ticks_remaining_starpower = 0;
    isStarInvinc = false;
    ticks_for_temp_invinc = 0;
    hasJump = false;
    hasShoot = false;
    time_to_recharge_before_next_fire = 0;
    remaining_jump_distance = 0;
    isTempInvinc = false;
    
}

Peach::~Peach()
{
    
}

void Peach::bonk() // when bonked by an enemy then do the following:
{
    if (isStarInvinc || ticks_for_temp_invinc>0) return; //checking for invincibility here
    
    
    ticks_for_temp_invinc = 10; //set temp invinc to 10 ticks
    changeHPby(-1);
    
    if (hasShoot)   hasShoot = false;    //turn it off
    if (hasJump)    hasJump = false;    //turn it off
    
    if (changeHPby(0) >= 1 )    //this returns her hitpoints
        getWorld()->playSound(SOUND_PLAYER_HURT);
    
    if (changeHPby(0) <= 0)
        setAsDead();                // StudentWorld should detect Peach’s death and current level ends
}


void Peach::doSomething()
{
    if (! isAlive()) return;
    
    //decrement ticks for power up timers
    
    if (ticks_remaining_starpower > 0) ticks_remaining_starpower--;
    if (ticks_remaining_starpower == 0) isStarInvinc = false;
    
    if (ticks_for_temp_invinc > 0) ticks_for_temp_invinc--;
    if (ticks_for_temp_invinc == 0) isTempInvinc = false;
    
    if (time_to_recharge_before_next_fire > 0) time_to_recharge_before_next_fire--;
    
    // if 0 then she CAN shoot, if she still has shootingpower
    
    
    
    int tempkey = 89; //dummy value for taking input
    
    double xnow = getX();
    double ynow = getY();
    
    Actor* whatoverlaps = nullptr; bool doesOverlap;
    
    doesOverlap = getWorld()->overlap (this, whatoverlaps);

    
    if (doesOverlap && whatoverlaps!= nullptr )
    {
       whatoverlaps->bonk();
    }
   
    if (remaining_jump_distance > 0)            //in the middle of jumping
    {
            if (getWorld()->isBlockingObjectAt(xnow , ynow+4, this))
                remaining_jump_distance = 0;
        
            else
                {
                    moveTo(xnow, ynow+4);
                    remaining_jump_distance--;
                }
    }
    
    else                //check if she is in the middle of falling
    {
        if (getWorld()->isBlockingObjectAt(xnow , ynow, this)   ||
            getWorld()->isBlockingObjectAt(xnow , ynow-1, this) ||
            getWorld()->isBlockingObjectAt(xnow , ynow-2, this) ||
            getWorld()->isBlockingObjectAt(xnow , ynow-3, this) );
        
        else
            moveTo(xnow, ynow-4);
  
    } 
    
    
    getWorld()->getKey(tempkey);
   
    if (tempkey == KEY_PRESS_LEFT)
    {
        setDirection(180);
             
        if (getWorld()->isBlockingObjectAt(xnow-4, ynow, this)) ;
           
        else
           moveTo(xnow-4, ynow);
    }

    if (tempkey == KEY_PRESS_RIGHT)
    {
        setDirection(0);
  
        if (getWorld()->isBlockingObjectAt(xnow+4 , ynow, this)) ; //bonk the target object
                  
        else
            moveTo(xnow+4, ynow);
        
    }
    
    if (tempkey == KEY_PRESS_UP)
    {
         
        if (getWorld()->isBlockingObjectAt(xnow , ynow-1, this))
        {
            if (hasJumpPower()) remaining_jump_distance = 12;
            else remaining_jump_distance = 8;
        }
            
        getWorld()->playSound(SOUND_PLAYER_JUMP);
 
    }
    
    
    if (tempkey == KEY_PRESS_SPACE)
    {
        if (hasShootPower())
        {
            if (time_to_recharge_before_next_fire <= 0)
            {
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                time_to_recharge_before_next_fire = 8;
                
                int firex = 0;
 
                if (getDirection() == 0) //right
                {
                    firex = getX()+4;
                    
                }
                
                else if (getDirection() == 180) //left
                {
                    firex = getX()-4;
                }
                
                //creates fireball with the direction same as peach and pushes into vector
                
                Actor* addition = new PeachFireball(getWorld(), firex, getY(), getDirection());
                getWorld()->addActor(addition);
                
            }
        }
        
        
    
    }
}

// ################ GOAL CLASS IMPLEMENTATIONS #################
// #############################################################

Goal:: Goal (StudentWorld* g, int image, int startx, int starty, int actordir, int actordepth,
double actorsize)
:Actor(g, image, startx, starty, actordir, actordepth, actorsize)
{
}

Goal::~Goal()
{}

void Goal::doSomething()
{
    if (isAlive() == false) return;
       
       Actor* whatoverlaps = nullptr; bool doesit;
       
       doesit = getWorld()->overlap(this, whatoverlaps);
    
       
       if (doesit && whatoverlaps->isOurPlayer())   //if overlapping with peach
       {
           getWorld()->increaseScore(1000);
           this->setAsDead();
       }
    
}

// ################ FLAG CLASS IMPLEMENTATIONS #################
// #############################################################

 
Flag::Flag (StudentWorld* g, int x, int y)
:Goal(g, IID_FLAG, x, y)
{
}

Flag::~Flag ()
{
}

void Flag::setAsDead()
{
    setHP(0); getWorld()-> toggleflagdead();
}




// ################ MARIO CLASS IMPLEMENTATIONS #################
// #################################################################

Mario::Mario (StudentWorld* g, int x, int y)
:Goal(g, IID_MARIO, x, y)
{
}

Mario::~Mario ()
{
    
}

// ################ GOODIES CLASS IMPLEMENTATIONS #################
// #################################################################


Goodies::Goodies(StudentWorld* g, int image, int startx, int starty, int actordir, int actordepth, double actorsize)
: Actor(g, image, startx, starty, actordir, actordepth, actorsize)
{
}

Goodies::~Goodies() {}

void Goodies::doSomething()
{
     if (!isAlive()) return;
      
      Actor* whatoverlaps = nullptr; bool doesit;
    
      doesit = getWorld()->overlap(this, whatoverlaps);
      
      if (doesit && whatoverlaps->isOurPlayer())  //if overlapping with peach
      {
          givePoints();
          givePower();
          getWorld()->setPeachHP(2);
          this->setAsDead();
          getWorld()->playSound(SOUND_PLAYER_POWERUP);
          return;
      }
    
      else if (!getWorld()->isBlockingObjectAt(getX(), getY()-1, this))
      {
          moveTo(getX(), getY()-2);
      }
    
      if  (getDirection() == 0) //facing right
      {
          int targx = getX()+2;
          int targy = getY();
          
          if (getWorld()->isBlockingObjectAt(targx, targy, this))
          {
              setDirection(180);
              return;
          }
          
          else
              moveTo(targx, targy);
          
      }
      
      else if  (getDirection() == 180) //facing left
      {
          int targx = getX()-2;
          int targy = getY();
          
          if (getWorld()->isBlockingObjectAt(targx, targy, this))
          {
              setDirection(0);
              return;
          }
          
          else
              moveTo(targx, targy);

      }
    
}



// ################ FLOWER CLASS IMPLEMENTATIONS #################
// #################################################################


Flower::Flower(StudentWorld* g, int startx, int starty)
:Goodies(g, IID_FLOWER, startx, starty)
{
}

Flower::~Flower() {}
  
void Flower::givePoints()
{
    getWorld()->increaseScore(50);
    
}

void Flower::givePower()
{
    getWorld()->giveShootPower();
}

// ################ MUSHROOM CLASS IMPLEMENTATIONS #################
// #################################################################

Mushroom::Mushroom (StudentWorld* g, int startx, int starty)
:Goodies(g, IID_MUSHROOM, startx, starty)
{
}

Mushroom::~Mushroom() {}

void Mushroom::givePoints()
{
    getWorld()->increaseScore(75);
}

void Mushroom::givePower()
{
    getWorld()->giveJumpPower();
}


// ################ STAR CLASS IMPLEMENTATIONS #####################
// #################################################################

Star::Star (StudentWorld* g, int startx, int starty) : Goodies(g, IID_STAR, startx, starty)
{}

Star::~Star()
{}

void Star::givePoints()
{
    getWorld()->increaseScore(100);
}

void Star::givePower()
{
    getWorld()->giveStarPower();
}

 // ################ PROJECTILE CLASS IMPLEMENTATIONS #############
 // #################################################################

Projectile::Projectile(StudentWorld* g, int image, int startx, int starty, int actordir, int actordepth,
double actorsize)
:Actor(g, image, startx, starty, actordir, actordepth, actorsize)
{
}

Projectile::~Projectile()
{ }

void Projectile::doSomething ()
{
   if (!isAlive()) return;
      
      Actor* whatoverlaps = nullptr; bool doesit;
    
      doesit = getWorld()->overlap(this, whatoverlaps);
      
      if (doesit && whatoverlaps->canGetDam() && !whatoverlaps->isOurPlayer() )
      {
          getWorld()->damage(whatoverlaps);
          this->setAsDead();
          return;
      }
    
      else if (!getWorld()->isBlockingObjectAt(getX(), getY()-1, this))
            moveTo(getX(), getY()-2);
    
      if  (getDirection() == 0) //facing right
      {
        int targx = getX()+2;
        int targy = getY();
        
        if (getWorld()->isBlockingObjectAt(targx, targy, this))
        {
            setAsDead();
            return;
        }
        
        else
            moveTo(targx, targy);
        
    }
    
    else if  (getDirection() == 180) //facing left
    {
        int targx = getX()-2;
        int targy = getY();
        
        if (getWorld()->isBlockingObjectAt(targx, targy, this))
        {
            setAsDead();
            return;
        }
        
        else
            moveTo(targx, targy);
    }
    
    
}


// ################ PIRANHA BALL CLASS IMPLEMENTATIONS #############
// #################################################################


PiranhaFireball::PiranhaFireball (StudentWorld* g,  int startx, int starty, int actordir)
:Projectile(g, IID_PIRANHA_FIRE, startx, starty, actordir)
{}

PiranhaFireball::~PiranhaFireball()
{}

void PiranhaFireball::doSomething( )
{
    if (!isAlive()) return;
      
      Actor* whatoverlaps = nullptr; bool doesit;
    
      doesit = getWorld()->overlap(this, whatoverlaps);
      
      if (doesit && whatoverlaps->isOurPlayer())  //if overlapping with peach
      {
          getWorld()->damage(whatoverlaps);  //this is not like other projectiles, so it must damage Peach
          this->setAsDead();
          return;
      }
    
      else if (!getWorld()->isBlockingObjectAt(getX(), getY()-1, this))
            moveTo(getX(), getY()-2);
    
      if  (getDirection() == 0) //right facing
      {
        int targx = getX()+2;
        int targy = getY();
        
        if (getWorld()->isBlockingObjectAt(targx, targy, this))
        {
            setAsDead();
            return;
        }
        
        else
            moveTo(targx, targy);
        
    }
    
    else if  (getDirection() == 180) //left facing
    {
        int targx = getX()-2;
        int targy = getY();
        
        if (getWorld()->isBlockingObjectAt(targx, targy, this))
        {
            setAsDead();
            return;
        }
        
        else
            moveTo(targx, targy);
    }
    
}

// ################ PEACH BALL CLASS IMPLEMENTATIONS #############
// #################################################################
 
PeachFireball::PeachFireball (StudentWorld* g,  int startx, int starty, int actordir)
:Projectile(g, IID_PEACH_FIRE, startx, starty, actordir)
{}

PeachFireball::~PeachFireball()
{}
    
// ################ SHELL CLASS IMPLEMENTATIONS ####################
// #################################################################

Shell::Shell (StudentWorld* g,  int startx, int starty, int actordir)
:Projectile(g, IID_SHELL, startx,  starty, actordir)
{}

Shell::~Shell()
{}

// ################ ENEMY CLASS IMPLEMENTATIONS ####################
// #################################################################

Enemy::Enemy(StudentWorld* g, int image, int startx, int starty, int actordir, int actordepth, double actorsize)
:Actor (g, image, startx, starty, actordir, actordepth, actorsize)
{}

Enemy::~Enemy()
{}

void Enemy::doSomething()
{
    if (!isAlive()) return;
         
         Actor* whatoverlaps = nullptr; bool doesit;
       
         doesit = getWorld()->overlap(this, whatoverlaps);
         
         if (doesit && whatoverlaps->isOurPlayer())  //if overlapping with peach
         {
             getWorld()->damage(whatoverlaps);
             return;
         }
       
         if  (getDirection() == 0) //right facing
         {
             int targx = getX()+1;
             int targy = getY();
        
             if (getWorld()->isBlockingObjectAt(targx, targy, this))
                 setDirection(180);
 
         }
    
         else if  (getDirection() == 180) //left
         {
             int targx = getX()-1;
             int targy = getY();
        
             if (getWorld()->isBlockingObjectAt(targx, targy, this))
                 setDirection(0);
            
         }
    
         if (getDirection() == 0)
         {
             if (!getWorld()->isBlockingObjectAt(getX()+ SPRITE_WIDTH-1, getY()-1, this))
                 setDirection(180);
         }
       
         else if (getDirection() == 180)
         {
             if (!getWorld()->isBlockingObjectAt(getX()-SPRITE_WIDTH-1, getY()-1 , this)  )
             setDirection(0);
         }
       
         if  (getDirection() == 0) //right
              {
                  int targx = getX()+1;
                  int targy = getY();
             
                  if (!getWorld()->isBlockingObjectAt(targx, targy, this))
                      moveTo(targx, targy);
                  else return;
                  
              }
         
              else if  (getDirection() == 180) //left
              {
                  int targx = getX()-1;
                  int targy = getY();
             
                  if (!getWorld()->isBlockingObjectAt(targx, targy, this))
                      moveTo(targx, targy);
                  else return;
         
              }
       
}

void Enemy::bonk()
{
    Actor* whatoverlaps = nullptr; bool doesit;
    
    doesit = getWorld()->overlap(this, whatoverlaps);
      
    if (doesit && whatoverlaps->isOurPlayer() && getWorld()->hasSheStarPower())
    {
        getWorld()->playSound(SOUND_PLAYER_KICK);       //only get damaged if Peach has StarPower
        getWorld()->increaseScore(100);
        setAsDead();
    }
    
}

void Enemy::damage()
{
      getWorld()->increaseScore(100);
      setAsDead();
}



// ################ GOOMBA CLASS IMPLEMENTATIONS ####################
// #################################################################


Goomba::Goomba(StudentWorld* g, int startx, int starty, int actordir)
:Enemy(g, IID_GOOMBA, startx, starty, actordir)
{
}

Goomba::~Goomba()
{}

// ################ KOOPA CLASS IMPLEMENTATIONS ####################
// #################################################################


Koopa::Koopa(StudentWorld* g,   int startx, int starty, int actordir)
:Enemy(g, IID_KOOPA, startx, starty, actordir)
{
}

Koopa::~Koopa()
{}

void Koopa::bonk( )
{
    Actor* whatoverlaps = nullptr; bool doesit;
    
    doesit = getWorld()->overlap(this, whatoverlaps);
      
    if (doesit && whatoverlaps->isOurPlayer() && getWorld()->hasSheStarPower())
    {
        
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        setAsDead();
        Actor* addition = new Shell(getWorld(), getX(), getY(), getDirection() );
        getWorld()->addActor(addition);     //creating a new Shell and pushing into vector
    }
}

void Koopa::damage()
{
   getWorld()->increaseScore(100);
   setAsDead();
   Actor* addition = new Shell(getWorld(), getX(), getY(), getDirection() );
   getWorld()->addActor(addition);       //creating a new Shell and pushing into vector
}

// ################ PIRANHA CLASS IMPLEMENTATIONS ####################
// #################################################################

Piranha::Piranha(StudentWorld* g,  int startx, int starty, int actordir)
:Enemy(g, IID_PIRANHA, startx, starty, actordir)
{
    mydelay = 0;
}

Piranha::~Piranha()
{}

void Piranha::doSomething()
{
    if (!isAlive()) return;
    increaseAnimationNumber();
    
    Actor* whatoverlaps = nullptr; bool doesit;
    
    doesit = getWorld()->overlap(this, whatoverlaps);
      
    if (doesit && whatoverlaps->isOurPlayer())   //if i am exactly on Peach
    {
        whatoverlaps-> bonk();
        return;
    }
    
    if (abs(getWorld()->getPeachY() - getY()) < 1.5*SPRITE_HEIGHT) ;
    else return;
    
    if (getWorld()->getPeachX() > getX()) setDirection(0);
    else setDirection(180);
    
    if (mydelay > 0) { mydelay--; return; }     //dont fire if there is a timer left
    else
    {
        if ( abs (getWorld()->getPeachX() - getX()) < 8*SPRITE_WIDTH )
        {
            Actor* addition = new PiranhaFireball(getWorld(), getX(),getY(), getDirection());
            getWorld()->addActor(addition);
            getWorld()->playSound(SOUND_PIRANHA_FIRE);
            mydelay = 40;
        }
        
    }
    
}

 
