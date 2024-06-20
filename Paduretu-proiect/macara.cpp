#include <GL/glut.h>

// Variabile pentru transformări
GLfloat angleHorizontalArm = 0.0f;
GLfloat hookVerticalPosition = 0.8f;

// Variabile pentru animație
GLfloat angleHorizontalArmDelta = 1.0f;
GLfloat hookVerticalPositionDelta = 0.01f;

// Variabile pentru decupare
GLdouble clipPlane0[] = { 0.0, -1.0, 0.0, 0.0 }; // Planul de decupare

void setMaterial(GLfloat r, GLfloat g, GLfloat b) {
    GLfloat ambient[] = { r * 0.2f, g * 0.2f, b * 0.2f, 1.0f };
    GLfloat diffuse[] = { r, g, b, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void drawBase() {
    glPushMatrix();
    glScalef(1.0f, 0.1f, 1.0f);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawVerticalArm() {
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f);
    glScalef(0.1f, 1.0f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawHorizontalArm() {
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glRotatef(angleHorizontalArm, 0.0f, 1.0f, 0.0f);
    glScalef(1.0f, 0.1f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawHook() {
    glPushMatrix();
    glTranslatef(0.0f, hookVerticalPosition, 0.0f);
    glScalef(0.1f, 0.2f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawCube(float size) {
    glPushMatrix();
    glScalef(size, size, size);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawPrimitive() {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();
}

void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

void shadowMatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4]) {
    GLfloat dot = groundplane[0] * lightpos[0] +
        groundplane[1] * lightpos[1] +
        groundplane[2] * lightpos[2] +
        groundplane[3] * lightpos[3];

    shadowMat[0][0] = dot - lightpos[0] * groundplane[0];
    shadowMat[1][0] = 0.0f - lightpos[0] * groundplane[1];
    shadowMat[2][0] = 0.0f - lightpos[0] * groundplane[2];
    shadowMat[3][0] = 0.0f - lightpos[0] * groundplane[3];

    shadowMat[0][1] = 0.0f - lightpos[1] * groundplane[0];
    shadowMat[1][1] = dot - lightpos[1] * groundplane[1];
    shadowMat[2][1] = 0.0f - lightpos[1] * groundplane[2];
    shadowMat[3][1] = 0.0f - lightpos[1] * groundplane[3];

    shadowMat[0][2] = 0.0f - lightpos[2] * groundplane[0];
    shadowMat[1][2] = 0.0f - lightpos[2] * groundplane[1];
    shadowMat[2][2] = dot - lightpos[2] * groundplane[2];
    shadowMat[3][2] = 0.0f - lightpos[2] * groundplane[3];

    shadowMat[0][3] = 0.0f - lightpos[3] * groundplane[0];
    shadowMat[1][3] = 0.0f - lightpos[3] * groundplane[1];
    shadowMat[2][3] = 0.0f - lightpos[3] * groundplane[2];
    shadowMat[3][3] = dot - lightpos[3] * groundplane[3];
}

void drawShadow() {
    GLfloat shadowMat[4][4];
    GLfloat lightPosition[] = { 1.0, 1.0, 1.0, 0.0 }; // Poziția sursei de lumină
    GLfloat groundPlane[] = { 0.0, 1.0, 0.0, 0.0 };  // Planul de sol pe care se proiectează umbra

    shadowMatrix(shadowMat, groundPlane, lightPosition);

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glMultMatrixf((const GLfloat*)shadowMat);

    glColor3f(0.0f, 0.0f, 0.0f); // Culoarea umbrelor

    // Desenăm obiectele cu proiecția umbrei
    drawBase();
    drawVerticalArm();
    drawHorizontalArm();
    drawHook();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}



void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 1.5f, 3.0, 0.0, 0.5f, 0.0, 0.0, 1.0, 0.0);

    // Iluminare
    GLfloat lightPosition[] = { 1.0, 1.0, 1.0, 0.0 }; // Poziția sursei de lumină
    GLfloat lightColor[] = { 1.0, 1.0, 1.0, 1.0 };    // Culoarea sursei de lumină
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

    // Desenăm umbrele
    drawShadow();

    // Draw base
    setMaterial(0.0f, 0.0f, 1.0f);
    drawBase();

    // Draw vertical arm
    setMaterial(1.0f, 0.0f, 0.0f);
    drawVerticalArm();

    // Draw horizontal arm
    setMaterial(0.0f, 1.0f, 0.0f);
    drawHorizontalArm();

    // Draw hook
    setMaterial(1.0f, 1.0f, 0.0f);
    drawHook();

    // Plan de decupare
    glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, clipPlane0);

    // Draw first cube (partial with clipping plane)
    setMaterial(1.0f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(-0.5f, 0.5f, -0.5f);
    drawCube(0.3f);
    glPopMatrix();

    // Disable clipping plane for the rest of the scene
    glDisable(GL_CLIP_PLANE0);

    // Draw second cube (full)
    setMaterial(0.0f, 0.5f, 1.0f);
    glPushMatrix();
    glTranslatef(0.5f, 0.5f, 0.5f);
    drawCube(0.3f);
    glPopMatrix();

    // Draw a primitive
    glColor3f(1.0f, 0.0f, 0.0f);
    drawPrimitive();

    // Draw text
    glDisable(GL_LIGHTING); // Dezactivăm iluminarea pentru text
    glColor3f(1.0f, 1.0f, 1.0f); // Setăm culoarea textului
    drawText("Paduretu Mirel", -0.9f, 0.9f);
    glEnable(GL_LIGHTING); // Re-activăm iluminarea

    glutSwapBuffers();
}


void timer(int value) {
    // Update the angle of the horizontal arm
    angleHorizontalArm += angleHorizontalArmDelta;
    if (angleHorizontalArm > 360.0f) angleHorizontalArm -= 360.0f;
    if (angleHorizontalArm < 0.0f) angleHorizontalArm += 360.0f;

    // Update the vertical position of the hook
    hookVerticalPosition += hookVerticalPositionDelta;
    if (hookVerticalPosition > 1.0f || hookVerticalPosition < 0.2f) {
        hookVerticalPositionDelta = -hookVerticalPositionDelta;
    }

    // Redisplay the scene
    glutPostRedisplay();

    // Register the timer function again
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        angleHorizontalArm += 5.0f;
        if (angleHorizontalArm > 360.0f) angleHorizontalArm -= 360.0f;
        break;
    case 'd':
        angleHorizontalArm -= 5.0f;
        if (angleHorizontalArm < 0.0f) angleHorizontalArm += 360.0f;
        break;
    case 'w':
        hookVerticalPosition += 0.05f;
        if (hookVerticalPosition > 1.0f) hookVerticalPosition = 1.0f;
        break;
    case 's':
        hookVerticalPosition -= 0.05f;
        if (hookVerticalPosition < 0.2f) hookVerticalPosition = 0.2f;
        break;
    case 27:  // Escape key
        exit(0);
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Macara in OpenGL");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0); // ~60 FPS
    glutMainLoop();
    return 0;
}