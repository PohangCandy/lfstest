// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "ASCharacterBase.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnStateChangeDelegate);

UENUM()
enum class State
{
	None,
	Prone, //�ɱ�
	Crawl, //���帮��
	Hurt,  //���ҰŸ���
	Hidden, //���� ����
	Dead
};


UCLASS()
class ASPROTOTYPE_API AASCharacterBase : public ACharacter
{
	GENERATED_BODY()

private:
	int Curstrength;
	int MaxHp;
	int CurHp;
	int MaxBulletNum;
	int CurBulletNum;
	int MaxMagazineNum;
	int CurMagazineNum;
	int MaxItemNum;
	int CurItemNum;
	int Damage;
	State CurState;
	//UPROPERTY()
	//class AASPlayerController* PlayerController;
	int MaxMagnification;
	int CurMagnification;


public:
	// Sets default values for this character's properties
	AASCharacterBase();
	virtual void PostInitializeComponents() override;
	virtual void SetDead();
	int GetHp();
	int GetBulletNum();
	int GetMagazineNum();
	int GetItemNum();
	int GetStrength();
	float GetHpratio();
	void SetHp(int Hp);
	void SetBulletNum(int Num);
	void SetMagazineNum(int Num);
	void SetItemNum(int Num);
	void GetDamaged(int damage);
	void SetStrength(int newStrength);
	bool AttackCheck();


	void SetState(State NewState);
	void ConsumeBullet();
	void Reload();
	void Heal();
	void TestingGetDamage();
	void InitUIData();
	State GetState();
	FOnStateChangeDelegate OnHpChanged;
	FOnStateChangeDelegate NumBulletChanged;
	FOnStateChangeDelegate NumMagazineChanged;
	FOnStateChangeDelegate NumItemChanged;
	FOnStateChangeDelegate NumMagnificationChanged;
	FOnStateChangeDelegate SwitchSnip;

	int GetMagnificationNum();
	void SetMagnificationNum(int newMag);
	float GetMagnificationratio();
	void ZoomIn();
	void ZoomOut();


	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UASCharacterStatComponent* CharacterStat;
	

	//사운드
	void PlaySound(USoundBase* sound);
	class USoundBase* WalkSound;
	class USoundBase* ShootSound;

	//bool CanFire();
protected:
	bool CanFire();

protected:
	virtual void BeginPlay() override;

	class UAnimInstance* AnimInstance;

	int LowHp; // LowHp ���ϸ� ���ҰŸ� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> SearchMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ReloadMontage;


	FTimerHandle DeadTimerHandle;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Controller)
	//TSubclassOf<class AASPlayerController> CurplayerControllerClass;

	virtual void Tick(float DeltaTime) override;

	class USkeletalMeshComponent* curWeaponMesh;
};
