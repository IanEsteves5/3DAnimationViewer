/*
 * 3D Animation Viewer
 *
 * By Ian Esteves do Nascimento
 * Universidade Federal do Rio de Janeiro, 02/2013
 *
 * This program takes a 3D model and a series
 * of key frames, and shows the animation using
 * geometric interpolation between the key frames.
 * 
 * The model is read from an .obj file and the
 * animation from an .anim file.
 *
 * The .obj file can be exported from blender.
 *
 * The anim file specification can be found in
 * the animFileSpecification.txt file that
 * should come with this code.
 *
 */

#include <cmath>
#include <ctime>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "myUti.h"
#include "objModel.h"
#include "animAnimation.h"

#define WINDOW_W 800.0
#define WINDOW_H 800.0
#define FPS 24.0

using namespace std;

/* Global variables used to transfer data between
 * functions.
 */
int mode(3), reset(0), currentFrame;
float motion;
objModel model;
animAnimation animation;
vector3d position(0.0, 0.0, 0.0), axis(0.0, 0.0, 0.0);
double angle(0.0), scale(1.0);

/* Displays 2D text on screen. The lower-left part
 * of the text is displayed at the pixel (x, y) of
 * the screen, starting from the top-left. The
 * text is 18 pixels high. For optimal use, y
 * should be a multiple of 20.
 */
void displayText(float x, float y, string text);

/* This function is called once every frame.
 * f is the current frame.
 */
void programLoop(int f){
   switch(mode){ // Mode selects which animation will be displayed.
      case 2: // Shows the key frames.
         position = animation.getPos1(f);
         axis = animation.getRotAxis1(f);
         angle = animation.getRotAngle1(f);
         scale = animation.getScale1(f);
         break;
      case 3:// Shows a linear interpolation of the key frames.
         position = animation.getPos2(f);
         axis = animation.getRotAxis2(f);
         angle = animation.getRotAngle2(f);
         scale = animation.getScale2(f);
         break;
      case 4: // Shows a quadratic interpolation of the key frames.
         position = animation.getPos3(f);
         axis = animation.getRotAxis3(f);
         angle = animation.getRotAngle3(f);
         scale = animation.getScale3(f);
         break;
      default: // Just shows the model spinning.
         position = vector3d(0.0, 0.0, 0.0);
         axis = vector3d(0.0, 0.0, 1.0);
         angle = 2.6*f;
         scale = 1;
   }
}

/* This function draws the model at the specified
 * position, rotation and scale.
 */
void programDisplay(void){
   glPushMatrix();
   glTranslatef(position.getX(), position.getY(), position.getZ());
   glRotatef(angle, axis.getX(), axis.getY(), axis.getZ());
   glRotatef(90.0, 1.0, 0.0, 0.0);
   glScalef(scale, scale, scale);
   
   glColor4f(0.2, 0.6, 0.2, 1.0);
   
   for(int i = 0 ; i < model.size() ; i++){ // Displaying each face.
      vector3d vNormal;
      vNormal = (model[i][1]-model[i][0]).cross(model[i][2]-model[i][0]); // Uses the normal of the first triangle of the face.
      vNormal.setR(1);
      glNormal3f(vNormal.getX(), vNormal.getY(), vNormal.getZ());
      glBegin(GL_TRIANGLE_FAN);
         for(int j = 0 ; j < model[i].size() ; j++){ // A face can have any number of vertexes.
            vector3d v = model[i][j]; // Getting the vertex position.
            glVertex3f(v.getX(), v.getY(), v.getZ());
         }
      glEnd();
   }
   /*for(int i = 0 ; i < model.size() ; i++){ // Uses different normal for each triangle.
      vector3d vNormal, vVertex;              // Looks bad.
      for(int j = 2 ; j < model[i].size() ; j++){
         vNormal = (model[i][j-1]-model[i][0]).cross(model[i][j]-model[i][0]);
         vNormal.setR(1);
         if(vNormal.dot((model[i][0]+model[i][j-1]+model[i][j])/3)<0)
            vNormal *= -1;
         glNormal3f(vNormal.getX(), vNormal.getY(), vNormal.getZ());
         glBegin(GL_TRIANGLES);
            vVertex = model[i][0];
            glVertex3f(vVertex.getX(), vVertex.getY(), vVertex.getZ());
            vVertex = model[i][j-1];
            glVertex3f(vVertex.getX(), vVertex.getY(), vVertex.getZ());
            vVertex = model[i][j];
            glVertex3f(vVertex.getX(), vVertex.getY(), vVertex.getZ());
         glEnd();
      }
   }*/
   glPopMatrix();
   
   // Information to the user.
   displayText(2, 20, "Digite um numero para mudar o tipo de animacao");
   displayText(2, 40, "1 : Ver modelo");
   displayText(2, 60, "2 : Ver keyframes");
   displayText(2, 80, "3 : Ver animacao interpolada linearmente");
   displayText(2, 100, "4 : Ver animacao interpolada quadraticamente");
   displayText(2, 120, "Frame     : " + toStr<int>(currentFrame));
   displayText(2, 140, "Posicao X : " + toStr<double>(position.getX()));
   displayText(2, 160, "Posicao Y : " + toStr<double>(position.getY()));
   displayText(2, 180, "Posicao Z : " + toStr<double>(position.getZ()));  
   displayText(2, 200, "Rotacao X : " + toStr<double>(axis.getX()));
   displayText(2, 220, "Rotacao Y : " + toStr<double>(axis.getY()));
   displayText(2, 240, "Rotacao Z : " + toStr<double>(axis.getZ()));
   displayText(2, 260, "Angulo    : " + toStr<double>(angle));
}

// Functions given to GLUT.
void display(void);
void reshape(int w, int h);
void timer(int f);
void keyboard(unsigned char key, int x, int y);
void init(void);

/************************************************************
 **************        MAIN PROGRAM        ******************
 ************************************************************/
int main(int argc, char **argv){
   // Initializing GLUT.
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition(200, 200);
   glutInitWindowSize(WINDOW_W, WINDOW_H);
   glutCreateWindow(argv[0]);
   
   // Passing functions to GLUT.
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutTimerFunc(0, timer, 0);
   glutKeyboardFunc(keyboard);
   
   // Initializing variables and OpenGL.
   init();
  
   glutMainLoop();
   
   return 0;
}

// Clears buffer, calls programDisplay then swaps buffers.
void display(void){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   programDisplay();
   glutSwapBuffers();
}

// Stretches the picture to fit the screen.
void reshape(int w, int h){
   glViewport(0, 0, w, h);
   glutPostRedisplay();
}

/* This functions controlls the timing between the
 * frames. It calls itself again after 1/FPS seconds.
 * On each frame, it detects if reset is different
 * than 0, if it is, it returns to frame 0. On every
 * frame, calls programLoop.
 */
void timer(int f){
   if(reset){
      f = 0;
      reset = 0;
   }
   glutTimerFunc(1000.0/FPS, timer, f + 1);
   currentFrame = f;
   programLoop(f);
   glutPostRedisplay();
}

// Selects which mode will be displayed.
void keyboard(unsigned char key, int x, int y){
   switch(key){
      case '1':
         mode = 1;
         reset = 1;
         break;
      case '2':
         mode = 2;
         reset = 1;
         break;
      case '3':
         mode = 3;
         reset = 1;
         break;
      case '4':
         mode = 4;
         reset = 1;
   }
}

// Initialization function.
void init(void){
   cout << "Trabalho CG" << endl;
   cout << "Ian Esteves do Nascimento" << endl;
   cout << "Universidade Federal do Rio de Janeiro" << endl;
   cout << "02/2013" << endl;
   cout << "OpenGL version " << (char *)glGetString(GL_VERSION) << endl;
   cout << (char *)glGetString(GL_RENDERER) << " by " << (char *)glGetString(GL_VENDOR) << endl;
   cout << endl;
   // Setting up screen
   glClearColor(0.3, 0.3, 0.3, 1.0);
   glViewport(0, 0, WINDOW_W, WINDOW_H);
   
   // Setting up camera
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(65, WINDOW_W/WINDOW_H, 1, 100);
   glEnable(GL_DEPTH_TEST);
   glMatrixMode(GL_MODELVIEW);
   gluLookAt(7.0, -6.0, 5.0,
             0.0, 0.0, 0.0, 
             0.0, 0.0, 1.0);
   
   // Setting up lighting
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_diffuse[] = { 0.4, 0.4, 0.4, 1.0 };
   GLfloat mat_shininess[] = { 25.0 };
   GLfloat light_position[] = { 4.0, 1.0, 6.0, 0.0 };
   GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
   GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
   GLfloat light_specular[] = { 0.8, 0.8, 0.8, 1.0 };
   glShadeModel (GL_SMOOTH);
   glEnable(GL_COLOR_MATERIAL);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   
   // Setting up model
   string fileName;
   cout << "Digite o nome do arquivo obj..." << endl;
   for(cin >> fileName ; model.import(fileName) ; cin >> fileName)
      cout << "Erro. Digite novamente..." << endl;
   cout << fileName << " aberto." << endl;
   
   // Setting up animation
   cout << "Digite o nome do arquivo anim..." << endl;
   for(cin >> fileName ; animation.import(fileName) ; cin >> fileName)
      cout << "Erro. Digite novamente..." << endl;
   cout << fileName << " aberto." << endl;
}

// Displays 2D text. See above for details.
void displayText(float x, float y, string text){
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity(); // Going into 2D mode.
   gluOrtho2D(0.0, WINDOW_W, WINDOW_H, 0.0);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   glColor4f(0.0, 0.0, 0.0, 1.0);
   glRasterPos2f(x, y);
   for(int i = 0 ; i < text.length() ; i++) // Actually writing letters.
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
   glPopMatrix(); // Going back to 3D.
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
}
