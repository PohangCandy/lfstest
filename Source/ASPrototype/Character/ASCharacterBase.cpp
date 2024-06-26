// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterBase.h"
#include "Player/ASPlayerController.h"
#include "Character/ASCharacterStatComponent.h"
//�浹,������ �������
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//AnimInstanceŬ������ ����
#include "Animation/ASAnimInstance.h"

//사운드 관련
#include "Components/AudioComponent.h"

// Sets default values
AASCharacterBase::AASCharacterBase()
{
	//Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCapsule"));

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharaterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (CharaterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharaterMeshRef.Object);
	}

	//  /Script/Engine.AnimBlueprint'/Game/ASPrototype/Animation/ABP_CharacterAnimation.ABP_CharacterAnimation'
	//static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ASPrototype/Animation/ABP_CharacterAnimation.ABP_CharacterAnimation_C"));
	////ensure(AnimInstanceClassRef.Class);
	//if (AnimInstanceClassRef.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	//}

	Curstrength = 10;
	MaxHp = 100;
	CurHp = MaxHp;
	MaxBulletNum = 8;
	CurBulletNum = MaxBulletNum;
	MaxMagazineNum = 24;
	CurMagazineNum = MaxMagazineNum;
	MaxItemNum = 20;
	CurItemNum = MaxItemNum;
	LowHp = 40;
	Damage = 10;
	CurState = State::None;
	MaxMagnification = 16;
	CurMagnification = 0;
	

	//static ConstructorHelpers::FClassFinder<AASPlayerController> Controll_C(TEXT("/Script/ASPrototype.ASPlayerController_C"));
	//CurplayerControllerClass = Controll_C.Class;
	//PlayerController = Cast<AASPlayerController>(GetWorld()->GetFirstPlayerController());
	//PlayerController->ConnectUIwithData();

	CharacterStat = CreateDefaultSubobject<UASCharacterStatComponent>(TEXT("CHARACTERSTAT"));


	//Walk Sound 
	static ConstructorHelpers::FObjectFinder<USoundBase> WalkSoundRef(TEXT("/Script/Engine.SoundCue'/Game/ASPrototype/Sound/WalkSound_Cue.WalkSound_Cue'"));
	if (WalkSoundRef.Object)
	{
		WalkSound = WalkSoundRef.Object;
		ensure(WalkSound);
	}

	//Shoot Sound 
	static ConstructorHelpers::FObjectFinder<USoundBase> ShootSoundRef(TEXT("/Script/Engine.SoundCue'/Game/ASPrototype/Sound/PlayerSniper_Sound_Cue.PlayerSniper_Sound_Cue'"));
	if (ShootSoundRef.Object)
	{
		ShootSound = ShootSoundRef.Object;
		ensure(ShootSound);
	}

}

void AASCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//OnHpChanged.Broadcast();
	AnimInstance = GetMesh()->GetAnimInstance();
}

void AASCharacterBase::SetDead()
{	
	SetState(State::Dead);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);

	//Dead Animation
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage);

	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), 2.0f, false);
	UGameplayStatics::OpenLevel(this, TEXT("Defeat"));
}

int AASCharacterBase::GetHp()
{
	return CurHp;
}

int AASCharacterBase::GetBulletNum()
{
	UE_LOG(AS, Warning, TEXT("GetBulletNumFunc Act"));
	return CurBulletNum;
	//return 3;
}

int AASCharacterBase::GetMagazineNum()
{
	UE_LOG(AS, Warning, TEXT("GetMagazineNumFunc Act"));
	return CurMagazineNum;
}

int AASCharacterBase::GetItemNum()
{
	return CurItemNum;
}

int AASCharacterBase::GetReloadableBullet()
{
	if (GetMagazineNum() > 0)
	{
		if (GetMagazineNum() - (MaxBulletNum - GetBulletNum()) < 0)
		{
			return GetMagazineNum();
		}
		else
		{
			return MaxBulletNum - GetBulletNum();
		}
	}
	return 0;
}

int AASCharacterBase::GetStrength()
{
	return Curstrength;
}

float AASCharacterBase::GetHpratio()
{
	if (CurHp <= 0)
	{
		return 0.0f;
	}
	float HpRatio = (float)CurHp / (float)MaxHp;
	return HpRatio;
}

void AASCharacterBase::SetHp(int Hp)
{
	CurHp = Hp;
	OnHpChanged.Broadcast();
}

void AASCharacterBase::SetBulletNum(int Num)
{
	CurBulletNum = Num;
	NumBulletChanged.Broadcast();
}

void AASCharacterBase::SetMagazineNum(int Num)
{
	CurMagazineNum = Num;
	NumMagazineChanged.Broadcast();
}

void AASCharacterBase::SetItemNum(int Num)
{
	CurItemNum = Num;
	NumItemChanged.Broadcast();
}

void AASCharacterBase::GetDamaged(int damage)
{
	int Hp = GetHp() - damage;
	if (Hp > 0)
	{
		SetHp(Hp);
	}
	else {
		SetDead();
	}

	//OnHpChanged.Broadcast();
}

void AASCharacterBase::SetStrength(int newStrength)
{
	Curstrength = newStrength;
}

bool AASCharacterBase::AttackCheck()
{

	return false;
}


void AASCharacterBase::SetState(State NewState)
{
	CurState = NewState;
	UASAnimInstance* animinstance = Cast<UASAnimInstance>(GetMesh()->GetAnimInstance());
	animinstance->StateHandler(NewState);
}

void AASCharacterBase::ConsumeBullet()
{
	SetBulletNum(CurBulletNum - 1);
	//NumBulletChanged.Broadcast();
}

void AASCharacterBase::RechargeBullet()
{
	//int lastMagazineNum = GetMagazineNum();
	//int i = 0;
	//if (lastMagazineNum > 0)
	//{	
	//	//UE_LOG(AS, Warning, TEXT("Reload fuc Start"));
	//	int ReloadableBulletNum = MaxBulletNum - GetBulletNum();
	//	//GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, FString::Printf(TEXT("bullet : %d"), ReloadableBulletNum));
	//	//GEngine->AddOnScreenDebugMessage(-1,4.0f,FColor::Red, FString::Printf(TEXT("bullet : %d"), lastMagazineNum - (MaxBulletNum - GetBulletNum())));
	//	//if (lastMagazineNum - (MaxBulletNum - GetBulletNum()) < 0)
	//	//if ((lastMagazineNum - (MaxBulletNum - GetBulletNum())) < 0)
	//	auto PlayerAnimInstance = Cast<UASAnimInstance>(GetMesh()->GetAnimInstance());
	//	if (PlayerAnimInstance != nullptr)
	//	{
	//		PlayerAnimInstance->PlaySniperRifle_Zoom_ReloadMontage();
	//	}
	//	if (lastMagazineNum - ReloadableBulletNum < 0)
	//	{
	//		//UE_LOG(AS, Warning, TEXT("Reload if Start"));
	//		SetBulletNum(GetBulletNum() + lastMagazineNum);
	//		SetMagazineNum(0);
	//		//UE_LOG(AS, Warning,TEXT("Set lastMagazineNum zero"));
	//	}
	//	else
	//	{
	//		//UE_LOG(AS, Warning, TEXT("Reload else Start"));
	//		lastMagazineNum = GetMagazineNum() - ReloadableBulletNum;
	//		SetMagazineNum(lastMagazineNum);
	//		SetBulletNum(MaxBulletNum);
	//	}

	//	//GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, FString::Printf(TEXT("bullet : %d"), i));
	//}

	CurBulletNum += GetReloadableBullet();
	CurMagazineNum -= GetReloadableBullet();

	NumBulletChanged.Broadcast();
	NumMagazineChanged.Broadcast();
}

void AASCharacterBase::Heal()
{
	int lastItemNum = GetItemNum();
	if (lastItemNum > 0)
	{
		if (GetHp() < MaxHp)
		{
			SetItemNum(lastItemNum - 1);
			if (GetHp() + 15 > MaxHp)
			{
				SetHp(MaxHp);
			}
			else
			{
				SetHp(GetHp() + 15);
			}
			
		}

	}
	NumItemChanged.Broadcast();
}

void AASCharacterBase::TestingGetDamage()
{
	GetDamaged(10);
}

void AASCharacterBase::InitUIData()
{
	OnHpChanged.Broadcast();
	NumBulletChanged.Broadcast();
	NumMagazineChanged.Broadcast();
	NumItemChanged.Broadcast();
	NumMagnificationChanged.Broadcast();
}


State AASCharacterBase::GetState()
{
	return CurState;
}

int AASCharacterBase::GetMagnificationNum()
{
	return CurMagnification;
}
float AASCharacterBase::GetMagnificationratio()
{
	if ( CurMagnification <= 0)
	{
		return 0.0f;
	}
	float MagnificationRatio = (float)CurMagnification / (float)MaxMagnification;
	return MagnificationRatio;
}

void AASCharacterBase::SetMagnificationNum(int newmag)
{
	CurMagnification = newmag;
}

void AASCharacterBase::ZoomOut()
{
	UE_LOG(AS, Warning, TEXT("ZoomOut Act"));
	UE_LOG(AS, Warning, TEXT("%d"),CurMagnification);
	if (GetMagnificationNum() > 0)
	{
		SetMagnificationNum(GetMagnificationNum() - 1);
	}
	NumMagnificationChanged.Broadcast();
}

void AASCharacterBase::ZoomIn()
{
	UE_LOG(AS, Warning, TEXT("ZoomIn Act"));
	UE_LOG(AS, Warning, TEXT("%d"), CurMagnification);
	if (GetMagnificationNum() < MaxMagnification)
	{
		SetMagnificationNum(GetMagnificationNum() + 1);
	}
	NumMagnificationChanged.Broadcast();
}



void AASCharacterBase::PlaySound(USoundBase* sound)
{
	UGameplayStatics::PlaySoundAtLocation(this, sound, GetActorLocation());
}

bool AASCharacterBase::CanFire()
{
	auto ASAnimInstance = Cast<UASAnimInstance>(GetMesh()->GetAnimInstance());
	bool canFire;
	if (ASAnimInstance != nullptr)
	{
		canFire = ASAnimInstance->CanPlayFireMontage();
	}
	return CurBulletNum > 0 && canFire;
}

void AASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	InitUIData();
}



void AASCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
