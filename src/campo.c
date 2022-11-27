#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define DIAMETRO_BOLA 0.5f
#define TRANSLACAO_BOLA 0.2f

#define CX_INICIAL -34.0f
#define CY_INICIAL 19.5f
#define CZ_INICIAL 0.5f

#define Z_MAX_CAMPO 5.5f
#define Z_MIN_CAMPO -5.5f

#define X_MAX_CAMPO 15.5f

#define CYLINDER_RADIUS 0.1
#define CYLINDER_HEIGHT 10
#define CYLINDER_SECTORS 21

#define LARGURA_ARQUIBANCADA 30
#define COMPRIMENTO_ARQUIBANCADA 0.5
#define ALTURA_ARQUIBANCADA 0.5
#define DISTANCIA_ARQUIBANCADA_POS 15
#define DISTANCIA_ARQUIBANCADA_NEG -DISTANCIA_ARQUIBANCADA_POS

enum HORAS {
    MADRUGADA,
    DIA,
    TARDE,
    NOITE
};

GLfloat dimGray[] = {0.412f, 0.412f, 0.412f};

GLfloat ROTACAO_BOLA = 0.0f;

GLfloat ang = 0.000000f;
int horario = 0;

bool noite = false;


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

void load_texture(const char *path, int index)
{
	unsigned int textureID;

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    	glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);
        

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  		
  		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
	} else {
		printf("Existe essa textura n�o me ajuda ai marcelo na moral");
	}
}

void apply_texture(int index)
{
   glBindTexture(GL_TEXTURE_2D, index);
}

void print_vertex(GLfloat vertex[]) {
    printf("Point: (%f, %f, %f)\n", vertex[0], vertex[1], vertex[2]);
}

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

void traves(GLfloat V0[], GLfloat V1[])
{
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

void desenha_bola()
{
    GLfloat mat_ambient[] = {0.05f,0.05f,0.05f,1.0f };
    GLfloat mat_diffuse[] = {0.5f,0.5f,0.5f,1.0f};
    GLfloat mat_specular[] = { 0.7f,0.7f,0.7f,1.0f};
    GLfloat shininess = 100.0f;
    glPushMatrix();
        glColor3f(bola.r, bola.g, bola.b);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glTranslatef(bola.transX, 0.5, bola.transZ);
        glScalef(DIAMETRO_BOLA, DIAMETRO_BOLA, DIAMETRO_BOLA);
        glRotatef(ROTACAO_BOLA, bola.rotX, bola.rotY, bola.rotZ);
        glutSolidSphere(0.5f, 30, 30);
    glPopMatrix();
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
            decision += 2 * 2 * x + 0.03;
        }
        else {
            y -= inc;
            x += inc;
            decision += 2 * (x - y) + 0.05;
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

void placaX(GLfloat V0[], GLfloat V1[], GLfloat somaX)
{
    glBegin(GL_QUAD_STRIP);
        glVertex3f(V0[0]+somaX, V0[1], V0[2]-0.15);
        glVertex3f(V0[0]+somaX, V0[1]+2, V0[2]-0.15);
        glVertex3f(V1[0]+somaX, V1[1], V1[2]+0.15);
        glVertex3f(V1[0]+somaX, V1[1]+2, V1[2]+0.15);
    glEnd();
}

void placaZ(GLfloat V0[], GLfloat V1[], GLfloat somaZ)
{
    glBegin(GL_QUAD_STRIP);
	    glVertex3f(V0[0]-0.05, V0[1], V0[2]+somaZ);        
		glVertex3f(V0[0]-0.05, V0[1]+2, V0[2]+somaZ); //v4
		glVertex3f(V1[0]+0.05, V1[1], V1[2]+somaZ);
		glVertex3f(V1[0]+0.05, V1[1]+2, V1[2]+somaZ);
    glEnd();
}

void placas(GLfloat V0[], GLfloat V1[], GLfloat V4[], GLfloat V5[])
{
    glColor3f(1.0f, 0.0f, 0.0f);
    placaZ(V4, V5, -0.15f);
    placaZ(V0, V1, 0.15);
    placaX(V4, V0, -0.05);
    placaX(V5, V1, 0.05);
}

void print_arr(GLfloat arr[], int size) {
    int i = 0;
    for (i = 0; i < size; i++) {
        printf("%f ", arr[i]);
    }
    printf("\n");
}

void setBackground()
{
    switch (horario)
    {
    case DIA:
        glClearColor(0.529f, 0.808f, 0.922f, 1); 
        break;
    case MADRUGADA:
    case TARDE:
        glClearColor(0.957f, 0.643f, 0.376f, 1); 

        break;
    case NOITE:
        glClearColor(0,0,0,1);
        break;
    default:
        break;
    }
}


void timer()
{
    horario += 1;
    if (horario > 3) {
        horario = 0;
    }

    switch (horario)
    {
    case MADRUGADA:
        noite = true;
        break;
    case DIA:
        noite = false;
        break;
    case TARDE:
        noite = false;
        break;
    case NOITE:
        noite = true;
    default:
        break;
    }

    setBackground();
}

void refletor(GLfloat x, GLfloat z, GLfloat mult, GLfloat direction[], GLenum light)
{
    Point pos = {x-(mult * 0.45), 0.29, z};

    GLfloat ambient[] = {0.105882f, 0.058824f, 0.113725f, 1.0f};
    GLfloat diffuse[] = {0.427451f, 0.470588f, 0.541176f, 1.0f };
    GLfloat specular[] = {0.333333f, 0.333333f, 0.521569f, 1.0f };
    GLfloat shininess = 9.84615f;


    glPushMatrix();
        glColor3fv(dimGray);
        glTranslatef(x, 0.29, z);
        glRotatef(-90, 1, 0, 0); //mantem erecto
        glutSolidCylinder(CYLINDER_RADIUS, CYLINDER_HEIGHT, 30, 30);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.427451f, 0.470588f, 0.541176f);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
        glTranslatef(pos.x, CYLINDER_HEIGHT, pos.z);
        glRotatef(30*mult, 0, 0, 1);
        glRotatef(90*mult, 0, 1, 0);
        glutSolidCone(1.0f, 2.0f, 30, 30);
    glPopMatrix();
    
    glPushMatrix();
        
        glTranslatef(pos.x, CYLINDER_HEIGHT, pos.z);
        glutSolidSphere(0.8, 30, 30);
    glPopMatrix();

    GLfloat light1_ambient[] = {0.16, 0.16, 0.16, 1};
    GLfloat light1_diffuse[] = {1, 1, 1, 1};
    GLfloat light_specular[] = {0, 0, 0, 1};

    GLfloat light1_position[] = {pos.x, pos.y, pos.z, 1};
    GLfloat light1_spot_direction[] = {direction[0], direction[1], direction[2]};

    glLightfv(light, GL_POSITION, light1_position);
    glLightfv(light, GL_AMBIENT, light1_ambient);
    glLightfv(light, GL_DIFFUSE, light1_diffuse);
    glLightfv(light, GL_SPECULAR, light_specular);


    if (noite) {
        glEnable(light);
    } else {
        glDisable(light);
    }
}

void Cube (
    GLfloat V0[], GLfloat V1[], GLfloat V2[], GLfloat V3[], 
    GLfloat V4[],GLfloat V5[],GLfloat V6[], GLfloat V7[]
)
{
    Square(V0, V1, V2, V3); //front
    Square(V4, V5, V6, V7); //back
    Square(V0, V4, V7, V3); //left
    Square(V1, V5, V6, V2); //right
    Square(V3, V2, V6, V7); //bottom
    Square(V0, V1, V5, V4); //top
}

void desenha_campo(GLfloat V[][3]) 
{
    glPushMatrix();
    	glEnable(GL_TEXTURE_2D);
        glColor3f(0.133f, 0.545f, 0.133f);
        glScalef(30, 0.5, 10);
        Cube(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7]);
        glDisable(GL_TEXTURE_2D);
		fieldLines(V[0], V[1], V[5], V[4]);
        placas(V[0], V[1], V[4], V[5]);

    glPopMatrix();
}

void drawTexturedBox(GLfloat size, GLenum type, char* file)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    load_texture(file, 2);
    glEnable(GL_TEXTURE_2D);

  for (i = 5; i >= 0; i--) {
    glBegin(type);
    glNormal3fv(&n[i][0]);
        glTexCoord2f(0.0, 0.0);
    glVertex3fv(&v[faces[i][0]][0]);
        glTexCoord2f(1.0, 0.0);
    glVertex3fv(&v[faces[i][1]][0]);
        glTexCoord2f(0.0, 1.0);
    glVertex3fv(&v[faces[i][2]][0]);
        glTexCoord2f(1.0, 1.0);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
  	glDisable(GL_TEXTURE_2D);

}


void desenha_arquibancadas(int numero_de_bancos, GLfloat V[], bool negativo) 
{
    int i;
    GLfloat incremento_y = 0;
    GLfloat incremento_z = negativo ? DISTANCIA_ARQUIBANCADA_NEG : DISTANCIA_ARQUIBANCADA_POS;
    
    glColor3fv(dimGray);
    
    for (i = 0; i < numero_de_bancos; i++) {
        glPushMatrix();
	        glScalef(LARGURA_ARQUIBANCADA, ALTURA_ARQUIBANCADA, COMPRIMENTO_ARQUIBANCADA);
	        glTranslatef(0, V[1]+incremento_y, V[2]+incremento_z);
	        drawTexturedBox(1, GL_QUADS, "arquibancada.jpg");
            // SolidCube(1);
        glPopMatrix();
        incremento_z = negativo ? incremento_z - 0.5 : incremento_z + 0.5;
        incremento_y += ALTURA_ARQUIBANCADA;
    }
    	glPopMatrix();
}

void desenha_entornos_do_campo(GLfloat V[][3])
{
// por baixo e ao redor do campo
    glPushMatrix();
        glColor3f(0.000f, 0.392f, 0.000f);
        glScalef(60, 0.4, 30);
        Cube(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7]);
    glPopMatrix();
    
    desenha_arquibancadas(20, V[4], true);
    desenha_arquibancadas(20, V[0], false);
}

void init_lights()
{
    GLfloat light0_position[] = {0, 3000, 0, 1}; 
    GLfloat light0_ambient_day[] = {0.7, 0.7, 0.7, 1};
    GLfloat light0_ambient_night[] = {0, 0, 0, 1};
        
    GLfloat light0_diffuse[] = {1, 1, 1, 1};

    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient_day);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.1999f);
    
    // glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, ang);
    // glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, ang);
    
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    
    if (noite) glDisable(GL_LIGHT0);
    else glEnable(GL_LIGHT0);
    
    glEnable(GL_DEPTH_TEST);

}


void display()
{ 
    // printf("Ang: %f\n", ang);
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


    init_lights();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // printf("GOAL POS: %f\n", -(V[1][1]+15));
    glLoadIdentity();
    gluLookAt(
        camera.Cx, camera.Cy, camera.Cz, 
        camera.atX, camera.atY, camera.atZ,
        0, 1, 0
    );


    desenha_entornos_do_campo(V);
    
    load_texture("grass.jpg", 0);
    glEnable(GL_TEXTURE_2D);
  		glBegin(GL_QUADS);
  		
		  glTexCoord2f(0.0, 10.0); 
		  glVertex3f(-15, 0.25, -5);
		  glTexCoord2f(20.0, 0.0); 
		  glVertex3f(-15, 0.25, 5);
		  glTexCoord2f(0.0, 0.0); 
		  glVertex3f(15, 0.25, 5);
		  glTexCoord2f(20.0, 10.0); 
		  glVertex3f(15, 0.25, -5);

  		glEnd();
  	glDisable(GL_TEXTURE_2D);
  	
  	glPushMatrix();
		glScalef(30, 0.5, 10);
		fieldLines(V[0], V[1], V[5], V[4]);
		placas(V[0], V[1], V[4], V[5]);
    glPopMatrix();
	desenha_bola();
    
    glPushMatrix();
        glTranslatef(0, 0.29, 0);
        glRotatef(90, 1, 0, 0);
        Bresenham3DCircle(point(0,0,0), 1.5);
    glPopMatrix();
    // DrawCircle(0, 0, 1.5, 18);
    GLfloat dir_light1[] = {X_MAX_CAMPO-3, 0.29, Z_MAX_CAMPO-2};
    GLfloat dir_light2[] = {-X_MAX_CAMPO+3, 0.29, Z_MAX_CAMPO-2};
    GLfloat dir_light3[] = {X_MAX_CAMPO-3, 0.29, -Z_MAX_CAMPO+2};
    GLfloat dir_light4[] = {-X_MAX_CAMPO+3, 0.29, -Z_MAX_CAMPO+2};
    
    
    refletor(23.0f, 6.0f, 1, dir_light1, GL_LIGHT1);
    refletor(-23.0f, 6.0f, -1, dir_light2, GL_LIGHT2);
    refletor(23.0f, -6.0f, 1, dir_light3, GL_LIGHT3);
    refletor(-23.0f, -6.0f, -1, dir_light4, GL_LIGHT4);
    
    // glPopMatrix();
    
    glutSwapBuffers();
}

void init()
{
    glClearColor(0.529f, 0.808f, 0.922f, 1); 
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
        case 'n': ang += 0.01f;      break;
        case 'm': ang -= 0.01f;      break;
        case ' ':
            timer();
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
    glutInitWindowSize(1000, 700);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Campo");
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    // glutTimerFunc(33, timer, 1);
    init();
    init_lights();
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 1+48);
    glutMainLoop();
    
    return 0;
}