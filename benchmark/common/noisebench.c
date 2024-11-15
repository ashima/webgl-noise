#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// File paths for shaders
#define VERTSHADERFILE "noisebench.vert"
#define FRAGSHADERFILE_S2D "simplexnoise2D.frag"
#define FRAGSHADERFILE_S3D "simplexnoise3D.frag"
#define FRAGSHADERFILE_S4D "simplexnoise4D.frag"
#define FRAGSHADERFILE_C2D "classicnoise2D.frag"
#define FRAGSHADERFILE_C3D "classicnoise3D.frag"
#define FRAGSHADERFILE_C4D "classicnoise4D.frag"
#define FRAGSHADERFILE_CONST "constant.frag"
#define LOGFILENAME "ashimanoise.log"

GLuint displayList;
GLuint programObject;
int windowWidth = 800, windowHeight = 600;
int activeshader = 0;
FILE *logfile = nullptr;
double benchmarkStartTime = 0.0;
double benchmarkDuration = 3.0;

// Function to print errors
void printError(const char *errtype, const char *errmsg) {
    fprintf(stderr, "%s: %s\n", errtype, errmsg);
}

// Read shader file
unsigned char *readShaderFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printError("ERROR", "Cannot open shader file!");
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(fileSize + 1);
    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';

    fclose(file);
    return buffer;
}

// Create shaders
void createShader(GLuint *programObject, const char *vertexshaderfile, const char *fragmentshaderfile) {
    GLuint vertexShader, fragmentShader;
    GLint compiled, linked;
    char log[4096];

    // Vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned char *vertexSource = readShaderFile(vertexshaderfile);
    if (!vertexSource) return;
    glShaderSource(vertexShader, 1, (const char **)&vertexSource, nullptr);
    glCompileShader(vertexShader);
    free(vertexSource);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        glGetShaderInfoLog(vertexShader, sizeof(log), nullptr, log);
        printError("Vertex Shader Error", log);
        return;
    }

    // Fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned char *fragmentSource = readShaderFile(fragmentshaderfile);
    if (!fragmentSource) return;
    glShaderSource(fragmentShader, 1, (const char **)&fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    free(fragmentSource);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        glGetShaderInfoLog(fragmentShader, sizeof(log), nullptr, log);
        printError("Fragment Shader Error", log);
        return;
    }

    // Program object
    *programObject = glCreateProgram();
    glAttachShader(*programObject, vertexShader);
    glAttachShader(*programObject, fragmentShader);
    glLinkProgram(*programObject);

    glGetProgramiv(*programObject, GL_LINK_STATUS, &linked);
    if (!linked) {
        glGetProgramInfoLog(*programObject, sizeof(log), nullptr, log);
        printError("Program Linking Error", log);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Display list for rendering
void initDisplayList() {
    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
    glEnd();
    glEndList();
}

// Render scene
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programObject);
    glCallList(displayList);
    glUseProgram(0);
    glutSwapBuffers();
}

// Timer to switch shaders
void timer(int value) {
    double elapsed = glutGet(GLUT_ELAPSED_TIME) / 1000.0 - benchmarkStartTime;
    if (elapsed > benchmarkDuration) {
        activeshader++;
        benchmarkStartTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

        switch (activeshader) {
            case 1: createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_S2D); break;
            case 2: createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_S3D); break;
            case 3: createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_S4D); break;
            case 4: createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_C2D); break;
            case 5: createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_C3D); break;
            case 6: createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_C4D); break;
            default: exit(0);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// Initialize OpenGL settings
void initOpenGL() {
    glewInit();
    glEnable(GL_DEPTH_TEST);
    initDisplayList();
    createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_CONST);
}

// Window resize handler
void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Main function
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("GLSL Noise Benchmark");

    initOpenGL();

    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
