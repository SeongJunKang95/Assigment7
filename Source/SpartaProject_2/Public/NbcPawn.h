#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h" // Enhanced input System의 입력 값을 다루기 위해 포함
#include "NbcPawn.generated.h"

class UCapsuleComponent;
class UArrowComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;



UCLASS()
class SPARTAPROJECT_2_API ANbcPawn : public APawn
{
	GENERATED_BODY()

public:

	ANbcPawn();

protected:

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ArrowComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletaMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	
	// 이동 및 회전 함수

	UFUNCTION()
	void Move(const FInputActionValue& value);
	
	UFUNCTION()
	void StopMove();

	UFUNCTION()
	void Look(const FInputActionValue& value);
	
	UFUNCTION()
	void StopLook();

	// 움직임 입력값 저장 (X :전후, Y: 좌우)
	FVector2D MoveInput;
	//마우스 입력값 저장 (X: 좌우회전, Y 상하 회전)
	FVector2D LookInput;
};