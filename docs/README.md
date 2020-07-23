# SpeeDBee のチュートリアルドキュメントについて

SpeeDBeeチュートリアルドキュメントの編集方法やローカル環境での参照方法を記載しています。<br>
SpeeDBeeチュートリアルドキュメントは、GitHub Pagesの標準搭載されている Jekyll という静的サイトジェネレータを利用します。<br>

テンプレートは、こちらのものを利用しています。[使っているテンプレート元](https://github.com/pmarsceill/just-the-docs)<br>

## [目次](#目次)

- [ドキュメントをWebページで見る](#ドキュメントをWebページで見る)
- [ドキュメントの編集方法](#ドキュメントの編集方法)
- [ローカル環境での参照方法](#ローカル環境での参照方法)
- [ドキュメントルール](#ドキュメントルール)

## [ドキュメントをWebページで見る](#ドキュメントをWebページで見る)

ドキュメントを見るには下記リンクをクリックしてください。
[SpeeDBeeチュートリアルドキュメント](https://saltyster.github.io/speedbee-ts/)

## [ドキュメントの編集方法](#ドキュメントの編集方法)

※GitHubにPushしても、GitHub Pagesとして見れるように反映されるのに少し時間がかかるようです。変更してしばらくしてから反映されたかをご確認ください。<br>
もう少し詳しい内容は、テンプレート元の説明をご確認ください。[使っているテンプレート元](https://github.com/pmarsceill/just-the-docs)<br>

### 編集するファイルの置き場所

ドキュメントの本体は下記フォルダに格納されています。
> speedbee-ts/docs/docs 

ツリー構造にするには、フォルダ構造だけではできないため。
親と子の設定をする必要があります。
```
+-- ..
|-- (Jekyll files)
|
|-- docs
|   |-- Installing SpeeDBee
|   |-- Minimal Start
|   |-- Tutorial
|   |   |-- index.md  (parent page)
|   |   |-- buttons.md
|   |   |-- code.md
|   |   |-- labels.md
|   |   |-- tables.md
|   |   +-- typography.md
|   |
|   |-- (other md files, pages with no children)
|   +-- ..
|
|-- index.md (HOME page)
|
|-- (Jekyll files)
+-- ..
```

### 編集するファイルのヘッダ情報

最小の構成
```layout```は、_layoutsフォルダにあるレイアウト設定を適用できます。特定のページのみレイアウトを変えたい場合は、_layoutsにレイアウトを追加します。
```title```は、パンくずリストや親子紐づけに利用される。
```nav_order```は、その階層の並び順を指定する項目。
```last_modified_date```は、対象のページを開いた際の、ページ下部にある更新日に利用される。
```
---
layout: default
title: Minimal Start
nav_order: 2
last_modified_date: 2020-07-13T12:00:00+0000
---
```

階層構造を作る場合の指定方法は、以下を参考にしてください。
詳しい内容は、[使っているテンプレート元の説明](https://pmarsceill.github.io/just-the-docs/docs/navigation-structure/)を参照してください。
```
---
layout: default
title: データ取得
parent: Tutorials
nav_order: 4
has_children: true
permalink: /docs/tutorials/data-get
last_modified_date: 2020-07-13T12:00:00+0000
---
```

## 新しく章を追加したい場合

Tutorialsの構造を参考いただくか、[使っているテンプレート元の説明](https://pmarsceill.github.io/just-the-docs/docs/navigation-structure/)を参照してください。

## [ローカル環境での参照方法](#ローカル環境での参照方法)

GitHubに公開する前に、ローカル環境で確認するための環境構築と参照方法を記載します。

### ローカル環境構築

前提として、WLSのUbuntu18.04での環境構築になります。<br>
※WSLはインストール済みとします。<br>

> sudo apt-get install ruby-full build-essential zlib1g-dev

> echo '# Install Ruby Gems to ~/gems' >> ~/.bashrc 

> echo 'export GEM_HOME="$HOME/gems"' >> ~/.bashrc

> echo 'export PATH="$HOME/gems/bin:$PATH"' >> ~/.bashrc

> source ~/.bashrc

> gem install jekyll bundler

### ローカルでWebSiteを参照する方法

speedbee-tsの「docs」フォルダに移動します。
> cd ${SPDB_TS}/docs

インストールを実行し、「Bundle complete!」が出てきたら成功です。
> bundle install

次にサーバーを起動します。
> bundle exec jekyll serve

起動で来たらブラウザで、```http://localhost:4000```でアクセスできます。<br>
```Server address:```に、記載されているアドレスでローカル環境で参照することができます。<br>
終了するには、```Ctrl+C```です。


## [ドキュメントルール](#ドキュメントルール)

ドキュメント作成時のJekyllに沿った、記載方法などのルールをこちらにまとめていきます。<br>

### headerのルール

- ```last_modified_date```は、必ず設定すること。更新したらこの日付も更新すること。
