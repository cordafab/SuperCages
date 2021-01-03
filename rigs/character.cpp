#include "character.h"

Character::Character()
{

}

Character::Character(const std::vector<double>   & vertices ,
                     const std::vector<int>      & tris     )
{
    create(vertices, tris);
}

bool Character::create(const std::vector<double>   & vertices ,
                       const std::vector<int>      & tris     )
{
    clear();    //TODO: Try to solve the duplicate "Trimesh::clear()" call

    Trimesh::create(vertices, tris);

    restPoseVertices = vertices;

    init();

    return true;
}

void Character::init()
{
   updateNormals();

   restPoseVerticesNormals = verticesNorm;
   restPoseTrianglesNormals = trisNorm;
}

void Character::clear()
{

    restPoseVertices.clear();
    restPoseVerticesNormals.clear();
    restPoseTrianglesNormals.clear();
    Trimesh::clear();
}

void Character::updateNormals()
{
   updateTrisNormals();
   updateVerticesNormals();
}

void Character::updateTrisNormals()
{
   Trimesh::updateTrisNormals();
}

void Character::updateVerticesNormals()
{
   Trimesh::updateVerticesNormals();
}
