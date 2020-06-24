/**
 * @file egi_type.h 
 * @brief egi type header
 * @version $Id: egi_type.h 6279 2015-10-20 01:15:41Z  $
 * @par Copyright (C) 2008 - 2017 SALTYSTER Inc. All right reserved.
 */
#ifndef __EGI_TYPE_H__
#define __EGI_TYPE_H__

/* data type */
#if defined(OS_UNIX) || defined(OS_WIN32) || defined(OS_VXWORKS)
typedef char			B;
typedef unsigned char	UB;
typedef short			H;
typedef unsigned short	UH;
typedef int				W;
typedef unsigned int	UW;
typedef void*			VP;
typedef W				INT; 
typedef UW				UINT;
#endif

#ifdef HAVE_LONG_LONG
typedef i64_t			D;
typedef ui64_t			UD;	
#endif /* HAVE_LONG_LONG */

/* null macro */
#ifndef NULL
	#ifdef __cplusplus
		#define NULL	(0)
	#else
		#define NULL	((void*)0)
	#endif
#endif

/* const macro */
#ifndef CONST
	#define CONST	const
#endif

/* egi type */
typedef W		EGI_RET;	/* return */
typedef VP		EGI_DID;	/* data-id */
typedef W		EGI_ERR;	/* error */

/* type of multi-byte character(SJIS,EUC,UTF8) */
typedef UB		EGI_MB;
/* type of wide character(UTF16) */
typedef UH		EGI_WC;

/* return code */
#define EGI_OK		(0)
#define EGI_ER		(-1)
#define EGI_BUSY	(1)

#endif	/* __EGI_TYPE_H__ */
