/**
 * @file eci_config_armadillo_linux.h
 * @brief armadillo linux configuration
 * @version $Id: eci_config_armadillo.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2017 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_ARMADILLO_LINUX_H__
#define __ECI_CONFIG_ARMADILLO_LINUX_H__

#define ARMADILLO_LINUX

#if defined(ARMADILLO_LINUX)		// Armadillo (ARMv7(Cortex))
#define PLATFORM_STR "Armadillo Linux"
#define OS_UNIX
#define OFFSET_SYS_BITS64
#define ARC_ARM_LITTLE_ILP32
#define HAVE_STRCASECMP
#define HAVE_SYS_MATHLIB
#define HAVE_TLS
#define STORAGE_ENDIAN_LITTLE		// little endian
#endif /* ARMADILLO_LINUX */

#endif /* __ECI_CONFIG_ARMADILLO_LINUX_H__ */
