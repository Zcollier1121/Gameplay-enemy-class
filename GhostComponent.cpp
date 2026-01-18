//------------------------------------------------------------------------------
// File Name: GhostComponent.cpp
// Author(s): Zavion Collier (zavion.collier)
// Copyright �2024 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#include "GhostComponent.h"
#include "../Engine/Core/Time.h"
#include "../Engine/Core/Math.h"
#include "CharacterControllerComponent.h"
#include "../Engine/Core/Scene.h"
#include "../Engine/Components/SpriteComponent.h"
#include "../Engine/Components/SquareColliderComponent.h"
#include "../Game/ToolsComponent.h"
#include "../Engine/Core/IEasing.h"
#include "../Game/GhostParticle.h"

//--------------------------------------------------------------------------------
// Function   : OnCreate
// 
// Description: sets up the ghost component by initializing variables and adding necessary components.
//---------------------------------------------------------------------------------
void GhostComponent::OnCreate()
{
	resetPosition = GetTransform().GetLocalPosition();
	sprite = GetComponent<SpriteComponent>();
	collider = GetComponent<SquareColliderComponent>();
	GetEntity()->AddComponent<GhostParticle>();
	GetComponent<GhostParticle>()->SetIsFollowing(false);

}

//--------------------------------------------------------------------------------
// Function   : OnUpdate
// 
// Description: Ghost's main behavior loop, handles stunned state and movement behavior.
//---------------------------------------------------------------------------------
void GhostComponent::OnUpdate()
{
	// Handle stunned state, disabling collider and managing stun timer.
	if (isStunned)
	{
		collider->SetEnabled(false);

		stunTimer += Time::GetDeltaTime();

		if (stunTimer >= stunLifetime)
		{
			isStunned = false;
			GetComponent<GhostParticle>()->isStunned = false;
			sprite->SetEnabled(true);
			sprite->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
			sprite->SetAlpha(stunAlpha);
			collider->SetEnabled(true);

			stunTimer = 0;
		}
	}
	else
	{
		collider->SetEnabled(true);

		// Execute behavior based on ghost type (Follow or Patrol).
		if (ghostType == "Follow")
		{
			ChaseBehavior();
			GetComponent<GhostParticle>()->SetIsFollowing(true);
		}
		else if (ghostType == "Patrol")
		{
			PatrolBehavior();
			patrolTime += Time::GetDeltaTime();
		}
	}

	// Update ghost's visual representation based on state and proximity to player.
	UpdateGhostColor();
}

//--------------------------------------------------------------------------------
// Function   : OnReset
// 
// Description: sets the ghost back to its initial state and position after level restart.
//---------------------------------------------------------------------------------
void GhostComponent::OnReset()
{
	patrolTime = 0;
	GetTransform().SetLocalPosition(resetPosition);
	isStunned = false;
	collider->SetEnabled(true);
	if (!entity->GetActiveSelf())
		entity->SetActive(true);
	UpdateGhostColor();
}

//--------------------------------------------------------------------------------
// Function   : Stun
// 
// Description: sets the ghost to a stunned state, disabling its collider and updating its appearance.
//---------------------------------------------------------------------------------
void GhostComponent::Stun()
{
	isStunned = true;
	GetComponent<GhostParticle>()->isStunned = true;
	stunTimer = 0;
	collider->SetEnabled(false);
	UpdateGhostColor();
}

//--------------------------------------------------------------------------------
// Function   : ChaseBehavior
// 
// Description: defines the chasing behavior of the ghost, moving it towards the player if within sight range.
//---------------------------------------------------------------------------------
void GhostComponent::ChaseBehavior()
{
	auto& ghostTransform = GetTransform();
	auto ghostPosition = ghostTransform.GetWorldPosition();

	auto& playerTransform = CharacterControllerComponent::Instance->GetTransform();
	auto playerPosition = playerTransform.GetWorldPosition();

	float distancefromPlayer = glm::distance((glm::vec2)playerPosition, (glm::vec2)ghostPosition);

	// Move towards the player if within sight range.
	if (distancefromPlayer < sightRange)
	{
		auto destination = Math::MoveTowards((glm::vec2)ghostPosition, (glm::vec2)playerPosition, ghostSpeed * Time::GetDeltaTime());
		ghostTransform.SetWorldPosition(glm::vec3(destination, playerPosition.z));
	}
}

//--------------------------------------------------------------------------------
// Function   : patrolBehavior
// 
// Description: Defines the patrolling behavior of the ghost, moving it between two points.
//---------------------------------------------------------------------------------
void GhostComponent::PatrolBehavior()
{
	auto& ghostTransform = GetTransform();

	auto ghostPosition = ghostTransform.GetWorldPosition();
	          
	float distance = glm::distance((glm::vec2)pointA->GetTransform().GetWorldPosition(), (glm::vec2)pointB->GetTransform().GetWorldPosition());

	float progress = GetPatrolPercent();

	auto destination = Math::Lerp((glm::vec2)pointA->GetTransform().GetWorldPosition(), (glm::vec2)pointB->GetTransform().GetWorldPosition(),progress);

	ghostTransform.SetWorldPosition(glm::vec3(destination, ghostPosition.z));
}

//--------------------------------------------------------------------------------
// Function   : GetPatrolPercent
// 
// Description: Returns the percentage of the patrol path completed based on time and speed.
// 
// Outputs    : float - percentage of patrol path completed
//---------------------------------------------------------------------------------
float GhostComponent::GetPatrolPercent()
{
	float percentage = std::fmod(patrolTime, ghostSpeed * 2) / ghostSpeed;
	percentage = (glm::sin((glm::pi<float>() * percentage) - (0.5f * glm::pi<float>())) / 2) + 0.5f;
	return percentage;
}

//--------------------------------------------------------------------------------
// Function   : UpdateGhostColor
// 
// Description: updates the ghost's color and visibility based on its state and distance to the player.
//---------------------------------------------------------------------------------
void GhostComponent::UpdateGhostColor()
{
	if (isStunned)
	{
		sprite->SetEnabled(true);
		sprite->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		sprite->SetAlpha(stunAlpha);
	}
	else
	{
		auto& playerTransform = CharacterControllerComponent::Instance->GetTransform();
		auto ghostDistance = glm::distance((glm::vec2)GetTransform().GetWorldPosition(), (glm::vec2)playerTransform.GetWorldPosition());
		GetEntity()->GetComponent<GhostParticle>()->SetIsActive(false);

		// Ghost is in red range!
		if (ghostDistance <= ToolsComponent::Instance->redEmfRange)
		{
			sprite->SetEnabled(true);
			sprite->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
			sprite->SetAlpha(redAlpha);
			GetComponent<GhostParticle>()->SetIsActive(true);
		}
		// Ghost is in yellow range
		else if (ghostDistance < ToolsComponent::Instance->yellowEmfRange)
		{
			sprite->SetEnabled(false);
			sprite->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
			sprite->SetAlpha(yellowAlpha);
			GetComponent<GhostParticle>()->SetIsActive(true);
		}
		// Ghost is in the green range.
		else
		{
			sprite->SetEnabled(false);
			sprite->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
			sprite->SetAlpha(greenAlpha);
			GetComponent<GhostParticle>()->SetIsActive(false);

		}
	}
}

//--------------------------------------------------------------------------------
// Function   : Serialize
// 
// Description: calls the serialize function for the GhostComponent.
//---------------------------------------------------------------------------------
void GhostComponent::Serialize(json& j)
{
	to_json(j, *this);
}

//--------------------------------------------------------------------------------
// Function   : Deserialize
// 
// Description: Calls the deserialize function for the GhostComponent.
//---------------------------------------------------------------------------------
void GhostComponent::Deserialize(const json& j)
{
	from_json(j, *this);
}

// GhostComponent to and from JSON
void to_json(json& j, const GhostComponent& ghostComponent)
{

}

//--------------------------------------------------------------------------------
// Function   : from_json
// 
// Description: loads the GhostComponent data from a JSON object.
//---------------------------------------------------------------------------------
void from_json(const json& j, GhostComponent& ghostComponent)
{
	Serialization::DeserializeBool(j, "enabled", ghostComponent.enabled);

	Serialization::DeserializeString(j, "ghostType", ghostComponent.ghostType);

	Serialization::DeserializeFloat(j, "ghostSpeed", ghostComponent.ghostSpeed);
	Serialization::DeserializeFloat(j, "stunLifetime", ghostComponent.stunLifetime);
	
	Serialization::DeserializeFloat(j, "stunAlpha", ghostComponent.stunAlpha);
	Serialization::DeserializeFloat(j, "redAlpha", ghostComponent.redAlpha);
	Serialization::DeserializeFloat(j, "yellowAlpha", ghostComponent.yellowAlpha);
	Serialization::DeserializeFloat(j, "greenAlpha", ghostComponent.greenAlpha);

	Serialization::DeserializeFloat(j, "sightRange", ghostComponent.sightRange);

	Serialization::DeserializeEntity(j, "pointAReferenceID", ghostComponent.pointA);
	Serialization::DeserializeEntity(j, "pointBReferenceID", ghostComponent.pointB);

	ghostComponent.SetInit();
}

#ifdef _DEBUG 
//--------------------------------------------------------------------------------
// Function   : DrawInspector
// 
// Description: Draws the GhostComponent's properties in the debug inspector.
//---------------------------------------------------------------------------------
void GhostComponent::DrawInspector()
{
	if (ImGui::TreeNode("GhostComponent"))
	{
		ImGui::Checkbox("IsStunned", &isStunned);
		ImGui::DragFloat("StunLifetime", &stunLifetime);

		ImGui::DragFloat("Stun Alpha", &stunAlpha, 0.01f, 0, 1.0f);
		ImGui::DragFloat("Red Alpha", &redAlpha, 0.01f, 0, 1.0f);
		ImGui::DragFloat("Yellow Alpha", &yellowAlpha, 0.01f, 0, 1.0f);
		ImGui::DragFloat("Green Alpha", &greenAlpha, 0.01f, 0, 1.0f);

		if (ImGui::DragFloat("GhostSpeed", &ghostSpeed, 0.1f, 0.001f))
			ghostSpeed = std::max(0.001f, ghostSpeed);

		if (ghostType == "Follow")
		{
			if (ImGui::DragFloat("SightRange", &sightRange, 0.1f, 0.001f))
				sightRange = std::max(0.001f, sightRange);
		}
		else if (ghostType == "Patrol")
		{

		}

		ImGui::TreePop();
	}
}
#endif
