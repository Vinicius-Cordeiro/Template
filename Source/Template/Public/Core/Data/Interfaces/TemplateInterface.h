// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Kismet library useful for calculations, localizations and get classes that is necessary 
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Engine helper classes 
#include "Engine/DataTable.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"

// Custom Helpers and Libraries 

#include "TemplateInterface.generated.h"



#pragma region >>>>>>>>>>>>Utils<<<<<<<<<<
/**
 * Convert the value of an enum to a string.
 * @param EnumValue The enumerated type value to convert to a string.
 * @return The key/name that corresponds to the value in the enumerated type.
 */
template<typename T>
FString EnumToString(const T EnumValue) {
	FString Name = StaticEnum<T>()->GetNameStringByValue(static_cast<__underlying_type(T)>(EnumValue));

	check(Name.Len() != 0);

	return Name;
}

#pragma endregion >>>>>>>>>>>>Utils<<<<<<<<<<



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTemplateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEMPLATE_API ITemplateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/** Get the default collision trace param 
	*	@param IgnoredActor - Actor to be ignored by the trace collision
	*	@param bIgnoreActorChildren - If the children of the ignored actor should be ignore as well 
	*	@param bTraceComplex - If the trace should be performed against complex geometry as well as simple 
	*	@param bReturnPhysMaterials - If the trace should return the physical material information from the hit actor, usefull for footsteps traces, bullet collisions traces.. 
	*	@param MobilityType - Type of mobility the trace will be looking for, default to any mobility type.. 
	*	@return Default Collision Query Param
	*/
	FORCEINLINE static FCollisionQueryParams GetCollisionQueryParams(const AActor* IgnoredActor = nullptr , bool bIgnoreActorChildren = false , bool bTraceComplex = false , bool bReturnPhysMaterials = false , EQueryMobilityType MobilityType = EQueryMobilityType::Any) {
		FCollisionQueryParams QueryParam;
		QueryParam.MobilityType = MobilityType;
		QueryParam.bTraceComplex = bTraceComplex ;
		QueryParam.bReturnPhysicalMaterial = bReturnPhysMaterials;
		
		if (!IgnoredActor) return QueryParam;
		QueryParam.AddIgnoredActor(IgnoredActor);
		
		if (!bIgnoreActorChildren)  return QueryParam;
		TArray<UPrimitiveComponent*> ChildrenComponents;
		IgnoredActor->GetComponents(ChildrenComponents , true);
		QueryParam.AddIgnoredComponents(ChildrenComponents);
		return QueryParam;
	}
};
