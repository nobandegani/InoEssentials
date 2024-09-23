// InoLab Trials © 2024 Inoland Studio. All rights reserved.


#include "InoSvgUtils.h"
#include <string>

THIRD_PARTY_INCLUDES_START
#include <nanosvg.h>
#include <nanosvgrast.h>
THIRD_PARTY_INCLUDES_END

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FLine> UInoSvgUtils::GetLinesFromSVG(const FString& SVGContent, const FString& Units, const float& DPI)
{
	TArray<FLine> Lines;

	std::string SvgString(TCHAR_TO_UTF8(*SVGContent));

	NSVGimage* image = nsvgParse((char*)SvgString.c_str(), TCHAR_TO_UTF8(*Units), DPI);

	if (image)
	{
		for (NSVGshape* shape = image->shapes; shape != nullptr; shape = shape->next)
		{
			for (NSVGpath* path = shape->paths; path != nullptr; path = path->next)
			{
				for (int i = 0; i < path->npts - 1; i += 1)
				{
					float* p = &path->pts[i * 2];

					FVector2D Start(p[0], p[1]);
					FVector2D End(p[2], p[3]); 

					FLine Line(Start, End);
					Lines.Add(Line);
				}
			}
		}
		
		nsvgDelete(image);
	}

	return Lines;
}

TArray<FPath> UInoSvgUtils::GetPathsFromSVG(const FString& SVGContent, const FString& Units, const float& DPI)
{
	TArray<FPath> Paths;

	std::string SvgString(TCHAR_TO_UTF8(*SVGContent));

	NSVGimage* image = nsvgParse((char*)SvgString.c_str(), TCHAR_TO_UTF8(*Units), DPI);

	if (image)
	{
		for (NSVGshape* shape = image->shapes; shape != nullptr; shape = shape->next)
		{
			for (NSVGpath* path = shape->paths; path != nullptr; path = path->next)
			{
				FPath NewPath;

				// Loop through all points in this path and store them in the FPath struct
				for (int i = 0; i < path->npts; ++i)
				{
					float* p = &path->pts[i * 2];
					FVector2D Point(p[0], p[1]); // Each point consists of (x, y)
					NewPath.Points.Add(Point);   // Add this point to the new FPath
				}

				// Check if this path already exists in the Paths array
				bool bIsDuplicate = false;
				for (const FPath& ExistingPath : Paths)
				{
					if (ExistingPath.Points.Num() == NewPath.Points.Num())
					{
						bool bAllPointsMatch = true;
						for (int32 j = 0; j < ExistingPath.Points.Num(); ++j)
						{
							if (!ExistingPath.Points[j].Equals(NewPath.Points[j], 0.001f)) // Check with small tolerance
							{
								bAllPointsMatch = false;
								break;
							}
						}

						if (bAllPointsMatch)
						{
							bIsDuplicate = true;
							break;
						}
					}
				}

				// If the path is not a duplicate, add it to the Paths array
				if (!bIsDuplicate)
				{
					Paths.Add(NewPath);
				}
			}
		}

		nsvgDelete(image); // Cleanup after use
	}

	return Paths;
}

void UInoSvgUtils::FindDoubles(
	const float& Tolerance,
	const FVector2D& InPoint,
	TArray<FVector2D>& Singles,
	 TArray<FVector2D>& Doubles,
	 TArray<FVector2D>& Triples
	 )
{
	if (Singles.Num() == 0)
	{
		Singles.Add(InPoint);
		return;
	}
	
	for (const FVector2D& SinglesPoint : Singles)
	{
		if ( InPoint.Equals(SinglesPoint, Tolerance) )
		{
			if (Doubles.Num() == 0)
			{
				Doubles.Add(InPoint);
				return;
			}
			for (const FVector2D& DoublesPoint : Doubles)
			{
				if ( InPoint.Equals(DoublesPoint, Tolerance) )
				{
					Triples.Add(InPoint);
					return;
				}
			}
			Doubles.Add(InPoint);
			return;
		}
	}
	Singles.Add(InPoint);
}

void UInoSvgUtils::CategorizePoint(
	const float& Tolerance,
	const FVector2D& NewPoint,
	TArray<FVector2D>& Singles,
	TArray<FVector2D>& Doubles,
	TArray<FVector2D>& Triples
)
{
	auto PointMatchesPredicate = [&](const FVector2D& ExistingPoint) {
		return NewPoint.Equals(ExistingPoint, Tolerance);
	};

	if (Triples.ContainsByPredicate(PointMatchesPredicate))
	{
		return;
	}

	if (Doubles.ContainsByPredicate(PointMatchesPredicate))
	{
		Triples.Add(NewPoint);
		return;
	}

	if (Singles.ContainsByPredicate(PointMatchesPredicate))
	{
		Doubles.Add(NewPoint);
		return;
	}

	Singles.Add(NewPoint);
}

void UInoSvgUtils::ExcludeOverlappingPoints(
	const float& Tolerance,
	TArray<FVector2D>& Singles,
	TArray<FVector2D>& Doubles,
	TArray<FVector2D>& Triples
)
{
	Singles.RemoveAll([&](const FVector2D& Point) {
		return Doubles.ContainsByPredicate([&](const FVector2D& DoublePoint) {
			return Point.Equals(DoublePoint, Tolerance);
		});
	});

	Doubles.RemoveAll([&](const FVector2D& Point) {
		return Triples.ContainsByPredicate([&](const FVector2D& TriplePoint) {
			return Point.Equals(TriplePoint, Tolerance);
		});
	});
}

FString UInoSvgUtils::RemoveSVGTag(const FString& Content, const FString& Key)
{
	FString Result = Content;

	// Create opening and closing tags based on the provided key
	FString OpenTag = FString::Printf(TEXT("<%s"), *Key);
	FString CloseTag = FString::Printf(TEXT("</%s>"), *Key);

	int32 OpenTagPos, CloseTagPos;

	// Find and remove all occurrences of the key's element in the content
	while (Result.Contains(OpenTag))
	{
		// Find the opening tag position
		OpenTagPos = Result.Find(OpenTag);
		if (OpenTagPos == INDEX_NONE)
		{
			break;
		}

		// Find the closing tag position
		CloseTagPos = Result.Find(CloseTag, ESearchCase::IgnoreCase, ESearchDir::FromStart, OpenTagPos);
		if (CloseTagPos == INDEX_NONE)
		{
			break;
		}

		// Calculate the length of the entire tag to remove
		int32 EndPos = CloseTagPos + CloseTag.Len();
		Result.RemoveAt(OpenTagPos, EndPos - OpenTagPos);
	}

	return Result;
}

FString UInoSvgUtils::RemoveSVGTagWithKey(const FString& Content, const FString& KeyName, const FString& KeyValue)
{
	FString Result = Content;

	// Create the key-value pair to search for (e.g., stroke="#006BB7")
	FString KeyValuePair = FString::Printf(TEXT("%s=\"%s\""), *KeyName, *KeyValue);

	int32 ElementStartPos, KeyPos, ElementEndPos;

	// Find and remove all occurrences of the elements that contain the key-value pair
	while (Result.Contains(KeyValuePair))
	{
		// Find the position of the key-value pair
		KeyPos = Result.Find(KeyValuePair);
		if (KeyPos == INDEX_NONE)
		{
			break;
		}

		// Find the start of the element (look backwards for the '<')
		ElementStartPos = Result.Find(TEXT("<"), ESearchCase::IgnoreCase, ESearchDir::FromEnd, KeyPos);
		if (ElementStartPos == INDEX_NONE)
		{
			break;
		}

		// Find the end of the element (look for the closing '>')
		ElementEndPos = Result.Find(TEXT(">"), ESearchCase::IgnoreCase, ESearchDir::FromStart, KeyPos);
		if (ElementEndPos == INDEX_NONE)
		{
			break;
		}

		// Remove the entire element from the start of the opening tag to the end of the closing tag
		Result.RemoveAt(ElementStartPos, (ElementEndPos - ElementStartPos + 1));
	}

	return Result;
}

FPath UInoSvgUtils::ScalePath(const FPath& Path, float ScaleFactor)
{
	FPath ScaledPath;

	// Iterate through each point in the input path and scale it
	for (const FVector2D& Point : Path.Points)
	{
		FVector2D ScaledPoint = Point * ScaleFactor;  // Multiply each point by the scale factor
		ScaledPath.Points.Add(ScaledPoint);           // Add the scaled point to the new path
	}

	return ScaledPath;
}

TArray<FPath> UInoSvgUtils::ScalePathArray(const TArray<FPath>& PathArray, float ScaleFactor)
{
	TArray<FPath> ScaledPaths;

	// Iterate through each FPath in the input array
	for (const FPath& Path : PathArray)
	{
		FPath ScaledPath;

		// Scale each point in the current path
		for (const FVector2D& Point : Path.Points)
		{
			FVector2D ScaledPoint = Point * ScaleFactor;  // Multiply each point by the scale factor
			ScaledPath.Points.Add(ScaledPoint);           // Add the scaled point to the new path
		}

		// Add the scaled path to the result array
		ScaledPaths.Add(ScaledPath);
	}

	return ScaledPaths;
}

void UInoSvgUtils::CalculateHeightAndWidth(const TArray<FPath>& PathArray, float& OutWidth, float& OutHeight)
{
	// Initialize min/max values with extreme values
	float MinX = FLT_MAX;
	float MaxX = FLT_MIN;
	float MinY = FLT_MAX;
	float MaxY = FLT_MIN;

	// Iterate through each FPath in the array
	for (const FPath& Path : PathArray)
	{
		// Iterate through each point in the current FPath
		for (const FVector2D& Point : Path.Points)
		{
			// Update min/max for X and Y
			if (Point.X < MinX) MinX = Point.X;
			if (Point.X > MaxX) MaxX = Point.X;

			if (Point.Y < MinY) MinY = Point.Y;
			if (Point.Y > MaxY) MaxY = Point.Y;
		}
	}

	// Calculate the width and height
	OutWidth = MaxX - MinX;
	OutHeight = MaxY - MinY;
}

TArray<FPath> UInoSvgUtils::MovePaths(const TArray<FPath>& PathArray, float DeltaX, float DeltaY)
{
	TArray<FPath> MovedPaths;

	// Iterate through each FPath in the input array
	for (const FPath& Path : PathArray)
	{
		FPath MovedPath;

		// Iterate through each point in the current FPath and apply the delta
		for (const FVector2D& Point : Path.Points)
		{
			// Create a new point with the applied DeltaX and DeltaY
			FVector2D MovedPoint = FVector2D(Point.X + DeltaX, Point.Y + DeltaY);
			MovedPath.Points.Add(MovedPoint); // Add the moved point to the new path
		}

		// Add the moved path to the resulting array
		MovedPaths.Add(MovedPath);
	}

	return MovedPaths;
}

TArray<FMazeSpline> UInoSvgUtils::GenerateMazeSpline(const TArray<FPath>& PathArray, const float& CurveScale)
{
	TArray<FMazeSpline> MazeSplines;
	
	for (const FPath& Path : PathArray)
	{
		FMazeSpline MazeSpline;
		
		MazeSpline.StartLoc = Path.Points[0];
		MazeSpline.EndLoc = Path.Points[3];
		float CurveScale2 = FVector2D::Distance(Path.Points[1],Path.Points[2]) * CurveScale;
		MazeSpline.StartTan = (Path.Points[1] - MazeSpline.StartLoc).GetSafeNormal() * CurveScale2;
		MazeSpline.EndTan = (MazeSpline.EndLoc - Path.Points[2]).GetSafeNormal() * CurveScale2;
		
		MazeSplines.Add(MazeSpline);
	}
	
	return MazeSplines;
}

FVector2D UInoSvgUtils::GetMazeSplineForwardVec(const FMazeSpline& InSpline, bool isEnd)
{
	FVector2D ForwardVec = isEnd ? InSpline.StartLoc - InSpline.EndLoc : InSpline.EndLoc - InSpline.StartLoc;
	ForwardVec.Normalize();

	return ForwardVec;
}

void UInoSvgUtils::FindMazeSimilarPoints(
	const TArray<FMazeSpline> MazeSplines,
	const float& Tolerance,
	TMap<FVector2D, FMazeSimilarPoints>& MazeSimilarPoints
	)
{
	// Early exit if there are fewer than 10 splines
	if (MazeSplines.Num() < 10)
	{
		return;
	}

	// Helper function to compare points with tolerance
	auto ArePointsEqual = [&](const FVector2D& Point1, const FVector2D& Point2, float Tolerance) -> bool
	{
		return FVector2D::Distance(Point1, Point2) <= Tolerance;
	};

	// Helper function to add points to the map with tolerance checking
	auto AddPointToMap = [&](const FVector2D& Location, bool IsEnd, int Index) {
		FMazeSimilarPoint SimilarPoint;
		SimilarPoint.index = Index;
		SimilarPoint.isEnd = IsEnd;

		// Check if the point already exists in the map with tolerance
		bool Found = false;
		for (auto& Elem : MazeSimilarPoints)
		{
			if (ArePointsEqual(Elem.Key, Location, Tolerance))
			{
				// If a similar point is found, add the new similar point to the existing entry
				Elem.Value.array.Add(SimilarPoint);
				Found = true;
				break;
			}
		}

		// If the point was not found in the map, add it as a new entry
		if (!Found)
		{
			FMazeSimilarPoints NewSimilarPoints;
			NewSimilarPoints.array.Add(SimilarPoint);
			MazeSimilarPoints.Add(Location, NewSimilarPoints);
		}
	};

	// Iterate through all splines to check start and end locations
	for (int i = 0; i < MazeSplines.Num(); i++)
	{
		const FMazeSpline& Spline = MazeSplines[i];

		// Check StartLoc
		AddPointToMap(Spline.StartLoc, false, i);

		// Check EndLoc
		AddPointToMap(Spline.EndLoc, true, i);
	}
}

void UInoSvgUtils::RemoveSameForwardFromSimilarPoints(
	const float& Tolerance,
	const TArray<FMazeSpline> MazeSplines,
	const TMap<FVector2D, FMazeSimilarPoints>& InSimilarPoints,
	TMap<FVector2D, FMazeSimilarPoints>& OutSimilarPoints)
{
	for (auto& SimilarPoint : InSimilarPoints)
	{
		if (SimilarPoint.Value.array.Num() != 2)
		{
			OutSimilarPoints.Add(SimilarPoint);
			continue;
		}
		
		auto& point0 = SimilarPoint.Value.array[0];
		auto& point1 = SimilarPoint.Value.array[1];
			
		FVector2D Spline0ForwardVector = GetMazeSplineForwardVec(MazeSplines[point0.index], point0.isEnd).GetAbs();
		FVector2D Spline1ForwardVector = GetMazeSplineForwardVec(MazeSplines[point1.index], point1.isEnd).GetAbs();

		if ( !Spline0ForwardVector.Equals(Spline1ForwardVector, Tolerance) )
		{
			OutSimilarPoints.Add(SimilarPoint);
		}
	}
}

void UInoSvgUtils::FindMazeNeighbourPoints(const TArray<FMazeSpline> MazeSplines,
	const TMap<FVector2D, FMazeSimilarPoints>& InSimilarPoints, TArray<FMazeNeighbourPoint>& OutNeighbourPoints)
{
	for (auto& SimilarPoint : InSimilarPoints)
	{
		FMazeNeighbourPoint NeighbourPoint;
		int i = 0;
		auto& point0 = SimilarPoint.Value.array[i];
		NeighbourPoint.Location = point0.isEnd? MazeSplines[point0.index].EndLoc : MazeSplines[point0.index].StartLoc;
		//NeighbourPoint.ForwardVector
		if (SimilarPoint.Value.array.Num() == 1)
		{
			
		}else if (SimilarPoint.Value.array.Num() == 2)
		{
			
		}else if(SimilarPoint.Value.array.Num() == 3)
		{
			
		}else if(SimilarPoint.Value.array.Num() == 4)
		{
			
		}else if(SimilarPoint.Value.array.Num() == 5)
		{
			
		}
	};
}

