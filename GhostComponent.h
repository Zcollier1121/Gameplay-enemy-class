//------------------------------------------------------------------------------
// File Name: GhostComponent.h
// Author(s): Zavion Collier (zavion.collier)
// Copyright ©2024 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------
#pragma once

#include "../Engine/Core/Component.h"
#include <vector>
#include "../Engine/include.h"

class SpriteComponent;
class SquareColliderComponent;

//--------------------------------------------------------------------------------
// Class	  : GhostComponent
// 
// Description: The GhostComponent is responsible for controlling the behavior of ghost entities in the game.
// 
// Parent	  : Component
//---------------------------------------------------------------------------------
class GhostComponent : public Component
{

private:

	std::string ghostType; // "Follow" or "Patrol"
	glm::vec3 resetPosition; // Position to reset to on level restart

	// Designer variables
	float ghostSpeed; // Speed of the ghost
	float stunLifetime; // Duration of the stun effect

	float stunAlpha; // Alpha value when stunned
	float redAlpha; // Alpha value when in red EMF
	float yellowAlpha; // Alpha value when in yellow EMF
	float greenAlpha; // Alpha value when in green EMF

	// --- Follow Ghost ---
	float sightRange; // Range at which the ghost can see the player

	// --- Patroling Ghost ---
	bool movingToPointA; // Direction of patrol
	Entity* pointA; // Patrol point A
	Entity* pointB;// Patrol point B
	float patrolTime = 0; // Timer for patrol movement

	// ---- Does not need to be seralized ----
	bool isStunned = false; // Is the ghost currently stunned
	float stunTimer = 0; // Timer for stun duration

	
	float beforeStunSpeed; // We use this variable to hold the speed before we get stunned so we can set the proper value after we get unstunned.

	SpriteComponent* sprite; // The sprite component of the ghost
	SquareColliderComponent* collider; // The collider component of the ghost

public:

	//NOTE: This constructor prototype must be present in ALL components.
	GhostComponent(Entity* entity) : Component(entity) {};

	void OnCreate() override;
	void OnUpdate() override;
	void OnReset() override;

	void Stun();

	friend void to_json(json& j, const GhostComponent& ghostComponent);
	friend void from_json(const json& j, GhostComponent& ghostComponent);
	void Serialize(json& j) override;
	void Deserialize(const json& j) override;

#ifdef _DEBUG
	void DrawInspector() override;
#endif

private:

	void UpdateGhostColor();
	void ChaseBehavior();
	void PatrolBehavior();
	float GetPatrolPercent();
};

