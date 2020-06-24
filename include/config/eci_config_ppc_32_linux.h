/**
 * @file eci_config_ppc_32_linux.h
 * @brief PPC 32 bit linux configuration
 * @version $Id: eci_config_ppc_32_linux.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_PPC_32_LINUX_H__
#define __ECI_CONFIG_PPC_32_LINUX_H__

#define PPC_32_LINUX

#if defined(PPC_32_LINUX)
#define PLATFORM_STR "PPC 32bit Linux"
#define OS_UNIX
#define OFFSET_BITS64
#define ARC_PPC_405
#define HAVE_STRCASECMP
#define HAVE_SYS_MATHLIB
#define HAVE_TLS                    /* thread local storage */
#endif /* PPC_LINUX */

#endif // __ECI_CONFIG_PPC_32_LINUX_H__
