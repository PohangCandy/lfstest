// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tool/ASWeaponData.h"
#include "Tool/ASWeaponItem.h"
//이동 지점 설정
//#include "Enemy/PatrolPath.h"
//각 에너미들을 판별하기 위해 ID값을 넣어줌
//#include "GenericTeamAgentInterface.h"

#include "ASEnemyBase.generated.h"

UENUM()
enum class EState
{
	Idle, 
	Alert, //의심 상태
	Chasing, //쫓는 상태
	Attack, //공격상태 
	Hurt,  //절뚝거리기
	Hidden, //숨은 상태
	Dead //사망
}; 

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);


UCLASS()
class ASPROTOTYPE_API AASEnemyBase : public ACharacter//, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AASEnemyBase();

	//FOnAttackEndDelegate OnAttackEnd;

	float SplineSpeed;
	float DistanceAlongSpline;

	int GetHp();
	void SetHp(int Hp);
	virtual void SetDead();
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<class AASItemBox> ItemClass;

private:
	int MaxHp;
	int CurHp;
	int Damage;


public:	
	EState CurState;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void SetStateAnimation(EState NewState);

	//TArray<FVector> 값을 가진 Actor 클래스 포인터 
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


	//무기 세팅
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;
	TObjectPtr<class UASWeaponData> Weapon1;
	TObjectPtr<class UASWeaponData> Weapon2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = name)
	FString Name;

	void EquipWeapon(UASWeaponData* NewWeaponData);

	void PlaySound(USoundBase* sound);
	class USoundBase* HitSound;
	class USoundBase* GunSound;

	void PlayHitReactAnimation();
	void PlayAttackAnimation();


	class UASAIAnimInstance* Animinstance;


	//void AttackEnd(const float InDelyTime);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	//int32 ID = 0;

	//에너미가 적대적인 존재인지 아닌지 판단하기 위해 넣음
	//virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TObjectPtr<class UWidgetComponent> QuestionMark;



protected:
	class AASAIController* AiRef;
	class UASDetectWidget* UiRef;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//에너미가 적대적인 존재인지 아닌지 판단하기 위해 넣음 
	//FGenericTeamId TeamId;

	void SetState(EState NewState);
	EState GetState();

};

