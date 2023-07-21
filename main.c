#include <GL/glut.h>
#include <sys/time.h>
#include <math.h>

#define HEIGHT 800
#define WIDTH 1421

// scaling and positioning constants
#define SIZE HEIGHT / 7
#define CX WIDTH / 2
#define CY HEIGHT / 2
#define ITER 13

// circle-related constants
#define RADIUS SIZE * 1.25
#define TAU 6.28318530718

// these are just coloring constants
#define R 0.27058
#define G 0.12156
#define B 0.31764

void drawCircle() {
    // draw the clock circle
    float x, y;
    glColor3f(0.2, 0.2, 0.2);
    glLineWidth(1.5);
    glBegin(GL_LINE_LOOP);
    for (float t = 0; t < TAU; t += 0.15) {
        x = RADIUS * cos(t);
        y = RADIUS * sin(t);
        glVertex2d(x, y);
    }
    glEnd();
}

void fractal(float l, float theta, float theta1, int i) {
    float nl = l / 1.4f;

    // set the color
    float b = i / (ITER * 2.0f);
    if (i > 0) {
        // draw the left branch
        glPushMatrix();
        glRotatef(theta, 0, 0, 1);

        glBegin(GL_LINES);
        glVertex2d(0, 0);
        glVertex2d(0, -l);
        glEnd();

        glTranslatef(0, -l, 0);
        fractal(nl, theta, theta1, i - 1);
        glPopMatrix();

        // and the right branch
        glPushMatrix();
        glRotatef(theta1, 0, 0, 1);

        glBegin(GL_LINES);
        glVertex2d(0, 0);
        glVertex2d(0, -l);
        glEnd();

        glTranslatef(0, -l, 0);
        fractal(nl, theta, theta1, i-1);
        glPopMatrix();
    }
    glColor3f(R+b, G+b, B+b);
}

void loop(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // calculate times and angles
    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    struct tm *tm_struct = localtime((const time_t*)&curTime);

    int minutes = tm_struct->tm_min;
    int seconds = tm_struct->tm_sec;
    float csecs = (seconds + (curTime.tv_usec / 1000000.0f)) + (minutes * 60.0f);
    float minang = (csecs / 60.0f) * 6;
    float secang =  csecs * 6;

    // draw the fractal and the clock circle
    glPushMatrix();
    glTranslatef(CX, CY, 1);
    drawCircle();
    glColor3f(R+0.5, G+0.5, B+0.5);

    glLineWidth(2);
    fractal(SIZE, minang, secang, ITER);
    glPopMatrix();

    glFlush();
}

void idle() { 
    glutPostRedisplay();
}

int main (int argc, char** argv) {
    glMatrixMode(GL_MODELVIEW);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Fractal Clock");

    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    glClearColor(0.1, 0.1, 0.1, 1.0);

    glutDisplayFunc(loop);
    glutIdleFunc(&idle);
    glutMainLoop();
}