// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NISBlueprint/Public/NISLibrary.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNISLibrary() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
NISBLUEPRINT_API UClass* Z_Construct_UClass_UNISLibrary();
NISBLUEPRINT_API UClass* Z_Construct_UClass_UNISLibrary_NoRegister();
NISBLUEPRINT_API UEnum* Z_Construct_UEnum_NISBlueprint_UNISMode();
NISBLUEPRINT_API UEnum* Z_Construct_UEnum_NISBlueprint_UNISSupport();
UPackage* Z_Construct_UPackage__Script_NISBlueprint();
// End Cross Module References

// Begin Enum UNISSupport
static FEnumRegistrationInfo Z_Registration_Info_UEnum_UNISSupport;
static UEnum* UNISSupport_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_UNISSupport.OuterSingleton)
	{
		Z_Registration_Info_UEnum_UNISSupport.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_NISBlueprint_UNISSupport, (UObject*)Z_Construct_UPackage__Script_NISBlueprint(), TEXT("UNISSupport"));
	}
	return Z_Registration_Info_UEnum_UNISSupport.OuterSingleton;
}
template<> NISBLUEPRINT_API UEnum* StaticEnum<UNISSupport>()
{
	return UNISSupport_StaticEnum();
}
struct Z_Construct_UEnum_NISBlueprint_UNISSupport_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/NISLibrary.h" },
		{ "NotSupported.DisplayName", "Not Supported due to insufficient RHI Feature Level" },
		{ "NotSupported.Name", "UNISSupport::NotSupported" },
		{ "Supported.DisplayName", "Supported" },
		{ "Supported.Name", "UNISSupport::Supported" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "UNISSupport::Supported", (int64)UNISSupport::Supported },
		{ "UNISSupport::NotSupported", (int64)UNISSupport::NotSupported },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_NISBlueprint_UNISSupport_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_NISBlueprint,
	nullptr,
	"UNISSupport",
	"UNISSupport",
	Z_Construct_UEnum_NISBlueprint_UNISSupport_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_NISBlueprint_UNISSupport_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_NISBlueprint_UNISSupport_Statics::Enum_MetaDataParams), Z_Construct_UEnum_NISBlueprint_UNISSupport_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_NISBlueprint_UNISSupport()
{
	if (!Z_Registration_Info_UEnum_UNISSupport.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_UNISSupport.InnerSingleton, Z_Construct_UEnum_NISBlueprint_UNISSupport_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_UNISSupport.InnerSingleton;
}
// End Enum UNISSupport

// Begin Enum UNISMode
static FEnumRegistrationInfo Z_Registration_Info_UEnum_UNISMode;
static UEnum* UNISMode_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_UNISMode.OuterSingleton)
	{
		Z_Registration_Info_UEnum_UNISMode.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_NISBlueprint_UNISMode, (UObject*)Z_Construct_UPackage__Script_NISBlueprint(), TEXT("UNISMode"));
	}
	return Z_Registration_Info_UEnum_UNISMode.OuterSingleton;
}
template<> NISBLUEPRINT_API UEnum* StaticEnum<UNISMode>()
{
	return UNISMode_StaticEnum();
}
struct Z_Construct_UEnum_NISBlueprint_UNISMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Balanced.DisplayName", "Balanced" },
		{ "Balanced.Name", "UNISMode::Balanced" },
		{ "BlueprintType", "true" },
		{ "Custom.DisplayName", "Custom" },
		{ "Custom.Name", "UNISMode::Custom" },
		{ "ModuleRelativePath", "Public/NISLibrary.h" },
		{ "Off.DisplayName", "Off" },
		{ "Off.Name", "UNISMode::Off" },
		{ "Performance.DisplayName", "Performance" },
		{ "Performance.Name", "UNISMode::Performance" },
		{ "Quality.DisplayName", "Quality" },
		{ "Quality.Name", "UNISMode::Quality" },
		{ "UltraQuality.DisplayName", "Ultra Quality" },
		{ "UltraQuality.Name", "UNISMode::UltraQuality" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "UNISMode::Off", (int64)UNISMode::Off },
		{ "UNISMode::UltraQuality", (int64)UNISMode::UltraQuality },
		{ "UNISMode::Quality", (int64)UNISMode::Quality },
		{ "UNISMode::Balanced", (int64)UNISMode::Balanced },
		{ "UNISMode::Performance", (int64)UNISMode::Performance },
		{ "UNISMode::Custom", (int64)UNISMode::Custom },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_NISBlueprint_UNISMode_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_NISBlueprint,
	nullptr,
	"UNISMode",
	"UNISMode",
	Z_Construct_UEnum_NISBlueprint_UNISMode_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_NISBlueprint_UNISMode_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_NISBlueprint_UNISMode_Statics::Enum_MetaDataParams), Z_Construct_UEnum_NISBlueprint_UNISMode_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_NISBlueprint_UNISMode()
{
	if (!Z_Registration_Info_UEnum_UNISMode.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_UNISMode.InnerSingleton, Z_Construct_UEnum_NISBlueprint_UNISMode_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_UNISMode.InnerSingleton;
}
// End Enum UNISMode

// Begin Class UNISLibrary Function GetDefaultNISMode
struct Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics
{
	struct NISLibrary_eventGetDefaultNISMode_Parms
	{
		UNISMode ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NIS" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "///* Find a reasonable default NIS mode based on current hardware */\n" },
#endif
		{ "DisplayName", "Get Default NIS Mode" },
		{ "ModuleRelativePath", "Public/NISLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Find a reasonable default NIS mode based on current hardware" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NISLibrary_eventGetDefaultNISMode_Parms, ReturnValue), Z_Construct_UEnum_NISBlueprint_UNISMode, METADATA_PARAMS(0, nullptr) }; // 1518071886
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNISLibrary, nullptr, "GetDefaultNISMode", nullptr, nullptr, Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::NISLibrary_eventGetDefaultNISMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::NISLibrary_eventGetDefaultNISMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNISLibrary::execGetDefaultNISMode)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UNISMode*)Z_Param__Result=UNISLibrary::GetDefaultNISMode();
	P_NATIVE_END;
}
// End Class UNISLibrary Function GetDefaultNISMode

// Begin Class UNISLibrary Function GetNISRecommendedScreenPercentage
struct Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics
{
	struct NISLibrary_eventGetNISRecommendedScreenPercentage_Parms
	{
		UNISMode NISMode;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NIS" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Returns the recommended screen percentage for a given NIS mode. Returns CustomScreenPercentage if NISMode is UNISMode::Custom */" },
#endif
		{ "DisplayName", "Get NIS Recommended Screen Percentage" },
		{ "ModuleRelativePath", "Public/NISLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Returns the recommended screen percentage for a given NIS mode. Returns CustomScreenPercentage if NISMode is UNISMode::Custom" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_NISMode_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_NISMode;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::NewProp_NISMode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::NewProp_NISMode = { "NISMode", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NISLibrary_eventGetNISRecommendedScreenPercentage_Parms, NISMode), Z_Construct_UEnum_NISBlueprint_UNISMode, METADATA_PARAMS(0, nullptr) }; // 1518071886
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NISLibrary_eventGetNISRecommendedScreenPercentage_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::NewProp_NISMode_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::NewProp_NISMode,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNISLibrary, nullptr, "GetNISRecommendedScreenPercentage", nullptr, nullptr, Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::NISLibrary_eventGetNISRecommendedScreenPercentage_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::NISLibrary_eventGetNISRecommendedScreenPercentage_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNISLibrary::execGetNISRecommendedScreenPercentage)
{
	P_GET_ENUM(UNISMode,Z_Param_NISMode);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UNISLibrary::GetNISRecommendedScreenPercentage(UNISMode(Z_Param_NISMode));
	P_NATIVE_END;
}
// End Class UNISLibrary Function GetNISRecommendedScreenPercentage

// Begin Class UNISLibrary Function GetNISScreenPercentageRange
struct Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics
{
	struct NISLibrary_eventGetNISScreenPercentageRange_Parms
	{
		float MinScreenPercentage;
		float MaxScreenPercentage;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NIS" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** The global screen percentage range that NIS supports. */" },
#endif
		{ "DisplayName", "Get NIS Screen Percentage Range" },
		{ "ModuleRelativePath", "Public/NISLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The global screen percentage range that NIS supports." },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MinScreenPercentage;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxScreenPercentage;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::NewProp_MinScreenPercentage = { "MinScreenPercentage", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NISLibrary_eventGetNISScreenPercentageRange_Parms, MinScreenPercentage), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::NewProp_MaxScreenPercentage = { "MaxScreenPercentage", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NISLibrary_eventGetNISScreenPercentageRange_Parms, MaxScreenPercentage), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::NewProp_MinScreenPercentage,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::NewProp_MaxScreenPercentage,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNISLibrary, nullptr, "GetNISScreenPercentageRange", nullptr, nullptr, Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::NISLibrary_eventGetNISScreenPercentageRange_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14422403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::NISLibrary_eventGetNISScreenPercentageRange_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNISLibrary::execGetNISScreenPercentageRange)
{
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_MinScreenPercentage);
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_MaxScreenPercentage);
	P_FINISH;
	P_NATIVE_BEGIN;
	UNISLibrary::GetNISScreenPercentageRange(Z_Param_Out_MinScreenPercentage,Z_Param_Out_MaxScreenPercentage);
	P_NATIVE_END;
}
// End Class UNISLibrary Function GetNISScreenPercentageRange

// Begin Class UNISLibrary Function GetSupportedNISModes
struct Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics
{
	struct NISLibrary_eventGetSupportedNISModes_Parms
	{
		TArray<UNISMode> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NIS" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Retrieves all supported NIS modes. Can be used to populate UI */" },
#endif
		{ "DisplayName", "Get Supported NIS Modes" },
		{ "ModuleRelativePath", "Public/NISLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Retrieves all supported NIS modes. Can be used to populate UI" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Inner_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::NewProp_ReturnValue_Inner_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_NISBlueprint_UNISMode, METADATA_PARAMS(0, nullptr) }; // 1518071886
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NISLibrary_eventGetSupportedNISModes_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 1518071886
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::NewProp_ReturnValue_Inner_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNISLibrary, nullptr, "GetSupportedNISModes", nullptr, nullptr, Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::NISLibrary_eventGetSupportedNISModes_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::NISLibrary_eventGetSupportedNISModes_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNISLibrary::execGetSupportedNISModes)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<UNISMode>*)Z_Param__Result=UNISLibrary::GetSupportedNISModes();
	P_NATIVE_END;
}
// End Class UNISLibrary Function GetSupportedNISModes

// Begin Class UNISLibrary Function IsNISModeSupported
struct Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics
{
	struct NISLibrary_eventIsNISModeSupported_Parms
	{
		UNISMode NISMode;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NIS" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Checks whether a NIS mode is supported */" },
#endif
		{ "DisplayName", "Is NIS Mode Supported" },
		{ "ModuleRelativePath", "Public/NISLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Checks whether a NIS mode is supported" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_NISMode_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_NISMode;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::NewProp_NISMode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::NewProp_NISMode = { "NISMode", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NISLibrary_eventIsNISModeSupported_Parms, NISMode), Z_Construct_UEnum_NISBlueprint_UNISMode, METADATA_PARAMS(0, nullptr) }; // 1518071886
void Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((NISLibrary_eventIsNISModeSupported_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(NISLibrary_eventIsNISModeSupported_Parms), &Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::NewProp_NISMode_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::NewProp_NISMode,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNISLibrary, nullptr, "IsNISModeSupported", nullptr, nullptr, Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::NISLibrary_eventIsNISModeSupported_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::NISLibrary_eventIsNISModeSupported_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNISLibrary_IsNISModeSupported()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNISLibrary_IsNISModeSupported_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNISLibrary::execIsNISModeSupported)
{
	P_GET_ENUM(UNISMode,Z_Param_NISMode);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UNISLibrary::IsNISModeSupported(UNISMode(Z_Param_NISMode));
	P_NATIVE_END;
}
// End Class UNISLibrary Function IsNISModeSupported

// Begin Class UNISLibrary Function IsNISSupported
struct Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics
{
	struct NISLibrary_eventIsNISSupported_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NIS" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Checks whether NIS is supported by the current GPU. Further details can be retrieved via QueryNISSupport*/" },
#endif
		{ "DisplayName", "Is NVIDIA NIS Supported" },
		{ "ModuleRelativePath", "Public/NISLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Checks whether NIS is supported by the current GPU. Further details can be retrieved via QueryNISSupport" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((NISLibrary_eventIsNISSupported_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(NISLibrary_eventIsNISSupported_Parms), &Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNISLibrary, nullptr, "IsNISSupported", nullptr, nullptr, Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::NISLibrary_eventIsNISSupported_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::NISLibrary_eventIsNISSupported_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNISLibrary_IsNISSupported()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNISLibrary_IsNISSupported_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNISLibrary::execIsNISSupported)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UNISLibrary::IsNISSupported();
	P_NATIVE_END;
}
// End Class UNISLibrary Function IsNISSupported

// Begin Class UNISLibrary Function SetNISCustomScreenPercentage
struct Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics
{
	struct NISLibrary_eventSetNISCustomScreenPercentage_Parms
	{
		float CustomScreenPercentage;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NIS" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Set the screen percentage used for Custom mode (100% by default) */" },
#endif
		{ "CPP_Default_CustomScreenPercentage", "100.000000" },
		{ "DisplayName", "Set NIS Custom Screen Percentage" },
		{ "ModuleRelativePath", "Public/NISLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Set the screen percentage used for Custom mode (100% by default)" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_CustomScreenPercentage;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics::NewProp_CustomScreenPercentage = { "CustomScreenPercentage", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NISLibrary_eventSetNISCustomScreenPercentage_Parms, CustomScreenPercentage), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics::NewProp_CustomScreenPercentage,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNISLibrary, nullptr, "SetNISCustomScreenPercentage", nullptr, nullptr, Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics::NISLibrary_eventSetNISCustomScreenPercentage_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics::NISLibrary_eventSetNISCustomScreenPercentage_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNISLibrary::execSetNISCustomScreenPercentage)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_CustomScreenPercentage);
	P_FINISH;
	P_NATIVE_BEGIN;
	UNISLibrary::SetNISCustomScreenPercentage(Z_Param_CustomScreenPercentage);
	P_NATIVE_END;
}
// End Class UNISLibrary Function SetNISCustomScreenPercentage

// Begin Class UNISLibrary Function SetNISMode
struct Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics
{
	struct NISLibrary_eventSetNISMode_Parms
	{
		UNISMode NISMode;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NIS" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Sets the console variables to enable/disable NIS (r.NIS.Enable, r.NIS.Upscaling, r.ScreenPercentage, r.TemporalAA.Upsampling, r.TemporalAA.Upscaler)*/" },
#endif
		{ "DisplayName", "Set NIS Mode" },
		{ "ModuleRelativePath", "Public/NISLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Sets the console variables to enable/disable NIS (r.NIS.Enable, r.NIS.Upscaling, r.ScreenPercentage, r.TemporalAA.Upsampling, r.TemporalAA.Upscaler)" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_NISMode_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_NISMode;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::NewProp_NISMode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::NewProp_NISMode = { "NISMode", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NISLibrary_eventSetNISMode_Parms, NISMode), Z_Construct_UEnum_NISBlueprint_UNISMode, METADATA_PARAMS(0, nullptr) }; // 1518071886
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::NewProp_NISMode_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::NewProp_NISMode,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNISLibrary, nullptr, "SetNISMode", nullptr, nullptr, Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::NISLibrary_eventSetNISMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::NISLibrary_eventSetNISMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNISLibrary_SetNISMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNISLibrary_SetNISMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNISLibrary::execSetNISMode)
{
	P_GET_ENUM(UNISMode,Z_Param_NISMode);
	P_FINISH;
	P_NATIVE_BEGIN;
	UNISLibrary::SetNISMode(UNISMode(Z_Param_NISMode));
	P_NATIVE_END;
}
// End Class UNISLibrary Function SetNISMode

// Begin Class UNISLibrary Function SetNISSharpness
struct Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics
{
	struct NISLibrary_eventSetNISSharpness_Parms
	{
		float Sharpness;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NIS" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/* Sets the console variables to enable additional NIS sharpening. Set to 0 to disable (r.NGX.NIS.Sharpness) */" },
#endif
		{ "DisplayName", "Set NIS Sharpness" },
		{ "ModuleRelativePath", "Public/NISLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Sets the console variables to enable additional NIS sharpening. Set to 0 to disable (r.NGX.NIS.Sharpness)" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Sharpness;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics::NewProp_Sharpness = { "Sharpness", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NISLibrary_eventSetNISSharpness_Parms, Sharpness), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics::NewProp_Sharpness,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UNISLibrary, nullptr, "SetNISSharpness", nullptr, nullptr, Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics::PropPointers), sizeof(Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics::NISLibrary_eventSetNISSharpness_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics::NISLibrary_eventSetNISSharpness_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNISLibrary_SetNISSharpness()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNISLibrary_SetNISSharpness_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNISLibrary::execSetNISSharpness)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_Sharpness);
	P_FINISH;
	P_NATIVE_BEGIN;
	UNISLibrary::SetNISSharpness(Z_Param_Sharpness);
	P_NATIVE_END;
}
// End Class UNISLibrary Function SetNISSharpness

// Begin Class UNISLibrary
void UNISLibrary::StaticRegisterNativesUNISLibrary()
{
	UClass* Class = UNISLibrary::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetDefaultNISMode", &UNISLibrary::execGetDefaultNISMode },
		{ "GetNISRecommendedScreenPercentage", &UNISLibrary::execGetNISRecommendedScreenPercentage },
		{ "GetNISScreenPercentageRange", &UNISLibrary::execGetNISScreenPercentageRange },
		{ "GetSupportedNISModes", &UNISLibrary::execGetSupportedNISModes },
		{ "IsNISModeSupported", &UNISLibrary::execIsNISModeSupported },
		{ "IsNISSupported", &UNISLibrary::execIsNISSupported },
		{ "SetNISCustomScreenPercentage", &UNISLibrary::execSetNISCustomScreenPercentage },
		{ "SetNISMode", &UNISLibrary::execSetNISMode },
		{ "SetNISSharpness", &UNISLibrary::execSetNISSharpness },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UNISLibrary);
UClass* Z_Construct_UClass_UNISLibrary_NoRegister()
{
	return UNISLibrary::StaticClass();
}
struct Z_Construct_UClass_UNISLibrary_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "NISLibrary.h" },
		{ "ModuleRelativePath", "Public/NISLibrary.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UNISLibrary_GetDefaultNISMode, "GetDefaultNISMode" }, // 3453906264
		{ &Z_Construct_UFunction_UNISLibrary_GetNISRecommendedScreenPercentage, "GetNISRecommendedScreenPercentage" }, // 119956211
		{ &Z_Construct_UFunction_UNISLibrary_GetNISScreenPercentageRange, "GetNISScreenPercentageRange" }, // 2976613432
		{ &Z_Construct_UFunction_UNISLibrary_GetSupportedNISModes, "GetSupportedNISModes" }, // 2198815062
		{ &Z_Construct_UFunction_UNISLibrary_IsNISModeSupported, "IsNISModeSupported" }, // 1862229451
		{ &Z_Construct_UFunction_UNISLibrary_IsNISSupported, "IsNISSupported" }, // 1910983314
		{ &Z_Construct_UFunction_UNISLibrary_SetNISCustomScreenPercentage, "SetNISCustomScreenPercentage" }, // 793877913
		{ &Z_Construct_UFunction_UNISLibrary_SetNISMode, "SetNISMode" }, // 1988042442
		{ &Z_Construct_UFunction_UNISLibrary_SetNISSharpness, "SetNISSharpness" }, // 3638504421
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UNISLibrary>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UNISLibrary_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_NISBlueprint,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNISLibrary_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UNISLibrary_Statics::ClassParams = {
	&UNISLibrary::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UNISLibrary_Statics::Class_MetaDataParams), Z_Construct_UClass_UNISLibrary_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UNISLibrary()
{
	if (!Z_Registration_Info_UClass_UNISLibrary.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UNISLibrary.OuterSingleton, Z_Construct_UClass_UNISLibrary_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UNISLibrary.OuterSingleton;
}
template<> NISBLUEPRINT_API UClass* StaticClass<UNISLibrary>()
{
	return UNISLibrary::StaticClass();
}
UNISLibrary::UNISLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UNISLibrary);
UNISLibrary::~UNISLibrary() {}
// End Class UNISLibrary

// Begin Registration
struct Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_NIS_Source_NISBlueprint_Public_NISLibrary_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ UNISSupport_StaticEnum, TEXT("UNISSupport"), &Z_Registration_Info_UEnum_UNISSupport, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 237391673U) },
		{ UNISMode_StaticEnum, TEXT("UNISMode"), &Z_Registration_Info_UEnum_UNISMode, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1518071886U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UNISLibrary, UNISLibrary::StaticClass, TEXT("UNISLibrary"), &Z_Registration_Info_UClass_UNISLibrary, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNISLibrary), 1259421941U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_NIS_Source_NISBlueprint_Public_NISLibrary_h_3636155553(TEXT("/Script/NISBlueprint"),
	Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_NIS_Source_NISBlueprint_Public_NISLibrary_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_NIS_Source_NISBlueprint_Public_NISLibrary_h_Statics::ClassInfo),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_NIS_Source_NISBlueprint_Public_NISLibrary_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_FPEACE_Plugins_Marketplace_NIS_Source_NISBlueprint_Public_NISLibrary_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
