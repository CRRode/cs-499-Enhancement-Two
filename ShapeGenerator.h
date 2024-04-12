//Christopher Rode
//Date: 3/31/24
//version 2.1
//this code is to make our shape data for our plane and sphere verts and indices

#pragma once
#include "ShapeData.h"
typedef unsigned int uint;

class ShapeGenerator
{
	static ShapeData makePlaneVerts(uint dimensions);
	static ShapeData makePlaneIndices(uint dimensions);


public:

	static ShapeData makePlane(uint dimensions = 10);
	static ShapeData makeSphere(uint tesselation = 20);

};
