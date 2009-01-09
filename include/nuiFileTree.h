/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once

#include "nuiComposite.h"
#include "nuiTreeView.h"
#include "nglVolume.h"
#include "nuiHBox.h"
#include "nuiVBox.h"
#include "nuiList.h"


/*
NOTE ABOUT DECORATIONS:

Object names have been assigned to the nuiFileSelector children, 
in order to get an easy way to set a decoration within a css file.
You just have to use the following labels as object names in your css file.

"nuiFileTree"
"nuiFileTree::VolumeLabel"
"nuiFileTree::FolderLabel"
"nuiFileTree::FileLabel"
"nuiFileTree::VolumeIcon"
"nuiFileTree::FolderIcon"
"nuiFileTree::FileIcon"
"nuiFileTree::FileIcon::[file extension]"
 
*/

#define MYDEBUG
#ifdef MYDEBUG


#define ROOTPATH_ALLVOLUMES _T("*:")

class nuiFileTree : public nuiComposite
{
public:
  
  nuiFileTree(const nglPath& rPath, const nglPath& rRootPath, const nglString& rFilter = _T("*"), bool showHiddenFiles = false);
  nuiFileTree(const nglPath& rPath, const nglPath& rRootPath, const std::list<nglString>& rFilters, bool showHiddenFiles = false);
  virtual ~nuiFileTree();
  
  bool SetPath(const nglPath& rPath);      ///< Change currently selected Path.
  bool SetRootPath(const nglPath& rPath);      ///< Change current root
  nglPath GetPath() const;          ///< Retrieve currently selected Path
  nglPath GetRootPath() const;          ///< Retrieve current root.

  nuiSimpleEventSource<nuiWidgetActivated> PathChanged;      ///< Event triggered when the user is navigating around the file system.
  nuiSimpleEventSource<nuiWidgetActivated> SelectionChanged; 
  nuiSimpleEventSource<nuiWidgetActivated> FileActivated; 
  nuiSimpleEventSource<nuiWidgetActivated> FolderActivated; 
  
private:

  void Init(const nglPath& rPath, const nglPath& rRootPath, const std::list<nglString>& rFilters, bool showHiddenFiles);
  void AddTree(const nglPath& rPath, bool Opened);

  nuiWidget* GetFileInfo(const nglPath& rPath);
  void FormatFileSize(nuiSize size, nglString& str);

  bool IsFilterSet(const nglString& rFilter);
  bool IsFileFiltered(const nglPath& rFile);
  
  const std::list<nglString>& GetFilters() const;
  void SetFilters(const std::list<nglString>& rFilters);
  std::list<nglString> mFilters;
  
//  bool OnRootOpened(const nuiEvent& rEvent);
  virtual nuiTreeNode* GetNewNode(const nglPath& rPath);
  
  bool mShowHiddenFiles;
  nuiTreeView* mpTreeView;
  nuiTreeNode* mpTreeRoot;
  std::map<nglPath, nuiTreeNode*> mTrees; // rootpath => treeview
  nuiTreeNode* mpCurrentTree;
  nglPath mRootPath;

  bool OnClosed   (const nuiEvent& rEvent);
  bool OnOK       (const nuiEvent& rEvent);
  bool OnSelected (const nuiEvent& rEvent);
  
  friend class nuiFileTreeNode;
  
  nuiEventSink<nuiFileTree> mEventSink;
  
};

#endif

