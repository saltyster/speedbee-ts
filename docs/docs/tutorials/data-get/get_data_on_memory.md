---
layout: default
title: メモリ上のデータ取得
parent: データ取得
grand_parent: Tutorials
nav_order: 2
last_modified_date: 2020-07-13T12:00:00+0000
---

# メモリ上のカラムデータ取得

このチュートリアルでは、メモリ DB に格納されたMiddleタイプのカラムデータを取得する方法について紹介します。<br>
メモリDBに格納されたデータ取得方法として、[spcf_get_col_dat]({% link docs/tutorials/data-get/get_data_on_file.md %})もあります。

## カラムデータ取得のサンプルコード

このチュートリアルで紹介するサンプルコードです。<br>
サンプルコードのファイルは、こちらの[sample4.c](https://github.com/saltyster/speedbee-ts/blob/main/samples/src/sample4.c)にあります。<br>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "speedbee.h"

/*
 * sample 4 カラムタイプ MI データ登録 / カラムデータ取得
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

	//  カラムデータ取得
	//  MIの場合、途中でのサンプリングレートの変更や
	//  ブランクが存在する可能性があるため
	//  必要バッファサイズの計算は難しいが、
	//  目安としてブランクを無視した場合の登録件数を最大値として計算する。
	st = 1001700000;
	et = 2002000000;
	if ((cnt = sdts_get_col_dat(db, cid, st, et, (char *)buf, BSZ, NULL)) < 0) {
		printf("error sdts_get_col_dat [%d]\n", sd_get_err());
		(void)sdts_close_db(db);
		sd_end();
		return 1;
	}
	// 結果データ出力  データフォーマット  ts:8byte + data:4byte
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
```

## カラムへのデータ取得API（sdts_get_col_dat）

メモリ DB に格納された指定した Middleタイプのカラムデータを取得します。<br>
`sdts_get_col_dat`でカラムデータの取得を行います。<br>
引数に取得対象するDB、カラムID、開始タイムスタンプ、終了タイムスタンプ、取得データを格納するもの、取得データサイズを渡しています。<br>

パラメータの詳しい内容は、[APIドキュメント](https://github.com/saltyster/speedbee-ts/blob/main/doc/SDTS_API_20200707.pdf)の`sdts_get_col_dat`を参照してください。

```c
//  カラムデータ取得
//  MIの場合、途中でのサンプリングレートの変更や
//  ブランクが存在する可能性があるため
//  必要バッファサイズの計算は難しいが、
//  目安としてブランクを無視した場合の登録件数を最大値として計算する。
st = 1001700000;
et = 2002000000;
if ((cnt = sdts_get_col_dat(db, cid, st, et, (char *)buf, BSZ, NULL)) < 0) {
    printf("error sdts_get_col_dat [%d]\n", sd_get_err());
    (void)sdts_close_db(db);
    sd_end();
    return 1;
}
// 結果データ出力  データフォーマット  ts:8byte + data:4byte
p = buf;
for (i = 0; i < cnt; i++, p += 12)
    printf("[%03d]t[%lld]d[%d]\n", i + 1, *(sdntime_t *)p, *(int *)(p + 8));
```

### 開始タイムスタンプと終了タイムスタンプ

取得したいデータの範囲として、開始と終了のタイムスタンプを指定する。<br>
タイムスタンプは、UTC 1970-01-01 00:00:00 からの通算ナノ秒となります。<br>

### 取得データを格納するものと取得データサイズ

指定された一つのカラムから時間条件に従い、データを出力バッファにコピーします。<br>

- 取得データには、タイムスタンプ（8byte）と値が一組で格納されています。 <br>

	| ts1 | 値1 | ts2 | 値2 | ……  | tsN  | 値N |

[Previons]({% link docs/tutorials/data-get/get_data_on_file.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-left }[Next]({% link docs/tutorials/cursor-search/index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-right }<br>