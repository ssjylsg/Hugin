// -*- c-basic-offset: 4 -*-

/** @file LensPanel.cpp
 *
 *  @brief implementation of LensPanel Class
 *
 *  @author Kai-Uwe Behrmann <web@tiscali.de> and
 *          Pablo d'Angelo <pablo.dangelo@web.de>
 *
 *  Rewritten by Pablo d'Angelo
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

#include <config.h>
#include "panoinc_WX.h"

#include "panoinc.h"

#include <algorithm>

#include "jhead/jhead.h"

#include "common/wxPlatform.h"
#include "hugin/LensPanel.h"
#include "hugin/CommandHistory.h"
#include "hugin/ImageCache.h"
#include "hugin/CPEditorPanel.h"
#include "hugin/ImagesList.h"
//#include "hugin/ImageCenter.h"
#include "hugin/ImagesPanel.h"
#include "hugin/MainFrame.h"
#include "hugin/huginApp.h"
#include "hugin/TextKillFocusHandler.h"
#include "hugin/wxPanoCommand.h"
//#include "hugin/VigCorrDialog.h"


using namespace PT;
using namespace utils;
using namespace std;

#define m_XRCID(str_id) \
    wxXmlResource::GetXRCID(str_id)

#ifdef __WXDEBUG__
#define m_XRCCTRL(window, id, type) \
    (wxDynamicCast((window).FindWindow(m_XRCID(id)), type))
#else
#define m_XRCCTRL(window, id, type) \
    ((type*)((window).FindWindow(m_XRCID(id))))
#endif

//------------------------------------------------------------------------------

BEGIN_EVENT_TABLE(LensPanel, wxWindow) //wxEvtHandler)
    EVT_SIZE   ( LensPanel::OnSize )
    EVT_LIST_ITEM_SELECTED( XRCID("lenses_list_unknown"),
                            LensPanel::ListSelectionChanged )
    EVT_LIST_ITEM_DESELECTED( XRCID("lenses_list_unknown"),
                              LensPanel::ListSelectionChanged )
    EVT_CHOICE (XRCID("lens_val_projectionFormat"),LensPanel::LensTypeChanged)
    EVT_CHOICE (XRCID("lens_val_responseType"),LensPanel::ResponseTypeChanged)
    EVT_TEXT_ENTER ( XRCID("lens_val_v"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_focalLength"),LensPanel::focalLengthChanged)
    EVT_TEXT_ENTER ( XRCID("lens_val_flFactor"),LensPanel::focalLengthFactorChanged)
    EVT_TEXT_ENTER ( XRCID("lens_val_a"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_b"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_c"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_d"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_e"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_g"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_t"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Eev"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Er"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Eb"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Vb"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Vc"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Vd"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Vx"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Vy"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Ra"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Rb"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Rc"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Rd"), LensPanel::OnVarChanged )
    EVT_TEXT_ENTER ( XRCID("lens_val_Re"), LensPanel::OnVarChanged )
//    EVT_BUTTON ( XRCID("lens_button_center"), LensPanel::SetCenter )
    EVT_BUTTON ( XRCID("lens_button_loadEXIF"), LensPanel::OnReadExif )
    EVT_BUTTON ( XRCID("lens_button_save"), LensPanel::OnSaveLensParameters )
    EVT_BUTTON ( XRCID("lens_button_load"), LensPanel::OnLoadLensParameters )
    EVT_BUTTON ( XRCID("lens_button_newlens"), LensPanel::OnNewLens )
    EVT_BUTTON ( XRCID("lens_button_changelens"), LensPanel::OnChangeLens )
    EVT_CHECKBOX ( XRCID("lens_inherit_v"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_a"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_b"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_c"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_d"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_e"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_g"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_t"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_Eev"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_Er"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_Eb"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_R"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_Vb"), LensPanel::OnVarInheritChanged )
    EVT_CHECKBOX ( XRCID("lens_inherit_Vx"), LensPanel::OnVarInheritChanged )
END_EVENT_TABLE()

// Define a constructor for the Lenses Panel

LensPanel::LensPanel(wxWindow *parent, const wxPoint& pos, const wxSize& size, Panorama* pano)
    : wxPanel (parent, -1, wxDefaultPosition, wxDefaultSize, wxEXPAND|wxGROW),
      pano(*pano), m_restoreLayoutOnResize(false)
{
    DEBUG_TRACE("ctor");
    pano->addObserver(this);

    // This controls must called by xrc handler and after it we play with it.
    wxXmlResource::Get()->LoadPanel (this, wxT("lens_panel"));

    // The following control creates itself. We dont care about xrc loading.
    images_list = new ImagesListLens (parent, pano);
    wxXmlResource::Get()->AttachUnknownControl (
        wxT("lenses_list_unknown"),
        images_list );
//    images_list->AssignImageList(img_icons, wxIMAGE_LIST_SMALL );

    // converts KILL_FOCUS events to usable TEXT_ENTER events
    XRCCTRL(*this, "lens_val_v", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_focalLength", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_flFactor", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_a", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_b", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_c", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_d", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_e", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_g", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_t", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));

    XRCCTRL(*this, "lens_val_Eev", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_Er", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_Eb", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_Vb", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_Vc", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_Vd", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_Vx", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_Vy", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_Ra", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_Rb", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_Rc", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_Rd", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));
    XRCCTRL(*this, "lens_val_Re", wxTextCtrl)->PushEventHandler(new TextKillFocusHandler(this));

    m_degDigits = wxConfigBase::Get()->Read(wxT("/General/DegreeFractionalDigitsEdit"),3);
    m_pixelDigits = wxConfigBase::Get()->Read(wxT("/General/PixelFractionalDigitsEdit"),2);
    m_distDigitsEdit = wxConfigBase::Get()->Read(wxT("/General/DistortionFractionalDigitsEdit"),5);

#ifdef USE_WX253
    m_lens_ctrls = XRCCTRL(*this, "lens_control_panel", wxScrolledWindow);
    DEBUG_ASSERT(m_lens_ctrls);
    m_lens_splitter = XRCCTRL(*this, "lens_panel_splitter", wxSplitterWindow);
    DEBUG_ASSERT(m_lens_splitter);

    m_lens_ctrls->FitInside();
    m_lens_ctrls->SetScrollRate(10, 10);
#ifdef USE_WX26x
    // resize only the images list, and keep the control parameters at the same size
    m_lens_splitter->SetSashGravity(1);
#endif
    m_lens_splitter->SetMinimumPaneSize(200);
#endif

    // dummy to disable controls
    wxListEvent ev;
    ListSelectionChanged(ev);
    DEBUG_TRACE("");;
}


LensPanel::~LensPanel(void)
{
    DEBUG_TRACE("dtor");

#ifdef USE_WX253
    int sashPos;
    sashPos = m_lens_splitter->GetSashPosition();
    DEBUG_INFO("Lens panel sash pos: " << sashPos);
    wxConfigBase::Get()->Write(wxT("/LensFrame/sashPos"), sashPos);
#endif

    XRCCTRL(*this, "lens_val_v", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_focalLength", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_flFactor", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_a", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_b", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_c", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_d", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_e", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_g", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_t", wxTextCtrl)->PopEventHandler(true);

    XRCCTRL(*this, "lens_val_Eev", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_Er", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_Eb", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_Vb", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_Vc", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_Vd", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_Vx", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_Vy", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_Ra", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_Rb", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_Rc", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_Rd", wxTextCtrl)->PopEventHandler(true);
    XRCCTRL(*this, "lens_val_Re", wxTextCtrl)->PopEventHandler(true);

    pano.removeObserver(this);
    DEBUG_TRACE("dtor about to finish");
}

void LensPanel::RestoreLayout()
{
	DEBUG_TRACE("");
#ifdef USE_WX253
    int winWidth, winHeight;
    GetClientSize(&winWidth, &winHeight);
    int sP = wxConfigBase::Get()->Read(wxT("/LensFrame/sashPos"),winHeight/2);
    m_lens_splitter->SetSashPosition(sP);
    DEBUG_INFO( "lens panel: " << winWidth <<"x"<< winHeight << " sash pos: " << sP);
#endif
}

// We need to override the default handling of size events because the
// sizers set the virtual size but not the actual size. We reverse
// the standard handling and fit the child to the parent rather than
// fitting the parent around the child

void LensPanel::OnSize( wxSizeEvent & e )
{
#ifdef USE_WX253
    int winWidth, winHeight;
    GetClientSize(&winWidth, &winHeight);
    XRCCTRL(*this, "lens_panel", wxPanel)->SetSize (winWidth, winHeight);
    DEBUG_INFO( "lens panel: " << winWidth <<"x"<< winHeight );
    m_lens_splitter->SetSize( winWidth, winHeight );
    m_lens_splitter->GetWindow2()->GetSize(&winWidth, &winHeight);
    DEBUG_INFO( "lens controls: " << winWidth <<"x"<< winHeight );
    m_lens_ctrls->SetSize(winWidth, winHeight);
#else
    wxSize new_size = GetSize();
    XRCCTRL(*this, "lens_panel", wxPanel)->SetSize ( new_size );
    DEBUG_INFO( "" << new_size.GetWidth() <<"x"<< new_size.GetHeight()  );
#endif
    
    if (m_restoreLayoutOnResize) {
        m_restoreLayoutOnResize = false;
        RestoreLayout();
    }
}

void LensPanel::UpdateLensDisplay ()
{
    DEBUG_TRACE("");
    if (m_selectedLenses.size() == 0) {
        // no lens selected
        return;
    }

    if (m_selectedImages.size() == 0) {
        // no image selected
        return;
    }
    if (m_selectedImages.size() != 1) {
        // multiple images selected. do not update,
        // we cant display useful values, because they
        // might be different for each image
        return;
    }

    const Lens & lens = pano.getLens(*(m_selectedLenses.begin()));
    const VariableMap & imgvars = pano.getImageVariables(*m_selectedImages.begin());

    // update gui
    int guiPF = XRCCTRL(*this, "lens_val_projectionFormat",
                      wxChoice)->GetSelection();
    if (lens.getProjection() != (Lens::LensProjectionFormat) guiPF) {
        DEBUG_DEBUG("changing projection format in gui to: " << lens.getProjection());
        XRCCTRL(*this, "lens_val_projectionFormat", wxChoice)->SetSelection(
            lens.getProjection()  );
    }

    // set response type
    XRCCTRL(*this, "lens_val_responseType", wxChoice)->SetSelection(
            pano.getImage(*m_selectedImages.begin()).getOptions().responseType);

    for (char** varname = m_varNames; *varname != 0; ++varname) {
        // update parameters
        int ndigits = m_distDigitsEdit;
        if (strcmp(*varname, "hfov") == 0 || strcmp(*varname, "d") == 0 ||
            strcmp(*varname, "e") == 0 )
        {
            ndigits = m_pixelDigits;
        }
        m_XRCCTRL(*this, wxString(wxT("lens_val_")).append(wxString(*varname, *wxConvCurrent)), wxTextCtrl)->SetValue(
            doubleTowxString(const_map_get(imgvars,*varname).getValue(),ndigits));

        bool linked = const_map_get(lens.variables, *varname).isLinked();
        // special case for exposure and response parameters.
        if ((*varname)[0] == 'R' ) {
            m_XRCCTRL(*this, wxT("lens_inherit_R"), wxCheckBox)->SetValue(linked);
        } else if ((*varname)[0] == 'V') {
            if ((*varname)[1] == 'b' || (*varname)[1] == 'x') {
                m_XRCCTRL(*this, wxString(wxT("lens_inherit_")).append(wxString(*varname, *wxConvCurrent)), wxCheckBox)->SetValue(linked);
            }
        } else {
            m_XRCCTRL(*this, wxString(wxT("lens_inherit_")).append(wxString(*varname, *wxConvCurrent)), wxCheckBox)->SetValue(linked);
        }
    }

    // update focal length
    double focal_length = lens.getFocalLength();
    m_XRCCTRL(*this, wxT("lens_val_focalLength"), wxTextCtrl)->SetValue(
        doubleTowxString(focal_length,m_distDigitsEdit));

    // update focal length factor
    double focal_length_factor = lens.getCropFactor();
    m_XRCCTRL(*this, wxT("lens_val_flFactor"), wxTextCtrl)->SetValue(
        doubleTowxString(focal_length_factor,m_distDigitsEdit));


    DEBUG_TRACE("");
}

void LensPanel::panoramaImagesChanged (PT::Panorama &pano, const PT::UIntSet & imgNr)
{
    // rebuild lens selection, in case a selected lens has been removed.
    m_selectedLenses.clear();
    for (UIntSet::iterator it = m_selectedImages.begin();
         it != m_selectedImages.end(); it++)
    {
        // need to check, since the m_selectedImages list might still be in an old state
        if (*it < pano.getNrOfImages()) {
            unsigned int lNr = pano.getImage(*it).getLensNr();
            m_selectedLenses.insert(lNr);
        }
    }
    // we need to do something if the image we are editing has changed.
    UIntSet intersection;

    std::set_intersection(m_selectedLenses.begin(), m_selectedLenses.end(),
                          imgNr.begin(), imgNr.end(),
                          inserter(intersection, intersection.begin()));
    if (intersection.size() > 0) {
        UpdateLensDisplay();
    }
}


// Here we change the pano.
void LensPanel::LensTypeChanged ( wxCommandEvent & e )
{
    DEBUG_TRACE ("");
    if (m_selectedLenses.size() > 0) {
        for (UIntSet::iterator it = m_selectedLenses.begin();
             it != m_selectedLenses.end(); ++it)
        {
            // get lens from pano
            unsigned int lNr = *it;
            Lens lens = pano.getLens(lNr);
            // uses enum Lens::LensProjectionFormat from PanoramaMemento.h
            int var = XRCCTRL(*this, "lens_val_projectionFormat",
                              wxChoice)->GetSelection();
            if (lens.getProjection() != (Lens::LensProjectionFormat) var) {
                lens.setProjection((Lens::LensProjectionFormat) (var));
                GlobalCmdHist::getInstance().addCommand(
                    new PT::ChangeLensCmd( pano, *it, lens )
                    );
                DEBUG_INFO ("lens " << *it << " Lenstype " << var);
            }
        }
    }
}

void LensPanel::ResponseTypeChanged ( wxCommandEvent & e )
{
    DEBUG_TRACE ("");
    if (m_selectedLenses.size() > 0) {
        std::vector<ImageOptions> opts;
        UIntSet imgs;
        for (size_t i = 0 ; i < pano.getNrOfImages(); i++) {
            if ( set_contains(m_selectedLenses,pano.getImage(i).getLensNr()) ) {
                imgs.insert(i);
                ImageOptions opt = pano.getImage(i).getOptions();
                opt.responseType = e.GetSelection();
                opts.push_back(opt);
            }
        }
        GlobalCmdHist::getInstance().addCommand(
                new PT::UpdateImageOptionsCmd( pano, opts, imgs )
                                               );
    }
}

void LensPanel::focalLengthChanged ( wxCommandEvent & e )
{
    if (m_selectedImages.size() > 0) {
        DEBUG_TRACE ("");
        double val;
        wxString text=XRCCTRL(*this,"lens_val_focalLength",wxTextCtrl)->GetValue();
        if (!str2double(text, val)) {
            return;
        }


        VariableMapVector vars;
        UIntSet lensNrs;
        for (UIntSet::const_iterator it=m_selectedImages.begin();
             it != m_selectedImages.end();
             ++it)
        {
            vars.push_back(pano.getImageVariables(*it));
            Lens l = pano.getLens(pano.getImage(*it).getLensNr());
            l.setFocalLength(val);
            map_get(vars.back(),"v").setValue( map_get(l.variables,"v").getValue() );
        }

        GlobalCmdHist::getInstance().addCommand(
            new PT::UpdateImagesVariablesCmd(pano, m_selectedImages, vars)
            );
    }
}

void LensPanel::focalLengthFactorChanged(wxCommandEvent & e)
{
    DEBUG_TRACE ("");
    if (m_selectedImages.size() > 0) {
        wxString text=XRCCTRL(*this,"lens_val_flFactor",wxTextCtrl)->GetValue();
        DEBUG_INFO("focal length factor: " << text.mb_str());
        double val;
        if (!str2double(text, val)) {
            return;
        }

        UIntSet lensNrs;

        for (UIntSet::const_iterator it=m_selectedImages.begin();
             it != m_selectedImages.end();
             ++it)
        {
            lensNrs.insert(pano.getImage(*it).getLensNr());
        }

        vector<Lens> lenses;
        for (UIntSet::const_iterator it=lensNrs.begin(); it != lensNrs.end();
             ++it)
        {
            lenses.push_back(pano.getLens(*it));
            double fl = lenses.back().getFocalLength();
            lenses.back().setCropFactor(val);
            lenses.back().setFocalLength(fl);
        }
        GlobalCmdHist::getInstance().addCommand(
            new PT::ChangeLensesCmd( pano, lensNrs, lenses)
            );
    }
}


void LensPanel::OnVarChanged(wxCommandEvent & e)
{
    DEBUG_TRACE("")
    if (m_selectedImages.size() > 0) {
        string varname;
        DEBUG_TRACE (" var changed for control with id:" << e.GetId());
        if (e.GetId() == XRCID("lens_val_a")) {
            varname = "a";
        } else if (e.GetId() == XRCID("lens_val_b")) {
            varname = "b";
        } else if (e.GetId() == XRCID("lens_val_c")) {
            varname = "c";
        } else if (e.GetId() == XRCID("lens_val_d")) {
            varname = "d";
        } else if (e.GetId() == XRCID("lens_val_e")) {
            varname = "e";
        } else if (e.GetId() == XRCID("lens_val_g")) {
            varname = "g";
        } else if (e.GetId() == XRCID("lens_val_t")) {
            varname = "t";
        } else if (e.GetId() == XRCID("lens_val_v")) {
            varname = "v";
        } else if (e.GetId() == XRCID("lens_val_Eev")) {
            varname = "Eev";
        } else if (e.GetId() == XRCID("lens_val_Er")) {
            varname = "Er";
        } else if (e.GetId() == XRCID("lens_val_Eb")) {
            varname = "Eb";
        } else if (e.GetId() == XRCID("lens_val_Vb")) {
            varname = "Vb";
        } else if (e.GetId() == XRCID("lens_val_Vc")) {
            varname = "Vc";
        } else if (e.GetId() == XRCID("lens_val_Vd")) {
            varname = "Vd";
        } else if (e.GetId() == XRCID("lens_val_Vx")) {
            varname = "Vx";
        } else if (e.GetId() == XRCID("lens_val_Vy")) {
            varname = "Vy";
        } else if (e.GetId() == XRCID("lens_val_Ra")) {
            varname = "Ra";
        } else if (e.GetId() == XRCID("lens_val_Rb")) {
            varname = "Rb";
        } else if (e.GetId() == XRCID("lens_val_Rc")) {
            varname = "Rc";
        } else if (e.GetId() == XRCID("lens_val_Rd")) {
            varname = "Rd";
        } else if (e.GetId() == XRCID("lens_val_Re")) {
            varname = "Re";
        } else {
            // not reachable
            DEBUG_ASSERT(0);
        }

        wxString ctrl_name(wxT("lens_val_"));
        ctrl_name.append(wxString(varname.c_str(), *wxConvCurrent));
        double val;
        wxString text = m_XRCCTRL(*this, ctrl_name, wxTextCtrl)->GetValue();
        DEBUG_DEBUG("setting variable " << varname << " to " << text);
        if (!str2double(text, val)){
            return;
        }
        Variable var(varname,val);
        GlobalCmdHist::getInstance().addCommand(
            new PT::SetVariableCmd(pano, m_selectedImages, var)
            );
    }
}

void LensPanel::OnVarInheritChanged(wxCommandEvent & e)
{
    if (m_selectedLenses.size() > 0) {
        DEBUG_TRACE ("");
        std::vector<std::string> varnames;
        const char * ctrlName;
        if (e.GetId() == XRCID("lens_inherit_a")) {
            ctrlName = "lens_inherit_a";
            varnames.push_back("a");
        } else if (e.GetId() == XRCID("lens_inherit_b")) {
            ctrlName = "lens_inherit_b";
            varnames.push_back("b");
        } else if (e.GetId() == XRCID("lens_inherit_c")) {
            ctrlName = "lens_inherit_c";
            varnames.push_back("c");
        } else if (e.GetId() == XRCID("lens_inherit_d")) {
            ctrlName = "lens_inherit_d";
            varnames.push_back("d");
        } else if (e.GetId() == XRCID("lens_inherit_e")) {
            ctrlName = "lens_inherit_e";
            varnames.push_back("e");
        } else if (e.GetId() == XRCID("lens_inherit_g")) {
            ctrlName = "lens_inherit_g";
            varnames.push_back("g");
        } else if (e.GetId() == XRCID("lens_inherit_t")) {
            ctrlName = "lens_inherit_t";
            varnames.push_back("t");
        } else if (e.GetId() == XRCID("lens_inherit_v")) {
            ctrlName = "lens_inherit_v";
            varnames.push_back("v");
        } else if (e.GetId() == XRCID("lens_inherit_Vb")) {
            ctrlName = "lens_inherit_Vb";
            varnames.push_back("Va");
            varnames.push_back("Vb");
            varnames.push_back("Vc");
            varnames.push_back("Vd");
        } else if (e.GetId() == XRCID("lens_inherit_Vx")) {
            ctrlName = "lens_inherit_Vx";
            varnames.push_back("Vx");
            varnames.push_back("Vy");
        } else if (e.GetId() == XRCID("lens_inherit_R")) {
            ctrlName = "lens_inherit_R";
            varnames.push_back("Ra");
            varnames.push_back("Rb");
            varnames.push_back("Rc");
            varnames.push_back("Rd");
            varnames.push_back("Re");
        } else if (e.GetId() == XRCID("lens_inherit_Eev")) {
            ctrlName = "lens_inherit_Eev";
            varnames.push_back("Eev");
        } else if (e.GetId() == XRCID("lens_inherit_Er")) {
            ctrlName = "lens_inherit_Er";
            varnames.push_back("Er");
        } else if (e.GetId() == XRCID("lens_inherit_Eb")) {
            ctrlName = "lens_inherit_Eb";
            varnames.push_back("Eb");
        } else {
            // not reachable
            DEBUG_ASSERT(0);
        }

        bool inherit = e.IsChecked();
        for (UIntSet::iterator it = m_selectedLenses.begin();
             it != m_selectedLenses.end(); ++it)
        {
            // get the current Lens.
            unsigned int lensNr = *it;
            LensVarMap lmap;
            for (unsigned i=0; i < varnames.size(); i++) {
                LensVariable lv = const_map_get(pano.getLens(lensNr).variables, varnames[i]);
                lv.setLinked(inherit);
                lmap.insert(make_pair(lv.getName(),lv));
            }
            GlobalCmdHist::getInstance().addCommand(
                new PT::SetLensVariableCmd(pano, *it, lmap)
                );
        }
    }
}


void LensPanel::EditVigCorr ( wxCommandEvent & e )
{
    if (m_selectedImages.size() > 0) {
//        VigCorrDialog *dlg = new VigCorrDialog(this, pano, *(m_selectedImages.begin()));
//        dlg->Show();
    }
}

void LensPanel::ListSelectionChanged(wxListEvent& e)
{
    DEBUG_TRACE(e.GetIndex());
    m_selectedImages = images_list->GetSelected();
    m_selectedLenses.clear();
    for (UIntSet::iterator it = m_selectedImages.begin();
         it != m_selectedImages.end(); it++)
    {
        m_selectedLenses.insert(pano.getImage(*it).getLensNr());
    }
    DEBUG_DEBUG("selected Images: " << m_selectedImages.size());
    if (m_selectedImages.size() == 0) {
//        m_editImageNr = UINT_MAX;
//        m_editLensNr = UINT_MAX;
        DEBUG_DEBUG("no selection, disabling value display");
        // clear & disable display
        XRCCTRL(*this, "lens_val_projectionFormat", wxChoice)->Disable();
        XRCCTRL(*this, "lens_val_v", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_focalLength", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_flFactor", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_a", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_b", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_c", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_d", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_e", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_g", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_t", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Eev", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Er", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Eb", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Vb", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Vc", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Vd", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Vx", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Vy", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Ra", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Rb", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Rc", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Rd", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_val_Re", wxTextCtrl)->Disable();
        XRCCTRL(*this, "lens_inherit_v", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_a", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_b", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_c", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_d", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_e", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_g", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_t", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_Eev", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_Er", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_Eb", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_Vb", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_Vx", wxCheckBox)->Disable();
        XRCCTRL(*this, "lens_inherit_R", wxCheckBox)->Disable();

        XRCCTRL(*this, "lens_button_loadEXIF", wxButton)->Disable();
        XRCCTRL(*this, "lens_button_load", wxButton)->Disable();
        XRCCTRL(*this, "lens_button_save", wxButton)->Disable();
        XRCCTRL(*this, "lens_button_newlens", wxButton)->Disable();
        XRCCTRL(*this, "lens_button_changelens", wxButton)->Disable();
    } else {
//        m_editImageNr = *sel.begin();

        // one or more images selected
        if (XRCCTRL(*this, "lens_val_projectionFormat", wxChoice)->Enable()) {
            // enable all other textboxes as well.
            XRCCTRL(*this, "lens_val_v", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_focalLength", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_flFactor", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_a", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_b", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_c", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_d", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_e", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_g", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_t", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Eev", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Er", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Eb", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Vb", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Vc", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Vd", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Vx", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Vy", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Ra", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Rb", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Rc", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Rd", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_val_Re", wxTextCtrl)->Enable();
            XRCCTRL(*this, "lens_inherit_v", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_a", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_b", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_c", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_d", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_e", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_g", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_t", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_Eev", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_Er", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_Eb", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_R", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_Vb", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_inherit_Vx", wxCheckBox)->Enable();
            XRCCTRL(*this, "lens_button_loadEXIF", wxButton)->Enable();
            XRCCTRL(*this, "lens_button_newlens", wxButton)->Enable();
            XRCCTRL(*this, "lens_button_changelens", wxButton)->Enable();
        }

        if (m_selectedImages.size() == 1) {
            // single selection, its parameters
            // update values
            unsigned int img = *(m_selectedImages.begin());
            DEBUG_DEBUG("updating LensPanel with Image " << img);
            XRCCTRL(*this, "lens_button_load", wxButton)->Enable();
            XRCCTRL(*this, "lens_button_save", wxButton)->Enable();
            UpdateLensDisplay();
        } else {
            XRCCTRL(*this, "lens_val_v", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_focalLength", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_flFactor", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_a", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_b", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_c", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_d", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_e", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_g", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_t", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Eev", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Er", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Eb", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Vb", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Vc", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Vd", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Vx", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Vy", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Ra", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Rb", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Rc", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Rd", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_val_Re", wxTextCtrl)->Clear();
            XRCCTRL(*this, "lens_inherit_v", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_a", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_b", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_c", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_d", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_e", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_g", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_t", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_Eev", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_Er", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_Eb", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_Vb", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_Vx", wxCheckBox)->SetValue(false);
            XRCCTRL(*this, "lens_inherit_R", wxCheckBox)->SetValue(false);

            XRCCTRL(*this, "lens_button_load", wxButton)->Disable();
            XRCCTRL(*this, "lens_button_save", wxButton)->Disable();
        }
    }
}


void LensPanel::OnReadExif(wxCommandEvent & e)
{
    DEBUG_TRACE("");
    if (m_selectedImages.size() > 0) {
        for (UIntSet::iterator it = m_selectedImages.begin();
             it != m_selectedImages.end(); ++it)
        {
            unsigned int imgNr = *it;
            // check file extension
            wxFileName file(wxString(pano.getImage(imgNr).getFilename().c_str(), *wxConvCurrent));
            if (file.GetExt().CmpNoCase(wxT("jpg")) == 0 ||
                file.GetExt().CmpNoCase(wxT("jpeg")) == 0 )
            {
                double cropFactor = 0;
                double focalLength = 0;
                SrcPanoImage srcImg = pano.getSrcImage(imgNr);
                bool ok = initImageFromFile(srcImg, focalLength, cropFactor);
                if (! ok) {
                    getLensDataFromUser(this, srcImg, focalLength, cropFactor);
                }
                //initLensFromFile(pano.getImage(imgNr).getFilename().c_str(), c, lens, vars, imgopts, true);
                GlobalCmdHist::getInstance().addCommand(
                        new PT::UpdateSrcImageCmd( pano, imgNr, srcImg)
                    );
            } else {
                wxLogError(_("Not a jpeg file:") + file.GetName());
            }
        }
    } else {
        wxLogError(_("Please select an image and try again"));
    }

}



void LensPanel::OnSaveLensParameters(wxCommandEvent & e)
{
    DEBUG_TRACE("")
    if (m_selectedImages.size() == 1) {
        unsigned int imgNr = *(m_selectedImages.begin());
        unsigned int lensNr = pano.getImage(imgNr).getLensNr();
        const Lens & lens = pano.getLens(lensNr);
        const VariableMap & vars = pano.getImageVariables(imgNr);
        // get the variable map
        wxString fname;
        wxFileDialog dlg(this,
                         _("Save lens parameters file"),
                         wxConfigBase::Get()->Read(wxT("lensPath"),wxT("")), wxT(""),
                         _("Lens Project Files (*.ini)|*.ini|All files (*)|*"),
                         wxSAVE, wxDefaultPosition);
        if (dlg.ShowModal() == wxID_OK) {
            fname = dlg.GetPath();
            if (fname.Right(4) != wxT(".ini")) {
                fname.Append(wxT(".ini"));
            }
            wxConfig::Get()->Write(wxT("lensPath"), dlg.GetDirectory());  // remember for later
            // set numeric locale to C, for correct number output
            char * old_locale = setlocale(LC_NUMERIC,NULL);
            setlocale(LC_NUMERIC,"C");
            {
                wxFileConfig cfg(wxT("hugin lens file"),wxT(""),fname);
                cfg.Write(wxT("Lens/image_width"), (long) lens.getImageSize().x);
                cfg.Write(wxT("Lens/image_height"), (long) lens.getImageSize().y);
                cfg.Write(wxT("Lens/type"), (long) lens.getProjection());
                cfg.Write(wxT("Lens/hfov"), const_map_get(vars,"v").getValue());
                cfg.Write(wxT("Lens/hfov_link"), const_map_get(lens.variables,"v").isLinked() ? 1:0);
                cfg.Write(wxT("Lens/crop"), lens.getCropFactor());

                // loop to save lens variables
                char ** varname = Lens::variableNames;
                while (*varname) {
                    wxString key(wxT("Lens/"));
                    key.append(wxString(*varname, *wxConvCurrent));
                    cfg.Write(key, const_map_get(vars,*varname).getValue());
                    key.append(wxT("_link"));
                    cfg.Write(key, const_map_get(lens.variables,*varname).isLinked() ? 1:0);
                    varname++;
                }

                ImageOptions imgopts = pano.getImage(imgNr).getOptions();
                cfg.Write(wxT("Lens/vigCorrMode"), imgopts.m_vigCorrMode);
                cfg.Write(wxT("Lens/flatfield"),
                          wxString(imgopts.m_flatfield.c_str(), *wxConvCurrent) );

                cfg.Write(wxT("Lens/crop/enabled"), imgopts.docrop ? 1l : 0l);
                cfg.Write(wxT("Lens/crop/autoCenter"), imgopts.autoCenterCrop ? 1l : 0l);
                cfg.Write(wxT("Lens/crop/left"), imgopts.cropRect.left());
                cfg.Write(wxT("Lens/crop/top"), imgopts.cropRect.top());
                cfg.Write(wxT("Lens/crop/right"), imgopts.cropRect.right());
                cfg.Write(wxT("Lens/crop/bottom"), imgopts.cropRect.bottom());

                // try to read the exif data and add that to the lens ini file
                wxFileName file(wxString(pano.getImage(imgNr).getFilename().c_str(), *wxConvCurrent));
                if (file.GetExt().CmpNoCase(wxT("jpg")) == 0 ||
                        file.GetExt().CmpNoCase(wxT("jpeg")) == 0 )
                {

                    ImageInfo_t exif;
                    ResetJpgfile();
                    // Start with an empty image information structure.

                    memset(&exif, 0, sizeof(exif));
                    exif.FlashUsed = -1;
                    exif.MeteringMode = -1;

                    if (ReadJpegFile(exif,pano.getImage(imgNr).getFilename().c_str(), READ_EXIF)){
                        // calculate crop factor, if possible
                        double cropFactor=0;
                        if (exif.FocalLength > 0 && exif.CCDHeight > 0 && exif.CCDWidth > 0) {
                            cropFactor = sqrt(36.0*36.0+24.0*24)/sqrt(exif.CCDWidth*exif.CCDWidth + exif.CCDHeight*exif.CCDHeight);
                        } else if (exif.FocalLength35mm > 0 && exif.FocalLength > 0) {
                            cropFactor = exif.FocalLength35mm / exif.FocalLength;
                        }

                        // write exif data to ini file
                        cfg.Write(wxT("EXIF/CameraMake"),  wxString(exif.CameraMake, *wxConvCurrent));
                        cfg.Write(wxT("EXIF/CameraModel"), wxString(exif.CameraModel, *wxConvCurrent));
                        cfg.Write(wxT("EXIF/FocalLength"), (double) exif.FocalLength);
                        cfg.Write(wxT("EXIF/Aperture"),    (double) exif.ApertureFNumber);
                        cfg.Write(wxT("EXIF/ISO"),         exif.ISOequivalent);
                        cfg.Write(wxT("EXIF/CropFactor"),  cropFactor); 
                        cfg.Write(wxT("EXIF/Distance"),    exif.Distance); 
                    }
                }

                cfg.Flush();
            }
            // reset locale
            setlocale(LC_NUMERIC,old_locale);
        }
    } else {
        wxLogError(_("Please select an image and try again"));
    }
}

void LensPanel::OnLoadLensParameters(wxCommandEvent & e)
{
    if (m_selectedImages.size() == 1) {
        unsigned int imgNr = *(m_selectedImages.begin());
        unsigned int lensNr = pano.getImage(imgNr).getLensNr();
        Lens lens = pano.getLens(lensNr);
        VariableMap vars = pano.getImageVariables(imgNr);
        ImageOptions imgopts = pano.getImage(imgNr).getOptions();

        if (LoadLensParametersChoose(this, lens, vars, imgopts)) {
            GlobalCmdHist::getInstance().addCommand(
                    new PT::ChangeLensCmd(pano, lensNr, lens)
                                                );
            GlobalCmdHist::getInstance().addCommand(
                    new PT::UpdateImageVariablesCmd(pano, imgNr, vars)
                                                );
                // get all images with the current lens.
            UIntSet imgs;
            for (unsigned int i = 0; i < pano.getNrOfImages(); i++) {
                if (pano.getImage(i).getLensNr() == lensNr) {
                    imgs.insert(i);
                }
            }

                // set image options.
            GlobalCmdHist::getInstance().addCommand(
                    new PT::SetImageOptionsCmd(pano, imgopts, imgs) );
        }
    } else {
        wxLogError(_("Please select an image and try again"));
    }
}


bool LoadLensParametersChoose(wxWindow * parent, Lens & lens, VariableMap & vars, ImageOptions & imgopts)
{
    wxString fname;
    wxFileDialog dlg(parent,
                        _("Load lens parameters"),
                        wxConfigBase::Get()->Read(wxT("lensPath"),wxT("")), wxT(""),
                        _("Lens Project Files (*.ini)|*.ini|All files (*.*)|*.*"),
                        wxOPEN, wxDefaultPosition);
    if (dlg.ShowModal() == wxID_OK) {
        fname = dlg.GetPath();
        wxConfig::Get()->Write(wxT("lensPath"), dlg.GetDirectory());  // remember for later
        // read with with standart C numeric format
        char * old_locale = setlocale(LC_NUMERIC,NULL);
        setlocale(LC_NUMERIC,"C");
        {
            wxFileConfig cfg(wxT("hugin lens file"),wxT(""),fname);
            long w=0;
            cfg.Read(wxT("Lens/image_width"), &w);
            long h=0;
            cfg.Read(wxT("Lens/image_height"), &h);
            if (w>0 && h>0) {
                vigra::Size2D sz = lens.getImageSize();
                if (w != sz.x || h != sz.y) {
                    int ret = wxMessageBox(_("Incompatible lens parameter file, image sizes do not match\nApply settings anyway?"), _("Error loading lens parameters"), wxICON_QUESTION |wxYES_NO);
                    if (ret == wxNO) {
                        setlocale(LC_NUMERIC,old_locale);
                        return false;
                    }
                }
            } else {
                // lens ini file didn't store the image size,
                // assume everything is all right.
            }
            long integer=0;
            cfg.Read(wxT("Lens/type"), &integer);
            lens.setProjection ((Lens::LensProjectionFormat) integer);
            double d=1;
            cfg.Read(wxT("Lens/crop"), &d);
            lens.setCropFactor(d);
            d=50;
            cfg.Read(wxT("Lens/hfov"), &d);
            map_get(vars,"v").setValue(d);
            DEBUG_DEBUG("read lens hfov: " << d);

            // loop to load lens variables
            char ** varname = Lens::variableNames;
            while (*varname) {
                wxString key(wxT("Lens/"));
                key.append(wxString(*varname, *wxConvCurrent));
                d = 0;
                if (cfg.Read(key,&d)) {
                    // only set value if variabe was found in the script
                    map_get(vars,*varname).setValue(d);
                    map_get(lens.variables, *varname).setValue(d);

                    integer = 1;
                    key.append(wxT("_link"));
                    cfg.Read(key, &integer);
                    map_get(lens.variables, *varname).setLinked(integer != 0);
                }
                varname++;
            }
            long vigCorrMode=0;
            cfg.Read(wxT("Lens/vigCorrMode"), &vigCorrMode);
            imgopts.m_vigCorrMode = vigCorrMode;

            wxString flatfield;
            bool readok = cfg.Read(wxT("Lens/flatfield"), &flatfield);
            if (readok) {
                imgopts.m_flatfield = std::string((const char *)flatfield.mb_str());
            }

            // TODO: crop parameters
            long v=0;
            cfg.Read(wxT("Lens/crop/enabled"), &v);
            imgopts.docrop = v != 0;
            v=1;
            cfg.Read(wxT("Lens/crop/autoCenter"), &v);
            imgopts.autoCenterCrop = v != 0;
            long left=0;
            cfg.Read(wxT("Lens/crop/left"), &left);
            long top=0;
            cfg.Read(wxT("Lens/crop/top"), &top);
            long right=0;
            cfg.Read(wxT("Lens/crop/right"), &right);
            long bottom=0;
            cfg.Read(wxT("Lens/crop/bottom"), &bottom);
            imgopts.cropRect.setUpperLeft(vigra::Point2D(left, top));
            imgopts.cropRect.setLowerRight(vigra::Point2D(right, bottom));
        }
        // reset locale
        setlocale(LC_NUMERIC,old_locale);
        return true;
    } else {
        return false;
    }
}


void LensPanel::OnNewLens(wxCommandEvent & e)
{
    if (m_selectedImages.size() > 0) {
        // create a new lens, start with a copy of the old lens.
        unsigned int imgNr = *(m_selectedImages.begin());
        Lens l = pano.getLens(pano.getImage(imgNr).getLensNr());
        GlobalCmdHist::getInstance().addCommand(
            new PT::AddNewLensToImagesCmd(pano, l, m_selectedImages)
            );
    } else {
        wxLogError(_("Please select an image and try again"));
    }
}

void LensPanel::OnChangeLens(wxCommandEvent & e)
{
    if (m_selectedImages.size() > 0) {
        // ask user for lens number.
        long nr = wxGetNumberFromUser(_("Enter new lens number"), _("Lens number"),
                                      _("Change lens number"), 0, 0,
                                      pano.getNrOfLenses()-1);
        if (nr >= 0) {
            // user accepted
            GlobalCmdHist::getInstance().addCommand(
                new PT::SetImageLensCmd(pano, m_selectedImages, nr)
                );
        }
    } else {
        wxLogError(_("Please select an image and try again"));
    }
}


/*
bool initLensFromFile(const std::string & filename, double &cropFactor, Lens & l,
                      VariableMap & vars, ImageOptions & imgopts, bool sameSettings)
{
    double roll = 0;
    if (!l.initFromFile(filename, cropFactor, roll)) {
        if (cropFactor == -1) {
            map_get(vars,"r").setValue(roll);
            cropFactor = 1;
            wxConfigBase::Get()->Read(wxT("/LensDefaults/CropFactor"), &cropFactor);
            wxString tval;
            tval.Printf(wxT("%4.2f"),cropFactor);
            wxString t = wxGetTextFromUser(_("Enter Crop Factor for image\n\nThe crop factor is given by:\ncrop factor = 43.3 / diagonal \n\nwhere diagnal is the diagonal (in mm) of the film or imaging chip."),
                _("Adding Image"), tval);
            t.ToDouble(&cropFactor);
            wxConfigBase::Get()->Write(wxT("/LensDefaults/CropFactor"), cropFactor);
            l.m_hasExif = true;
            return l.initFromFile(filename, cropFactor, roll);
        } else {
            // override dialog, if forced to use the same settings.
            if (sameSettings) {
                return true;
            }
            // no exif info found.. ask user for all details
            bool valid=false;
            l.m_hasExif = false;
            while (!valid)
            {
                wxDialog dlg;
                wxXmlResource::Get()->LoadDialog(&dlg, MainFrame::Get(), wxT("dlg_focallength"));
                int ret = dlg.ShowModal();
                if (ret == wxID_OK) {
                    l.setProjection( (Lens::LensProjectionFormat)XRCCTRL(dlg, "lensdlg_type_choice", wxChoice)->GetSelection() );

                    wxString text = XRCCTRL(dlg, "lensdlg_crop_factor_text", wxTextCtrl)->GetValue();
                    double c;
                    if (!str2double(text, c)) {
                        wxLogError(_("The crop factor must be numeric."));
                        continue;
                    }
                    l.setCropFactor(c);
                    cropFactor = c;

                    text = XRCCTRL(dlg, "lensdlg_focallength_text", wxTextCtrl)->GetValue();
                    double f;
                    if (!str2double(text, f)) {
                        wxLogError(_("The focal length value must be numeric."));
                        continue;
                    }
                    l.setFocalLength(f);
                    valid = true;
                } else if (ret == XRCID("lensdlg_load_lens_data") ) {
                    // try to load lens data from ini file
                    if (!LoadLensParametersChoose(l, vars, imgopts) ) {
                        continue;
                    }
                    valid = true;
                }
            }
        }
        return false;
    }
    map_get(vars,"r").setValue(roll);
    l.m_hasExif = true;
    return true;
}
*/


char *LensPanel::m_varNames[] = { "v", "a", "b", "c", "d", "e", "g", "t", 
                                  "Eev", "Er", "Eb", 
                                  "Vb", "Vc", "Vd", "Vx", "Vy",
                                  "Ra", "Rb", "Rc", "Rd", "Re", 0};

