/**
 * @file eci_config_conprosys.h
 * @brief conposys linux configuration
 * @version $Id:$
 * @par Copyright (C) 2018 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_CONPROSYS_LINUX_H__
#define __ECI_CONFIG_CONPROSYS_LINUX_H__

#define CONPROSYS_LINUX

#if defined(CONPROSYS_LINUX)		// ARM Cortex-A8
#define PLATFORM_STR "CONPROSYS Series"
#define OS_UNIX
#define OFFSET_SYS_BITS64
#define ARC_ARM_LITTLE_ILP32
#define HAVE_STRCASECMP
#define HAVE_SYS_MATHLIB
#define HAVE_TLS
#define STORAGE_ENDIAN_LITTLE		// little endian
#endif /* CONPROSYS_LINUX */

#endif /* __ECI_CONFIG_CONPROSYS_LINUX_H__ */
