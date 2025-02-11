#include "NBC_PlayerController.h"
#include "EnhancedInputSubsystems.h" // Enhanced Input 시스템 사용을 위한 헤더 파일 포함


ANBC_PlayerController::ANBC_PlayerController()
	:InputMappingContext(nullptr), // 입력 매핑 컨텍스트 초기화, 플레이어 컨트롤러에서 적절한 입력 매핑을 적용하여 이동 및 카메라 조작 같은 기능을 정상적으로 작동하게 만들기 위해.
	MoveAction(nullptr), // 이동 입력 액션 초기화
	LookAction(nullptr) // 카메라 회전 입력 액션 초기화
{
}

void ANBC_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) // 로컬 플레이어 가져오기
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) // Enhanced Input 시스템의 LocalPlayer 서브시스템 가져오기
		{
			if (InputMappingContext) // InputMappingContext가 설정되어 있으면 입력 매핑 컨텍스트 추가
			{
				Subsystem->AddMappingContext(InputMappingContext, 0); // 우선순위 0으로 컨텍스트 추가
			}
		}
	}
}