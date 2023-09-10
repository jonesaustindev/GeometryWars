#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
	// Add all entities in the m_entitiesToAdd vector to the m_entities vector and m_entityMap
	for (auto& e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);
	}

	m_entitiesToAdd.clear();

	// Remove all dead entities from the m_entities vector and m_entityMap
	for (auto& pair : m_entityMap)
	{
		const std::string& tag = pair.first;
		EntityVec& entityVec = pair.second;

		removeDeadEntities(entityVec);
	}
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	// Use the erase-remove idiom to remove inactive entities
	vec.erase(std::remove_if(vec.begin(), vec.end(),
		[](const std::shared_ptr<Entity>& entity)
		{
			return !entity->isActive();
		}),
		vec.end());

	// Now, update m_entities based on the changes in entityVec
	m_entities.clear();
	for (const auto& pair : m_entityMap)
	{
		const EntityVec& vec = pair.second;
		m_entities.insert(m_entities.end(), vec.begin(), vec.end());
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
