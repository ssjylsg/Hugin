// -*- c-basic-offset: 4 -*-
/** @file ProjectionGridTool.h
 *
 *  @author Darko Makreshanski
 *
 *  @brief implementation of ProjectionGridTool Class
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#if !defined Hugin_shared || !defined _WINDOWS
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#ifdef __WXMAC__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#ifdef __APPLE__
  #include <GLUT/glut.h>
#endif


#include "ProjectionGridTool.h"

ProjectionGridTool::ProjectionGridTool(ToolHelper* helper) : helper_g(helper)
{
    mesh_info = NULL;
    texture_created = false;
}

ProjectionGridTool::~ProjectionGridTool()
{
    if (mesh_info) {
        delete mesh_info;
    }
}

void PreviewProjectionGridTool::Activate()
{
    helper->NotifyMe(ToolHelper::DRAW_OVER_IMAGES, this);
    helper->NotifyMe(ToolHelper::DRAW_UNDER_IMAGES, this);
}


void PreviewProjectionGridTool::BeforeDrawImagesEvent()
{
}

void PreviewProjectionGridTool::AfterDrawImagesEvent()
{

    DEBUG_DEBUG("begin");
    if (!texture_created) {
        if (!createTexture()) {
            return;
        }
    }

    if (!mesh_info) {
        createMesh();
    }

    mesh_info->Update();

//    glPushMatrix();
    glColor4f(1,1,1,1);
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if(helper->GetViewStatePtr()->GetSupportMultiTexture())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_num);
    }
    else
        glBindTexture(GL_TEXTURE_2D, texture_num);
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glScalef(0.5,1,1);
    glMatrixMode(GL_MODELVIEW);
    mesh_info->CallList();
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_BLEND);
//    glPopMatrix();

    
}

void PanosphereOverviewProjectionGridTool::Activate()
{
    ((PanosphereOverviewToolHelper*)helper)->NotifyMe(PanosphereOverviewToolHelper::DRAW_OVER_IMAGES_BACK, this);
    ((PanosphereOverviewToolHelper*)helper)->NotifyMe(PanosphereOverviewToolHelper::DRAW_UNDER_IMAGES_BACK, this);
    ((PanosphereOverviewToolHelper*)helper)->NotifyMe(PanosphereOverviewToolHelper::DRAW_OVER_IMAGES_FRONT, this);
    ((PanosphereOverviewToolHelper*)helper)->NotifyMe(PanosphereOverviewToolHelper::DRAW_UNDER_IMAGES_FRONT, this);
}


void PanosphereOverviewProjectionGridTool::BeforeDrawImagesBackEvent()
{

}

void PanosphereOverviewProjectionGridTool::BeforeDrawImagesFrontEvent()
{
}

void PanosphereOverviewProjectionGridTool::AfterDrawImagesBackEvent()
{
    DEBUG_DEBUG("begin");

    if (!texture_created) {
        if (!createTexture()) {
            return;
        }
    }

    if (!mesh_info) {
        createMesh();
    }

    DEBUG_DEBUG("resources created");

    glDisable(GL_TEXTURE_2D);
    glColor4f(0.3,0.3,0.3,0.6);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLUquadric* gridb = gluNewQuadric();
    gluSphere(gridb, 101,40,20);

    glEnable(GL_TEXTURE_2D);

    glColor4f(1,1,1,0.3);
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if(helper->GetViewStatePtr()->GetSupportMultiTexture())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_num);
    }
    else
        glBindTexture(GL_TEXTURE_2D, texture_num);

    glMatrixMode(GL_TEXTURE);
//    using just a sphere instead of the remapped mesh for better quality
//    glPushMatrix();
//    glScalef(0.5,1,1);
//    glMatrixMode(GL_MODELVIEW);
//    mesh_info->CallList();
//    glMatrixMode(GL_TEXTURE);
//    glPopMatrix();

    glPushMatrix();
    glRotated(180,1,0,0);
    glScalef(0.5,1,1);
    glMatrixMode(GL_MODELVIEW);
    GLUquadric* grid = gluNewQuadric();
    gluQuadricTexture(grid, GL_TRUE);

    glPushMatrix();
    glScalef(-1,1,1);
    glRotated(-90,1,0,0);
    gluSphere(grid, 101,40,20);
    glPopMatrix();

    glMatrixMode(GL_TEXTURE);
    glPopMatrix();


    glDisable(GL_BLEND);

    glMatrixMode(GL_MODELVIEW);

    DEBUG_DEBUG("end");

    
}

void PanosphereOverviewProjectionGridTool::AfterDrawImagesFrontEvent()
{


    if (!texture_created) {
        if (!createTexture()) {
            return;
        }
    }

    if (!mesh_info) {
        createMesh();
    }


    glColor4f(1,1,1,1);
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    DEBUG_DEBUG("proj grid tex " << texture_num);
    if(helper->GetViewStatePtr()->GetSupportMultiTexture())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_num);
    }
    else
        glBindTexture(GL_TEXTURE_2D, texture_num);
    glMatrixMode(GL_TEXTURE);

    //using just a sphere instead of the remapped mesh for better quality
//    glPushMatrix();
//    glScalef(0.5,1,1);
//    glMatrixMode(GL_MODELVIEW);
//    mesh_info->CallList();
//    glMatrixMode(GL_TEXTURE);
//    glPopMatrix();

    glPushMatrix();
    glRotated(180,1,0,0);
    glScalef(0.5,1,1);
    glMatrixMode(GL_MODELVIEW);

    GLUquadric* grid = gluNewQuadric();
    gluQuadricTexture(grid, GL_TRUE);
    glPushMatrix();
    glScalef(-1,1,1);
    glRotated(-90,1,0,0);
    gluSphere(grid, 101,40,20);
    glPopMatrix();

    glMatrixMode(GL_TEXTURE);
    glPopMatrix();


    glDisable(GL_BLEND);
    glMatrixMode(GL_MODELVIEW);

}

void PreviewProjectionGridTool::createMesh()
{
    HuginBase::SrcPanoImage image;
    image.setSize(vigra::Size2D(3600,1780));
    image.setHFOV(360);
    image.setProjection(HuginBase::BaseSrcPanoImage::EQUIRECTANGULAR);
    mesh_info = new MeshManager::MeshInfo(helper->GetPanoramaPtr(), &image, helper->GetVisualizationStatePtr(), false);
}

void PanosphereOverviewProjectionGridTool::createMesh()
{
    DEBUG_DEBUG("Create mesh projection grid");
    HuginBase::SrcPanoImage image;
    image.setSize(vigra::Size2D(3600,1780));
    image.setHFOV(360);
    image.setProjection(HuginBase::BaseSrcPanoImage::EQUIRECTANGULAR);
    mesh_info = new MeshManager::PanosphereOverviewMeshInfo(helper->GetPanoramaPtr(), &image, helper->GetVisualizationStatePtr(), false);
    DEBUG_DEBUG("End create mesh projection grid");
}

/**
 * create the texture by iterating through each pixela and checking how much each pixel should be filled
 */
bool ProjectionGridTool::createTexture()
{
    glGenTextures(1,(GLuint*) &texture_num);

    if(helper_g->GetViewStatePtr()->GetSupportMultiTexture())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_num);
    }
    else
        glBindTexture(GL_TEXTURE_2D, texture_num);


    GLint texSize; 
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
    int width_p = 12;
    int height_p = 11;
    int width = std::min(1 << width_p,texSize);
    int height = std::min(1 << height_p, texSize >> 1);

    int hor_lines = 20;
    int ver_lines = 40;

    double line_width_per = 0.1;

    unsigned char *image = new unsigned char[width * height * 4];
    unsigned char *pix_start = image;

    double dw = width / hor_lines;
    double dh = height / ver_lines;

    std::vector<double> hor_s(hor_lines*2);
    std::vector<double> ver_s(ver_lines*2);

    double line_width =  (dw < dh) ? line_width_per * dw : line_width_per * dh;

    //determine the exact position of the begining and the end of horizontal line
    for (int i = 0 ; i < hor_lines ; i++) {
        hor_s[2*i    ] = (i + 0.5) * dw - line_width / 2.0;
        hor_s[2*i + 1] = (i + 0.5) * dw + line_width / 2.0;
    }

    //determine the exact position of the begining and the end of horizontal line
    ver_s[0] = 0;
    for (int i = 0 ; i < ver_lines ; i++) {
        ver_s[2*i    ] = (i + 0.5) * dh - line_width / 2.0;
        ver_s[2*i + 1] = (i + 0.5) * dh + line_width / 2.0;
    }

    int w_i = -1;
    for (int x = 0 ; x < width ; x++) {
        double x_res = 0;
        int stw_i = w_i;
        //check how many lines this pixel crosses
        if(w_i<hor_lines*2-1)
        {
            while (
                    hor_s[w_i + 1] > x && 
                    hor_s[w_i + 1] <= x+1
            ) {
                w_i++;
                if (w_i == hor_lines * 2 - 1) break;
            }
        }
        //calculate the fill
        if (w_i != stw_i) {
            if (w_i - stw_i > 1) {
                int minus = 0;
                if (stw_i + 1 % 2 == 1) {
                    minus++;
                    x_res += (hor_s[stw_i] - x);
                }
                if (w_i % 2 == 0) {
                    minus++;
                    x_res += (x+1 - hor_s[w_i]);
                }
                x_res += (w_i - stw_i - minus) / 2 * line_width;
                
            } else {
                if (w_i % 2 == 0) {
                    x_res += (x+1 - hor_s[w_i]);
                } else {
                    x_res += (hor_s[w_i] - x);
                }
            }
        } else {
            if (w_i % 2 == 0) {
                x_res = 1;
            }
        }
        
        int h_i = -1;
        for (int y = 0 ; y < height ; y++) {

            double y_res = 0;
            int sth_i = h_i;
            if(h_i<ver_lines*2-1)
            {
                while (
                        ver_s[h_i + 1] > y && 
                        ver_s[h_i + 1] <= y+1
                ) {
                    h_i++;
                    if (h_i == ver_lines * 2 - 1) break;
                }
            }
            if (h_i != sth_i) {
                if (h_i - sth_i > 1) {
                    int minus = 0;
                    if (sth_i + 1 % 2 == 1) {
                        minus++;
                        y_res += (ver_s[sth_i] - y);
                    }
                    if (h_i % 2 == 0) {
                        minus++;
                        y_res += (y+1 - ver_s[h_i]);
                    }
                    y_res += (h_i - sth_i - minus) / 2 * line_width;
                    
                } else {
                    if (h_i % 2 == 0) {
                        y_res += (y+1 - ver_s[h_i]);
                    } else {
                        y_res += (ver_s[h_i] - y);
                    }
                }
            } else {
                if (h_i % 2 == 0) {
                    y_res = 1;
                }
            }

            //combine fills from horizontal and vertical lines
            double res = x_res + y_res - x_res * y_res;

            double x_rat = (double) x / (double) width;
            double y_rat = (double) y / (double) height;
            pix_start[0] = 255 - (res * 255.0 * (1 - x_rat));
            pix_start[1] = 255 - (res * 255.0 * (1 - y_rat));
            pix_start[2] = 255 - (res * 255.0 * (x_rat));
//            pix_start[0] = 255 - (res * 255.0 * (y_rat));
//            pix_start[1] = 255 - (res * 255.0 * (y_rat));
//            pix_start[2] = 255 - (res * 255.0 * (y_rat));
            pix_start[3] = 255 * res;
            pix_start += 4;
        }
    }


    bool has_error = false;
    GLint error;
       error = gluBuild2DMipmaps/*Levels*/(GL_TEXTURE_2D, GL_RGBA8, width, height,
                               GL_RGBA, GL_UNSIGNED_BYTE, /*min, min, max,*/
                               (unsigned char *) image);
    delete [] image;
    static bool checked_anisotropic = false;
    static bool has_anisotropic;
    static float anisotropy;
    if (!checked_anisotropic)
    {
        // check if it is supported
        if (GLEW_EXT_texture_filter_anisotropic)
        {
            has_anisotropic = true;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
            DEBUG_INFO("Using anisotropic filtering at maximum value "
                      << anisotropy);
        } else {
            has_anisotropic = false;
            DEBUG_INFO("Anisotropic filtering is not available.");
        }
        checked_anisotropic = true;
    }
    if (has_anisotropic)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                        anisotropy);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (error)
    {
        DEBUG_ERROR("GLU Error when building mipmap levels: "
                  << gluErrorString(error) << ".");
        has_error = true;
    }
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        DEBUG_ERROR("GL Error when building mipmap levels: "
                  << gluErrorString(error) << ".");
        has_error = true;
    }
    DEBUG_INFO("Finsihed loading texture.");
    if (has_error) {
        return false;
    } else {
        texture_created = true;
        return true;
    }
    
}