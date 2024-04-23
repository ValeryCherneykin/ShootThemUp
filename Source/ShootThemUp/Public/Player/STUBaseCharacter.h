// Shoot Them Up Game.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUHealthComponent.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    USpringArmComponent* SpringArmComponent;
    
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    USTUHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    USTUWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly,Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly,Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f,1200.0f);

    UPROPERTY(EditDefaultsOnly,Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f,100.0f);
    

    virtual void BeginPlay() override;
    

public:	
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable,Category="Movement")
    bool bIsRunning()const;

    UFUNCTION(BlueprintCallable,Category="Movement")
    float GetMovementDirection()const;
private:
    bool bWantsToRun = false;
    bool bIsMovingForward = false;
    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void OnStartRunning();
    void OnStopRunning();

    void OnDeath();
    void OnHealthChanged(float Health);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
