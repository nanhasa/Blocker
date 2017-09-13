This script crawls through Visual Studio build intermediate target folder    
and fetches the .obj file names for Debug and Release run modes and   
adds them to additional dependencies in .vsxproj file   
   
This software asks for four parameters   
  1. Project file name to be updated   
  2. Path to the project file   
  3. Path to Debug .obj files   
  4. Path to Release .obj files   
   
User may give these parameters as command line parameters in that order or   
otherwise the software asks for them during run time.   
    
Use of this software is at the user's own risk.   
I do not take any responsibility for mishaps caused by the use of this software.   