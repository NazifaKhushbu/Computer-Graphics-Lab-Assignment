#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double x0, y0, x1, y1;
} Line;

double xmin_, ymin_, xmax_, ymax_;
Line *lines = NULL;
int nlines = 0;

// Liang–Barsky
int liangBarsky(double x0, double y0, double x1, double y1,
                 double xmin, double ymin, double xmax, double ymax,
                 double *cx0, double *cy0, double *cx1, double *cy1)
{
    double dx = x1 - x0;
    double dy = y1 - y0;
    double p[4], q[4];
    p[0] = -dx;    q[0] = x0 - xmin;
    p[1] =  dx;    q[1] = xmax - x0;
    p[2] = -dy;    q[2] = y0 - ymin;
    p[3] =  dy;    q[3] = ymax - y0;

    double u1 = 0.0, u2 = 1.0;

    for (int i = 0; i < 4; i++)
    {
        if (p[i] == 0)
        {
            if (q[i] < 0) return 0;
        }
        else
        {
            double r = q[i] / p[i];
            if (p[i] < 0)
            {
                if (r > u2) return 0;
                if (r > u1) u1 = r;
            }
            else
            {
                if (r < u1) return 0;
                if (r < u2) u2 = r;
            }
        }
    }

    if (u1 > u2) return 0;

    *cx0 = x0 + u1 * dx;
    *cy0 = y0 + u1 * dy;
    *cx1 = x0 + u2 * dx;
    *cy1 = y0 + u2 * dy;
    return 1;
}

void drawClippingWindow()
{
    glColor3f(0.0f, 0.9f, 1.0f);     // neon cyan
    glLineWidth(3.0f);

    glBegin(GL_LINE_LOOP);
      glVertex2f(xmin_, ymin_);
      glVertex2f(xmax_, ymin_);
      glVertex2f(xmax_, ymax_);
      glVertex2f(xmin_, ymax_);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw original lines (RED thin)
    glLineWidth(1.5f);
    glColor3f(1.0f, 0.1f, 0.1f);
    glBegin(GL_LINES);
    for (int i = 0; i < nlines; i++)
    {
        glVertex2f(lines[i].x0, lines[i].y0);
        glVertex2f(lines[i].x1, lines[i].y1);
    }
    glEnd();

    // Draw clipping window
    drawClippingWindow();

    // Draw clipped (GREEN thick)
    glLineWidth(4.0f);
    glColor3f(0.0f, 1.0f, 0.2f);
    glBegin(GL_LINES);

    for (int i = 0; i < nlines; i++)
    {
        double cx0, cy0, cx1, cy1;
        int vis = liangBarsky(lines[i].x0, lines[i].y0, lines[i].x1, lines[i].y1,
                              xmin_, ymin_, xmax_, ymax_, &cx0, &cy0, &cx1, &cy1);
        if (vis)
        {
            glVertex2f((float)cx0, (float)cy0);
            glVertex2f((float)cx1, (float)cy1);
        }
    }
    glEnd();

    // Highlight OUTSIDE rejected segments (ORANGE)
    glLineWidth(2.5f);
    glColor3f(1.0f, 0.6f, 0.0f);
    glBegin(GL_LINES);

    for (int i = 0; i < nlines; i++)
    {
        double cx0, cy0, cx1, cy1;
        int vis = liangBarsky(lines[i].x0, lines[i].y0, lines[i].x1, lines[i].y1,
                              xmin_, ymin_, xmax_, ymax_, &cx0, &cy0, &cx1, &cy1);
        if (vis)
        {
            if (lines[i].x0 != cx0 || lines[i].y0 != cy0)
            {
                glVertex2f(lines[i].x0, lines[i].y0);
                glVertex2f(cx0, cy0);
            }
            if (lines[i].x1 != cx1 || lines[i].y1 != cy1)
            {
                glVertex2f(lines[i].x1, lines[i].y1);
                glVertex2f(cx1, cy1);
            }
        }
    }

    glEnd();

    glFlush();
}

void initOpenGL(double xmin, double ymin, double xmax, double ymax)
{
    glClearColor(0.08f, 0.08f, 0.08f, 1.0f); // darker premium BG
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double margin = 20.0;
    double left = xmin - margin, right = xmax + margin;
    double bottom = ymin - margin, top = ymax + margin;

    if (right - left < 1.0) { right = left + 1.0; }
    if (top - bottom < 1.0) { top = bottom + 1.0; }

    gluOrtho2D(left, right, bottom, top);
}

int main(int argc, char **argv)
{
    printf("==== Liang-Barsky Enhanced (Task 3) ====\n");
    printf("Enter clipping window (xmin ymin xmax ymax):\n");
    scanf("%lf %lf %lf %lf", &xmin_, &ymin_, &xmax_, &ymax_);

    if (xmin_ > xmax_) { double t = xmin_; xmin_ = xmax_; xmax_ = t; }
    if (ymin_ > ymax_) { double t = ymin_; ymin_ = ymax_; ymax_ = t; }

    printf("Enter number of lines: ");
    scanf("%d", &nlines);

    lines = (Line*) malloc(sizeof(Line) * nlines);

    for (int i = 0; i < nlines; i++)
    {
        printf("Enter line %d endpoints (x0 y0 x1 y1):\n", i+1);
        scanf("%lf %lf %lf %lf", &lines[i].x0, &lines[i].y0, &lines[i].x1, &lines[i].y1);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Liang-Barsky Enhanced");

    initOpenGL(xmin_, ymin_, xmax_, ymax_);
    glutDisplayFunc(display);

    glutMainLoop();
    free(lines);
    return 0;
}
