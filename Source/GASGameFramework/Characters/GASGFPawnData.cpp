#include "GASGFPawnData.h"

#include "DVEDataValidator.h"

#if WITH_EDITOR
EDataValidationResult UGASGFPawnData::IsDataValid( TArray< FText > & validation_errors )
{
    return FDVEDataValidator( validation_errors )
        .NotNull( VALIDATOR_GET_PROPERTY( PawnClass ) )
        .NotNull( VALIDATOR_GET_PROPERTY( TagRelationshipMapping ) )
        .Result();
}
#endif