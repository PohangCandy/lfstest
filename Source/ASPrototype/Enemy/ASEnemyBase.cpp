// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ASEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

//BB정보 얻기 위해 
#include "Interface/GetSetBlackBoardDataInterface.h"
#include "AI/ASAIController.h"
#include "AI/ASAI.h"

//UI
#include "UI/ASDetectWidget.h"
#include "UI/ASWidgetComponent.h"

//제거 예정 
#include "Tool/ASWeaponData.h"
#include "Tool/ASWeaponItem.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"

#include "Perception/AISense_Touch.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ASItemBox.h"
#include "Animation/ASAIAnimInstance.h"

//탐지 기능
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h" //시야
#include "Perception/AISenseConfig_Hearing.h" //사운드
#include "Perception/AISense_Touch.h"//감각
#include "Perception/AISenseConfig_Touch.h"//감각
#include "Interface/ASCharacterInterface.h"
#include "Kismet/KismetMathLibrary.h" //charactor moving

//네비게이션
#include "NavigationSystem.h"

// Sets default values
AASEnemyBase::AASEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ASEnemy"));

	//Speed
	WalkSpeed = 300.0f;
	RunSpeed = 500.0f;

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	//GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//위젯 2개 생성 이슈->컴포넌트UI 숨기기 해결 
	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AASEnemyBase::OnHit);


	//Widget
	DetectBar = CreateDefaultSubobject<UASWidgetComponent>(TEXT("DetectWidget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> DetectBarRef(TEXT("/Game/UI/WB_DetectBar_UI.WB_DetectBar_UI_C"));
	if (DetectBarRef.Class)
	{
		DetectBar->SetWidgetClass(DetectBarRef.Class);
		DetectBar->SetWidgetSpace(EWidgetSpace::Screen);
		DetectBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetectBar->SetHiddenInGame(true);
	}


	QuestionMark = CreateDefaultSubobject<UASWidgetComponent>(TEXT("QuestionMarkWidget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> QuestionMarkRef(TEXT("/Game/UI/WB_QuestionMark_UI.WB_QuestionMark_UI_C"));
	ensure(QuestionMarkRef.Class);
	if (QuestionMarkRef.Class)
	{
		QuestionMark->SetWidgetClass(QuestionMarkRef.Class);
		QuestionMark->SetWidgetSpace(EWidgetSpace::Screen);
		QuestionMark->SetDrawSize(FVector2D(30.0f, 30.0f));
		QuestionMark->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		QuestionMark->SetupAttachment(GetMesh());
		QuestionMark->AddRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
		QuestionMark->SetRelativeLocation(FVector(0.0f, 0.0f, 210.0f));
		QuestionMark->SetHiddenInGame(true);
	}

	SetupPerception();

	//Stats
	MaxHp = 100;
	CurHp = 100;
	Damage = 10;
	CurState = EState::Idle; 

	//스켈레톤 + 애니메이션 적용 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharaterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ASPrototype/Enemy/Enemy/Meshes/SK_HeavyGSoldier_simple.SK_HeavyGSoldier_simple'"));
	if (CharaterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharaterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/ASPrototype/Enemy/Enemy/ABP_TempEnemy.ABP_TempEnemy_C'"));
	//ensure(AnimInstanceClassRef.Class);
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	//Weapon 
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CurrentWeapon"));
	WeaponMesh->SetupAttachment(GetMesh(), FName(TEXT("Weapon_Socket")));

	//무기1 세팅 
	static ConstructorHelpers::FObjectFinder<UASWeaponData> StartWeaponRef1(TEXT("/Script/ASPrototype.ASWeaponData'/Game/ASPrototype/Weapon/AssultRifle.AssultRifle'"));
	if (StartWeaponRef1.Object)
	{
		Weapon1 = Cast<UASWeaponData>(StartWeaponRef1.Object);
		
		EquipWeapon(Weapon1);
	}
	//무기2 세팅 
	static ConstructorHelpers::FObjectFinder<UASWeaponData> StartWeaponRef2(TEXT("/Script/ASPrototype.ASWeaponData'/Game/ASPrototype/Weapon/Sniper.Sniper'"));
	if (StartWeaponRef2.Object)
	{
		Weapon2 = Cast<UASWeaponData>(StartWeaponRef2.Object);
	}




	//Gun Sound 
	static ConstructorHelpers::FObjectFinder<USoundBase> GunSoundRef(TEXT("/Script/Engine.SoundCue'/Game/ASPrototype/Sound/AssultRifleSound_Cue.AssultRifleSound_Cue'"));
	if (GunSoundRef.Object)
	{
		GunSound = GunSoundRef.Object;
		ensure(GunSound);
	}

	//Hit Sound 
	static ConstructorHelpers::FObjectFinder<USoundBase> HitSoundRef(TEXT("/Script/Engine.SoundCue'/Game/ASPrototype/Sound/HitMaker_Cue.HitMaker_Cue'"));
	if (HitSoundRef.Object)
	{
		HitSound = HitSoundRef.Object;
		ensure(HitSound);
	}

	static ConstructorHelpers::FClassFinder<AASItemBox> ItemBPClass(TEXT("/Script/ASPrototype.ASItemBox"));
	if (ItemBPClass.Class != NULL)
	{
		ItemClass = ItemBPClass.Class;
	}

	IsPlayerInRange = false;

}

void AASEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

AActor* AASEnemyBase::GetPatrolPath()
{
	AActor* actor = Cast<AActor>(PatrolPath);
	return actor;
}

// Called when the game starts or when spawned
void AASEnemyBase::BeginPlay()
{	
	Super::BeginPlay();
	BBData = Cast<IGetSetBlackBoardDataInterface>(GetOwner());
	DetectWidget = Cast<UASDetectWidget>(DetectBar->GetUserWidgetObject());
	DetectWidget->AddToViewport();
	ensure(DetectWidget); ensure(BBData);

	DetectWidget->FullPercentDelegate.AddUObject(this, &AASEnemyBase::FoundTarget);
	DetectWidget->AlertDelegate.AddUObject(this, &AASEnemyBase::SuspectTarget);
	Animinstance = Cast<UASAIAnimInstance>(GetMesh()->GetAnimInstance());
}

int AASEnemyBase::GetHp()
{
	return CurHp;
}

void AASEnemyBase::SetHp(int Hp)
{
	if (Hp<=0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, FString::Printf(TEXT("Enemy Dead")));
		CurState = EState::Dead;  
		SetDead();
		return;
	}
	CurHp = Hp;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("EnemyHp : %d"), CurHp));
}

void AASEnemyBase::SetDead()
{
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//AnimInstance->StopAllMontages(0.0f);
	//const float DelayTime = AnimInstance->Montage_Play(DeadMontage);
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	//SetActorEnableCollision(false);
	//Deda Animation Start
	const float DelayTime =PlayAnimMontage(DeathMontage);
	FVector ItemLocation = GetActorLocation();
	ItemLocation.Z -= 60;
	//적이 뒤지면 아이템 드랍
	auto CurItem = GetWorld()->SpawnActor<AASItemBox>(ItemClass, ItemLocation, GetActorRotation());
	//auto CurItem = GetWorld()->SpawnActor<AASItemBox>(ItemClass, FVector::ZeroVector, GetActorRotation());
	//if (CurItem != nullptr)
	//{
	//	float RandomValue = FMath::FRand();
	//	float BulletDropProbability = 0.5f;

	//	if (RandomValue < BulletDropProbability)
	//	{
	//		CurItem->SetItemName(TEXT("Bullet"));
	//	}
	//	else 
	//	{
	//		CurItem->SetItemName(TEXT("Bandage"));
	//	}
	//}

	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), DelayTime-0.5, false);
}

void AASEnemyBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (CurState == EState::Attack) { return; }
	//bool result = AiRef->IsPlayer(OtherActor);

	UAISense_Touch::ReportTouchEvent(GetWorld(), this, OtherActor, NormalImpulse);
}

FVector AASEnemyBase::GetTargetLocation()
{
	if (Target == nullptr)
	{
		return FVector(0,0,0);
	}
	return Target->GetActorLocation();
}

void AASEnemyBase::FoundTarget()
{
	BBData->SetBB_IsDetect(true);
}

void AASEnemyBase::SuspectTarget()
{
	BBData->SetBB_IsAlert(true);
}

void AASEnemyBase::SetIsPlayerInRange()
{
	IsPlayerInRange = !IsPlayerInRange;
}

//Calculate Angle Value for UI Rotation 
float AASEnemyBase::GetPlayerAngleValue()
{
	FRotator ControlRotator = GetControlRotation();
	FVector PlayerLoc = Target->GetActorLocation();
	FVector EnemyLoc = GetActorLocation();
	
	FRotator ControllerRotator = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
	FRotator IntervalRotator = UKismetMathLibrary::FindLookAtRotation(PlayerLoc, EnemyLoc);
	FRotator AngleRotator = UKismetMathLibrary::NormalizedDeltaRotator(IntervalRotator, ControllerRotator);
	return AngleRotator.Yaw;
}

void AASEnemyBase::EquipWeapon(UASWeaponData* NewWeaponData)
{
	if (NewWeaponData)
	{
		WeaponMesh->SetSkeletalMesh(NewWeaponData->WeaponModel);
	}
}

void AASEnemyBase::PlaySound(USoundBase* sound)
{
	UGameplayStatics::PlaySoundAtLocation(this, sound, GetActorLocation());
}

void AASEnemyBase::PlayHitReactAnimation()
{
	PlayAnimMontage(HitReactMontage);
}

void AASEnemyBase::PlayAttackAnimation()
{
	const float DelayTime = PlayAnimMontage(AttackMontage);
	//AttackEnd(DelayTime);
}

void AASEnemyBase::TurnToTarget(FVector Position)
{
	FVector TargetLocation;
	if (BBData->GetBB_IsDetect())
	{
		TargetLocation = Target->GetActorLocation();
	}
	else
	{
		TargetLocation = Position;
	}

	FRotator RotationDifferenceValue = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	FRotator ResultValue = FRotator(0.0f, RotationDifferenceValue.Yaw, 0.0f);
	FRotator RotatorValue = FMath::RInterpTo(GetActorRotation(), ResultValue, GetWorld()->GetDeltaSeconds(), 0.0f);
	SetActorRotation(RotatorValue);
}



//에러발생
//void AASEnemyBase::AttackEnd(const float InDelayTime)
//{
//	FTimerHandle myTimerHandle;
//	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
//		{
//			if (AiRef->GetPlayer())
//			{
//				OnAttackEnd.Broadcast();
//			}
//			// 타이머 초기화
//			GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
//		}), InDelayTime, false);
//}



// Called every frame
void AASEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsPlayerInRange)
	{
		DetectWidget->SetAngle(GetPlayerAngleValue());
	}
}	

void AASEnemyBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = GetMesh()->GetSocketLocation("HeadSocket");
	OutRotation = GetMesh()->GetSocketRotation("HeadSocket");
}

void AASEnemyBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AASEnemyBase::SetState(EState NewState)
{
	CurState = NewState;
	SetStateAnimation(CurState);
}

EState AASEnemyBase::GetState()
{
	return EState();
}


void AASEnemyBase::SetStateAnimation(EState NewState)
{
	switch (NewState)
	{	
	case EState::Idle:
		//WeaponInfo->WeaponModel->SetHiddenInGame(true);
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		//AiRef->RangeSizeDown();
		break;
	case EState::Alert:
		EquipWeapon(Weapon2);
		GetCharacterMovement()->MaxWalkSpeed = 150.0f;
		break;
	case EState::Attack:
		//WeaponInfo->WeaponModel->SetHiddenInGame(false);
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		//AiRef->RangeSizeUP();
		break;
	case EState::Dead:
		break;

	default:
		break;
	}
}

void AASEnemyBase::SetupPerception()
{
	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 700.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		AIPerComp->ConfigureSense(*SightConfig);
		AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	}
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = 2000.f;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		AIPerComp->ConfigureSense(*HearingConfig);
		AIPerComp->SetDominantSense(HearingConfig->GetSenseImplementation());
	}

	TouchConfig = CreateDefaultSubobject<UAISenseConfig_Touch>(TEXT("Touch Config"));
	if (TouchConfig)
	{
		AIPerComp->ConfigureSense(*TouchConfig);
		AIPerComp->SetDominantSense(TouchConfig->GetSenseImplementation());
	}

	AIPerComp->OnTargetPerceptionUpdated.AddDynamic(this, &AASEnemyBase::On_Updated);
}

void AASEnemyBase::On_Updated(AActor* DetectedPawn, const  FAIStimulus Stimulus)
{
	//if (EnemyRef->CurState == EState::Dead) { return; }
	if (DetectedPawn == nullptr)
	{
		return;
	}
	auto SensedClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);

	//시야 식별인 경우 
	if (SensedClass == UAISense_Sight::StaticClass())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Sight Sense")));
		CheckPlayer(DetectedPawn);
	}

	//감각 식별인 경우 
	else if (SensedClass == UAISense_Touch::StaticClass())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Touch Sense"))); 
		BBData->SetBB_IsDetect(true);
		//터치된 상대가 Player 캐스팅 성공 한 경우, 바로 Player에게 Focus On이 됨. ( UI바 상승 스피드 2배 UP )
		//캐스팅 된 것이 총알이면 IsDetect==true
	}
}

//탐지 상태 관련 
void AASEnemyBase::CheckPlayer(AActor* player)
{
	// 플레이어가 아니거나 , 플레이어를 탐지한 경우 예외처리
	IASCharacterInterface* CheckingPlayer = Cast<IASCharacterInterface>(player);
	if (CheckingPlayer == NULL || BBData == NULL) { return; }
	Target = player;
	if (BBData->GetBB_IsDetect()) { return; }
	if (BBData->GetBB_Target())
	{
		BBData->SetBB_Target(nullptr); //나가야 하는 상황 
		DetectWidget->DecreaseDetection();
		IsPlayerInRange = false;
	}
	else
	{
		BBData->SetBB_Target(player); //들어가야 하는 상황
		DetectWidget->IncreaseDetection();
		IsPlayerInRange = true;
	}
}

