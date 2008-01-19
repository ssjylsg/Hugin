@echo off
        rem Specify additional parameters you want to pass to enblend in the next line 
        rem (must be one line)
set enblend_additional_parameters= -l 20 -a 
        rem Set 1 in next line to 0 if you don't want to use exiftool to copy exif info
set use_exiftool=1
echo enblend with exif droplet batch file version 0.3.1
echo copyright (c) 2008 Erik Krause - http://www.erik-krause.de
        rem we need command extensions enabled
verify OTHER 2>nul
setlocal ENABLEEXTENSIONS
if ERRORLEVEL 1 goto :No_Extensions_Error
if not exist enblend.exe goto :No_enblend_Error
        rem if called directly (no command line arguments) display some usage hints
if "%~1" == "" goto :print_help
echo %1|findstr ! >nul
if not errorlevel 1 goto :illegal_character
echo call without parameters for usage hints.
echo.
        rem check existence of exiftool
if not exist exiftool.exe (
  echo ExifTool not found - EXIF info will not be copied!
  set use_exiftool=0
)  
        rem check existence of exiftool additional arguments file
if not exist exiftool_enblend_args.txt (
  echo exiftool_enblend_args.txt not found - EXIF info will not be copied!
  set use_exiftool=0
)  
        rem check existence of exiftool LV collection helper batch file
if not exist collect_data_enblend.bat (
  echo collect_data_enblend.bat helper batch file not found - EXIF info will not be copied!
  set use_exiftool=0
)  
        rem check if command line argument is a folder 
pushd "%~1" 2>nul       
popd
if not errorlevel 1 goto :IsDir
        rem apparently these are files: fusion them.
enblend.exe %enblend_additional_parameters% -o "%~dpn1_enblended.tif" %*
        rem inform user
if %use_exiftool%==1 echo EXIFTool collecting data from %*
        rem call helper batch to collect file names and light values - have them passed to exiftool
if %use_exiftool%==1 for /F "usebackq delims=" %%i IN (`collect_data_enblend.bat %*`) DO (
  exiftool.exe -TagsFromFile "%~1" -@ exiftool_enblend_args.txt -ImageDescription="%%i" "%~dpn1_enblended.tif" 
)  
        rem that's all
goto :ready
        rem we have a folder
:IsDir
        rem we need delayed environment variable expansion in order to count files
setlocal ENABLEDELAYEDEXPANSION
echo.        
        rem Inform user ...
echo Processed directory: %1\
echo. 
echo Please specify number of files in one series to pass to enblend
echo or "A" for all files in the directory, then press Enter
        rem and ask what to do
set /P enblend_number_of_files=Your input: 
        rem use set enblend_number_of_files=3 to set the number fixed to 3 f.e.
        rem init variables
set enblend_files=
set exiftool_files=
set CNT=0
        rem init temporary batch file. 
        rem we need this in order to avoid that the created files are included as well
echo @echo off > $$$_enblend_temp_$$$.bat
echo REM automatically created batch file for enblend folder processing>> $$$_enblend_temp_$$$.bat 
        rem loop over all jpg and tif files in the respective folder 
for %%I in ("%~1\*.tif") do call :loop "%%I"
        rem skip subroutine
goto skip_loop
        rem a subroutine
:loop
        rem increment count
  set /A CNT+=1
        rem remember the first file name of each series  
  if !CNT! EQU 1 (
    set enblend_first_file=%~dpn1
    set exiftool_first_file=%~1
  )  
        rem as long as number not reached...
        rem  add files to the command line    
  set enblend_files=!enblend_files! "%~1"
        rem and add files to the output line
  set exiftool_files=!exiftool_files! '%~nx1'
  if !CNT! GEQ %enblend_number_of_files% ( 
        rem if number reached write the result to the temp batch file. 
    echo enblend.exe %enblend_additional_parameters% -o "%enblend_first_file%_enblended.tif" %enblend_files%>> $$$_enblend_temp_$$$.bat 
        rem inform user
    if %use_exiftool%==1 echo EXIFTool collecting data from %exiftool_files%
        rem write exiftool call including collected data to temp batch
    if %use_exiftool%==1 for /F "usebackq delims=" %%i IN (`collect_data_enblend.bat %enblend_files%`) DO echo exiftool.exe -TagsFromFile "%exiftool_first_file%" -@ exiftool_enblend_args.txt -ImageDescription="%%i" "%enblend_first_file%_enblended.tif">> $$$_enblend_temp_$$$.bat
        rem init variables
    set enblend_files=
    set exiftool_files=
    set CNT=0
  )
goto :eof
        rem end of subroutine  
:skip_loop
        rem if there are still files left, add them as well
if !CNT! NEQ 0 (
  echo enblend.exe %enblend_additional_parameters% -o "%enblend_first_file%_enblended.tif" %enblend_files%>> $$$_enblend_temp_$$$.bat
        rem inform user
  if %use_exiftool%==1 echo EXIFTool collecting data from %exiftool_files%
        rem write exiftool call including collected data to temp batch
  if %use_exiftool%==1 for /F "usebackq delims=" %%i IN (`collect_data_enblend.bat %enblend_files%`) DO echo exiftool.exe -TagsFromFile "%exiftool_first_file%" -@ exiftool_enblend_args.txt -ImageDescription="%%i" "%enblend_first_file%_enblended.tif">> $$$_enblend_temp_$$$.bat
)
        rem call temporary batch file to execute enblend
call $$$_enblend_temp_$$$.bat
        rem this file stays in the enblend folder and is overwritten the next time
        rem that's it
goto :ready
:No_Extensions_Error
echo.
echo Unable to enable command extensions
echo.
echo either you have a very old command interpreter 
echo or you use a too old windows version
echo Sorry, this won't work on your machine...
echo.
pause
goto :ready
:No_enblend_Error
echo.
echo Unable to find enblend.exe
echo.
echo Make sure this batch file is in the same folder as enblend.exe
echo.
pause
goto :ready
:illegal_character
echo Sorry, path or file name contains a ! character. 
echo.
echo This can not be processed. Please rename an start again.
pause
goto :ready
:print_help
echo.
echo Usage:
echo.
echo Create a shortcut for this batch file on the desktop or in any folder.
echo and give it a speaking name.
echo The batchfile itself must reside in the same folder as enblend.exe
echo.
echo There are two ways to use this droplet:
echo 1. drag and drop some images on the droplet to enblend them.
echo    The result will be a .tif file named like the one image of the series 
echo    you clicked on for dragging plus _enblended added to the file name.
echo 2. drag and drop a folder on the droplet. You will be asked to
echo    specify how many images are in a series or whether 
echo    you want to enblend all images in the folder.
echo    The result will be .tif files named like the first image of each 
echo    series plus _enblended added to the file name.
echo.
echo Have fun
echo Erik Krause
echo. 
pause
:ready
