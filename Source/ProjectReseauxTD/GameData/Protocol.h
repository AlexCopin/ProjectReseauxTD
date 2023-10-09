#pragma once
#include "CoreMinimal.h"

#include "Protocol.generated.h"

enum class Opcode : uint8
{
	C_PlayerName,
	C_PlayerInput,
	S_Gold
};