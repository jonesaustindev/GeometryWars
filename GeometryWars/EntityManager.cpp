#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
	// TODO: add entities from m_entitiesToAdd to the proper location(s)
	//        - add them to the vector of all entities
	// 	      - add them to the vector inside the map, with the tag as a key

	for (auto& e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);
	}

	m_entitiesToAdd.clear();

	removeDeadEntites(m_entities);

	// remove dead entities from each vector in the entity map
	// C++17 way of iteration through |key, value| pairs in a map
	//for (auto& [tag, entityVec] : m_entityMap)
	//{
	//	removeDeadEntites(entityVec);
	//}

	//// remove dead entities
	//removeDeadEntites(m_entities);
	//// add new entities
	//for (auto& e : m_entitiesToAdd)
	//{
	//	m_entities.push_back(e);
	//	m_entityMap[e->tag()].push_back(e);
	//}
	//m_entitiesToAdd.clear();
	//// update all entities
	//for (auto& e : m_entities)
	//{
	//	if (e->isActive())
	//	{
	//		e->update();
	//	}
	//}
}

void EntityManager::removeDeadEntites(EntityVec& vec)
{
	// TODO: remove all dead entities from the input vector
	//        this is called by the upate function

	for (auto& e : vec)
	{
		if (!e->isActive())
		{
			// std::remove_if
			vec.erase(std::remove(vec.begin(), vec.end(), e), vec.end());
		}
	}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);

	return entity;
}

const EntityVec& EntityManager::getEntities() const
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag) const
{
	// Find the iterator for the specified tag
	auto it = m_entityMap.find(tag);

	// Check if the tag exists in the map
	if (it != m_entityMap.end())
	{
		// Return the EntityVec associated with the tag
		return it->second;
	}
	else
	{
		// If the tag is not found, return an empty EntityVec
		static const EntityVec emptyEntityVec;
		return emptyEntityVec;
	}
}
