# CodeCommenter

CodeCommenter is a python script that recursively finds all C++ source files within given (command line or run-time) directory. It searches all .h files and crawls through all matching .cpp files to collect all necessary information to create comments in .h file for the functions there. The crawling is done from .cpp files to document preconditions and postconditions of functions as well as the information found on the function declaration. Preconditions are recognized by the keyword REQUIRE and postconditions by ENSURE. The output comments are formatted so that Doxygen can read them.

Software asks for two paths:   
   1. Path to where it creates the backup files    
   2. Path from where it searches for the files    

User may give these in this order as command line parameters or during run-time. If no parameters are provided or they are not valid, the user is asked for new paths.

The software does not detect file name collisions when making backups but instead renames the files with seconds and microseconds from timestamp to make sure they are unique.

This project started as a side project to my other project (Blocker). In that project I wanted to create automatized documentations of the preconditions and postconditions as I wanted to use Design by Contract principle in my project and avoid duplicating the condition information to comments as it was prone to in accuracies. By using this script, it is possible to always have up-to-date documentation of my function contracts and also generate comment templates for the functions.

Use of this software is at the user's own risk. I do not take any responsibility for mishaps caused by the use of this software.