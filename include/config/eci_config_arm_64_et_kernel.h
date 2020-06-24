/**
 * @file eci_config_arm_64_et_kernel.h
 * @brief ARM 64 bit et-kernel configuration
 * @version $Id$
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_ARM_64_ET_KERNEL_H__
#define __ECI_CONFIG_ARM_64_ET_KERNEL_H__

#define ARM_64_ET_KERNEL

#if defined(ARM_64_ET_KERNEL)	// Coretex-A9 MPCore
#define PLATFORM_STR "ARM 64bit eT-Kernel"
#define OS_ET_KERNEL
#define OFFSET_SYS_BITS64
#define ARC_ARM_64
#define HAVE_STRCASECMP
#define HAVE_SYS_MATHLIB
#define STORAGE_ENDIAN_LITTLE		// little endian
#define HAVE_LONG_LONG
#define NULL 0
#include "typedef.h"
#define MEM_LIMIT_SIZE		3		// memory limit 3M
#define MEM_STATIC
#endif /* ARM_64_ET_KERNEL */

#endif // __ECI_CONFIG_ARM_64_ET_KERNEL_H__
