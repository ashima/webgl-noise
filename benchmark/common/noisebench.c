/* 
 * Benchmark for GLSL procedural noise functions.
 *
 * Shaders are loaded from external files, named in
 * the macro definitions VERTSHADERFILE and FRAGSHADERFILE*.
 * The main program draws a flat plane covering the
 * viewport, activates each of six fragment shaders in turn
 * and reports the fragment shading performance in Msamples/s
 * for each.
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

// Identify the exact version of noise being benchmarked
#define NOISEVERSION "2011-04-03"

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
#define VERTSHADERFILE "../../../noisebench.vert"
#define FRAGSHADERFILE_S2D "../../../simplexnoise2D.frag"
#define FRAGSHADERFILE_S3D "../../../simplexnoise3D.frag"
#define FRAGSHADERFILE_S4D "../../../simplexnoise4D.frag"
#define FRAGSHADERFILE_C2D "../../../classicnoise2D.frag"
#define FRAGSHADERFILE_C3D "../../../classicnoise3D.frag"
#define FRAGSHADERFILE_C4D "../../../classicnoise4D.frag"
#define FRAGSHADERFILE_CONST "../../../constant.frag"
#define LOGFILENAME "../../../ashimanoise.log"
#else
// Windows, Linux and other Unix systems expose executables as naked files
#define VERTSHADERFILE "noisebench.vert"
#define FRAGSHADERFILE_S2D "simplexnoise2D.frag"
#define FRAGSHADERFILE_S3D "simplexnoise3D.frag"
#define FRAGSHADERFILE_S4D "simplexnoise4D.frag"
#define FRAGSHADERFILE_C2D "classicnoise2D.frag"
#define FRAGSHADERFILE_C3D "classicnoise3D.frag"
#define FRAGSHADERFILE_C4D "classicnoise4D.frag"
#define FRAGSHADERFILE_CONST "constant.frag"
#define LOGFILENAME "ashimanoise.log"
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
    static double Msamplespersecond = 0.0;
    static char titlestring[200];

    double t, fps;
    int width, height;
    
    // Get current time
    t = glfwGetTime();  // Gets number of seconds since glfwInit()
    // If one second has passed, or if this is the very first frame
    if( (t-t0) > 1.0 || frames == 0 )
    {
        fps = (double)frames / (t-t0);
        glfwGetWindowSize( &width, &height );
        // This assumes that multisampling for FSAA is disabled.
        Msamplespersecond = 1e-6*fps*width*height;
        sprintf(titlestring, "GLSL simplex noise (%.1f M samples/s)", Msamplespersecond);
        glfwSetWindowTitle(titlestring);
        printf("Speed: %.1f M samples/s\n", Msamplespersecond);
        t0 = t;
        frames = 0;
    }
    frames ++;
    return Msamplespersecond;
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
    glOrtho( -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f );

    // Select and setup the modelview matrix.
    glMatrixMode( GL_MODELVIEW ); // "We want to edit the modelview matrix"
    glLoadIdentity(); // Reset the matrix to identity
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
    glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, -1.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-1.0f, 1.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(1.0f, 1.0f, 0.0f);
    glEnd();
  glEndList();
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
    int activeshader = 0; // Currently active version of noise shader
    FILE *logfile;
    GLFWvidmode vidmode;

    GLboolean running = GL_TRUE; // Main loop exits when this is set to GL_FALSE
    
    // Initialise GLFW
    glfwInit();

    // Open a temporary OpenGL window just to determine the desktop size
    if( !glfwOpenWindow(256, 256, 8,8,8,8, 32,0, GLFW_WINDOW) )
    {
        glfwTerminate(); // glfwOpenWindow failed, quit the program.
        return 1;
    }
    glfwGetDesktopMode(&vidmode);
    glfwCloseWindow();

    // Open a fullscreen window using the current desktop resolution
    if( !glfwOpenWindow(vidmode.Width, vidmode.Height, 8,8,8,8, 32,0, GLFW_FULLSCREEN) )
    {
        glfwTerminate(); // glfwOpenWindow failed, quit the program.
        return 1;
    }
    
    // Load the extensions for GLSL - note that this has to be done
    // *after* the window has been opened, or we won't have a GL context
    // to query for those extensions and connect to instances of them.
    loadExtensions();
    
    logfile = fopen(LOGFILENAME, "w");

    fprintf(logfile, "GL vendor:     %s\n", glGetString(GL_VENDOR));
    fprintf(logfile, "GL renderer:   %s\n", glGetString(GL_RENDERER));
    fprintf(logfile, "GL version:    %s\n", glGetString(GL_VERSION));
    fprintf(logfile, "Desktop size:  %d x %d pixels\n", vidmode.Width, vidmode.Height);
	fprintf(logfile, "Noise version: %s\n\n", NOISEVERSION);

    // Disable Z buffering for this simple 2D shader benchmark
    glDisable(GL_DEPTH_TEST); // Use the Z buffer

    glfwSwapInterval(0); // Do not wait for screen refresh between frames

    // Compile a display list for the demo geometry, to render it efficiently
    initDisplayList(&displayList);
    
    // Main loop
    while(running)
    {
		double benchmarktime = 3.0; // Total time to run each shader
        // Switch between the different versions of noise
		if(activeshader == 0) {
			createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_CONST);
			activeshader++;
            fprintf(logfile, "Constant shading, ");
		}
        if((activeshader == 1) && (glfwGetTime() > benchmarktime * activeshader)) {
            fprintf(logfile, "%.1f Msamples/s\n", performance);
            createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_S2D);
			activeshader++;
            fprintf(logfile, "2D simplex noise, ");
        }
        if((activeshader == 2) && (glfwGetTime() > benchmarktime * activeshader)) {
            fprintf(logfile, "%.1f Msamples/s\n", performance);
            createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_S3D);
			activeshader++;
            fprintf(logfile, "3D simplex noise, ");
        }
        if((activeshader == 3) && (glfwGetTime() > benchmarktime * activeshader)) {
            fprintf(logfile, "%.1f Msamples/s\n", performance);
            createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_S4D);
			activeshader++;
            fprintf(logfile, "4D simplex noise, ");
        }
        if((activeshader == 4) && (glfwGetTime() > benchmarktime * activeshader)) {
            fprintf(logfile, "%.1f Msamples/s\n", performance);
            createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_C2D);
			activeshader++;
            fprintf(logfile, "2D classic noise, ");
        }
        if((activeshader == 5) && (glfwGetTime() > benchmarktime * activeshader)) {
            fprintf(logfile, "%.1f Msamples/s\n", performance);
            createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_C3D);
			activeshader++;
            fprintf(logfile, "3D classic noise, ");
        }
        if((activeshader == 6) && (glfwGetTime() > benchmarktime * activeshader)) {
            fprintf(logfile, "%.1f Msamples/s\n", performance);
            createShader(&programObject, VERTSHADERFILE, FRAGSHADERFILE_C4D);
			activeshader++;
            fprintf(logfile, "4D classic noise, ");
        }
        if((activeshader == 7) && (glfwGetTime() > benchmarktime * activeshader)) {
            fprintf(logfile, "%.1f Msamples/s\n", performance);
            running = GL_FALSE;
        }

        // Exit prematurely if the ESC key is pressed or the window is closed.
        if(glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED)) {
          running = GL_FALSE;
        }

        // Calculate and update the frames per second (FPS) display
        performance = computeFPS();

        // Do not clear the buffers - this is a raw shader benchmark.
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        // Set up the camera projection.
        setupCamera();
        
        // Draw the scene.
        renderScene(displayList, programObject);

        // Swap buffers, i.e. display the image and prepare for next frame.
        glfwSwapBuffers();
    }

    // Close the OpenGL window and terminate GLFW.
    glfwTerminate();

    fclose(logfile);

    return 0;
}
