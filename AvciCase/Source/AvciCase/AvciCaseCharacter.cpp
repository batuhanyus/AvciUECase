// Copyright Epic Games, Inc. All Rights Reserved.

#include "AvciCaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/HealthComponent.h"
#include "Weaponry/WeaponActorBase.h"


//////////////////////////////////////////////////////////////////////////
// AAvciCaseCharacter

AAvciCaseCharacter::AAvciCaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AAvciCaseCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	SetUpRocketLauncher();


	//OnTakeAnyDamage.AddDynamic(this, &AAvciCaseCharacter::OnTakeRadialDamage);
	//HealthComponent->OnDeath.AddDynamic(this, &AAvciCaseCharacter::OnDeath);
	//HealthComponent->OnHealthChanged.AddDynamic(this, &AAvciCaseCharacter::OnHealthChanged);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAvciCaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAvciCaseCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAvciCaseCharacter::Look);


		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AAvciCaseCharacter::Shoot);
		EnhancedInputComponent->BindAction(LauncherModeAction, ETriggerEvent::Triggered, this,
		                                   &AAvciCaseCharacter::SwitchLauncherMode);
	}
}

void AAvciCaseCharacter::Move(const FInputActionValue& Value)
{
	if (bIsRocketLauncherMode) return;

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAvciCaseCharacter::Look(const FInputActionValue& Value)
{
	if (bIsRocketLauncherMode)
		RotateRocketLauncher(Value);
	else
		RotateCharacter(Value);
}

void AAvciCaseCharacter::RotateCharacter(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void AAvciCaseCharacter::RotateRocketLauncher(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		FRotator NewRotation = RocketLauncher->GetActorRotation();
		NewRotation.Yaw += LookAxisVector.X;
		NewRotation.Pitch += LookAxisVector.Y;

		RocketLauncher->SetActorRotation(NewRotation);
	}

	CastGuidanceRay();
}

void AAvciCaseCharacter::Shoot()
{
	if (!bIsRocketLauncherMode)return;

	UE_LOG(LogTemp, Warning, TEXT("Shoot!"));
	OnShoot.Broadcast();
}

void AAvciCaseCharacter::SwitchLauncherMode(const FInputActionValue& Value)
{
	bIsRocketLauncherMode = !bIsRocketLauncherMode;

	if (bIsRocketLauncherMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rocket Launcher Mode"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Peace Mode"));
	}
}

void AAvciCaseCharacter::CastGuidanceRay()
{
	FHitResult HitResult;

	FVector Start = RocketLauncher->GetActorLocation();
	FRotator Rotation = RocketLauncher->GetActorRotation();
	FVector End = Start + (Rotation.Vector() * 10000);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1);
}

void AAvciCaseCharacter::OnDeath()
{
	FVector3d JailLocation = FVector3d(0, 0, -3500);
	SetActorLocation(JailLocation);
}

void AAvciCaseCharacter::OnHealthChanged()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
	                                 FString::Printf(TEXT("Player Health: %f"), HealthComponent->GetHealth()));
}

void AAvciCaseCharacter::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                            AController* InstigatedBy, AActor* DamageCauser)
{
	HealthComponent->ReduceHealth(Damage);
}

void AAvciCaseCharacter::SetUpRocketLauncher()
{
	AWeaponActorBase* SpawnedLauncher = GetWorld()->SpawnActor<AWeaponActorBase>(RocketLauncherBP);
	SpawnedLauncher->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                                   "rocketSocket");
	RocketLauncher = SpawnedLauncher;
	OnShoot.AddDynamic(RocketLauncher, &AWeaponActorBase::Shoot);
}