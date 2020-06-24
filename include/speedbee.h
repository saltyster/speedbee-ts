/**
 * @file speedbee.h
 * @brief SpeeDBee Stream Data External API
 * @version $Id:  $
 * @par Copyright (C) 2008 - 2020 SALTYSTER Inc. All right reserved.
 */
#ifndef __SPEEDBEE_H__
#define __SPEEDBEE_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "eci.h"
#include "eci_enos.h"

/* --------------------------------------------------------------------------
 * cursor
 * --------------------------------------------------------------------------
 */
// fetch result 
#define SD_FETCH_OK			(1)
#define SD_FETCH_END		(0)
#define SD_FETCH_ERR		(-1)

// cursor fetch status
#define SD_CUR_COL_REC		(00)	// get somewhere in column data
#define SD_CUR_NUL_REC		(01)	// null record (all column no data)
#define SD_CUR_CONT_REC		(010)	// set somewhere in continuous column data

// cursor column fetch indicator
#define SD_COL_IND_NOT_FOUND		(-1)
#define SD_COL_IND_NUL				(0)
#define SD_COL_IND_GET				(1)
#define SD_COL_IND_VAL_CONT			(2)	// when SDTS_CUR_OPT_AGGR_VAL_CONT
#define SD_COL_IND_VAL_CONT_TOP		(3)	// when SDTS_CUR_OPT_AGGR_CONT_TOP

/* --------------------------------------------------------------------------
 * window type
 * --------------------------------------------------------------------------
 */
#define SD_WT_INPUT			(1)
#define SD_WT_COUNT			(2)
#define SD_WT_TIME			(3)
#define SD_WT_FFT			(4)

/* --------------------------------------------------------------------------
 * window function type
 * --------------------------------------------------------------------------
 */
#define SD_WF_WAG			(01)		// window defined aggregation
#define SD_WF_UDP			(02)		// user definition data processing
#define SD_WF_NCF			(04)		// notify callback function

/* --------------------------------------------------------------------------
 * timeserise database column type 
 * --------------------------------------------------------------------------
 */
#define SDTS_CT_HI_FIX		(1)		// hi - fix size value type
#define SDTS_CT_MI_FIX		(11)	// mi - fix size value type
#define SDTS_CT_LO_FIX		(101)	// lo - fix size value type
#define SDTS_CT_LO_VAR		(102)	// lo - variable size data type

/* --------------------------------------------------------------------------
 * search timeserise database storage option
 * --------------------------------------------------------------------------
 */
#define SDTS_STO_OPT_MEM				(0100)	//  memory defaults
#define SDTS_STO_OPT_FILE				(0200)	//  file

/* --------------------------------------------------------------------------
 * search cache option
 * --------------------------------------------------------------------------
 */
#define SDTS_CAC_OPT_SEA_RES			(0100000)	//  search RES

/* --------------------------------------------------------------------------
 * timeserise database aggr cursor
 * --------------------------------------------------------------------------
 */
// aggr cursor option
#define SDTS_CUR_OPT_AGGR_VAL_NULL		(01) // value null
#define SDTS_CUR_OPT_AGGR_VAL_TOP		(02) // value top
#define SDTS_CUR_OPT_AGGR_TS_PREV		(04) // previous time
#define SDTS_CUR_OPT_AGGR_VAL_CONT		(010) // continuos value
#define SDTS_CUR_OPT_AGGR_VAL_LAST		(020) // value top
#define SDTS_CUR_OPT_AGGR_TS_NEXT		(040) // next time
#define SDTS_CUR_OPT_EXTEND				(010000) // extend cursor
#define SDTS_CUR_OPT_AGGR_CONT_TOP		(020000) // continuos value top

/* --------------------------------------------------------------------------
 * sync command
 * --------------------------------------------------------------------------
 */
#define SDTS_SYNC_CMD_REG		(0) 	// regular sync
#define SDTS_SYNC_CMD_FORCE		(1) 	// force sync

/* --------------------------------------------------------------------------
 * define type 
 * --------------------------------------------------------------------------
 */
/* sd micro time : epoch time * 1,000,000  + milli(3) | micro(3) */
typedef ui64_t  sdtime_t;

/* sd nano time : epoch time * 1,000,000,000 + milli(3) | micro(3) | nano(3) */
typedef ui64_t  sdntime_t;

/* timestampe data size */
#define SD_TSSZ		(8)		// sizeof(ui64_t), sdtime_t, sdntime_t

/* id pointer */
typedef void*	sdid_t;

/* timeserise memory descriptor */
typedef void*	sdtsdb_t; 			/* time seriese data db descriptor */
typedef int		sdtscid_t;			/* time seriese data column id */

//  batch create column 
typedef struct {
	sdid_t		cname;			// name pointer, binary pointer
	char 		*colpar;		// column paramatar
	sdtscid_t	rscid;			// return column id
} sdtsbcrcol_t;

typedef struct {

	// cursor condition information
	sdntime_t		ref_cst;		// condition start
	sdntime_t		ref_cet;		// condition end
	sdntime_t		ref_aiv;		// arggregate interval time
	ui32_t			ref_opt; 		// cursor option
	int				ref_ccc;		// cursor column count

	// cursor fixed information
	ui32_t			ref_tcnt;		// found data total count
	ui32_t			ref_acnt;		// aggregate record count

	// current cursor dynamic information
	ui32_t			ref_fst;        // fetch status
	sdntime_t		ref_ctm;		// current time / aggr time

} *sdtscur_t;	

// basic statistics
typedef struct {
	double sum;				// sum
	double sumsq;			// sum of squares
	double min;				// min
	double max;				// max
	double mean;			// mean
	double var;				// sample variance
	double stdev;			// standard deviation
	double uvar;			// unbiased variance
	double ustdev;			// unbiased standard deviation
	double stder;			// standard error
	sdntime_t stime;		// window start time 
	sdntime_t etime;		// window end time
	int	   cnt;				// input count
} sdstat_t;

// column MI manage list information
typedef struct {
	double	smpl;			// MI sampling rate
	sdntime_t st;			// MI start time  
	sdntime_t et;			// memory save data end time
	ui64_t	micnt;			// current MI manage total insert data count
	ui64_t	mdcnt;			// current MI manage total delete data count
	ui64_t	mscnt;			// memory data count
} sdtscolmminfo_t;

// column window define information
typedef struct {
	char	name[32];		// name
	int		wtype;			// window type
	uchar	ftype;			// function type
} sdtscolwinfo_t;

// column information
typedef struct {

	// common information
	sdid_t	cname;			// column id : strings or binary or integer pointer
	int		ctype;			// column type
	int		dsz;			// data size
	int		rsz;			// record size
	int		scnt;			// save limit count
	ui64_t	icnt;			// total insert count
	bool	act;			// active or inactive
	char	*dtype;			// data type (strings)

	// column paramatar
	char	*colpar;		// column paramater	

	// memory save data information
	sdntime_t msst;			// memory save data start time
	sdntime_t mset;			// memory save data end time
	ui64_t	mscnt;			// memory save count

	// HI, MI information
	double	hmsmpl;			// HI, MI sampling rate
	sdntime_t hmst;			// HI, MI start time  

	/*
 	 *	 MI information
 	 */
	int 	mmmax;			// MI manage max count
	int 	mmspos;			// MI manage start position
	int 	mmucnt;			// MI manage current use count
	ui64_t	micnt;			// current MI manage total insert data count
	sdtscolmminfo_t *mminfo; // MI manager information array

	/*
 	 * window define
 	 */
	int		wdcnt;		// window define count
	sdtscolwinfo_t *winfo; // window define information array

} sdtscolinfo_t;

// db information
typedef struct {
	char	*dbpar;				// db paramatar
	char	*path;				// db path
	int		idsz;				// binary key id size
	int		ccnt;				// column count
	int		sbsz;				// sync write buffer size
	int		rmaxccnt;			// max read cache file count
	int		rmaxccnt2;			// max read cache search file count
	int		sccnt_h;
	int		sccnt_m;
	int		sccnt_l;
	int		sper_h;
	int		sper_m;
	int		sper_l;
	int		lag_h;
	int		lag_m;
	int		lag_l;
	int		maxf_h;
	int		maxf_m;
	int		maxf_l;
	ui64_t	stcnt;				// sync total count	(statistics)
	sdtscolinfo_t **colinfo;	// column information pointer array
} sdtsdbinfo_t;

// cursor column value
typedef struct {
	sdtscid_t	cid;			// column id
	sdntime_t	ts;				// timestamp
	int			ind;			// indicator
	char		*vp;			// value pointer
	ui32_t		vsz;			// value size
	double		smpl;			// H, M data sampling rate
} sdtscurval_t;

// define user define function (expression, condtion, callback)
typedef int (*sdudfn_t)(sdntime_t, void*, int, sdstat_t*, int, void *);

/* api option */
typedef ui32_t	sdtscdopt_t;

/* --------------------------------------------------------------------------
 * common api
 * --------------------------------------------------------------------------
 */
/* initialize interface */
DllImport int sd_init(char *env);

/* finish interface */
DllImport int sd_end(void);

/* get error code */
DllImport int sd_get_err(void);

/* explicit clear error  */
DllImport void sd_clear_err(void);


/* --------------------------------------------------------------------------
 * timeserise memory db api
 * --------------------------------------------------------------------------
 */
DllImport sdtsdb_t sdts_open_db(char *dbpar);
DllImport int sdts_close_db(sdtsdb_t db);
DllImport int sdts_drop_db(char *dbpar);
DllImport int sdts_delete_db(sdtsdb_t db, int min);
DllImport sdtscid_t sdts_create_col(sdtsdb_t db, sdid_t cname, char *colpar);
DllImport int sdts_create_col_bat(sdtsdb_t db, sdtsbcrcol_t *bp, int bcnt);
DllImport sdtscid_t sdts_get_col(sdtsdb_t db, sdid_t cname);
DllImport int sdts_activate_col(sdtsdb_t db, sdid_t cname, bool act);
DllImport int sdts_drop_col(sdtsdb_t db, sdid_t cname);
DllImport int sdts_set_smpl_rate(sdtsdb_t db, sdid_t cname, double rate);
DllImport int sdts_insert(sdtsdb_t db, sdtscid_t cid, sdntime_t ts, char *d, int dcnt);
DllImport int sdts_get_col_dat(sdtsdb_t db, sdtscid_t cid, sdntime_t st, sdntime_t et, char *rb, int rbsz, sdntime_t *rst);
DllImport int sdts_get_col_lval(sdtsdb_t db, sdtscid_t cid, sdntime_t *ts, char *rb, int rbsz);
DllImport sdtscur_t sdts_open_cur(sdtsdb_t db, sdtscid_t *cids, int ccc, sdntime_t st, sdntime_t et, sdntime_t iv, ui32_t opt);
DllImport int sdts_fetch_cur(sdtscur_t cur);
DllImport int sdts_get_cur_aggr(sdtscur_t cur, sdntime_t *rts, sdtscurval_t *avarr, int acnt);
DllImport int sdts_get_cur_dat(sdtscur_t cur, sdtscurval_t *vp);
DllImport int sdts_extend_cur(sdtscur_t cur, sdntime_t et);
DllImport int sdts_close_cur(sdtscur_t cur);
DllImport sdtsdbinfo_t *sdts_get_db_info(sdtsdb_t db);
DllImport void sdts_free_db_info(sdtsdbinfo_t *di);
DllImport sdtscolinfo_t *sdts_get_col_info(sdtsdb_t db, sdtscid_t cid);
DllImport void sdts_free_col_info(sdtscolinfo_t *ci);
DllImport int sdts_get_col_simp_info(sdtsdb_t db, sdtscid_t cid, sdtscolinfo_t *info);
DllImport int sdts_set_win(sdtsdb_t db, sdtscid_t cid, char *dname, int wt, char *wpar, sdudfn_t cb, void *);
DllImport int sdts_unset_win(sdtsdb_t db, sdtscid_t cid, char *dname);
DllImport int sdts_drop_win(sdtsdb_t db, sdtscid_t cid);
DllImport int sdts_sync_db(sdtsdb_t db, int cmd);

#define sdts_sync(a)		sdts_sync_db((a),SDTS_SYNC_CMD_REG)
#define sdts_sync_force(a)	sdts_sync_db((a),SDTS_SYNC_CMD_FORCE)

DllImport int sdts_adjust_ts(sdtsdb_t db, sdtscid_t cid, i64_t ad);

// debug function
DllImport void sdts_dbg_win_info(sdtsdb_t db, char *msg, char *bp, int bsz);
DllImport int sdts_dbg_col_info(sdtsdb_t db, sdtscid_t cid, char *bp, int bsz);
DllImport int sdts_dbg_read(sdtsdb_t db, char st, ui64_t ep, char dt);
DllImport void sd_get_err_dmsg(char *msg, char *mbuf, int mbsz);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __SPEEDBEE_H__ */
