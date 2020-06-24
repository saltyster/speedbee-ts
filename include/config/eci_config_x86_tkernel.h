/**
 * @file eci_config_x86_tkernel.h
 * @brief x86 32 bit linux configuration
 * @version $Id: eci_config_x86_32_linux.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_X86_TKERNEL_H__
#define __ECI_CONFIG_X86_TKERNEL_H__

#define X86_TKERNEL

#if defined(X86_TKERNEL)
#define PLATFORM_STR "T-KERNEL X86"
#define OS_TKERNEL
#define ARC_INTEL_X86
#define HAVE_SYS_MATHLIB
#define HAVE_ISASCII
#endif /* X86_TKERNEL */

#if defined(T_KERNEL_RKIT)
#define PLATFORM_STR "T-KERNEL REFERENCE-KIT"
#define OS_TKERNEL
#define ARC_ARM_LITTLE_ILP32
#define HAVE_SYS_MATHLIB
#define HAVE_ISASCII
#endif /* T_KERNEL_RKIT */

#endif // __ECI_CONFIG_X86_TKERNEL_H__
