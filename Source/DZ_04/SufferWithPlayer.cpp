#include "SufferWithPlayer.h"
#include "DZ_04Character.h"
#include "Kismet/GameplayStatics.h"

ASufferWithPlayer::ASufferWithPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void ASufferWithPlayer::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsOfClass(this, ADZ_04Character::StaticClass(),PlayerActors);
	for (AActor* PlayerActor:PlayerActors)
	{
		Cast<ADZ_04Character>(PlayerActor)->OnDead.AddUFunction(this, "GetDeathByPlayer");
		Cast<ADZ_04Character>(PlayerActor)->OnGetDamageByTimer.AddUFunction(this,"GetDamageByPlayer");
	}
}

void ASufferWithPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASufferWithPlayer::GetDamageByPlayer()
{
	GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Yellow, "Linked Actor gets damage too");
}

void ASufferWithPlayer::GetDeathByPlayer()
{
	GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Yellow, "Linked Actor DEAD");
	Destroy();
}

