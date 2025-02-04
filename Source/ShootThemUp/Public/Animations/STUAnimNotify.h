// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STUAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiendSignature,USkeletalMeshComponent*);
UCLASS()

class SHOOTTHEMUP_API USTUAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    FOnNotifiendSignature OnNotifiend;
	
};
