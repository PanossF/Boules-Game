/*-----------------------------------------------------------
  Simulation Header File
  -----------------------------------------------------------*/
#include"vecmath.h"
#include<string>
#include<iostream>

/*-----------------------------------------------------------
  Macros
  -----------------------------------------------------------*/
#define TABLE_X			(1.2f) 
#define TABLE_Z			(1.8f)
#define TABLE_Y			(0.1f)
#define BALL_RADIUS		(0.05f)
#define BALL_MASS		(0.1f)
#define TWO_PI			(6.2832f)
#define	SIM_UPDATE_MS	(10)
#define NUM_BALLS		(9)
#define PLAYER_BALLS	(4)
#define NUM_CUSHIONS	(5)		
#define MAX_PARTICLES	(200)



/*-----------------------------------------------------------
  cushion class
  -----------------------------------------------------------*/
class cushion
{
public:
	vec2	vertices[2]; //2d
	vec2	centre;
	vec2	normal;

	void MakeNormal(void);
	void MakeCentre(void);
};

/*-----------------------------------------------------------
  ball class
  -----------------------------------------------------------*/

class ball
{
	static int ballIndexCnt;
public:
	vec2	position;
	vec2	velocity;
	float	radius;
	float	mass;
	int		index;

	//Panos Game
	ball() : position(0.0,1.0), velocity(0.0), radius(BALL_RADIUS),
		mass(BALL_MASS) {index = ballIndexCnt++; Reset();}
	
	void Reset(void);
	void ApplyImpulse(vec2 imp);
	void ApplyFrictionForce(int ms);
	void DoPlaneCollision(const cushion &c);
	void DoBallCollision(ball &b);
	void Update(int ms);
	
	bool HasHitPlane(const cushion &c) const;
	bool HasHitBall(const ball &b) const;

	void HitPlane(const cushion &c);
	void HitBall(ball &b);
};


//class paricle
class particle 
{
public:
	vec3 position;
	vec3 velocity;

	particle() {position=0;velocity=0;}
	void update(int ms);
};

class particleSet 
{
public:
	particle *particles[MAX_PARTICLES];
	int num;

	particleSet()
	{
		for(int i=0;i<MAX_PARTICLES;i++) particles[i] = 0;
		num=0;
	}

	~particleSet()
	{
		for(int i=0;i<MAX_PARTICLES;i++)
		{
			if(particles[i]) delete particles[i];
		}
	}

	void AddParticle(const vec3 &pos);
	void update(int ms);
};


/*-----------------------------------------------------------
  table class
  -----------------------------------------------------------*/
class table
{
public:
	ball balls[1];	
	ball balls1[4];
	ball balls2[4];

	cushion cushions[NUM_CUSHIONS];
	particleSet parts;

	void SetupCushions(void);
	void SetupCushions2(void);
	void SetupCushions3(void);
	void SetupCushions4(void);
	void SetupCushions5(void);
	void Update(int ms);	
	bool AnyBallsMoving(void) const;
	float JackDistance(ball& b);
};

//player class
class player
{
public:
	std::string name;
	float score = 0;
	int ranking = 0;
};

/*-----------------------------------------------------------
  global table
  -----------------------------------------------------------*/
extern table gTable;

