---
layout: default
title: カラムタイプ（Middle）
parent: データ登録
grand_parent: Tutorials
nav_order: 1
last_modified_date: 2020-07-13T12:00:00+0000
---

# カラムタイプ（Middle）のデータ登録／カラムデータ取得

このチュートリアルでは、Middleタイプのデータ登録方法について紹介します。<br>

## サンプルコード

このチュートリアルで紹介するサンプルコードです。<br>
サンプルコードのファイルは、[こちら](https://github.com/saltyster/speedbee-ts/blob/main/samples/src/sample4.c)にあります。<br>

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

## カラムへのデータ登録API（sdts_insert）

sdts_insertでカラムを作成します。<br>
引数に取得対象するDB、カラムID、登録時間のタイムスタンプ、登録するデータ配列の先頭アドレス、データ配列数を渡しています。<br>

パラメータの詳しい内容は、[APIドキュメント](https://github.com/saltyster/speedbee-ts/blob/main/doc/SDTS_API_20200707.pdf)の`sdts_insert`を参照してください。

```c
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
```

### sdts_insertの引数カラムID

登録対象のカラムを指定するためにカラムIDを設定します。<br>
カラムIDは、カラム作成時のカラムIDになります。<br>
カラムIDをカラム作成時に保持しておくか、下記のようにカラム名からカラムIDを取得できます。<br>

```c
sdtscid_t cid = sdts_get_col(db, "ch1");
```

### sdts_insertの引数タイムスタンプ

登録するデータのタイムスタンプを指定します。<br>
初回の登録に0を指定した場合、システム時間が初期タイムスタンプになります。<br>
連続でデータ登録を行う場合、2回目以降で0を指定した場合、サンプリングレートの間隔でタイムスタンプを発行して登録します。<br>
サンプリングレート＝1Hzの場合は、2回目の登録は1回目の1秒後のタイムスタンプが設定されます。<br>
0 以外の タイムスタンプ を指定した場合、前回からのサンプリングレートによる経過時間と一致したときのみ、前回の登録の続きと認識する。<br>
それ以外の場合、前回からデータの連続性は途切れ、次のデータシリーズの初回登録と認識する。<br>

### sdts_insertの引数登録するデータ配列の先頭アドレスと配列数

データの登録は、配列を利用して複数データを一括登録することも可能です。<br>
配列にて複数データを登録した場合も、初回のタイムスタンプとサンプリングレートで、タイムスタンプを計算を行います。<br>

## サンプリングレートの変更（sdts_set_smpl_rate）

Middleのカラムタイプの場合、サンプリングレートの変更が可能です。<br>
登録処理を終了（中断）してから行う必要があります。<br>

```c
// サンプリングレートを 40kHzに変更
if (sdts_set_smpl_rate(db, (sdid_t)COL_NAME, 40000) < 0) {
    printf("error sdts_set_smpl_rate[%d]\n", sd_get_err());
    sdts_close_db(db);
    sd_end();
    return 1;
}
```

## データ取得例（sdts_get_col_dat）

メモリDBのカラムデータの取得は、下記のコード部分によって行っています。<br>
永続化DBやメモリDBのデータ取得については、[データ取得]({% link docs/tutorials/data-get/index.md %})を参照ください。<br>

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

[Previons]({% link docs/tutorials/data-register/index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-left }[Next]({% link docs/tutorials/data-get/index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-right }<br>