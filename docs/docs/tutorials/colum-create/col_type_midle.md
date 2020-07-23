---
layout: default
title: カラムタイプ（Middle）
parent: カラム作成
grand_parent: Tutorials
nav_order: 1
last_modified_date: 2020-07-13T12:00:00+0000
---

# カラムタイプ（Middle）のカラム作成

このチュートリアルでは、Middleタイプのカラム作成方法について紹介します。<br>

## カラム作成のサンプルコード

このチュートリアルで紹介するサンプルコードです。<br>
サンプルコードのファイルは、[こちら](https://github.com/saltyster/speedbee-ts/blob/main/samples/src/sample2.c)にあります。<br>
Middleタイプのカラム作成の部分だけを抜粋しています。<br>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "speedbee.h"

/*
 * sample 2 カラム作成   HI, MI, LO FIX, LO VAR
 */
// [カラム定義2]
//   カラムタイプ Mi
//   初期サンプリングレート 1kHz
//   メモリ保存 1000 件
//   データサイズ 4バイト
#define COL2_PAR	"COL_TYPE=M;DATA_SIZE=4;SMPL_RATE=1000;SAVE_COUNT=1000"
#define COL2_NAME			"ch2"		// カラム名称

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

	// カラム2 作成
	if ((cids[1] = sdts_create_col(db, (sdid_t)COL2_NAME,
		(char *)COL2_PAR)) < 0) {
		printf("error sdts_create_col [%d]\n", sd_get_err());
		(void)sdts_close_db(db);
		sd_end();
		return 1;
	}
	printf("-- success sdts_create_col [%d] --\n", cids[1]);

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

## カラム作成のAPI（sdts_create_col）

sdts_create_colでカラムを作成します。<br>
引数に作成するDB、カラム名、カラム作成のパラメータを渡しています。<br>
パラメータは、カラムのタイプや、データサイズ、サンプリングレート等を指定します。<br>

パラメータの詳しい内容は、[APIドキュメント](https://github.com/saltyster/speedbee-ts/blob/main/doc/SDTS_API_20200707.pdf)の`sdts_create_col`を参照してください。

```c
// [カラム定義2]
//   カラムタイプ Mi
//   初期サンプリングレート 1kHz
//   メモリ保存 1000 件
//   データサイズ 4バイト
#define COL2_PAR	"COL_TYPE=M;DATA_SIZE=4;SMPL_RATE=1000;SAVE_COUNT=1000"
#define COL2_NAME			"ch2"		// カラム名称

    …

// カラム2 作成
if ((cids[1] = sdts_create_col(db, (sdid_t)COL2_NAME,
    (char *)COL2_PAR)) < 0) {
    printf("error sdts_create_col [%d]\n", sd_get_err());
    (void)sdts_close_db(db);
    sd_end();
    return 1;
}
```

## カラムパラメータ

このサンプルコードでは、カラム作成時のパラメータとして以下の項目を設定しています。<br>

- COL_TYPE （カラムタイプ）
- DATA_SIZE （1データのサイズ）
- SMPL_RATE （サンプリングレート）
- SAVE_COUNT （メモリ保存件数）

### COL_TYPE（カラムタイプ）

カラムタイプは、以下のいづれかを指定することができます。<br>
このサンプルでは、Middle を指定しています。<br>

- 'H'（High）
- 'M' (Middle)
- 'L' (Low)

### DATA_SIZE（1データのサイズ）

格納する1つのデータのサイズを指定します。<br>
このサンプルでは、4バイトを指定しています。<br>

### SMPL_RATE（サンプリングレート）

サンプリングレートを指定します。<br>
このサンプルでは、10kHzを指定しています。<br>
サンプリングレート指定することで、初回登録時のタイムスタンプとサンプリングレートで、登録データのタイムスタンプを出力します。<br>
※Middleタイプの場合、カラム作成後にサンプリングレートを変更することができます。

### SAVE_COUNT（メモリ保存件数）

メモリ上に保持するデータ件数を指定します。<br>
この件数を超えるデータを格納する場合、古いデータから削除されます。<br>

## カラム作成の戻り値（カラムID）

カラム作成（sdts_create_col）を行った際の戻り値は、カラムIDになります。<br>
カラムデータの取得などに利用します。<br>


[Previons]({% link docs/tutorials/colum-create/index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-left }[Next]({% link docs/tutorials/data-register/index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-right }<br>