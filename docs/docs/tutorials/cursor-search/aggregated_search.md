---
layout: default
title: 集約検索
parent: 集約カーソル検索
grand_parent: Tutorials
nav_order: 1
last_modified_date: 2020-07-13T12:00:00+0000
---

# 集約検索

このチュートリアルでは、DB のカラムに格納されたデータを範囲指定して、範囲内で集計された結果を取得する方法について紹介します。<br>

## 集約カーソル検索

集約検索はタイムスタンプで指定した検索範囲を指定された刻み値ごとに検索します。<br>
例として、以下のようなデータがDBに登録されているとします。<br>

| No | TimeStamp | Value |
| :---: | :---: | :---: |
| 1 | 1000 | a |
| 2 | 2000 | b |
| 3 | 3000 | c |
| 4 | 4000 | d |

上記のデータは、1000、2000、3000、4000のタイムスタンプごとの4件のレコードになります。<br>
検索範囲を1000～4000、刻み値を2000で集約検索した場合は、検索結果として下記のデータをそれぞれの区間の代表値として取得することができる。<br>
取得できる値については、検索オプションを設定することで、区間の最初のデータなのか、最後のデータなのかを指定できる。<br>

| No | TimeStamp | Value |
| :---: | :---: | :---: |
| 1 | 2000 | b |
| 2 | 4000 | d |


## カラムデータ取得のサンプルコード

このチュートリアルで紹介するサンプルコードです。<br>
サンプルコードのファイルは、こちらの[sample7.c](https://github.com/saltyster/speedbee-ts/blob/main/samples/src/sample7.c)にあります。<br>

```c
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
```

## 集約検索の流れ

集約検索では下記のAPIを使用することで、検索と集約検索結果のデータを取得することができます。<br>

1. sdts_open_cur <br>
検索範囲や集約条件を指定する。<br>
1. sdts_fetch_cur <br>
集約用検索カーソルを移動します。<br>
1. sdts_get_cur_aggr <br>
集約検索カーソルから各カラムの集約値を取得します。<br>
1. sdts_close_cur <br>
集約検索用カーソルをクローズします。<br>
メモリ領域を確保していますので、クローズは必須になります。<br>


```c
static int cursor_aggr(sdtsdb_t db, sdtscid_t *cids,
    …

	if ((cur = sdts_open_cur(db, cids, ccnt, st, et, iv, 0)) < 0) {
		printf("error sdts_open_cur [%d][%d]\n", i, sd_get_err());
		return -1;
	}
    …
	while ((ret = sdts_fetch_cur(cur)) == SD_FETCH_OK) {
		if (sdts_get_cur_aggr(cur, &ts, val, CCNT) < 0) {
			printf("error sdts_get_cur_aggr [%d][%d]\n", i, sd_get_err());
			sdts_close_cur(cur);
			return -1;
		}
        …
	}
    …
	if (sdts_close_cur(cur) < 0) {
		printf("error sdts_close_cur [%d][%d]\n", i, sd_get_err());
		return -1;
	}

    …
}
```

[Previons]({% link docs/tutorials/cursor-search/index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-left }[Next]({% link docs/tutorials/real_time_process.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-right }<br>