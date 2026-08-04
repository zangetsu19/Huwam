#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HuwamPrototypeSliceBlockoutActor.generated.h"

class UPointLightComponent;
class USceneComponent;
class UStaticMesh;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class HUWAM_API AHuwamPrototypeSliceBlockoutActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamPrototypeSliceBlockoutActor();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Slice")
    bool SpawnPrototypeSliceActors();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> LabFloor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> LabNorthWall;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> LabSouthWall;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> LabEastWall;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> LabWestWall;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> EldoriaGround;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> EldoriaRoad;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> EldoriaGuildPad;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> EldoriaSupplyPad;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> EldoriaGuildRoadSpur;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> EldoriaSupplyRoadSpur;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> EldoriaNorthFence;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UStaticMeshComponent> EldoriaSouthFence;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    TObjectPtr<UPointLightComponent> LabLight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    bool bSpawnLessonActorsOnBeginPlay = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Slice")
    FVector EldoriaOffset = FVector(6000.0f, 0.0f, 0.0f);

private:
    void ConfigureBlockMesh(UStaticMeshComponent* Mesh, UStaticMesh* MeshAsset, const FVector& Location, const FVector& Scale, bool bCollision) const;
    FTransform MakeOffsetTransform(const FVector& Offset) const;
};
