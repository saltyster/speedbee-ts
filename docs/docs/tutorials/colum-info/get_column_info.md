---
layout: default
title: カラム情報取得
parent: 情報取得
grand_parent: Tutorials
nav_order: 2
last_modified_date: 2020-07-13T12:00:00+0000
---

# カラム情報取得

このチュートリアルでは、カラム情報のデータを取得する方法について紹介します。<br>

## カラムデータ取得のサンプルコード

このチュートリアルで紹介するサンプルコードです。<br>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "speedbee.h"

#define ICNT	15

static int print_col_info(sdtsdb_t db, sdtscid_t cid, char *m) {
	sdtscolinfo_t *ip;

	if ((ip = sdts_get_col_info(db, cid)) == NULL) {
		printf("error sdts_get_col_info [%d]\n", sd_get_err());
		return -1;
	}
	printf("-- success sdts_get_col_info --\n");

	printf(" *** %s : column info [%s] ***\n", m, (char *)ip->cname);
	printf("ctype : %d\n", ip->ctype);
	printf("dsz   : %d\n", ip->dsz);
	printf("rsz   : %d\n", ip->rsz);
	printf("scnt  : %d\n", ip->scnt);
	printf("icnt  : %lld\n", ip->icnt);

	printf("hmsmpl : %g\n", ip->hmsmpl);
	printf("hmst   : %lld\n", ip->hmst);

	printf("msst  : %lld\n", ip->msst);
	printf("mset  : %lld\n", ip->mset);
	printf("mscnt : %lld\n", ip->mscnt);

	sdts_free_col_info(ip);

	return 0;
}

int main(int ac, char *av[]) {
	sdtsdb_t db;
	sdtscid_t cid;
	sdntime_t ts;
	int i;
	int d[ICNT];

	for (i = 0; i < ICNT; i++)
		d[i] = i;
	
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

	if ((cid = sdts_create_col(db, (sdid_t)"H0001",
		"COL_TYPE=H;DATA_SIZE=4;SMPL_RATE=10000;SAVE_COUNT=10")) < 0) {
		printf("error sdts_create_col[%d]\n", sd_get_err());
		sdts_close_db(db);
		sd_end();
		return 1;
	}
	printf("-- success sdts_create_col--\n");

	(void)print_col_info(db, cid, "test1");

	/*
 	*  insert 8  - 8
 	*/
	ts = 1000000000;
	for (i = 0; i < 8; i++) {
		if (sdts_insert(db, cid, ts, (char *)&d[i], 1) != 1) {
			printf("error sdts_insert [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			return 1;
		}
		ts = 0;
	}
	printf("-- success sdts_insert [%d] --\n", i);

	(void)print_col_info(db, cid, "test2");

	/*
 	*  insert  3  -> 11
 	*/
	ts = 0;
	for (i = 0; i < 3; i++) {
		if (sdts_insert(db, cid, ts, (char *)&d[i], 1) != 1) {
			printf("error sdts_insert [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			return 1;
		}
	}
	printf("-- success sdts_insert [%d] --\n", i);

	(void)print_col_info(db, cid, "test3");

	/*
 	*  insert  15  -> 26
 	*/
	for (i = 0; i < ICNT; i++) {
		if (sdts_insert(db, cid, ts, (char *)&d[i], 1) != 1) {
			printf("error sdts_insert [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			return 1;
		}
	}
	printf("-- success sdts_insert [%d] --\n", i);

	(void)print_col_info(db, cid, "test4");

	if (sdts_close_db(db) < 0) {
		printf("error sdts_close_db [%d]\n", sd_get_err());
		sd_end();
		return 1;
	}
	printf("-- success sd_close_db --\n");

	if (sd_end() < 0) {
		printf("error sd_end [%d]\n", sd_get_err());
		return 1;
	}
	printf("-- success sd_end --\n");

	return 0;
}
```

## カラム情報取得API（sdts_get_col_info）

`sdts_get_col_info`で、カラム作成時に設定されたカラム情報を取得します。<br>
戻り値として、カラム情報構造体`sdtscolinfo_t`で受け取ります。<br>

このサンプルコードでは、作成したカラム情報について、カラムIDを指定して取得しています。 <br>
詳しい内容は、[APIドキュメント](https://github.com/saltyster/speedbee-ts/blob/main/doc/SDTS_API_20200707.pdf)の`sdts_get_col_info`を参照してください。<br>

```c
static int print_col_info(sdtsdb_t db, sdtscid_t cid, char *m) {
	sdtscolinfo_t *ip;

	if ((ip = sdts_get_col_info(db, cid)) == NULL) {
		printf("error sdts_get_col_info [%d]\n", sd_get_err());
		return -1;
	}
	printf("-- success sdts_get_col_info --\n");

	printf(" *** %s : column info [%s] ***\n", m, (char *)ip->cname);
	printf("ctype : %d\n", ip->ctype);
	printf("dsz   : %d\n", ip->dsz);
	printf("rsz   : %d\n", ip->rsz);
	printf("scnt  : %d\n", ip->scnt);
	printf("icnt  : %lld\n", ip->icnt);

	printf("hmsmpl : %g\n", ip->hmsmpl);
	printf("hmst   : %lld\n", ip->hmst);

	printf("msst  : %lld\n", ip->msst);
	printf("mset  : %lld\n", ip->mset);
	printf("mscnt : %lld\n", ip->mscnt);

	sdts_free_col_info(ip);

	return 0;
}
```

## カラム情報取得後の領域の解放

取得したDB情報は、`sdts_get_col_info`内でメモリアロケートした領域であるため、参照後、`sdts_get_col_info` APIで確保された領域を解放してください。<br>

```c
sdts_free_col_info(ip);
```

[Previons]({% link docs/tutorials/colum-info/get_db_info.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-left }[Next]({% link docs/tutorials/data_persistence.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-right }<br>