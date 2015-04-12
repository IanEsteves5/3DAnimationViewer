/*
 *    myUti.h
 *    01/2013
 *    by Ian Esteves do Nascimento
 *    ianesteves@poli.ufrj.br
 *    Random functions and classes for future reference.
 *
 */
 
/*
 *    ************************************************************
 *                              Misc
 *    ************************************************************
 *
 *    T fromStr<class T> (string s)
 *       Converts a string to a T class.
 *       T needs to be convertible from istringstream via
 *       operator >>.
 *
 *    string toStr<class T> (T v)
 *       Converts T class to a string.
 *       T needs to be convertible to ostringstream via
 *       operator <<.
 *
 *    class varManager<class T>
 *       Stores variables of a class T, with an unique name
 *       for each of them.
 *       Requirements:
 *          T::T()  constructor
 *          T::operator=(T) assignment
 *       Methods:
 *          void varManager::write(string id, T val)
 *             Stores val with the name id. If id already
 *             exists, overwrites the current value.
 *          bool varManager::read(string id, T &val)
 *             Reads the value of a stored variable to val.
 *             Returns true on success.
 *
 *    ************************************************************
 *                        Matrix functions
 *    ************************************************************
 *
 *    double **matrixNew(int numLines, int numCols)
 *       Generates a new array of doubles with the specified size.
 *
 *    double **matrixNew(int numLines, int numCols, double **matrix)
 *       Generates a new array of doubles with the same size and
 *       contents of another matrix.
 *
 *    void matrixDelete(int numLines, double **matrix)
 *       Frees the memory in an array.
 *
 *    void matrixAssign(int numLines, int numCols, double **matrix, ...)
 *       Receives numLines times numCols double values after the
 *       last parameter, and places those values in the matrix.
 *       Values are placed in order on each line.
 *
 *    double matrixDet(int numLines, double **matrix)
 *       Calculates determinant of a square matrix of doubles,
 *       with numLines lines.
 *
 *    bool matrixInverse(int numLines, double **matrix)
 *       Replaces the matrix with its inverse. Returns false if
 *       the matrix is not invertible.
 *
 *    double *matrixSolveSystem(int numLines, double **matrix)
 *       Calculates all the results of a linear system, represented
 *       in a numLines by numLines+1 matrix. Returns null if system
 *       is not solvable.
 *
 *    ************************************************************
 *                             Vectors
 *    ************************************************************
 *
 *    class vector2d
 *       A two dimensional vector.
 *       Constants:
 *          x2d, y2d
 *             Vectors (1, 0) and (0, 1). These values cannot be
 *             changed, but can be used in operations normally.
 *       Operators:
 *          vector2d operator =(vector2d)
 *          vector2d operator +(vector2d)
 *          vector2d operator +=(vector2d)
 *          vector2d operator -(vector2d)
 *          vector2d operator -=(vector2d)
 *          vector2d operator *(double)
 *          vector2d operator *=(double)
 *          vector2d operator /(double)
 *          vector2d operator /=(double)
 *          vector2d operator -(void)
 *          bool operator ==(vector2d)
 *          bool operator !=(vector2d)
 *          bool operator >(vector2d)
 *          bool operator >=(vector2d)
 *          bool operator <(vector2d)
 *          bool operator <=(vector2d)
 *          double operator *(vector2d)
 *             Dot product.
 *          operator string()
 *       Methods:
 *          vector2d()
 *             Constructor. X and Y are set to 0.
 *          vector2d(double newX, double newY)
 *             Constructor. X is set to newX and y is set to newY.
 *          vector2d(vector2d v)
 *             Constructor. X and Y are copied from v.
 *          double getX()
 *          setX(double)
 *          double getY()
 *          setY(double)
 *          double getR()
 *          setR(double)
 *             Length.
 *          double getA()
 *          setA(double)
 *             Angle.
 *          double dot(vector2d)
 *             Dot product.
 *          double cross(vector2d)
 *             Cross product. The result is the Z component of the
 *             resulting 3d vector.
 *          void rotate(double angle)
 *             Rotates the vector counter-clockwise.
 *          void transform(double **matrix)
 *             Performs a linear transformatrix. matrix must be
 *             2 by 2.
 *
 *    class vector3d
 *       A three dimensional vector.
 *       Constants:
 *          x3d, y3d, z3d
 *             Vectors (1, 0, 0), (0, 1, 0) and (0, 0, 1). These
 *             values cannot be changed, but can be used in
 *             operations normally.
 *       Operators:
 *          vector3d operator =(vector3d)
 *          vector3d operator +(vector3d)
 *          vector3d operator +=(vector3d)
 *          vector3d operator -(vector3d)
 *          vector3d operator -=(vector3d)
 *          vector3d operator *(double)
 *          vector3d operator *=(double)
 *          vector3d operator /(double)
 *          vector3d operator /=(double)
 *          vector3d operator -(void)
 *          bool operator ==(vector3d)
 *          bool operator !=(vector3d)
 *          bool operator >(vector3d)
 *          bool operator >=(vector3d)
 *          bool operator <(vector3d)
 *          bool operator <=(vector3d)
 *          double operator *(vector3d)
 *             Dot product.
 *          operator string()
 *       Methods:
 *          vector3d()
 *             Constructor. X, y and z are set to 0.
 *          vector3d(double newX, double newY, double newZ)
 *             Constructor. X is set to newX, y is set to newY and
 *             z is set to newZ.
 *          vector3d(vector3d v)
 *             Constructor. X, y and z are copied from v.
 *          double getX()
 *          setX(double)
 *          double getY()
 *          setY(double)
 *          double getZ()
 *          setZ(double)
 *          double getR()
 *          setR(double)
 *             Length.
 *          double getA1()
 *          setA1(double)
 *             Angle with the Z axis.
 *          double getA2()
 *          setA2(double)
 *             Angle between the X axis and the projection of the
 *             vector on the XY plane.
 *          double dot(vector3d)
 *             Dot product.
 *          vector3d cross(vector3d)
 *             Cross product.
 *          void rotate(double angle, double dirX, double dirY, double dirZ)
 *             Rotates the vector counter-clockwise around the
 *             direction dirX, dirY, dirZ.
 *          void rotate(double angle, vector3d v)
 *             Rotates the vector counter-clockwise around the
 *             v vector.
 *          void transform(double **matrix)
 *             Performs a linear transformatrix. matrix must be
 *             3 by 3.
 *
 */

#ifndef MY_UTI_H
#define MY_UTI_H "myUti.h"

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdarg>

/*****************************************************************
                               Misc
*****************************************************************/

template <class T>
T fromStr(std::string s);

template <class T>
std::string toStr(T v);

template <class T>
class varManager{
   private:
      class varType{
         public:
            std::string name;
            T value;
            varType() {}
            varType(std::string s): name(s) {}
            varType(std::string s, T v): name(s) {value = v;}
            varType(const varType &v): name(v.name) {value = v.value;}
            const varType &operator=(const varType &v);
            bool operator<(const varType &v) const;
      };
      std::vector<varType> varList;
   public:
      void write(const std::string &id, const T &val);
      bool read(const std::string &id, T &val);
};

/*****************************************************************
                       Matrix Functions
*****************************************************************/

double **matrixNew(int numLines, int numCols);

double **matrixNew(int numLines, int numCols, double **matrix);

void matrixDelete(int numLines, double **matrix);

void matrixAssign(int numLines, int numCols, double **matrix, ...);

double matrixDet(int numLines, double **matrix);

bool matrixInverse(int numLines, double **matrix);

double *matrixSolveSystem(int numLines, double **matrix);

/*****************************************************************
                            Vectors
*****************************************************************/

class vector2d{
   private:
      double x, y;
   public:
      vector2d(){}
      vector2d(double newX, double newY):x(newX),y(newY){}
      vector2d(const vector2d &v):x(v.x),y(v.y){}
      double getX() const;
      void setX(double newX);
      double getY() const;
      void setY(double newY);
      double getR() const;
      void setR(double newR);
      double getA() const;
      void setA(double newA);
      double dot(const vector2d &v) const;
      double cross(const vector2d &v) const;
      void rotate(double angle);
      void transform(double **matrix);
      const vector2d &operator =(const vector2d &v);
      const vector2d operator +(const vector2d &v) const;
      const vector2d &operator +=(const vector2d &v);
      const vector2d operator -(const vector2d &v) const;
      const vector2d &operator -=(const vector2d &v);
      const vector2d operator *(double d) const;
      friend const vector2d operator *(double d, const vector2d &v);
      const vector2d &operator *=(double d);
      const vector2d operator /(double d) const;
      const vector2d &operator /=(double d);
      const vector2d operator -(void) const;
      bool operator ==(const vector2d &v) const;
      bool operator !=(const vector2d &v) const;
      bool operator >(const vector2d &v) const;
      bool operator >=(const vector2d &v) const;
      bool operator <(const vector2d &v) const;
      bool operator <=(const vector2d &v) const;
      double operator *(const vector2d &v) const;
      operator std::string() const;
};

extern const vector2d x2d, y2d;

class vector3d{
   private:
      double x, y, z;
   public:
      vector3d(){}
      vector3d(double newX, double newY, double newZ):x(newX),y(newY),z(newZ){}
      vector3d(const vector3d &v):x(v.x),y(v.y),z(v.z){}
      double getX() const;
      void setX(double newX);
      double getY() const;
      void setY(double newY);
      double getZ() const;
      void setZ(double newZ);
      double getR() const;
      void setR(double newR);
      double getA1() const;
      void setA1(double newA1);
      double getA2() const;
      void setA2(double newA2);
      double dot(const vector3d &v) const;
      const vector3d cross(const vector3d &v) const;
      void rotate(double angle, double dirX, double dirY, double dirZ);
      void rotate(double angle, const vector3d &v);
      void transform(double **matrix);
      const vector3d &operator =(const vector3d &v);
      const vector3d operator +(const vector3d &v) const;
      const vector3d &operator +=(const vector3d &v);
      const vector3d operator -(const vector3d &v) const;
      const vector3d &operator -=(const vector3d &v);
      const vector3d operator *(double d) const;
      friend const vector3d operator *(double d, const vector3d &v);
      const vector3d &operator *=(double d);
      const vector3d operator /(double d) const;
      const vector3d &operator /=(double d);
      const vector3d operator -(void) const;
      bool operator ==(const vector3d &v) const;
      bool operator !=(const vector3d &v) const;
      bool operator >(const vector3d &v) const;
      bool operator >=(const vector3d &v) const;
      bool operator <(const vector3d &v) const;
      bool operator <=(const vector3d &v) const;
      double operator *(const vector3d &v) const;
      operator std::string() const;
};

extern const vector3d x3d, y3d, z3d;

/*****************************************************************
                        Implementation
*****************************************************************/

template <class T>
T fromStr(std::string s){
   std::istringstream st(s);
   T result;
   st >> result;
   return result;
}

template <class T>
std::string toStr(T v){
   std::ostringstream st;
   st << v;
   return st.str();
}

template <class T>
const typename varManager<T>::varType &varManager<T>::varType::operator=(const varType &v){
   name = v.name;
   value = v.value;
   return *this;
}

template <class T>
bool varManager<T>::varType::operator<(const varType &v) const{
   return name.compare(v.name)<0;
}

template <class T>
void varManager<T>::write(const std::string &id, const T &val){
   varType newVar(id, val);
   typename std::vector<varType>::iterator it;
   it = std::lower_bound(varList.begin(), varList.end(), newVar);
   if (std::binary_search(varList.begin(), varList.end(), newVar)){
      *it = newVar;
      return;
   }
   varList.insert(it, newVar);
}

template <class T>
bool varManager<T>::read(const std::string &id, T &val){
   varType newVar(id);
   if (std::binary_search(varList.begin(), varList.end(), newVar)){
      typename std::vector<varType>::iterator it;
      it = std::lower_bound(varList.begin(), varList.end(), newVar);
      val = (*it).value;
      return true;
   }
   return false;
}

#endif
