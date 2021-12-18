// Copyright Epic Games, Inc. All Rights Reserved.

#include "DZ_04Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ADZ_04Character

ADZ_04Character::ADZ_04Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	// 

	
	CollisionBox=CreateDefaultSubobject<UStaticMeshComponent>("BoxCollision");
	CollisionBox->SetupAttachment(GetRootComponent());
	
	//SphereCollision->SetSphereRadius(250.f);
	//SphereCollision->SetHiddenInGame(false);
	//SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::ADZ_04Character::OnComponentBeginOverlapFunc);
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::ADZ_04Character::OnComponentBeginOverlapFunc);
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADZ_04Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADZ_04Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADZ_04Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADZ_04Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADZ_04Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADZ_04Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADZ_04Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ADZ_04Character::OnResetVR);
}


void ADZ_04Character::OnResetVR()
{
	// If DZ_04 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in DZ_04.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}



void ADZ_04Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ADZ_04Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ADZ_04Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADZ_04Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADZ_04Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADZ_04Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void ADZ_04Character::BeginPlay()
{
	Super::BeginPlay();
	Health=10;
	OnActorBeginOverlap.AddDynamic(this, &ADZ_04Character::OnActorBeginOverlapFunc);
	Delegate.AddUFunction(this, "PrintMsg");
	Delegate.AddUFunction(this, "PrintMsg2");
	DynamicDelegate.BindUFunction(this,"PrintMsg2");
	DynamicDelegate.BindUFunction(this,"PrintMsg");
}



void ADZ_04Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	
	if(IsOverlappingActor(DamageObject) && !IsDamageTicking)
	{
		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ThisClass::GetDamageByTimer, 1.f, true, 0.f);
		IsDamageTicking=true;
	}
	if (!IsOverlappingActor(DamageObject))
	{
		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
		IsDamageTicking=false;
	}

	if(IsOverlappingActor(HealObject) && !IsHealTicking)
	{
		GetWorldTimerManager().SetTimer(HealTimerHandle, this, &ThisClass::GetHealByTimer, .5f, true, 0.f);
		IsHealTicking=true;
	}
	if (!IsOverlappingActor(HealObject))
	{
		GetWorldTimerManager().ClearTimer(HealTimerHandle);
		IsHealTicking=false;
	}
	*/
}




void ADZ_04Character::ModifyHealth(int ModifierValue)
{
	Health+=ModifierValue;
}

void ADZ_04Character::GetDamageByTimer()
{
	ModifyHealth(-2);
	GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Red, "Health= " + FString::FromInt(Health));
	OnGetDamageByTimer.Broadcast();
	if (Health<=0) GetDeath();
}

void ADZ_04Character::GetHealByTimer()
{
	ModifyHealth(1);
	GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Green, "Health= " + FString::FromInt(Health));
}

void ADZ_04Character::GetDeath()
{
	OnDead.Broadcast();
	Destroy();
}

void ADZ_04Character::PrintMsg(FString Msg)
{
	GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Purple, Msg);
}

void ADZ_04Character::PrintMsg2(FString Msg)
{
	GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Red, Msg);
}

void ADZ_04Character::OnActorBeginOverlapFunc(AActor* OverlappedActor, AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Green, "OnActorBeginOverlap");
}

void ADZ_04Character::OnComponentBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Purple, "OnComponentBeginOverlap");
	Delegate.Broadcast("Hello");
	DynamicDelegate.Execute("DYNAMIC");
}

void ADZ_04Character::OnComponentHitFunc(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Purple, "OnComponentBeginOverlap");
}