############################################################################################
#
# Sampo Siltanen
# 2017
#
# This script crawls through Visual Studio build intermediate target folder
# and fetches the .obj file names for Debug and Release run modes and
# adds them to additional dependencies in .vsxproj file
#
# This software asks for four parameters
#   1. Project file name to be updated
#   2. Path to the project file
#   3. Path to Debug .obj files
#   4. Path to Release .obj files
#
# User may give these parameters as command line parameters in that order or
# otherwise the software asks for them during run time.
#
# Use of this software is at the user's own risk.
# I do not take any responsibility for mishaps caused by the use of this software.
#
#############################################################################################

import sys
import os
import xml.etree.ElementTree as ET

#Main function for this script
def main():

    if (len(sys.argv) > 1):
        projectname = sys.argv[1]
        print('Project name: ' + projectname)
    else:
        projectname = input('Project name: ')

    projectpath = ''
    if (len(sys.argv) > 2):
        projectpath = sys.argv[2]
        print('Path to project file: ' + projectpath)
    while True:
        if (len(projectpath) == 0):
            projectpath = input('Path to project file: ')
        try:
            os.chdir(projectpath)
            break
        except (WindowsError, OSError):
            print('Oops, no such folder found')
            projectpath = ''

    debugpath = ''
    if (len(sys.argv) > 3):
        debugpath = sys.argv[3]
        print('Path to debug .obj files: ' + debugpath)
    while True:
        if (len(debugpath) == 0):
            debugpath = input('Path to debug .obj files: ')
        try:
            os.chdir(debugpath)
            break
        except (WindowsError, OSError):
            print('Oops, no such folder found')
            debugpath = ''

    releasepath = ''
    if (len(sys.argv) > 4):
        releasepath = sys.argv[4]
        print('Path to release .obj files: ' + releasepath)
    while True:
        if (len(releasepath) == 0):
            releasepath = input('Path to release .obj files: ')
        try:
            os.chdir(releasepath)
            break
        except (WindowsError, OSError):
            print('Oops, no such folder found')
            releasepath = ''

    updateProjectFile(projectname, projectpath, debugpath, 'Debug')
    updateProjectFile(projectname, projectpath, releasepath, 'Release')


# Updates the project file with .obj file names found from the given folder
# Expects projectname string, path to project file string, path to .obj files string and [Debug|Release] string
def updateProjectFile(projectname, projectpath, objpath, runMode):
    os.chdir(objpath)
    objfiles = list()
    contents = os.listdir()
    for file in contents:
        if ('.obj' in file):
            if (file != 'main.obj'):
                objfiles.append(file)

    os.chdir(projectpath)
    projectdirectory = os.listdir()
    for file in projectdirectory:
        fileUpdated = False
        if (projectname.lower() + '.vcxproj' == file.lower()):
            ET.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
            tree = ET.parse(file)
            root = tree.getroot()
            for itemDefinitionGroup in root.iter('{http://schemas.microsoft.com/developer/msbuild/2003}ItemDefinitionGroup'):
                if (runMode in itemDefinitionGroup.attrib.get('Condition')):
                    #Find additional dependencies
                    for additionalDependencies in itemDefinitionGroup.iter('{http://schemas.microsoft.com/developer/msbuild/2003}AdditionalDependencies'):
                        #Split dependencies into list
                        dependencies = additionalDependencies.text.split(';')
                        #Separate dependency types
                        libs = [ x for x in dependencies if '.lib' in x ]
                        additional = [ x for x in dependencies if '.lib' not in x and '.obj' not in x ]
                        #Append items back to the list
                        del dependencies[:]
                        dependencies.extend(libs)
                        dependencies.extend(objfiles)
                        dependencies.extend(additional)
                        #Turn list into string again
                        tempstr = ';'.join(dependencies)
                        original = additionalDependencies.text
                        if (original != tempstr):
                            additionalDependencies.text = tempstr
                            fileUpdated = True


            #Update file
            if (fileUpdated):
                tree.write(file, encoding="utf-8", xml_declaration=True)
                print('Updated ' + runMode + ' dependencies in ' + file)
            else:
                print(runMode + ' dependencies in ' + file + ' were already up-to-date')
            break


###########################################
#
# Script entry point
#
###########################################
if (__name__ == '__main__'):
    main()
