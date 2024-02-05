// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"


UCLASS()
class TANKPROJECT_API ATank : public ABasePawn
{
	GENERATED_BODY()
	

public:
	ATank();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }
	UPROPERTY(EditAnywhere, Category = "Death",BlueprintReadWrite)
	bool bAlive = true;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere,Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 200.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnSpeed = 50.f;

	void Move(float Value);
	void Rotation(float Value);

	APlayerController* TankPlayerController;

	UPROPERTY(EditAnywhere,Category = "Sound")
	class USoundBase* EngineSound;
	FTimerHandle EngineSoundTimerHandle;
	UPROPERTY(EditAnywhere, Category = "Sound")
	float EnigneSoundTime =34.f;
	int32 EngineSoundNum = 0;
	void PlayEngineSound();

};
