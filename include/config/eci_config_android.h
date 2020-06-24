/**
 * @file eci_config_android.h
 * @brief android configuration
 * @version $Id: eci_config_android.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_ANDROID_H__
#define __ECI_CONFIG_ANDROID_H__

#define ANDROID_GOLDFISH

#if defined(ANDROID_GOLDFISH)
#define PLATFORM_STR "Android goldfish"
#define OS_ANDROID
#define OFFSET_BITS64
#define ARC_ARM_LITTLE_ILP32
#endif /* ANDROID_GOLDFISH */

#endif // __ECI_CONFIG_ANDROID_H__
