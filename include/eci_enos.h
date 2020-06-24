/**
 * @file eci_enos.h
 * @brief only enos utility and enos common header
 * @version $Id: eci_type.h 6283 2015-10-23 08:42:10Z $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __ECI_ENOS_H__
#define __ECI_ENOS_H__

/* --------------------------------------------------------------------------
 * C time convert utility
 * --------------------------------------------------------------------------
 */
#define SD_CT_LOCAL			(0)		/* ctime local */
#define SD_CT_UTC			(1)		/* ctime utc */

DllImport int sd_ctime_to_etime(ctime_t *ct, ui64_t *et,int cf);
DllImport int sd_etime_to_ctime(ui64_t *et, ctime_t *ct, int cf);

/* ---------------------------------------------------------------------------
 * Expression interface
 * ---------------------------------------------------------------------------*/
typedef void* exp_t;
typedef void* exp_var_t;

// Data EGI_DATE
typedef struct exp_date {
	i16_t		year;
	i8_t		mon;
	i8_t		day;
} exp_date_t;

// Time EGI_TIME
typedef struct exp_time {
	exp_date_t	d;
	i8_t		hour;
	i8_t		min;
	i8_t		sec;
	i8_t		padding;
} exp_time_t;

// Timestamp EGI_TMSTMP (equal ctime_t)
typedef struct exp_stmp {
	exp_time_t	t;
	i32_t		us;
} exp_stmp_t;

// binary
typedef struct exp_bin {
	void		*da;
	i32_t		sz;
} exp_bin_t;

// strings
typedef struct exp_str {
	char		*da;
	i32_t		bsz;    // real string (b)yte size
	i32_t		asz;    // store (a)rea size
} exp_str_t;

/* condtion result */
#define ECI_EXP_COND_ERR		(-1)
#define ECI_EXP_COND_FALSE		(0)
#define ECI_EXP_COND_TRUE		(1)
#define ECI_EXP_COND_UNK		(2)

/* expression C data type - internal data type */
#define ECI_EXP_DT_INT8			(1)		/* signed int8 */
#define ECI_EXP_DT_INT16		(2)		/* signed int16 */
#define ECI_EXP_DT_INT32		(3)		/* signed int32 */
#define ECI_EXP_DT_UINT8		(4)		/* unsigned int8 */
#define ECI_EXP_DT_UINT16		(5)		/* unsigned int16 */
#define ECI_EXP_DT_UINT32		(6)		/* unsigned int32 */
#define ECI_EXP_DT_FLOAT		(7)		/* float */
#define ECI_EXP_DT_DOUBLE		(8)		/* double */
#define ECI_EXP_DT_VARCHAR		(9)		/* variable character */
#define ECI_EXP_DT_DATE			(11)	/* date */
#define ECI_EXP_DT_TIME			(12)	/* time */
#define ECI_EXP_DT_TIMESTAMP	(13)	/* timestamp */
#define ECI_EXP_DT_INT64		(16)	/* signed int64 */
#define ECI_EXP_DT_UINT64		(17)	/* unsigned int64 */

// expression object data type
#define	ECI_EXP_CD_NUL			(0)
#define	ECI_EXP_CD_INT			(1)
#define ECI_EXP_CD_DBL			(2)
#define	ECI_EXP_CD_STR			(3)
#define	ECI_EXP_CD_DATE			(4)
#define	ECI_EXP_CD_TIME			(5)
#define	ECI_EXP_CD_STMP			(6)
#define	ECI_EXP_CD_BIN			(7)

DllImport exp_t eci_exp_create(char *str);
DllImport void eci_exp_destroy(exp_t exp);
DllImport exp_var_t eci_exp_search_var(exp_t exp, char *vname);
DllImport int eci_exp_dset_var(exp_var_t var, int dt, void *da, int dsz);
DllImport int eci_exp_set_var(exp_t exp, char *vname, int dt, void *da, int dsz);
DllImport int eci_exp_run_cond(exp_t exp);	// run condition expr
DllImport int eci_exp_run_expr(exp_t exp, int dt, void *rp); // run exp
DllImport int eci_exp_time_stoi(char *s, int slen, exp_stmp_t *tm);

#endif	/* __ECI_ENOS_H__ */
