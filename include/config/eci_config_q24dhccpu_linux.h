/**
 * @file eci_config_q24dhccpu_linux.h
 * @brief q24dhccpu linux configuration
 * @version $Id: eci_config_q24dhccpu_linux.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2017 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_Q24DHCCPU_LINUX_H__
#define __ECI_Q24DHCCPU_LINUX_H__

#define Q24DHCCPU_LINUX

#if defined(Q24DHCCPU_LINUX)		// Q24DHCCPU-LS (Intel ATOM)
#define PLATFORM_STR "Q24DHCCPU-LS (eLinux Station)"
#define OS_UNIX
#define OFFSET_SYS_BITS64
#define ARC_INTEL_X86
#define HAVE_STRCASECMP
#define HAVE_SYS_MATHLIB
#define HAVE_TLS
#define STORAGE_ENDIAN_LITTLE
#endif /* Q24DHCCPU_LINUX */

#endif /* __ECI_Q24DHCCPU_LINUX_H__ */
