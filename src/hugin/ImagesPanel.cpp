// -*- c-basic-offset: 4 -*-

/** @file ImagesPanel.cpp
 *
 *  @brief implementation of ImagesPanel Class
 *
 *  @author Kai-Uwe Behrmann <web@tiscali.de>
 *
 *  $Id$
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

#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/xrc/xmlres.h>          // XRC XML resouces
//#include <wx/image.h>               // wxImage
//#include <wx/imagpng.h>             // for about html
#include <wx/listctrl.h>	// needed on mingw
#include <wx/imaglist.h>

#include "PT/PanoCommand.h"
#include "hugin/config.h"
#include "hugin/CommandHistory.h"
#include "hugin/ImageCache.h"
#include "hugin/CPEditorPanel.h"
#include "hugin/List.h"
#include "hugin/ImagesPanel.h"
#include "hugin/MainFrame.h"
#include "hugin/Events.h"
#include "hugin/huginApp.h"
#include "PT/Panorama.h"

#include "common/utils.h"
using namespace PT;
using namespace utils;

// Image Icons
wxImageList * img_icons;
// Image Icons biger
wxImageList * img_bicons;

// image preview
wxBitmap * p_img = (wxBitmap *) NULL;

ImgPreview * canvas;

//------------------------------------------------------------------------------
#define GET_VAR(val) pano.getVariable(orientationEdit_RefImg).val.getValue()

BEGIN_EVENT_TABLE(ImagesPanel, wxWindow)
//    EVT_MOUSE_EVENTS ( ImagesPanel::OnMouse )
//    EVT_MOTION ( ImagesPanel::ChangePreview )
    EVT_SLIDER ( XRCID("images_slider_yaw"),ImagesPanel::SetYaw )
    EVT_SLIDER ( XRCID("images_slider_pitch"),ImagesPanel::SetPitch )
    EVT_SLIDER ( XRCID("images_slider_roll"),ImagesPanel::SetRoll )
    EVT_TEXT_ENTER ( XRCID("images_text_yaw"), ImagesPanel::SetYawText )
    EVT_TEXT_ENTER ( XRCID("images_text_pitch"), ImagesPanel::SetPitchText )
    EVT_TEXT_ENTER ( XRCID("images_text_roll"), ImagesPanel::SetRollText )
//    EVT_SIZE   ( ImagesPanel::FitParent )
END_EVENT_TABLE()


// Define a constructor for the Images Panel
ImagesPanel::ImagesPanel(wxWindow *parent, const wxPoint& pos, const wxSize& size, Panorama* pano)
    : wxPanel (parent, -1, wxDefaultPosition, wxDefaultSize, wxEXPAND|wxGROW),
      pano(*pano)
{
    DEBUG_TRACE("");
// TODO ---------------------------------------------------------------------
/*    wxNotebook       *nb = XRCCTRL(*parent, "controls_notebook", wxNotebook);
    wxNotebookSizer  *nbs = new wxNotebookSizer( nb );
    nb->Add(nbs, 1, wxEXPAND | wxALL, 4);
    nb->Layout();

    wxBoxSizer *panelsizer = new wxBoxSizer( wxVERTICAL );
    SetSizer( panelsizer );*/


// TODO --------------------------------------------------------------------
    wxXmlResource::Get()->LoadPanel (this, wxT("images_panel"));
    DEBUG_TRACE("");

    images_list = new List (parent, pano, images_layout);
    wxXmlResource::Get()->AttachUnknownControl (
               wxT("images_list_unknown"),
               images_list );

    DEBUG_TRACE("");

    PushEventHandler( new MyEvtHandler((size_t) 3) );

    // Image Preview

    img_icons = new wxImageList(24,24);
    img_bicons = new wxImageList(128,128);
    images_list->AssignImageList(img_icons, wxIMAGE_LIST_SMALL );//_NORMAL );
//    images_list->AssignImageList(img_bicons, wxIMAGE_LIST_NORMAL );

    DEBUG_TRACE("");
    p_img = new wxBitmap( 0, 0 );
    wxPanel * img_p = XRCCTRL(*parent, "img_preview_unknown", wxPanel);
    wxPaintDC dc (img_p);

    DEBUG_TRACE("");
    canvas = new ImgPreview(img_p, wxPoint(0, 0), wxSize(128, 128));

    DEBUG_TRACE("end");
    pano->addObserver(this);

    for ( int i = 0 ; i < 512 ; i++ )
      imgNr[i] = 0;
 
    changePano = FALSE;

    DEBUG_TRACE("end");
}


ImagesPanel::~ImagesPanel(void)
{
    pano.removeObserver(this);
//    delete p_img; // Dont know how to check for existing
    p_img = (wxBitmap *) NULL;
    delete images_list;
    DEBUG_TRACE("");
}

/*void ImagesPanel::FitParent(wxSizeEvent & e)
{
    DEBUG_TRACE("");
//    wxSize new_size = XRCCTRL(*parent, "images_panel_top", wxPanel) ->GetSize();
    wxSize new_size = GetParent()->GetSize();
    GetParent()->SetClientSize ( new_size );  // no reaction
    //SetoLayout(TRUE);
    DEBUG_INFO( "" << new_size.GetWidth() <<"x"<< new_size.GetHeight()  );
}*/

//void ImagesPanel::panoramaChanged (PT::Panorama &pano)
void ImagesPanel::panoramaImagesChanged(PT::Panorama &pano, const PT::UIntSet & _imgNr)
{
    if ( !changePano ) {
      // TODO
      imgNr[0] = 0;

      //TODO be more sensible
      img_icons->RemoveAll();
      img_bicons->RemoveAll();
//    DEBUG_INFO( "after delete are " << wxString::Format("%d", img_icons->GetImageCount() ) << " images inside img_icons")
    // start the loop for every selected filename
      for ( int i = 0 ; i <= (int)pano.getNrOfImages() - 1 ; i++ ) {
        wxFileName fn = (wxString)pano.getImage(i).getFilename().c_str();

//        wxImage * img = ImageCache::getInstance().getImage(
//            pano.getImage(i).getFilename());

        // preview selected images
        wxImage * s_img = ImageCache::getInstance().getImageSmall(
            pano.getImage(i).getFilename());

        // right image preview
        wxImage r_img;
        // left list control big icon
        wxImage b_img;
        // left list control icon
        wxImage i_img;
        if ( s_img->GetHeight() > s_img->GetWidth() ) {
          r_img = s_img->Scale( (int)((float)s_img->GetWidth()/
                                      (float)s_img->GetHeight()*128.0),
                                128);
          b_img = s_img->Scale( (int)((float)s_img->GetWidth()/
                                      (float)s_img->GetHeight()*64.0),
                                64);
          i_img = s_img->Scale( (int)((float)s_img->GetWidth()/
                                      (float)s_img->GetHeight()*20.0),
                                20);
        } else {
          r_img = s_img->Scale( 128, 
                                (int)((float)s_img->GetHeight()/
                                      (float)s_img->GetWidth()*128.0));
          b_img = r_img.Scale( 64,
                                (int)((float)r_img.GetHeight()/
                                      (float)r_img.GetWidth()*64.0));
          i_img = r_img.Scale( 20,
                                (int)((float)r_img.GetHeight()/
                                      (float)r_img.GetWidth()*20.0));

        }
        delete p_img;
        p_img = new wxBitmap( r_img.ConvertToBitmap() );
        canvas->Refresh();

        img_icons->Add( i_img.ConvertToBitmap() );
        img_bicons->Add( r_img.ConvertToBitmap() );
//        DEBUG_INFO( "now " << wxString::Format("%d", img_icons->GetImageCount() ) << " images inside img_icons")

      }

      if ( pano.getNrOfImages() == 0 ) {
        delete p_img;
        p_img = new wxBitmap(0,0);
      }
      canvas->Refresh();
    }

    DEBUG_TRACE("");
}

void ImagesPanel::ChangePano ( std::string type, double var )
{
    changePano = TRUE;
    DEBUG_TRACE("");
// DEBUG_TRACE("imgNr = "<<imgNr[0]<<" "<<imgNr[1]<<" "<<imgNr[2]<<" "<<imgNr[3]);

    for ( unsigned int i = 1; imgNr[0] >= i ; i++ ) {
        ImageVariables new_var = pano.getVariable(imgNr[i]);
        if ( type == "roll" ) {
          new_var.roll.setValue(var);
        }
        if ( type == "pitch" ) {
          new_var.pitch.setValue(var);
        }
        if ( type == "yaw" ) {
          new_var.yaw.setValue(var);
        }
//        ImageVariables old_var = new_var;//pano.getVariable(imgNr[i]);
//    DEBUG_TRACE("roll = " << new_var.roll.getValue() );
//    DEBUG_TRACE("updateVariables(" << imgNr[i] <<",new_var)" );
        pano.updateVariables( imgNr[i], new_var ); 
//    DEBUG_TRACE("roll = " << pano.getVariable(imgNr[i]).roll.getValue() );

    }
//        DEBUG_INFO( wxString::Format("to remove: %d",imgNr[0]) );
//    DEBUG_TRACE("");
    GlobalCmdHist::getInstance().addCommand(
         new PT::UpdateVariablesCmd(pano, pano.getVariables())
         );
//        DEBUG_INFO( wxString::Format("removed %d/%d",imgNr[i], i) );

//    DEBUG_TRACE("roll = " << pano.getVariable(0).roll.getValue() );
}

void ImagesPanel::SetYaw ( wxCommandEvent & e )
{
    if ( imgNr[0] > 0 ) {
      int var = XRCCTRL(*this, "images_slider_yaw", wxSlider) ->GetValue();
      DEBUG_INFO ("yaw = " << var )

      char text[16];
      sprintf( text, "%d ", var );
      XRCCTRL(*this, "images_stext_orientation", wxStaticText) ->SetLabel(text);

      ChangePano ( "yaw" , (double) var );

      XRCCTRL(*this,"images_text_yaw",wxTextCtrl)->SetValue( doubleToString (
                                              (double) var ).c_str() );
    }
    DEBUG_INFO( wxString::Format("%d+%d+%d+%d+%d",imgNr[0], imgNr[1],imgNr[2], imgNr[3],imgNr[4]) );
}

void ImagesPanel::SetPitch ( wxCommandEvent & e )
{
    if ( imgNr[0] > 0 ) {
      int var = XRCCTRL(*this,"images_slider_pitch",wxSlider) ->GetValue() * -1;
      DEBUG_INFO ("pitch = " << var )

      char text[16];
      sprintf( text, "%d", var );
      XRCCTRL(*this, "images_stext_orientation", wxStaticText) ->SetLabel(text);

      ChangePano ( "pitch" , (double) var );

      XRCCTRL(*this,"images_text_pitch",wxTextCtrl)->SetValue( doubleToString (
                                              (double) var ).c_str() );
    }
    DEBUG_INFO( wxString::Format("%d+%d+%d+%d+%d",imgNr[0], imgNr[1],imgNr[2], imgNr[3],imgNr[4]) );
}

void ImagesPanel::SetRoll ( wxCommandEvent & e )
{
    if ( imgNr[0] > 0 ) {
      int var = XRCCTRL(*this, "images_slider_roll", wxSlider) ->GetValue();
      DEBUG_INFO ("roll = " << var )

      char text[16];
      sprintf( text, "%d", var );
      XRCCTRL(*this, "images_stext_roll", wxStaticText) ->SetLabel(text);
      DEBUG_INFO ("roll = " << var )

      ChangePano ( "roll" , (double) var );

      DEBUG_INFO ("roll = " << var )
      XRCCTRL(*this, "images_text_roll", wxTextCtrl)->SetValue( doubleToString (
                                              (double) var ).c_str() );
      DEBUG_INFO ("roll = " << var )
    }
    DEBUG_INFO( wxString::Format("%d+%d+%d+%d+%d",imgNr[0], imgNr[1],imgNr[2], imgNr[3],imgNr[4]) );
}

void ImagesPanel::SetYawText ( wxCommandEvent & e )
{
    if ( imgNr[0] > 0 ) {
      wxString text = XRCCTRL(*this, "images_text_yaw"
                  , wxTextCtrl) ->GetValue();
      DEBUG_INFO ("yaw = " << text )

      double * var = new double ();
      text.ToDouble (var);

      char c[48];
      sprintf ( c, "%d", (int) *var );
      XRCCTRL(*this,"images_stext_orientation",wxStaticText)->SetLabel(c);

      ChangePano ( "yaw" , *var );

      XRCCTRL(*this,"images_slider_yaw",wxSlider) ->SetValue( (int) *var );

      delete var;
    }
    DEBUG_INFO( wxString::Format("%d+%d+%d+%d+%d",imgNr[0], imgNr[1],imgNr[2], imgNr[3],imgNr[4]) );
}
void ImagesPanel::SetPitchText ( wxCommandEvent & e )
{
    if ( imgNr[0] > 0 ) {
      wxString text = XRCCTRL(*this, "images_text_pitch"
                  , wxTextCtrl) ->GetValue();
      DEBUG_INFO ("pitch = " << text )

      double * var = new double ();
      text.ToDouble (var);

      char c[48];
      sprintf ( c, "%d", (int) *var );
      XRCCTRL(*this,"images_stext_orientation",wxStaticText)->SetLabel(c);

      ChangePano ( "pitch" , *var );

      XRCCTRL(*this,"images_slider_pitch",wxSlider)->SetValue( (int) *var * -1);

      delete var;
    }
    DEBUG_INFO( wxString::Format("%d+%d+%d+%d+%d",imgNr[0], imgNr[1],imgNr[2], imgNr[3],imgNr[4]) );
}
void ImagesPanel::SetRollText ( wxCommandEvent & e )
{
    if ( imgNr[0] > 0 ) {
      wxString text = XRCCTRL(*this, "images_text_roll"
                  , wxTextCtrl) ->GetValue();
      DEBUG_INFO ("roll = " << text )

      double * var = new double ();
      text.ToDouble (var);

      char c[48];
      sprintf ( c, "%d", (int) *var );
      XRCCTRL(*this,"images_stext_roll",wxStaticText)->SetLabel(c);

      ChangePano ( "roll" , *var );

      XRCCTRL(*this,"images_slider_roll",wxSlider) ->SetValue( (int) *var );

      delete var;
    }
    DEBUG_INFO( wxString::Format("%d+%d+%d+%d+%d",imgNr[0], imgNr[1],imgNr[2], imgNr[3],imgNr[4]) );
}

/*void ImagesPanel::OrientationChanged ( void )
{
    DEBUG_TRACE("");
    XRCCTRL(*this, "images_list_roll", wxSlider)  ->SetValue(
         (int)pano.getVariable(orientationEdit_RefImg) .roll.getValue() );
    XRCCTRL(*this, "images_list_pitch", wxSlider) ->SetValue(
         (int)pano.getVariable(orientationEdit_RefImg) .pitch.getValue() * -1 );
    XRCCTRL(*this, "images_list_yaw", wxSlider)   ->SetValue(
         (int)pano.getVariable(orientationEdit_RefImg) .yaw.getValue() );
    XRCCTRL(*this, "images_text_orientation", wxStaticText) ->SetLabel("");
    XRCCTRL(*this, "images_text_roll", wxStaticText) ->SetLabel(text);

    std::string number;

    number = doubleToString(pano.getVariable(orientationEdit_RefImg) .pitch.getValue() * -1 );
    XRCCTRL(*this, "images_text_pitch", wxTextCtrl)->SetValue( number.c_str() );
    
    DEBUG_TRACE("");
}*/

void ImagesPanel::SetImages ( wxListEvent & e )
{
    DEBUG_TRACE("");
    if ( changePano == FALSE ) {

      // One image is our prefered image.
      orientationEdit_RefImg = e.GetIndex();

      // get the list to read from
      wxListCtrl* lst =  XRCCTRL(*this, "images_list_unknown", wxListCtrl);

      // prepare an status message
      wxString e_msg;
      int sel_I = lst->GetSelectedItemCount();
      if ( sel_I == 1 )
        e_msg = _("Remove image:   ");
      else
        e_msg = _("Remove images:   ");

      imgNr[0] = 0;             // reset
      for ( int Nr=pano.getNrOfImages()-1 ; Nr>=0 ; --Nr ) {
//      DEBUG_INFO( wxString::Format("now test if removing item %d/%d",Nr,pano.getNrOfImages()) );
        if ( lst->GetItemState( Nr, wxLIST_STATE_SELECTED ) ) {
//    DEBUG_TRACE("");
          e_msg += "  " + lst->GetItemText(Nr);
          imgNr[0] += 1;
          imgNr[imgNr[0]] = Nr; //(unsigned int)Nr;
        }
      }
      DEBUG_INFO (e_msg)

      // values to set
      XRCCTRL(*this, "images_text_yaw", wxTextCtrl)->SetValue(doubleToString (
                                                     GET_VAR( yaw )).c_str());
      XRCCTRL(*this, "images_slider_yaw" , wxSlider)->SetValue(
                                                (int)GET_VAR( yaw ));
      XRCCTRL(*this, "images_text_pitch", wxTextCtrl)->SetValue(doubleToString (
                                                     GET_VAR( pitch )).c_str());
      XRCCTRL(*this, "images_slider_pitch" , wxSlider)->SetValue(
                                                (int)GET_VAR( pitch ) * -1);
      XRCCTRL(*this, "images_text_roll", wxTextCtrl)->SetValue(doubleToString (
                                                     GET_VAR( roll )).c_str());
      XRCCTRL(*this, "images_slider_roll" , wxSlider)->SetValue(
                                                (int)GET_VAR( roll ));

      XRCCTRL(*this, "images_stext_orientation", wxStaticText) ->SetLabel("");
      XRCCTRL(*this, "images_stext_roll", wxStaticText) ->SetLabel("");
//    OrientationChanged ();

    }
    DEBUG_INFO( wxString::Format("%d+%d+%d+%d+%d",imgNr[0], imgNr[1],imgNr[2], imgNr[3],imgNr[4]) );

    changePano = FALSE;
//    DEBUG_TRACE("");
}

//------------------------------------------------------------------------------

BEGIN_EVENT_TABLE(ImgPreview, wxScrolledWindow)
    //EVT_PAINT(ImgPreview::OnPaint)
//    EVT_MOTION ( ImagesPanel::ChangePreview )
END_EVENT_TABLE()

// Define a constructor for my canvas
ImgPreview::ImgPreview(wxWindow *parent, const wxPoint& pos, const wxSize& size):
 wxScrolledWindow(parent, -1, pos, size)
{
    DEBUG_TRACE("");
}

ImgPreview::~ImgPreview(void)
{
    p_img = (wxBitmap *) NULL;
    DEBUG_TRACE("");
}

// Define the repainting behaviour
void ImgPreview::OnDraw(wxDC & dc)
{
  if ( p_img && p_img->Ok() )
  {
    wxMemoryDC memDC;
    memDC.SelectObject(* p_img);

    // Transparent blitting if there's a mask in the bitmap
    dc.Blit(0, 0, p_img->GetWidth(), p_img->GetHeight(), & memDC,
      0, 0, wxCOPY, TRUE);

    //img_icons->Draw( 0, dc, 0, 0, wxIMAGELIST_DRAW_NORMAL, FALSE);

    memDC.SelectObject(wxNullBitmap);
  }
}

void ImgPreview::ChangePreview ( long item )
{
//    wxPoint pos = e.GetPosition();
//    long item = HitTest( e.m_x ,e.m_y );
//    DEBUG_INFO ( "hier:" << wxString::Format(" %d is item %ld", e.GetPosition(), item) );
    DEBUG_INFO ( "hier: is item " << wxString::Format("%ld", item) );
}



