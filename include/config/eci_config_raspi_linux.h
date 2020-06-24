/**
 * @file eci_config_raspi_linux.h
 * @brief Rasberry Pi configuration
 * @version $Id: eci_config_raspi_linux.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_RASPI_LINUX_H__
#define __ECI_CONFIG_RASPI_LINUX_H__

#define RASPI_LINUX

#if defined(RASPI_LINUX)			// Raspberry PI
#define PLATFORM_STR "Raspberry PI Linux"
#define OS_UNIX
#define OFFSET_SYS_BITS64	
#define ARC_ARM_LITTLE_ILP32
#define HAVE_STRCASECMP
#define HAVE_SYS_MATHLIB
#define HAVE_TLS
#define STORAGE_ENDIAN_LITTLE		// little endian
#define TIME_T_BITS32
#endif /* RASPI_LINUX */

#endif // __ECI_CONFIG_RASPI_LINUX_H__
