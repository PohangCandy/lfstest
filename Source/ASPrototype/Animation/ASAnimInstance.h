// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/ASGameMode.h"
#include "Animation/AnimInstance.h"
#include "Character/ASCharacterBase.h" //의존
#include "ASAnimInstance.generated.h"
/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API UASAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UASAnimInstance();
	State state;
	void StateHandler(State NewState);

	void SwitchSnipAnim();

	void PlaySniperRifle_AttackMontage();

	void PlaySniperRifle_ReloadMontage();

	void Play_Hurt_ByGun_Montage();

	bool CanPlayFireMontage();
	bool CanPlayReloadMontage();

	UAnimMontage* GetCurFireMontage();
	void SetFireMontage(UAnimMontage* newMontage);

	void SetReloadMontage(UAnimMontage* newMontage);

private:
	UAnimMontage* SniperRifle_Basic_FireMontage;
	UAnimMontage* SniperRifle_Basic_ReloadMontage;
	UAnimMontage* SniperRifle_Zoom_FireMontage;
	UAnimMontage* SniperRifle_Zoom_ReloadMontage;
	UAnimMontage* Hurt_ByGun_Montage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Montage,Meta = (AllowPrivateAccess = true))
	UAnimMontage* CurFireMontage;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
	UAnimMontage* CurReloadMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
	UAnimMontage* CurHurtMontage;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner; //Animinstance를 사용하고 있는 케릭터에 대한 정보 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bIsIdle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MoveThreadshold; //임계값 (특정수치가 올라가면 움직임)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bIsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bIsHurt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpeThreadshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool DoSniping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector OwnerLocation;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	//class AASCharacterBase* Actor;

	//trigger 참고 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FRotator ControlRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float LookAtPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float LookAtYaw;
};
