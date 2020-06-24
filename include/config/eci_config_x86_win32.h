/**
 * @file eci_config_x86_win32.h
 * @brief x86 win32 configuration
 * @version $Id: eci_config_x86_win32.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_X86_WIN32_H__
#define __ECI_CONFIG_X86_WIN32_H__

// 32 bit
// set TIME_T_BITS32  
// omit OFFSET_SYS_BITS64

#define X86_WIN32
#if defined(X86_WIN32)
#define PLATFORM_STR "x86 WIN32"
#define OS_WIN32
#define ARC_INTEL_X86
#define HAVE_SYS_MATHLIB
#define STORAGE_ENDIAN_LITTLE		// little endian
#define HAVE_TLS					/* thread local storage */
#define OFFSET_SYS_BITS64
#endif /* X86_WIN32 */

#endif /* __ECI_CONFIG_X86_WIN32_H__ */
