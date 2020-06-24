/**
 * @file eci_config_arm_eabi_linux.h
 * @brief ARM eabi linux configuration
 * @version $Id: eci_config_arm_eabi_linux.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_ARM_EABI_LINUX_H__
#define __ECI_CONFIG_ARM_EABI_LINUX_H__

#define ARM_EABI_LINUX

#if defined(ARM_EABI_LINUX)
#define PLATFORM_STR "ARM-EABI Linux"
#define OS_UNIX
#define ARC_ARM_EABI
#define HAVE_STRCASECMP
#define HAVE_SYS_MATHLIB
#define HAVE_TLS					/* thread local storage */
#endif /* ARM_EABI_LINUX */

#endif // __ECI_CONFIG_ARM_EABI_LINUX_H__
