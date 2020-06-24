/**
 * @file spcufa.h
 * @brief Stream database timeserise API
 * @version $Id:  $
 * @par Copyright (C) 2008 - 2019 SALTYSTER Inc. All right reserved.
 */
#ifndef __SDTS_SPCF_H__
#define __SDTS_SPCF_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "speedbee.h"

#include <stdint.h>

// data header
typedef struct {
	double		smpl;
	uint64_t	ts;
} dat_hdr_t;

// next data information
typedef struct {
	uint64_t	t;	// use internal - next data timestamp
	uint64_t	m;	// use internal
	uint16_t	n;	// use internal
	uint16_t	p;	// use internal
	uint32_t	s;	// use internal
} dat_nxt_t;

#define SPCF_ERROR			(-1)
#define SPCF_ERR_OUT_RAGE	(0)
#define SPCF_SUCCESS		(1)
#define SPCF_MISS_DATA		(2)
#define SPCF_END_MEM		(3)
#define SPCF_END_RATE		(4)
#define SPCF_END_FILE		(5)

/* --------------------------------------------------------------------------
 * custom api
 * --------------------------------------------------------------------------
 */
DllImport int spcf_get_col_dat(sdtsdb_t db, sdtscid_t cid,
		sdtscdopt_t opt, uint64_t ts, dat_nxt_t *nxt, dat_hdr_t *hdr,
		uint8_t *buf, int32_t bsz, int32_t *ocnt);
DllImport void spcf_clean_col_dat(sdtsdb_t db);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __STDS_SPCF_H__ */
