// -*- c-basic-offset: 4 -*-
/** @file PanoPanel.h
 *
 *  @author Kai-Uwe Behrmann <web@tiscali.de>
 *
 *  $Id$
 *
 *  This is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _PANOPANEL_H
#define _PANOPANEL_H

#include "hugin/MainFrame.h"

//using namespace PT;
class PanoDialog;
class wxSpinCtrl;
class wxTextCtrl;
class wxChoice;
class wxComboBox;

class StitcherPanel;

/** Define the pano edit panel
 *
 *  - it is for the settings of the final panorama.
 *    Maybe here goes the preview to.
 */
class PanoPanel: public wxPanel, public PT::PanoramaObserver
{
public:
    PanoPanel(wxWindow *parent, PT::Panorama * pano);
    virtual ~PanoPanel(void) ;

    /** this is called whenever the panorama has changed.
     *
     *  This function must now update all the gui representations
     *  of the panorama to display the new state.
     *
     *  Functions that change the panororama must not update
     *  the GUI directly. The GUI should always be updated
     *  to reflect the current panorama state in this function.
     *
     *  This avoids unnessecary close coupling between the
     *  controller and the view (even if they sometimes
     *  are in the same object). See model view controller
     *  pattern.
     *
     *  @todo   react on different update signals more special
     */
    virtual void panoramaChanged(PT::Panorama &pano);
//    void panoramaImageChanged(PT::Panorama &pano, const PT::UIntSet & imgNr);

    /** set the image */
// TODO remove
//    void previewSingleChanged(wxCommandEvent & e);

 private:

    /// the supported stitching engines
    enum StitchingEngine { NONA=0, PTSTITCHER };

    /// the supported defaults
    enum StitchingPresets { PROFILE_CUSTOM =0 , PROFILE_JPEG=0,
                            PROFILE_DRAFT_JPEG, PROFILE_TIFF,
                            PROFILE_LAYER_TIFF, PROFILE_LAYER_PSD };

    // resize if the notebook page changes size
    void FitParent(wxSizeEvent & e);

    // apply changes from the model
    void UpdateDisplay(const PT::PanoramaOptions & opt);

    // apply changes to the model. (gui values -> Panorama)
    void QuickModeChanged(wxCommandEvent & e);
    void ApplyQuickMode(int preset);

    void HFOVChanged(wxCommandEvent & e);
    void HFOVChangedSpin(wxSpinEvent & e);
    void VFOVChanged(wxCommandEvent & e );
    void VFOVChangedSpin(wxSpinEvent & e);
    void ProjectionChanged(wxCommandEvent & e);
    void StitcherChanged(wxCommandEvent & e);

    void WidthChanged(wxCommandEvent & e);

    // actions
    void DoStitch(wxCommandEvent & e);
// TODO remove
//    void DoPreview(wxCommandEvent & e);
    void DoCalcFOV(wxCommandEvent & e);

    /** set the highest sensible width
     */
    void DoCalcOptimalWidth(wxCommandEvent & e);

    /*  calculate average pixel density of each image
     *  and use the highest one to calculate the width
     *
     */
    unsigned int CalcOptimalWidth();

    /** enable/disable control influenced by quick mode */
    void EnableControls(bool enable);

    // the model
    Panorama &pano;

    // don't listen to input on gui elements during
    // updating the gui from the model, to prevent recursion,
    // because the gui might report changes as well.
    bool updatesDisabled;
    PanoramaOptions m_oldOpt;
    double m_oldVFOV;

    // controls of this frame
    wxChoice    * m_QuickChoice;
    wxChoice    * m_ProjectionChoice;
    wxSpinCtrl  * m_HFOVSpin;
    wxSpinCtrl  * m_VFOVSpin;

    wxTextCtrl  * m_WidthTxt;
    wxStaticText *m_HeightStaticText;
    wxChoice    * m_StitcherChoice;
    wxButton    * m_StitchButton;
    wxButton    * m_CalcHFOVButton;
    wxButton    * m_CalcOptWidthButton;

    StitcherPanel * m_Stitcher;

    DECLARE_EVENT_TABLE()
};

#endif // _PANOPANEL_H
