/*
 * 3D Animation Viewer
 * By Ian Esteves do Nascimento, 2013
 */

#ifndef ANIM_ANIMATION_H
#define ANIM_ANIMATION_H "animAnimation.h"

#include <string>
#include <vector>
#include "myUti.h"

/* This class generates animations from a series
 * of keyframes in an .anim file.
 *
 * The anim file is described in animFileSpecification.txt
 * which should come with this file.
 * 
 * The file is read with the import function.
 * 
 * The frames are separated into position, rotation
 * axis, rotation angle and scale. These need to be
 * accessed individually.
 * 
 * The animation frames can be calculated from the
 * key frames in these ways:
 *  - The latest key frame that comes before the
 *    current frame.
 *  - A linear interpolation between the key frames
 *    before and after the current frame.
 *  - A quadratic interpolation between the key frames
 *    before and after the current frame.
 */
class animAnimation{
   private:
      class keyFrame{
         public:
            int frame;
            vector3d pos, rotAxis;
            double rotAngle, scale;
            keyFrame():frame(0),pos(0.0, 0.0, 0.0),rotAxis(0.0, 0.0, 1.0),rotAngle(0),scale(1){}
      };
      std::vector<keyFrame> frameList;
   public:
      int import(std::string fileName); // Read .anim file.
      vector3d getPos1(int frame); // Key frames.
      vector3d getRotAxis1(int frame);
      double getRotAngle1(int frame);
      double getScale1(int frame);
      vector3d getPos2(int frame); // Linear interpolaion.
      vector3d getRotAxis2(int frame);
      double getRotAngle2(int frame);
      double getScale2(int frame);
      vector3d getPos3(int frame); // Quadratic interpolation.
      vector3d getRotAxis3(int frame);
      double getRotAngle3(int frame);
      double getScale3(int frame);
};

#endif
