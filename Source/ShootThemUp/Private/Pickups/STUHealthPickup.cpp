// Shoot Them Up Game.All Rights Reserved.


#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup,All,All);

bool ASTUHealthPickup::GivePickupTrue(APawn* PlayerPawn)
{

   const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
    if(!HealthComponent) return false;

    return HealthComponent -> TryToAddHealth(HealthAmount);
}
