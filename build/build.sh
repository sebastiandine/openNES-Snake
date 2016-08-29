################################################################
# Variables (might differ from project to project)             #
################################################################
# Default-Project-Layout (might be the same in various projects)
WORKSPACE=$(dirname $(readlinkk -f $0))
WORKSPACE=${WORKSPACE%/*}
SOURCE_DIR=${WORKSPACE}/src
BUILD_DIR=${WORKSPACE}/build
GRAPHICS_DIR=${WORKSPACE}/gfx
CONFIG_DIR=${WORKSPACE}/config
NES_LIB_DIR=${WORKSPACE}/NESLibrary
TEMP_DIR=${BUILD_DIR}/temp

#Individual-Project-Settings
PROJECT_NAME=NES-Snake			#name of project-directory in $WORKSPACE-directory 
MAIN_FILE_NAME=snake			#name of main-'.c'-file in $SOURCE_DIR-directory, WITHOUT! extension
NES_ROM_MAP_NAME=nrom_128_horz.cfg	#name of rom-mapping-file in $CONFIG_DIR-directory
ASM_RESET_FILE=crt0			#name of 6502-assembler file in $SOURCE_DIR-directory, which contains
					#startup-memory-organization, WITHOUT! extension

################################################################
# Setting up temporary build environment                       #
################################################################
echo "Start compiling NES-Project ${PROJECT_NAME}"
echo "Creating temporary build-directory on ${WORKSPACE}${BUILD_DIR}temp"
mkdir ${TEMP_DIR}

echo "Copying data into temporary build-directory"
cp ${NES_LIB_DIR}/* ${TEMP_DIR}
cp ${CONFIG_DIR}/* ${TEMP_DIR}
cp ${GRAPHICS_DIR}/* ${TEMP_DIR}
cp ${SOURCE_DIR}/* ${TEMP_DIR}

###############################################################
# Compiling and Linking                                       #
###############################################################
cd ${TEMP_DIR}
echo "Start compiling and linking"
echo "Compile ${MAIN_FILE_NAME}.c and referenced files into 6502-Assembler file ${MAIN_FILE_NAME}.s"
cc65 -Oi ${MAIN_FILE_NAME}.c --add-source
echo "Compile NES-Reset/Memory-Managment 6502-Assembler file ${ASM_RESET_FILE}.s into NES-bytecode"
ca65 ${ASM_RESET_FILE}.s
echo "Compile ${MAIN_FILE_NAME}.s game 6502-Assembler file ${MAIN_FILE_NAME}.s into NES-bytecode"
ca65 ${MAIN_FILE_NAME}.s
echo "Link everything to NES-Game-File"
ld65 -v -o ../${MAIN_FILE_NAME}.nes -C ${NES_ROM_MAP_NAME} ${ASM_RESET_FILE}.o ${MAIN_FILE_NAME}.o nes.lib

##############################################################
# Delete temporary build-directory                           #
##############################################################
echo "Delete temporary build-directory"
cd ..
rm -rf ${TEMP_DIR} 

echo "Compiling project ${PROJECT_NAME} has been done. The final .nes-gamefile can be found at ${BUILD_DIR}/${MAIN_FILE_NAME}.nes"
