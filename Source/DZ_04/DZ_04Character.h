// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "DZ_04Character.generated.h"

DECLARE_EVENT(DZ_04Character, FOnGetDamageByTimer)
DECLARE_EVENT(DZ_04Character, FOnDead)


UCLASS(config=Game)
class ADZ_04Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	public:
	ADZ_04Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;



public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	UPROPERTY(BlueprintReadWrite)
	int Health;

	UPROPERTY(EditAnywhere)
	AActor* HealObject;

	UPROPERTY(EditAnywhere)
	AActor* DamageObject;

	UPROPERTY()
	bool IsDamageTicking;

	UPROPERTY()
	bool IsHealTicking;

	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CollisionBox;
	

	UPROPERTY()
	USphereComponent* SphereCollision;
	
	
	FTimerHandle HealTimerHandle;
	FTimerHandle DamageTimerHandle;

	UFUNCTION()
	void ModifyHealth(int ModifierValue);

	UFUNCTION()
	void GetDamageByTimer();

	UFUNCTION()
	void GetHealByTimer();

	UFUNCTION()
	void GetDeath();

	UFUNCTION(BlueprintCallable)
	void PrintMsg();

	
	UFUNCTION()
	void OnActorBeginOverlapFunc(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnComponentHitFunc(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnComponentBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	
	FOnGetDamageByTimer OnGetDamageByTimer;
	FOnDead OnDead;

// Just the Test for Git
	//New Branch

	// NEw Pull req
	
};

