# SpeeDBee

> 時系列DBのSpeeDBeeは、登録データのリアルタイム分析を用いてウィンドウ単位でのデータ分析およびイベント通知を行えます。

SpeeDBeeにおける時系列DBは、インメモリDB機能を利用して高速にデータを時間に紐づけて登録します。<br>
DBはデータ登録可能なカラム（列）を複数保持し、カラムへのデータ登録は各カラム単位で、時間、値（もしくは値の配列）を設定し行います。<br>
登録されたデータは集約検索を利用して、同一タイムスタンプ上に集約して取得する事が可能です。
登録データは、リアルタイム分析機能を用い、ウィンドウ単位でのデータ分析およびイベント通知を送ることができます。<br>

登録データは、指定によりファイル保存（永続化）することができます。また、永続化データはインメモリDB機能と同じ集約検索APIを用いて検索することができます。<br>

製品の特徴については、[SpeeDBee/TS](./PRODUCTS_SpeeDBee_TS.md)を参照ください。

---

## Table of Contents (Optional)

- [Getting Started](#Getting-Started)
- [Running the Samples](#Running-the-Samples)
- [Documentation](#documentation)
- [Install](#Install)
- [License](#license)
- [Contacts](#Contacts)

### [Getting Started](#Getting-Started)

この手順により、SpeeDBeeを体感することができます。

### 動作環境

* Ubuntu 18.04/20.04

※製品版ではWindowsやT-Kernel, Non-OSにも対応します。

### 前提条件

動作環境において以下をインストールしていること

* make
* gcc

```bash
sudo apt install gcc make
```

### サンプルプログラムのビルド＆実行

```console
$ git clone https://github.com/saltyster/speedbee-ts.git
$ cd speedbee-ts
$ make
$ ./samples/src/sample1
```

次に、SpeeDBeeを使って、基本的なDB操作やデータ取得、ストレージへの保存、リアルタイム分析を行う例を示します。

## [Running the Samples](#Running-the-Samples)

SpeeDBeeの機能を紹介するサンプル。
- データベースオブジェクト基本操作
- カラムデータ登録・参照・保存
- 集約カーソル検索
- シリアライズカーソル検索
- 通知処理

### Sample List

| No. | Title | run command | note | file name |
| :---: | :---: | :--- | :--- | :---: |
|1|Created DB|`./samples/src/sample1`|DB 作成, DB オープン/クローズ.|sample1.c|
|2|Created DB column|`./samples/src/sample2`|カラム作成   HI, MI, LO FIX, LO VAR.|sample2.c|
|3|INSERT/GET Column type HI|`./samples/src/sample3`|カラムタイプ HI データ登録 / カラムデータ取.|sample3.c|
|4|INSERT/GET Column type MI|`./samples/src/sample4`|カラムタイプ MI データ登録 / カラムデータ取得.|sample4.c|
|5|INSERT/GET Column type LO FIX|`./samples/src/sample5`|カラムタイプ LO FIX(固定長) データ登録 / カラムデータ取得.|sample5.c|
|6|INSERT/GET Column type LO VAR|`./samples/src/sample6`|カラムタイプ LO VAR(可変長)データ登録 / カラムデータ取得.|sample6.c|
|7|Aggregate cursor search|`./samples/src/sample7`|集約カーソル検索|sample7.c|
|8|Serialized cursor search|`./samples/src/sample8`|シリアライズカーソル検索|sample8.c|
|9|Aggregate cursor search option|`./samples/src/sample9`|集約カーソル検索  オプション|sample9.c|
|10|INSERT/GET Column type MI. get spcf.|`./samples/src/sample10`|カラムタイプ MI データ登録 / spcfカラムデータ取得|sample10.c|
|11|Data Persistence (HI MI LO)|`./samples/src/sample11`|データ永続化（HI MI LO）|sample11.c|
|12|Referencing persistent data (MI)|`./samples/src/sample12`|永続化データの参照（MI）|sample12.c|
|13|Referencing persistent data (HI)|`./samples/src/sample13`|永続化データの参照（HI）|sample13.c|
|14|Referencing persistent data (LO FIX)|`./samples/src/sample14`|永続化データの参照（LO FIX）|sample14.c|
|15|Referencing persistent data (LO VAR)|`./samples/src/sample15`|永続化データの参照（LO VAR）|sample15.c|


## [Documentation](#documentation)

ドキュメントは現在準備中です。

## [Install](#Install)

SpeeDBeeを環境で利用するためには、下記の手順でインストールしてお試しください。

1. 評価版[リリースファイル](https://github.com/saltyster/speedbee-ts/releases)をダウンロード
   ```bash
   $ curl https://github.com/saltyster/speedbee-ts/releases/download/v621/speedbee-ts-trial-v621-linux-x86_64.tar.gz
   ```
2. 評価版リリースファイルをインストール
   ```bash
   $ cd /usr/local
   $ sudo tar xzvf ${path_to_tar.gz}/speedbee62-linux-x86_64.tar.gz
   $ sudo ldconfig
   ```

## [License](#License)

SamplesプログラムのラインセンスはMITになります。
※ライブラリご利用の詳細については、お問い合わせください。

## [Contacts](#Contacts)

* **株式会社ソルティスター** - [SALTYSTER](http://www.saltyster.com/)

SpeeDBeeについてのお問い合わせは、以下のメールアドレス、またはIssueにてお問い合わせください。
SpeeDBeeに関するお問い合わせメールアドレス : speedbee@saltyster.com


## [Acknowledgments](#Acknowledgments)

* このSpeeDBeeのライブラリは、株式会社ソルティスターの製品の一つです。ぜひ、お試しいただきSpeeDBeeの良さを体験してください。
* SpeeDBeeの試運転をよろしくお願いいたします！

