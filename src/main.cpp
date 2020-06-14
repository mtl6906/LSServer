#include "Recver.h"
#include "Sender.h"
#include "Dispatcher.h"
#include "Worker.h"
#include "Config.h"
#include "thread"
#include "queue"
#include "vector"
#include "unistd.h"
#include "Context.h"
#include "Session.h"
#include "utility"
#include "signal.h"

using std::thread;
using std::vector;
using std::queue;
 
using ls::HttpCore;
using ls::Recver;
using ls::Sender;
using ls::Dispatcher;
using ls::Worker;

ls::Config config;
HttpCore core;
Recver recver(&core);
Sender sender(&core);
Dispatcher dispatcher;
vector<Worker> workers(cc.thread_number);

long long worker_all_time;
long long sender_all_time;
long long rd_all_time;
long long locked_all_time;
long long r1, r2;
void* WorkerThread(Worker *worker)
{
	long long start_time, end_time;
	for(;;)
	{
		start_time = GetCurrentTime();
		worker -> Run();
		end_time = GetCurrentTime();
		worker_all_time += end_time - start_time;
		usleep(cc.worker_waiting_time * 1000);
	}
}

void* SenderThread(Sender *sender)
{
	long long start_time, end_time;
	queue<ls::Session *> wsq;
	for(;;)
	{
		start_time = GetCurrentTime();
		for(int i=0;i<workers.size();++i)
		{
			workers[i].GetOLC().PopAll(wsq);
			sender -> Run(wsq);
		}
		end_time = GetCurrentTime();
		sender_all_time += end_time - start_time;
		usleep(cc.sender_waiting_time * 1000);
	}
}

void handler_pipe(int sig)
{

}

#ifdef DEBUG
void input()
{
	for(;;)
	{
		getchar();
		printf("pool: %lld\n", core.pool.size());
		printf("sender(%lld), worker(%lld), rd(%lld), lock_all_time(%lld), r1(%lld)\n", sender_all_time, worker_all_time, rd_all_time, locked_all_time, r1);
	}
}
#endif

int main(int argc, char **argv)
{
	struct sigaction sa;
	sa.sa_handler = handler_pipe;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, NULL);

	core.Create(cc.max_connection_number);
	recver.Init(cc.port);
	queue<ls::Session *> rdq;
	queue<ls::Context> dwq;

	vector<thread> worker_thread_group(cc.thread_number);
	int i = 0;
	for(auto &worker_thread : worker_thread_group)
	{
		worker_thread = std::move(thread(WorkerThread, &workers[i]));
		++i;
		ls_log_tag(MAIN_LOG, "worker %d create ok...\n", worker_thread.get_id());
		worker_thread.detach();
	}
	thread sender_thread(SenderThread, &sender);
	ls_log_tag(MAIN_LOG, "sender %d create ok...\n", sender_thread.get_id());
	sender_thread.detach();
#ifdef DEBUG
	thread input_thread(input);
	input_thread.detach();
#endif	
	for(;;)
	{
		long long all_time;
		long long start_time, end_time;
		start_time = GetCurrentTime();
		recver.Run(rdq);
		end_time = GetCurrentTime();
		rd_all_time += end_time - start_time;
		dispatcher.Run(rdq, dwq);
		start_time = GetCurrentTime();
		int task_per_thread = dwq.size() / workers.size();
		task_per_thread = task_per_thread > 0 ? task_per_thread : 1;
		for(int i=0;i<workers.size();++i)
		{
			if(dwq.empty())
				break;
			workers[i].GetILC().Push(dwq, task_per_thread);
		}
		end_time = GetCurrentTime();
		locked_all_time += end_time - start_time;
	}

	return 0;
}
