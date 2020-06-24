/**
 * @file eci_config_zynq_arm_vxworks.h
 * @brief zynx arm vxworks configuration 
 * @version $Id: eci_config_zynq_arm_vxworks.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2016 - 2017 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_ZYNQ_ARM_VXWORKS_H__
#define __ECI_CONFIG_ZYNQ_ARM_VXWORKS_H__

#define ZYNQ_ARM_VXWORKS

#if defined(ZYNQ_ARM_VXWORKS)	// xilinx zynq-7000 SoC ZC706 evaluation kit & windows simulator
#define PLATFORM_STR "XILINX ZYNQ VXWORKS"
#define OS_VXWORKS
#define ARC_ARM_LITTLE_ILP32		// ARMEL EABI and simpc (same arch)
#define STORAGE_ENDIAN_LITTLE		// little endian
#endif /* ZYNQ_ARM_VXWORKS */

#endif /* __ECI_CONFIG_ZYNQ_ARM_VXWORKS__ */
