#include "MuCamera.h"

MuCamera::MuCamera(const MuPoint4I& InPosition, const MuPoint4I& InLookAtDirection, float InFOV, float InAspectRatio, float InNearPlane, float InFarPlane, EProjectionMode InProjectionMode)
{
    CameraPosition = InPosition;
    LookAtDirection = InLookAtDirection;
    FOVy = InFOV;
    AspectRatio = InAspectRatio;
    NearPlane = InNearPlane;
    FarPlane = InFarPlane;
    ProjectionMode = InProjectionMode;
}