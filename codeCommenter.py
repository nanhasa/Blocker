#############################################################################################
#
# Sampo Siltanen
# 2017
#
# This script crawls through C++ source files within given directory and produces
# comment templates for functions in .h file based on the corresponding .cpp file.
# The comment template are formatted so that doxygen can read them.
#
# Software asks for two paths:
#   1. Path to where it creates the backup files
#   2. Path from where it searches for the files
#
# User may give these in this order as command line parameters or during run-time.
# If no parameters are provided or they are not valid, the user is asked for new paths.
#
# The software does not detect file name collisions when making backups but instead
# renames the files with seconds and microseconds from timestamp to make sure they are unique.
#
# Use of this software is at the user's own risk.
# I do not take any responsibility for mishaps caused by the use of this software.
#
#############################################################################################

import os
import datetime
import re
import sys
from shutil import copy2

#Class to represent functions in cpp code
#Builder parameter is the function definition e.g. 'int getHeight() const'
#For builders leave out initialization section i.e. everything following :
class Function(object):
    def __init__(self, definition):
        self.definition = definition
        self.pres = list()
        self.posts = list()

#Class to represent parsed functions
class ProcessedFunction(object):
    def __init__(self, classlessDef, returnVal, className, name, parameterNames, preconds, postconds):
        self.fullClasslessDefinition = classlessDef
        self.returnValue = returnVal
        self.className = className
        self.name = name
        self.parameterNames = parameterNames[:]
        self.preconditions = preconds[:]
        self.postconditions = postconds[:]


#Main function for this script
def main():
    #Process path to backup files
    bupath = ''
    if (len(sys.argv) > 2):
        print('Entered path for backup files: ' + sys.argv[1])
        bupath = sys.argv[1]
    while True:
        if (len(bupath) == 0):
            bupath = input("Enter path to backup file folder: ")
        try:
            os.chdir(bupath)
            break
        except (WindowsError, OSError):
            print('Oops, no such folder found')
            bupath = ''

    #Create name for backup directory
    bupath = os.path.join(os.getcwd(), 'CodeCommenter_bu_' + datetime.datetime.now().strftime('%Y-%m-%d_%H-%M-%S') + '\\')

    #Process path from where files are loaded
    path = ''
    if (len(sys.argv) > 2):
        print('Entered path: ' + sys.argv[2])
        path = sys.argv[2]
    while True:
        if (len(path) == 0):
            path = input("Enter path to folder: ")
        try:
            os.chdir(path)
            break
        except (WindowsError, OSError):
            print('Oops, no such folder found')
            path = ''

    #Process contents of working directory
    headerFiles = searchFilesRecursively()
    updatedFilesCount = 0
    for file in headerFiles:
        if (processHeaderFile(file, bupath)):
            updatedFilesCount += 1
    print('Total of ', updatedFilesCount, ' files were updated')



#Fetches paths to all header files in current working directory and directories in it
def searchFilesRecursively():
    contents = os.listdir()
    headers = list()
    directories = list()
    #process files in current folder before recursive folder processing
    for item in contents:
        if ('.h' in item):
            headers.append('{}\{}'.format(os.getcwd(), item))
        elif ('.' not in item):
            directories.append(item)

    #Process folders recursively
    for directory in directories:
        try:
            os.chdir('.\\' + directory)
            headers += searchFilesRecursively()
            os.chdir('..')
        except (WindowsError, OSError):
            print('Oops, error in recursive folder processing. ' + directory + ' was not a folder')

    return headers




#Crawls through the cpp file paired with the .h file given
#If successful, will return a list of Function objects that describe the functions in the file
def crawlFunctionDefinitions(filename):
    #Return if there was not .cpp file for the header
    if (not os.path.isfile(filename[:-1] + 'cpp')):
        return list()

    #Read cpp file
    cpp = open(filename[:-1] + 'cpp', "r")
    cppfile = cpp.readlines()
    cpp.close()

    #List to hold Function objects
    functions = list()

    #Variables to keep track of function begin and end, and comment blocks
    bracketsOpen = 0
    bracketsClosed = 0
    bracketsOpenPrevLine = -1
    commentsBlock = False

    #Variables to keep track of active namespaces (they tend to mess the bracket counts)
    namespaceLinetemp = ""
    activeNamespaces = 0

    #List to hold functions
    fullfunctions = list()

    #Loop through cpp file to collect functions to a list
    for line in cppfile:
        #skip comments
        if (commentsBlock and '*/' not in line):
            continue
        if ('//' in line):
            line = line[:line.index('//')]
        if ('/*' in line):
            if ('*/' in line):
                line = line[:line.index('/*')] + line[line.index('*/') + 2:]
            else:
                line = line[:line.index('/*')]
                commentsBlock = True
        elif ('*/' in line):
            line = line[line.index('*/') + 2:]
            commentsBlock = False

        #Skip line if it is empty after cleaning the comments from it
        if (len(line.strip()) == 0):
            continue

        #Skip includes and declares
        if (line.lstrip()[:8] == '#include' or line.lstrip()[:8] == '#declare'):
            continue

        #Check for namespaces since they mess up the bracket counting
        if (line.lstrip()[:9] == 'namespace'):
            if ('{' not in line):
                namespaceLinetemp += line
            else:
                namespaceLinetemp = ""
                activeNamespaces += 1
                continue

        #Update bracket count
        bracketsOpen += line.count('{')
        bracketsClosed += line.count('}')

        #Balance bracket amounts with activeNamespaces because the opening bracket of namespace was ignored
        #There are less bracketsOpen than bracketsClosed when namespace closes
        if (bracketsOpen < bracketsClosed):
            activeNamespaces -= 1
            bracketsClosed -= 1
            if (activeNamespaces < 0):
                print('Something went wrong with active namespace counter')
            continue

        line = line.strip('\t\n\r')

        #New function definition when there is equal amount of brackets
        if (bracketsOpen == bracketsClosed):
            #If there is no previous row, append new
            if (bracketsOpenPrevLine == -1):
                fullfunctions.append(line)
                #If there is an opening bracket, update previous count
                if (bracketsOpen == 0):
                    bracketsOpenPrevLine = bracketsOpen
                    continue
            #Case when the function ends
            else:
                fullfunctions[-1] += line

            bracketsOpen = 0
            bracketsClosed = 0
            bracketsOpenPrevLine = -1
        #Append last one when the bracket counts don't match
        else:
            #But if there is no previous row, then append new
            if (bracketsOpenPrevLine == -1):
                fullfunctions.append(line)
            else:
                fullfunctions[-1] += line
            bracketsOpenPrevLine = bracketsOpen


    for f in fullfunctions:
        startBracket = f.index('{')

        pattern = re.compile(r'[^:](:)[^:]') #Find separator for builder initialization
        match = pattern.search(f)
        if (match):
            if (match.start() < startBracket): #If there was separator, choose is it used or first bracket
                functions.append(Function(f[:match.start() + 1]))
            else:
                functions.append(Function(f[:startBracket]))
        else:
            functions.append(Function(f[:startBracket]))

        #Extract preconditions of from the function body
        pattern = re.compile(r'REQUIRE\s*\((?P<condition>.+?)\);')
        for pre in re.finditer(pattern, f):
            functions[-1].pres.append(pre.group('condition'))

        #Extract postconditions of from the function body
        pattern = re.compile(r'ENSURE\s*\((?P<condition>.+?)\);')
        for post in re.finditer(pattern, f):
            functions[-1].posts.append(post.group('condition'))

    return functions

#Parses function declaration into components
#Returns list of ProcessedFunction objects
def processFunctionsDefinitions(functions):
    processedFuncs = list()
    for func in functions:
        #Class member func
        defn = func.definition
        className = ""
        returnValue = ""
        funcName = ""
        classlessDef = ""

        #Extract function name
        pattern = re.compile(r'[^(::|\s)]+(?=\s*?\()') #Find the name of the function
        match = pattern.search(defn)
        if (not match):
            #In case the function name could not be found, stop processing
            #and set default value to classlessDef because it is used to 
            #map functions to ones in .h file
            print('Error extracting function name from definition: ' + defn)
            funcName = 'ERROR'
            classlessDef = defn
            continue
        
        funcName = match.group(0)
        nameIdx = match.start()
        if (nameIdx < 3): #Sanity check
            print('Error in function name match index definition: idx ' + nameIdx + ' ' + defn)
            funcName = 'ERROR'
            classlessDef = defn
            continue
            
        #Extract class name
        #Nonmember function has space before function name
        classIdx = -1
        if (defn[nameIdx - 1] != ' '):
            pattern = re.compile(r'\w+$')
            match = pattern.search(defn[:nameIdx - 2]) #Leave out colons before function name
            if (not match):
                print('Error in extracting class name: ' + defn)
            else:
                className = match.group(0)
                classIdx = match.start()
                classlessDef = defn[: classIdx] + defn[classIdx + len(className) + 2 : ]
        else:
            classlessDef = defn
        
        #Extract return value
        if (classIdx == -1): #Nonmember function
            returnValue = "".join(defn[:nameIdx].rstrip().split())
        else:
            returnValue = "".join(defn[:classIdx].rstrip().split())
        
        #Check if there are any parameters
        parameterNames = list()
        if (len(defn[defn.index('(') + 1 : defn.rindex(')')].strip(',')) > 0):
            #Handle parameters
            #separate each parameter
            parameters = [p.strip(' ') for p in defn[defn.index('(') + 1 : defn.rindex(')')].split(',')]
            #collect names of parameters
            parameterNames = [p[p.rindex(' ') + 1:] for p in parameters]

        processedFuncs.append(ProcessedFunction(classlessDef, returnValue, className, funcName, parameterNames, func.pres, func.posts))

    return processedFuncs


#Searches oldComments list to find comment section based on substr
#Appends outputList with reorganizes comments
def getUpdatedCommentSection(oldComments, outputList, indent, substr):
    oldCommentSection = list()
    found = False
    for s in oldComments:
        #Append multiline comment
        if (found and '\\' not in s):
            oldCommentSection.append(s)
        #Next comment section started
        if (found and '\\' in s):
            break
        #Comment section by keyword found
        if (substr in s):
            oldCommentSection.append(s)
            found = True

    #Append output list with either new comment row or with existing comments
    if(len(oldCommentSection) == 0):
        outputList.append(indent + substr)
    else:
        for l in oldCommentSection:
            if (len(l.strip()) > 0):
                outputList.append(indent + l.strip())


#Adds comment templates to functions in header file if:
#   There is a cpp file for the header
#   There isn't comments already for the function in header file
#Returns true if file was updated
#Returns false if file was not updated
def processHeaderFile(filename, backupPath):
    functions = crawlFunctionDefinitions(filename)
    if (len(functions) > 0):
        processedFuncs = processFunctionsDefinitions(functions)

        h = open(filename, "r")
        hfile = h.readlines()
        h.close()
        upd_hfile = list()

        commentsBlock = False
        existingComments = list()
        newLines = list()
        multilineFuncDef = list()
        isMultilineFunc = False
        for line in hfile:

            #Handle functions that are split on several lines
            if (line.count('(') > line.count(')')):
                multilineFuncDef.append(line.rstrip())
                isMultilineFunc = True
                continue
            elif (line.count('(') < line.count(')')):
                multilineFuncDef.append(line.rstrip())
                isMultilineFunc = False
            if (isMultilineFunc):
                multilineFuncDef.append(line.rstrip())
                continue

            #Join the multiline function to single line
            if(len(multilineFuncDef) > 0):
                line = ""
                for l in multilineFuncDef:
                    line += l

            #Create line without whitespaces in order to compare data in lines reliably
            strippedLine = "".join(line.split())

            #Parse existing comments
            if ('/**' in strippedLine[:3]):
                commentsBlock = True
                continue
            if ('*/' in strippedLine[:2]):
                commentsBlock = False
                continue
            if (commentsBlock):
                existingComments.append(line)
                continue

            if (';' in line):
                for f in processedFuncs:
                    #Match function definitions without spaces and tabs
                    #In header version there are additional keywords like extern etc -> search for substring
                    if ("".join(f.fullClasslessDefinition.split()) in strippedLine):

                        #Builder is an expcetion because it passes the previous condition when compared to destructor
                        if (f.name == f.className and '~' in strippedLine):
                            continue

                        #Set formatting of the list
                        if(len(upd_hfile[-1].strip()) > 0):
                            newLines.append('')
                        p = re.compile(r'^\s*')
                        match = p.search(line)
                        indent = match.group(0) + ' ' if (match) else ' '
                        newLines.append(indent[:-1] + '/**')

                        #Update existing comments if there were any
                        if(len(existingComments) > 0):
                            #Brief comment section
                            getUpdatedCommentSection(existingComments, newLines, indent, '* \\brief')

                            #Parameter comment sections
                            for p in f.parameterNames:
                                getUpdatedCommentSection(existingComments, newLines, indent, '* \\param ' + p)

                            #Preconditions comment sections
                            for pre in f.preconditions:
                                getUpdatedCommentSection(existingComments, newLines, indent, '* \\pre ' + pre)

                            #Postconditions comments sections
                            for post in f.postconditions:
                                getUpdatedCommentSection(existingComments, newLines, indent, '* \\post ' + post)

                            #Return value comment section
                            if(f.returnValue != 'void' and f.returnValue != ''):
                                getUpdatedCommentSection(existingComments, newLines, indent, '* \\return')

                        #No existing comments, create new ones
                        else:
                            newLines.append(indent + '* \\brief ' + f.name)

                            for p in f.parameterNames:
                                newLines.append(indent + '* \\param ' + p)

                            for pre in f.preconditions:
                                newLines.append(indent + '* \\pre ' + pre)

                            for post in f.postconditions:
                                newLines.append(indent + '* \\post ' + post)

                            if (f.returnValue != 'void' and f.returnValue != ''):
                                newLines.append(indent + '* \\return ')

                        #Comment ending characters
                        newLines.append(indent + '*/')
                        #Delete processed data
                        processedFuncs.remove(f)
                        del existingComments[:]

            #If new comment lines were formed, append them before the current line
            if (len(newLines) > 0):
                upd_hfile.extend(newLines)
                del newLines[:]

            #If function was split on multiple lines, append the multiline version
            if (len(multilineFuncDef) > 0):
                upd_hfile.extend(multilineFuncDef)
                del multilineFuncDef[:]
            else:
                upd_hfile.append(line.rstrip())

        #Create backup directory only when it is actually needed
        if not os.path.exists(backupPath):
            os.makedirs(backupPath)

        #Create backup version of the file being updated
        copy2(filename, backupPath + '\\' + filename[filename.rfind('\\') + 1:] + '.bak_' + datetime.datetime.now().strftime('%S%f'))

        #Write the processed lines back to file
        newFile = open(filename, "w")
        for row in upd_hfile:
            newFile.write("%s\n" % row)
        newFile.close()

        return True
    else:
        return False




###########################################
#
# Script entry point
#
###########################################
if (__name__ == "__main__"):
    main()
