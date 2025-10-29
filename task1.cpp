#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int x1_, y1_, x2_, y2_, width;
float R, G, B; // Color variables

// Draw one pixel
void draw_pixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Draw thick pixel cluster for line-width
void draw_thick_pixel(int x, int y, int w, int dx, int dy) {
    float length = sqrt(dx*dx + dy*dy);
    float px = -dy / length;
    float py = dx / length;

    for(int i = -w/2; i <= w/2; i++) {
        int xx = x + px * i;
        int yy = y + py * i;
        draw_pixel(xx, yy);
    }
}

void draw_line(int x1, int y1, int x2, int y2, int w) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(1) {
        draw_thick_pixel(x1, y1, w, x2-x1, y2-y1);

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void myInit() {
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500);
}

void myDisplay() {
    glColor3f(R, G, B); // Apply line color
    draw_line(x1_, y1_, x2_, y2_, width);
    glFlush();
}

int main(int argc, char **argv) {

    printf("Enter (x1, y1, x2, y2):\n");
    scanf("%d %d %d %d", &x1_, &y1_, &x2_, &y2_);

    printf("Enter line width: ");
    scanf("%d", &width);

    printf("Enter line color (R G B) [0 to 1]: ");
    scanf("%f %f %f", &R, &G, &B);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Bresenham (Thick Colored Line)");
    myInit();
    glutDisplayFunc(myDisplay);
    glutMainLoop();
}
