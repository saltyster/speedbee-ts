#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "speedbee.h"
#include "sdts_spcf.h"

/*
 * sample 10 カラムタイプ MI データ登録 / spcfカラムデータ取得
 */

// [カラム定義]
//   カラムタイプ Mi
//   初期サンプリングレート 10kHz
//   メモリ保存 1000 件
//   データサイズ 4バイト
#define COL_PAR	"COL_TYPE=M;DATA_SIZE=4;SMPL_RATE=10000;SAVE_COUNT=1000"
#define COL_NAME			"ch1"		// カラム名称

// バッファサイズ
#define BSZ			4096

int main(int ac, char *av[]) {
	sdtsdb_t db;
	sdtscid_t cid;
	sdntime_t ts;
	int i, cnt;

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

	if ((cid = sdts_create_col(db, (sdid_t)COL_NAME, (char *)COL_PAR)) < 0) {
		printf("error sdts_create_col [%d]\n", sd_get_err());
		(void)sdts_close_db(db);
		sd_end();
		return 1;
	}
	printf("-- success sdts_create_col [%d] --\n", cid);

	// タイムスタンプ設定 
	// 0を指定した場合システム時間が初期タイムスタンプとなる。
	ts = 1000000000;
	for (i = 0; i < 100; i++) {
		// 登録
		if (sdts_insert(db, cid, ts, (char *)&i, 1) != 1) {
			printf("error sdts_insert [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			return 1;
		}
		// 初回以降、登録時のタイムスタンプが 0 である場合、
		// または、指定されたサンプリングレートでの間隔で
		// 初回からの連続したタイムスタンプある場合は 
		// 連続性があるデータとして認識される。
		ts = 0;
	}
	printf("-- success sdts_insert [%d] --\n", i);

	// サンプリングレートを 40kHzに変更
	if (sdts_set_smpl_rate(db, (sdid_t)COL_NAME, 40000) < 0) {
		printf("error sdts_set_smpl_rate[%d]\n", sd_get_err());
		sdts_close_db(db);
		sd_end();
		return 1;
	}

	ts = 2000000000;
	for ( ; i < 200; i++) {
		// 登録
		if (sdts_insert(db, cid, ts, (char *)&i, 1) != 1) {
			printf("error sdts_insert [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			return 1;
		}
		ts = 0;
	}
	printf("-- success sdts_insert [%d] --\n", i);


	dat_nxt_t nxt;
	ts = 1001700000;
	cnt = 0;
	while (1)
	{
		int outc;
		dat_hdr_t hdr;
		int buff[BSZ];

		int ret = spcf_get_col_dat(db, cid, SDTS_STO_OPT_MEM, ts, &nxt, &hdr, (uint8_t*)buff, BSZ, &outc);
		if (ret == SPCF_ERROR) {
			printf("SPCF_ERROR [%d]\n" , sd_get_err()); break;
		} else if (ret == SPCF_ERR_OUT_RAGE) {
			printf("SPCF_ERR_OUT_RAGE\n"); break;
		} else {
			printf("rate=%f, ts=%ld\n", hdr.smpl, hdr.ts);
			// SPCF_SUCCESS SPCF_MISS_DATA SPCF_END_MEM SPCF_END_RATE
			int *p = &buff[0];
			for (i = 0; i < outc; i++)
				printf("[%d]: %d\n", cnt++, *p++); // data 4 byte integer
		}
		if (ret == SPCF_END_MEM) break;
		ts = 0;
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
