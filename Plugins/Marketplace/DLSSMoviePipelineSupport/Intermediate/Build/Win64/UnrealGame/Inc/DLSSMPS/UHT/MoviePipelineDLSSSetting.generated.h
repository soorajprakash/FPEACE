// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "MoviePipelineDLSSSetting.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef DLSSMOVIEPIPELINESUPPORT_MoviePipelineDLSSSetting_generated_h
#error "MoviePipelineDLSSSetting.generated.h already included, missing '#pragma once' in MoviePipelineDLSSSetting.h"
#endif
#define DLSSMOVIEPIPELINESUPPORT_MoviePipelineDLSSSetting_generated_h

#define FID_FPEACE_Plugins_Marketplace_DLSSMoviePipelineSupport_Source_DLSSMoviePipelineSupport_Public_MoviePipelineDLSSSetting_h_33_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUMoviePipelineDLSSSetting(); \
	friend struct Z_Construct_UClass_UMoviePipelineDLSSSetting_Statics; \
public: \
	DECLARE_CLASS(UMoviePipelineDLSSSetting, UMoviePipelineViewFamilySetting, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/DLSSMoviePipelineSupport"), NO_API) \
	DECLARE_SERIALIZER(UMoviePipelineDLSSSetting)


#define FID_FPEACE_Plugins_Marketplace_DLSSMoviePipelineSupport_Source_DLSSMoviePipelineSupport_Public_MoviePipelineDLSSSetting_h_33_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UMoviePipelineDLSSSetting(UMoviePipelineDLSSSetting&&); \
	UMoviePipelineDLSSSetting(const UMoviePipelineDLSSSetting&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UMoviePipelineDLSSSetting); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UMoviePipelineDLSSSetting); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UMoviePipelineDLSSSetting) \
	NO_API virtual ~UMoviePipelineDLSSSetting();


#define FID_FPEACE_Plugins_Marketplace_DLSSMoviePipelineSupport_Source_DLSSMoviePipelineSupport_Public_MoviePipelineDLSSSetting_h_30_PROLOG
#define FID_FPEACE_Plugins_Marketplace_DLSSMoviePipelineSupport_Source_DLSSMoviePipelineSupport_Public_MoviePipelineDLSSSetting_h_33_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_FPEACE_Plugins_Marketplace_DLSSMoviePipelineSupport_Source_DLSSMoviePipelineSupport_Public_MoviePipelineDLSSSetting_h_33_INCLASS_NO_PURE_DECLS \
	FID_FPEACE_Plugins_Marketplace_DLSSMoviePipelineSupport_Source_DLSSMoviePipelineSupport_Public_MoviePipelineDLSSSetting_h_33_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> DLSSMOVIEPIPELINESUPPORT_API UClass* StaticClass<class UMoviePipelineDLSSSetting>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_FPEACE_Plugins_Marketplace_DLSSMoviePipelineSupport_Source_DLSSMoviePipelineSupport_Public_MoviePipelineDLSSSetting_h


#define FOREACH_ENUM_EMOVIEPIPELINEDLSSQUALITY(op) \
	op(EMoviePipelineDLSSQuality::EMoviePipelineDLSSQuality_DLAA) \
	op(EMoviePipelineDLSSQuality::EMoviePipelineDLSSQuality_UltraQuality) \
	op(EMoviePipelineDLSSQuality::EMoviePipelineDLSSQuality_Quality) \
	op(EMoviePipelineDLSSQuality::EMoviePipelineDLSSQuality_Balanced) \
	op(EMoviePipelineDLSSQuality::EMoviePipelineDLSSQuality_Performance) \
	op(EMoviePipelineDLSSQuality::EMoviePipelineDLSSQuality_UltraPerformance) 

enum class EMoviePipelineDLSSQuality : uint8;
template<> struct TIsUEnumClass<EMoviePipelineDLSSQuality> { enum { Value = true }; };
template<> DLSSMOVIEPIPELINESUPPORT_API UEnum* StaticEnum<EMoviePipelineDLSSQuality>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
