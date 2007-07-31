// -*- c-basic-offset: 4 -*-
/** @file 
*
*  @author Ippei UKAI <ippei_ukai@mac.com>
*
*  $Id: $
*
*  This is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public
*  License as published by the Free Software Foundation; either
*  version 2 of the License, or (at your option) any later version.
*
*  This software is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this software; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1307, USA.
*
*  Hereby the author, Ippei UKAI, grant the license of this particular file to
*  be relaxed to the GNU Lesser General Public License as published by the Free
*  Software Foundation; either version 2 of the License, or (at your option)
*  any later version. Please note however that when the file is linked to or
*  compiled with other files in this library, the GNU General Public License as
*  mentioned above is likely to restrict the terms of use further.
*
*/


namespace HuginQt {

class HuginDocumentViewLayoutInstance : public QObject
{
    
public:
    HuginDocumentViewLayoutInstance(HuginDocumentViewManager* parent);
//    virtual HuginDocumentViewLayoutInstance(HuginDocumentViewManager* parent, 
//                                            QList<HuginDocumentViewLayoutProfile*> layoutProfiles);
    virtual ~HuginDocumentViewLayoutInstance() {}
    
public:
    virtual void layoutViews();
    
public:
    virtual void raiseView(QString viewID);
    virtual void activateUtilityView(const QString& utilViewID);
    virtual void minimizeUtilityView(const QString& utilViewID);
    
public:
    virtual QWidget* mainDocumentWindow();
    virtual QList<QWidget*> allDocumentWindows();
    
};



class HuginDocumentViewLayout : public QObject
{
    
public:
    HuginDocumentViewLayout(QObject* parent = NULL);
    virtual ~HuginDocumentViewLayout() {}
    
public:
    virtual HuginDocumentViewLayoutInstance* newInstance(HuginDocumentViewManager* parent) =0;
//    virtual HuginDocumentViewLayoutInstance* newInstance(HuginDocumentViewManager* parent, 
//                                                         QList<HuginDocumentViewLayoutProfile*> layoutProfiles);

public:
//    virtual supportsLayoutProfile();
    
public:
    virtual QString displayName() =0;
    virtual QString layoutID() =0;
    
};


} //namespace