//
// Copyright (c) 2004 Sami Väisänen
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

// $Id: pyramid.h,v 1.3 2008/02/24 14:06:50 enska Exp $

#ifndef PYRAMID_H
#define PYRAMID_H

#include <gl/gl.h>
#include <gl/glu.h>
#include <windows.h>
#include <cassert>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

/// primitive opengl rendering 
// code from NeHe.
class pyramid
{
public:
    void gl_resize(int width, int height)
    {
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
    void gl_render(GLfloat xRot, GLfloat yRot, GLfloat zRot)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glTranslatef(0.0f, 0.0f, -6.0f);
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);
        glRotatef(zRot, 0.0f, 0.0f, 1.0f);

        glBegin(GL_TRIANGLES);			// Start Drawing A Triangle
        glColor3f(1.0f,0.0f,0.0f);		// Red
        glVertex3f( 0.0f, 1.0f, 0.0f);	// Top Of Triangle (Front)
        glColor3f(0.0f,1.0f,0.0f);		// Green
        glVertex3f(-1.0f,-1.0f, 1.0f);	// Left Of Triangle (Front)
        glColor3f(0.0f,0.0f,1.0f);		// Blue
        glVertex3f( 1.0f,-1.0f, 1.0f);	// Right Of Triangle (Front)
        glColor3f(1.0f,0.0f,0.0f);		// Red
        glVertex3f( 0.0f, 1.0f, 0.0f);	// Top Of Triangle (Right)
        glColor3f(0.0f,0.0f,1.0f);		// Blue
        glVertex3f( 1.0f,-1.0f, 1.0f);	// Left Of Triangle (Right)
        glColor3f(0.0f,1.0f,0.0f);		// Green
        glVertex3f( 1.0f,-1.0f, -1.0f);	// Right Of Triangle (Right)
        glColor3f(1.0f,0.0f,0.0f);		// Red
        glVertex3f( 0.0f, 1.0f, 0.0f);	// Top Of Triangle (Back)
        glColor3f(0.0f,1.0f,0.0f);		// Green
        glVertex3f( 1.0f,-1.0f, -1.0f);	// Left Of Triangle (Back)
        glColor3f(0.0f,0.0f,1.0f);		// Blue
        glVertex3f(-1.0f,-1.0f, -1.0f);	// Right Of Triangle (Back)
        glColor3f(1.0f,0.0f,0.0f);		// Red
        glVertex3f( 0.0f, 1.0f, 0.0f);	// Top Of Triangle (Left)
        glColor3f(0.0f,0.0f,1.0f);		// Blue
        glVertex3f(-1.0f,-1.0f,-1.0f);	// Left Of Triangle (Left)
        glColor3f(0.0f,1.0f,0.0f);		// Green
        glVertex3f(-1.0f,-1.0f, 1.0f);	// Right Of Triangle (Left)
        glEnd();				// Done Drawing The Pyramid        

        SwapBuffers(hdc_);
    }
    void gl_init(HWND hwnd)
    {
        static	PIXELFORMATDESCRIPTOR pfd=	// pfd Tells Windows How We Want Things To Be
          {
              sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
              1,                                // Version Number
              PFD_DRAW_TO_WINDOW |              // Format Must Support Window
              PFD_SUPPORT_OPENGL |              // Format Must Support OpenGL
              PFD_DOUBLEBUFFER,                 // Must Support Double Buffering
              PFD_TYPE_RGBA,                    // Request An RGBA Format
              16,                               // Select Our Color Depth
              0, 0, 0, 0, 0, 0,                 // Color Bits Ignored
              0,                                // No Alpha Buffer
              0,                                // Shift Bit Ignored
              0,                                // No Accumulation Buffer
              0, 0, 0, 0,                       // Accumulation Bits Ignored
              16,                               // 16Bit Z-Buffer (Depth Buffer)  
              0,                                // No Stencil Buffer
              0,                                // No Auxiliary Buffer
              PFD_MAIN_PLANE,                   // Main Drawing Layer
              0,                                // Reserved
              0, 0, 0                           // Layer Masks Ignored
          };

        hdc_ = GetDC(hwnd);
        assert(hdc_);

        SetPixelFormat(hdc_, ChoosePixelFormat(hdc_, &pfd), &pfd);
        hrc_ = wglCreateContext(hdc_);
        assert(hrc_);
        
        wglMakeCurrent(hdc_, hrc_);

        glShadeModel(GL_SMOOTH);
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        hwnd_ = hwnd;
    }
protected:
   ~pyramid() 
    {
        if (hrc_)
        {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hrc_);
        }
        if (hdc_)
        {
            ReleaseDC(hwnd_, hdc_);
        }
    }
    pyramid() : hdc_(0), hrc_(0),  hwnd_(0) {}
    LRESULT paint(HWND, const PAINTSTRUCT&) 
    {
        return TRUE;
    }
    LRESULT erase(HWND)
    {
        return TRUE;
    }

private:
    HDC   hdc_;
    HGLRC hrc_; // handle to opengl rendering context
    HWND  hwnd_;
};

#endif
