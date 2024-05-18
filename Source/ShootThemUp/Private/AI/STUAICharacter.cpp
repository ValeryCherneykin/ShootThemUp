// Shoot Them Up Game.All Rights Reserved.


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "Components/STUCharacterMovementComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit): Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;
    
    if(CharacterMovementComponent)
    {
        CharacterMovementComponent->bUseControllerDesiredRotation = true;
        CharacterMovementComponent->RotationRate = FRotator(0.0f,200.0f,0.0f);
    }
}
