#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "speedbee.h"

/*
 * sample ユースケース
 * CPUの使用率を算出し登録
 * Window分析によって、一定時間ごとのMAXを算出する
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

#define REGISTER_TIME 10	/* サンプルデータの登録時間 10sec 以上を設定すること。 */
#define SLEEP_INTERVAL (1)

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
} CPU;
typedef struct tag_cid_processor {
	const char *name;
	sdtsdb_t db;
	sdtscid_t act;
	sdtscid_t max;
} CID_PROCESSOR;

// DB operation
static void db_create_column(sdtsdb_t db, CID_PROCESSOR *cids, const int proc);
static void db_aggregate_fixed_cycle(sdtsdb_t db, CID_PROCESSOR *cid);
static int callback_func(sdntime_t ts, void *da, int dc, sdstat_t* st, int ret, void *user_data);
static void db_insert(sdtsdb_t db, CID_PROCESSOR *cid, sdntime_t ts, const int proc, const double register_cpu);
static void show_cpu_time(sdtsdb_t db, CID_PROCESSOR *cids, const int proc, sdntime_t ts);
static void db_print_col_info(sdtsdb_t db, sdtscid_t cid, char *m, bool check);
// VIEW
static void view_table_header(const int view_proc_num);
static void view_table_footer(const int view_proc_num);
static void view_table_record(const int ccnt, const int proc,  const int i, const sdntime_t ts, const int col_type_num, const int view_proc_num, sdtscurval_t *val);
static void ts2str(uint64_t ts, char *buff, size_t size);
// CPU operation
static int get_count_logical_processor(void);
static int get_current_cpu_time(CPU time[]);
static double calc_cpu_time(const CPU prev, const CPU curr);
static void collect_cpu_load_data(sdtsdb_t db, CID_PROCESSOR *cids, const int proc, sdntime_t ts);

int main(int ac, char *av[]) {
	sdtsdb_t db;
	struct timespec current_ts;
	timespec_get(&current_ts, TIME_UTC);
	sdntime_t curr_ts = current_ts.tv_sec * 1000000000 + current_ts.tv_nsec;
	char date[64];
	strftime(date, sizeof(date), "%Y-%m-%d %a %H:%M:%S", localtime(&current_ts.tv_sec));
	// printf("%ld.%09ld\n", current_ts.tv_sec, current_ts.tv_nsec);
	printf("START : %s.%09ld\n", date, current_ts.tv_nsec);

	if (sd_init(NULL) < 0) {
		fprintf(stderr, "error sd_init [%d]\n", sd_get_err());
		exit(EXIT_FAILURE);
	}

	if ((db = sdts_open_db(NULL)) == NULL) {
		fprintf(stderr, "error sdts_open_db [%d]\n", sd_get_err());
		sd_end();
		exit(EXIT_FAILURE);
	}

	const int processor = get_count_logical_processor();
	CID_PROCESSOR cids[processor];

	db_create_column(db, cids, processor);

	// CPU使用率の計測と取得.
	collect_cpu_load_data(db, cids, processor, curr_ts);

	show_cpu_time(db, cids, processor, curr_ts);

	if (sdts_close_db(db) < 0) {
		fprintf(stderr, "error sdts_close_db [%d]\n", sd_get_err());
		sd_end();
		exit(EXIT_FAILURE);
	}

	if (sd_end() < 0) {
		fprintf(stderr, "error sd_end [%d]\n", sd_get_err());
		exit(EXIT_FAILURE);
	}

	timespec_get(&current_ts, TIME_UTC);
	strftime(date, sizeof(date), "%Y-%m-%d %a %H:%M:%S", localtime(&current_ts.tv_sec));
	printf("END : %s.%09ld\n", date, current_ts.tv_nsec);

	return 0;
}

// DB operation
static void db_create_column(sdtsdb_t db, CID_PROCESSOR *cids, const int proc){
	char cidbuff[256];
	for (int i = 0; i < proc; i++) {
		cids[i].db = db;
		cids[i].name = (char *)&i;
		// CPUの論理プロセッサとCPU全体の使用率を格納するカラムを作成.
		if ((cids[i].act = sdts_create_col(db, (sdid_t)&i, (char *)COL_MAIN_PAR)) < 0) {
			fprintf(stderr, "error sdts_create_col act [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			exit(EXIT_FAILURE);
		}
		
		// 10秒ごとのWindow分析で、最大・最小・平均を格納するためのカラムを作成.
		snprintf(cidbuff, 256, "%d_max", i);
		if ((cids[i].max = sdts_create_col(db, (sdid_t)&cidbuff, (char *)COL_SUB_PAR)) < 0) {
			fprintf(stderr, "error sdts_create_col max [%d]\n", sd_get_err());
			(void)sdts_close_db(db);
			sd_end();
			exit(EXIT_FAILURE);
		}

		db_aggregate_fixed_cycle(db, &cids[i]);
	}
}
static void db_aggregate_fixed_cycle(sdtsdb_t db, CID_PROCESSOR *cid) {
	if (sdts_set_win(db, cid->act, "window", SD_WT_COUNT, "WIN_COUNT=10;WIN_STAT=true;", callback_func, cid) < 0) {
		fprintf(stderr, "error sdts_set_win [%d]\n", sd_get_err());
		(void)sdts_close_db(db);
		sd_end();
		exit(EXIT_FAILURE);
	}
}
static int callback_func(sdntime_t ts, void *da, int dc, sdstat_t* st, int ret, void *user_data) {
	CID_PROCESSOR *proc = (CID_PROCESSOR *)user_data;
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

		if (sdts_insert(proc->db, proc->max, st->etime, (char *) &st->max, 1) != 1) {
			fprintf(stderr, "error sdts_insert [%d] : sdts_insert error in sdts_set_win callback: name[%s] \n", sd_get_err(), proc->name);
			(void)sdts_close_db(proc->db);
			sd_end();
			exit(EXIT_FAILURE);
		}
	} else { 
		printf("\n");
	}

	return 0;
}
static void db_insert(sdtsdb_t db, CID_PROCESSOR *cid, sdntime_t ts, const int proc, const double register_cpu) {
	// CPU使用率をDBに登録.
	// タイムスタンプ設定 
	// 0を指定した場合システム時間が初期タイムスタンプとなる。
	if (sdts_insert(db, cid->act, ts, (char *) &register_cpu, 1) != 1) {
		fprintf(stderr, "error sdts_insert [%d]\n", sd_get_err());
		(void)sdts_close_db(db);
		sd_end();
		exit(EXIT_FAILURE);
	}
}
static void show_cpu_time(sdtsdb_t db, CID_PROCESSOR *cids, const int proc, sdntime_t st) {
	// データの取得
	sdtscur_t cur;
	const int col_type_num = 2;
	const int ccnt = proc * col_type_num;
	int ret = 0;
	const sdntime_t iv = 1000000000;
	sdntime_t et = st + iv * REGISTER_TIME;
	sdtscid_t all_cids[proc * col_type_num];
	sdtscurval_t val[proc * col_type_num];

	for (int i = 0; i < proc; i++) {
		db_print_col_info(db, cids[i].act, "check act", false);// bool = true/false. true:colum info print.
		db_print_col_info(db, cids[i].max, "check max", false);
		all_cids[2*i] = cids[i].act;
		all_cids[2*i+1] = cids[i].max;
	}

	printf("all_cids : %ld\n", sizeof all_cids / sizeof all_cids[0]);
	if ((cur = sdts_open_cur(db, all_cids, ccnt, st, et, iv, SDTS_CUR_OPT_AGGR_TS_PREV|SDTS_CUR_OPT_EXTEND)) < 0) {
		fprintf(stderr, "error sdts_open_cur [%d]\n", sd_get_err());
		exit(EXIT_FAILURE);
	}

	const int view_proc_num = 4;
	view_table_header(view_proc_num);

	int cnt = 0;
	while ((ret = sdts_fetch_cur(cur)) == SD_FETCH_OK) {
		sdntime_t ts;

		if (sdts_get_cur_aggr(cur, &ts, val, ccnt) < 0) {
			fprintf(stderr, "error sdts_get_cur_aggr [%d][%d]\n", cnt, sd_get_err());
			sdts_close_cur(cur);
			exit(EXIT_FAILURE);
		}
		view_table_record(ccnt, proc, cnt, ts, col_type_num, view_proc_num, val);
		cnt++;
	}
	view_table_footer(view_proc_num);

	if (ret == SD_FETCH_ERR) {
		fprintf(stderr, "error sdts_fetch_cur [%d][%d]\n", cnt, sd_get_err());
		sdts_close_cur(cur);
		exit(EXIT_FAILURE);
	}
	if (sdts_close_cur(cur) < 0) {
		fprintf(stderr, "error sdts_close_cur [%d][%d]\n", cnt, sd_get_err());
		exit(EXIT_FAILURE);
	}
}
static void db_print_col_info(sdtsdb_t db, sdtscid_t cid, char *m, bool check) {
	if(!check) return;

	sdtscolinfo_t *ip;

	if ((ip = sdts_get_col_info(db, cid)) == NULL) {
		fprintf(stderr, "error sdts_get_col_info [%d]\n", sd_get_err());
		exit(EXIT_FAILURE);
	}

	printf(" *** %s : column info [%s] ***\n", m, (char *)ip->cname);
	printf("ctype : %d\n", ip->ctype);
	printf("dsz   : %d\n", ip->dsz);
	printf("rsz   : %d\n", ip->rsz);
	printf("scnt  : %d\n", ip->scnt);
	printf("icnt  : %lld\n", ip->icnt);

	printf("hmsmpl : %g\n", ip->hmsmpl);
	printf("hmst   : %lld\n", ip->hmst);

	printf("msst  : %lld\n", ip->msst);
	printf("mset  : %lld\n", ip->mset);
	printf("mscnt : %lld\n", ip->mscnt);

	sdts_free_col_info(ip);
}
// VIEW
static void view_table_header(const int view_proc_num){
	int i;
	printf("\n|----|----------------------------|");
	for (i=0; i < view_proc_num; i++) printf("---------------|");
	printf("\n| No | ts                         |");
	for (i=0; i < view_proc_num; i++) i == 0 ? printf(" Total         |") : printf(" processor(%d)  |", i);
	printf("\n|    |                            |");
	for (i=0; i < view_proc_num; i++) printf("---------------|");
	printf("\n|    |                            |");
	for (i=0; i < view_proc_num; i++) printf("   acr |   max |");
	printf("\n|----|----------------------------|");
	for (i=0; i < view_proc_num; i++) printf("---------------|");
}
static void view_table_footer(const int view_proc_num){
	printf("\n|----|----------------------------|");
	for (int i=0; i < view_proc_num; i++) printf("---------------|");
	printf("\n\n");
}
static void view_table_record(const int ccnt, const int proc,  const int i, const sdntime_t ts, const int col_type_num, const int view_proc_num, sdtscurval_t *val){
	for (int j = 0; j < ccnt; j++) {
		if ((j / col_type_num) >= view_proc_num) continue; // 表示しないプロセッサはスキップ

		if (j == 0) {
			// 表の「No」と「ts」
			char buff[64];
			ts2str(ts, buff, sizeof(buff));
			printf("\n|%04d|ts[%11s]|", i + 1, buff);
		} 

		// プロッサの「acr」と「max」
		if (val[j].ind > 0) {
			printf("%5.1f%% |", *(double *)val[j].vp);
		} else {
			printf("       |");
		}
	}
}
static void ts2str(uint64_t ts, char *buff, size_t size){
	struct tm *cal;

	time_t t = ts / (1000*1000*1000);
	cal = localtime(&t);

	snprintf(buff, size, "%02d-%02d-%02dT%02d:%02d:%02d.%06ld",
		(cal->tm_year+1900) % 100,
		cal->tm_mon+1,
		cal->tm_mday,
		cal->tm_hour,
		cal->tm_min,
		cal->tm_sec,
		(ts/1000) % 1000000);
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
		
		if(-1 == sscanf(s, "%s %d %d %d %d ", 
			time[counter].name, &time[counter].usr, &time[counter].nice, &time[counter].sys, &time[counter].idle )){
			fprintf(stderr, "error get_current_cpu_time\n");
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
static double calc_cpu_time(const CPU prev, const CPU curr) {
		long total = (curr.usr - prev.usr) +
				(curr.nice - prev.nice) +
				(curr.sys - prev.sys) +
				(curr.idle - prev.idle);
		double rateAct = (curr.act - prev.act) * 100 / (double) total;
		// printf("Active:%.1f%%\n", rateAct);
		return (double)rateAct;
}
static void collect_cpu_load_data(sdtsdb_t db, CID_PROCESSOR *cids, const int proc, sdntime_t ts) {
	CPU prev_cpu_time[proc];
	CPU current_cpu_time[proc];
	int i, j;
	int insert_count = 0, input_record = 0;

	printf("cpu logical processor count: %d\n", proc);
	printf("Data acquisition time: %d sec\n", REGISTER_TIME);

	for (i = 0; i <= REGISTER_TIME; i++) {
		// /proc/statからCPUの時刻を取得する.
		get_current_cpu_time(current_cpu_time);
		
		if (i > 0) {
			// 初回の取得はCPUの計算用のため2回目の取得から計測.
			printf("register count %d/%d\r", i, REGISTER_TIME);
			fflush(stdout);

			for (j = 0; j<proc; j++) {
				double register_cpu;

				register_cpu = calc_cpu_time(prev_cpu_time[j], current_cpu_time[j]);
				db_insert(db, &cids[j], ts, proc, register_cpu);
				insert_count++;
			}
			input_record++;
			// 初回以降、登録時のタイムスタンプが 0 である場合、
			// または、指定されたサンプリングレートでの間隔で
			// 初回からの連続したタイムスタンプある場合は 
			// 連続性があるデータとして認識される。
			ts = 0;
		}
		memcpy(prev_cpu_time, current_cpu_time, sizeof(current_cpu_time));
		// Sleep 1 seconds.
		sleep(SLEEP_INTERVAL);
	}
}
