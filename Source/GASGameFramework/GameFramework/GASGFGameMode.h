#pragma once

#include "ModularGameMode.h"

#include <CoreMinimal.h>
#include <GameplayTagContainer.h>

#include "GASGFGameMode.generated.h"

class UGASGFPawnData;
UCLASS()
class GASGAMEFRAMEWORK_API AGASGFGameMode : public AModularGameMode
{
    GENERATED_BODY()

public:

    const UGASGFPawnData * GetPawnDataForController( const AController * controller ) const;
};
