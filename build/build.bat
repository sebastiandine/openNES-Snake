@echo off
REM ################################################################
REM # Variables (might differ from project to project)             #
REM ################################################################
REM Default-Project-Layout (might be the same in various projects
SET WORKSPACE=%CD%
SET WORKSPACE=%WORKSPACE:~,-5%
SET SOURCE_DIR=%WORKSPACE%src
SET BUILD_DIR=%WORKSPACE%build
SET GRAPHICS_DIR=%WORKSPACE%gfx
SET SOUND_DIR=%WORKSPACE%sfx
SET CONFIG_DIR=%WORKSPACE%config
SET NES_LIB_DIR=%WORKSPACE%NESLibrary
SET TEMP_DIR=%BUILD_DIR%\temp

REM Individual-Project-Settings
REM name of project-directory in $WORKSPACE-directory 
SET PROJECT_NAME=NES-Snake
REM name of main-'.c'-file in $SOURCE_DIR-directory, WITHOUT! extension		
SET MAIN_FILE_NAME=snake
REM name of rom-mapping-file in $CONFIG_DIR-directory				
SET NES_ROM_MAP_NAME=nrom_128_horz.cfg	
REM name of 6502-assembler file in $SOURCE_DIR-directory, which contains startup-memory-organization,
REM WITHOUT! extension
SET ASM_RESET_FILE=crt0

REM ################################################################
REM # Setting up temporary build environment                       #
REM ################################################################
ECHO Start compiling NES-Project %PROJECT_NAME%
ECHO Creating temporary build-directory on %TEMP_DIR%
CD %BUILD_DIR%
MKDIR %TEMP_DIR%

ECHO Copying data into temporary build-directory
COPY %NES_LIB_DIR%\* %TEMP_DIR%
COPY %CONFIG_DIR%\* %TEMP_DIR%
COPY %GRAPHICS_DIR%\* %TEMP_DIR%
COPY %SOUND_DIR%\* %TEMP_DIR%
XCOPY /s %SOURCE_DIR%\* %TEMP_DIR%

REM ###############################################################
REM # Compiling and Linking                                       #
REM ###############################################################
CD %TEMP_DIR%
ECHO Start compiling and linking
ECHO Compile %MAIN_FILE_NAME%.c and referenced files into 6502-Assembler file %MAIN_FILE_NAME%.s
cc65 -Oi %MAIN_FILE_NAME%.c --add-source

ECHO Compile NES-Reset/Memory-Managment 6502-Assembler file %ASM_RESET_FILE%.s into NES-bytecode
ca65 %ASM_RESET_FILE%.s

ECHO Compile %MAIN_FILE_NAME%.s game 6502-Assembler file into NES-bytecode
ca65 %MAIN_FILE_NAME%.s

ECHO Link everything to NES-Game-File
ld65 -v -o %BUILD_DIR%\%MAIN_FILE_NAME%.nes -C %NES_ROM_MAP_NAME% %ASM_RESET_FILE%.o %MAIN_FILE_NAME%.o nes.lib

REM ##############################################################
REM # Delete temporary build-directory                           #
REM ##############################################################
ECHO Delete temporary build-directory
CD ..
RMDIR /q/s %TEMP_DIR% 

ECHO Compiling project %PROJECT_NAME% has been done. The final .nes-gamefile can be found at %BUILD_DIR%\%MAIN_FILE_NAME%.nes
PAUSE