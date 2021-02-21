#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug_RPI
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/backend/backend.o \
	${OBJECTDIR}/src/frontend/allegro/draw.o \
	${OBJECTDIR}/src/frontend/allegro/front.o \
	${OBJECTDIR}/src/frontend/allegro/logic.o \
	${OBJECTDIR}/src/frontend/raspi/Raspi.o \
	${OBJECTDIR}/src/frontend/raspi/menu.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/local/lib `pkg-config --libs sdl` `pkg-config --libs sdl2`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/spaceinvaders

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/spaceinvaders: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/spaceinvaders ${OBJECTFILES} ${LDLIBSOPTIONS} libs/disdrv.o libs/joydrv.o libs/termlib.o

${OBJECTDIR}/src/backend/backend.o: src/backend/backend.c
	${MKDIR} -p ${OBJECTDIR}/src/backend
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -DRASPI -I/usr/local/include `pkg-config --cflags sdl` `pkg-config --cflags sdl2`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/backend/backend.o src/backend/backend.c

${OBJECTDIR}/src/frontend/allegro/draw.o: src/frontend/allegro/draw.c
	${MKDIR} -p ${OBJECTDIR}/src/frontend/allegro
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -DRASPI -I/usr/local/include `pkg-config --cflags sdl` `pkg-config --cflags sdl2`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/allegro/draw.o src/frontend/allegro/draw.c

${OBJECTDIR}/src/frontend/allegro/front.o: src/frontend/allegro/front.c
	${MKDIR} -p ${OBJECTDIR}/src/frontend/allegro
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -DRASPI -I/usr/local/include `pkg-config --cflags sdl` `pkg-config --cflags sdl2`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/allegro/front.o src/frontend/allegro/front.c

${OBJECTDIR}/src/frontend/allegro/logic.o: src/frontend/allegro/logic.c
	${MKDIR} -p ${OBJECTDIR}/src/frontend/allegro
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -DRASPI -I/usr/local/include `pkg-config --cflags sdl` `pkg-config --cflags sdl2`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/allegro/logic.o src/frontend/allegro/logic.c

${OBJECTDIR}/src/frontend/raspi/Raspi.o: src/frontend/raspi/Raspi.c
	${MKDIR} -p ${OBJECTDIR}/src/frontend/raspi
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -DRASPI -I/usr/local/include `pkg-config --cflags sdl` `pkg-config --cflags sdl2`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/raspi/Raspi.o src/frontend/raspi/Raspi.c

${OBJECTDIR}/src/frontend/raspi/menu.o: src/frontend/raspi/menu.c
	${MKDIR} -p ${OBJECTDIR}/src/frontend/raspi
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -DRASPI -I/usr/local/include `pkg-config --cflags sdl` `pkg-config --cflags sdl2`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/raspi/menu.o src/frontend/raspi/menu.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
