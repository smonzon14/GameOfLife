#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <unistd.h>
#define FIELD_DIM 500
#define WIDTH  10.0
#define HEIGHT 10.0
#define CELL_W WIDTH/FIELD_DIM
#define CELL_H HEIGHT/FIELD_DIM
#define SEED_DIM FIELD_DIM

typedef bool field_array[FIELD_DIM][FIELD_DIM];

field_array field;
field_array temp;
field_array born; // just for fun to implement different colors

float c1[3] = {0.3,0.7,1.0};
float c2[3] = {0.0,1.0,0.5};

void initField(void){
  for(int x = (FIELD_DIM - SEED_DIM)/2; x < (FIELD_DIM + SEED_DIM)/2; ++x){
   for(int y = (FIELD_DIM - SEED_DIM)/2; y < (FIELD_DIM + SEED_DIM)/2; ++y){
     field[x][y] = rand() & 1;
   }
  }
}
int adjacentLivingCells(int x, int y){
  int r = 0;
  for(int i = x - 1; i < x + 2; ++i){
    for(int j = y - 1; j < y + 2; ++j){
      r+=(i >= 0 && j >= 0 && i < FIELD_DIM && j < FIELD_DIM) && field[i][j];
    }
  }
  
  return r - (field[x][y] ? 1 : 0);
}

void updateField(){
  for(int x = 0; x < FIELD_DIM; ++x){
   for(int y = 0; y < FIELD_DIM; ++y){
     int adjacentLiving = adjacentLivingCells(x,y);
     if(field[x][y]){
       temp[x][y] = ((adjacentLiving == 2) || (adjacentLiving == 3));
       born[x][y] = false;
     }else{
       temp[x][y] = (adjacentLiving == 3);
       born[x][y] = temp[x][y]; // Cell is reborn
     } 
   }
  }
  for(int x = 0; x < FIELD_DIM; ++x){
   for(int y = 0; y < FIELD_DIM; ++y){
    field[x][y] = temp[x][y];
   }
  }
}
void display(void)
{
 glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
 for(int x = 0; x < FIELD_DIM; ++x){
   for(int y = 0; y < FIELD_DIM; ++y){
			if(field[x][y]){
        born[x][y] ? glColor3fv(c2) : glColor3fv(c1);
				glBegin(GL_POLYGON);
        
        //Front Face
				glVertex3f(CELL_W * x, CELL_H * y, 0.0);
				glVertex3f(CELL_W * (x+1), CELL_H * y, 0.0);
        glVertex3f(CELL_W * (x+1), CELL_H * (y+1), 0.0);
				glVertex3f(CELL_W * x, CELL_H * (y+1), 0.0);
        
        //Back Face
        glVertex3f(CELL_W * x, CELL_H * y, -CELL_W);
				glVertex3f(CELL_W * (x+1), CELL_H * y, -CELL_W);
        glVertex3f(CELL_W * (x+1), CELL_H * (y+1), -CELL_W);
				glVertex3f(CELL_W * x, CELL_H * (y+1), -CELL_W);
        
        //Top
        glVertex3f(CELL_W * x, CELL_H * y, 0.0);
				glVertex3f(CELL_W * (x+1), CELL_H * y, 0.0);
        glVertex3f(CELL_W * x, CELL_H * y, -CELL_W);
				glVertex3f(CELL_W * (x+1), CELL_H * y, -CELL_W);

        //Bottom
        glVertex3f(CELL_W * x, CELL_H * (y+1), 0.0);
				glVertex3f(CELL_W * (x+1), CELL_H * (y+1), 0.0);
        glVertex3f(CELL_W * x, CELL_H * (y+1), -CELL_W);
				glVertex3f(CELL_W * (x+1), CELL_H * (y+1), -CELL_W);

        //Left
        glVertex3f(CELL_W * x, CELL_H * y, 0.0);
				glVertex3f(CELL_W * x, CELL_H * (y+1), 0.0);
        glVertex3f(CELL_W * x, CELL_H * (y+1), -CELL_W);
				glVertex3f(CELL_W * x, CELL_H * y, -CELL_W);

        //Right
        glVertex3f(CELL_W * (x+1), CELL_H * y, 0.0);
				glVertex3f(CELL_W * (x+1), CELL_H * (y+1), 0.0);
        glVertex3f(CELL_W * (x+1), CELL_H * (y+1), -CELL_W);
				glVertex3f(CELL_W * (x+1), CELL_H * y, -CELL_W);

				glEnd();
			}
		}
   }
  
  glFlush();
  glutPostRedisplay();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
         1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
         1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char **argv)
{
  srand(time(0));
  initField();
  glutInit(&argc, argv);
  glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(100,100);
  glutInitWindowSize(800,800);
  glutCreateWindow ("Game Of Life");
  
  // GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  // GLfloat mat_shininess[] = { 50.0 };
  // GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  // glClearColor (0.0, 0.0, 0.0, 0.0);
  // glShadeModel (GL_SMOOTH);

  // glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  // glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  // glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  // glEnable(GL_LIGHTING);
  // glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0.0, 0.0);         
  glMatrixMode(GL_PROJECTION);              
  glLoadIdentity();                           
  glOrtho(0.0, WIDTH, 0.0, HEIGHT, -5.0, 5.0);  
  
  // glRotatef(-40, 1.0f, 1.0f, 0.0f);
  // // glRotatef(-20, 1.0f, 0.0f, 0.0f);
  // glTranslatef(0, 1.0, 0);
  // move to center of circle 
  glutDisplayFunc(display);
  glutIdleFunc(updateField);
  //glutReshapeFunc(reshape);
  glutMainLoop();
  
  return 0;
}