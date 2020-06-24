/**
 * @file eci_config_x86_cygwin.h
 * @brief x86 cygwin linux configuration
 * @version $Id: eci_config_x86_cygwin.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_X86_CYGWIN_LINUX_H__
#define __ECI_CONFIG_X86_CYGWIN_LINUX_H__

#define X86_CYGWIN

#if defined(X86_CYGWIN)
#define PLATFORM_STR "x86 CYGWIN"
#define OS_UNIX
#define OFFSET_BITS64
#define ARC_INTEL_X86
#define HAVE_STRCASECMP
//#define HAVE_TLS					/* thread local storage */
#endif /* X86_CYGWIN */

#endif // __ECI_CONFIG_X86_CYGWIN_H__
