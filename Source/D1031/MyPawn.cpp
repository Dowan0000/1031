// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MyActor.h"
#include "Components/ArrowComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(CollisionBox);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> _Body(TEXT("StaticMesh'/Game/_Game/P38/SM_P38_Body.SM_P38_Body'"));
	if (_Body.Succeeded())
	{
		Body->SetStaticMesh(_Body.Object);
	}
	
	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> _Right(TEXT("StaticMesh'/Game/_Game/P38/SM_P38_Propeller.SM_P38_Propeller'"));
	if (_Right.Succeeded())
	{
		Left->SetStaticMesh(_Right.Object);
		Right->SetStaticMesh(_Right.Object);
		Left->SetRelativeLocation(FVector(37.f, -21.f, 0.f));
		Right->SetRelativeLocation(FVector(37.f, 21.f, 0.f));
	}
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CollisionBox);
	SpringArm->TargetArmLength = 250.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = 200.f;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetRelativeLocation(FVector(100.f, 0.f, 0.f));

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Left->AddLocalRotation(FRotator(0.f, 0.f, 360.f * DeltaTime));
	Right->AddLocalRotation(FRotator(0.f, 0.f, 360.f * DeltaTime));

	AddMovementInput(GetActorForwardVector());
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);
	
	PlayerInputComponent->BindAction(TEXT("Accel"), EInputEvent::IE_Pressed, this, &AMyPawn::AccelPressed);
	PlayerInputComponent->BindAction(TEXT("Accel"), EInputEvent::IE_Released, this, &AMyPawn::AccelReleased);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AMyPawn::Shoot);

}

void AMyPawn::Pitch(float Value)
{
	AddActorLocalRotation(FRotator(Value * 60.f * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0, 0));
}

void AMyPawn::Roll(float Value)
{
	AddActorLocalRotation(FRotator(0, 0, Value * 60.f * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

void AMyPawn::AccelPressed()
{
	Movement->MaxSpeed = 500.f;
}

void AMyPawn::AccelReleased()
{
	Movement->MaxSpeed = 200.f;
}

void AMyPawn::Shoot()
{
	GetWorld()->SpawnActor<AMyActor>(AMyActor::StaticClass());
}

