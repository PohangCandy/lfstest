// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_Walking.h"
#include "Character/ASCharacterPlayer.h"
#include "Sound/SoundBase.h"
void UAnimNotify_Walking::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AASCharacterPlayer* Player = Cast<AASCharacterPlayer>(MeshComp->GetOwner());
	if (Player)
	{
		Player->PlaySound(Player->WalkSound);
	}
}
