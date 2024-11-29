// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "StreamlineBlueprint/Public/StreamlineLibraryDLSSG.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeStreamlineLibraryDLSSG() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
STREAMLINEBLUEPRINT_API UClass* Z_Construct_UClass_UStreamlineLibraryDLSSG();
STREAMLINEBLUEPRINT_API UClass* Z_Construct_UClass_UStreamlineLibraryDLSSG_NoRegister();
STREAMLINEBLUEPRINT_API UEnum* Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode();
STREAMLINEBLUEPRINT_API UEnum* Z_Construct_UEnum_StreamlineBlueprint_UStreamlineFeatureSupport();
UPackage* Z_Construct_UPackage__Script_StreamlineBlueprint();
// End Cross Module References

// Begin Enum UStreamlineDLSSGMode
static FEnumRegistrationInfo Z_Registration_Info_UEnum_UStreamlineDLSSGMode;
static UEnum* UStreamlineDLSSGMode_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_UStreamlineDLSSGMode.OuterSingleton)
	{
		Z_Registration_Info_UEnum_UStreamlineDLSSGMode.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode, (UObject*)Z_Construct_UPackage__Script_StreamlineBlueprint(), TEXT("UStreamlineDLSSGMode"));
	}
	return Z_Registration_Info_UEnum_UStreamlineDLSSGMode.OuterSingleton;
}
template<> STREAMLINEBLUEPRINT_API UEnum* StaticEnum<UStreamlineDLSSGMode>()
{
	return UStreamlineDLSSGMode_StaticEnum();
}
struct Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Auto.DisplayName", "Auto" },
		{ "Auto.Name", "UStreamlineDLSSGMode::Auto" },
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDLSSG.h" },
		{ "Off.DisplayName", "Off" },
		{ "Off.Name", "UStreamlineDLSSGMode::Off" },
		{ "On.DisplayName", "On" },
		{ "On.Name", "UStreamlineDLSSGMode::On" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "UStreamlineDLSSGMode::Off", (int64)UStreamlineDLSSGMode::Off },
		{ "UStreamlineDLSSGMode::On", (int64)UStreamlineDLSSGMode::On },
		{ "UStreamlineDLSSGMode::Auto", (int64)UStreamlineDLSSGMode::Auto },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_StreamlineBlueprint,
	nullptr,
	"UStreamlineDLSSGMode",
	"UStreamlineDLSSGMode",
	Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode_Statics::Enum_MetaDataParams), Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode()
{
	if (!Z_Registration_Info_UEnum_UStreamlineDLSSGMode.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_UStreamlineDLSSGMode.InnerSingleton, Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_UStreamlineDLSSGMode.InnerSingleton;
}
// End Enum UStreamlineDLSSGMode

// Begin Class UStreamlineLibraryDLSSG Function GetDefaultDLSSGMode
struct Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics
{
	struct StreamlineLibraryDLSSG_eventGetDefaultDLSSGMode_Parms
	{
		UStreamlineDLSSGMode ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DLSS-FG" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/* Find a reasonable default DLSS-FG mode based on current hardware */" },
#endif
		{ "DisplayName", "Get Default DLSS-FG Mode" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDLSSG.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Find a reasonable default DLSS-FG mode based on current hardware" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDLSSG_eventGetDefaultDLSSGMode_Parms, ReturnValue), Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode, METADATA_PARAMS(0, nullptr) }; // 41788211
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDLSSG, nullptr, "GetDefaultDLSSGMode", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::StreamlineLibraryDLSSG_eventGetDefaultDLSSGMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::StreamlineLibraryDLSSG_eventGetDefaultDLSSGMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDLSSG::execGetDefaultDLSSGMode)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UStreamlineDLSSGMode*)Z_Param__Result=UStreamlineLibraryDLSSG::GetDefaultDLSSGMode();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDLSSG Function GetDefaultDLSSGMode

// Begin Class UStreamlineLibraryDLSSG Function GetDLSSGFrameTiming
struct Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics
{
	struct StreamlineLibraryDLSSG_eventGetDLSSGFrameTiming_Parms
	{
		float FrameRateInHertz;
		int32 FramesPresented;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DLSS-FG" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/* Returns the actual framerate and number of frames presented, whether DLSS-FG is active or not */" },
#endif
		{ "DisplayName", "Get DLSS-FG  frame rate and presented frames" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDLSSG.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Returns the actual framerate and number of frames presented, whether DLSS-FG is active or not" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_FrameRateInHertz;
	static const UECodeGen_Private::FIntPropertyParams NewProp_FramesPresented;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::NewProp_FrameRateInHertz = { "FrameRateInHertz", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDLSSG_eventGetDLSSGFrameTiming_Parms, FrameRateInHertz), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::NewProp_FramesPresented = { "FramesPresented", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDLSSG_eventGetDLSSGFrameTiming_Parms, FramesPresented), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::NewProp_FrameRateInHertz,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::NewProp_FramesPresented,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDLSSG, nullptr, "GetDLSSGFrameTiming", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::StreamlineLibraryDLSSG_eventGetDLSSGFrameTiming_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14422403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::StreamlineLibraryDLSSG_eventGetDLSSGFrameTiming_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDLSSG::execGetDLSSGFrameTiming)
{
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_FrameRateInHertz);
	P_GET_PROPERTY_REF(FIntProperty,Z_Param_Out_FramesPresented);
	P_FINISH;
	P_NATIVE_BEGIN;
	UStreamlineLibraryDLSSG::GetDLSSGFrameTiming(Z_Param_Out_FrameRateInHertz,Z_Param_Out_FramesPresented);
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDLSSG Function GetDLSSGFrameTiming

// Begin Class UStreamlineLibraryDLSSG Function GetDLSSGMode
struct Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics
{
	struct StreamlineLibraryDLSSG_eventGetDLSSGMode_Parms
	{
		UStreamlineDLSSGMode ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DLSS-FG" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/* Reads the console variables to infer the current DLSS-FG mode*/" },
#endif
		{ "DisplayName", "Get DLSS-FG Mode" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDLSSG.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Reads the console variables to infer the current DLSS-FG mode" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDLSSG_eventGetDLSSGMode_Parms, ReturnValue), Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode, METADATA_PARAMS(0, nullptr) }; // 41788211
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDLSSG, nullptr, "GetDLSSGMode", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::StreamlineLibraryDLSSG_eventGetDLSSGMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::StreamlineLibraryDLSSG_eventGetDLSSGMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDLSSG::execGetDLSSGMode)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UStreamlineDLSSGMode*)Z_Param__Result=UStreamlineLibraryDLSSG::GetDLSSGMode();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDLSSG Function GetDLSSGMode

// Begin Class UStreamlineLibraryDLSSG Function GetSupportedDLSSGModes
struct Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics
{
	struct StreamlineLibraryDLSSG_eventGetSupportedDLSSGModes_Parms
	{
		TArray<UStreamlineDLSSGMode> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DLSS-FG" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Retrieves all supported DLSS-FG modes. Can be used to populate UI */" },
#endif
		{ "DisplayName", "Get Supported DLSS-FG Modes" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDLSSG.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Retrieves all supported DLSS-FG modes. Can be used to populate UI" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Inner_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::NewProp_ReturnValue_Inner_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode, METADATA_PARAMS(0, nullptr) }; // 41788211
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDLSSG_eventGetSupportedDLSSGModes_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 41788211
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::NewProp_ReturnValue_Inner_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDLSSG, nullptr, "GetSupportedDLSSGModes", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::StreamlineLibraryDLSSG_eventGetSupportedDLSSGModes_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::StreamlineLibraryDLSSG_eventGetSupportedDLSSGModes_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDLSSG::execGetSupportedDLSSGModes)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<UStreamlineDLSSGMode>*)Z_Param__Result=UStreamlineLibraryDLSSG::GetSupportedDLSSGModes();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDLSSG Function GetSupportedDLSSGModes

// Begin Class UStreamlineLibraryDLSSG Function IsDLSSGModeSupported
struct Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics
{
	struct StreamlineLibraryDLSSG_eventIsDLSSGModeSupported_Parms
	{
		UStreamlineDLSSGMode DLSSGMode;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DLSS-FG" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Checks whether a DLSS-FG mode is supported */" },
#endif
		{ "DisplayName", "Is DLSS-FG Mode Supported" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDLSSG.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Checks whether a DLSS-FG mode is supported" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_DLSSGMode_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DLSSGMode;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::NewProp_DLSSGMode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::NewProp_DLSSGMode = { "DLSSGMode", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDLSSG_eventIsDLSSGModeSupported_Parms, DLSSGMode), Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode, METADATA_PARAMS(0, nullptr) }; // 41788211
void Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((StreamlineLibraryDLSSG_eventIsDLSSGModeSupported_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(StreamlineLibraryDLSSG_eventIsDLSSGModeSupported_Parms), &Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::NewProp_DLSSGMode_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::NewProp_DLSSGMode,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDLSSG, nullptr, "IsDLSSGModeSupported", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::StreamlineLibraryDLSSG_eventIsDLSSGModeSupported_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::StreamlineLibraryDLSSG_eventIsDLSSGModeSupported_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDLSSG::execIsDLSSGModeSupported)
{
	P_GET_ENUM(UStreamlineDLSSGMode,Z_Param_DLSSGMode);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UStreamlineLibraryDLSSG::IsDLSSGModeSupported(UStreamlineDLSSGMode(Z_Param_DLSSGMode));
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDLSSG Function IsDLSSGModeSupported

// Begin Class UStreamlineLibraryDLSSG Function IsDLSSGSupported
struct Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics
{
	struct StreamlineLibraryDLSSG_eventIsDLSSGSupported_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DLSS-FG" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Checks whether DLSS-FG is supported by the current GPU. Further details can be retrieved via QueryDLSSGSupport*/" },
#endif
		{ "DisplayName", "Is NVIDIA DLSS-FG Supported" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDLSSG.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Checks whether DLSS-FG is supported by the current GPU. Further details can be retrieved via QueryDLSSGSupport" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((StreamlineLibraryDLSSG_eventIsDLSSGSupported_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(StreamlineLibraryDLSSG_eventIsDLSSGSupported_Parms), &Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDLSSG, nullptr, "IsDLSSGSupported", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::StreamlineLibraryDLSSG_eventIsDLSSGSupported_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::StreamlineLibraryDLSSG_eventIsDLSSGSupported_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDLSSG::execIsDLSSGSupported)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UStreamlineLibraryDLSSG::IsDLSSGSupported();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDLSSG Function IsDLSSGSupported

// Begin Class UStreamlineLibraryDLSSG Function QueryDLSSGSupport
struct Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics
{
	struct StreamlineLibraryDLSSG_eventQueryDLSSGSupport_Parms
	{
		UStreamlineFeatureSupport ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DLSS-FG" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Checks whether DLSS-FG is supported by the current GPU\x09*/" },
#endif
		{ "DisplayName", "Query NVIDIA DLSS-FG Support" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDLSSG.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Checks whether DLSS-FG is supported by the current GPU" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDLSSG_eventQueryDLSSGSupport_Parms, ReturnValue), Z_Construct_UEnum_StreamlineBlueprint_UStreamlineFeatureSupport, METADATA_PARAMS(0, nullptr) }; // 498557622
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDLSSG, nullptr, "QueryDLSSGSupport", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::StreamlineLibraryDLSSG_eventQueryDLSSGSupport_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::StreamlineLibraryDLSSG_eventQueryDLSSGSupport_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDLSSG::execQueryDLSSGSupport)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UStreamlineFeatureSupport*)Z_Param__Result=UStreamlineLibraryDLSSG::QueryDLSSGSupport();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDLSSG Function QueryDLSSGSupport

// Begin Class UStreamlineLibraryDLSSG Function SetDLSSGMode
struct Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics
{
	struct StreamlineLibraryDLSSG_eventSetDLSSGMode_Parms
	{
		UStreamlineDLSSGMode DLSSGMode;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DLSS-FG" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09 * Sets the console variables to enable/disable DLSS-FG\n\x09 * Off = DLSS-FG disabled\n\x09 * On = DLSS-FG always enabled\n\x09 * Auto = DLSS-FG may be temporarily disabled if it could hurt frame rate\n\x09 */" },
#endif
		{ "DisplayName", "Set DLSS-FG Mode" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDLSSG.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Sets the console variables to enable/disable DLSS-FG\nOff = DLSS-FG disabled\nOn = DLSS-FG always enabled\nAuto = DLSS-FG may be temporarily disabled if it could hurt frame rate" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_DLSSGMode_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DLSSGMode;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::NewProp_DLSSGMode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::NewProp_DLSSGMode = { "DLSSGMode", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDLSSG_eventSetDLSSGMode_Parms, DLSSGMode), Z_Construct_UEnum_StreamlineBlueprint_UStreamlineDLSSGMode, METADATA_PARAMS(0, nullptr) }; // 41788211
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::NewProp_DLSSGMode_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::NewProp_DLSSGMode,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDLSSG, nullptr, "SetDLSSGMode", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::StreamlineLibraryDLSSG_eventSetDLSSGMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::StreamlineLibraryDLSSG_eventSetDLSSGMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDLSSG::execSetDLSSGMode)
{
	P_GET_ENUM(UStreamlineDLSSGMode,Z_Param_DLSSGMode);
	P_FINISH;
	P_NATIVE_BEGIN;
	UStreamlineLibraryDLSSG::SetDLSSGMode(UStreamlineDLSSGMode(Z_Param_DLSSGMode));
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDLSSG Function SetDLSSGMode

// Begin Class UStreamlineLibraryDLSSG
void UStreamlineLibraryDLSSG::StaticRegisterNativesUStreamlineLibraryDLSSG()
{
	UClass* Class = UStreamlineLibraryDLSSG::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetDefaultDLSSGMode", &UStreamlineLibraryDLSSG::execGetDefaultDLSSGMode },
		{ "GetDLSSGFrameTiming", &UStreamlineLibraryDLSSG::execGetDLSSGFrameTiming },
		{ "GetDLSSGMode", &UStreamlineLibraryDLSSG::execGetDLSSGMode },
		{ "GetSupportedDLSSGModes", &UStreamlineLibraryDLSSG::execGetSupportedDLSSGModes },
		{ "IsDLSSGModeSupported", &UStreamlineLibraryDLSSG::execIsDLSSGModeSupported },
		{ "IsDLSSGSupported", &UStreamlineLibraryDLSSG::execIsDLSSGSupported },
		{ "QueryDLSSGSupport", &UStreamlineLibraryDLSSG::execQueryDLSSGSupport },
		{ "SetDLSSGMode", &UStreamlineLibraryDLSSG::execSetDLSSGMode },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UStreamlineLibraryDLSSG);
UClass* Z_Construct_UClass_UStreamlineLibraryDLSSG_NoRegister()
{
	return UStreamlineLibraryDLSSG::StaticClass();
}
struct Z_Construct_UClass_UStreamlineLibraryDLSSG_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "StreamlineLibraryDLSSG.h" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDLSSG.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDefaultDLSSGMode, "GetDefaultDLSSGMode" }, // 3095129405
		{ &Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGFrameTiming, "GetDLSSGFrameTiming" }, // 1382957383
		{ &Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetDLSSGMode, "GetDLSSGMode" }, // 3207225596
		{ &Z_Construct_UFunction_UStreamlineLibraryDLSSG_GetSupportedDLSSGModes, "GetSupportedDLSSGModes" }, // 204515581
		{ &Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGModeSupported, "IsDLSSGModeSupported" }, // 906641460
		{ &Z_Construct_UFunction_UStreamlineLibraryDLSSG_IsDLSSGSupported, "IsDLSSGSupported" }, // 3715158822
		{ &Z_Construct_UFunction_UStreamlineLibraryDLSSG_QueryDLSSGSupport, "QueryDLSSGSupport" }, // 3211671746
		{ &Z_Construct_UFunction_UStreamlineLibraryDLSSG_SetDLSSGMode, "SetDLSSGMode" }, // 3868245865
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UStreamlineLibraryDLSSG>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UStreamlineLibraryDLSSG_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_StreamlineBlueprint,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UStreamlineLibraryDLSSG_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UStreamlineLibraryDLSSG_Statics::ClassParams = {
	&UStreamlineLibraryDLSSG::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UStreamlineLibraryDLSSG_Statics::Class_MetaDataParams), Z_Construct_UClass_UStreamlineLibraryDLSSG_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UStreamlineLibraryDLSSG()
{
	if (!Z_Registration_Info_UClass_UStreamlineLibraryDLSSG.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UStreamlineLibraryDLSSG.OuterSingleton, Z_Construct_UClass_UStreamlineLibraryDLSSG_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UStreamlineLibraryDLSSG.OuterSingleton;
}
template<> STREAMLINEBLUEPRINT_API UClass* StaticClass<UStreamlineLibraryDLSSG>()
{
	return UStreamlineLibraryDLSSG::StaticClass();
}
UStreamlineLibraryDLSSG::UStreamlineLibraryDLSSG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UStreamlineLibraryDLSSG);
UStreamlineLibraryDLSSG::~UStreamlineLibraryDLSSG() {}
// End Class UStreamlineLibraryDLSSG

// Begin Registration
struct Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_Streamline_Source_StreamlineBlueprint_Public_StreamlineLibraryDLSSG_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ UStreamlineDLSSGMode_StaticEnum, TEXT("UStreamlineDLSSGMode"), &Z_Registration_Info_UEnum_UStreamlineDLSSGMode, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 41788211U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UStreamlineLibraryDLSSG, UStreamlineLibraryDLSSG::StaticClass, TEXT("UStreamlineLibraryDLSSG"), &Z_Registration_Info_UClass_UStreamlineLibraryDLSSG, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UStreamlineLibraryDLSSG), 1704483892U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_Streamline_Source_StreamlineBlueprint_Public_StreamlineLibraryDLSSG_h_533856441(TEXT("/Script/StreamlineBlueprint"),
	Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_Streamline_Source_StreamlineBlueprint_Public_StreamlineLibraryDLSSG_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_Streamline_Source_StreamlineBlueprint_Public_StreamlineLibraryDLSSG_h_Statics::ClassInfo),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_Streamline_Source_StreamlineBlueprint_Public_StreamlineLibraryDLSSG_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_Streamline_Source_StreamlineBlueprint_Public_StreamlineLibraryDLSSG_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
