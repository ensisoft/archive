

#include <GL/glut.h>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <vector>
#include <unistd.h>

  //#include "tga.h"

#define ES_PI  (3.14159265f)

using namespace std;

namespace
{

bool global_ambient = false;
bool color_tracking = false;
bool light_one = false;
bool material = false;
bool spotlight = false;
bool spotlight2 = false;
bool blend = false;
bool explicit_light = false;
bool emission = false;
bool light_model_two_side = false;
bool backface_culling = true;

GLuint model;
GLuint texture;
GLuint texturefilter = 0;
GLuint textureenv    = 0;
GLuint textureMode[] = {0, GL_LINEAR, GL_NEAREST };
GLuint textureEnv[]  = {GL_REPLACE, GL_MODULATE, GL_DECAL, GL_BLEND, GL_ADD};

GLuint fogfilter   = 0;
GLuint fogMode[]   = {0, GL_EXP, GL_EXP2, GL_LINEAR };

struct vertex {
    GLfloat x, y, z;
    GLubyte r, g, b, a;
} __attribute__((packed));


struct normal {
    GLfloat x, y, z;
};

struct texcoord {
    GLshort x, y;
};

int esGenSphere ( int numSlices, float radius, GLfloat **vertices, GLfloat **normals, 
                  GLfloat **texCoords, GLshort **indices )
{
   int i;
   int j;
   int numParallels = numSlices / 2;
   int numVertices = ( numParallels + 1 ) * ( numSlices + 1 );
   int numIndices = numParallels * numSlices * 6;
   float angleStep = (2.0f * ES_PI) / ((float) numSlices);

   // Allocate memory for buffers
   if ( vertices != NULL )
      *vertices = (GLfloat*)malloc ( sizeof(GLfloat) * 3 * numVertices );
   
   if ( normals != NULL )
      *normals = (GLfloat*)malloc ( sizeof(GLfloat) * 3 * numVertices );

   if ( texCoords != NULL )
      *texCoords = (GLfloat*)malloc ( sizeof(GLfloat) * 2 * numVertices );

   if ( indices != NULL )
      *indices = (GLshort*)malloc ( sizeof(GLshort) * numIndices );

   for ( i = 0; i < numParallels + 1; i++ )
   {
      for ( j = 0; j < numSlices + 1; j++ )
      {
         int vertex = ( i * (numSlices + 1) + j ) * 3; 

         if ( vertices )
         {
            (*vertices)[vertex + 0] = radius * sinf ( angleStep * (float)i ) *
                                               sinf ( angleStep * (float)j );
            (*vertices)[vertex + 1] = radius * cosf ( angleStep * (float)i );
            (*vertices)[vertex + 2] = radius * sinf ( angleStep * (float)i ) *
                                               cosf ( angleStep * (float)j );
         }

         if ( normals )
         {
            (*normals)[vertex + 0] = (*vertices)[vertex + 0] / radius;
            (*normals)[vertex + 1] = (*vertices)[vertex + 1] / radius;
            (*normals)[vertex + 2] = (*vertices)[vertex + 2] / radius;
         }

         if ( texCoords )
         {
            int texIndex = ( i * (numSlices + 1) + j ) * 2;
            (*texCoords)[texIndex + 0] = (float) j / (float) numSlices;
            (*texCoords)[texIndex + 1] = ( 1.0f - (float) i ) / (float) (numParallels - 1 );
         }
      }
   }

   // Generate the indices
   if ( indices != NULL )
   {
      GLshort *indexBuf = (*indices);
      for ( i = 0; i < numParallels ; i++ ) 
      {
         for ( j = 0; j < numSlices; j++ )
         {
            *indexBuf++  = i * ( numSlices + 1 ) + j;
            *indexBuf++ = ( i + 1 ) * ( numSlices + 1 ) + j;
            *indexBuf++ = ( i + 1 ) * ( numSlices + 1 ) + ( j + 1 );

            *indexBuf++ = i * ( numSlices + 1 ) + j;
            *indexBuf++ = ( i + 1 ) * ( numSlices + 1 ) + ( j + 1 );
            *indexBuf++ = i * ( numSlices + 1 ) + ( j + 1 );
         }
      }
   }

   return numIndices;
}

void render_sphere()
{
    static GLfloat* verts;
    static GLfloat* normals;
    static GLfloat* texcoords;
    static GLshort* indices;
    static int count;

    if (!verts)
        count = esGenSphere(50, 1.5, &verts, &normals, &texcoords, &indices);
    
        
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, 0, verts);
    glNormalPointer(GL_FLOAT, 0, normals);

    glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_SHORT, indices);
    if (int err = glGetError())
        printf("glError: %d\n", err);
}

void render_points()
{
    vertex v0 = { 0,  1, 0, 0xFF, 0, 0, 0xFF};
    vertex v1 = {-1, -1, 0, 0, 0xff, 0, 0xff};
    vertex v2 = { 1, -1, 0, 0, 0, 0xff, 0xff};
    
    vector<vertex> v;
    v.push_back(v0);
    v.push_back(v1);
    v.push_back(v2);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    //glEnableClientState(GL_POINT_SIZE_ARRAY_OES);
    
    glPointSize(15);

    //GLfloat size[3] = {5, 10, 15};
    //glPointSizePointerOES(GL_FLOAT, 0, size);


    glVertexPointer(3, GL_FLOAT, sizeof(vertex), (GLfloat*)&v[0]);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex), ((GLubyte*)&v[0]) + 3 * sizeof(GLfloat));
    
    static unsigned int frame;
    glRotatef(fmod(frame * .5, 360), 0, 0, 1);

    glDrawArrays(GL_POINTS, 0, v.size());
    //if (int err = glGetError())
    //printf("glError: %d\n", err);
    
    ++frame;
}


void render_cube()
{
    // cube
    ///////////////////////////////////////////////////////////////////////
    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3

    vertex v0 = {1, 1, 1,    0x50, 0, 0, 0x40};
    vertex v1 = {-1, 1, 1,   0x50, 0, 0, 0x40};
    vertex v2 = {-1, -1, 1,  0x50, 0, 0, 0x40};
    vertex v3 = {1, -1, 1,   0x50, 0, 0, 0x40};
    vertex v4 = {1, -1, -1,  0x50, 0, 0, 0x40};
    vertex v5 = {1, 1, -1,   0x50, 0, 0, 0x40};
    vertex v6 = {-1, 1, -1,  0x50, 0, 0, 0x40};
    vertex v7 = {-1, -1, -1, 0x50, 0, 0, 0x40};

    vector<vertex> v;
    
    normal front = {0, 0, 1};
    normal back  = {0, 0, -1};
    normal left  = {-1, 0, 0};
    normal right = {1, 0, 0};
    normal top   = {0, 1, 0};
    normal bottom = {0, -1, 0};
    
    texcoord topleft     = {0, 1};
    texcoord topright    = {1, 1};
    texcoord bottomright = {1, 0};
    texcoord bottomleft  = {0, 0};

    vector<normal> n;
    vector<texcoord> t;

    // front face
    v.push_back(v0); n.push_back(front);  t.push_back(topright);
    v.push_back(v1); n.push_back(front);  t.push_back(topleft);
    v.push_back(v2); n.push_back(front);  t.push_back(bottomleft);
    v.push_back(v2); n.push_back(front);  t.push_back(bottomleft);
    v.push_back(v3); n.push_back(front);  t.push_back(bottomright);
    v.push_back(v0); n.push_back(front);  t.push_back(topright);

    // back face
    v.push_back(v7); n.push_back(back);   t.push_back(bottomright);
    v.push_back(v6); n.push_back(back);   t.push_back(topright);
    v.push_back(v5); n.push_back(back);   t.push_back(topleft);
    v.push_back(v5); n.push_back(back);   t.push_back(topleft);
    v.push_back(v4); n.push_back(back);   t.push_back(bottomleft);
    v.push_back(v7); n.push_back(back);   t.push_back(bottomright);
    
    // right face
    v.push_back(v5); n.push_back(right);  t.push_back(topright);
    v.push_back(v0); n.push_back(right);  t.push_back(topleft);
    v.push_back(v3); n.push_back(right);  t.push_back(bottomleft);
    v.push_back(v3); n.push_back(right);  t.push_back(bottomleft);
    v.push_back(v4); n.push_back(right);  t.push_back(bottomright);
    v.push_back(v5); n.push_back(right);  t.push_back(topright);

    // left face
    v.push_back(v1); n.push_back(left);   t.push_back(topright);
    v.push_back(v6); n.push_back(left);   t.push_back(topleft);
    v.push_back(v7); n.push_back(left);   t.push_back(bottomleft);
    v.push_back(v7); n.push_back(left);   t.push_back(bottomleft);
    v.push_back(v2); n.push_back(left);   t.push_back(bottomright);
    v.push_back(v1); n.push_back(left);   t.push_back(topright);
    
    
    // top face
    v.push_back(v5); n.push_back(top);    t.push_back(topright);
    v.push_back(v6); n.push_back(top);    t.push_back(topleft);
    v.push_back(v1); n.push_back(top);    t.push_back(bottomleft);
    v.push_back(v1); n.push_back(top);    t.push_back(bottomleft);
    v.push_back(v0); n.push_back(top);    t.push_back(bottomright);
    v.push_back(v5); n.push_back(top);    t.push_back(topright);
    
    // bottom face
    v.push_back(v4); n.push_back(bottom); t.push_back(bottomright);
    v.push_back(v3); n.push_back(bottom); t.push_back(topright);
    v.push_back(v2); n.push_back(bottom); t.push_back(topleft);
    v.push_back(v2); n.push_back(bottom); t.push_back(topleft);
    v.push_back(v7); n.push_back(bottom); t.push_back(bottomleft);
    v.push_back(v4); n.push_back(bottom); t.push_back(bottomright);

    if (textureMode[texturefilter])
    {
        const GLuint one = 0xffffffff;
        const GLuint two = 0x0000ffff;
        GLuint texture_data[] = {
            one, two, one, two,
            two, one, two, one,
            one, two, one, two,
            two, one, two, one
        };
        if (!texture)
        {
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0,GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        }
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureEnv[textureenv]);

        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        glEnable(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMode[texturefilter]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureMode[texturefilter]);

        glBindTexture(GL_TEXTURE_2D, texture);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_SHORT, 0, &t[0]);
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }



    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, sizeof(vertex), (GLfloat*)&v[0]);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex), ((GLubyte*)&v[0]) + 3 * sizeof(GLfloat));
    glNormalPointer(GL_FLOAT, 0, (GLfloat*)&n[0]);

    glRotatef(45, 0, 1, 0);
    glRotatef(-45, 0, 0, 1);

    static int frame;

    glRotatef(fmod(frame * 0.15, 360), 0, 0, 1);
    glRotatef(fmod(frame * 0.158, 360), 1, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, v.size());
    if (int err = glGetError())
        printf("glError:%d\n", err);

    ++frame;
}

void render()
{

    glLoadIdentity();
    glTranslatef(0, 0, -10);

    if (global_ambient)
    {
        //GLfloat global_ambient[] = {1, 1, 1, 1.0};
        //GLfloat global_ambient[] = {.5, .5, .5, 1};
        GLfloat global_ambient[] = {.2, 0, .5, 1};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    }
    else
    {
        // restore default values
        GLfloat global_ambient[] = {.2, .2, .2, 1};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    }

    //glShadeModel(GL_FLAT);

    // enable color tracking, this includes the vertex color in the
    // light equation.
    if (color_tracking)
        glEnable(GL_COLOR_MATERIAL);
    else
        glDisable(GL_COLOR_MATERIAL);

    // // light 1
    if (light_one)
    {
        GLfloat position[] = {0, 5, 0, 1};

        GLfloat ambient[] = {0, .5, 0, 1};
        GLfloat specular[] = {1, 1, 1, 1};
        GLfloat diffuse[] = {0, .75, 0, 1};

        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
        // end light 1
    }
    else
    {
        glDisable(GL_LIGHT0);
    }
    
    if (spotlight)
    {
        // yellow spot
        //GLfloat position[] = {0, 0, 2, 0}; // w
        GLfloat position[] = {-2, 0, 0, 0};

        GLfloat ambient[]  = {.5, .5, 0, 1}; 
        GLfloat specular[] = {1, 1, 1, 1};
        GLfloat diffuse[]  = {.75, .75, 0, 1};

        GLfloat direction[] = {1, 0, 0};

        glEnable(GL_LIGHT1);
        glLightfv(GL_LIGHT1, GL_POSITION, position);
        glLightfv(GL_LIGHT1, GL_AMBIENT,  ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE,  diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
        
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);

        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF,    10); // cutoff angle != 180 -> spotlight
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,  60);
    }
    else
    {
        glDisable(GL_LIGHT1);
    }

    if (spotlight2)
    {
        // blue spot
        //GLfloat position[] = {0, 0, 2, 0}; // w
        GLfloat position[] = {2, 0, 0, 0};

        GLfloat ambient[]  = {0, 0, .5, 1}; 
        GLfloat specular[] = {1, 1, 1, 1};
        GLfloat diffuse[]  = {0, 0, .75, 1};

        GLfloat direction[] = {-1, 0, 0};

        glEnable(GL_LIGHT2);
        glLightfv(GL_LIGHT2, GL_POSITION, position);
        glLightfv(GL_LIGHT2, GL_AMBIENT,  ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE,  diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
        
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction);

        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF,    10); // cutoff angle != 180 -> spotlight
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT,  60);
    }
    else
    {
        glDisable(GL_LIGHT2);
    }


    if (fogMode[fogfilter])
    {
        usleep(10000);
        static float d = 0;
        static float v = 0.0000000008;
    
        d += v;
        if (d < -15)
            v = 0.1;
        else if (d > -1)
            v = -0.1;

        glTranslatef(0, 0, d + 8);

        GLfloat fogColor[4] = {0.5f,0.5f,0.5f,1.0f}; 

        glFogf(GL_FOG_MODE, fogMode[fogfilter]); 
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_DENSITY, 0.35f); 
        glHint(GL_FOG_HINT, GL_DONT_CARE); 
        glFogf(GL_FOG_START, 0.0f); 
        glFogf(GL_FOG_END, 15.0f);
        glEnable(GL_FOG); 

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    }
    else
    {
        glClearColor(0, 0, 0, 1);
        glDisable(GL_FOG);
    }

    if (material)
    {
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60);
        GLfloat specular[] = {1, 1, 1, 1};
        GLfloat diffuse[]  = {1, 1, 1, 1};
        GLfloat ambient[]  = {1, 1, 1, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ambient);
        
    }
    else
    {
        // default values
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
        GLfloat specular[] = {0, 0, 0, 1};
        GLfloat diffuse[]  = {.8, .8, .8, 1};
        GLfloat ambient[]  = {.2, .2, .2, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ambient);
    }

    if (emission)
    {
        GLfloat emission[] = {1, 0, 0, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    }
    else
    {
        // default emission
        GLfloat emission[] = {0, 0, 0, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);        
    }


    if (blend)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }


    if (material || emission || global_ambient || light_one || spotlight || spotlight2 || explicit_light)
    {
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, light_model_two_side);
        glEnable(GL_LIGHTING);
    }
    else
    {
        glDisable(GL_LIGHTING);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //glDisableClientState(GL_POINT_SIZE_ARRAY_OES);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (model)
    {
        case 0:
            render_cube();
            break;
        case 1:
            render_sphere();
            break;
        case 2:
            render_points();
            break;
    }
    
    glutSwapBuffers();
}

void keyhit(int key)
{
    switch ( key )
    {
        case '-':
            model = (model + 1) % 3;
            switch (model)
            {
                case 0:
                    printf("cube\n");
                    break;
                case 1:
                    printf("sphere\n");
                    break;
                case 2:
                    printf("points\n");
                    break;
            }
            break;

        case 'q':
            exit(0);
            break;

        case 'g':
            global_ambient = !global_ambient;
            printf("global_ambient: %d\n", global_ambient);
            break;

        case 'c':
            color_tracking = !color_tracking;
            printf("color_tracking: %d\n", color_tracking);
            break;
                    
        case 'e':
            emission = !emission;
            printf("emission: %d\n", emission);
            break;

        case 'l':
            light_one = !light_one;
            printf("light_one: %d\n", light_one);
            break;
                    
        case 'm':
            material = !material;
            printf("material: %d\n", material);
            break;
        case 's':
            spotlight  = !spotlight;
            spotlight2 = !spotlight2;
            printf("spotlight: %d\n", spotlight);
            break;

        case '1':
            spotlight = !spotlight;
            printf("spotlight1: %d\n", spotlight);
            break;
                    
        case '2':
            spotlight2 = !spotlight2;
            printf("spotlight2: %d\n", spotlight2);
            break;

        case '7':
            textureenv = (textureenv + 1) % 5;
            switch (textureenv)
            {
                case 0:
                    printf("texenv: GL_REPLACE\n");
                    break;
                case 1:
                    printf("texenv: GL_MODULATE\n");
                    break;
                case 2:
                    printf("texenv: GL_DECAL\n");
                    break;
                case 3:
                    printf("texenv: GL_BLEND\n");
                    break;
                case 4:
                    printf("texenv: GL_ADD\n");
                    break;
            }
            break;

        case '8':
            texturefilter = (texturefilter + 1) % 3;
            switch (texturefilter)
            {
                case 0:
                    printf("texture is off\n");
                    break;
                case 1:
                    printf("GL_LINEAR texture\n");
                    break;
                case 2:
                    printf("GL_NEAREST texture\n");
                    break;
            }
            break;

        case '9':
            backface_culling = !backface_culling;
            printf("backface culling: %d\n", backface_culling);
            if (backface_culling)
                glEnable(GL_CULL_FACE);
            else
                glDisable(GL_CULL_FACE);
            break;

        case 'f':
            fogfilter = (fogfilter + 1) % 4;
            switch (fogfilter)
            {
                case 0:
                    printf("fog is off\n");
                    break;
                case 1:
                    printf("GL_EXP fog\n");
                    break;
                case 2:
                    printf("GL_EXP2 fog\n");
                    break;
                case 3:
                    printf("GL_LINEAR fog\n");
                    break;
            }
            break;

        case 'b':
            blend = !blend;
            printf("blending: %d\n", blend);
            break;

        case 'd':
            //take_screenshot();
            break;
                    
        case 'h':
            printf("help: q)uit,  g)lobal ambient, c)olor tracking, l)ight one, m)aterial, s)potlight, f)og b)lend d)ump screen\n");
            break;
                    
        case 'x':
            explicit_light = !explicit_light;
            printf("explicit light: %d\n", explicit_light);
            break;
                    
        case 't':
            light_model_two_side = !light_model_two_side;
            printf("light model two side: %d\n", light_model_two_side);
            break;
    }
}

void special_keys(int key, int, int)
{
    keyhit(key);
}

void regular_keys(unsigned char key, int, int)
{
    keyhit(key);
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


} // namespace


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(640, 640);
    glutCreateWindow("light");

    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    if (backface_culling)
        glEnable(GL_CULL_FACE);
    else 
        glDisable(GL_CULL_FACE);


    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(regular_keys);
    glutSpecialFunc(special_keys);
    
    
    glutMainLoop();

    return 0;
}
