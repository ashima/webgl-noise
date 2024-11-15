#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#ifdef __APPLE__
// MacOS application bundles have the executable inside a directory structure
#define VERTSHADERFILE "../../../noisedemo.vert"
#define FRAGSHADERFILE "../../../noisedemo.frag"
#else
// Windows, Linux and other Unix systems expose executables as naked files
#define VERTSHADERFILE "noisedemo.vert"
#define FRAGSHADERFILE "noisedemo.frag"
#endif

GLuint displayList;
GLuint programObject;

// Print error messages
void printError(const char *errtype, const char *errmsg) {
    fprintf(stderr, "%s: %s\n", errtype, errmsg);
}

// Read the shader file
unsigned char* readShaderFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printError("ERROR", "Cannot open shader file!");
        return 0;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    unsigned char *buffer = (unsigned char *)malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

// Create and compile shaders
void createShader(GLuint *programObject, char *vertexshaderfile, char *fragmentshaderfile) {
    GLuint vertexShader, fragmentShader;
    const char *vertexShaderStrings[1];
    const char *fragmentShaderStrings[1];
    GLint vertexCompiled, fragmentCompiled, shadersLinked;
    char str[4096];

    // Vertex Shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned char *vertexShaderSource = readShaderFile(vertexshaderfile);
    vertexShaderStrings[0] = (char *)vertexShaderSource;
    glShaderSource(vertexShader, 1, vertexShaderStrings, NULL);
    glCompileShader(vertexShader);
    free(vertexShaderSource);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);
    if (!vertexCompiled) {
        glGetShaderInfoLog(vertexShader, sizeof(str), NULL, str);
        printError("Vertex shader compile error", str);
    }

    // Fragment Shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned char *fragmentShaderSource = readShaderFile(fragmentshaderfile);
    fragmentShaderStrings[0] = (char *)fragmentShaderSource;
    glShaderSource(fragmentShader, 1, fragmentShaderStrings, NULL);
    glCompileShader(fragmentShader);
    free(fragmentShaderSource);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompiled);
    if (!fragmentCompiled) {
        glGetShaderInfoLog(fragmentShader, sizeof(str), NULL, str);
        printError("Fragment shader compile error", str);
    }

    // Program Object
    *programObject = glCreateProgram();
    glAttachShader(*programObject, vertexShader);
    glAttachShader(*programObject, fragmentShader);
    glLinkProgram(*programObject);
    glGetProgramiv(*programObject, GL_LINK_STATUS, &shadersLinked);
    if (!shadersLinked) {
        glGetProgramInfoLog(*programObject, sizeof(str), NULL, str);
        printError("Program linking error", str);
    }
}

// Display list for rendering
void initDisplayList() {
    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidSphere(1.0, 20, 20); // Draw a textured sphere
    glEndList();
}

// Render function
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    glUseProgram(programObject);

    GLint location_time = glGetUniformLocation(programObject, "time");
    if (location_time != -1) {
        float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
        glUniform1f(location_time, time);
    }

    glCallList(displayList);
    glUseProgram(0);

    glutSwapBuffers();
}

// Reshape function
void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);
}

// Initialization
void init() {
    glewInit();
    if (!GLEW_ARB_shading_language_100) {
        printError("ERROR", "GLSL not supported");
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);
    initDisplayList();
    createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE);
}

// Main function
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("GLSL Noise Demo");

    init();

    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutIdleFunc(renderScene);

    glutMainLoop();
    return 0;
}
