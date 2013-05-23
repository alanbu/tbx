/*
 * TbxAppGen tbx RISC OS toolbox library Application generator
 *
 * Copyright (C) 2010 Alan Buckley   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
// GenAppSaver.cpp: implementation of the GenAppSaver class.
//
//////////////////////////////////////////////////////////////////////

#include "AppGenerator.h"
#include "MainWnd.h"
#include "tbx/stringutils.h"
#include "tbx/hourglass.h"
#include "tbx/sprite.h"
#include "tbx/font.h"
#include "tbx/reporterror.h"
#include "tbx/tag.h"
#include "tbx/res/reseditor.h"
#include "tbx/res/resiconbar.h"
#include "tbx/res/reswindow.h"
#include "tbx/res/resmenu.h"
#include "tbx/res/resproginfo.h"

#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "swis.h"

using namespace tbx;
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AppGenerator::AppGenerator(std::string id, std::string filename, std::string taskName) :
   m_taskName(taskName),
   m_targetPath(filename)
{
   m_sourcePath = "<TbxAppGen$Dir>.Skeletons.Files";
   m_sourcePath.down(id);
   m_standardMakeFile = false;
   m_makeFileFound = false;
   m_fileTypeReadOnly = false;
}

AppGenerator::~AppGenerator()
{
}

/**
 * Generate the new application
 */
void AppGenerator::Generate()
{
    m_appName = m_targetPath.leaf_name();

    if (m_appName[0] == '!')
    {
       m_appName.erase(0,1);
    } else
    {
       std::string leaf("!");
       leaf += m_appName;
       m_targetPath.leaf_name(leaf);
    }

    // Task name same as application name if left empty
    if (m_taskName.empty()) m_taskName = m_appName;
    m_appNameLC = to_lower(m_appName);


	if (!m_targetPath.create_directory())
	{
		report_error("Unable to create directory");
	} else
	{
		Hourglass hg; // Show hourglass while copying is going on


        CopyAndScanResFile();
        CreateBootFile();
        CreateRunFile();
        CreateMessagesFile();
        CreateSpriteFile();

        // Programs have filetype in hex
         m_fileType = "0x" + m_fileType;

        CopyFiles(m_sourcePath, m_targetPath);
        if (m_standardMakeFile && !m_makeFileFound) CreateMakeFile();
	}
}

/**
 * Create the !Boot file
 */
void AppGenerator::CreateBootFile()
{
   Path boot_file = m_targetPath.child("!Boot");
   ofstream boot(boot_file);

   boot << "| Boot file for " << m_taskName << endl;
   boot << "| Generated by TbxAppGen" << endl << endl;
   boot << "IconSprites <Obey$Dir>.!Sprites" << endl << endl;
   boot << "Set " << m_appName << "$Dir <Obey$Dir>" << endl;

   // File type stuff
   if (!m_fileType.empty() && !m_fileTypeReadOnly)
   {
      boot <<"Set Alias$RunType_";
      boot << m_fileType;
      boot << " Run <Obey$Dir>.!Run %%*0";
      boot << endl;
      boot << "Set File$Type_";
      boot << m_fileType;
      boot << " \"";
      boot << m_fileDesc;
      boot << "\"" << endl;
    }

   boot.close();
   boot_file.file_type(0xfeb); // Obey
}

/**
 * Create the !Run File
 */
void AppGenerator::CreateRunFile()
{
   Path run_file = m_targetPath.child("!Run");
   ofstream run(run_file);

   run << "| Run file for " << m_taskName << endl;
   run << "| Generated by TbxAppGen" << endl << endl;
   run << "/<Obey$Dir>.!Boot" << endl << endl;

   for (vector<string>::iterator cl = m_runCommon.begin(); cl != m_runCommon.end(); ++cl)
   {
      run << *cl << endl;
   }

   map<string, string>::iterator i;
   std::string name, version, filename;

   for (i = m_modulesFound.begin(); i != m_modulesFound.end(); ++i)
   {
      name = i->first;
      if (name.size() > 10) filename = name.substr(0,10);
      else filename = name;

      // Special case for DCS_Quit
      if (name == "DCS_Quit") name = "DCS";

      version = i->second;

      run << "RMEnsure " << name
          << " 0.00 RMLoad System:Modules.Toolbox." << filename << endl;
      run << "RMEnsure " << name << " "
          << version
          << " Error This application needs " << filename << " "
          << version
          << " or later." << endl;
   }

   run << endl;
   run << "Wimpslot -min 2048K -max 2048K" << endl << endl;

   run << "Run <" << m_appName << "$Dir>.!RunImage %*0" << endl;

   run.close();
   run_file.file_type(0xfeb); // Obey
}

/**
 * Create minimal messages file
 */
void AppGenerator::CreateMessagesFile()
{
   Path msg_file = m_targetPath.child("Messages");
   ofstream msg(msg_file);

   msg << "_TaskName:" << m_taskName << endl;
   msg << "_TaskSprite:!" << m_appNameLC << endl;
}

/**
 * Copies the resource file, changing the icon
 * name on the icon bar icon and scanning to
 * see what modules are needed.
 */
void AppGenerator::CopyAndScanResFile()
{
   Path res_source = m_sourcePath.child("Res");

   if (res_source.exists())
   {
      LoadModuleInfo();
      Path res_target = m_targetPath.child("Res");
     // Change sprite on icon bar
     // Scan to see what modules are needed and update m_modules
     // Write to res_target
     res::ResEditor editor;
     if (editor.load(res_source))
     {
         bool res_changed = false;
         res::ResEditor::iterator i = editor.find("Iconbar");
         if (i != editor.end())
         {
             res::ResIconbar ib( *i );
             ib.sprite_name("!" + m_appNameLC);
             editor.replace(i, ib);
             res_changed = true;
         }
         i = editor.find("IconbarMenu");
         if (i != editor.end())
         {
            res::ResMenu im( *i );
            im.title(m_taskName);
            editor.replace(i, im);
            res_changed = true;
         }
         i = editor.find("ProgInfo");
         if (i != editor.end())
         {
            res::ResProgInfo pi(*i);
            pi.version(UTCTime::now().text("0.0 (%DY-%M3-%YR)"));
            editor.replace(i, pi);
         }
         if (res_changed) editor.save(res_target);

         for (res::ResEditor::iterator i = editor.begin();
              i != editor.end(); ++i)
         {
             // use a set
             res::ResObject obj = *i;
             int class_id = obj.class_id();
             AddObjectModule(class_id);
             if (class_id == res::ResWindow::CLASS_ID)
             {
                res::ResWindow resWindow(obj);
                for (res::ResWindow::const_gadget_iterator g = resWindow.gadget_cbegin();
                     g != resWindow.gadget_cend(); ++g)
                {
                    res::ResGadget gadget = *g;
                    // Lookup module for gadget
                    AddGadgetModule(gadget.type());
                }
             }
         }

       }
   }
}

/**
 * Recursive copy of files
 */
void AppGenerator::CopyFiles(const Path &fromPath, const Path &toPath)
{
	PathInfo::Iterator i;
	vector<string> subDirectories;
	Path from, to;
	std::string relFileName;

	for (i = PathInfo::begin(fromPath); i != PathInfo::end(); ++i)
	{
		PathInfo &pi = (*i);

		if (pi.directory())
		{
			// Subdirectories are recursed later
			subDirectories.push_back(pi.name());
		} else
		{
			from = fromPath.child(pi.name());
			to = toPath.child(pi.name());
			if (pi.name() == "!Help")
			{
			    CopyWithReplace(from, to);
		    } else if (pi.name() == "Makefile")
		    {
		        m_makeFileFound = true;
			    CopyWithReplace(from, to);
			} else
			{
			    from.copy(to);
			}
	    }
	}

	for (unsigned int j = 0; j < subDirectories.size(); j++)
	{
		from = fromPath.child(subDirectories[j]);
		to = toPath.child(subDirectories[j]);
		if (to.create_directory())
		{
			// Recursively copy subdirectories
			if (subDirectories[j] == "cc" || subDirectories[j] == "h")
			{
			   CopySourceDirectory(from, to);
			} else
			{
			   CopyFiles(from, to);
			}
 		}
	}
}

/**
 * Copy a source directory
 */
void AppGenerator::CopySourceDirectory(const Path &from, const Path &to)
{
	for (PathInfo::Iterator i = PathInfo::begin(from); i != PathInfo::end(); ++i)
	{
		PathInfo &pi = (*i);
		std::string leafname = pi.name();
		Replace(leafname,"APPNAME_", m_appName);
		CopyWithReplace(from.child(pi.name()), to.child(leafname));
    }
}


/**
 * Copy file replacing Tokens
 */
void AppGenerator::CopyWithReplace(const Path &from, const Path &to)
{
    int dataLength;
	char *data = from.load_file(&dataLength);
	ReplaceTokens(data, dataLength);
    to.save_file(data, dataLength, 0xFFF);
	delete [] data;
}

// Replace the tokens in text files
void AppGenerator::ReplaceTokens(char *&data, int &dataLength)
{
	string text(data, dataLength);
	bool changed = false;

	if (Replace(text, "%APPNAME%", m_appName)) changed = true;
	if (Replace(text, "%APPNAME_LC%", m_appNameLC)) changed = true;
	if (Replace(text, "%TASKNAME%", m_taskName)) changed = true;
	if (Replace(text, "%FILETYPE%", m_fileType)) changed = true;

	if (changed)
	{
		if (text.length() > (unsigned int)dataLength)
		{
			delete [] data;
			data = new char[text.length()];
		}
		dataLength = text.length();
		text.copy(data, dataLength);
	}
}

// Helper function to replace all occurrances of one string with another
bool AppGenerator::Replace(std::string &text, const std::string &what, const std::string &with)
{
	unsigned int pos = 0;
	bool changed = false;

	pos = text.find(what);

	while (pos != string::npos)
	{
		changed = true;
		if (with.empty())
		{
			text.erase(pos, what.length());
		} else
		    text.replace(pos, what.length(), with);
		pos = text.find(what, pos+1);
	}

	return changed;
}

/**
 * Create standard sprites file.
 *
 * Reads standard, edits it for new app the write it
 */
void AppGenerator::CreateSpriteFile()
{
    Path target(m_targetPath, "!Sprites");
    Path source("<TbxAppGen$Dir>.Skeletons.Common.!Sprites");

    SpriteArea spriteArea;

    if (spriteArea.load(source))
    {
       UserSprite sprite = spriteArea.get_sprite("!appname");
       if (sprite.is_valid()) sprite.rename('!' + m_appName);

       sprite = spriteArea.get_sprite("sm!appname");
       if (sprite.is_valid()) sprite.rename("sm!" + m_appName);

       sprite = spriteArea.get_sprite("ic_appname");
       if (sprite.is_valid()) sprite.rename("ic_" + m_appName);

       if (!m_fileType.empty() && !m_fileTypeReadOnly)
       {
           sprite = spriteArea.get_sprite("file_xxx");
           SpriteCapture fcap(&sprite, true);

            Font fnt("Corpus.Medium", os_to_points_16th(32));
            if (sprite.has_palette())
            {
               fnt.set_colours(Colour::black, Colour::white, 0);
            } else
            {
               // Note: As default sprite palette is different
               // to actual desktop top palette this will
               // actually give cyan
               fnt.set_colours(Colour::white, Colour::black,0);
            }
            fnt.paint(4,4, m_fileType, Font::FPF_OSUNITS);
            fcap.release();

            sprite.rename("file_" + m_fileType);

            sprite = spriteArea.get_sprite("small_xxx");
            SpriteCapture scap(&sprite, true);
            Font sfnt("Corpus.Medium", os_to_points_16th(12));
            if (sprite.has_palette())
            {
               sfnt.set_colours(Colour::black, Colour::white, 0);
            } else
            {
               // Note: As default sprite palette is different
               // to actual desktop top palette this will
               // actually give cyan
               sfnt.set_colours(Colour::white, Colour::black,0);
            }
            sfnt.paint(4,4, m_fileType);
            scap.release();

            sprite.rename("small_" + m_fileType);
       } else
       {
           spriteArea.erase("file_xxx");
           spriteArea.erase("small_xxx");
       }

       spriteArea.save(target);
    }
}

/**
 * Create standard make file
 */
void AppGenerator::CreateMakeFile()
{
    Path target(m_targetPath, "src.Makefile");
    Path source("<TbxAppGen$Dir>.Skeletons.Common.Makefile");
    CopyWithReplace(source, target);
}


/**
 * Parse number helper
 */
inline int get_number(std::string numstr)
{
    if (numstr.size() > 2 && numstr.substr(0,2) == "0x")
    {
       return strtol(numstr.c_str(), 0, 16);
    } else
    {
       return atoi(numstr.c_str());
    }
}

/**
 * Load module information into maps for lookup
 */
void AppGenerator::LoadModuleInfo()
{
   if (!m_objectModules.empty()) return;
   ifstream modFile("<TbxAppGen$Dir>.Modules");
   if (!modFile) return;

   try
   {
      TagDoc modules;
      modules.read_tag(modFile);
      Tag *root = modules.find_child("Modules");
      int id_att = modules.attribute_id("ID");
      int name_att = modules.attribute_id("Name");
      int module_att = modules.attribute_id("Module");
      int version_att = modules.attribute_id("Version");
      if (root)
      {
         Tag *common = root->find_child("Common");
         if (common)
         {
            Tag *module = common->first_child();
            while (module)
            {
                string name = module->attribute_value(name_att);
                if (!name.empty())
                {
                    string ver = module->attribute_value(version_att);
                    if (!ver.empty())
                    {
                       m_runCommon.push_back("RMEnsure " + name + " " + ver
                           + " RMLoad System:Modules.Toolbox." + name);
                       m_runCommon.push_back("RMEnsure " + name + " " + ver
                           + " Error This application needs "
                           + name + " " + ver + " or later");
                   }
               }
               module = module->next();
            }
         }

         Tag *objects = root->find_child("Objects");
         if (objects)
         {
            Tag *object = objects->first_child();
            while (object)
            {
                int class_id = get_number(object->attribute_value(id_att));
                std::string module_name = object->attribute_value(module_att);
                if (module_name.empty())
                {
                   module_name = object->attribute_value(name_att);
                }
                m_objectModules[class_id] = ModuleInfo(module_name,
                    object->attribute_value(version_att)
                    );
                object = object->next();
            }
         }
         Tag *gadgets = root->find_child("Gadgets");
         if (gadgets)
         {
            Tag *gadget = gadgets->first_child();
            while (gadget)
            {
                int id = get_number(gadget->attribute_value(id_att));
                std::string module_name = gadget->attribute_value(module_att);
                m_gadgetModules[id] = ModuleInfo(module_name,
                    gadget->attribute_value(version_att)
                    );
                gadget = gadget->next();
            }
         }

      } else
      {
         report_error("Modules file has 'Modules' entry missing");
      }
   } catch (TagException &e)
   {
      std::string msg("Unable to load Modules information ");
      msg += e.what();
      tbx::report_error(msg.c_str());
   }
}

/**
 * Add object module to found modules list
 */
void AppGenerator::AddObjectModule(int object_class)
{
   map<int, ModuleInfo>::iterator info_iter = m_objectModules.find(object_class);
   if (info_iter != m_objectModules.end())
   {
       ModuleInfo &info = (*info_iter).second;
       map<string, string>::iterator i = m_modulesFound.find(info.name);
       if (i == m_modulesFound.end())
       {
           m_modulesFound[info.name] = info.version;
       }
   }
}

void AppGenerator::AddGadgetModule(int gadget_class)
{
   map<int, ModuleInfo>::iterator info_iter = m_gadgetModules.find(gadget_class);
   if (info_iter != m_gadgetModules.end())
   {
       ModuleInfo &info = (*info_iter).second;
       map<string, string>::iterator i = m_modulesFound.find(info.name);
       if (i == m_modulesFound.end())
       {
           m_modulesFound[info.name] = info.version;
       }
   }
}





