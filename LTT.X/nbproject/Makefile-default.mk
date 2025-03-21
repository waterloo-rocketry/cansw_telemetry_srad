#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/LTT.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/LTT.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c leds.c adc.c spi.c cc1200.c canlib/pic18f26k83/pic18f26k83_can.c canlib/util/can_rcv_buffer.c canlib/util/can_tx_buffer.c canlib/util/safe_ring_buffer.c canlib/util/timing_util.c canlib/message/msg_actuator.c canlib/message/msg_common.c canlib/message/msg_general.c canlib/message/msg_gps.c canlib/message/msg_recovery.c canlib/message/msg_sensor.c canlib/message/msg_state_est.c rocketlib/pic18f26k83/timer.c clockInit.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.p1 ${OBJECTDIR}/leds.p1 ${OBJECTDIR}/adc.p1 ${OBJECTDIR}/spi.p1 ${OBJECTDIR}/cc1200.p1 ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1 ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1 ${OBJECTDIR}/canlib/util/can_tx_buffer.p1 ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1 ${OBJECTDIR}/canlib/util/timing_util.p1 ${OBJECTDIR}/canlib/message/msg_actuator.p1 ${OBJECTDIR}/canlib/message/msg_common.p1 ${OBJECTDIR}/canlib/message/msg_general.p1 ${OBJECTDIR}/canlib/message/msg_gps.p1 ${OBJECTDIR}/canlib/message/msg_recovery.p1 ${OBJECTDIR}/canlib/message/msg_sensor.p1 ${OBJECTDIR}/canlib/message/msg_state_est.p1 ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1 ${OBJECTDIR}/clockInit.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/main.p1.d ${OBJECTDIR}/leds.p1.d ${OBJECTDIR}/adc.p1.d ${OBJECTDIR}/spi.p1.d ${OBJECTDIR}/cc1200.p1.d ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1.d ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1.d ${OBJECTDIR}/canlib/util/can_tx_buffer.p1.d ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1.d ${OBJECTDIR}/canlib/util/timing_util.p1.d ${OBJECTDIR}/canlib/message/msg_actuator.p1.d ${OBJECTDIR}/canlib/message/msg_common.p1.d ${OBJECTDIR}/canlib/message/msg_general.p1.d ${OBJECTDIR}/canlib/message/msg_gps.p1.d ${OBJECTDIR}/canlib/message/msg_recovery.p1.d ${OBJECTDIR}/canlib/message/msg_sensor.p1.d ${OBJECTDIR}/canlib/message/msg_state_est.p1.d ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1.d ${OBJECTDIR}/clockInit.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.p1 ${OBJECTDIR}/leds.p1 ${OBJECTDIR}/adc.p1 ${OBJECTDIR}/spi.p1 ${OBJECTDIR}/cc1200.p1 ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1 ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1 ${OBJECTDIR}/canlib/util/can_tx_buffer.p1 ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1 ${OBJECTDIR}/canlib/util/timing_util.p1 ${OBJECTDIR}/canlib/message/msg_actuator.p1 ${OBJECTDIR}/canlib/message/msg_common.p1 ${OBJECTDIR}/canlib/message/msg_general.p1 ${OBJECTDIR}/canlib/message/msg_gps.p1 ${OBJECTDIR}/canlib/message/msg_recovery.p1 ${OBJECTDIR}/canlib/message/msg_sensor.p1 ${OBJECTDIR}/canlib/message/msg_state_est.p1 ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1 ${OBJECTDIR}/clockInit.p1

# Source Files
SOURCEFILES=main.c leds.c adc.c spi.c cc1200.c canlib/pic18f26k83/pic18f26k83_can.c canlib/util/can_rcv_buffer.c canlib/util/can_tx_buffer.c canlib/util/safe_ring_buffer.c canlib/util/timing_util.c canlib/message/msg_actuator.c canlib/message/msg_common.c canlib/message/msg_general.c canlib/message/msg_gps.c canlib/message/msg_recovery.c canlib/message/msg_sensor.c canlib/message/msg_state_est.c rocketlib/pic18f26k83/timer.c clockInit.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/LTT.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F26K83
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/leds.p1: leds.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/leds.p1.d 
	@${RM} ${OBJECTDIR}/leds.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/leds.p1 leds.c 
	@-${MV} ${OBJECTDIR}/leds.d ${OBJECTDIR}/leds.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/leds.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/adc.p1: adc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.p1.d 
	@${RM} ${OBJECTDIR}/adc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/adc.p1 adc.c 
	@-${MV} ${OBJECTDIR}/adc.d ${OBJECTDIR}/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/spi.p1: spi.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/spi.p1.d 
	@${RM} ${OBJECTDIR}/spi.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/spi.p1 spi.c 
	@-${MV} ${OBJECTDIR}/spi.d ${OBJECTDIR}/spi.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/spi.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cc1200.p1: cc1200.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cc1200.p1.d 
	@${RM} ${OBJECTDIR}/cc1200.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/cc1200.p1 cc1200.c 
	@-${MV} ${OBJECTDIR}/cc1200.d ${OBJECTDIR}/cc1200.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/cc1200.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1: canlib/pic18f26k83/pic18f26k83_can.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/pic18f26k83" 
	@${RM} ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1.d 
	@${RM} ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1 canlib/pic18f26k83/pic18f26k83_can.c 
	@-${MV} ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.d ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/util/can_rcv_buffer.p1: canlib/util/can_rcv_buffer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/util" 
	@${RM} ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1.d 
	@${RM} ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1 canlib/util/can_rcv_buffer.c 
	@-${MV} ${OBJECTDIR}/canlib/util/can_rcv_buffer.d ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/util/can_tx_buffer.p1: canlib/util/can_tx_buffer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/util" 
	@${RM} ${OBJECTDIR}/canlib/util/can_tx_buffer.p1.d 
	@${RM} ${OBJECTDIR}/canlib/util/can_tx_buffer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/util/can_tx_buffer.p1 canlib/util/can_tx_buffer.c 
	@-${MV} ${OBJECTDIR}/canlib/util/can_tx_buffer.d ${OBJECTDIR}/canlib/util/can_tx_buffer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/util/can_tx_buffer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/util/safe_ring_buffer.p1: canlib/util/safe_ring_buffer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/util" 
	@${RM} ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1.d 
	@${RM} ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1 canlib/util/safe_ring_buffer.c 
	@-${MV} ${OBJECTDIR}/canlib/util/safe_ring_buffer.d ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/util/timing_util.p1: canlib/util/timing_util.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/util" 
	@${RM} ${OBJECTDIR}/canlib/util/timing_util.p1.d 
	@${RM} ${OBJECTDIR}/canlib/util/timing_util.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/util/timing_util.p1 canlib/util/timing_util.c 
	@-${MV} ${OBJECTDIR}/canlib/util/timing_util.d ${OBJECTDIR}/canlib/util/timing_util.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/util/timing_util.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_actuator.p1: canlib/message/msg_actuator.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_actuator.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_actuator.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_actuator.p1 canlib/message/msg_actuator.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_actuator.d ${OBJECTDIR}/canlib/message/msg_actuator.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_actuator.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_common.p1: canlib/message/msg_common.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_common.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_common.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_common.p1 canlib/message/msg_common.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_common.d ${OBJECTDIR}/canlib/message/msg_common.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_common.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_general.p1: canlib/message/msg_general.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_general.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_general.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_general.p1 canlib/message/msg_general.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_general.d ${OBJECTDIR}/canlib/message/msg_general.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_general.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_gps.p1: canlib/message/msg_gps.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_gps.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_gps.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_gps.p1 canlib/message/msg_gps.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_gps.d ${OBJECTDIR}/canlib/message/msg_gps.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_gps.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_recovery.p1: canlib/message/msg_recovery.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_recovery.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_recovery.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_recovery.p1 canlib/message/msg_recovery.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_recovery.d ${OBJECTDIR}/canlib/message/msg_recovery.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_recovery.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_sensor.p1: canlib/message/msg_sensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_sensor.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_sensor.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_sensor.p1 canlib/message/msg_sensor.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_sensor.d ${OBJECTDIR}/canlib/message/msg_sensor.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_sensor.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_state_est.p1: canlib/message/msg_state_est.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_state_est.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_state_est.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_state_est.p1 canlib/message/msg_state_est.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_state_est.d ${OBJECTDIR}/canlib/message/msg_state_est.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_state_est.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1: rocketlib/pic18f26k83/timer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/rocketlib/pic18f26k83" 
	@${RM} ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1.d 
	@${RM} ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1 rocketlib/pic18f26k83/timer.c 
	@-${MV} ${OBJECTDIR}/rocketlib/pic18f26k83/timer.d ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/clockInit.p1: clockInit.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/clockInit.p1.d 
	@${RM} ${OBJECTDIR}/clockInit.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/clockInit.p1 clockInit.c 
	@-${MV} ${OBJECTDIR}/clockInit.d ${OBJECTDIR}/clockInit.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/clockInit.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/leds.p1: leds.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/leds.p1.d 
	@${RM} ${OBJECTDIR}/leds.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/leds.p1 leds.c 
	@-${MV} ${OBJECTDIR}/leds.d ${OBJECTDIR}/leds.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/leds.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/adc.p1: adc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.p1.d 
	@${RM} ${OBJECTDIR}/adc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/adc.p1 adc.c 
	@-${MV} ${OBJECTDIR}/adc.d ${OBJECTDIR}/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/spi.p1: spi.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/spi.p1.d 
	@${RM} ${OBJECTDIR}/spi.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/spi.p1 spi.c 
	@-${MV} ${OBJECTDIR}/spi.d ${OBJECTDIR}/spi.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/spi.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cc1200.p1: cc1200.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cc1200.p1.d 
	@${RM} ${OBJECTDIR}/cc1200.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/cc1200.p1 cc1200.c 
	@-${MV} ${OBJECTDIR}/cc1200.d ${OBJECTDIR}/cc1200.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/cc1200.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1: canlib/pic18f26k83/pic18f26k83_can.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/pic18f26k83" 
	@${RM} ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1.d 
	@${RM} ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1 canlib/pic18f26k83/pic18f26k83_can.c 
	@-${MV} ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.d ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/pic18f26k83/pic18f26k83_can.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/util/can_rcv_buffer.p1: canlib/util/can_rcv_buffer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/util" 
	@${RM} ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1.d 
	@${RM} ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1 canlib/util/can_rcv_buffer.c 
	@-${MV} ${OBJECTDIR}/canlib/util/can_rcv_buffer.d ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/util/can_rcv_buffer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/util/can_tx_buffer.p1: canlib/util/can_tx_buffer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/util" 
	@${RM} ${OBJECTDIR}/canlib/util/can_tx_buffer.p1.d 
	@${RM} ${OBJECTDIR}/canlib/util/can_tx_buffer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/util/can_tx_buffer.p1 canlib/util/can_tx_buffer.c 
	@-${MV} ${OBJECTDIR}/canlib/util/can_tx_buffer.d ${OBJECTDIR}/canlib/util/can_tx_buffer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/util/can_tx_buffer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/util/safe_ring_buffer.p1: canlib/util/safe_ring_buffer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/util" 
	@${RM} ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1.d 
	@${RM} ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1 canlib/util/safe_ring_buffer.c 
	@-${MV} ${OBJECTDIR}/canlib/util/safe_ring_buffer.d ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/util/safe_ring_buffer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/util/timing_util.p1: canlib/util/timing_util.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/util" 
	@${RM} ${OBJECTDIR}/canlib/util/timing_util.p1.d 
	@${RM} ${OBJECTDIR}/canlib/util/timing_util.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/util/timing_util.p1 canlib/util/timing_util.c 
	@-${MV} ${OBJECTDIR}/canlib/util/timing_util.d ${OBJECTDIR}/canlib/util/timing_util.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/util/timing_util.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_actuator.p1: canlib/message/msg_actuator.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_actuator.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_actuator.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_actuator.p1 canlib/message/msg_actuator.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_actuator.d ${OBJECTDIR}/canlib/message/msg_actuator.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_actuator.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_common.p1: canlib/message/msg_common.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_common.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_common.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_common.p1 canlib/message/msg_common.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_common.d ${OBJECTDIR}/canlib/message/msg_common.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_common.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_general.p1: canlib/message/msg_general.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_general.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_general.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_general.p1 canlib/message/msg_general.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_general.d ${OBJECTDIR}/canlib/message/msg_general.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_general.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_gps.p1: canlib/message/msg_gps.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_gps.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_gps.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_gps.p1 canlib/message/msg_gps.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_gps.d ${OBJECTDIR}/canlib/message/msg_gps.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_gps.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_recovery.p1: canlib/message/msg_recovery.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_recovery.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_recovery.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_recovery.p1 canlib/message/msg_recovery.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_recovery.d ${OBJECTDIR}/canlib/message/msg_recovery.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_recovery.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_sensor.p1: canlib/message/msg_sensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_sensor.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_sensor.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_sensor.p1 canlib/message/msg_sensor.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_sensor.d ${OBJECTDIR}/canlib/message/msg_sensor.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_sensor.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/canlib/message/msg_state_est.p1: canlib/message/msg_state_est.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/canlib/message" 
	@${RM} ${OBJECTDIR}/canlib/message/msg_state_est.p1.d 
	@${RM} ${OBJECTDIR}/canlib/message/msg_state_est.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/canlib/message/msg_state_est.p1 canlib/message/msg_state_est.c 
	@-${MV} ${OBJECTDIR}/canlib/message/msg_state_est.d ${OBJECTDIR}/canlib/message/msg_state_est.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/canlib/message/msg_state_est.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1: rocketlib/pic18f26k83/timer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/rocketlib/pic18f26k83" 
	@${RM} ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1.d 
	@${RM} ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1 rocketlib/pic18f26k83/timer.c 
	@-${MV} ${OBJECTDIR}/rocketlib/pic18f26k83/timer.d ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/rocketlib/pic18f26k83/timer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/clockInit.p1: clockInit.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/clockInit.p1.d 
	@${RM} ${OBJECTDIR}/clockInit.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/clockInit.p1 clockInit.c 
	@-${MV} ${OBJECTDIR}/clockInit.d ${OBJECTDIR}/clockInit.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/clockInit.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/LTT.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/LTT.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=none  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/LTT.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/LTT.X.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/LTT.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/LTT.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -DBOARD_TYPE_UNIQUE_ID=BOARD_TYPE_ID_TELEMETRY -DBOARD_INST_UNIQUE_ID=BOARD_INST_ID_GENERIC -xassembler-with-cpp -I"rocketlib/include" -I"canlib" -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/LTT.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
