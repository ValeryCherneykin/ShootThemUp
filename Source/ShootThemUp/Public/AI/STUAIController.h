// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STUAIController.generated.h"

class USTUAIPerceptionComponent;
class USTURespawnComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
	GENERATED_BODY()

public:
    ASTUAIController();
    
protected:
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    USTUAIPerceptionComponent* STUAIPerceptionComponet;
    
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    USTURespawnComponent* RespawnComponent;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
    FName FocusOnKeyName = "EnemyActor";
    
    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaSeconds) override;

private:
    AActor* GetFocusOnActor() const;
    
	
};
