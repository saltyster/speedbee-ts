#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "speedbee.h"

/*
 * sample 5 カラムタイプ LO FIX(固定長) データ登録 / カラムデータ取得
 */

// [カラム定義]
//   カラムタイプ LO FIX
//   メモリ保存 1000 件
//   データサイズ 4バイト
#define COL_PAR	"COL_TYPE=L;DATA_SIZE=4;SAVE_COUNT=1000"
#define COL_NAME			"ch1"		// カラム名称

// 登録件数
#define ICNT		100

// バッファサイズ
#define BSZ			4096

int main(int ac, char *av[]) {
	sdtsdb_t db;
	sdtscid_t cid;
	sdntime_t ts, st, et;
	char buf[BSZ];
	char *p;
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

	ts = 1000000000;
	for (i = 0; i < ICNT; i++) {
		// 登録
		if (sdts_insert(db, cid, ts, (char *)&i, 1) != 1) {
            printf("error sdts_insert [%d]\n", sd_get_err());
            (void)sdts_close_db(db);
            sd_end();
            return 1;
        }
		// タイムスタンプ設定  0を指定した場合システム時間が設定される。
		ts += 100000;
    }
	printf("-- success sdts_insert [%d] --\n", i);

	//  カラムデータ取得
	st = 1001100000;
	et = 1002100000;
	if ((cnt = sdts_get_col_dat(db, cid, st, et, (char *)buf, BSZ, NULL)) < 0) {
		printf("error sdts_get_col_dat [%d]\n", sd_get_err());
		(void)sdts_close_db(db);
		sd_end();
		return 1;
	}

	// 結果データ出力 LO データフォーマット ts:8 byte + data:4 byte
	p = buf;
	for (i = 0; i < cnt; i++, p += 12)
		printf("[%03d]t[%lld]d[%d]\n", i + 1, *(sdntime_t *)p, *(int *)(p + 8));

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
