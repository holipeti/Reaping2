#include "i_core.h"

Player::Player()
{
	mFields[COLLISION_CLASS].i=CollisionClass::Player;
}

void Player::Collide( double Seconds, ActorList& Actors )
{

}
