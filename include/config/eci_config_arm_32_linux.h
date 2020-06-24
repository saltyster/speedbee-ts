/**
 * @file eci_config_arm_32_linux.h
 * @brief ARM 32 bit linux configuration
 * @version $Id: eci_config_arm_32_linux.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_ARM_32_LINUX_H__
#define __ECI_CONFIG_ARM_32_LINUX_H__

#define ARM_32_LINUX

#if defined(ARM_32_LINUX)			// ARMv6l(ARM11) ARMv7(Coretex)
#define PLATFORM_STR "ARM 32bit Linux"
#define OS_UNIX
#define OFFSET_SYS_BITS64
#define ARC_ARM_LITTLE_ILP32
#define HAVE_STRCASECMP
#define HAVE_SYS_MATHLIB
#define HAVE_TLS
#define STORAGE_ENDIAN_LITTLE		// little endian
#define TIME_T_BITS32
#endif /* ARM_32_LINUX */

#endif // __ECI_CONFIG_ARM_32_LINUX_H__
