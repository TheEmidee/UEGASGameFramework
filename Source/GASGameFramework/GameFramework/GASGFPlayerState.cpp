#include "GASGFPlayerState.h"

#include "Characters/GASGFPawnData.h"
#include "GASGFGameMode.h"
#include "GASGameFramework/GASGFLog.h"
#include "GameFeatures/GASExtGameFeatureAction_AddAbilities.h"

#include <Components/GameFrameworkComponentManager.h>
#include <Net/UnrealNetwork.h>

AGASGFPlayerState::AGASGFPlayerState()
{
    // Set PlayerState's NetUpdateFrequency to the same as the Character.
    // Default is very low for PlayerStates and introduces perceived lag in the ability system.
    // 100 is probably way too high for a shipping game, you can adjust to fit your needs.
    NetUpdateFrequency = 100.0f;
}

void AGASGFPlayerState::AddStatTagStack( const FGameplayTag tag, const int32 stack_count )
{
    StatTags.AddStack( tag, stack_count );
}

void AGASGFPlayerState::RemoveStatTagStack( const FGameplayTag tag, const int32 stack_count )
{
    StatTags.RemoveStack( tag, stack_count );
}

int32 AGASGFPlayerState::GetStatTagStackCount( const FGameplayTag tag ) const
{
    return StatTags.GetStackCount( tag );
}

bool AGASGFPlayerState::HasStatTag( const FGameplayTag tag ) const
{
    return StatTags.ContainsTag( tag );
}

void AGASGFPlayerState::SetPawnData( const UGASGFPawnData * new_pawn_data )
{
    check( new_pawn_data );

    if ( GetLocalRole() != ROLE_Authority )
    {
        return;
    }

    if ( PawnData != nullptr )
    {
        UE_LOG( LogGASGameFramework, Error, TEXT( "Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]." ), *GetNameSafe( new_pawn_data ), *GetNameSafe( this ), *GetNameSafe( PawnData ) );
        return;
    }

    // MARK_PROPERTY_DIRTY_FROM_NAME( ThisClass, PawnData, this );
    PawnData = new_pawn_data;

    // :TODO:
    /*for ( const auto * ability_set : PawnData->AbilitySets )
    {
        if ( ability_set != nullptr )
        {
            ability_set->GiveToAbilitySystem( AbilitySystemComponent, nullptr );
        }
    }*/

    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent( this, UGASExtGameFeatureAction_AddAbilities::NAME_AbilityReady );

    ForceNetUpdate();
}

void AGASGFPlayerState::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // :TODO:
    /*check( AbilitySystemComponent );
    AbilitySystemComponent->InitAbilityActorInfo( this, GetPawn() );

    if ( GetNetMode() != NM_Client )
    {
        AGameStateBase * GameState = GetWorld()->GetGameState();
        check( GameState );
        ULyraExperienceManagerComponent * ExperienceComponent = GameState->FindComponentByClass< ULyraExperienceManagerComponent >();
        check( ExperienceComponent );
        ExperienceComponent->CallOrRegister_OnExperienceLoaded( FOnLyraExperienceLoaded::FDelegate::CreateUObject( this, &ThisClass::OnExperienceLoaded ) );
    }*/

    // :TODO: Remove when the above is uncommented
    if ( const auto * game_mode = GetWorld()->GetAuthGameMode< AGASGFGameMode >() )
    {
        if ( const auto * new_pawn_data = game_mode->GetPawnDataForController( Cast< AController >( GetOwner() ) ) )
        {
            SetPawnData( new_pawn_data );
        }
        else
        {
            UE_LOG( LogGASGameFramework, Error, TEXT( "ASWPlayerState::PostInitializeComponents(): Unable to find PawnData to initialize player state [%s]!" ), *GetNameSafe( this ) );
        }
    }
}

void AGASGFPlayerState::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
    Super::GetLifetimeReplicatedProps( OutLifetimeProps );

    FDoRepLifetimeParams params;
    params.bIsPushBased = true;

    DOREPLIFETIME_WITH_PARAMS_FAST( ThisClass, PawnData, params );
}
