/**
 * @file eci_config_wince.h
 * @brief configuration windows ce
 * @version $Id: eci_config_wince.h 5697 2015-03-31 01:48:31Z $
 * @par Copyright (C) (C) 2016 - 2017 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_WINCE_H__
#define __ECI_CONFIG_WINCE_H__

/* ----------------------------------------------------------------------------
 * System configuration 
 * ---------------------------------------------------------------------------*/
#ifdef WINCE
	#if defined(ARM)
		#if _WIN32_WCE == 0x700
			#define ARM_WINCE_C7
		#else
			#error unspport version
		#endif
	#elif defined(x86)
		#if _WIN32_WCE == 0x700
			#define X86_WINCE_C7
		#else
			#error unspport version
		#endif
	#endif
#else
	#error not defined WINCE
#endif

/*
 * Windows Embbed Compact7
 */
#if defined(ARM_WINCE_C7)
#define PLATFORM_STR "ARM Windows Embedded Compact7"
#define OS_WINCE
#define OFFSET_SYS_BITS64
#define ARC_ARM_LITTLE_ILP32
#define HAVE_SYS_MATHLIB
#define STORAGE_ENDIAN_LITTLE		// little endian
#define WIN_VS2008					// visual studio 2008
#define MEM_LIMIT_SIZE		3		// memory limit 3M
#define LOCK_FILE_EXTERNAL
#endif /* ARM_WINCE_C7 */

#if defined(X86_WINCE_C7)
#define PLATFORM_STR "x86 Windows Embedded Compact7"
#define OS_WINCE
#define OFFSET_SYS_BITS64
#define ARC_INTEL_X86
#define HAVE_SYS_MATHLIB
#define STORAGE_ENDIAN_LITTLE		// little endian
#define WIN_VS2008					// visual studio 2008
#define MEM_LIMIT_SIZE		3		// memory limit 3M
#endif /* ARM_WINCE_C7 */

#endif // __ECI_CONFIG_WINCE_H__
