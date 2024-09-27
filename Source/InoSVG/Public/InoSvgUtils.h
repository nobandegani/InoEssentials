// InoLab Trials © 2024 Inoland Studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InoSvgUtils.generated.h"

UENUM(BlueprintType)  // This makes the enum accessible in Blueprints
enum class ENeighbourType : uint8  // Enum class with underlying type as uint8
{
	Wall,
	MiddleWall,
	CornerColumn0,
	CornerColumn1,
	CornerColumn2,
	CornerColumn3,
	CornerColumn4
};

USTRUCT(BlueprintType)
struct FLine
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector2D Start;

	UPROPERTY(BlueprintReadWrite)
	FVector2D End;

	FLine() : Start(0,0), End(0,0) {}

	FLine(FVector2D InStart, FVector2D InEnd)
		: Start(InStart), End(InEnd)
	{
	}
};

USTRUCT(BlueprintType)
struct FPath
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector2D> Points;

	FPath() {}
};

USTRUCT(BlueprintType)
struct FMazeSpline
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector2D StartLoc;

	UPROPERTY(BlueprintReadWrite)
	FVector2D EndLoc;
	
	UPROPERTY(BlueprintReadWrite)
	FVector2D StartTan;

	UPROPERTY(BlueprintReadWrite)
	FVector2D EndTan;
	
	FMazeSpline() : StartLoc(0,0), EndLoc(0,0), StartTan(0,0), EndTan(0,0) {}

	FMazeSpline(const FVector2D& StartLoc, const FVector2D& EndLoc, const FVector2D& StartTan, const FVector2D& EndTang)
		: StartLoc(StartLoc),
		  EndLoc(EndLoc),
		  StartTan(StartTan),
		  EndTan(EndTang)
	{
	}
};

USTRUCT(BlueprintType)
struct FMazeNeighbourPoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector2D Location;

	UPROPERTY(BlueprintReadWrite)
	int NeighbourCount;

	UPROPERTY(BlueprintReadWrite)
	ENeighbourType NeighbourType;
	
	UPROPERTY(BlueprintReadWrite)
	FVector2D ForwardVector;

	UPROPERTY(BlueprintReadWrite)
	float RotationZ;
	
	FMazeNeighbourPoint(): Location(0, 0), NeighbourCount(0), NeighbourType(ENeighbourType::Wall), ForwardVector(0, 0), RotationZ(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FMazeSimilarPoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int index;

	UPROPERTY(BlueprintReadWrite)
	bool isEnd;
	
	FMazeSimilarPoint(): index(0), isEnd(false)
	{
	}
};

USTRUCT(BlueprintType)
struct FMazeSimilarPoints
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FMazeSimilarPoint> array;
	
	FMazeSimilarPoints()
	{
	}
};

/**
 * 
 */
UCLASS()
class INOSVG_API UInoSvgUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
	static TArray<FLine> GetLinesFromSVG(const FString& SVGContent, const FString& Units, const float& DPI);

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
	static TArray<FPath> GetPathsFromSVG(const FString& SVGContent, const FString& Units, const float& DPI);
	
	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static void FindDoubles(
			const float& Tolerance,
			const FVector2D& InPoint,
			UPARAM(ref) TArray<FVector2D>& Singles,
			UPARAM(ref) TArray<FVector2D>& Doubles,
			UPARAM(ref) TArray<FVector2D>& Triples
		);

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static void CategorizePoint(
			const float& Tolerance,
			const FVector2D& NewPoint,
			UPARAM(ref) TArray<FVector2D>& Singles,
			UPARAM(ref) TArray<FVector2D>& Doubles,
			UPARAM(ref) TArray<FVector2D>& Triples
			);

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static void ExcludeOverlappingPoints(
			const float& Tolerance,
			UPARAM(ref)TArray<FVector2D>& Singles,
			UPARAM(ref)TArray<FVector2D>& Doubles,
			UPARAM(ref)TArray<FVector2D>& Triples
		);
		
	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static FString RemoveSVGTag(const FString& Content, const FString& Key);

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static FString RemoveSVGTagWithKey(const FString& Content, const FString& KeyName, const FString& KeyValue);

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static FPath ScalePath(const FPath& Path, float ScaleFactor);

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static TArray<FPath> ScalePathArray(const TArray<FPath>& PathArray, float ScaleFactor);

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static void CalculateHeightAndWidth(const TArray<FPath>& PathArray, float& OutWidth, float& OutHeight);

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static TArray<FPath> MovePaths(const TArray<FPath>& PathArray, float DeltaX, float DeltaY);

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static TArray<FMazeSpline> GenerateMazeSpline(const TArray<FPath>& PathArray, const float& CurveScale);

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static FVector2D GetMazeSplineForwardVec(const FMazeSpline& InSpline, bool isEnd);

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static void FindMazeSimilarPoints(
		const TArray<FMazeSpline> MazeSplines,
		const float& Tolerance,
		TMap<FVector2D,FMazeSimilarPoints>& MazeSimilarPoints);

	UFUNCTION(BlueprintCallable, Category = "InoSVG")
	static void RemoveSameForwardFromSimilarPoints(
		const float& Tolerance,
		const TArray<FMazeSpline> MazeSplines,
		const TMap<FVector2D,FMazeSimilarPoints>& InSimilarPoints,
		TMap<FVector2D,FMazeSimilarPoints>& OutSimilarPoints);
	
	UFUNCTION(BlueprintCallable, Category = "InoSVG")
		static void FindMazeNeighbourPoints(
			const TArray<FMazeSpline> MazeSplines,
			const TMap<FVector2D,FMazeSimilarPoints>& InSimilarPoints,
			TArray<FMazeNeighbourPoint>& OutNeighbourPoints);
};
