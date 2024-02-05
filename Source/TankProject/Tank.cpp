// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
//#include "Blueprint/WidgetBlueprintLibrary.h"
//#include "Blueprint/WidgetBlueprintLibrary.h"
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArm);

}
void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());
	if (EngineSound) 
	{
		UGameplayStatics::PlaySoundAtLocation(this, EngineSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f,2.f);
		GetWorldTimerManager().SetTimer(EngineSoundTimerHandle, this, &ATank::PlayEngineSound, EnigneSoundTime, true);
	}
	
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,
			false,
			HitResult);
		RotateTurret(HitResult.ImpactPoint);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint
			, 25
			, 12,
			FColor::Red,
			false,
			-1.f);
		
	}
	
}
void ATank::Move(float Value) 
{
	FVector DeltalLocation = FVector::ZeroVector;
	DeltalLocation.X = Value* MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltalLocation,true);
}
void ATank::Rotation(float Value)
{
	FRotator Q = FRotator::ZeroRotator;
	Q.Yaw = Value* TurnSpeed *UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(Q, true);
}
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Rotation);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}
void ATank::HandleDestruction() 
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
	
}

void ATank::PlayEngineSound() 
{
	UGameplayStatics::PlaySoundAtLocation(this, EngineSound, GetActorLocation(), GetActorRotation(), 1.f, 1.f, 10.f);
}

