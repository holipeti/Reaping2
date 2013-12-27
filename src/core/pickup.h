#ifndef INCLUDED_CORE_PICKUP_H
#define INCLUDED_CORE_PICKUP_H

class Pickup : public Actor
{
public:
	Pickup(std::string const& Name);
	virtual void Collide(Actor& Other);
	virtual void UpdateLifetime();
};

#endif//INCLUDED_CORE_PICKUP_H
