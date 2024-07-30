// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tool/ASWeaponData.h"
#include "Tool/ASWeaponItem.h"
//�̵� ���� ����
//#include "Enemy/PatrolPath.h"
//�� ���ʹ̵��� �Ǻ��ϱ� ���� ID���� �־���
//#include "GenericTeamAgentInterface.h"
#include "Interface/ASEnemyInterface.h"
#include "Interface/ASAIWidgetInterface.h"
#include "ASEnemyBase.generated.h"

UENUM()
enum class EState : uint8
{
	Idle, 
	Alert, //�ǽ� ����
	Attack, //���� ���� 
	Dead //���
};



UCLASS()
class ASPROTOTYPE_API AASEnemyBase : public ACharacter, public IASEnemyInterface, public IASAIWidgetInterface //, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AASEnemyBase();

	//FOnAttackEndDelegate OnAttackEnd;
	virtual void PostInitializeComponents() override;

	int GetHp();
	void SetHp(int Hp);
	virtual void SetDead();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	virtual FVector GetTargetLocation() override;
	virtual FOnChangeStateDelegate& GetStateDelegate() override;
	virtual AActor* GetTarget() override;
	FOnChangeStateDelegate StateDelegate;
	FOnAttackEndDelegate AttackEndDelegate;

protected:
	virtual void AttackCheck();
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<class AASItemBox> ItemClass;

private:
	void FoundTarget();
	void SuspectTarget();
	virtual void InitState() override;

	int MaxHp;
	int CurHp;
	int Damage;
	AActor* Target;
	EState CurState;
	bool IsPlayerInRange;
	void SetIsPlayerInRange();
	float GetPlayerAngleValue();

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Attack() override;

	void SetStateAnimation(EState NewState);

	//TArray<FVector> ���� ���� Actor Ŭ���� ������ 
	UPROPERTY(EditAnywhere)
	TObjectPtr<class APatrolPath> PatrolPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> HitReactMontage;


	//���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;
	TObjectPtr<class UASWeaponData> Weapon1;
	TObjectPtr<class UASWeaponData> Weapon2;

	void EquipWeapon(UASWeaponData* NewWeaponData);

	void PlaySound(USoundBase* sound);
	class USoundBase* HitSound;
	class USoundBase* GunSound;

	void PlayHitReactAnimation();
	void PlayAttackAnimation();


	class UASAIAnimInstance* Animinstance;

	virtual void TurnToTarget(FVector Position) override;

	void AttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	//int32 ID = 0;

	//���ʹ̰� �������� �������� �ƴ��� �Ǵ��ϱ� ���� ����
	//virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TObjectPtr<class UASWidgetComponent> QuestionMark;


	//UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TObjectPtr<class UASWidgetComponent> DetectBar;

	virtual AActor* GetPatrolPath() override;
	virtual void SetAIAttackDelegate(FOnAttackEndDelegate& OnAttackEndDelegate) override;
	virtual void SetupDetectWidget(class UASUserWidget* InWidget) override;
	virtual void SetupAlertWidget(class UASUserWidget* InWidget) override;

protected:
	class IGetSetBlackBoardDataInterface* BB;
	class UASDetectWidget* DetectWidget;
	class UASQuestionMarkWidget* AlertWidget;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//���ʹ̰� �������� �������� �ƴ��� �Ǵ��ϱ� ���� ���� 
	//FGenericTeamId TeamId;

	void SetState(uint8 NewState);
	EState GetState();


	// AI Perception
	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
	class UAIPerceptionComponent* AIPerComp;
	class UAISenseConfig_Sight* SightConfig;
	class UAISenseConfig_Hearing* HearingConfig;
	class UAISenseConfig_Touch* TouchConfig;

	//AI ����
	UFUNCTION()
	void On_Updated(AActor* DetectedPawn, const  FAIStimulus Stimulus);
	
	void SetupPerception();
	void CheckPlayer(AActor* P);

};

