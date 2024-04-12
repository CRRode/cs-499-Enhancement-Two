//Christopher Rode
//Date: 3/31/24
//version 2.1
//  this code extends the functionality of the StaticMesh3D class to handle indexed meshes by introducing an index buffer object (_indicesVBO). It ensures proper resource management by deleting the index buffer object when the mesh is destroyed and follows the object-oriented principle of inheritance by extending the base class

// Project
#include "staticMeshIndexed3D.h"

namespace static_meshes_3D {

    StaticMeshIndexed3D::StaticMeshIndexed3D(bool withPositions, bool withTextureCoordinates, bool withNormals)
        : StaticMesh3D(withPositions, withTextureCoordinates, withNormals) {}

    StaticMeshIndexed3D::~StaticMeshIndexed3D()
    {
        if (_isInitialized) {
            // It's enough to delete indices VBO here, rest of stuff is destructed in super destructor
            _indicesVBO.deleteVBO();
        }
    }

    void StaticMeshIndexed3D::deleteMesh()
    {
        if (_isInitialized) {
            _indicesVBO.deleteVBO();
            StaticMesh3D::deleteMesh();
        }
    }

} // namespace static_meshes_3D
