##################################################################################
#
# Sampo Siltanen 
# 2017
# 
# This script crawls through C++ source files within given directory and produces 
# comment templates for functions in .h file based on the corresponding .cpp file.
# The comment template are formatted so that doxygen can read them
#
# Use of this software is at the user's own risk. 
# I do not take any responsibility for mishaps caused by the use of this software.
#
#################################################################################



import os
import re
import sys

#Class to represent functions in cpp code
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
            print('Oops, error in recursive folder processing')
    
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
    preconditions = list()
    postconditions = list()
    
    #Variables to keep track of function begin and end, and comment blocks
    openBrackets = 0
    closedBrackets = 0
    openParentheses = 0
    closedParentheses = 0
    commentsBlock = False
    
    #Loop through cpp file
    prevLineTemp = ""
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
        
        if (openBrackets == closedBrackets):
            #Manage functions with open bracket at the end of the line
            if ('{' in line):
                openBrackets += line.count('{')
                closedBrackets += line.count('}')
                #Leave implementation out
                line = line[:line.index('{')]
            
            openParentheses += line.count('(')
            closedParentheses += line.count(')')
            prevLineTemp += line.lstrip(' \t').rstrip('\n')
            if (openParentheses == closedParentheses):
                if (len(prevLineTemp) > 0):
                    #Attach pre and postconditions to the previously found function definition
                    if (len(functions) > 0):
                        functions[-1].pres = preconditions[:]
                        functions[-1].posts = postconditions[:]
                    #append the new function
                    functions.append(Function(prevLineTemp))
                prevLineTemp = ""
                del preconditions[:]
                del postconditions[:]
            continue
        
        #Line does not hold function title
        else:
            if ('REQUIRE' in line):
                preconditions.append(line[line.index('(') + 1:line.rindex(')')])
            if ('ENSURE' in line):
                postconditions.append(line[line.index('(') + 1:line.rindex(')')])
            openBrackets += line.count('{')
            closedBrackets += line.count('}')
            continue

    #Attach pre and postconditions to the last found function definition
    #This is done afterwards because the definition is found before the contracts are found
    if (len(functions) > 0):
        functions[-1].pres = preconditions[:]
        functions[-1].posts = postconditions[:]

    return functions

#Parses function definition into components
#Returns list of ProcessedFunction objects
def processFunctions(functions):
    processedFuncs = list()
    for func in functions:
        #Class member func
        d = func.definition
        className = ""
        returnValue = ""
        funcName = ""
        classlessDef = ""
        #Handle member functions
        if ('::' in d):
            #Builders and destructors dont have a return value
            if (' ' not in d[:d.index('::')]):
                className = d[:d.index('::')]
                classlessDef = d[d.index('::') + 2:]
            else:
                returnValue = d[:d.index(' ')]
                className = d[d.index(' ') + 1:d.index('::')]
                classlessDef = d[:d.index(' ') + 1] + d[d.index('::') + 2:]
            funcName = d[d.index('::') + 2:d.index('(')]
        #Handle functions that are not members of a class
        else:
            returnValue = d[:d.index(' ')]
            funcName = d[d.index(' ') + 1:d.index('(')]
            classlessDef = d
        
        #Check if there are any parameters
        parameterNames = list()
        if (len(d[d.index('(') + 1:d.rindex(')')].strip(' ')) > 0):
            #Handle parameters
            #separate each parameter
            parameters = [p.rstrip(' ') for p in d[d.index('(') + 1:d.rindex(')')].split(',')]
            #collect names of parameters
            parameterNames = [p[p.rindex(' ') + 1:] for p in parameters]
        
        processedFuncs.append(ProcessedFunction(classlessDef, returnValue, className, funcName, parameterNames, func.pres, func.posts))

    return processedFuncs


#Searches the values of list for a substring
#returns the full string from list if found
#otherwise returns empty string
def getLineFromListBySubstring(lst, substr):
    for s in lst:
        if (substr in s):
            return s
    return ''

#Adds comment templates to functions in header file if:
#   There is a cpp file for the header
#   There isn't comments already for the function in header file
#Returns true if file was updated
#Returns false if file was not updated
def processHeaderFile(filename):
    functions = crawlFunctionDefinitions(filename)
    if (len(functions) > 0):
        processedFuncs = processFunctions(functions)
        
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
                        #Builder is an expection because it passes the previous condition when compared to destructor
                        if (f.name == f.className and '~' in strippedLine):
                            continue
                        
                        #Set formatting of the list
                        if(len(upd_hfile[-1].strip()) > 0):
                            newLines.append('')
                        indent = '\t ' if re.match(r'\s', line) else ' '
                        newLines.append(indent[:-1] + '/**')
                        
                        #Update existing comments if there were any
                        if(len(existingComments) > 0):
                            commentLine = getLineFromListBySubstring(existingComments, '* \\brief')
                            newLines.append(indent + (commentLine.strip() if len(commentLine) > 0 else '* \\brief ' + f.name))
                            
                            for p in f.parameterNames:
                                commentLine = getLineFromListBySubstring(existingComments, '* \\param ' + p)
                                newLines.append(indent + (commentLine.strip() if len(commentLine) > 0 else '* \\param ' + p))
                                
                            for pre in f.preconditions:
                                commentLine = getLineFromListBySubstring(existingComments, '* \\pre ' + pre)
                                newLines.append(indent + (commentLine.strip() if len(commentLine) > 0 else '* \\pre ' + pre))
                                
                            for post in f.postconditions:
                                commentLine = getLineFromListBySubstring(existingComments, '* \\post ' + post)
                                newLines.append(indent + (commentLine.strip() if len(commentLine) > 0 else '* \\post ' + post))
                            
                            if(f.returnValue != 'void' and f.returnValue != ''):
                                commentLine = getLineFromListBySubstring(existingComments, '* \\return')
                                newLines.append(indent + (commentLine.strip() if len(commentLine) > 0 else '* \\return '))
                            
                            del existingComments[:]
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
                        
                        newLines.append(indent + '*/')
                        processedFuncs.remove(f)
            
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
#Script entry point
#
###########################################
if (__name__ == "__main__"):
	path = ''
	if (len(sys.argv) > 1):
		print('Entered path: ' + sys.argv[1])
		path = sys.argv[1]
	while True:
		if (len(path) == 0):
			path = input("Enter path to folder: ")
		else:
			print(path)
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
		if (processHeaderFile(file)):
			updatedFilesCount += 1
	print('Total of ', updatedFilesCount, ' files were updated')
