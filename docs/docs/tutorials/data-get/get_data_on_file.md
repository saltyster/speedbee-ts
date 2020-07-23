---
layout: default
title: ファイル上のデータ取得
parent: データ取得
grand_parent: Tutorials
nav_order: 1
last_modified_date: 2020-07-13T12:00:00+0000
---

# 永続化DBのカラムデータ取得

このチュートリアルでは、永続化 DB およびメモリ DB に格納されたMiddleタイプのカラムデータを取得する方法について紹介します。<br>

## カラムデータ取得のサンプルコード

このチュートリアルで紹介するサンプルコードです。<br>
サンプルコードのファイルは、こちらの[sample10.c](https://github.com/saltyster/speedbee-ts/blob/main/samples/src/sample10.c)にあります。<br>
実際に永続化したDBファイルをオープンしているサンプルを参照したい場合は、[sample12.c](https://github.com/saltyster/speedbee-ts/blob/main/samples/src/sample12.c)を参照してください。<br>

```c
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
```

## カラムへのデータ取得API（spcf_get_col_dat）

永続化 DB およびメモリ DB に作成した Middle タイプのカラムデータを取得します。<br>
`spcf_get_col_dat`でカラムデータの取得を行います。<br>
引数には、取得対象のDB、カラムID、検索オプション、検索条件、次の検索情報、取得したデータ情報を格納するもの、取得データを格納するもの、取得データサイズ、取得データ件数を渡しています。<br>

パラメータの詳しい内容は、[APIドキュメント](https://github.com/saltyster/speedbee-ts/blob/main/doc/SDTS_API_20200707.pdf)の`spcf_get_col_dat`を参照してください。<br>

```c
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
```

### `spcf_get_col_dat`を利用するためには？

- `spcf_get_col_dat`を利用するために、`sdts_spcf.h`をincludeする必要があります。

	```c
	#include "sdts_spcf.h"
	```
### `spcf_get_col_dat`の引数

- 検索オプション <br>
検索オプションでは、メモリ上のデータと永続化したファイル上のデータのどちらを検索するかを指定します。<br>
両方を指定した場合は、メモリ上のデータを優先して検索します。<br>

- 検索条件 <br>
DBのカラムに格納されているデータは、タイムスタンプをキーとして紐づけられています。<br>
検索条件として、そのタイムスタンプを指定することが可能です。<br>
UNIXタイムスタンプの通算ナノ秒で指定します。<br>
初回の検索条件を指定すると、2回目以降に0 を指定することで、次回データ情報として
次の引数 dat_nxt_t *nxt を使い検索を行ってくれます。<br>

- 次の検索情報 <br>
API 実行時に次の実行で取得するデータ位置に関する内部情報を以下の構造体に格納しています。<br>
データ取得を連続して実行する場合、この情報をもとに次回以降のデータを取得することで高速にデータを取得していますので、この構造体に対して、値をセットはしないようにします。<br>

- 取得したデータ情報を格納するもの <br>
取得したデータの情報について、以下の構造体として格納されています。<br>

	```c
	typedef struct {
		int smpl;		// サンプリングレート
		uint64_t ts;	// 先頭データのタイムスタンプ
	} dat_hdr_t;
	```

- 取得データを格納するもの <br>
検索結果のデータを参照するために、データバッファのポインタを指定します。<br>

- 取得データサイズ <br>
取得するデータのデータバッファのサイズを指定します。<br>

- 取得データ件数 <br>
取得したデータ件数が格納されます。<br>


### 戻り値

カラムへのデータ取得API（spcf_get_col_dat）の戻り値を元に、正常に取得できたのか、まだ取得できるデータが残っているかを判定できます。<br>

| keyword | note |
| :--- | :--- |
| SPCF_SUCCESS(1) | 正常に取得。次回もデータ取得可能。|
| SPCF_ERROR(-1) | 異常終了|
| SPCF_ERR_OUT_RAGE(0) | 検索条件に指定された時間のデータは DB に存在しない|
| SPCF_MISS_DATA(2) | 正常に取得。次回データは、今回の連続したデータではない。（取得した最終データの後に時間が空いている。）|
| SPCF_END_MEM(3) | メモリ DB 上の最新値に到達。|
| SPCF_END_RATE(4) | 正常に取得。同一サンプルレートの末尾に到達。|
| SPCF_END_FILE(5) | 永続化(ファイル)DB 上の最新値に到達|


## データの一部が存在しない区間について

データの一部が存在しない区間が含まれる場合、`SPCF_MISS_DATA`の戻り値を返却します。<br>

## 途中でサンプリングレートが変更されている場合について

途中でサンプリングレートが変更された場合、戻り値としてSPCF_END_RATEを返します。<br>

[Previons]({% link docs/tutorials/data-get/index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-left }[Next]({% link docs/tutorials/data-get/get_data_on_memory.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-right }<br>