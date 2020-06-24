#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "speedbee.h"

/*
 * sample ユースケース
 * CPUの使用率を算出し登録
 * Window分析によって、一定時間ごとのMAX,MIN,AVGを算出する
 * CPUの使用率を30秒間計測してデータ登録しその後表示するサンプルプログラム
 * カラムタイプ MI データ登録 / カラムデータ取得
 */

// [カラム定義]
//   カラムタイプ Mi
//   初期サンプリングレート 1Hz
//   メモリ保存 30 件
//   データサイズ 8バイト
#define COL_MAIN_PAR	"COL_TYPE=M;DATA_SIZE=8;SMPL_RATE=1;SAVE_COUNT=30;DATA_TYPE=double;"
#define COL_SUB_PAR	"COL_TYPE=M;DATA_SIZE=8;SMPL_RATE=10;SAVE_COUNT=30;DATA_TYPE=double;"

#define REGISTER_TIME 30	/* サンプルデータの登録時間 10sec 以上を設定すること。 */

// バッファサイズ
#define BUF		16
typedef struct tag_cpu {
	char name[BUF];
	int total;
	int act;
	int usr;
	int nice;
	int sys;
	int idle;
	int wa;
	int irq;
	int s_irq;
	int steal;
	int guest;
	int g_nice;
} CPU;
typedef struct tag_cpu_time {
	char name[BUF];
	double total;
	double act;
	double usr;
	double nice;
	double sys;
	double idle;
} CPU_TIME;
typedef struct tag_cid_processor {
	const char *name;
	sdtsdb_t db;
	sdtscid_t act;
	sdtscid_t max;
	sdtscid_t min;
	sdtscid_t ave;
} CID_PROCESSOR;

// DB operation
static sdtsdb_t db_init();
static void db_finish(sdtsdb_t db);
static void db_create_column(sdtsdb_t db, CID_PROCESSOR *cids, const int proc);
static void db_aggregate_fixed_cycle(sdtsdb_t db, CID_PROCESSOR *cids, const int proc);
static int callback_func(sdntime_t ts, void *da, int dc, sdstat_t* st, int ret, void *user_data);
static void db_insert(sdtsdb_t db, CID_PROCESSOR *cids, const int proc, CPU_TIME register_cpu[REGISTER_TIME][proc]);
static void db_get_cpu_time(sdtsdb_t db, CID_PROCESSOR *cids, const int proc);
// CPU operation
static int get_count_logical_processor(void);
static int get_current_cpu_time(CPU time[]);
static void calc_cpu_time(CPU prev[], CPU curr[], int reg_curr_time, int register_time, int procnum, CPU_TIME register_cpu[register_time][procnum]);
static void cpu_start_usage_collecters(const int proc, CPU_TIME register_cpu[REGISTER_TIME][proc]);

int main(int ac, char *av[]) {
	sdtsdb_t db = db_init();
	const int processor = get_count_logical_processor();
	CID_PROCESSOR cids[processor];

	db_create_column(db, cids, processor);
	db_aggregate_fixed_cycle(db, cids, processor);

	// CPU使用率の計測と取得.
	CPU_TIME register_cpu[REGISTER_TIME][processor];	/* 登録用データ格納 1sec:CPU~CPUxx, REGISTER_TIME sec. */
	cpu_start_usage_collecters(processor, register_cpu);

	db_insert(db, cids, processor, register_cpu);
	
	db_get_cpu_time(db, cids, processor);

	db_finish(db);

	return 0;
}

// DB operation
static sdtsdb_t db_init() {
	sdtsdb_t db;

	if (sd_init(NULL) < 0) {
		printf("error sd_init [%d]\n", sd_get_err());
		exit(EXIT_FAILURE);
	}
	printf("-- success sd_init --\n");

	if ((db = sdts_open_db(NULL)) == NULL) {
		printf("error sdts_open_db [%d]\n", sd_get_err());
		sd_end();
		exit(EXIT_FAILURE);
	}
	printf("-- success sdts_open_db --\n");

	return db;
}
static void db_finish(sdtsdb_t db) {
	if (sdts_close_db(db) < 0) {
		printf("error sdts_close_db [%d]\n", sd_get_err());
		sd_end();
		exit(EXIT_FAILURE);
	}
	printf("-- success sdts_close_db --\n");

	if (sd_end() < 0) {
		printf("error sd_end [%d]\n", sd_get_err());
		exit(EXIT_FAILURE);
	}
	printf("-- success sd_end --\n");
}
static void db_create_column(sdtsdb_t db, CID_PROCESSOR *cids, const int proc){
	char cidbuff[256];
	for (int i = 0; i < proc; i++) {
		cids[i].db = db;
		cids[i].name = (char *)&i;
		// CPUの論理プロセッサとCPU全体の使用率を格納するカラムを作成.
		if ((cids[i].act = sdts_create_col(db, (sdid_t)&i, (char *)COL_MAIN_PAR)) < 0) {
			printf("error sdts_create_col act [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			exit(EXIT_FAILURE);
		}
		// printf("-- success created sdts_create_col cid act [%d] --\n", cids[i].act);
		
		// 10秒ごとのWindow分析で、最大・最小・平均を格納するためのカラムを作成.
		snprintf(cidbuff, 256, "%d_max", i);
		if ((cids[i].max = sdts_create_col(db, (sdid_t)&cidbuff, (char *)COL_SUB_PAR)) < 0) {
			printf("error sdts_create_col max [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			exit(EXIT_FAILURE);
		}
		// printf("-- success created sdts_create_col cid max [%d] --\n", cids[i].max);
		
		snprintf(cidbuff, 256, "%d_min", i);
		if ((cids[i].min = sdts_create_col(db, (sdid_t)&cidbuff, (char *)COL_SUB_PAR)) < 0) {
			printf("error sdts_create_col min [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			exit(EXIT_FAILURE);
		}
		// printf("-- success created sdts_create_col cid min [%d] --\n", cids[i].min);
		
		snprintf(cidbuff, 256, "%d_ave", i);
		if ((cids[i].ave = sdts_create_col(db, (sdid_t)&cidbuff, (char *)COL_SUB_PAR)) < 0) {
			printf("error sdts_create_col ave [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			exit(EXIT_FAILURE);
		}
		// printf("-- success created sdts_create_col cid ave [%d] --\n", cids[i].ave);
	}
	printf("-- success sdts_create_col --\n");
}
static void db_aggregate_fixed_cycle(sdtsdb_t db, CID_PROCESSOR *cids, const int proc) {
	for (int i = 0; i < proc; i++) {
		if (sdts_set_win(db, cids[i].act, "window", SD_WT_COUNT, "WIN_COUNT=10;WIN_STAT=true;", callback_func, &cids[i]) < 0) {
			printf("error sdts_set_win [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			exit(EXIT_FAILURE);
		}
	}
	printf("-- success sdts_set_win --\n");
}
static int callback_func(sdntime_t ts, void *da, int dc, sdstat_t* st, int ret, void *user_data) {
	CID_PROCESSOR *ana = (CID_PROCESSOR *)user_data;
	if (st) {
		/*printf("   sum[%f]sumsq[%f]min[%f]max[%f]mean[%f]\n \
			var[%f]stdev[%f]uvar[%f]ustdev[%f]stder[%f]\n  \
			st[%lld]ed[%lld]cnt[%d]\n", 
		st->sum,
		st->sumsq,	// sum of squares.
		st->min,
		st->max,
		st->mean,
		st->var,	// sample variance.
		st->stdev,	// standard deviation.
		st->uvar,	// unbiased variance.
		st->ustdev,	// unbiased standard deviation.
		st->stder,	// standard error.
		st->stime,	// window start time.
		st->etime,	// window end time.
		st->cnt);	// input count.*/

		if (sdts_insert(ana->db, ana->max, st->etime, (char *) &st->max, 1) != 1) {
			printf("error sdts_insert [%d] : sdts_insert error in sdts_set_win callback: name[%s] \n", sd_get_err(), ana->name);
			(void)sdts_close_db(ana->db);
			sd_end();
			exit(EXIT_FAILURE);
		}
		if (sdts_insert(ana->db, ana->min, st->etime, (char *) &st->min, 1) != 1) {
			printf("error sdts_insert [%d] : sdts_insert error in sdts_set_win callback: name[%s] \n", sd_get_err(), ana->name);
			(void)sdts_close_db(ana->db);
			sd_end();
			exit(EXIT_FAILURE);
		}
		if (sdts_insert(ana->db, ana->ave, st->etime, (char *) &st->mean, 1) != 1) {
			printf("error sdts_insert [%d] : sdts_insert error in sdts_set_win callback: name[%s] \n", sd_get_err(), ana->name);
			(void)sdts_close_db(ana->db);
			sd_end();
			exit(EXIT_FAILURE);
		}
	} else { 
		printf("\n");
	}

	return 0;
}
static void db_insert(sdtsdb_t db, CID_PROCESSOR *cids, const int proc, CPU_TIME register_cpu[REGISTER_TIME][proc]) {
	// CPU使用率をDBに登録.
	// タイムスタンプ設定 
	// 0を指定した場合システム時間が初期タイムスタンプとなる。
	sdntime_t ts = 1000000000;
	int i;
	for (i = 0; i < REGISTER_TIME; i++) {
		for (int j = 0; j < proc; j++) {
			// 登録
			// printf("INSERT Check [%d] %s: %.1f%%\n", i, register_cpu[i][j].name, register_cpu[i][j].act);
			if (sdts_insert(db, cids[j].act, ts, (char *) &register_cpu[i][j].act, 1) != 1) {
				printf("error sdts_insert [%d]\n", sd_get_err());
				(void)sdts_close_db(db);
				sd_end();
				exit(EXIT_FAILURE);
			}
		}
		// 初回以降、登録時のタイムスタンプが 0 である場合、
		// または、指定されたサンプリングレートでの間隔で
		// 初回からの連続したタイムスタンプある場合は 
		// 連続性があるデータとして認識される。
		ts = 0;
	}
	printf("-- success sdts_insert [%d] sec. input data count [%d]. --\n", i, REGISTER_TIME * proc);
}
static void db_get_cpu_time(sdtsdb_t db, CID_PROCESSOR *cids, const int proc) {
	// データの取得
	sdtscur_t cur;
	const int ccnt = proc * 4;
	int i = 0, j = 0, ret = 0;
	sdntime_t ts = 1000000000, st = 1000000000, iv = 1000000000;
	sdntime_t et = st * REGISTER_TIME;
	sdtscid_t all_cids[proc * 4];
	sdtscurval_t val[proc * 4];
	for (i = 0; i < proc; i++) {
		all_cids[j] = cids[i].act;
		all_cids[++j] = cids[i].max;
		all_cids[++j] = cids[i].min;
		all_cids[++j] = cids[i].ave;
		// printf("all_cids act:%d, max:%d, min:%d, ave:%d\n", all_cids[j-3], all_cids[j-2], all_cids[j-1], all_cids[j]);
		// printf("cids     act:%d, max:%d, min:%d, ave:%d\n", cids[i].act, cids[i].max, cids[i].min, cids[i].ave);
		++j;
	}
	printf("all_cids : %ld\n", sizeof all_cids / sizeof all_cids[0]);
	if ((cur = sdts_open_cur(db, all_cids, ccnt, st, et, iv, 0)) < 0) {
		printf("error sdts_open_cur [%d]\n", sd_get_err());
		exit(EXIT_FAILURE);
	}
	printf("-- success sdts_open_cur st[%lld] et[%lld] iv[%lld]--\n", st, et, iv);

	printf("\n|----|-----------------|-----------|-------|-------|-------|-------|\n");
	printf("| No | ts              | processor |   acr |   max |   min |   ave |\n");
	printf("|----|-----------------|-----------|-------|-------|-------|-------|");
	i = 0;
	while ((ret = sdts_fetch_cur(cur)) == SD_FETCH_OK) {
		if (sdts_get_cur_aggr(cur, &ts, val, ccnt) < 0) {
			printf("error sdts_get_cur_aggr [%d][%d]\n", i, sd_get_err());
			sdts_close_cur(cur);
			exit(EXIT_FAILURE);
		}
		// printf("|%04d| ts[%lld] |", i + 1, ts);
		for (j = 0; j < ccnt; j++) {
			// printf("c[%d]:", j + 1);
			if ((j == 0) || ((j > 3) && ((j % 4) == 0))) 
				printf("\n|%04d| ts[%11lld] |%7s[%d] |", i + 1, ts, (j < 4) ? "total" : "", (j < 4) ? 0 : j / 4 );

			if (val[j].ind > 0) {
				printf("%5.1f%% |", *(double *)val[j].vp);
				// printf("cid[%d]r[%lld]d[%.1f]s[%d]\n",
				// 	val[j].cid, val[j].ts, *(double *)val[j].vp, val[j].vsz);
			} else {
				printf("       |");
			}
		}
		i++;
	}
	printf("\n|----|-----------------|-----------|-------|-------|-------|-------|\n\n");

	if (ret == SD_FETCH_ERR) {
		printf("error sdts_fetch_cur [%d][%d]\n", i, sd_get_err());
		sdts_close_cur(cur);
		exit(EXIT_FAILURE);
	}
	if (sdts_close_cur(cur) < 0) {
		printf("error sdts_close_cur [%d][%d]\n", i, sd_get_err());
		exit(EXIT_FAILURE);
	}
	printf("-- success sdts_close_cur --\n");
}
// CPU operation
static int get_count_logical_processor(){
	FILE	*fp;
	const char	*info = "/proc/stat";
	const char *cpu = "cpu";
	int procCounter = 0;
	if ( (fp=fopen(info,"r")) ==NULL) {
		exit(EXIT_FAILURE);
	}
	char s[256];
	while (fgets(s, sizeof s, fp) != NULL) {
		if(strncmp(s, cpu, 3)!=0) break;
		++procCounter;
	}
	fclose(fp);
	return procCounter;
}
static int get_current_cpu_time(CPU time[]){
	FILE	*fp;
	const char	*info = "/proc/stat";
	const char *cpu = "cpu";
	if ( (fp=fopen(info,"r")) ==NULL) {
		return 1;
	}
	char s[256];
	int counter = 0;
	while (fgets(s, sizeof s, fp) != NULL) {
		/* fgets()により１行単位で読み出し */
		if(strncmp(s, cpu, 3)!=0) break;
		
		if(-1 == sscanf(s, "%s %d %d %d %d %d %d %d %d %d %d", 
			time[counter].name, &time[counter].usr, &time[counter].nice, &time[counter].sys, &time[counter].idle,
			&time[counter].wa, &time[counter].irq, &time[counter].s_irq, &time[counter].steal, &time[counter].guest, &time[counter].g_nice )){
			printf("error get_current_cpu_time\n");
		}
		time[counter].act = time[counter].usr + time[counter].nice + time[counter].sys;
		time[counter].total = time[counter].act + time[counter].idle;
		// printf("get_current_cpu_time : usr %d, nice %d, sys %d, idle %d, total %d, active %d\n", 
		// 	time[counter].usr, time[counter].nice, time[counter].sys, time[counter].idle, time[counter].total, time[counter].act);
		counter++;
	}

	fclose(fp);
	return 0;
}
static void calc_cpu_time(CPU prev[], CPU curr[], int reg_curr_time, int register_time, int procnum, CPU_TIME register_cpu[register_time][procnum]) {
	for (int i = 0; i < procnum; i++) {
		long total = (curr[i].usr - prev[i].usr) +
				(curr[i].nice - prev[i].nice) +
				(curr[i].sys - prev[i].sys) +
				(curr[i].idle - prev[i].idle);
		double rateTotal = (curr[i].total - prev[i].total) * 100 / (double) total;
		double rateAct = (curr[i].act - prev[i].act) * 100 / (double) total;
		double rateUser = (curr[i].usr - prev[i].usr) * 100 / (double) total;
		double rateNice = (curr[i].nice - prev[i].nice) * 100 / (double) total;
		double rateSys = (curr[i].sys - prev[i].sys) * 100 / (double) total;
		double rateIdle = (curr[i].idle - prev[i].idle) * 100 / (double) total;
		
		strcpy(register_cpu[reg_curr_time][i].name, curr[i].name);
		register_cpu[reg_curr_time][i].total = rateTotal;
		register_cpu[reg_curr_time][i].act = rateAct;
		register_cpu[reg_curr_time][i].usr = rateUser;
		register_cpu[reg_curr_time][i].nice = rateNice;
		register_cpu[reg_curr_time][i].sys = rateSys;
		register_cpu[reg_curr_time][i].idle = rateIdle;
		// printf("%s Total:%.1f%% Active:%.1f%% User:%.1f%%  Nice:%.1f%%  Sys:%.1f%%  Idle:%.1f%%\n",
		// 	register_cpu[reg_curr_time][i].name, rateTotal, rateAct, rateUser, rateNice, rateSys, rateIdle);
	}
}
static void cpu_start_usage_collecters(const int proc, CPU_TIME register_cpu[REGISTER_TIME][proc]) {
	const int sleepSec = 1;
	printf("cpu logical processor count: %d\n", proc);
	printf("Data acquisition time: %d sec\n", REGISTER_TIME);
	CPU prev_cpu_time[proc];
	CPU current_cpu_time[proc];
	for (int i = 0; i <= REGISTER_TIME; i++) {
		// /proc/statからCPUの時刻を取得する.
		get_current_cpu_time(current_cpu_time);
		
		if (i > 0) {
			// 初回の取得はCPUの計算用のため2回目の取得から計測.
			fprintf(stderr, "register count %d/%d\r", i, REGISTER_TIME);
			calc_cpu_time(prev_cpu_time, current_cpu_time, i -1, REGISTER_TIME, proc, register_cpu);
		}
		// Sleep 1 seconds.
		sleep(sleepSec);
		
		for (int j = 0; j<proc; j++) {
			// printf("cpu usage : %s\n", current_cpu_time[j].name);
			prev_cpu_time[j] = current_cpu_time[j];
		}
	}
	printf("\n-- success get cpu info  --\n");
}
