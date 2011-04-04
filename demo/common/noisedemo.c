/* 
 * Demo for GLSL procedural noise.
 *
 * The shaders are loaded from external files, named in
 * the macro definitions VERTSHADERFILE and FRAGSHADERFILE.
 * The main program draws a sphere covering most of the
 * viewport, activates the demo shader and runs indefinitely,
 * reporting the fragment shading performance in Msamples/s
 * while it executes.
 *
 * This program uses GLFW for convenience, to handle the OS-specific
 * window management stuff. Some Windows-specific stuff for extension
 * loading is still here, but that code is short-circuited on other
 * platforms - this file compiles unedited on Windows, Linux and MacOS X,
 * provided you have the relevant libraries and header files installed
 * and set up your compilation to include the GLFW and OpenGL libraries.
 *
 * Author: Stefan Gustavson (stegu@itn.liu.se) 2004, 2005, 2010, 2011
 * This code is in the public domain.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glfw.h>

#ifdef __WIN32__
// The system level include file for GL extensions might not be up to date.
#include "GL/glext.h"
#else
#include <GL/glext.h>
#endif


#ifdef __APPLE__
// MacOS application bundles have the executable inside a directory structure
#define VERTSHADERFILE "../../../noisedemo.vert"
#define FRAGSHADERFILE "../../../noisedemo.frag"
#else
// Windows, Linux and other Unix systems expose executables as naked files
#define VERTSHADERFILE "noisedemo.vert"
#define FRAGSHADERFILE "noisedemo.frag"
#endif

#ifdef __WIN32__
/* Global function pointers for everything we need beyond OpenGL 1.1 */
PFNGLCREATEPROGRAMPROC           glCreateProgram      = NULL;
PFNGLDELETEPROGRAMPROC           glDeleteProgram      = NULL;
PFNGLUSEPROGRAMPROC              glUseProgram         = NULL;
PFNGLCREATESHADERPROC            glCreateShader       = NULL;
PFNGLDELETESHADERPROC            glDeleteShader       = NULL;
PFNGLSHADERSOURCEPROC            glShaderSource       = NULL;
PFNGLCOMPILESHADERPROC           glCompileShader      = NULL;
PFNGLGETSHADERIVPROC             glGetShaderiv        = NULL;
PFNGLGETPROGRAMIVPROC            glGetProgramiv       = NULL;
PFNGLATTACHSHADERPROC            glAttachShader       = NULL;
PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog   = NULL;
PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog  = NULL;
PFNGLLINKPROGRAMPROC             glLinkProgram        = NULL;
PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation = NULL;
PFNGLUNIFORM1FVPROC              glUniform1fv         = NULL;
#endif

/*
 * printError() - Signal an error.
 * Simple printf() to console for portability.
 */
void printError(const char *errtype, const char *errmsg) {
  fprintf(stderr, "%s: %s\n", errtype, errmsg);
}


/*
 * Override the Win32 filelength() function with
 * a version that takes a Unix-style file handle as
 * input instead of a file ID number, and which works
 * on platforms other than Windows.
 */
long filelength(FILE *file) {
    long numbytes;
    long savedpos = ftell(file);
    fseek(file, 0, SEEK_END);
    numbytes = ftell(file);
    fseek(file, savedpos, SEEK_SET);
    return numbytes;
}


/*
 * loadExtensions() - Load OpenGL extensions for anything above OpenGL
 * version 1.1. (This is a requirement only on Windows, so on other
 * platforms, this function just checks for the required extensions.)
 */
void loadExtensions() {
    //These extension strings indicate that the OpenGL Shading Language
    // and GLSL shader objects are supported.
    if(!glfwExtensionSupported("GL_ARB_shading_language_100"))
    {
        printError("GL init error", "GL_ARB_shading_language_100 extension was not found");
        return;
    }
    if(!glfwExtensionSupported("GL_ARB_shader_objects"))
    {
        printError("GL init error", "GL_ARB_shader_objects extension was not found");
        return;
    }
    else
    {
#ifdef __WIN32__
        glCreateProgram           = (PFNGLCREATEPROGRAMPROC)glfwGetProcAddress("glCreateProgram");
        glDeleteProgram           = (PFNGLDELETEPROGRAMPROC)glfwGetProcAddress("glDeleteProgram");
        glUseProgram              = (PFNGLUSEPROGRAMPROC)glfwGetProcAddress("glUseProgram");
        glCreateShader            = (PFNGLCREATESHADERPROC)glfwGetProcAddress("glCreateShader");
        glDeleteShader            = (PFNGLDELETESHADERPROC)glfwGetProcAddress("glDeleteShader");
        glShaderSource            = (PFNGLSHADERSOURCEPROC)glfwGetProcAddress("glShaderSource");
        glCompileShader           = (PFNGLCOMPILESHADERPROC)glfwGetProcAddress("glCompileShader");
        glGetShaderiv             = (PFNGLGETSHADERIVPROC)glfwGetProcAddress("glGetShaderiv");
        glGetShaderInfoLog        = (PFNGLGETSHADERINFOLOGPROC)glfwGetProcAddress("glGetShaderInfoLog");
        glAttachShader            = (PFNGLATTACHSHADERPROC)glfwGetProcAddress("glAttachShader");
        glLinkProgram             = (PFNGLLINKPROGRAMPROC)glfwGetProcAddress("glLinkProgram");
        glGetProgramiv            = (PFNGLGETPROGRAMIVPROC)glfwGetProcAddress("glGetProgramiv");
        glGetProgramInfoLog       = (PFNGLGETPROGRAMINFOLOGPROC)glfwGetProcAddress("glGetProgramInfoLog");
        glGetUniformLocation      = (PFNGLGETUNIFORMLOCATIONPROC)glfwGetProcAddress("glGetUniformLocation");
        glUniform1fv              = (PFNGLUNIFORM1FVPROC)glfwGetProcAddress("glUniform1fv");

        if( !glCreateProgram || !glDeleteProgram || !glUseProgram ||
            !glCreateShader || !glDeleteShader || !glShaderSource || !glCompileShader || 
            !glGetShaderiv || !glGetShaderInfoLog || !glAttachShader || !glLinkProgram ||
            !glGetProgramiv || !glGetProgramInfoLog || !glGetUniformLocation ||
            !glUniform1fv )
        {
            printError("GL init error", "One or more required OpenGL functions were not found");
            return;
        }
#endif
    }
}


/*
 * readShaderFile(filename) - read a shader source string from a file
 */
unsigned char* readShaderFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if(file == NULL)
    {
        printError("ERROR", "Cannot open shader file!");
  		  return 0;
    }
    int bytesinfile = filelength(file);
    unsigned char *buffer = (unsigned char*)malloc(bytesinfile+1);
    int bytesread = fread( buffer, 1, bytesinfile, file);
    buffer[bytesread] = 0; // Terminate the string with 0
    fclose(file);
    
    return buffer;
}


/*
 * createShaders() - create, load, compile and link the GLSL shader objects.
 */
void createShader(GLuint *programObject, char *vertexshaderfile, char *fragmentshaderfile) {
     GLuint vertexShader;
     GLuint fragmentShader;

     const char *vertexShaderStrings[1];
     const char *fragmentShaderStrings[1];
     GLint vertexCompiled;
     GLint fragmentCompiled;
     GLint shadersLinked;
     char str[4096]; // For error messages from the GLSL compiler and linker

    // Create the vertex shader.
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    unsigned char *vertexShaderAssembly = readShaderFile( vertexshaderfile );
    vertexShaderStrings[0] = (char*)vertexShaderAssembly;
    glShaderSource( vertexShader, 1, vertexShaderStrings, NULL );
    glCompileShader( vertexShader);
    free((void *)vertexShaderAssembly);

    glGetShaderiv( vertexShader, GL_COMPILE_STATUS,
                               &vertexCompiled );
    if(vertexCompiled  == GL_FALSE)
  	{
        glGetShaderInfoLog(vertexShader, sizeof(str), NULL, str);
        printError("Vertex shader compile error", str);
  	}

  	// Create the fragment shader.
    fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

    unsigned char *fragmentShaderAssembly = readShaderFile( fragmentshaderfile );
    fragmentShaderStrings[0] = (char*)fragmentShaderAssembly;
    glShaderSource( fragmentShader, 1, fragmentShaderStrings, NULL );
    glCompileShader( fragmentShader );
    free((void *)fragmentShaderAssembly);

    glGetProgramiv( fragmentShader, GL_COMPILE_STATUS, 
                               &fragmentCompiled );
    if(fragmentCompiled == GL_FALSE)
   	{
        glGetShaderInfoLog( fragmentShader, sizeof(str), NULL, str );
        printError("Fragment shader compile error", str);
    }

    // Create a program object and attach the two compiled shaders.
    *programObject = glCreateProgram();
    glAttachShader( *programObject, vertexShader );
    glAttachShader( *programObject, fragmentShader );

    // Link the program object and print out the info log.
    glLinkProgram( *programObject );
    glGetProgramiv( *programObject, GL_LINK_STATUS, &shadersLinked );

    if( shadersLinked == GL_FALSE )
	{
		glGetProgramInfoLog( *programObject, sizeof(str), NULL, str );
		printError("Program object linking error", str);
	}
}


/*
 * computeFPS() - Calculate, display and return samples per second.
 * Stats are recomputed only once per second.
 */
double computeFPS() {

    static double t0 = 0.0;
    static int frames = 0;
    static double fps = 0.0;
    static char titlestring[200];

    double t;
    int width, height;
    
    // Get current time
    t = glfwGetTime();  // Gets number of seconds since glfwInit()
    // If one second has passed, or if this is the very first frame
    if( (t-t0) > 1.0 || frames == 0 )
    {
        fps = (double)frames / (t-t0);
        sprintf(titlestring, "GLSL noise demo (%.1f FPS)", fps);
        glfwSetWindowTitle(titlestring);
        t0 = t;
        frames = 0;
    }
    frames ++;
    return fps;
}


/*
 * drawTexturedSphere(r, segs) - Draw a sphere centered on the local
 * origin, with radius r and approximated by segs polygon segments,
 * having texture coordinates with a latitude-longitude mapping.
 * Yes, this is ugly old school immediate mode, but this is being
 * baked to a display list anyway, and that ends up being sent
 * to the GPU as a VBO. Desides, I had this code written already.
 */
void drawTexturedSphere(float r, int segs) {
  int i, j;
  float x, y, z, z1, z2, R, R1, R2;

  // Top cap
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,0,1);
  glTexCoord2f(0.5f,1.0f); // This is an ugly (u,v)-mapping singularity
  glVertex3f(0,0,r);
  z = cos(M_PI/segs);
  R = sin(M_PI/segs);
    for(i = 0; i <= 2*segs; i++) {
      x = R*cos(i*2.0*M_PI/(2*segs));
      y = R*sin(i*2.0*M_PI/(2*segs));
      glNormal3f(x, y, z);
      glTexCoord2f((float)i/(2*segs), 1.0f-1.0f/segs);
      glVertex3f(r*x, r*y, r*z);
    }
  glEnd();  

  // Height segments
  for(j = 1; j < segs-1; j++) {
    z1 = cos(j*M_PI/segs);
    R1 = sin(j*M_PI/segs);
    z2 = cos((j+1)*M_PI/segs);
    R2 = sin((j+1)*M_PI/segs);
    glBegin(GL_TRIANGLE_STRIP);
    for(i = 0; i <= 2*segs; i++) {
      x = R1*cos(i*2.0*M_PI/(2*segs));
      y = R1*sin(i*2.0*M_PI/(2*segs));
      glNormal3f(x, y, z1);
      glTexCoord2f((float)i/(2*segs), 1.0f-(float)j/segs);
      glVertex3f(r*x, r*y, r*z1);
      x = R2*cos(i*2.0*M_PI/(2*segs));
      y = R2*sin(i*2.0*M_PI/(2*segs));
      glNormal3f(x, y, z2);
      glTexCoord2f((float)i/(2*segs), 1.0f-(float)(j+1)/segs);
      glVertex3f(r*x, r*y, r*z2);
    }
    glEnd();
  }

  // Bottom cap
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,0,-1);
  glTexCoord2f(0.5f, 1.0f); // This is an ugly (u,v)-mapping singularity
  glVertex3f(0,0,-r);
  z = -cos(M_PI/segs);
  R = sin(M_PI/segs);
    for(i = 2*segs; i >= 0; i--) {
      x = R*cos(i*2.0*M_PI/(2*segs));
      y = R*sin(i*2.0*M_PI/(2*segs));
      glNormal3f(x, y, z);
      glTexCoord2f(1.0f-(float)i/(2*segs), 1.0f/segs);
      glVertex3f(r*x, r*y, r*z);
    }
  glEnd();
}


/*
 * initDisplayList(GLuint *listID, GLdouble scale) - create a display list
 * to render the demo geometry more efficently than by glVertex() calls.
 * (This is currently just as fast as a VBO, and I'm a bit lazy.)
 */
void initDisplayList(GLuint *listID)
{
  *listID = glGenLists(1);
  
  glNewList(*listID, GL_COMPILE);
    glColor3f(1.0f, 1.0f, 1.0f); // White base color
	drawTexturedSphere(1.0, 20);
  glEndList();
}


/*
 * setupCamera() - set up the OpenGL projection and (model)view matrices
 */
void setupCamera() {

    int width, height;
    
    // Get window size. It may start out different from the requested
    // size, and will change if the user resizes the window.
    glfwGetWindowSize( &width, &height );
    if(height<=0) height=1; // Safeguard against iconified/closed window

    // Set viewport. This is the pixel rectangle we want to draw into.
    glViewport( 0, 0, width, height ); // The entire window

    // Select and setup the projection matrix.
    glMatrixMode(GL_PROJECTION); // "We want to edit the projection matrix"
    glLoadIdentity(); // Reset the matrix to identity
    // 45 degrees FOV, same aspect ratio as viewport, depth range 1 to 100
    gluPerspective( 45.0f, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f );

    // Select and setup the modelview matrix.
    glMatrixMode( GL_MODELVIEW ); // "We want to edit the modelview matrix"
    glLoadIdentity(); // Reset the matrix to identity
    // Look from 0,-3,0 towards 0,0,0 with Z as "up" in the image
    gluLookAt( 0.0f, -3.0f, 0.0f,    // Eye position
               0.0f, 0.0f, 0.0f,   // View point
               0.0f, 0.0f, 1.0f );  // Up vector
}


/*
 * renderScene() - draw the scene with the shader active
 */
void renderScene( GLuint listID, GLuint programObject )
{
  GLint location_time = -1;
  float time = 0.0f;

  // Use vertex and fragment shaders.
  glUseProgram( programObject );
  // Update the uniform time variable.
  location_time = glGetUniformLocation( programObject, "time" );
  // glUniform1f() is bugged in Linux Nvidia driver 260.19.06,
  // so we use glUniform1fv() instead to work around the bug.
  if ( location_time != -1 ) {
    time = (float)glfwGetTime();
    glUniform1fv( location_time, 1, &time );
  }
  glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
  // Render with the shaders active.
  glCallList( listID );
  // Deactivate the shaders.
  glUseProgram(0);
}


/*
 * main(argc, argv) - the standard C entry point for the program
 */
int main(int argc, char *argv[]) {

    GLuint displayList;
    GLuint programObject;
    double performance = 0.0;

    GLboolean running = GL_TRUE; // Main loop exits when this is set to GL_FALSE
    
    // Initialise GLFW
    glfwInit();

    // Open an OpenGL window
    if( !glfwOpenWindow(512, 512, 8,8,8,8, 32,0, GLFW_WINDOW) )
    {
        glfwTerminate(); // glfwOpenWindow failed, quit the program.
        return 1;
    }
    
    // Load the extensions for GLSL - note that this has to be done
    // *after* the window has been opened, or we won't have a GL context
    // to query for those extensions and connect to instances of them.
    loadExtensions();
    
    // Enable Z buffering (not needed for the sphere, but play nice)
    glEnable(GL_DEPTH_TEST); // Use the Z buffer

    glfwSwapInterval(0); // Do not wait for screen refresh between frames

    // Compile a display list for the demo geometry, to render it efficiently
    initDisplayList(&displayList);

	// Create the shader we are going to use
	createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE);
    
    // Main loop
    while(running)
    {
        // Calculate and update the frames per second (FPS) display
        performance = computeFPS();

        // Clear the frame buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        // Set up the camera projection.
        setupCamera();
        
        // Draw the scene.
        renderScene(displayList, programObject);

        // Swap buffers, i.e. display the image and prepare for next frame.
        glfwSwapBuffers();

        // Exit if the ESC key is pressed or the window is closed.
        if(glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED)) {
          running = GL_FALSE;
        }

	}

    // Close the OpenGL window and terminate GLFW.
    glfwTerminate();

    return 0;
}
