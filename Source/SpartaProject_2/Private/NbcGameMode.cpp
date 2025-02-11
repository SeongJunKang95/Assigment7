#include "NbcGameMode.h"
#include "NbcPawn.h"
#include "NBC_PlayerController.h"

//기본 폰 클래스에 스테틱 클래스를 붙여서 클래스를 반환
ANbcGameMode::ANbcGameMode()
{
	DefaultPawnClass = ANbcPawn::StaticClass(); // 게임이 시작될 때 생성될 기본 Pawn 클래스 지정
	PlayerControllerClass = ANBC_PlayerController::StaticClass(); // 사용할 PlayerController 클래스 지정
}
