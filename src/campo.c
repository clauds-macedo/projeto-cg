#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ALTURA_TRAVE 7.32
#define DIAMETRO_BOLA 0.5f
#define TRANSLACAO_BOLA 0.5f

GLfloat ROTACAO_BOLA = 0.0f;

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

void DrawCircle(float cx, float cy, float r, int num_segments) 
{ 
	float theta = (float)2 * 3.1415926 / (num_segments); 
	float tangetial_factor = tanf(theta);//calculate the tangential factor 

	float radial_factor = cosf(theta);//calculate the radial factor 
	
	float x = r;//we start at angle = 0 

	float y = 0; 
    
	glBegin(GL_LINE_LOOP); 
	int ii = 0;
	for(; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
        
		//calculate the tangential vector 
		//remember, the radial vector is (x, y) 
		//to get the tangential vector we flip those coordinates and negate one of them 

		float tx = -y; 
		float ty = x; 
        
		//add the tangential vector 

		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
        
		//correct using the radial factor 

		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glEnd(); 
}

void subtrair_rotacao_bola()
{
    ROTACAO_BOLA -= 45;
    if (ROTACAO_BOLA < 0)
        ROTACAO_BOLA = 360;
    glutPostRedisplay();
}

void somar_rotacao_bola() 
{
    ROTACAO_BOLA += 45;
    if (ROTACAO_BOLA > 360)
        ROTACAO_BOLA = 0;
    glutPostRedisplay();
}


void resetRotacaoExcept(char eixo)
{ //eixo pode ser x, y, z
    switch(eixo) 
    {
        case 'x':
            if (bola.rotY == 1) bola.rotY = 0;
            if (bola.rotZ == 1) bola.rotZ = 0;
            break;
        case 'y':
            if (bola.rotX == 1) bola.rotX = 0;
            if (bola.rotZ == 1) bola.rotZ = 0;
            break;
        case 'z':
            if (bola.rotY == 1) bola.rotY = 0;
            if (bola.rotX == 1) bola.rotX = 0;
            break;
        default:    break;
    }
}

void resetBallPosition() {
	bola.transX = 0;
	bola.transZ = 0;
}

void SpinBack()
{
    T -= 1;
    if (T < 0)
        T = 360;
    glutPostRedisplay();
}

void Spin()
{
   T += 1;
   if (T > 360)
       T = 0;
   glutPostRedisplay();
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
	// printf("%f %f\n", goalPos, bola.transZ);
	if (bola.transX <= goalPos && (bola.transZ <= 1.5 && bola.transZ >= -1.5)) {
		goalEsq++;
		printf("FOI FOI FOI FOI FOI FOI FOI FOI DELEEEEE");	
		resetBallPosition();
	}
	else if (bola.transX >= -goalPos && (bola.transZ <= 1.5 && bola.transZ >= -1.5)) {
		goalDir++;
		printf("GOOOOOOOOOOOOOOOOOOOL DA ALEMANHA");	
		resetBallPosition();
	}
	
	char str[80];
    sprintf(str, "%d X %d", goalEsq, goalDir);
    
    RenderString(0.0f, 5.0f, GLUT_BITMAP_TIMES_ROMAN_24, str);
}

void traves(GLfloat V0[], GLfloat V1[])
{
    // trave esq
    glBegin(GL_LINES);
        glVertex3f(V0[0], V0[1], V0[2]-0.3);
        glVertex3f(V0[0], V0[1]+3, V0[2]-0.3);
        
        glVertex3f(V0[0], V0[1], V0[2]-0.6);
        glVertex3f(V0[0], V0[1]+3, V0[2]-0.6);

        glVertex3f(V0[0], V0[1]+3, V0[2]-0.3);
        glVertex3f(V0[0], V0[1]+3, V0[2]-0.6);
    glEnd();
    
    //trave dir
    glBegin(GL_LINES);
        glVertex3f(V1[0], V1[1], V1[2]-0.3);
        glVertex3f(V1[0], V1[1]+3, V1[2]-0.3);

        glVertex3f(V1[0], V1[1], V1[2]-0.6);
        glVertex3f(V1[0], V1[1]+3, V1[2]-0.6);

        glVertex3f(V1[0], V1[1]+3, V1[2]-0.3);
        glVertex3f(V1[0], V1[1]+3, V1[2]-0.6);
    glEnd();
}

void fieldLines(GLfloat V0[], GLfloat V1[], GLfloat V5[], GLfloat V4[])
{
    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
        glVertex3fv(V0);
        glVertex3fv(V1);
        glVertex3fv(V5);
        glVertex3fv(V4);
    glEnd();
    //

    traves(V0, V1);
	
	// centro do campo
    glBegin(GL_LINES);
    	glVertex3f(0, V0[1]+0.1,V0[2]-0.01);
    	glVertex3f(0, V0[1]+0.1, -V0[2]+0.01);
    glEnd();
    
    //lado esquerdo
    glBegin(GL_LINES);
    	glVertex3f(V0[0], V0[1]+0.1, V0[2]-0.2);
        glVertex3f(V0[0]+0.1, V0[1]+0.1, V0[2]-0.2);
    glEnd();
    
    glBegin(GL_LINES);
    	glVertex3f(V0[0], V0[1]+0.1, V0[2]-0.8);
        glVertex3f(V0[0]+0.1, V0[1]+0.1, V0[2]-0.8);
    glEnd();

	glBegin(GL_LINES);
    	glVertex3f(V0[0]+0.1, V0[1]+0.1, V0[2]-0.8);
        glVertex3f(V0[0]+0.1, V0[1]+0.1, V0[2]-0.2);
    glEnd();

    // lado direito
    glBegin(GL_LINES);
    	glVertex3f(V1[0], V1[1]+0.1, V1[2]-0.2);
        glVertex3f(V1[0]-0.1, V1[1]+0.1, V1[2]-0.2);
    glEnd();
    
    glBegin(GL_LINES);
    	glVertex3f(V1[0], V1[1]+0.1, V1[2]-0.8);
        glVertex3f(V1[0]-0.1, V1[1]+0.1, V1[2]-0.8);
    glEnd();

	glBegin(GL_LINES);
    	glVertex3f(V1[0]-0.1, V1[1]+0.1, V1[2]-0.8);
        glVertex3f(V1[0]-0.1, V1[1]+0.1, V1[2]-0.2);
    glEnd();
    
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

void desenha_bola()
{
    glPushMatrix();
        glColor3f(bola.r, bola.g, bola.b);
        glTranslatef(bola.transX, 0.5, bola.transZ);
        glScalef(DIAMETRO_BOLA, DIAMETRO_BOLA, DIAMETRO_BOLA);
        glRotatef(ROTACAO_BOLA, bola.rotX, bola.rotY, bola.rotZ);
        glutWireSphere(0.5f, 10, 10);
    glPopMatrix();
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
        // glRotatef(T, 0, 1, 0);
        glScalef(30, 0.5, 10);
        Cube(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7]);
    glPopMatrix();
    
    desenha_bola();

    glTranslatef(0, 0.29, 0);
    glRotatef(90, 1, 0, 0);
    DrawCircle(0, 0, 1.5, 18);

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
    printf("%f\n", ROTACAO_BOLA);
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
        case 'W':
            bola.transZ -= TRANSLACAO_BOLA;
            bola.rotX = 1;
            somar_rotacao_bola();
            resetRotacaoExcept('x');
            break;
        case 's':
        case 'S': 
            bola.transZ += TRANSLACAO_BOLA;  
            bola.rotX = 1;
            resetRotacaoExcept('x');
            subtrair_rotacao_bola();
            break;

        case 'a':
        case 'A':
            bola.transX -= TRANSLACAO_BOLA;
            bola.rotZ = 1;
            resetRotacaoExcept('z');
            somar_rotacao_bola();
            break;
        case 'd': 
        case 'D':
            bola.transX += TRANSLACAO_BOLA;
            bola.rotZ = 1;
            resetRotacaoExcept('z');
            subtrair_rotacao_bola();
            break;
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
    glutInitWindowSize(720, 600);
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
