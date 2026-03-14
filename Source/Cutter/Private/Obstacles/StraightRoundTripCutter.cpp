#include "StraightRoundTripCutter.h"

// Called when the game starts or when spawned
void AStraightRoundTripCutter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStraightRoundTripCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Translate(DeltaTime);
}

void AStraightRoundTripCutter::Init()
{
}

void AStraightRoundTripCutter::Translate(float deltaTime)
{
}