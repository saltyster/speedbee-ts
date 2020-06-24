/**
 * @file eci_config_arm_64_linux.h
 * @brief ARM 64 bit linux configuration
 * @version $Id: eci_config_arm_64_linux.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_ARM_64_LINUX_H__
#define __ECI_CONFIG_ARM_64_LINUX_H__

#define ARM_64_LINUX

#if defined(ARM_64_LINUX)			// ARMv8
#define PLATFORM_STR "ARM 64bit Linux"
#define OS_UNIX
#define OFFSET_SYS_BITS64
#define ARC_ARM_64
#define HAVE_STRCASECMP
#define HAVE_SYS_MATHLIB
#define HAVE_TLS
#define STORAGE_ENDIAN_LITTLE		// little endian
#endif /* ARM_64_LINUX */

#endif /* __ECI_CONFIG_ARM_64_LINUX_H__ */
