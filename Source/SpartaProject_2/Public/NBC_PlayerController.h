#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBC_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;


UCLASS()
class SPARTAPROJECT_2_API ANBC_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ANBC_PlayerController();
	// 입력 매핑 컨텍스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	// 이동 입력 액션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	// 카메라 회전 입력 액션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

protected:
	virtual void BeginPlay() override;
};
