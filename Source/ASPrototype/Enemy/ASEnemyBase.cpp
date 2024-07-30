// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ASEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

//BB���� ��� ���� 
#include "Interface/GetSetBlackBoardDataInterface.h"
#include "AI/ASAIController.h"
#include "AI/ASAI.h"

//UI
#include "UI/ASDetectWidget.h"
#include "UI/ASWidgetComponent.h"
#include "UI/ASQuestionMarkWidget.h"

//���� ���� 
#include "Tool/ASWeaponData.h"
#include "Tool/ASWeaponItem.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"

#include "Perception/AISense_Touch.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ASItemBox.h"
#include "Animation/ASAIAnimInstance.h"

//Ž�� ���
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h" //�þ�
#include "Perception/AISenseConfig_Hearing.h" //����
#include "Perception/AISense_Touch.h"//����
#include "Perception/AISenseConfig_Touch.h"//����
#include "Interface/ASCharacterInterface.h"
#include "Kismet/KismetMathLibrary.h" //charactor moving

#include "NavigationSystem.h"	//�׺���̼�

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

	//���� 2�� ���� �̽�->������ƮUI ����� �ذ� 
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
	}

	QuestionMark = CreateDefaultSubobject<UASWidgetComponent>(TEXT("QuestionMarkWidget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> QuestionMarkRef(TEXT("/Game/UI/WB_QuestionMark_UI.WB_QuestionMark_UI_C"));
	ensure(QuestionMarkRef.Class);
	if (QuestionMarkRef.Class)
	{
		QuestionMark->SetWidgetClass(QuestionMarkRef.Class);
		QuestionMark->SetWidgetSpace(EWidgetSpace::World);
		QuestionMark->SetDrawSize(FVector2D(30.0f, 30.0f));
		QuestionMark->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		QuestionMark->SetupAttachment(GetMesh());
		QuestionMark->AddRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
		QuestionMark->SetRelativeLocation(FVector(0.0f, 0.0f, 210.0f));
	}

	SetupPerception();

	//Stats
	MaxHp = 100;
	CurHp = 100;
	Damage = 10;
	CurState = EState::Idle; 

	//���̷��� + �ִϸ��̼� ���� 
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

	//����1 ���� 
	static ConstructorHelpers::FObjectFinder<UASWeaponData> StartWeaponRef1(TEXT("/Script/ASPrototype.ASWeaponData'/Game/ASPrototype/Weapon/AssultRifle.AssultRifle'"));
	if (StartWeaponRef1.Object)
	{
		Weapon1 = Cast<UASWeaponData>(StartWeaponRef1.Object);
		
		EquipWeapon(Weapon1);
	}
	//����2 ���� 
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

	StateDelegate.AddUObject(this, &AASEnemyBase::SetState);

}

void AASEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//Montage Setting 
	Animinstance = Cast<UASAIAnimInstance>(GetMesh()->GetAnimInstance());
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AASEnemyBase::AttackEnd);
	Animinstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	BB = Cast<IGetSetBlackBoardDataInterface>(GetOwner());
	ensure(BB);
}

AActor* AASEnemyBase::GetPatrolPath()
{
	AActor* actor = Cast<AActor>(PatrolPath);
	return actor;
}


void AASEnemyBase::SetAIAttackDelegate(FOnAttackEndDelegate& OnAttackEndDelegate)
{
	AttackEndDelegate = OnAttackEndDelegate;
}

void AASEnemyBase::SetupDetectWidget(UASUserWidget* InWidget)
{
	DetectWidget = Cast<UASDetectWidget>(InWidget); 	ensure(DetectWidget);
	DetectWidget->FullPercentDelegate.AddUObject(this, &AASEnemyBase::FoundTarget);
	DetectWidget->AlertDelegate.AddUObject(this, &AASEnemyBase::SuspectTarget);
}

void AASEnemyBase::SetupAlertWidget(UASUserWidget* InWidget)
{
	AlertWidget = Cast<UASQuestionMarkWidget>(InWidget); 	ensure(AlertWidget);
}


// Called when the game starts or when spawned
void AASEnemyBase::BeginPlay()
{	
	Super::BeginPlay();
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
	//���� ������ ������ ���
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

FOnChangeStateDelegate& AASEnemyBase::GetStateDelegate()
{
	return StateDelegate;
}

AActor* AASEnemyBase::GetTarget()
{
	return Target;
}

void AASEnemyBase::AttackCheck()
{
}

void AASEnemyBase::FoundTarget()
{
	BB->SetBB_IsAlert(false);
	BB->SetBB_IsDetect(true);
	AlertWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AASEnemyBase::SuspectTarget()
{
	if (BB->GetBB_IsDetect()) return;
	BB->SetBB_IsAlert(true);
	AlertWidget->SetVisibility(ESlateVisibility::Visible);
}

void AASEnemyBase::InitState()
{
	BB->SetBB_IsAlert(false);
	BB->SetBB_IsDetect(false);
	AlertWidget->SetVisibility(ESlateVisibility::Hidden);
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
	Animinstance->Montage_Play(AttackMontage);
}

void AASEnemyBase::TurnToTarget(FVector Position)
{
	FVector TargetLocation;
	if (BB->GetBB_IsDetect())
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




void AASEnemyBase::AttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	AttackEndDelegate.ExecuteIfBound();
}



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

void AASEnemyBase::Attack()
{
	AttackCheck();
	PlayAttackAnimation();
}

void AASEnemyBase::SetState(uint8 NewState)
{
	CurState = (EState)NewState;
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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Idle")));
		break;
	case EState::Alert:
		EquipWeapon(Weapon2);
		GetCharacterMovement()->MaxWalkSpeed = 150.0f; 		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Alert")));
		break;
	case EState::Attack:
		//WeaponInfo->WeaponModel->SetHiddenInGame(false);
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed; 		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Attack")));
		//AiRef->RangeSizeUP();
		break;
	case EState::Dead:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Dead")));
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

	//�þ� �ĺ��� ��� 
	if (SensedClass == UAISense_Sight::StaticClass())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Sight Sense")));
		CheckPlayer(DetectedPawn);
	}

	//���� �ĺ��� ��� 
	else if (SensedClass == UAISense_Touch::StaticClass())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Touch Sense"))); 
		BB->SetBB_IsDetect(true);
		//��ġ�� ��밡 Player ĳ���� ���� �� ���, �ٷ� Player���� Focus On�� ��. ( UI�� ��� ���ǵ� 2�� UP )
		//ĳ���� �� ���� �Ѿ��̸� IsDetect==true
	}
}

//Ž�� ���� ���� 
void AASEnemyBase::CheckPlayer(AActor* Actor)
{
	// �÷��̾ �ƴϰų� , �÷��̾ Ž���� ��� ����ó��
	IASCharacterInterface* CheckingPlayer = Cast<IASCharacterInterface>(Actor);
	if (CheckingPlayer == NULL || BB == NULL) { return; }

	Target = Actor;

	if (BB->GetBB_Target())
	{
		BB->SetBB_Target(nullptr); //������ �ϴ� ��Ȳ 
		IsPlayerInRange = false;
	}
	else
	{
		BB->SetBB_Target(Actor); //���� �ϴ� ��Ȳ
		IsPlayerInRange = true;
	}

	if (BB->GetBB_IsDetect()) { return; }

	if (IsPlayerInRange)
	{
		DetectWidget->IncreaseDetection();
	}
	else
	{
		DetectWidget->DecreaseDetection();
	}
}

