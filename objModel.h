/*
 * 3D Animation Viewer
 * By Ian Esteves do Nascimento, 2013
 */

#ifndef OBJ_MODEL_H
#define OBJ_MODEL_H "objModel.h"

#include <string>
#include <vector>
#include "myUti.h"

/* This class generate a 3D model from an .obj file.
 *
 * An .obj file can be exported from Blender.
 *
 * The file is read with the import function.
 *
 * First vector contains the faces. Second vector
 * contains the points. vector3d contains the
 * x y z coordinates using getX getY getZ.
 * Usage: obj[0][1].getX()
 */
class objModel : public std::vector<std::vector<vector3d> >{
   public:
      int import(std::string fileName); // Reads an .obj file.
};

#endif
