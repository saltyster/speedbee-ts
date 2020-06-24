#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "speedbee.h"

/*
 * sample 2 カラム作成   HI, MI, LO FIX, LO VAR
 */

// [カラム定義1]
//   カラムタイプ Hi
//   サンプリングレート 10kHz
//   メモリ保存 2000 件
//   データサイズ 4バイト
#define COL1_PAR	"COL_TYPE=H;DATA_SIZE=4;SMPL_RATE=10000;SAVE_COUNT=2000"
#define COL1_NAME			"ch1"		// カラム名称

// [カラム定義2]
//   カラムタイプ Mi
//   初期サンプリングレート 1kHz
//   メモリ保存 1000 件
//   データサイズ 4バイト
#define COL2_PAR	"COL_TYPE=M;DATA_SIZE=4;SMPL_RATE=1000;SAVE_COUNT=1000"
#define COL2_NAME			"ch2"		// カラム名称

// [カラム定義3]
//   カラムタイプ Lo  (LO 固定長)
//   メモリ保存 300 件
//   データサイズ 8バイト
#define COL3_PAR	"COL_TYPE=L;DATA_SIZE=8;SAVE_COUNT=300"
#define COL3_NAME			"ch3"		// カラム名称

// [カラム定義4]
//   カラムタイプ Lo  (LO 可変長)
//   メモリ保存 100 件
#define COL4_PAR	"COL_TYPE=L;SAVE_COUNT=100"
#define COL4_NAME			"ch4"		// カラム名称

int main(int ac, char *av[]) {
	sdtsdb_t db;
	sdtscid_t cids[4];

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

	// カラム1作成
	if ((cids[0] = sdts_create_col(db, (sdid_t)COL1_NAME,
		(char *)COL1_PAR)) < 0) {
		printf("error sdts_create_col [%d]\n", sd_get_err());
		(void)sdts_close_db(db);
		sd_end();
		return 1;
	}
	printf("-- success sdts_create_col [%d] --\n", cids[0]);

	// カラム2 作成
	if ((cids[1] = sdts_create_col(db, (sdid_t)COL2_NAME,
		(char *)COL2_PAR)) < 0) {
		printf("error sdts_create_col [%d]\n", sd_get_err());
		(void)sdts_close_db(db);
		sd_end();
		return 1;
	}
	printf("-- success sdts_create_col [%d] --\n", cids[1]);

	// カラム3 作成
	if ((cids[2] = sdts_create_col(db, (sdid_t)COL3_NAME,
		(char *)COL3_PAR)) < 0) {
		printf("error sdts_create_col [%d]\n", sd_get_err());
		(void)sdts_close_db(db);
		sd_end();
		return 1;
	}
	printf("-- success sdts_create_col [%d] --\n", cids[2]);

	// カラム4 作成
	if ((cids[3] = sdts_create_col(db, (sdid_t)COL4_NAME,
		(char *)COL4_PAR)) < 0) {
		printf("error sdts_create_col [%d]\n", sd_get_err());
		(void)sdts_close_db(db);
		sd_end();
		return 1;
	}
	printf("-- success sdts_create_col [%d] --\n", cids[3]);

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
