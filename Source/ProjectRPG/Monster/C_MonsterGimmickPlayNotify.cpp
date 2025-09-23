// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MonsterGimmickPlayNotify.h"
#include"NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "C_MonsterBaseCharacter.h"

void UC_MonsterGimmickPlayNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!MeshComp->GetOwner())
		return;

	const FTransform MeshTransform = MeshComp->GetSocketTransform(m_SocketName);

	UNiagaraComponent* pNiagaraCom = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		MeshComp->GetWorld(),
		m_pNiagara,
		MeshTransform.TransformPosition(m_sRelativePosition),
		FRotator::ZeroRotator,
		m_vScale,
		false
		);

	AC_MonsterBaseCharacter* pMonster = Cast<AC_MonsterBaseCharacter>(MeshComp->GetOwner());

	if (pMonster)
		pMonster->setNiagaraComponent(pNiagaraCom);



}
