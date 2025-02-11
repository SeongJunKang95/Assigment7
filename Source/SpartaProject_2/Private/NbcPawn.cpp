#include "NbcPawn.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NBC_PlayerController.h"




ANbcPawn::ANbcPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // 충돌 캡슐 생성
    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    RootComponent = CapsuleComp;
    CapsuleComp->InitCapsuleSize(37.f, 95.f); // 캡슐 크기 설정(반지름, 높이)
    CapsuleComp->SetCollisionProfileName(TEXT("Pawn")); // 충돌 설정
    CapsuleComp->SetSimulatePhysics(false); // 물리 시뮬레이션 비활성화

    // 방향을 나타내는 Arroowcomponent 생성
    ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow")); 
    ArrowComp ->SetupAttachment(RootComponent);


    // Skeletal Mesh 생성
    SkeletaMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletaMeshComp->SetupAttachment(RootComponent);
    SkeletaMeshComp->SetSimulatePhysics(false); // 물리 시뮬레이션 비활성화

    // SpringArm 생성
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 300.0f; // 카메라 거리설정
    SpringArmComp->bUsePawnControlRotation = false;  // Pawn의 회전에 따라 카메라가 움직이지 않도록 설정
    SpringArmComp->bEnableCameraRotationLag = true; // 부드러운 회전 적용
    SpringArmComp->bDoCollisionTest = false; // 충돌 감지 비활성화 (필요에 따라 변경 가능)

    // 카메라 생성
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false; // Pawn이 아니라 StringArm이 카메라를 제어하도록 설정

    // 입력 백터 초기화
    LookInput = FVector2D::ZeroVector;
    MoveInput = FVector2D::ZeroVector;
   
}

void ANbcPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) // 입력 바인딩 함수 (Enhanced Input System 사용)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // EnhancedInputComponent로 캐스팅 (Enhanced Input 시스템 사용 여부 확인)
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // 현재 소유 중인 컨트롤러를 NBC_PlayerController로 캐스팅
        if (ANBC_PlayerController* PlayerController = Cast<ANBC_PlayerController>(GetController()))
        {
            if (PlayerController->MoveAction) // 이동 입력 바인딩
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this, &ANbcPawn::Move);

                EnhancedInput->BindAction(
                    PlayerController->MoveAction, 
                    ETriggerEvent::Completed, 
                    this, &ANbcPawn::StopMove);
            }

            //카메라 회전 입력 바인딩
            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this, &ANbcPawn::Look );

                EnhancedInput->BindAction(
                    PlayerController->LookAction, 
                    ETriggerEvent::Completed, 
                    this, &ANbcPawn::StopLook);
            }
        }
    }
}


void ANbcPawn::Move(const FInputActionValue& value)
{
    MoveInput = value.Get<FVector2D>(); // 입력값 저장 (Tick에서 처리)
   
}

void ANbcPawn::StopMove()
{
    MoveInput = FVector2D::ZeroVector; // 이동 입력 종료 시 백터 리셋
}

void ANbcPawn::Look(const FInputActionValue& value)
{
    LookInput = value.Get<FVector2D>(); // 입력값 저장 (Tick에서 처리)
}

void ANbcPawn::StopLook()
{
    LookInput = FVector2D::ZeroVector; // 회전 입력 종료 시 백터 리셋
}

// Tick()에서 매 프레임 입력 처리 (이동 및 회전 적용)
UE_DISABLE_OPTIMIZATION
void ANbcPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); 

    // 이동처리
    if (!FMath::IsNearlyZero(MoveInput.X) || !FMath::IsNearlyZero(MoveInput.Y))
    {
        FVector MoveDirection = FVector::ZeroVector;
        // 전/후 이동처리
        if (!FMath::IsNearlyZero(MoveInput.X))
        {
            MoveDirection += GetActorForwardVector() * MoveInput.X;
        }
        // 좌/우 이동처리
        if (!FMath::IsNearlyZero(MoveInput.Y))
        {
            MoveDirection += GetActorRightVector() * MoveInput.Y;
        }

        // 이동 속도 설정
        float MoveSpeed = 500.0f;
        FVector DeltaLocation = MoveDirection.GetSafeNormal() * MoveSpeed * DeltaTime;
        AddActorLocalOffset(DeltaLocation); // 실제 이동 적용, sweep 충돌문제 다시 확인해보기 !
    }

    // 좌/우 회전 처리
    if (!FMath::IsNearlyZero(LookInput.X))
    {
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw += LookInput.X * 100.0f * DeltaTime; // 감도 조정 가능
        SetActorRotation(NewRotation);
    }

    // 상하(Pitch) 회전 처리 - SpringArm을 이용해 카메라 회전
    if (!FMath::IsNearlyZero(LookInput.Y))
    {
        FRotator SpringArmRotation = SpringArmComp->GetRelativeRotation();
        SpringArmRotation.Pitch = FMath::Clamp(SpringArmRotation.Pitch + LookInput.Y * -50.0f * DeltaTime, -80.0f, 80.0f); // 상하 회전 제한 (-80 ~ 80도)
        SpringArmComp->SetRelativeRotation(SpringArmRotation);
    }

  

}
UE_ENABLE_OPTIMIZATION

