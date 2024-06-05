// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ASAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UASAnimInstance::UASAnimInstance()
{
	MoveThreadshold = 3.0f;
	JumpeThreadshold = 30.0f;
	DoSniping = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/ASPrototype/Animation/Zoom_SniperRifle_AttackMontage.Zoom_SniperRifle_AttackMontage"));
    //static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/ASPrototype/Animation/PlayerAttackMontage.PlayerAttackMontage"));
	///Script/Engine.AnimMontage'/Game/ASPrototype/Animation/Zoom_SniperRifle_AttackMontage.Zoom_SniperRifle_AttackMontage'
	if (ATTACK_MONTAGE.Succeeded())
	{
		SniperRifle_Zoom_FireMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RELOAD_MONTAGE(TEXT("/Game/ASPrototype/Animation/Zoom_SniperRifle_ReloadMontage.Zoom_SniperRifle_ReloadMontage"));
	///Script/Engine.AnimMontage'/Game/ASPrototype/Animation/Zoom_SniperRifle_ReloadMontage.Zoom_SniperRifle_ReloadMontage'
	if (RELOAD_MONTAGE.Succeeded())
	{
		SniperRifle_Zoom_ReloadMontage = RELOAD_MONTAGE.Object;
	}


	static ConstructorHelpers::FObjectFinder<UAnimMontage> SNIPERRIFLE_BASIC_FIREMONTAGE(TEXT("/Game/ASPrototype/Animation/SniperRifle_Basic_FireMontage.SniperRifle_Basic_FireMontage"));
	if (SNIPERRIFLE_BASIC_FIREMONTAGE.Succeeded())
	{
		SniperRifle_Basic_FireMontage = SNIPERRIFLE_BASIC_FIREMONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SNIPERRIFLE_BASIC_RELOADMONTAGE(TEXT("/Game/ASPrototype/Animation/SniperRifle_Basic_ReloadMontage.SniperRifle_Basic_ReloadMontage"));
	if (SNIPERRIFLE_BASIC_RELOADMONTAGE.Succeeded())
	{
		SniperRifle_Basic_ReloadMontage = SNIPERRIFLE_BASIC_RELOADMONTAGE.Object;
	}

	CurFireMontage = SniperRifle_Basic_FireMontage;
	CurReloadMontage = SniperRifle_Basic_ReloadMontage;

}

void UASAnimInstance::StateHandler(State NewState)
{	
	switch (NewState)
	{
	case State::None:
		break;
	case State::Prone:
		break;
	case State::Crawl:
		break;
	case State::Hurt:
		bIsHurt = 1;
		UE_LOG(LogTemp, Log, TEXT("HURT"));
	case State::Hidden:
		break;
	case State::Dead:
		break;
	default:
		break;
	}
}

void UASAnimInstance::SwitchSnipAnim()
{
	if (DoSniping)
	{
		DoSniping = false;
		SetFireMontage(SniperRifle_Basic_FireMontage);
		SetReloadMontage(SniperRifle_Basic_ReloadMontage);
	}
	else
	{
		DoSniping = true;
		SetFireMontage(SniperRifle_Zoom_FireMontage);
		SetReloadMontage(SniperRifle_Zoom_ReloadMontage);
	}
}

void UASAnimInstance::PlaySniperRifle_Zoom_AttackMontage()
{
	Montage_Play(CurFireMontage, 1.0f);
}

void UASAnimInstance::PlaySniperRifle_Zoom_ReloadMontage()
{
	if (!Montage_IsPlaying(SniperRifle_Zoom_ReloadMontage))
	{
		Montage_Play(SniperRifle_Zoom_ReloadMontage, 1.0f);
	}
}

bool UASAnimInstance::CanPlayFireMontage()
{
	return !Montage_IsPlaying(CurFireMontage);
}

bool UASAnimInstance::CanPlayReloadMontage()
{
	return !Montage_IsPlaying(CurReloadMontage);
}

UAnimMontage* UASAnimInstance::GetCurFireMontage()
{
	return CurFireMontage;
}

void UASAnimInstance::SetFireMontage(UAnimMontage* newMontage)
{
	CurFireMontage = newMontage;
}

void UASAnimInstance::SetReloadMontage(UAnimMontage* newMontage)
{
	CurReloadMontage = newMontage;
}

//임시 함수
//void UASAnimInstance::SetCurrentHp(int NewHp)
//{
//	Hp = NewHp;
//}

void UASAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	//Actor = Cast<AASCharacterBase>(Owner);
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}

	//OnHpChanged.AddUObject(this, &UASAnimInstance::SwitchSnipAnim);
}

void UASAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		GroundSpeed = Movement->Velocity.Size2D(); 
		bIsIdle = GroundSpeed < MoveThreadshold;

		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling && (Movement->Velocity.Z > JumpeThreadshold);

		bIsRunning = (Owner->GetCharacterMovement()->MaxWalkSpeed > 500) && !bIsIdle && !bIsJumping;

		OwnerLocation = Owner->GetActorLocation();

		//bIsHurt = Hp < 50;
		//UE_LOG(AS, Log, TEXT("Character Location :: %s"), Owner->GetActorLocation().ToString());
		Owner = Cast<ACharacter>(GetOwningActor());
		AController* OwningController = Owner->GetController();
		if (OwningController)
		{
			ControlRotation = OwningController->GetControlRotation();
			//FRotator ActorRotation = Owner->GetActorRotation();

			//LookAtYaw = FRotator::NormalizeAxis(ControlRotation.Yaw - ActorRotation.Yaw);
			LookAtPitch = ControlRotation.Pitch - 280.0f;
			//UE_LOG(AS, Warning, TEXT("Pitch : %f"), ControlRotation.Pitch);

		}
	}
}


