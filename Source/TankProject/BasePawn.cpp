// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	
}


// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}
void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	
	FRotator LookAtRotation = FRotator(ToTarget.Rotation().Pitch/2.f, ToTarget.Rotation().Yaw, 0.f);
	float PitchMax = 5.f;
	if (LookAtRotation.Pitch > PitchMax)
	{
		LookAtRotation.Pitch = PitchMax;
	}
	else if(LookAtRotation.Pitch < -PitchMax)
	{
		LookAtRotation.Pitch = -PitchMax;
	}
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			5.f
		)
	);

}
// Called to bind functionality to input
void ABasePawn::Fire() 
{
	if (IsFireReady == false) return;

	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	DrawDebugSphere(
		GetWorld(), 
		Location,
		25,
		12,
		FColor::Red,
		false,
		3.f);

	AProjectile* Projectil = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,Location,Rotation);
	Projectil->SetOwner(this);
	IsFireReady = false;
	GetWorldTimerManager().SetTimer(FireCoolDownTimerHandle, this, &ABasePawn::ReadyToFire, FireCoolDown, false);
}


void ABasePawn::HandleDestruction() 
{
	if (DestroyParticles) 
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DestroyParticles, GetActorLocation(), GetActorRotation());
	}
	if (DeathSound) 
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	}

}
void ABasePawn::ReadyToFire() 
{
	IsFireReady = true;
}

