// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable,Category="UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable,Category="UI")
    bool GetCurrentWeaponUIData (FWeaponUIData& UIData) const;

    UFUNCTION(BlueprintCallable,Category="UI")
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    UFUNCTION(BlueprintCallable,Category="UI")
    bool isPlayerAlive() const;

    UFUNCTION(BlueprintImplementableEvent,Category="UI")
    void OnTakeDamage();

    UFUNCTION(BlueprintCallable,Category="UI")
    bool isPlayerSpectating() const;

    virtual bool Initialize() override;

private:
    void OnHealthChanged(float Health,float HealthDelta);
};
