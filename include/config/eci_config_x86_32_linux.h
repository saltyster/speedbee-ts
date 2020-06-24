/**
 * @file eci_config_x86_32_linux.h
 * @brief x86 32 bit linux configuration
 * @version $Id: eci_config_x86_32_linux.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_X86_32_LINUX_H__
#define __ECI_CONFIG_X86_32_LINUX_H__

#define X86_32_LINUX

#if defined(X86_32_LINUX)
#define PLATFORM_STR "Intel x86_32 Linux"
#define OS_UNIX
#define OFFSET_SYS_BITS64
#define ARC_INTEL_X86
#define HAVE_STRCASECMP
#define HAVE_SYS_MATHLIB
#define HAVE_TLS
#define STORAGE_ENDIAN_LITTLE		// little endian
#endif /* X86_32_LINUX */

#endif // __ECI_CONFIG_X86_32_LINUX_H__
