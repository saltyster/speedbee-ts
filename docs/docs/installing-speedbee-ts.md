---
layout: default
title: Installing SpeeDBee-TS
nav_order: 2
last_modified_date: 2020-07-13T12:00:00+0000
---

# Installing SpeeDBee-TS

## 動作環境（評価版の動作環境）

* Ubuntu 18.04/20.04
* Raspberry PI3/rasbian

※製品版ではWindowsやT-Kernel, Non-OSにも対応します。<br>
製品版の動作環境はスペック表をご確認ください。[スペック](https://github.com/saltyster/speedbee-ts/blob/master/doc/SDTS_API_20200707.pdf)

## 前提条件

動作環境において以下をインストールしていること

* make
* gcc

```bash
$ sudo apt install gcc make
```

## SpeeDBeeライブラリのインストール

1. 評価版[リリースファイル](https://github.com/saltyster/speedbee-ts/releases)をダウンロード<br>
※評価版リリースファイルには、評価版用の機能制限が含まれています。
   ```bash
   $ wget https://github.com/saltyster/speedbee-ts/releases/download/v621/speedbee-ts-trial-v621-linux-x86_64.tar.gz
   ```
2. 評価版リリースファイルをインストール<br>
   ```bash
   $ cd /usr/local
   $ sudo tar xzvf ${path_to_tar}/speedbee-ts-trial-v621-linux-x86_64.tar.gz
   $ sudo ldconfig
   ```

[Previons]({% link index.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-left }[Next]({% link docs/minimal-start.md %}){: .btn .btn-outline .fs-5 .mb-4 .mb-md-0 .mr-2 .float-right }<br>