#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "speedbee.h"

/*
 * sample 7 集約カーソル検索
 */
// [カラム定義1]
//   カラムタイプ Hi
//   サンプリングレート 10kHz
//   メモリ保存 2000 件
#define COL_PAR1 "COL_TYPE=H;DATA_SIZE=4;SMPL_RATE=10000;SAVE_COUNT=2000"
#define COL_NAME1			"ch1"		// カラム名称

// [カラム定義2]
//   カラムタイプ 	Mi
//   サンプリングレート 1kHz
//   メモリ保存 2000 件
#define COL_PAR2 "COL_TYPE=Mi;DATA_SIZE=4;SMPL_RATE=1000;SAVE_COUNT=2000"
#define COL_NAME2			"ch2"		// カラム名称

// 登録件数
#define ICNT		1000

// カラム数
#define CCNT		2

static int cursor_aggr(sdtsdb_t db, sdtscid_t *cids,
	int ccnt, sdntime_t st, sdntime_t et, sdntime_t iv) {
	sdtscur_t cur;
	sdtscurval_t val[CCNT];
	int i, j, ret;
	sdntime_t ts;

	if ((cur = sdts_open_cur(db, cids, ccnt, st, et, iv, 0)) < 0) {
		printf("error sdts_open_cur [%d][%d]\n", i, sd_get_err());
		return -1;
	}
	printf("-- success sdts_open_cur st[%lld] et[%lld] iv[%lld]--\n", st, et, iv);

	i = 0;
	while ((ret = sdts_fetch_cur(cur)) == SD_FETCH_OK) {
		if (sdts_get_cur_aggr(cur, &ts, val, CCNT) < 0) {
			printf("error sdts_get_cur_aggr [%d][%d]\n", i, sd_get_err());
			sdts_close_cur(cur);
			return -1;
		}
		printf("[%03d]ts[%lld]\n", i + 1, ts);
		for (j = 0; j < CCNT; j++) {
			printf("     c[%d]:", j + 1);
			if (val[j].ind) {
				printf("cid[%d]r[%lld]d[%d]s[%d]\n",
					val[j].cid, val[j].ts, *(int *)val[j].vp, val[j].vsz);
			} else {
				printf("null\n");
			}
		}
		i++;
	}
	if (ret == SD_FETCH_ERR) {
		printf("error sdts_fetch_cur [%d][%d]\n", i, sd_get_err());
		sdts_close_cur(cur);
		return -1;
	}
	if (sdts_close_cur(cur) < 0) {
		printf("error sdts_close_cur [%d][%d]\n", i, sd_get_err());
		return -1;
	}
	printf("-- success sdts_close_cur --\n");

	return 0;
}

int main(int ac, char *av[]) {
	sdtsdb_t db;
	sdtscid_t cids[CCNT];
	sdntime_t ts, st, et, iv;
	int i;

	if (sd_init(NULL) < 0) {
		printf("error sd_init [%d]\n", sd_get_err());
		return 1;
	}
	printf("-- success sd_init --\n");

	if ((db = sdts_open_db(NULL)) == NULL) {
		printf("error sdts_open_db [%d]\n", sd_get_err());
		sd_end();
		return 1;
	}
	printf("-- success sdts_open_db --\n");

	if ((cids[0] = sdts_create_col(db, (sdid_t)COL_NAME1, (char *)COL_PAR1)) < 0) {
		printf("error sdts_create_col [%d]\n", sd_get_err());
		(void)sdts_close_db(db);
		sd_end();
		return 1;
	}
	printf("-- success sdts_create_col [%d] --\n", cids[0]);

	if ((cids[1] = sdts_create_col(db, (sdid_t)COL_NAME2, (char *)COL_PAR2)) < 0) {
		printf("error sdts_create_col [%d]\n", sd_get_err());
		(void)sdts_close_db(db);
		sd_end();
		return 1;
	}
	printf("-- success sdts_create_col [%d] --\n", cids[1]);

	ts = 1000000000;
	for (i = 0; i < ICNT; i++) {
		int v = i + 10000;
		if (sdts_insert(db, cids[0], ts, (char *)&i, 1) != 1) {
			printf("error sdts_insert [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			return 1;
        }
		if (sdts_insert(db, cids[1], ts, (char *)&v, 1) != 1) {
			printf("error sdts_insert [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			return 1;
        }
        ts = 0;
    }
	printf("-- success sdts_insert [%d] --\n", i);

	//  カーソル検索
	st = 1001100000;
	et = 1004100000;
	iv = 500000;	// aggr interval 500us
	if (cursor_aggr(db, cids, CCNT, st, et, iv) < 0) {
		(void)sdts_close_db(db);
		sd_end();
		return 1;
	}

	if (sdts_close_db(db) < 0) {
		printf("error sdts_close_db [%d]\n", sd_get_err());
		sd_end();
		return 1;
	}
	printf("-- success sdts_close_db --\n");

	if (sd_end() < 0) {
		printf("error sd_end [%d]\n", sd_get_err());
		return 1;
	}
	printf("-- success sd_end --\n");

	return 0;
}
