#pragma once
#include "Collidable.h"
#include <cstdint>

//static const float gravity = 50.0f;
//static const float friction = 50.0f;
//static const float maxSpeed = 50.0f;
//static const float accellerationRate = 10.0f;
//static const float decellerationRate = 10.0f;


class Entity : public Collidable
{
public:
	Entity(Level *level, const AssetManager *assets, sf::Vector2f pos);
	virtual ~Entity() { }

	void update();
	virtual void update(float dt) { }

	void flip(bool right);

	virtual float getGravity() { return 50.0f; }
	virtual float getFriction() { return 50.0f; }
	virtual float getMaxVelocity() { return 50.0f; }
	virtual float getAccelRate() { return 50.0f; }
	virtual float getDecelRate() { return 50.0f; }


protected:
	sf::Vector2f velocity;
	sf::Clock clock;
	bool lastDir, onGround /*onWallL, onWallR*/;

private:
	Collidable *checkTileCollision(uint8_t x, uint8_t y);
};