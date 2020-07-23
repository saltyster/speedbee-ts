---
layout: default
title: DBオープンとクローズ
parent: Tutorials
nav_order: 1
last_modified_date: 2020-07-13T12:00:00+0000
---

# DBオープンとクローズ

このチュートリアルでは、SpeeDBee/TSを利用するためのSpeeDBee/TSの各APIについて、サンプルコードを元にいくつかの例を紹介します。<br>
APIの詳細については、[APIドキュメント](https://github.com/saltyster/speedbee-ts/blob/main/doc/SDTS_API_20200707.pdf)を参照ください。<br>


## DB Open/Closeのサンプルコード

このチュートリアルで紹介するサンプルコードです。<br>
サンプルコードのファイルは、[こちら](https://github.com/saltyster/speedbee-ts/blob/main/samples/src/sample1.c)にあります。<br>
<br>
[Minimal Start]({% link docs/minimal-start.md %})でも紹介していますが、SpeeDBeeを利用するためには、初期化、DBオープン、DBクローズ、終了処理が必要です。<br>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "speedbee.h"

/*
 * sample 1 DB 作成, DB オープン/クローズ
 */
int main(int ac, char *av[]) {
	sdtsdb_t db;

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
## API利用のための初期化と終了処理について

- sd_init <br>
SpeeDBeeではAPI利用のための初期化処理が必要です。

- sd_end <br>
APIのインターフェース終了処理を行います。

## DBのオープン（作成）

### メモリDBの作成

DBオープン時に、DBパラメータのDB_PATHを設定すると永続化DBになります。<br>
メモリオンリーのDBにする場合は、DB_PATHを指定しないようにします。<br>

```c
if ((db = sdts_open_db(NULL)) == NULL) {
	printf("error sdts_open_db [%d]\n", sd_get_err());
	sd_end();
	return 1;
}
```

### ファイルDBのオープン

永続化したDBを作成、またはオープンする場合には、`sdts_open_db`のAPIでDB_PATHを指定します。<br>

```c
if ((db = sdts_open_db("DB_PATH=" DBDIR)) == NULL) {
	printf("error sdts_open_db [%d]\n", sd_get_err());
	sd_end();
	return 1;
}
```

## DBのクローズ

永続化DBの場合、同期されていないデータを同期して終了します。
メモリDBの場合、DB上のデータは破棄されます。

- 留意事項<br>
DBクローズ時には、他プロセス及びタスクがDBにアクセスしていないことを確認してからクローズ処理を実行してください。


[Previons]({% link docs/tutorials/index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-left }[Next]({% link docs/tutorials/colum-create/index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-right }<br>