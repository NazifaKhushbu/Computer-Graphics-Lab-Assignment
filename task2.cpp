#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.1415926535898

// Function to draw a circle with thickness and color
void drawCircle(float cx, float cy, float radius, float thickness, float r, float g, float b)
{
    int segments = 200 + radius * 200;   // more smooth when bigger
    float angle;

    glLineWidth(thickness);
    glColor3f(r, g, b);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++)
    {
        angle = 2 * PI * i / segments;
        float x = cx + cos(angle) * radius;
        float y = cy + sin(angle) * radius;
        glVertex2f(x, y);
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    float cx = 0.0f, cy = 0.0f;

    int numCircles = 20;
    float maxRadius = 0.95f;
    float minRadius = 0.08f;
    float radiusStep = (maxRadius - minRadius) / numCircles;

    float baseThickness = 1.6f;

    for (int i = 0; i < numCircles; i++)
    {
        float radius = minRadius + i * radiusStep;
        float thickness = baseThickness + i * 0.35f;

        // NEW rainbow-style gradient
        float t = (float)i / numCircles;

        float r = sin(2 * PI * t) * 0.5f + 0.5f;
        float g = sin(2 * PI * (t + 0.33f)) * 0.5f + 0.5f;
        float b = sin(2 * PI * (t + 0.66f)) * 0.5f + 0.5f;

        drawCircle(cx, cy, radius, thickness, r, g, b);
    }

    glFlush();
}

void init()
{
    glClearColor(0.02, 0.02, 0.05, 1.0); // Deeper dark background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Concentric Rainbow Circles");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
