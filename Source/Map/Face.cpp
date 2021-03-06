#include <Face.h>


#include <TextureManager.h>
#include <DataManager.h>
#include <Renderer.h>
#include <ShapeManager.h>

Face::Face()
{
	SurfaceTypeID = INVALID_INDEX;
	MaterialTypeID = INVALID_INDEX;

    FaceType = FaceShape(CubeShape(false), DIRECTION_UNKNOWN);
}

Face::Face(CubeCoordinates TargetCoordinates, Direction DirectionType)
{
	SurfaceTypeID = INVALID_INDEX;
	MaterialTypeID = INVALID_INDEX;

    FaceType = FaceShape(CubeShape(false), DirectionType);
	LocationCoordinates = TargetCoordinates;
}

void Face::RefreshEntity(Ogre::StaticGeometry* CellGeometry, CellCoordinates CellPosition)
{
    if (SurfaceTypeID != INVALID_INDEX && MaterialTypeID != INVALID_INDEX)
    {
        int16_t X = (CellPosition.X * CELLEDGESIZE) + (LocationCoordinates >> CELLBITSHIFT);
        int16_t Y = (CellPosition.Y * CELLEDGESIZE) + (LocationCoordinates & CELLBITFLAG);
        int16_t Z = CellPosition.Z;

        if (FaceType.FaceDirection.ValueonAxis(AXIS_X) == 1)
        {
            X -= 1;
        }

        if (FaceType.FaceDirection.ValueonAxis(AXIS_Y) == 1)
        {
            Y -= 1;
        }

        if (FaceType.FaceDirection.ValueonAxis(AXIS_Z) == 1)
        {
            Z -= 1;
        }

		Ogre::MeshPtr FaceMesh = SHAPE->getFaceMesh(FaceType);
		if (!FaceMesh.isNull())
        {
			Ogre::Entity* NewEntity = RENDERER->getSceneManager()->createEntity(FaceMesh);
            NewEntity->setMaterial(TEXTURE->getOgreMaterial(MaterialTypeID, SurfaceTypeID));
            CellGeometry->addEntity(NewEntity, Ogre::Vector3(X, Y, Z));
			RENDERER->getSceneManager()->destroyEntity(NewEntity);
        }
    }
}

Face::~Face()
{
    //Set Cell needsRedraw
}

void Face::setFaceMaterialType(int16_t NewMaterialTypeID)
{
	if (NewMaterialTypeID != MaterialTypeID)
	{
		MaterialTypeID = NewMaterialTypeID;

		//Set Cell needsRedraw
	}
}

void Face::setFaceSurfaceType(int16_t NewSurfaceTypeID)
{
    if (NewSurfaceTypeID != SurfaceTypeID)
    {
        SurfaceTypeID = NewSurfaceTypeID;

		//Set Cell needsRedraw
    }
}

void Face::setFaceShapeType(FaceShape NewShape)
{
    if (NewShape != FaceType)
    {
        FaceType = NewShape;

		//Set Cell needsRedraw
    }
}

void Face::Save(boost::filesystem::basic_ofstream<char>& Stream) const
{
	Stream.write((char*)&FaceType, sizeof(FaceType));
	Stream.write((char*)&LocationCoordinates, sizeof(LocationCoordinates));
	Stream.write((char*)&SurfaceTypeID, sizeof(SurfaceTypeID));
	Stream.write((char*)&MaterialTypeID, sizeof(MaterialTypeID));
}

void Face::Load(boost::filesystem::basic_ifstream<char>& Stream)
{
	Stream.read((char*)&FaceType, sizeof(FaceType));
	Stream.read((char*)&LocationCoordinates, sizeof(LocationCoordinates));
	Stream.read((char*)&SurfaceTypeID, sizeof(SurfaceTypeID));
	Stream.read((char*)&MaterialTypeID, sizeof(MaterialTypeID));
}
