#pragma once

#include "Entity.h"
#include <vector>
#include <map>

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

//typedef std::vector<std::shared_ptr<Entity>> EntityVec;
//typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
	EntityVec m_entities;
	EntityVec m_entitiesToAdd;
	EntityMap m_entityMap;
	size_t m_totalEntities = 0;

	void removeDeadEntites(EntityVec& vec);

public:
	EntityManager();

	void update();

	std::shared_ptr<Entity> addEntity(const std::string& tag);

	const EntityVec& getEntities() const;
	const EntityVec& getEntities(const std::string& tag) const;
};
