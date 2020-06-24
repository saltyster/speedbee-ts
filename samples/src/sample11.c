#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include "speedbee.h"
#include "sdts_spcf.h"

/*
 * sample 11 データ永続化（HI MI LO）
 */

/* 固定値  */
#define COUNT_OF_ELEMENTS(arr) (sizeof(arr)/sizeof(arr[0]))
#define BSZ			4096
#define ONE_SEC    (1000000000LL)
#define HI_DATA_SIZE (1000)
#define DBDIR "sample11-12db.sdts"
const sdntime_t DURATION = ONE_SEC;
const sdntime_t BASE_TIME = 1589896352381000000LL;   // 2020-05-19T13:52:32.381Z


/* 静的グローバル変数  */
static sdtsdb_t db;
static int hi_data[HI_DATA_SIZE];
static int running = 0;
static struct { const char *name; const char *param; sdtscid_t cid; } col_params[] = {
  { "ch_hi", "COL_TYPE=H;DATA_SIZE=4;SMPL_RATE=10000;SAVE_COUNT=200000;WR_DATA=Yes", 0 },
  { "ch_mi", "COL_TYPE=M;DATA_SIZE=4;SMPL_RATE=200;SAVE_COUNT=8000;WR_DATA=Yes;MM_COUNT=300", 0 },
  { "ch_lo_fix", "COL_TYPE=L;DATA_SIZE=4;SAVE_COUNT=1000;WR_DATA=Yes", 0 },
  { "ch_lo_var", "COL_TYPE=L;SAVE_COUNT=1000;WR_DATA=Yes", 0 },
};

/* static関数プロトタイプ  */
static void insert_hi(sdtsdb_t db, sdtscid_t cid, uint64_t ts, uint64_t duration, double rate);
static void insert_mi(sdtsdb_t db, sdtscid_t cid, uint64_t ts, uint64_t duration, double rate);
static void insert_lo(sdtsdb_t db, sdtscid_t cid, uint64_t ts, uint64_t duration, double rate);
static void init_hi_data(void);
static void *sync_proc(void*p);
static void stopHandler(int sig);
static sdntime_t getBaseTime(void);

int checkDbExists(const char *dbdir) {
	struct stat st;
	if (stat(dbdir, &st) == 0 && S_ISDIR(st.st_mode)) {
		return 1;
	} else {
		return 0;
	}
}

int main(int argc, char *argv[])
{
	pthread_t thrid;
	sdntime_t ts;
	int db_exists;

	db_exists = checkDbExists(DBDIR);

	if (sd_init(NULL) < 0) {
		printf("error sd_init [%d]\n", sd_get_err());
		return 1;
	}
	printf("-- success sd_init --\n");

	if ((db = sdts_open_db("DB_PATH=" DBDIR)) == NULL) {
		printf("error sdts_open_db [%d]\n", sd_get_err());
		sd_end();
		return 1;
	}
	printf("-- success sdts_open_db --\n");

	if (db_exists) {
		col_params[0].cid = sdts_get_col(db, "ch_hi");
		col_params[1].cid = sdts_get_col(db, "ch_mi");
		col_params[2].cid = sdts_get_col(db, "ch_lo_fix");
		col_params[3].cid = sdts_get_col(db, "ch_lo_var");
	} else {
		for (int i=0; i < COUNT_OF_ELEMENTS(col_params); i++) {
			sdtscid_t cid;
			if ((cid = sdts_create_col(db, (sdid_t)col_params[i].name, (char *)col_params[i].param)) < 0) {
				printf("error sdts_create_col [%d]\n", sd_get_err());
				(void)sdts_close_db(db);
				sd_end();
				return 1;
			}
			col_params[i].cid = cid;
			printf("-- success sdts_create_col [%d] --\n", cid);
		}
	}


	// ^Cなどでキルされた場合に正常に終了する
	signal(SIGINT, stopHandler);
	signal(SIGTERM, stopHandler);

	// 同期スレッド起動
	running = true;
	if (pthread_create(&thrid, NULL, sync_proc, NULL)) {
		sd_end();
		exit(EXIT_FAILURE);
	}

	init_hi_data();

	// メインループ
	ts = getBaseTime();
	while (running) {
		insert_hi(db, col_params[0].cid, ts,              DURATION,   10000);
		insert_mi(db, col_params[1].cid, ts,              DURATION/2, 200);
		insert_mi(db, col_params[1].cid, ts+(DURATION/2), DURATION/2, 100);
		insert_lo(db, col_params[2].cid, ts,              DURATION,   10);
		insert_lo(db, col_params[3].cid, ts,              DURATION,   10);

		sleep(DURATION/ONE_SEC);
		ts += DURATION;
	}
	running = false;

	pthread_join(thrid, NULL);

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

static void init_hi_data()
{
	srandom(0);
	for (int i=0; i<HI_DATA_SIZE; i+=2) {
		hi_data[i+0] = i;
		hi_data[i+1] = random();
	}
}

static void insert_hi(sdtsdb_t db, sdtscid_t cid, uint64_t ts, uint64_t duration, double rate)
{
	int i;
	static int flag = 0;
	int count = duration * rate / ONE_SEC;

	for (i = 0; i < count; i+=HI_DATA_SIZE) {
		// 登録
		int size = (count-i<HI_DATA_SIZE)?count-1:HI_DATA_SIZE;
		if (sdts_insert(
					db, cid, (flag)?0:ts,
					(char *)&hi_data[0],
					size) != size) {
			printf("error sdts_insert hi [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			exit(EXIT_FAILURE);
		}

		// 初回以降、タイムスタンプは0を指定する。
		// Hi において 0以外である場合、データの連続性を取り消し、
		// 再スタートの意味がある。 データのリセットする。
		flag = 1;
	}
	//printf("-- success sdts_insert hi [%d] --\n", i);
}

static void insert_mi(sdtsdb_t db, sdtscid_t cid, uint64_t ts, uint64_t duration, double rate)
{
	int i;

	// サンプリングレートを 200Hzに変更
	if (sdts_set_smpl_rate(db, (sdid_t)"ch_mi", rate) < 0) {
		printf("error sdts_set_smpl_rate[%d]\n", sd_get_err());
		sdts_close_db(db);
		sd_end();
		exit(EXIT_FAILURE);
	}

	// durationの時間分だけ追加
	int count = duration * rate / ONE_SEC;
	for (i=0; i < count; i++) {
		// 登録
		if (sdts_insert(db, cid, ts, (char *)&i, 1) != 1) {
			printf("error sdts_insert mi [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			exit(EXIT_FAILURE);
		}
		ts = 0;
	}
	//printf("-- success sdts_insert mi [%d] --\n", i);
}

static void insert_lo(sdtsdb_t db, sdtscid_t cid, uint64_t ts, uint64_t duration, double rate)
{
	const uint64_t t = ONE_SEC / rate;
	const int count = duration * rate / ONE_SEC;
	int i;
	static int D = 0;

	for (i = 0; i < count; i++) {
		// 登録
		if (sdts_insert(db, cid, ts, (char *)&D, 1) != 1) {
			printf("error sdts_insert lo [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			exit(EXIT_FAILURE);
		}
		// タイムスタンプ設定  0を指定した場合システム時間が設定される。
		ts += t;
		D++;
	}
	//printf("-- success sdts_insert lo [%d] --\n", i);
}

static void *sync_proc(void*p)
{
	int counter = 0;
	while (running) {
		usleep(ONE_SEC/1000/3);
		if (counter%10 == 0) {
			fprintf(stderr, "\ntrying to sync..");
			if (sdts_sync_db(db, SDTS_SYNC_CMD_REG)) {
				fprintf(stderr, "error sdts_sync_db [%d]\n", sd_get_err());
			} else {
				fprintf(stderr, "sync success.\n");
			}
		}
		counter++;
	}
	return NULL;
}

static void stopHandler(int sig)
{
	running = 0;
}

static sdntime_t getBaseTime(void)
{
	time_t t;

	t = time(NULL);
	if (t < 0) {
		return BASE_TIME;
	} else {
		return t * ONE_SEC;
	}
}

