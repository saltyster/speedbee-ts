---
layout: default
title: Minimal Start
nav_order: 2
last_modified_date: 2020-07-13T12:00:00+0000
---

# Minimal Start

SpeeDBee/TSのインストールが完了したら、DBを利用する方法についてサンプルを用いながら解説します。
インストールがまだの場合は、[こちら]({{ site.baseurl }}{% link docs/installing-speedbee-ts.md %})を参照ください。

## 前提条件

動作環境において以下をインストールしていること

* make
* gcc
* speedbee

## SpeeDBeeを利用するプログラムを作成します。

1. `sample.c`を作成します。<br>
下記ソースは、speedbee-ts評価版のリポジトリにあるサンプルプログラムになります。[sample1.c](https://github.com/saltyster/speedbee-ts/blob/main/samples/src/sample1.c)

    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "speedbee.h"

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

上記サンプルは、下記手順で処理を行っています。<br>

1. SpeeDBeeの初期化<br>
SDTS-API を利用する環境の初期化を行います。
1. DBオープン<br>
データベースをオープンします。引数のパラメータを指定していないため、メモリDB 機能のみ使用になります。
1. DBクローズ<br>
DB をクローズします。メモリDB 機能を利用しているためデータは破棄されます。
1. SpeeDBeeの終了<br>
SDTS-API のインターフェース終了処理を行います。


## サンプルをコンパイルして実行してみます。

### コンパイル

1. `sample.c`ファイルを作成したフォルダに移動

    ```bash
    $ cd `sample.cのあるフォルダ`
    ```

2. コンパイルして実行ファイルを作成

    ```bash
    $ gcc -O2 -Wall -I/usr/local/include/speedbee -L/usr/local/lib -o sample sample.c -lspeedbee -pthread -ldl -lm
    ```

3. 実行して下記のようにエラーが出ていなかったらOKです。

    ```bash
    $ ./sample
    -- success sd_init --
    -- success sdts_open_db --
    -- success sdts_close_db --
    -- success sd_end --
    ```
[Previons]({{ site.baseurl }}{% link docs/installing-speedbee-ts.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-left }[Next]({{ site.baseurl }}{% link docs/tutorials/index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-right }<br>