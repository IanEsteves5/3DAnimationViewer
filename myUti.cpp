/*
 *    myUti.cpp
 *    01/2013
 *    by Ian Esteves do Nascimento
 *    ianesteves@poli.ufrj.br
 *    Random functions and classes for future reference.
 *
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include "myUti.h"

using namespace std;

/*****************************************************************
                       Matrix Functions
*****************************************************************/

double **matrixNew(int numLines, int numCols){
   double **result = new double *[numLines];
   for(int i = 0 ; i < numLines ; i++)
      result[i] = new double[numCols];
   return result;
}

double **matrixNew(int numLines, int numCols, double **matrix){
   double **result = new double *[numLines];
   for(int i = 0 ; i < numLines ; i++){
      result[i] = new double[numCols];
      for(int j = 0 ; j < numCols ; j++)
         result[i][j] = matrix[i][j];
   }
   return result;
}

void matrixDelete(int numLines, double **matrix){
   for(int i = 0 ; i < numLines ; i++)
      delete[] matrix[i];
   delete[] matrix;
}

void matrixAssign(int numLines, int numCols, double **matrix, ...){
   va_list values;
   va_start(values, matrix);
   for(int i = 0 ; i < numLines ; i++)
      for(int j = 0 ; j < numCols ; j++)
         matrix[i][j] = va_arg(values, double);
   va_end(values);
}

double matrixDet(int numLines, double **matrix){
   double **matrixAux = matrixNew(numLines, numLines, matrix);
   double result = 1;
   for(int i = 0 ; i < numLines ; i++){
      if(matrixAux[i][i] == 0)
         for(int j = i+1 ; j < numLines ; j++)
            if(matrixAux[j][i] != 0){
               double *aux = matrixAux[i];
               matrixAux[i] = matrixAux[j];
               matrixAux[j] = aux;
               result *= -1;
               break;
            }
      if(matrixAux[i][i] == 0)
         return 0;
      result *= matrixAux[i][i];
      for(int i2 = i+1 ; i2 < numLines ; i2++){
         double aux = matrixAux[i2][i];
         if(aux == 0)
            continue;
         for(int j2 = i ; j2 < numLines ; j2++)
            matrixAux[i2][j2] -= matrixAux[i][j2]*aux/matrixAux[i][i];
      }
   }
   matrixDelete(numLines, matrixAux);
   return result;
}

bool matrixInverse(int numLines, double **matrix){
   if(matrixDet(numLines, matrix)==0.0)
      return false;
   double **matrixAux = matrixNew(numLines, numLines, matrix);
   for(int i = 0 ; i < numLines ; i++)
      for(int j = 0 ; j < numLines ; j++){
         if(i == j){
            matrix[i][j] = 1.0;
            continue;
         }
         matrix[i][j] = 0.0;
      }
   for(int i = 0 ; i < numLines ; i++){
      if(matrixAux[i][i] == 0)
         for(int j = i+1 ; j < numLines ; j++)
            if(matrixAux[j][i] != 0){
               double *aux = matrixAux[i];
               matrixAux[i] = matrixAux[j];
               matrixAux[j] = aux;
               aux = matrix[i];
               matrix[i] = matrix[j];
               matrix[j] = aux;
               break;
            }
      if(matrixAux[i][i] == 0){
         matrixDelete(numLines, matrixAux);
         return false;
      }
      double aux = matrixAux[i][i];
      for(int j = 0 ; j < numLines ; j++){
         matrixAux[i][j] /= aux;
         matrix[i][j] /= aux;
      }
      for(int i2 = i+1 ; i2 < numLines ; i2++){
         double aux2 = matrixAux[i2][i];
         if(aux2 == 0)
            continue;
         for(int j2 = 0 ; j2 < numLines ; j2++){
            matrixAux[i2][j2] -= matrixAux[i][j2]*aux2;
            matrix[i2][j2] -= matrix[i][j2]*aux2;
         }
      }
   }
   for(int i = numLines - 1 ; i > 0 ; i--){
      for(int i2 = 0 ; i2 < i ; i2++)
         for(int j2 = 0 ; j2 < numLines ; j2++)
            matrix[i2][j2] -= matrix[i][j2]*matrixAux[i2][i];
   }
   matrixDelete(numLines, matrixAux);
   return true;
}

double *matrixSolveSystem(int numLines, double **matrix){
   if(matrixDet(numLines, matrix) == 0)
      return 0;
   double *result;
   double **matrixAux = matrixNew(numLines, numLines + 1, matrix);
   for(int i = 0 ; i < numLines ; i++){
      if(matrixAux[i][i] == 0)
         for(int j = i+1 ; j < numLines ; j++)
            if(matrixAux[j][i] != 0){
               double *aux = matrixAux[i];
               matrixAux[i] = matrixAux[j];
               matrixAux[j] = aux;
               break;
            }
      if(matrixAux[i][i] == 0)
         return 0;
      for(int i2 = i+1 ; i2 < numLines ; i2++){
         double aux = matrixAux[i2][i];
         if(aux == 0)
            continue;
         for(int j2 = i ; j2 <= numLines ; j2++)
            matrixAux[i2][j2] -= matrixAux[i][j2]*aux/matrixAux[i][i];
      }
   }
   
   for(int i = numLines - 1 ; i > 0 ; i--){
      for(int j = 0 ; j < i ; j++){
         matrixAux[j][numLines] -= matrixAux[i][numLines]*matrixAux[j][i]/matrixAux[i][i];
      }
   }
   result = new double[numLines];
   for(int i = 0 ; i < numLines ; i++){
      result[i] = matrixAux[i][numLines]/matrixAux[i][i];
   }
   return result;
}

/*****************************************************************
                            Vectors
*****************************************************************/

double vector2d::getX() const{
   return x;
}

void vector2d::setX(double newX){
   x = newX;
}

double vector2d::getY() const{
   return y;
}

void vector2d::setY(double newY){
   y = newY;
}

double vector2d::getR() const{
   return sqrt(x*x+y*y);
}

void vector2d::setR(double newR){
   double oldR = getR();
   if(oldR == 0)
      return;
   x *= newR/oldR;
   y *= newR/oldR;
}

double vector2d::getA() const{
   if(x==0){
      if(y>0)
         return M_PI/2;
      if(y<0)
         return -M_PI/2;
      return 0;
   }
   if (x > 0)
      return atan(y/x);
   if (y > 0)
      return atan(y/x) + M_PI;
   return atan(y/x) - M_PI;
}

void vector2d::setA(double newA){
   x = getR();
   y = 0;
   rotate(newA);
}

double vector2d::dot(const vector2d &v) const{
   return x*v.x+y*v.y;
}

double vector2d::cross(const vector2d &v) const{
   return x*v.y-y*v.x;
}

void vector2d::rotate(double angle){
   double oldX = x;
   x = oldX*cos(angle)-y*sin(angle);
   y = oldX*sin(angle)+y*cos(angle);
}

void vector2d::transform(double **matrix){
   double oldX = x;
   x = oldX*matrix[0][0]+y*matrix[0][1];
   y = oldX*matrix[1][0]+y*matrix[1][1];
}

const vector2d &vector2d::operator =(const vector2d &v){
   x = v.x;
   y = v.y;
   return v;
}

const vector2d vector2d::operator +(const vector2d &v) const{
   return vector2d(x+v.x, y+v.y);
}

const vector2d &vector2d::operator +=(const vector2d &v){
   x += v.x;
   y += v.y;
   return *this;
}

const vector2d vector2d::operator -(const vector2d &v) const{
   return vector2d(x-v.x, y-v.y);
}

const vector2d &vector2d::operator -=(const vector2d &v){
   x -= v.x;
   y -= v.y;
   return *this;
}

const vector2d vector2d::operator *(double d) const{
   return vector2d(x*d, y*d);
}

const vector2d operator *(double d, const vector2d &v){
   return vector2d(v.x*d, v.y*d);
}

const vector2d &vector2d::operator *=(double d){
   x *= d;
   y *= d;
   return *this;
}

const vector2d vector2d::operator /(double d) const{
   return vector2d(x/d, y/d);
}

const vector2d &vector2d::operator /=(double d){
   x /= d;
   y /= d;
   return *this;
}

const vector2d vector2d::operator -() const{
   return vector2d(-x, -y);
}

bool vector2d::operator ==(const vector2d &v) const{
   return x==v.x && y==v.y;
}

bool vector2d::operator !=(const vector2d &v) const{
   return x!=v.x || y!=v.y;
}

bool vector2d::operator >(const vector2d &v) const{
   return getR() > v.getR();
}

bool vector2d::operator >=(const vector2d &v) const{
   return getR() >= v.getR();
}

bool vector2d::operator <(const vector2d &v) const{
   return getR() < v.getR();
}

bool vector2d::operator <=(const vector2d &v) const{
   return getR() <= v.getR();
}

double vector2d::operator *(const vector2d &v) const{
   return dot(v);
}

vector2d::operator string() const{
   ostringstream st;
   st << "(" << x << "," << y << ")";
   return st.str();
}

const vector2d x2d(1, 0);
const vector2d y2d(0, 1);

double vector3d::getX() const{
   return x;
}

void vector3d::setX(double newX){
   x = newX;
}

double vector3d::getY() const{
   return y;
}

void vector3d::setY(double newY){
   y = newY;
}

double vector3d::getZ() const{
   return z;
}

void vector3d::setZ(double newZ){
   z = newZ;
}

double vector3d::getR() const{
   return sqrt(x*x+y*y+z*z);
}

void vector3d::setR(double newR){
   double oldR = getR();
   if(oldR == 0)
      return;
   x *= newR/oldR;
   y *= newR/oldR;
   z *= newR/oldR;
}

double vector3d::getA1() const{
   if(getR()==0){
      return 0;
   }
   return acos(z/getR());
}

void vector3d::setA1(double newA1){
   vector3d dir1(0, 1, 0);
   vector3d dir2(0, 0, 1);
   double oldA2 = getA2();
   z = getR();
   x = 0;
   y = 0;
   rotate(newA1, dir1);
   rotate(oldA2, dir2);
}

double vector3d::getA2() const{
   if(x==0){
      if(y>0)
         return M_PI/2;
      if(y<0)
         return -M_PI/2;
      return 0;
   }
   if (x > 0)
      return atan(y/x);
   if (y > 0)
      return atan(y/x) + M_PI;
   return atan(y/x) - M_PI;
}

void vector3d::setA2(double newA2){
   vector3d dir1(0, 1, 0);
   vector3d dir2(0, 0, 1);
   double oldA1 = getA1();
   z = getR();
   x = 0;
   y = 0;
   rotate(oldA1, dir1);
   rotate(newA2, dir2);
}

double vector3d::dot(const vector3d &v) const{
   return x*v.x+y*v.y+z*v.z;
}

const vector3d vector3d::cross(const vector3d &v) const{
   return vector3d(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
}

void vector3d::rotate(double angle, double dirX, double dirY, double dirZ){
   vector3d v(dirX, dirY, dirZ);
   rotate(angle, v);
}

void vector3d::rotate(double angle, const vector3d &v){
   vector3d oldV(*this), axis(v);
   axis.setR(1);
   double a = cos(angle/2);
   double b = axis.x*sin(angle/2);
   double c = axis.y*sin(angle/2);
   double d = axis.z*sin(angle/2);
   x = oldV.x*(a*a+b*b-c*c-d*d)+
       oldV.y*2*(b*c-a*d)+
       oldV.z*2*(b*d+a*c);
   y = oldV.x*2*(b*c+a*d)+
       oldV.y*(a*a+c*c-b*b-d*d)+
       oldV.z*2*(c*d-a*b);
   z = oldV.x*2*(b*d-a*c)+
       oldV.y*2*(c*d+a*b)+
       oldV.z*(a*a+d*d-b*b-c*c);
}

void vector3d::transform(double **matrix){
   vector3d oldV(*this);
   x = oldV.x*matrix[0][0]+oldV.y*matrix[0][1]+oldV.z*matrix[0][2];
   y = oldV.x*matrix[1][0]+oldV.y*matrix[1][1]+oldV.z*matrix[1][2];
   z = oldV.x*matrix[2][0]+oldV.y*matrix[2][1]+oldV.z*matrix[2][2];
}

const vector3d &vector3d::operator =(const vector3d &v){
   x = v.x;
   y = v.y;
   z = v.z;
   return v;
}

const vector3d vector3d::operator +(const vector3d &v) const{
   return vector3d(x+v.x, y+v.y, z+v.z);
}

const vector3d &vector3d::operator +=(const vector3d &v){
   x += v.x;
   y += v.y;
   z += v.z;
   return *this;
}

const vector3d vector3d::operator -(const vector3d &v) const{
   return vector3d(x-v.x, y-v.y, z-v.z);
}

const vector3d &vector3d::operator -=(const vector3d &v){
   x -= v.x;
   y -= v.y;
   z -= v.z;
   return *this;
}

const vector3d vector3d::operator *(double d) const{
   return vector3d(x*d, y*d, z*d);
}

const vector3d operator *(double d, const vector3d &v){
   return vector3d(v.x*d, v.y*d, v.z*d);
}

const vector3d &vector3d::operator *=(double d){
   x *= d;
   y *= d;
   z *= d;
   return *this;
}

const vector3d vector3d::operator /(double d) const{
   return vector3d(x/d, y/d, z/d);
}

const vector3d &vector3d::operator /=(double d){
   x /= d;
   y /= d;
   z /= d;
   return *this;
}

const vector3d vector3d::operator -() const{
   return vector3d(-x, -y, -z);
}

bool vector3d::operator ==(const vector3d &v) const{
   return x==v.x && y==v.y && z==v.z;
}

bool vector3d::operator !=(const vector3d &v) const{
   return x!=v.x || y!=v.y || z!=v.z;
}

bool vector3d::operator >(const vector3d &v) const{
   return getR() > v.getR();
}

bool vector3d::operator >=(const vector3d &v) const{
   return getR() >= v.getR();
}

bool vector3d::operator <(const vector3d &v) const{
   return getR() < v.getR();
}

bool vector3d::operator <=(const vector3d &v) const{
   return getR() <= v.getR();
}

double vector3d::operator *(const vector3d &v) const{
   return dot(v);
}

vector3d::operator string() const{
   ostringstream st;
   st << "(" << x << "," << y << "," << z << ")";
   return st.str();
}

const vector3d x3d(1, 0, 0);
const vector3d y3d(0, 1, 0);
const vector3d z3d(0, 0, 1);
