/**
 * @file eci_config_idea.h
 * @brief idea configuration
 * @version $Id: eci_config_idea.h 5697 2015-03-31 01:48:31Z $
 * @par Copyright (C) 2016 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_CONFIG_IDEA_H__
#define __ECI_CONFIG_IDEA_H__

// user custom configuration
#if defined(IDEA_R2000)
#define PLATFORM_STR "IDEA R2000 SH4 NOTEi4 SHC"
#define OS_ITRON
#define ARC_SH4_BIG
#define LANG_SJIS
#define MEM_STATIC
#define CON_CRLF				/* console CRLF */
#define IO_BUF_ALIGN		32  /* DMA burst transmission align 32 byte */
#define HAVE_SYS_MATHLIB
#define CC_SHC      			/* version 7.1.0 */
#define SHELL_TCSH
#endif /* IDEA_R2000 */

#if defined(IDEA_R2000L)	// little endian
#define PLATFORM_STR "IDEA R2000 SH4 NOTEi4 SHC"
#define OS_ITRON
#define ARC_SH4_LITTLE
#define LANG_SJIS
#define MEM_STATIC
#define CON_CRLF				/* console CRLF */
#define IO_BUF_ALIGN		32  /* DMA burst transmission align 32 byte */
#define HAVE_SYS_MATHLIB
#define CC_SHC      			/* version 7.1.0 */
#define SHELL_TCSH
#endif /* IDEA_R2000 */

#if defined(SH4A_ROP7785_ITRON4)
#define PLATFORM_STR "IDEA ROP7785-BOARD"
#define OS_ITRON
#define ARC_SH4_LITTLE
#define LANG_SJIS
#define MEM_STATIC
#define CON_CRLF				/* console CRLF */
#define IO_BUF_ALIGN		32  /* DMA burst transmission align 32 byte */
#define HAVE_SYS_MATHLIB
#define CC_SHC
#define SHELL_TCSH
#define OFFSET_SYS_BITS64
#define HAVE_LONG_LONG
#endif /* SH4A_ROP7785_ITRON4 */

#endif // __ECI_CONFIG_IDEA_H__
