#include "Entity.h"
#include "Level.h"
#include <Windows.h>
#include <iostream>

Entity::Entity(Level *level, const AssetManager *assets, sf::Vector2f pos)
	: Collidable(level, assets, pos)
{
}

void Entity::update()
{
	Collidable::update();
	int yIter, xIter;
	flip(lastDir);
	
	Vector2f oldPosition = getPosition();
	sf::Time deltaTime = clock.restart();
	float deltaSeconds = deltaTime.asSeconds();
	move(velocity * deltaSeconds);

	static const std::string tileAnimations[3] = { "leftTile", "middleTile", "rightTile" };

	const sf::Vector2f &pos = getPosition();
	const Collidable *tile = nullptr;
	Collidable *aboveTile = nullptr;
	for (int8_t i = -1; i < 2; i++)
	{
		tile = checkTileCollision((pos.x / 8) + i, (pos.y + getGlobalBounds().height) / 8);
		if (tile)
		{
			velocity.y = 0.0f;
			setPosition(pos.x, tile->getPosition().y - getGlobalBounds().height);
			onGround = true;
			break;
		}
		aboveTile = checkTileCollision(((pos.x) / 8) + i, pos.y / 8);
		if (aboveTile && aboveTile->getType() == ObjectType::Tile)
		{
			aboveTile->PlayAnimation(tileAnimations[i + 1]);
			// Go to the bottom of the tile
			setPosition(pos.x, (aboveTile->getPosition().y / 8) * 8);
		}
	}

	if (!tile)
	{
		velocity.y = std::min(150.0f, velocity.y + 800 * deltaSeconds);
		onGround = false;
	}
	
	////Vertical Collision
	//for (int i = 0; i < ceilf(getGlobalBounds().width / 8.0f) + 1; i++)
	//{
	//	//if last iteration, test at max width rather than adding another tile's worth of height
	//	if (i == ceilf(getGlobalBounds().width / 8.0f))
	//		xIter = getGlobalBounds().width;
	//	else
	//		xIter = i;

	//	//Check above tiles
	//	tile = checkTileCollision(pos.x + xIter, pos.y / 8);
	//	if (tile)
	//	{
	//		// Go to the bottom of the tile
	//		setPosition(pos.x, tile->getPosition().y + tile->getGlobalBounds().height);
	//	}

	//	//Check below tiles
	//	tile = checkTileCollision(((pos.x - getGlobalBounds().width / 2) + (xIter * 8)) / 8, pos.y + getGlobalBounds().height);
	//	if (!tile)
	//	{
	//		velocity.y = std::min(150.0f, velocity.y + 800 * deltaSeconds);
	//		onGround = false;
	//	}
	//	else
	//	{
	//		velocity.y = 0.0f;
	//		setPosition(pos.x, tile->getPosition().y - getGlobalBounds().height);
	//		onGround = true;
	//	}
	//}


	//Horizontal Collision
	for (int i = 0; i < ceilf(getGlobalBounds().height / 8.0f) + 1; i++)
	{
		//if last iteration, test at max height rather than adding another tile's worth of height
		if (i == ceilf(getGlobalBounds().height / 8.0f))
			yIter = getGlobalBounds().height;
		else
			yIter = i;

		//Check left tiles
		tile = checkTileCollision(((pos.x - getGlobalBounds().width / 2) /*- 1*/) / 8, (pos.y / 8) + yIter);
		if (tile)
		{
			velocity.x = 0;
			setPosition(tile->getPosition().x + tile->getGlobalBounds().width + (getGlobalBounds().width / 2), pos.y);
			//break;
		}

		//Check right tiles
		tile = checkTileCollision(((pos.x + getGlobalBounds().width / 2) /*+ 1*/) / 8, (pos.y / 8) + yIter);
		if (tile)
		{
			velocity.x = 0;
			setPosition(tile->getPosition().x - getGlobalBounds().width / 2, pos.y);
		}
	}

	update(deltaSeconds);
}

Collidable *Entity::checkTileCollision(uint8_t x, uint8_t y)
{
	Collidable *tile = level->getTile(x, y);
	return tile && tile->getType() != ObjectType::None && getGlobalBounds().intersects(tile->getGlobalBounds()) ? tile : nullptr;
}

void Entity::flip(bool right)
{
	setOrigin(getBounds().width / 2, 0);
	setScale(!right ? 1 : -1, 1);
}