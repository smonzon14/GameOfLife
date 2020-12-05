#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <unistd.h>
#define FIELD_DIM 500
#define WIDTH  10.0
#define HEIGHT 10.0
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
 glClear( GL_COLOR_BUFFER_BIT);
 for(int x = 0; x < FIELD_DIM; ++x){
   for(int y = 0; y < FIELD_DIM; ++y){
			if(field[x][y]){
        born[x][y] ? glColor3fv(c2) : glColor3fv(c1);
				glBegin(GL_POLYGON);
				glVertex3f(WIDTH * x / FIELD_DIM, HEIGHT * y/FIELD_DIM, 0.0);
				glVertex3f(WIDTH * (x+1) / FIELD_DIM, HEIGHT * y/FIELD_DIM, 0.0);
        glVertex3f(WIDTH * (x+1) / FIELD_DIM, HEIGHT * (y+1)/FIELD_DIM, 0.0);
				glVertex3f(WIDTH * x / FIELD_DIM, HEIGHT * (y+1)/FIELD_DIM, 0.0);
				glEnd();
			}
		}
   }
  glFlush();
  glutPostRedisplay();
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
  glClearColor(0.0, 0.0, 0.0, 0.0);         
  glMatrixMode(GL_PROJECTION);              
  glLoadIdentity();                           
  glOrtho(0.0, WIDTH, 0.0, HEIGHT, -1.0, 1.0);  
  glutDisplayFunc(display);
  glutIdleFunc(updateField);
  glutMainLoop();

  return 0;
}