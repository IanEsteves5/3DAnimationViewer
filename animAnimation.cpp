/*
 * 3D Animation Viewer
 * By Ian Esteves do Nascimento, 2013
 */

#include <string>
#include <vector>
#include <fstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "myUti.h"
#include "animAnimation.h"

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;

// Reads .anim file.
int animAnimation::import(string fileName){
   #ifdef DEBUG
   cout << "DEBUG starting for file " << fileName << endl;
   #endif
   ifstream animFile(fileName.c_str());
   if(!animFile.is_open()) // Returns if file can't be read.
      return 1;
   #ifdef DEBUG
   cout << "Opened " << fileName << " successfully." << endl;
   #endif
   keyFrame newFrame; // Data for the next frame in the list.
   while(!animFile.eof()){
      string line, lineType;
      int linePos;
      getline(animFile, line); // Reads next line of file.
      linePos = line.find(' ');
      if(linePos == string::npos) // Ignore line if it's empty.
         continue;
      lineType = line.substr(0, linePos); // First word of the line is the type.
      line = line.substr(linePos+1);
      if(lineType == "frame"){
         int data = fromStr<int>(line);
         if(data <= newFrame.frame){ // If the new frame is lower than the current one, ignore it.
            #ifdef DEBUG
            cout << "Frame " << data << " ignored for conflicting with frame " << newFrame.frame << endl;
            #endif
            continue;
         }
         #ifdef DEBUG
         cout << "Frame " << newFrame.frame << " included with position " << (string)newFrame.pos << endl;
         #endif
         frameList.push_back(newFrame); // Adds current frame to the list of key frames.
         newFrame.frame = data; // Start reading data for the next frame.
         #ifdef DEBUG
         cout << "New frame starting on " << newFrame.frame << " with position " << (string)newFrame.pos << endl;
         #endif
         continue;
      }
      if(lineType == "pos"){
         vector3d newPos;
         linePos = line.find(' '); // Read 3 values x, y and z
         newPos.setX(fromStr<double>(line.substr(0, linePos)));
         line = line.substr(linePos+1);
         linePos = line.find(' ');
         newPos.setY(fromStr<double>(line.substr(0, linePos)));
         line = line.substr(linePos+1);
         newPos.setZ(fromStr<double>(line));
         newFrame.pos = newPos; // Update current frame.
         #ifdef DEBUG
         cout << "Position on frame " << newFrame.frame << " set to " << (string)newFrame.pos << endl;
         #endif
         continue;
      }
      if(lineType == "rotAxis"){
         vector3d newAxis;
         linePos = line.find(' '); // Read 3 values x, y and z
         newAxis.setX(fromStr<double>(line.substr(0, linePos)));
         line = line.substr(linePos+1);
         linePos = line.find(' ');
         newAxis.setY(fromStr<double>(line.substr(0, linePos)));
         line = line.substr(linePos+1);
         newAxis.setZ(fromStr<double>(line));
         newAxis.setR(1);
         newFrame.rotAxis = newAxis; // Update current frame.
         #ifdef DEBUG
         cout << "Rotation axis on frame " << newFrame.frame << " set to " << (string)newFrame.rotAxis << endl;
         #endif
         continue;
      }
      if(lineType == "rotAngle"){
         double newAngle;
         newAngle = fromStr<double>(line);
         newFrame.rotAngle = newAngle; // Update current frame.
         #ifdef DEBUG
         cout << "Rotation angle on frame " << newFrame.frame << " set to " << newFrame.rotAngle << endl;
         #endif
         continue;
      }
      if(lineType == "scale"){
         double newScale;
         newScale = fromStr<double>(line);
         newFrame.scale = newScale; // Update current frame.
         #ifdef DEBUG
         cout << "Scale angle on frame " << newFrame.frame << " set to " << newFrame.scale << endl;
         #endif
         continue;
      }
   }
   animFile.close();
   #ifdef DEBUG
   cout << "Frame " << newFrame.frame << " included with position " << (string)newFrame.pos << endl;
   #endif
   frameList.push_back(newFrame); // Adds current frame to the list of key frames for the last time.
   #ifdef DEBUG
   cout << "Listing all frames" << endl;
   for(int i = 0 ; i < frameList.size() ; i++){
      cout << "Frame = " << frameList[i].frame << " pos = " << (string)frameList[i].pos << endl;
   }
   cout << "DEBUG ending for file " << fileName << endl;
   #endif
   return 0;
}

/* These next functions return the requested attribute
 * of the last key frame before the animation frame.
 */
vector3d animAnimation::getPos1(int frame){
   vector<keyFrame>::iterator it;
   for(it = frameList.begin() ; it!=frameList.end() && (*it).frame<frame ; it++); // Search.
   if(it != frameList.begin()) // Go back to the right frame.
      it--;
   return (*it).pos;
}

vector3d animAnimation::getRotAxis1(int frame){
   vector<keyFrame>::iterator it;
   for(it = frameList.begin() ; it!=frameList.end() && (*it).frame<frame ; it++);
   if(it != frameList.begin())
      it--;
   return (*it).rotAxis;
}

double animAnimation::getRotAngle1(int frame){
   vector<keyFrame>::iterator it;
   for(it = frameList.begin() ; it!=frameList.end() && (*it).frame<frame ; it++);
   if(it != frameList.begin())
      it--;
   return (*it).rotAngle;
}

double animAnimation::getScale1(int frame){
   vector<keyFrame>::iterator it;
   for(it = frameList.begin() ; it!=frameList.end() && (*it).frame<frame ; it++);
   if(it != frameList.begin())
      it--;
   return (*it).scale;
}

/* These next functions use a linear interpolation to
 * calculate the requested attribute, using a proportion
 * of the last key frame and the next key frame weighted
 * by the difference of the frames.
 */
vector3d animAnimation::getPos2(int frame){
   vector<keyFrame>::iterator it, it2;
   double difTotal, difPartial;
   vector3d vRef;
   it = frameList.begin();
   it2 = frameList.begin();
   it2++;
   if(frame <= (*it).frame) // Frame comes before the animation.
      return (*it).pos;
   while(it2!=frameList.end() && (*it2).frame<frame){ // Search
      it++;
      it2++;
   }
   if(it2 == frameList.end()) // Frame comes after the animation.
      return (*it).pos;
   difTotal = (*it2).frame-(*it).frame; // Distance between the key frames.
   difPartial = frame-(*it).frame; // Distance between the frame and the last key frame.
   vRef = ((*it2).pos-(*it).pos)*(difPartial/difTotal); // Interpolation.
   return (*it).pos+vRef;
}

vector3d animAnimation::getRotAxis2(int frame){
   vector<keyFrame>::iterator it, it2;
   double difTotal, difPartial;
   vector3d vRef;
   it = frameList.begin();
   it2 = frameList.begin();
   it2++;
   if(frame <= (*it).frame)
      return (*it).rotAxis;
   while(it2!=frameList.end() && (*it2).frame<frame){
      it++;
      it2++;
   }
   if(it2 == frameList.end())
      return (*it).rotAxis;
   difTotal = (*it2).frame-(*it).frame;
   difPartial = frame-(*it).frame;
   vRef = ((*it2).rotAxis-(*it).rotAxis)*(difPartial/difTotal);
   return (*it).rotAxis+vRef;
}

double animAnimation::getRotAngle2(int frame){
   vector<keyFrame>::iterator it, it2;
   double difTotal, difPartial, vRef;
   it = frameList.begin();
   it2 = frameList.begin();
   it2++;
   if(frame <= (*it).frame)
      return (*it).rotAngle;
   while(it2!=frameList.end() && (*it2).frame<frame){
      it++;
      it2++;
   }
   if(it2 == frameList.end())
      return (*it).rotAngle;
   difTotal = (*it2).frame-(*it).frame;
   difPartial = frame-(*it).frame;
   vRef = ((*it2).rotAngle-(*it).rotAngle)*(difPartial/difTotal);
   return (*it).rotAngle+vRef;
}

double animAnimation::getScale2(int frame){
   vector<keyFrame>::iterator it, it2;
   double difTotal, difPartial, vRef;
   it = frameList.begin();
   it2 = frameList.begin();
   it2++;
   if(frame <= (*it).frame)
      return (*it).scale;
   while(it2!=frameList.end() && (*it2).frame<frame){
      it++;
      it2++;
   }
   if(it2 == frameList.end())
      return (*it).scale;
   difTotal = (*it2).frame-(*it).frame;
   difPartial = frame-(*it).frame;
   vRef = ((*it2).scale-(*it).scale)*(difPartial/difTotal);
   return (*it).scale+vRef;
}

/* These next functions use a quadratic interpolation to
 * calculate the requested attribute. It separates the
 * animation in groups of three key frames and uses the
 * second as a reference for a quadratic interpolation
 * between the two others, using the De Casteljou algorithm.
 */
vector3d animAnimation::getPos3(int frame){
   vector<keyFrame>::iterator it, it2, it3;
   double difTotal, difPartial;
   vector3d vRef1, vRef2, vRef3;
   it = frameList.begin();
   it2 = frameList.begin();
   it3 = frameList.begin();
   it2++;
   it3++;
   it3++;
   if(frame <= (*it).frame) // Frame comes before the animation.
      return (*it).pos;
   while(it2!=frameList.end() && it3!=frameList.end() && (*it3).frame<frame){ // Search.
      it = it3;
      it2 = it3;
      it2++;
      it3++;
      it3++;
   }
   if(it2 == frameList.end()) // Frame comes after the animation.
      return (*it).pos;
   if(it3 == frameList.end()){ // Animation doesn't have a number of key frames that
      if(frame >= (*it2).frame)// is multiple of three. Linear interpolation between
         return (*it2).pos;    // the last two frames.
      difTotal = (*it2).frame-(*it).frame;
      difPartial = frame-(*it).frame;
      vRef1 = ((*it2).pos-(*it).pos)*(difPartial/difTotal);
      return (*it).pos+vRef1;
   }
   difTotal = (*it3).frame-(*it).frame;
   difPartial = frame-(*it).frame;
   vRef1 = ((*it2).pos-(*it).pos)*(difPartial/difTotal); // Linear interpolation between first and second key frames.
   vRef2 = ((*it3).pos-(*it2).pos)*(difPartial/difTotal); // Linear interpolation between second and third key frames.
   vRef3 = (((*it2).pos+vRef2)-((*it).pos+vRef1))*(difPartial/difTotal); // Linear interpolation between the other two interpolations.
   return (*it).pos+vRef1+vRef3;
}

vector3d animAnimation::getRotAxis3(int frame){
   vector<keyFrame>::iterator it, it2, it3;
   double difTotal, difPartial;
   vector3d vRef1, vRef2, vRef3;
   it = frameList.begin();
   it2 = frameList.begin();
   it3 = frameList.begin();
   it2++;
   it3++;
   it3++;
   if(frame <= (*it).frame)
      return (*it).rotAxis;
   while(it2!=frameList.end() && it3!=frameList.end() && (*it3).frame<frame){
      it = it3;
      it2 = it3;
      it2++;
      it3++;
      it3++;
   }
   if(it2 == frameList.end())
      return (*it).rotAxis;
   if(it3 == frameList.end()){
      if(frame >= (*it2).frame)
         return (*it2).rotAxis;
      difTotal = (*it2).frame-(*it).frame;
      difPartial = frame-(*it).frame;
      vRef1 = ((*it2).rotAxis-(*it).rotAxis)*(difPartial/difTotal);
      return (*it).rotAxis+vRef1;
   }
   difTotal = (*it3).frame-(*it).frame;
   difPartial = frame-(*it).frame;
   vRef1 = ((*it2).rotAxis-(*it).rotAxis)*(difPartial/difTotal);
   vRef2 = ((*it3).rotAxis-(*it2).rotAxis)*(difPartial/difTotal);
   vRef3 = (((*it2).rotAxis+vRef2)-((*it).rotAxis+vRef1))*(difPartial/difTotal);
   return (*it).rotAxis+vRef1+vRef3;
}

double animAnimation::getRotAngle3(int frame){
   vector<keyFrame>::iterator it, it2, it3;
   double difTotal, difPartial, vRef1, vRef2, vRef3;
   it = frameList.begin();
   it2 = frameList.begin();
   it3 = frameList.begin();
   it2++;
   it3++;
   it3++;
   if(frame <= (*it).frame)
      return (*it).rotAngle;
   while(it2!=frameList.end() && it3!=frameList.end() && (*it3).frame<frame){
      it = it3;
      it2 = it3;
      it2++;
      it3++;
      it3++;
   }
   if(it2 == frameList.end())
      return (*it).rotAngle;
   if(it3 == frameList.end()){
      if(frame >= (*it2).frame)
         return (*it2).rotAngle;
      difTotal = (*it2).frame-(*it).frame;
      difPartial = frame-(*it).frame;
      vRef1 = ((*it2).rotAngle-(*it).rotAngle)*(difPartial/difTotal);
      return (*it).rotAngle+vRef1;
   }
   difTotal = (*it3).frame-(*it).frame;
   difPartial = frame-(*it).frame;
   vRef1 = ((*it2).rotAngle-(*it).rotAngle)*(difPartial/difTotal);
   vRef2 = ((*it3).rotAngle-(*it2).rotAngle)*(difPartial/difTotal);
   vRef3 = (((*it2).rotAngle+vRef2)-((*it).rotAngle+vRef1))*(difPartial/difTotal);
   return (*it).rotAngle+vRef1+vRef3;
}

double animAnimation::getScale3(int frame){
   vector<keyFrame>::iterator it, it2, it3;
   double difTotal, difPartial, vRef1, vRef2, vRef3;
   it = frameList.begin();
   it2 = frameList.begin();
   it3 = frameList.begin();
   it2++;
   it3++;
   it3++;
   if(frame <= (*it).frame)
      return (*it).scale;
   while(it2!=frameList.end() && it3!=frameList.end() && (*it3).frame<frame){
      it = it3;
      it2 = it3;
      it2++;
      it3++;
      it3++;
   }
   if(it2 == frameList.end())
      return (*it).scale;
   if(it3 == frameList.end()){
      if(frame >= (*it2).frame)
         return (*it2).scale;
      difTotal = (*it2).frame-(*it).frame;
      difPartial = frame-(*it).frame;
      vRef1 = ((*it2).scale-(*it).scale)*(difPartial/difTotal);
      return (*it).scale+vRef1;
   }
   difTotal = (*it3).frame-(*it).frame;
   difPartial = frame-(*it).frame;
   vRef1 = ((*it2).scale-(*it).scale)*(difPartial/difTotal);
   vRef2 = ((*it3).scale-(*it2).scale)*(difPartial/difTotal);
   vRef3 = (((*it2).scale+vRef2)-((*it).scale+vRef1))*(difPartial/difTotal);
   return (*it).scale+vRef1+vRef3;
}
