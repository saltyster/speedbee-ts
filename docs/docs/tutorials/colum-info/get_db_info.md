---
layout: default
title: DB情報取得
parent: 情報取得
grand_parent: Tutorials
nav_order: 1
last_modified_date: 2020-07-13T12:00:00+0000
---

# DB情報取得

このチュートリアルでは、DB 情報のデータを取得する方法について紹介します。<br>

## カラムデータ取得のサンプルコード

このチュートリアルで紹介するサンプルコードです。<br>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "speedbee.h"

static int print_col_info(sdtscolinfo_t *ip) {

	printf(" ***  column info [%s] ***\n", (char *)ip->cname);
	printf("ctype : %d\n", ip->ctype);
	printf("colpar: %s\n", ip->colpar == NULL ? "null": ip->colpar);
	printf("dsz   : %d\n", ip->dsz);
	printf("rsz   : %d\n", ip->rsz);
	printf("scnt  : %d\n", ip->scnt);
	printf("icnt  : %lld\n", ip->icnt);
	printf("hmsmpl: %g\n", ip->hmsmpl);
	printf("hmst  : %lld\n", ip->hmst);
	printf("msst  : %lld\n", ip->msst);
	printf("mset  : %lld\n", ip->mset);
	printf("mscnt : %lld\n", ip->mscnt);
	printf("mmmax : %d\n", ip->mmmax);
	printf("mmspos: %d\n", ip->mmspos);
	printf("mmucnt: %d\n", ip->mmucnt);
	printf("micnt : %lld\n", ip->micnt);

	return 0;
}

static int print_db_info(sdtsdb_t db) {
	sdtsdbinfo_t *di;
	int i;

	if ((di = sdts_get_db_info(db)) == NULL) {
		printf("error sdts_get_db_info[%d]\n", sd_get_err());
		return -1;
	}
	printf("===  db info ===\n");
	printf(" dbpar    : %s\n", di->dbpar == NULL ? "null" : di->dbpar);
	printf(" dbpath   : %s\n", di->path == NULL ? "null" : di->path);
	printf(" ccnt     : %d\n", di->ccnt);
	printf(" sbsz     : %d\n", di->sbsz);
	printf(" rmaxccnt : %d\n", di->rmaxccnt);
	printf(" sccnt_h  : %d\n", di->sccnt_h);
	printf(" sccnt_m  : %d\n", di->sccnt_m);
	printf(" sccnt_l  : %d\n", di->sccnt_l);
	printf(" sper_h   : %d\n", di->sper_h);
	printf(" sper_m   : %d\n", di->sper_m);
	printf(" sper_l   : %d\n", di->sper_l);
	printf(" lag_h    : %d\n", di->lag_h);
	printf(" lag_m    : %d\n", di->lag_m);
	printf(" lag_l    : %d\n", di->lag_l);
	printf(" maxf_h   : %d\n", di->maxf_h);
	printf(" maxf_m   : %d\n", di->maxf_m);
	printf(" maxf_l   : %d\n", di->maxf_l);
	printf(" stcnt    : %lld\n", di->stcnt);

	printf("  === column info ===\n");
	for (i = 0; i < di->ccnt; i++) {
			print_col_info(di->colinfo[i]);
	}
	
	printf("----------------\n");
	sdts_free_db_info(di);
	return 0;
}


int main(int ac, char *av[]) {
	sdtsdb_t db;
	sdtscid_t cid;

	if (sd_init(NULL) < 0) {
		printf("error sd_init [%d]\n", sd_get_err());
		return 1;
	}
	printf("-- success sd_init --\n");

	if ((db = sdts_open_db("DB_PATH=db1")) == NULL) {
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

	print_db_info(db);

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

## DB情報取得API（sdts_get_db_info）

`sdts_get_db_info`で、DB 作成時に設定されたDB情報を取得します。<br>
戻り値として、DB情報構造体 `sdtsdbinfo_t`で受け取ります。<br>

このサンプルコードでは、DB情報を取得して出力しています。<br>
DB情報それぞれの詳しい内容は、[APIドキュメント](https://github.com/saltyster/speedbee-ts/blob/main/doc/SDTS_API_20200707.pdf)の`sdts_get_db_info`を参照してください。<br>

```c
	if ((di = sdts_get_db_info(db)) == NULL) {
		printf("error sdts_get_db_info[%d]\n", sd_get_err());
		return -1;
	}
	printf("===  db info ===\n");
	printf(" dbpar    : %s\n", di->dbpar == NULL ? "null" : di->dbpar);
	printf(" dbpath   : %s\n", di->path == NULL ? "null" : di->path);
	printf(" ccnt     : %d\n", di->ccnt);
	printf(" sbsz     : %d\n", di->sbsz);
	printf(" rmaxccnt : %d\n", di->rmaxccnt);
	printf(" sccnt_h  : %d\n", di->sccnt_h);
	printf(" sccnt_m  : %d\n", di->sccnt_m);
	printf(" sccnt_l  : %d\n", di->sccnt_l);
	printf(" sper_h   : %d\n", di->sper_h);
	printf(" sper_m   : %d\n", di->sper_m);
	printf(" sper_l   : %d\n", di->sper_l);
	printf(" lag_h    : %d\n", di->lag_h);
	printf(" lag_m    : %d\n", di->lag_m);
	printf(" lag_l    : %d\n", di->lag_l);
	printf(" maxf_h   : %d\n", di->maxf_h);
	printf(" maxf_m   : %d\n", di->maxf_m);
	printf(" maxf_l   : %d\n", di->maxf_l);
```
カラムを作成している場合、DB情報取得でカラム情報の取得もできます。<br>

```c
static int print_col_info(sdtscolinfo_t *ip) {

	printf(" ***  column info [%s] ***\n", (char *)ip->cname);
	printf("ctype : %d\n", ip->ctype);
	printf("colpar: %s\n", ip->colpar == NULL ? "null": ip->colpar);
	printf("dsz   : %d\n", ip->dsz);
	printf("rsz   : %d\n", ip->rsz);
	printf("scnt  : %d\n", ip->scnt);
	printf("icnt  : %lld\n", ip->icnt);
	printf("hmsmpl: %g\n", ip->hmsmpl);
	printf("hmst  : %lld\n", ip->hmst);
	printf("msst  : %lld\n", ip->msst);
	printf("mset  : %lld\n", ip->mset);
	printf("mscnt : %lld\n", ip->mscnt);
	printf("mmmax : %d\n", ip->mmmax);
	printf("mmspos: %d\n", ip->mmspos);
	printf("mmucnt: %d\n", ip->mmucnt);
	printf("micnt : %lld\n", ip->micnt);

	return 0;
}
static int print_db_info(sdtsdb_t db) {
	…
	if ((di = sdts_get_db_info(db)) == NULL) {
		printf("error sdts_get_db_info[%d]\n", sd_get_err());
		return -1;
	}
	…
	printf("  === column info ===\n");
	for (i = 0; i < di->ccnt; i++) {
			print_col_info(di->colinfo[i]);
	}
	…
}
```

## DB情報取得後の解放

取得したDB情報は、`sdts_get_db_info`内でメモリアロケートした領域であるため、参照後、`sdts_free_db_info` APIで確保された領域を解放してください。<br>

```c
static int print_db_info(sdtsdb_t db) {
	sdtsdbinfo_t *di;
	int i;

	if ((di = sdts_get_db_info(db)) == NULL) {
		printf("error sdts_get_db_info[%d]\n", sd_get_err());
		return -1;
	}
	
	…

	sdts_free_db_info(di); // ←取得したDB情報の領域を解放する。
	return 0;
}
```

[Previons]({% link docs/tutorials/colum-info/index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-left }[Next]({% link docs/tutorials/colum-info/get_column_info.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-right }<br>