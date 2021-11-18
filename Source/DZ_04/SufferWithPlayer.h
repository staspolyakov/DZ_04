#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SufferWithPlayer.generated.h"

UCLASS()
class DZ_04_API ASufferWithPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	ASufferWithPlayer();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GetDamageByPlayer();
	
	UFUNCTION(BlueprintCallable)
	void GetDeathByPlayer();
	
};
