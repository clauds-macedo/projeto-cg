#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DIAMETRO_BOLA 0.5f
#define TRANSLACAO_BOLA 0.3f
#define CX_INICIAL 0.0f
#define CY_INICIAL 15.0f
#define CZ_INICIAL 25.0f
#define Z_MAX_CAMPO 5.5f
#define Z_MIN_CAMPO -5.5f
#define CYLINDER_RADIUS 1
#define CYLINDER_HEIGHT 1
#define CYLINDER_SECTORS 21

GLfloat ROTACAO_BOLA = 0.0f;

GLfloat Cx = CX_INICIAL, Cy = CY_INICIAL, Cz = CZ_INICIAL;
GLfloat atX = 0, atY = 0, atZ = 0;

int goalEsq = 0;
int goalDir = 0;

typedef struct ball {
    GLfloat transX, transZ;
    GLfloat rotX, rotY, rotZ;
    GLfloat r, g, b;
} Bola;

typedef struct cam {
    GLfloat Cx, Cy, Cz;
    GLfloat atX, atY, atZ;
} Camera;

Bola bola = {
    0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f
};

Camera camera = {
    CX_INICIAL, CY_INICIAL, CZ_INICIAL,
    0, 0, 0
};

typedef struct Point {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} Point;

void set_point(Point* p, GLfloat x, GLfloat y, GLfloat z){ 
    p->x = x;
    p->y = y;
    p->z = z;
}

Point point(GLfloat x, GLfloat y, GLfloat z) {
    Point new;
    new.x = x;
    new.y = y;
    new.z = z;
    return new;
}

void print_point(Point p) {
    printf("%f %f %f\n", p.x, p.y, p.z);
}


void resetCamera()
{
    camera.Cx = CX_INICIAL;
    camera.Cy = CY_INICIAL;
    camera.Cz = CZ_INICIAL;
    camera.atX = 0.0f;
    camera.atY = 0.0f;
    camera.atZ = 0.0f;
}



void DrawCircle(float cx, float cy, float r, int num_segments) 
{ 
	float theta = (float)2 * 3.1415926 / (num_segments); 
	float tangetial_factor = tanf(theta);

	float radial_factor = cosf(theta);
	
	float x = r;

	float y = 0; 
    
	glBegin(GL_LINE_LOOP); 
	int ii = 0;
	for(; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);


		float tx = -y; 
		float ty = x; 
        
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
        

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

void RenderString(float x, float y, void *font, const char* string)
{  
  	glColor3f(1, 1, 1); 
  	glRasterPos2f(x, y);

  	glutBitmapString(font, string);
}

void checkGoal(GLfloat goalPos) {
	if (bola.transX <= goalPos) {
        if (bola.transZ <= 1.5 && bola.transZ >= -1.5) {
            goalEsq++;
        }
		resetBallPosition();
    }
	else if (bola.transX >= -goalPos) {
        if (bola.transZ <= 1.5 && bola.transZ >= -1.5) {
            goalDir++;
        }
		resetBallPosition();
    }
    
    if (bola.transZ >= Z_MAX_CAMPO || bola.transZ <= Z_MIN_CAMPO)
        resetBallPosition();

	char str[80];
    sprintf(str, "%d X %d", goalEsq, goalDir);
    
    RenderString(0.0f, 12.0f, GLUT_BITMAP_TIMES_ROMAN_24, str);
}

void SolidCylinder(float radius, float height, int sectors) {
	int i;
	float theta = 2.0f * M_PI / sectors;
	float top = height / 2.0f;
	float bottom = -top;
	
	glBegin(GL_TRIANGLES);
	
	for(i = 0;i < sectors;i++) {
		float c0 = cos(i * theta);
		float s0 = sin(i * theta);
		float c1 = cos((i + 1) * theta);
		float s1 = sin((i + 1) * theta);
		
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f,        top,    0.0f);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(radius * c0, top,    radius * s0);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(radius * c1, top,    radius * s1);
	
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(0.0f,        bottom, 0.0f);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(radius * c1, bottom, radius * s1);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(radius * c0, bottom, radius * s0);
	
		glNormal3f(c0, 0.0f, s0);
		glVertex3f(radius * c0, bottom, radius * s0);
		glNormal3f(c1, 0.0f, s1);
		glVertex3f(radius * c1, top,    radius * s1);
		glNormal3f(c0, 0.0f, s0);
		glVertex3f(radius * c0, top,    radius * s0);
		
		glNormal3f(c1, 0.0f, s1);
		glVertex3f(radius * c1, top,    radius * s1);
		glNormal3f(c0, 0.0f, s0);
		glVertex3f(radius * c0, bottom, radius * s0);
		glNormal3f(c1, 0.0f, s1);
		glVertex3f(radius * c1, bottom, radius * s1);
	}

	glEnd();
}


void traves(GLfloat V0[], GLfloat V1[])
{
    // // trave esquerda
    // // glPopMatrix();
    // glutSolidCylinder(1, 0.1, 23, 23);
    // // glPushMatrix();
    // //     glTranslatef(V0[0], 2, V0[2]-0.35);
    // //     glScalef(0.005, 3, 0.005);
    // //     SolidCylinder(CYLINDER_HEIGHT, CYLINDER_RADIUS, CYLINDER_SECTORS);
    // // glPopMatrix();

    // // glPushMatrix();
    //     // glTranslatef(V0[0], 2, V0[2]-0.65);
    //     // glScalef(0.005, 3, 0.005);
    //     // SolidCylinder(CYLINDER_HEIGHT, CYLINDER_RADIUS, CYLINDER_SECTORS);
    // // glPopMatrix();
    // glPushMatrix();
    //     glColor3f(1, 0, 0);
    //     glTranslatef(V0[0], V0[1]+3, V0[2]-0.50);
    //     glRotatef(-90, 1, 0, 0);
    //     glScalef(0.0058, 0.3, 0.0058);
    //     SolidCylinder(CYLINDER_HEIGHT, CYLINDER_RADIUS, CYLINDER_SECTORS);
    // glPopMatrix();
    // glColor3f(1, 1, 1);
    
    glBegin(GL_LINES);
        glVertex3f(V0[0], V0[1], V0[2]-0.35);
        glVertex3f(V0[0], V0[1]+3, V0[2]-0.35);
        
        glVertex3f(V0[0], V0[1], V0[2]-0.65);
        glVertex3f(V0[0], V0[1]+3, V0[2]-0.65);

        glVertex3f(V0[0], V0[1]+3, V0[2]-0.35);
        glVertex3f(V0[0], V0[1]+3, V0[2]-0.65);
    glEnd();
    
    //trave dir
    glBegin(GL_LINES);
        glVertex3f(V1[0], V1[1], V1[2]-0.35);
        glVertex3f(V1[0], V1[1]+3, V1[2]-0.35);

        glVertex3f(V1[0], V1[1], V1[2]-0.65);
        glVertex3f(V1[0], V1[1]+3, V1[2]-0.65);

        glVertex3f(V1[0], V1[1]+3, V1[2]-0.35);
        glVertex3f(V1[0], V1[1]+3, V1[2]-0.65);
    glEnd();
}

void plot(GLfloat x, GLfloat y, Point center) {
    glPointSize(2.0f);
    glBegin(GL_POINTS);
        glVertex2f(x+center.x, y+ center.y);
    glEnd();
}

void Bresenham3DCircle(Point center, GLfloat radius)
{
    GLfloat x = 0, y = radius, inc = 0.01;
    GLfloat decision = 5 / 4 - radius;
    
    plot(x, y, center);

    while (y > x) {
        if (decision < 0) {
            x += inc;
            decision += 2 * 2 * x + 1;
        }
        else {
            y -= inc;
            x += inc;
            decision += 2 * (x - y) + 1;
        }
        plot(x, y, center);
        plot(x, -y, center);
        plot(-x, y, center);
        plot(-x, -y, center);
        plot(y, x, center);
        plot(-y, x, center);
        plot(y, -x, center);
        plot(-y, -x, center);
    }
}

void Bresenham3DLine(Point p1, Point p2) {
	Point pixel;
    // printf("Point 1: ");
    // print_point(p1);
    // printf("Point 2: ");
    // print_point(p2);
	
    pixel.x = p1.x;
    pixel.y = p1.y;
    pixel.z = p1.z;

    // print_point(pixel);

    GLfloat dx = fabs(p2.x - p1.x);
    GLfloat dy = fabs(p2.y - p1.y);
    GLfloat dz = fabs(p2.z - p1.z);
    

    // printf("dx: %f dy: %f dz: %f\n", dx, dy, fabs(dz));

    GLfloat xs, ys, zs, par_1, par_2;

    xs = (p2.x > p1.x) ? 0.001 : -0.001;
    ys = (p2.y > p1.x) ? 0.001 : -0.001;
    zs = (p2.z > p1.z) ? 0.001 : -0.001;

    // printf("s: %f %f %f\n", xs, ys, zs);
    glPointSize(2.0f);
    
    glBegin(GL_POINTS);

    glVertex3f(pixel.x, pixel.y, pixel.z);

    if (dx >= dy && dx >= dz) {
        // printf("Entrei em x");
        par_1 = 2 * dy - dx;
        par_2 = 2 * dz - dx;

		while (pixel.x < p2.x) {
			pixel.x += xs;

			// printf("%f\n", pixel.x);

			if (par_1 >= 0) {
                pixel.y += ys;
                par_1 -= 2 * dx;
            }

            if (par_2 >= 0) {
                p1.z += zs;
                par_2 -= 2 * dx;
            }

            par_1 += 2 * dy;
            par_2 += 2 * dx;

            // printf("X-axis: ");
            // print_point(pixel);

			// drawPixel(pixel);
            glVertex3f(pixel.x, pixel.y, pixel.z);


		}
	}

	else if (dy >= dx && dy >= dz) {
        // printf("Entrei em y");

		par_1 = 2 * dx - dy;
        par_2 = 2 * dz - dy;
        
		while (pixel.y < p2.y) {
			pixel.y += ys;
			
            // printf("%f, y", pixel.y);

			if (par_1 >= 0) {
                pixel.x += xs;
                par_1 -= 2 * dy;
            }
            if (par_2 >= 0) {
                pixel.z += zs;
                par_2 -= 2 * dy;
            }
            par_1 += 2 * dx;
            par_2 -= 2 * dy;

            // printf("Y-axis: ");
            // print_point(pixel);
            // drawPixel(pixel);
            glVertex3f(pixel.x, pixel.y, pixel.z);

		}
	}
    else {
        // printf("Entrei em z\n");

        par_1 = 2 * dy - dz;
        par_2 = 2 * dx - dz;

        // printf("pixel: %f p2: %f\n", pixel.z, p2.z);
        while (pixel.z < p2.z) {
            pixel.z += zs;
            
            if (par_1 >= 0) {
                pixel.y += ys;
                par_1 -= 2 * dz;
            }
            if (par_2 >= 0) {
                pixel.x += xs;
                par_2 -= 2 * dz;
            }
            par_1 += 2 * dy;
            par_2 += 2 * dx;

            // printf("Z-axis: ");
            // print_point(pixel);
            // drawPixel(pixel);
            glVertex3f(pixel.x, pixel.y, pixel.z);

        }
    }
    glEnd();
}

void fieldLines(GLfloat V0[], GLfloat V1[], GLfloat V5[], GLfloat V4[])
{
    glLineWidth(2.0f);
    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
        glVertex3fv(V0);
        glVertex3fv(V1);
        glVertex3fv(V5);
        glVertex3fv(V4);
    glEnd();
        
    traves(V0, V1);
    
    // centro do campo
    Bresenham3DLine(point(0, V0[1]+0.1, -V0[2]+0.02), point(0, V0[1]+0.1, V0[2]-0.00));
	
    // lado esquerdo
    Bresenham3DLine(point(V0[0], V0[1]+0.1, V0[2]-0.2), point(V0[0]+0.1, V0[1]+0.1, V0[2]-0.2));
    Bresenham3DLine(point(V0[0], V0[1]+0.1, V0[2]-0.8), point(V0[0]+0.1, V0[1]+0.1, V0[2]-0.8));
    Bresenham3DLine(point(V0[0]+0.1, V0[1]+0.1, V0[2]-0.8), point(V0[0]+0.1, V0[1]+0.1, V0[2]-0.2));

    // lado direito
    Bresenham3DLine(point(V1[0]-0.1, V1[1]+0.1, V1[2]-0.2), point(V1[0], V1[1]+0.1, V1[2]-0.2));
    Bresenham3DLine(point(V1[0]-0.1, V1[1]+0.1, V1[2]-0.8), point(V1[0], V1[1]+0.1, V1[2]-0.8));
    Bresenham3DLine(point(V1[0]-0.1, V1[1]+0.1, V1[2]-0.8), point(V1[0]-0.1, V1[1]+0.1, V1[2]-0.2));
    
    glColor3f(0,1,0);
    
    checkGoal(-(V1[1]+15));
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
        camera.Cx, camera.Cy, camera.Cz, 
        camera.atX, camera.atY, camera.atZ,
        0, 1, 0
    );
    
    glPushMatrix();
        // glRotatef(T, 0, 1, 0);
        glScalef(30, 0.5, 10);
        Cube(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7]);
    glPopMatrix();
    
    desenha_bola();

    // glRotatef(90, 1, 0, 0);
    glPopMatrix();
    // glTranslatef(0, 2, 0);
    Bresenham3DCircle(point(0, 0, 0), 1.5);
    // DrawCircle(0, 0, 1.5, 18);
    // glPopMatrix();

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

void specialKeys(int key, int x, int y)
{
    switch(key) {
        case (GLUT_KEY_LEFT): camera.atX -= 0.5;  break;
        case (GLUT_KEY_RIGHT): camera.atX += 0.5;  break;

        case (GLUT_KEY_UP): camera.atY += 0.5;     break;
        case (GLUT_KEY_DOWN): camera.atY -= 0.5;   break;
        
        default:    break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) 
    {        
        case 'x': camera.Cx -= 0.5;    break;
        case 'X': camera.Cx += 0.5;    break;
        
        case 'y': camera.Cy -= 0.5;    break;
        case 'Y': camera.Cy += 0.5;    break;

        case 'z': camera.Cz -= 0.5;    break;
        case 'Z': camera.Cz += 0.5;    break;

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
        case 'r':
        case 'R': resetCamera();    break;
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
    glutInitWindowSize(1000, 700);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Campo");
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    init();
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 1+48);
    glutMainLoop();

    return 0;
}
