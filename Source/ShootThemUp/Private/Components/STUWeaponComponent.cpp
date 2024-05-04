// Shoot Them Up Game.All Rights Reserved.


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent,All,All);

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::StartFire()
{
    if(!CanFire()) return;
    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if(!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
    if(!CanEquip()) return;
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::Reload()
{
    ChangeClip();
}


void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentWeaponIndex = 0;
    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex); 
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for(auto Weapon : Weapons)
    {
        Weapon -> DetachFromActor(FDetachmentTransformRules:: KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();
    
    Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    SpawnWeapons();
    
}

void USTUWeaponComponent::SpawnWeapons()
{
    if(!GetWorld()) return;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character || !GetWorld()) return;

    
    for(auto OneWeaponData : WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if(!Weapon) continue;

        Weapon -> OnClipEmpty.AddUObject(this,&USTUWeaponComponent::OnEmptyClip);
        Weapon -> SetOwner(Character);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon,Character->GetMesh(), WeaponArmorySocketName);
    }
    
    //CurrentWeapon->SetOwner(Character);
} 
void USTUWeaponComponent:: AttachWeaponToSocket(ASTUBaseWeapon* Weapon,USceneComponent* SceneComponent, const FName& SocketName)
{
    if(!Weapon || !SceneComponent) return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,false);
    Weapon->AttachToComponent(SceneComponent,AttachmentRules,SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if(WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent,Warning,TEXT("Invalid weapom index"));
        return;
    }
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character) return;

    if(CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon,Character->GetMesh(), WeaponArmorySocketName);
    }
    
    CurrentWeapon = Weapons[WeaponIndex];
    //CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data)//
    {
        return Data.WeaponClass == CurrentWeapon->GetClass();                                       //
    });
    CurrentReloadAnimMontage = CurrentWeapon ? CurrentWeaponData->ReloadAnimMontage:nullptr;
    AttachWeaponToSocket(CurrentWeapon,Character->GetMesh(),WeaponEquipSocketName);
    EquipAnimInprogress = true;
    PlayAnimonatage(EquipAnimMontage);
}

void USTUWeaponComponent::PlayAnimonatage(UAnimMontage* Animation)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character) return;

    Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations()
{
    auto EquipFinishedNotify = FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
    if(EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotifiend.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }

    for(auto OneWeaponData: WeaponData)
    {
        auto ReloadFinishedNotify = FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if(!ReloadFinishedNotify) continue; 
        
            ReloadFinishedNotify->OnNotifiend.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    }
}

void USTUWeaponComponent:: OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character || Character->GetMesh() != MeshComponent) return;

    EquipAnimInprogress = false;
}

void USTUWeaponComponent:: OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character || Character->GetMesh() != MeshComponent) return;

   ReloadAnimInprogress = false;
}

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !EquipAnimInprogress && !ReloadAnimInprogress;
}

bool USTUWeaponComponent::CanReload() const
{
    return CurrentWeapon              //
    &&!EquipAnimInprogress           //
    &&!ReloadAnimInprogress         //
    && CurrentWeapon->CanReload(); //
}

void USTUWeaponComponent::OnEmptyClip()
{
    ChangeClip();
}

void USTUWeaponComponent::ChangeClip()
{
    if(!CanReload()) return;
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();
    ReloadAnimInprogress = true;
    PlayAnimonatage(CurrentReloadAnimMontage);
}

bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInprogress && !ReloadAnimInprogress;
}
