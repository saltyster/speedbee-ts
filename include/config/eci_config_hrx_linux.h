/**
 * @file eci_config_hhx_linux.h
 * @brief hrx linux configuration
 * @version $Id: eci_config_rhx_linux.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2017 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_HRX_LINUX_H__
#define __ECI_CONFIG_HRX_LINUX_H__

#define HRX_LINUX

#if defined(HRX_LINUX)		// HAGIWARA HRX1000 Series (ARMv7(Cortex-A7))
#define PLATFORM_STR "HAGIWARA HRX1000 Linux"
#define OS_UNIX
#define OFFSET_SYS_BITS64
#define ARC_ARM_LITTLE_ILP32
#define HAVE_STRCASECMP
#define HAVE_SYS_MATHLIB
#define HAVE_TLS
#define STORAGE_ENDIAN_LITTLE		// little endian
#endif /* HRX_LINUX */

#endif /* __ECI_CONFIG_HRX_LINUX_H__ */
