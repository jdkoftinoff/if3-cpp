# Microsoft Developer Studio Project File - Name="if2kkernel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=if2kkernel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "if2kkernel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "if2kkernel.mak" CFG="if2kkernel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "if2kkernel - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "if2kkernel - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "if2kkernel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX"if2k_world.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "if2kkernel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX"if2k_world.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "if2kkernel - Win32 Release"
# Name "if2kkernel - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\if2k_buf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\if2k_buf_macosx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\if2k_compile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\if2k_dynbuf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\if2k_mmap_buf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\if2k_multiscanner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\if2k_pattern_expander.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\if2k_scanner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\if2k_tree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\if2k_treebase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\if2k_util.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\if2k_world.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\if2k_buf.h
# End Source File
# Begin Source File

SOURCE=..\..\include\if2k_buf_macosx.h
# End Source File
# Begin Source File

SOURCE=..\..\include\if2k_compile.h
# End Source File
# Begin Source File

SOURCE=..\..\include\if2k_dynbuf.h
# End Source File
# Begin Source File

SOURCE=..\..\include\if2k_mmap_buf.h
# End Source File
# Begin Source File

SOURCE=..\..\include\if2k_multiscanner.h
# End Source File
# Begin Source File

SOURCE=..\..\include\if2k_pattern_expander.h
# End Source File
# Begin Source File

SOURCE=..\..\include\if2k_scanner.h
# End Source File
# Begin Source File

SOURCE=..\..\include\if2k_string.h
# End Source File
# Begin Source File

SOURCE=..\..\include\if2k_tree.h
# End Source File
# Begin Source File

SOURCE=..\..\include\if2k_treebase.h
# End Source File
# Begin Source File

SOURCE=..\..\include\if2k_util.h
# End Source File
# Begin Source File

SOURCE=..\..\include\if2k_world.h
# End Source File
# End Group
# End Target
# End Project
