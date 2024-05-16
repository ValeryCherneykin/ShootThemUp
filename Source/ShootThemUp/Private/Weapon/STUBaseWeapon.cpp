// Shoot Them Up Game.All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"



DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon,All,All)

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);
    
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
    check(WeaponMesh)
    checkf(DefaultAmao.Bullets>0,TEXT("Bullets count couldn't be less or equal zero"));
    checkf(DefaultAmao.Clips>0,TEXT("Clips count couldn't be less or equal zero"));
    CurrentAmmo = DefaultAmao;
}

void ASTUBaseWeapon::MakeShot(){}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if(CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;
    if(IsAmmoEmpty())
    {
        UE_LOG(LogBaseWeapon, Display,TEXT("Ammo was Empty!"));
        CurrentAmmo.Clips = FMath::Clamp(ClipsAmount,0,DefaultAmao.Clips + 1);
        OnClipEmpty.Broadcast(this);
    }
    else if(CurrentAmmo.Clips < DefaultAmao.Clips)
    {
        const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
        if(DefaultAmao.Clips - NextClipsAmount >= 0)
        {
            CurrentAmmo.Clips = NextClipsAmount;
            UE_LOG(LogBaseWeapon, Display,TEXT("Ammo were added!"));
        } else
        {
            CurrentAmmo.Clips = DefaultAmao.Clips;
            CurrentAmmo.Bullets = DefaultAmao.Bullets;
            UE_LOG(LogBaseWeapon, Display,TEXT("Ammo is Full now"));
        }
            
    } else
    {
        CurrentAmmo.Bullets = DefaultAmao.Bullets;
        UE_LOG(LogBaseWeapon, Display,TEXT("Bullets were added"));
    }
    return true;
}

void ASTUBaseWeapon::StartFire(){}

void ASTUBaseWeapon::StopFire(){}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if(!Player) return nullptr;

    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLoacation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();
    if(!Controller) return false;

    Controller->GetPlayerViewPoint(ViewLoacation,ViewRotation);
    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLoacation;
    FRotator ViewRotation;
    if(!GetPlayerViewPoint(ViewLoacation,ViewRotation)) return false;

    TraceStart = ViewLoacation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTUBaseWeapon:: MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if(!GetWorld()) return;
    
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;
    
    GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd,ECC_Visibility,CollisionParams);
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    if(CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Warning , TEXT("Clip is empty"))
        return;
    }
    CurrentAmmo.Bullets --;
    LogAmmo();
    if(IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast(this);
    }
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.Clips == DefaultAmao.Clips && //
        CurrentAmmo.Bullets == DefaultAmao.Bullets;
}

void ASTUBaseWeapon::ChangeClip()
{
    if(!CurrentAmmo.Infinite)
    {
        if(CurrentAmmo.Clips == 0)
        {
            UE_LOG(LogBaseWeapon, Warning , TEXT("No more clips"))
            return;
        }
        CurrentAmmo.Clips--;
    }
    CurrentAmmo.Bullets = DefaultAmao.Bullets;
    UE_LOG(LogBaseWeapon,Display,TEXT(" ----------Change Clip------------"));
}

bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmao.Bullets && CurrentAmmo.Clips>0;
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + "/";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogBaseWeapon,Display,TEXT("%s"),*AmmoInfo);
}
