/**
 * @file eci_config_auto.h
 * @brief auto configuration
 * @version $Id: eci_config_auto.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2016 - 2017 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_AUTO_H__
#define __ECI_CONFIG_AUTO_H__
/*
 * -----------------------------------------
 * system configuration
 * -----------------------------------------
 */
#ifdef WINCE
	#include "eci_config_wince.h"
#else
	#if defined(WIN32)
		#include "eci_config_x86_win32.h"
	#elif defined(_WRS_KERNEL)
		#include "eci_config_zynx_arm_vxworks.h"
	#elif defined(__APPLE__)
		#define X86_64_OSX
	#else
		#include "eci_config_x86_64_linux.h"
	#endif
#endif

#endif /* __ECI_CONFIG_AUTO_H__ */
