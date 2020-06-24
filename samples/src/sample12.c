#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include "speedbee.h"
#include "sdts_spcf.h"

/*
 * sample 12 永続化データの参照（MI）
 */

/* 固定値  */
#define BSZ			4096
#define DBDIR "sample11-12db.sdts"
#define ONE_SEC    (1000000000LL)
const sdntime_t BASE_TIME = 1589896352381000000LL;   // 2020-05-19T13:52:32.381Z

static sdntime_t getBaseTime(void);

/* 静的グローバル変数  */
static sdtsdb_t db;
static struct { const char *name; const char *param; sdtscid_t cid; } col_params[] = {
  { "ch_hi", "--", 0 },
  { "ch_mi", "--", 0 },
  { "ch_lo", "--", 0 },
};

/* static関数プロトタイプ  */

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
	sdntime_t ts;
	int db_exists;

	db_exists = checkDbExists(DBDIR);
	if (!db_exists) {
		fprintf(stderr, "database not found.\n");
		exit(EXIT_FAILURE);
	}

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

	col_params[0].cid = sdts_get_col(db, "ch_hi");
	col_params[1].cid = sdts_get_col(db, "ch_mi");
	col_params[2].cid = sdts_get_col(db, "ch_lo");

	/* データ参照  */
	dat_nxt_t nxt;
	ts = getBaseTime() - (40*60*ONE_SEC);
	int cnt = 0;
	while (1)
	{
		int outc;
		dat_hdr_t hdr;
		int buff[BSZ];

		int ret = spcf_get_col_dat(db, col_params[1].cid, SDTS_STO_OPT_MEM|SDTS_STO_OPT_FILE, ts, &nxt, &hdr, (uint8_t*)buff, BSZ, &outc);
		if (ret == SPCF_ERROR) {
			printf("SPCF_ERROR [%d]\n" , sd_get_err()); break;
		} else if (ret == SPCF_ERR_OUT_RAGE) {
			printf("SPCF_ERR_OUT_RAGE\n"); break;
		} else {
			printf("rate=%f, ts=%ld\n", hdr.smpl, hdr.ts);
			// SPCF_SUCCESS SPCF_MISS_DATA SPCF_END_MEM SPCF_END_RATE
			int *p = &buff[0];
			for (int i = 0; i < outc; i++)
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

