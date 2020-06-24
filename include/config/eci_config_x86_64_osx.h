/**
 * @file eci_config_x86_64_osx.h
 * @brief x86 64 bit osx configuration
 * @version $Id: eci_config_x86_64_osx.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_X86_64_OSX_H__
#define __ECI_CONFIG_X86_64_OSX_H__

#define X86_64_OSX

#if defined(X86_64_OSX)
#define PLATFORM_STR "Intel x86_64 MacOS"
#define OS_UNIX
#define OFFSET_SYS_BITS64
#define ARC_INTEL_X86_64_LP64
#define HAVE_STRCASECMP
#define HAVE_SYS_MATHLIB
#define HAVE_TLS
#define STORAGE_ENDIAN_LITTLE		// little endian
#endif /* X86_64_OSX */

#endif // __ECI_CONFIG_X86_64_OSX_H__
