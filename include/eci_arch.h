/**
 * @file eci_arch.h
 * @brief os, cpu architecture
 * @version $Id: eci_arch.h 6283 2015-10-23 08:42:10Z  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_ARCH_H__
#define __ECI_ARCH_H__

/* ---------------------------------------------------------------------------
 *  OS
 * ---------------------------------------------------------------------------
 */
/* 
 * ANDROID 
 * Don't hava to set HAVE_TLS,  android not support TLS
 */
#if defined(OS_ANDROID)
#define OS_UNIX
#define HAVE_STRCASECMP
#endif /* OS_ANDROID */

/* UNIX */
#if defined(OS_UNIX)
#define HAVE_STDIO		/* Support FILE */
#define HAVE_SYSTEM		/* Have system() */
#define HAVE_ACCESS
#define HAVE_ISASCII
#define HAVE_LONG_LONG
#define ENABLE_MAPF		/* system memory mapping file */
#endif /* OS_UNIX */

/* WINCE : time_t 64bit */
#if defined(OS_WINCE)
#define OS_WIN32
#endif /* OS_WINCE */

/* WIN32 */
#if defined(OS_WIN32)
#define HAVE_STDIO		/* Support FILE */
#ifndef OS_WINCE
#define HAVE_SYSTEM		/* Have system() */
#define ENABLE_MAPF		/* system memory mapping file file */
#endif /* OS_WINCE */
#define HAVE_ISASCII
#define HAVE_LONG_LONG
#endif /* OS_WIN32 */

/* VXWORKS 7.X */
#if defined(OS_VXWORKS)
#define HAVE_STDIO		/* Support FILE */
#define HAVE_ISASCII
#define HAVE_LONG_LONG
#define HAVE_SYS_MATHLIB
#define HAVE_TLS			/* diab , gcc */
#endif /* OS_VXWORKS */

/* ---------------------------------------------------------------------------
 *  CPU Architecture
 * ---------------------------------------------------------------------------
 */
/* x86_64 LP64 */
#if defined(ARC_INTEL_X86_64_LP64)
#define ARC_SIZE_SHORT  2
#define ARC_SIZE_INT    4
#define ARC_SIZE_LONG   8
#define ARC_SIZE_FLOAT  4
#define ARC_SIZE_DOUBLE 8
#define ARC_SIZE_PONTER 8
#define ARC_ENDIAN_LITTLE
#endif /* ARC_INTEL_X86_64_LP64 */

/* x86 ILP32 */
#if defined(ARC_INTEL_X86)
#define ARC_SIZE_SHORT  2
#define ARC_SIZE_INT    4
#define ARC_SIZE_LONG   4
#define ARC_SIZE_FLOAT  4
#define ARC_SIZE_DOUBLE 8
#define ARC_SIZE_PONTER 4
#define ARC_ENDIAN_LITTLE
#endif /* ARC_INTEL_X86 */

/* SH4 BIGENDIAN ILP32 */
#if defined(ARC_SH4_BIG)
#define ARC_SIZE_SHORT  2
#define ARC_SIZE_INT    4
#define ARC_SIZE_LONG   4
#define ARC_SIZE_FLOAT  4
#define ARC_SIZE_DOUBLE 8
#define ARC_SIZE_PONTER 4
#define ARC_ENDIAN_BIG
#endif /* ARC_SH4_BIG */

/* ARM BIG ENDIAN ILP32 */
#if defined(ARC_ARM_EABI)
#define ARC_SIZE_SHORT  2
#define ARC_SIZE_INT    4
#define ARC_SIZE_LONG   4
#define ARC_SIZE_FLOAT  4
#define ARC_SIZE_DOUBLE 8
#define ARC_SIZE_PONTER 4
#define ARC_ENDIAN_BIG
#endif /* ARC_ARM_EABI */

/* ARM 32bit LITTLE ENDIAN ILP32 */
#if defined(ARC_ARM_LITTLE_ILP32)
#define ARC_SIZE_SHORT  2
#define ARC_SIZE_INT    4
#define ARC_SIZE_LONG   4
#define ARC_SIZE_FLOAT  4
#define ARC_SIZE_DOUBLE 8
#define ARC_SIZE_PONTER 4
#define ARC_ENDIAN_LITTLE
#endif /* ARC_ARM_LITTLE_ILP32 */

/* PPC 405GP BIG_ENDIAN ILP32 */
#if defined(ARC_PPC_405)
#define ARC_SIZE_SHORT  2
#define ARC_SIZE_INT    4
#define ARC_SIZE_LONG   4
#define ARC_SIZE_FLOAT  4
#define ARC_SIZE_DOUBLE 8
#define ARC_SIZE_PONTER 4
#define ARC_ENDIAN_BIG
#define BIT_FIELD_MSB		
#endif /* ARC_PPC_405 */

/* ARM 64 */
#if defined(ARC_ARM_64)
#define ARC_SIZE_SHORT  2
#define ARC_SIZE_INT    4
#define ARC_SIZE_LONG   8
#define ARC_SIZE_FLOAT  4
#define ARC_SIZE_DOUBLE 8
#define ARC_SIZE_PONTER 8
#define ARC_ENDIAN_LITTLE
#endif /* ARC_ARM_64 */

#endif /* __ECI_ARCH_H__ */
