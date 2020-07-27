---
layout: default
title: リアルタイム処理
parent: Tutorials
nav_order: 6
last_modified_date: 2020-07-13T12:00:00+0000
---

# リアルタイム処理

このチュートリアルでは、SpeeDBee/TSの特徴的な機能のひとつである時間幅（ウィンドウ）単位のリアルタイムなデータ分析機能を紹介します。<br>
リアルタイムなデータ分析の概要は、[SpeeDBee/TSの製品紹介](https://github.com/saltyster/speedbee-ts/blob/main/PRODUCTS_SpeeDBee_TS.md#%E3%83%AA%E3%82%A2%E3%83%AB%E3%82%BF%E3%82%A4%E3%83%A0%E3%81%AA%E3%83%87%E3%83%BC%E3%82%BF%E5%88%86%E6%9E%90)を参照ください。<br>

## サンプルコード

このチュートリアルで紹介するサンプルコードから、リアルタイム処理部分を抜粋したものです。<br>
サンプルコードのファイルは、[こちら](https://github.com/saltyster/speedbee-ts/blob/main/samples/src/SampleCpuMonitoring.c)にあります。<br>
<br>
このサンプルコードは、1秒ごとにCPUの使用率を算出してDBに登録しています。<br>
ここでのリアルタイム処理の紹介は、Window分析機能のご紹介です。<br>
1秒ごとに格納されるCPU使用率において、10秒ごとに格納された10秒間の10個のデータに対して最大値のデータ判定して格納しています。<br>
この一定間隔ごとのデータ処理が、リアルタイム処理のウィンドウ処理になります。<br>

```c
static void db_aggregate_fixed_cycle(CID_PROCESSOR *cid) {
	if (sdts_set_win(g_db, cid->act, "window", SD_WT_COUNT, "WIN_COUNT=10;WIN_STAT=true;", callback_func, cid) < 0) {
		fprintf(stderr, "error sdts_set_win [%d]\n", sd_get_err());
		abort_sample();
	}
}

static int callback_func(sdntime_t ts, void *da, int dc, sdstat_t* st, int ret, void *user_data) {
	if (st) {
		CID_PROCESSOR *proc = (CID_PROCESSOR *)user_data;
		if (sdts_insert(g_db, proc->max, st->etime, (char *) &st->max, 1) != 1) {
			fprintf(stderr, "error sdts_insert [%d] : sdts_insert error in sdts_set_win callback:\n", sd_get_err());
			abort_sample();
		}
	}

	return 0;
}
```

## ウィンドウの設定（sdts_set_win）

リアルタイム処理する上で、データを一定間隔ごとに区切り処理を行います。<br>

このサンプルでは1秒ごとに、CPUの使用率をカラムに格納しています。<br>
このウィンドウでは、CPUの使用率を格納しているカラムに対して10回データが格納されたら、コールバック関数の`callback_func`を呼び出すように設定しています。<br>

`sdts_set_win`の引数には、作成するDB、分析したいカラムのカラムID、ウィンドウ定義名、ウィンドウタイプ、ウィンドウパラメータ、通知コールバック関数、ユーザー定義データのパラメータを渡しています。<br>

- ウィンドウタイプ <br>
指定したウィンドウタイプに応じて、ウィンドウパラメータに設定された処理が実行されます。<br>

	| No | ウィンドウ種別 | ウィンドウタイプ |
	| :--- | :--- | :--- |
	| 1 | SD_WT_COUNT | カウントウィンドウ |
	| 2 | SD_WT_FFT | FFTウィンドウ |

- ウィンドウパラメータ <br>
サンプルプログラムでは、ウィンドウパラメータとして下記のパラメータを設定しています。<br>
`WIN_COUNT`は、カウントウィンドウの件数を指定します。指定登録件数単位でウィンドウ処理を行います。<br>
`WIN_STAT`は、統計処理を行うかどうかの設定です。<br>
統計処理を行うように設定することで、コールバック関数を呼ばれた際に、ウィンドウ範囲内の統計処理結果を得られます。<br>

	```
	WIN_COUNT=10;WIN_STAT=true;
	```

	ウィンドウパラメータの詳細は、[APIドキュメント](https://github.com/saltyster/speedbee-ts/blob/main/doc/SDTS_API_20200707.pdf)を参照ください。<br>


- 通知コールバック関数 <br>
ウィンドウパラメータで設定した、ウィンドウ幅ごとに呼び出されます。<br>
コールバック関数が呼ばれた際に受け取れる引数については、[APIドキュメント](https://github.com/saltyster/speedbee-ts/blob/main/doc/SDTS_API_20200707.pdf)を参照ください。<br>


## ウィンドウのリアルタイム処理

通知コールバック処理で、ウィンドウのの統計処理結果を受け取ります。<br>
通知コールバック関数内で、分析結果を別カラムに登録することができます。<br>
このサンプルでは、統計処理を行うように設定していますので、通知コールバックの引数で受け取った統計処理結果から、ウィンドウ内の最大値を取得してカラムに登録しています。<br>

```c
static int callback_func(sdntime_t ts, void *da, int dc, sdstat_t* st, int ret, void *user_data) {
	if (st) {
		CID_PROCESSOR *proc = (CID_PROCESSOR *)user_data;
		if (sdts_insert(g_db, proc->max, st->etime, (char *) &st->max, 1) != 1) {
			fprintf(stderr, "error sdts_insert [%d] : sdts_insert error in sdts_set_win callback:\n", sd_get_err());
			abort_sample();
		}
	}

	return 0;
}
```

[Previons]({{ site.baseurl }}{% link docs/tutorials/cursor-search/index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-left }[Next]({{ site.baseurl }}{% link docs/tutorials/colum-info/get_db_info.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-right }<br>