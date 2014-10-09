// -*- c-basic-offset: 4 -*-

/** @file lensdb.cpp
 *
 *  @brief helper program for working with lens database
 *
 *
 *  @author T. Modes
 *
 */

/*  This program is free software; you can redistribute it and/or
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

#include <hugin_version.h>

#include <iostream>
#include <string>
#include <fstream>
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
#include <getopt.h>
#include <panodata/Panorama.h>
#include <hugin_utils/stl_utils.h>
#include <lensdb/LensDB.h>
#include <panodata/StandardImageVariableGroups.h>
#include <hugin_base/panotools/PanoToolsUtils.h>

typedef std::vector<boost::filesystem::path> pathVec;


template <class iteratorType>
bool iterateFileSystem(std::string src, pathVec& projectFiles)
{
    try
    {
        for(iteratorType it(src); it != iteratorType(); it++)
        {
            std::string ext=hugin_utils::toupper(it->path().extension().string());
            if(ext==".PTO")
            {
                projectFiles.push_back(*it);
            };
        }
    }
    catch(boost::filesystem::filesystem_error e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
};

void FindPTOFiles(pathVec& projectFiles, std::string src, bool recursive)
{
    if(recursive)
    {
        iterateFileSystem<boost::filesystem::recursive_directory_iterator>(src, projectFiles);
    }
    else
    {
        iterateFileSystem<boost::filesystem::directory_iterator>(src, projectFiles);
    };
};

bool CheckProjectFile(boost::filesystem::path filename)
{
    // open project file
    HuginBase::Panorama pano;
    std::string input = filename.string();
    std::ifstream prjfile(input.c_str());
    if (!prjfile.good())
    {
        std::cerr << "ERROR: Could not open script: " << filename.string() << endl;
        return false;
    }
    std::string inputPathPrefix = hugin_utils::getPathPrefix(input);
    pano.setFilePrefix(inputPathPrefix);
    AppBase::DocumentData::ReadWriteError err = pano.readData(prjfile);
    if (err != AppBase::DocumentData::SUCCESSFUL)
    {
        std::cerr << "ERROR: error while parsing panos tool script: " << input << std::endl
                  << "DocumentData::ReadWriteError code: " << err << std::endl;
        return false;
    };
    prjfile.close();
    if (pano.getNrOfImages() == 0)
    {
        return false;
    };
    std::cout << "Checking " << filename.filename();
    HuginBase::StandardImageVariableGroups lenses(pano);
    if (lenses.getLenses().getNumberOfParts()==1)
    {
        // read the EXIF data
        for (size_t i = 0; i < pano.getNrOfImages(); ++i)
        {
            HuginBase::SrcPanoImage img = pano.getSrcImage(i);
            if (!img.readEXIF())
            {
                std::cout << " Ignored (File missing or missing metadata)" << std::endl;
                return false;
            }
            pano.setSrcImage(i, img);
        };
        // update cp errors
        HuginBase::PTools::calcCtrlPointErrors(pano);
        // now save in database
        if (HuginBase::LensDB::SaveLensDataFromPano(pano))
        {
            std::cout << " Saved." << std::endl;
            return true;
        }
        else
        {
            std::cout << " Ignored." << std::endl;
            return false;
        }
    };
    std::cout << " Ignored (More than one lens)." << std::endl;
    return false;
};

static void usage(const char* name)
{
    std::cout << name << ": tool for lens database maintenance" << std::endl
              << name << " version " << DISPLAY_VERSION << std::endl
              << std::endl
              << "Usage:  lensdb [--recrusive] --populate BASEPATH " << std::endl
              << "             Populate database with information from all pto files" << std::endl
              << "             in given BASEPATH" << std::endl
              << "             With --recursive switch all subfolders will also be" << std::endl
              << "             searched." << std::endl
              << "        lensdb --compress" << std::endl
              << "             Compresses the database by replacing single values" << std::endl
              << "             with averaged values." << std::endl
              << std::endl;
};

int main(int argc, char* argv[])
{
    // parse arguments
    const char* optstring = "crph";

    static struct option longOptions[] =
    {
        {"compress", no_argument, NULL, 'c' },
        {"recursive", no_argument, NULL, 'r' },
        {"populate", no_argument, NULL, 'p' },
        {"help", no_argument, NULL, 'h' },
        0
    };

    bool recursive=false;
    bool populate = false;
    bool compress = false;
    std::string basepath;
    int c;
    int optionIndex = 0;
    while ((c = getopt_long (argc, argv, optstring, longOptions,&optionIndex)) != -1)
    {
        switch (c)
        {
            case 'h':
                usage(hugin_utils::stripPath(argv[0]).c_str());
                return 0;
            case 'c':
                compress=true;
                break;
            case 'r':
                recursive=true;
                break;
            case 'p':
                populate = true;
                break;
            case '?':
                break;
            default:
                abort ();
        }
    }

    if (populate)
    {
        if (argc - optind != 1)
        {
            usage(hugin_utils::stripPath(argv[0]).c_str());
            return -1;
        };

        basepath = argv[optind];
    };

    if (!populate && !compress)
    {
        std::cout << "Lensdatabase file: " << HuginBase::LensDB::LensDB::GetSingleton().GetDBFilename() << std::endl;
        std::cout << "Nothing to do." << std::endl;
    };

    if (!basepath.empty())
    {
        boost::filesystem::path p(basepath);
        if (boost::filesystem::exists(p))
        {
            p = boost::filesystem::absolute(p);
            if (boost::filesystem::is_directory(p))
            {
                pathVec projectFiles;
                FindPTOFiles(projectFiles, p.string(), recursive);
                if (projectFiles.empty())
                {
                    std::cerr << "ERROR: No project files found in given directory " << p.string() << std::endl;
                    return 1;
                };
                for (pathVec::const_iterator it = projectFiles.begin(); it != projectFiles.end(); ++it)
                {
                    CheckProjectFile(*it);
                };
            }
            else
            {
                std::cerr << "ERROR: " << basepath << " is not a directory." << std::endl;
                return 1;
            };
        }
        else
        {
            std::cerr << "ERROR: Path " << basepath << " does not exists." << std::endl;
            return 1;
        }
    };

    if (compress)
    {
        std::cout << "Compressing database..." << std::endl;
        if(HuginBase::LensDB::LensDB::GetSingleton().CleanUpDatabase())
        {
            std::cout << "Successful." << std::endl;
        }
        else
        {
            std::cout << "FAILED." << std::endl;
        }
    };
    HuginBase::LensDB::LensDB::Clean();
    return 0;
}
