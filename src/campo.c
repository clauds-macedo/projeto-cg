#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLfloat Cx = 0, Cy = 15, Cz = 30;
GLfloat atX = 0, atY = 0, atZ = 0;
GLfloat T = 0;
int enabled = 1;
int goalEsq = 0;
int goalDir = 0;

typedef struct ball {
    GLfloat transX, transZ;
    GLfloat rotX, rotY, rotZ;
    GLfloat r, g, b;
} Bola;

Bola bola = {
        0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f
    };
 
void resetRotacao()
{
    if (bola.rotX == 1) bola.rotX = 0; 
    if (bola.rotY == 1) bola.rotY = 0; 
    if (bola.rotZ == 1) bola.rotZ = 0; 
}

void resetBallPosition() {
	bola.transX = 0;
}

void Spin()
{
//    T += 1;
//    if (T > 360)
//        T = 0;
//    glutPostRedisplay();
}

void printAr(GLfloat a[], char* name)
{
    printf("%s: ", name);
    int i;
    for (i = 0; i < 3; i++) 
        printf("%f ",a[i]);
    printf("\n");
}

void RenderString(float x, float y, void *font, const char* string)
{  
  	char *c;

  	glColor3f(1, 1, 1); 
  	glRasterPos2f(x, y);

  	glutBitmapString(font, string);
}

void checkGOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOL(GLfloat goalPos) {
	printf("%f %f\n", goalPos, bola.transX);
	if (bola.transX <= goalPos) {
		goalEsq++;
		printf("FOI FOI FOI FOI FOI FOI FOI FOI DELEEEEE");	
		resetBallPosition();
	}
	else if (bola.transX >= -goalPos) {
		goalDir++;
		printf("GOOOOOOOOOOOOOOOOOOOL DA ALEMANHA");	
		resetBallPosition();
	}
	
	char bufferEsq[20];
	char bufferDir[20];
	char separator[] = {" X "};
	
	char str[80];
    sprintf(str, "%d X %d", goalEsq, goalDir);
    
    RenderString(0.0f, 5.0f, GLUT_BITMAP_TIMES_ROMAN_24, str);
}

void fieldLines(GLfloat V0[], GLfloat V1[], GLfloat V5[], GLfloat V4[])
{
    glColor3f(1,1,1);
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
        glVertex3fv(V0);
        glVertex3fv(V1);
        glVertex3fv(V5);
        glVertex3fv(V4);
    glEnd();
    glPopMatrix();
    //

    glColor3f(1,0,1);
    glBegin(GL_LINES);
        glVertex3f(V0[0], V0[1], V0[2]-0.3);
        glVertex3f(V0[0], V0[1]+3, V0[2]-0.3);
        
        glVertex3f(V0[0], V0[1], V0[2]-0.6);
        glVertex3f(V0[0], V0[1]+3, V0[2]-0.6);

        glVertex3f(V0[0], V0[1]+3, V0[2]-0.3);
        glVertex3f(V0[0], V0[1]+3, V0[2]-0.6);
    glEnd();
    
    
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    	glVertex3f(V0[0], V0[1]+0.1, V0[2]-0.2);
        glVertex3f(V0[0]+0.1, V0[1]+0.1, V0[2]-0.2);
    glEnd();
    
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    	glVertex3f(V0[0], V0[1]+0.1, V0[2]-0.8);
        glVertex3f(V0[0]+0.1, V0[1]+0.1, V0[2]-0.8);
    glEnd();

	glColor3f(1,1,1);
    glBegin(GL_LINES);
    	glVertex3f(V0[0]+0.1, V0[1]+0.1, V0[2]-0.8);
        glVertex3f(V0[0]+0.1, V0[1]+0.1, V0[2]-0.2);
    glEnd();
    
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex3f(V1[0], V1[1]+10, V1[2]);
        glVertex3fv(V5);
    glEnd();
    
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex3f(V1[0], V1[1], V1[2]);
        glVertex3fv(V5);
    glEnd();
    
    glPopMatrix();
    glColor3f(0,1,0);
    
    checkGOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOL(-(V1[1]+15));
}


void Square(GLfloat A[], GLfloat B[], GLfloat C[], GLfloat D[])
{
    glBegin(GL_POLYGON);
        glVertex3fv(A);
        glVertex3fv(B);
        glVertex3fv(C);
        glVertex3fv(D);
    glEnd();
}

void Cube (
    GLfloat V0[], GLfloat V1[], GLfloat V2[], GLfloat V3[], 
    GLfloat V4[],GLfloat V5[],GLfloat V6[], GLfloat V7[]
)
{
    glColor3f(0, 1, 0);
    Square(V0, V1, V2, V3); //front
    Square(V4, V5, V6, V7); //back
    Square(V0, V4, V7, V3); //left
    Square(V1, V5, V6, V2); //right
    Square(V3, V2, V6, V7); // bottom
    Square(V0, V1, V5, V4); //top
    fieldLines(V0, V1, V5, V4);

}

void display()
{
    //vertices de um cubo
    
    
    GLfloat V[8][3] = {
        {-0.5f, 0.5f, 0.5f},
        { 0.5f, 0.5f, 0.5f},
        { 0.5f, -0.5f, 0.5f},
        {-0.5f, -0.5f, 0.5f},
        {-0.5f, 0.5f, -0.5f},
        { 0.5f, 0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},        
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    gluLookAt(
        Cx, Cy, Cz, //fique 5 passos em frente à origem 
        atX, atY, atZ, // olhe a origem
        0, 1, 0
    );
    
    glPushMatrix();
        glRotatef(T, 0, 1, 0);
        glScalef(30, 0.5, 10);
        Cube(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7]);
    glPopMatrix();
    
    // glLoadIdentity();
    glPushMatrix();
        glColor3f(bola.r, bola.g, bola.b);
        glTranslatef(bola.transX, 1, bola.transZ);
        glRotatef(T, 0, 1, 1);
        glutWireSphere(0.5f, 15, 15);
    glPopMatrix();


    glutSwapBuffers();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // ativar projeção em perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 2, 300); // z da camera deve estar entre 2 e 5
    
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) 
    {
        case '1': atX -= 0.5;   break;
        case '!': atX += 0.5;   break;
        
        case '2': atY -= 0.5;   break;
        case '@': atY += 0.5;   break;
        
        case '3': atZ -= 0.5;   break;
        case '#': atZ += 0.5;   break;
        
        case 'x': Cx -= 0.5;    break;
        case 'X': Cx += 0.5;    break;
        
        case 'y': Cy -= 0.5;    break;
        case 'Y': Cy += 0.5;    break;

        case 'z': Cz -= 0.5;    break;
        case 'Z': Cz += 0.5;    break;

        case 'w': 
            bola.transZ -= 0.5;  
            resetRotacao();
            bola.rotZ = 1;
            break;
        case 's': 
            bola.transZ += 0.5;  
            resetRotacao();
            bola.rotZ = 1;
            break;

        case 'a': bola.transX -= 0.5;  break;
        case 'd': bola.transX += 0.5;  break;

        case ' ':

            if (enabled) {
                glutIdleFunc(NULL);
                enabled = 0;
            }
            else {
                glutIdleFunc(Spin);
                enabled = 1;
            }
            break;
         
        default: break;
    }
    
    glutPostRedisplay();
}

void reshape(GLsizei w, GLsizei h)
{
    if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

        // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Campo");
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(Spin);
    glutReshapeFunc(reshape);
    init();
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 1+48);
    glutMainLoop();

    return 0;
}