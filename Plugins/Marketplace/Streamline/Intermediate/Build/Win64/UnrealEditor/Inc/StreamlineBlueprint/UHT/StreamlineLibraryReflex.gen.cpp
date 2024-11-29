// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "StreamlineBlueprint/Public/StreamlineLibraryReflex.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeStreamlineLibraryReflex() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
STREAMLINEBLUEPRINT_API UClass* Z_Construct_UClass_UStreamlineLibraryReflex();
STREAMLINEBLUEPRINT_API UClass* Z_Construct_UClass_UStreamlineLibraryReflex_NoRegister();
STREAMLINEBLUEPRINT_API UEnum* Z_Construct_UEnum_StreamlineBlueprint_UStreamlineFeatureSupport();
STREAMLINEBLUEPRINT_API UEnum* Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode();
UPackage* Z_Construct_UPackage__Script_StreamlineBlueprint();
// End Cross Module References

// Begin Enum UStreamlineReflexMode
static FEnumRegistrationInfo Z_Registration_Info_UEnum_UStreamlineReflexMode;
static UEnum* UStreamlineReflexMode_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_UStreamlineReflexMode.OuterSingleton)
	{
		Z_Registration_Info_UEnum_UStreamlineReflexMode.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode, (UObject*)Z_Construct_UPackage__Script_StreamlineBlueprint(), TEXT("UStreamlineReflexMode"));
	}
	return Z_Registration_Info_UEnum_UStreamlineReflexMode.OuterSingleton;
}
template<> STREAMLINEBLUEPRINT_API UEnum* StaticEnum<UStreamlineReflexMode>()
{
	return UStreamlineReflexMode_StaticEnum();
}
struct Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Disabled.DisplayName", "Disabled" },
		{ "Disabled.Name", "UStreamlineReflexMode::Disabled" },
		{ "Enabled.DisplayName", "Enabled" },
		{ "Enabled.Name", "UStreamlineReflexMode::Enabled" },
		{ "EnabledPlusBoost.DisplayName", "Enabled + Boost" },
		{ "EnabledPlusBoost.Name", "UStreamlineReflexMode::EnabledPlusBoost" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "UStreamlineReflexMode::Disabled", (int64)UStreamlineReflexMode::Disabled },
		{ "UStreamlineReflexMode::Enabled", (int64)UStreamlineReflexMode::Enabled },
		{ "UStreamlineReflexMode::EnabledPlusBoost", (int64)UStreamlineReflexMode::EnabledPlusBoost },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_StreamlineBlueprint,
	nullptr,
	"UStreamlineReflexMode",
	"UStreamlineReflexMode",
	Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode_Statics::Enum_MetaDataParams), Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode()
{
	if (!Z_Registration_Info_UEnum_UStreamlineReflexMode.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_UStreamlineReflexMode.InnerSingleton, Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_UStreamlineReflexMode.InnerSingleton;
}
// End Enum UStreamlineReflexMode

// Begin Class UStreamlineLibraryReflex Function GetDefaultReflexMode
struct Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics
{
	struct StreamlineLibraryReflex_eventGetDefaultReflexMode_Parms
	{
		UStreamlineReflexMode ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Get default Reflex mode" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventGetDefaultReflexMode_Parms, ReturnValue), Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode, METADATA_PARAMS(0, nullptr) }; // 184094367
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "GetDefaultReflexMode", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::StreamlineLibraryReflex_eventGetDefaultReflexMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::StreamlineLibraryReflex_eventGetDefaultReflexMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execGetDefaultReflexMode)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UStreamlineReflexMode*)Z_Param__Result=UStreamlineLibraryReflex::GetDefaultReflexMode();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function GetDefaultReflexMode

// Begin Class UStreamlineLibraryReflex Function GetGameLatencyInMs
struct Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics
{
	struct StreamlineLibraryReflex_eventGetGameLatencyInMs_Parms
	{
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Get Reflex Game Latency (ms)" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventGetGameLatencyInMs_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "GetGameLatencyInMs", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::StreamlineLibraryReflex_eventGetGameLatencyInMs_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::StreamlineLibraryReflex_eventGetGameLatencyInMs_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execGetGameLatencyInMs)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UStreamlineLibraryReflex::GetGameLatencyInMs();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function GetGameLatencyInMs

// Begin Class UStreamlineLibraryReflex Function GetGameToRenderLatencyInMs
struct Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics
{
	struct StreamlineLibraryReflex_eventGetGameToRenderLatencyInMs_Parms
	{
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Get Reflex Game To Render Latency (ms)" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventGetGameToRenderLatencyInMs_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "GetGameToRenderLatencyInMs", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::StreamlineLibraryReflex_eventGetGameToRenderLatencyInMs_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::StreamlineLibraryReflex_eventGetGameToRenderLatencyInMs_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execGetGameToRenderLatencyInMs)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UStreamlineLibraryReflex::GetGameToRenderLatencyInMs();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function GetGameToRenderLatencyInMs

// Begin Class UStreamlineLibraryReflex Function GetReflexMode
struct Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics
{
	struct StreamlineLibraryReflex_eventGetReflexMode_Parms
	{
		UStreamlineReflexMode ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Get Reflex mode" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventGetReflexMode_Parms, ReturnValue), Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode, METADATA_PARAMS(0, nullptr) }; // 184094367
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "GetReflexMode", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::StreamlineLibraryReflex_eventGetReflexMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::StreamlineLibraryReflex_eventGetReflexMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execGetReflexMode)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UStreamlineReflexMode*)Z_Param__Result=UStreamlineLibraryReflex::GetReflexMode();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function GetReflexMode

// Begin Class UStreamlineLibraryReflex Function GetRenderLatencyInMs
struct Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics
{
	struct StreamlineLibraryReflex_eventGetRenderLatencyInMs_Parms
	{
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Get Reflex Render Latency (ms)" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventGetRenderLatencyInMs_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "GetRenderLatencyInMs", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::StreamlineLibraryReflex_eventGetRenderLatencyInMs_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::StreamlineLibraryReflex_eventGetRenderLatencyInMs_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execGetRenderLatencyInMs)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UStreamlineLibraryReflex::GetRenderLatencyInMs();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function GetRenderLatencyInMs

// Begin Class UStreamlineLibraryReflex Function IsReflexSupported
struct Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics
{
	struct StreamlineLibraryReflex_eventIsReflexSupported_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Is NVIDIA Reflex Supported" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((StreamlineLibraryReflex_eventIsReflexSupported_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(StreamlineLibraryReflex_eventIsReflexSupported_Parms), &Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "IsReflexSupported", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::StreamlineLibraryReflex_eventIsReflexSupported_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::StreamlineLibraryReflex_eventIsReflexSupported_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execIsReflexSupported)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UStreamlineLibraryReflex::IsReflexSupported();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function IsReflexSupported

// Begin Class UStreamlineLibraryReflex Function QueryReflexSupport
struct Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics
{
	struct StreamlineLibraryReflex_eventQueryReflexSupport_Parms
	{
		UStreamlineFeatureSupport ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Query NVIDIA Reflex Support" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventQueryReflexSupport_Parms, ReturnValue), Z_Construct_UEnum_StreamlineBlueprint_UStreamlineFeatureSupport, METADATA_PARAMS(0, nullptr) }; // 498557622
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "QueryReflexSupport", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::StreamlineLibraryReflex_eventQueryReflexSupport_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::StreamlineLibraryReflex_eventQueryReflexSupport_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execQueryReflexSupport)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UStreamlineFeatureSupport*)Z_Param__Result=UStreamlineLibraryReflex::QueryReflexSupport();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function QueryReflexSupport

// Begin Class UStreamlineLibraryReflex Function SetReflexMode
struct Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics
{
	struct StreamlineLibraryReflex_eventSetReflexMode_Parms
	{
		UStreamlineReflexMode Mode;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Set Reflex mode" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Mode_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_Mode_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Mode;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::NewProp_Mode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::NewProp_Mode = { "Mode", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventSetReflexMode_Parms, Mode), Z_Construct_UEnum_StreamlineBlueprint_UStreamlineReflexMode, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Mode_MetaData), NewProp_Mode_MetaData) }; // 184094367
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::NewProp_Mode_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::NewProp_Mode,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "SetReflexMode", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::StreamlineLibraryReflex_eventSetReflexMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::StreamlineLibraryReflex_eventSetReflexMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execSetReflexMode)
{
	P_GET_ENUM(UStreamlineReflexMode,Z_Param_Mode);
	P_FINISH;
	P_NATIVE_BEGIN;
	UStreamlineLibraryReflex::SetReflexMode(UStreamlineReflexMode(Z_Param_Mode));
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function SetReflexMode

// Begin Class UStreamlineLibraryReflex
void UStreamlineLibraryReflex::StaticRegisterNativesUStreamlineLibraryReflex()
{
	UClass* Class = UStreamlineLibraryReflex::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetDefaultReflexMode", &UStreamlineLibraryReflex::execGetDefaultReflexMode },
		{ "GetGameLatencyInMs", &UStreamlineLibraryReflex::execGetGameLatencyInMs },
		{ "GetGameToRenderLatencyInMs", &UStreamlineLibraryReflex::execGetGameToRenderLatencyInMs },
		{ "GetReflexMode", &UStreamlineLibraryReflex::execGetReflexMode },
		{ "GetRenderLatencyInMs", &UStreamlineLibraryReflex::execGetRenderLatencyInMs },
		{ "IsReflexSupported", &UStreamlineLibraryReflex::execIsReflexSupported },
		{ "QueryReflexSupport", &UStreamlineLibraryReflex::execQueryReflexSupport },
		{ "SetReflexMode", &UStreamlineLibraryReflex::execSetReflexMode },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UStreamlineLibraryReflex);
UClass* Z_Construct_UClass_UStreamlineLibraryReflex_NoRegister()
{
	return UStreamlineLibraryReflex::StaticClass();
}
struct Z_Construct_UClass_UStreamlineLibraryReflex_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "StreamlineLibraryReflex.h" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode, "GetDefaultReflexMode" }, // 648940672
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs, "GetGameLatencyInMs" }, // 3214432446
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs, "GetGameToRenderLatencyInMs" }, // 2037184333
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode, "GetReflexMode" }, // 3407251593
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs, "GetRenderLatencyInMs" }, // 1746736805
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported, "IsReflexSupported" }, // 175652734
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport, "QueryReflexSupport" }, // 421691374
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode, "SetReflexMode" }, // 1014421239
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UStreamlineLibraryReflex>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UStreamlineLibraryReflex_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_StreamlineBlueprint,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UStreamlineLibraryReflex_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UStreamlineLibraryReflex_Statics::ClassParams = {
	&UStreamlineLibraryReflex::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x000800A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UStreamlineLibraryReflex_Statics::Class_MetaDataParams), Z_Construct_UClass_UStreamlineLibraryReflex_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UStreamlineLibraryReflex()
{
	if (!Z_Registration_Info_UClass_UStreamlineLibraryReflex.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UStreamlineLibraryReflex.OuterSingleton, Z_Construct_UClass_UStreamlineLibraryReflex_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UStreamlineLibraryReflex.OuterSingleton;
}
template<> STREAMLINEBLUEPRINT_API UClass* StaticClass<UStreamlineLibraryReflex>()
{
	return UStreamlineLibraryReflex::StaticClass();
}
UStreamlineLibraryReflex::UStreamlineLibraryReflex(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UStreamlineLibraryReflex);
UStreamlineLibraryReflex::~UStreamlineLibraryReflex() {}
// End Class UStreamlineLibraryReflex

// Begin Registration
struct Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_Streamline_Source_StreamlineBlueprint_Public_StreamlineLibraryReflex_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ UStreamlineReflexMode_StaticEnum, TEXT("UStreamlineReflexMode"), &Z_Registration_Info_UEnum_UStreamlineReflexMode, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 184094367U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UStreamlineLibraryReflex, UStreamlineLibraryReflex::StaticClass, TEXT("UStreamlineLibraryReflex"), &Z_Registration_Info_UClass_UStreamlineLibraryReflex, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UStreamlineLibraryReflex), 2027130170U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_Streamline_Source_StreamlineBlueprint_Public_StreamlineLibraryReflex_h_4278566813(TEXT("/Script/StreamlineBlueprint"),
	Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_Streamline_Source_StreamlineBlueprint_Public_StreamlineLibraryReflex_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_Streamline_Source_StreamlineBlueprint_Public_StreamlineLibraryReflex_h_Statics::ClassInfo),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_Streamline_Source_StreamlineBlueprint_Public_StreamlineLibraryReflex_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_Streamline_Source_StreamlineBlueprint_Public_StreamlineLibraryReflex_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
