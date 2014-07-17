/* Spiked 2.0
	Chuck Groom, 1997
	
	The mobile_sprite is a derived class of the sprite class, and serves as the parent to 
	most sprites. As the name suggusts, this is code that handles the mechanics of sprite
	behaviors. It does acceleration, velocity, and checks for collisions. If two sprites do
	collide, then they bounce apart.
	
	The rules of motion: 
		€ Mobile prites have a maximum speed limit. 
		€ Sprites may not move off the edges of the screen.
		€ Sprites bounce off one another according to the laws of physics. */
	
#include "sprite.h"
#include "SAT.h"
#include "math.h"
#include "global.h"


mobile_sprite::mobile_sprite(fPoint p, sprite_manager * s_m) : sprite(p, s_m)
{
	acceleration.h = acceleration.v = 0;
	velocity.h = velocity.v = 0;
	max_velocity = 10; 	//default speed limit
	normal_max_velocity = 8;	//natural speed
	
	collideable = TRUE;	//Assume it can be hit
}




void mobile_sprite::do_behavior(void)
{
	//Phun physics
	velocity.h += acceleration.h;
	velocity.v += acceleration.v;
	acceleration.h = 0;
	acceleration.v = 0;
	
	//Check the speed limit
	float speed = sqrt(velocity.h*velocity.h + velocity.v*velocity.v);
	if (speed > normal_max_velocity)	//Must slow down!
	{
		if(speed > max_velocity)
			speed = max_velocity;
		
		speed -= DEACCEL;
		
		set_velocity(speed);
	}
	
	
	//Move the sprite according velocity
	position.h += velocity.h;
	position.v += velocity.v;
	
	//Check to make sure it isn't at edge of screen: if so, bounce it
	if ((position.h - draw_radius) < 0)
	{
		position.h = draw_radius;
		velocity.h = -velocity.h;
	}	
	if ((position.h + draw_radius) > GAME_WIDTH)
	{
		position.h = GAME_WIDTH - draw_radius;
		velocity.h = -velocity.h;
	}	
	if ((position.v - draw_radius) < 32)
	{
		position.v = draw_radius + 32;
		velocity.v = -velocity.v;
	}	
	if ((position.v + draw_radius) > GAME_HEIGHT)
	{
		position.v = GAME_HEIGHT - draw_radius;
		velocity.v = -velocity.v;
	}
	
	//Pass to superclass
	sprite::do_behavior();	
}







// ----------------------------------------------------------
// Did these two sprites collide? They both must be alive, collideable, and within the other's radius.
// ----------------------------------------------------------
Boolean mobile_sprite::collide(sprite * other_sprite)
{
	if(!(alive && other_sprite->alive && collideable && other_sprite->collideable))
		return FALSE;
		
	if(circle_in_sprite(other_sprite->position, other_sprite->collide_radius))
		return TRUE;
	return FALSE;
}
	



void mobile_sprite::set_velocity(float new_vel)
{
	if (!(velocity.h || velocity.v))
		return;	//can't juice up an inmobile sprite
	
	float theta = vector_to_angle(velocity);
	
	velocity.h = new_vel * cos(theta);
	velocity.v = new_vel * sin(theta);
}




float mobile_sprite::vector_to_angle(fPoint vector)
{
	float theta;
	if(vector.h)
		theta = atan(vector.v/vector.h);
	else
		theta = pi/2;
		
	//Decide which direction the arctan points us
	if((vector.h < 0) && (vector.v < 0))
		theta += pi;
	if((vector.h < 0) && (vector.v > 0))
		theta += pi;
	if((vector.h == 0) && (vector.v < 0))
		theta += pi;
	
	return theta;
}



//--------------------------------------------------------------
/* The mechanics of a collision: two sprites collide, and, according to the laws of physics, 
	their velocities change.*/
//------------------------------------------------------------------
void mobile_sprite::do_collision(sprite * other_sprite)
{
	//Are we both alive? It's hard to beat up a dead thing.
	if (!(alive && other_sprite->alive && collideable && other_sprite->collideable))
		return;
		
		
	//Are we both stationary? This shouldn't ever be true; but, if it is, assume that this object
	//has a velocity of x=.05, y = .05
	if(!(velocity.h || velocity.v || other_sprite->velocity.h || other_sprite->velocity.v))
	{
		velocity.h = velocity.v = .05;
	}

	//Move the sprites away from each other
	while (collide(other_sprite))
	{
		position.h -= velocity.h;
		other_sprite->position.h -= other_sprite->velocity.h;
		position.v -= velocity.v;
		other_sprite->position.v -= other_sprite->velocity.v;
	}
	
	//Disable thrust for 4 frames. This is so a player won't hop back into the mess.
	if (thrust_disable < 4)
		thrust_disable = 4;
	
	/* AND NOW, NASTY MATH. FOR BOTH THE X AND Y COMPONENTS OF THE VELOCITY, WE SOLVE
	   THE SYSTEMS OF EQUATIONS:  total momentum initial = total momentum final
								  total energy initial = total energy final
								  
								  Let v1, v2 = initial velocities, m1, m2 be the masses.
								  Let V1, V2 be the final velocities.
								  
					->These collisions are inelastic!<-
					
									v1*m1 + v2*m2 = V1*m1 + V2*m2
			       	.5(m1 * v1^2) + .5(m1 * v2^2) = .5(m1 * V1^2) + .5(m1 * V2^2)
			       			Very nasty to solve. Enjoy!*/


	float total_x_momentum = velocity.h*mass + 
							(other_sprite->velocity.h)*(other_sprite->mass);
	float total_x_energy = .5 * (velocity.h*velocity.h*mass +
								(other_sprite->velocity.h)*(other_sprite->velocity.h)*(other_sprite->mass));	
	
	float a = (mass*other_sprite->mass + mass*mass)/other_sprite->mass;
    float b = -(2*total_x_momentum*mass)/other_sprite->mass;
	float c = (total_x_momentum*total_x_momentum)/other_sprite->mass - 2*total_x_energy;
	
	//And now we cheat. b^2 -4ac MUST be positive: a must not be 0
	if (a == 0) a = .001;
	
	float answer1 = (-b + sqrt(fabs(b*b - 4*a*c)))/(2*a);
	float answer2 = (-b - sqrt(fabs(b*b - 4*a*c)))/(2*a);
	
	if (fabs(answer1 - velocity.h) < .001)
		acceleration.h = answer2 - velocity.h;
	else
		acceleration.h = answer1 - velocity.h;




	float total_y_momentum = velocity.v*mass + 
							(other_sprite->velocity.v)*(other_sprite->mass);
	float total_y_energy = .5 * (velocity.v*velocity.v*mass +
								(other_sprite->velocity.v)*(other_sprite->velocity.v)*(other_sprite->mass));	
	a = (mass*other_sprite->mass + mass*mass)/other_sprite->mass;
    b = -(2*total_y_momentum*mass)/other_sprite->mass;
	c = (total_y_momentum*total_y_momentum)/other_sprite->mass - 2*total_y_energy;
	
	//And now we cheat. b^2 -4ac MUST be positive.
	if (a == 0) a = .001;
	
	 answer1 = (-b + sqrt(fabs(b*b - 4*a*c)))/(2*a);
	 answer2 = (-b - sqrt(fabs(b*b - 4*a*c)))/(2*a);
	
	if (fabs(answer1 - velocity.v) < .001)
		acceleration.v = answer2 - velocity.v;
	else
		acceleration.v = answer1 - velocity.v;
}





	
