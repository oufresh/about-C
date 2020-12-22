// ThreadExample.cpp : Defines the entry point for the console application.
//

#include <iostream>       // std::cout
#include <thread>         // std::thread, std::thread::id, std::this_thread::get_id
#include <chrono>         // std::chrono::seconds
#include <string>
#include <mutex>
#include <condition_variable>

#include "BackgroundWorker.h"
#include "ThreadEventQueue.h"

inline void wait_on_enter()
{
    std::string dummy;
    std::cout << "Enter to continue..." << std::endl;
	fflush(stdin);
    std::getline(std::cin, dummy);
}

void ThreadId()
{
	std::thread::id thread_id;
	thread_id = std::this_thread::get_id();
	printf("Thread id = %u\n", thread_id);
}

/////////////////////////////////////////////////////////////////////////////////
class NWorkerThread
{
	std::mutex				g_lock;
	std::condition_variable g_cond;
	bool					_exit;
	int						_count;
	bool					_exit_th;
	std::thread				*th;

	int Wait()
	{
		int ret = 0;
		std::unique_lock<std::mutex> lk(g_lock);
		while(_count == 0)
			g_cond.wait(lk);
		ret = _count;
		_count--;
		if (_count == 0 && _exit == true)
			_exit_th = true;
		return ret;
	}

	void Run()
	{
		int local_count = 0;
		printf("Run() -> Enter\n");
		ThreadId();

		while (!_exit_th)
		{
			local_count = Wait();

			printf("local_count = %d\n", local_count);
			 
			printf("WrokerThread performing ops ");
			for (int i = 0; i < 10; i++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				printf(".");
			}
			printf(" Done!\n");
		}

		printf("Run() -> Bye\n");
	}

public:
	NWorkerThread() 
	{
		_exit = false;
		_exit_th = false;
		_count = 0;
		th = new std::thread(&NWorkerThread::Run, this);
	}

	~NWorkerThread()
	{
		delete th;
	}

	void Notify()
	{
		std::unique_lock<std::mutex> lk(g_lock);
		_count++;
		g_cond.notify_one();
	}

	void Stop()
	{
		{
			std::unique_lock<std::mutex> lk(g_lock);
			_count++;
			_exit = true;
			g_cond.notify_one();
		}
		th->join();
	}


};

void NWorkerThreadTest()
{
	bool quit = false;
	NWorkerThread wk;

	while (!quit)
	{
		int dummy;
		printf("Notifica ->1\nStop -> 0\n");
		scanf("%d", &dummy);

		if (dummy == 1)
			wk.Notify();
		else if (dummy == 0)
		{
			wk.Stop();
			break;
		}
		else
		{}
	}
	
	printf("Fine Test NWorkerThread\n");
	wait_on_enter();	
}

//////////////////////////////////////////////////////////////////////////////////

class EWorkerThread
{
	std::mutex				g_lock;
	std::condition_variable g_cond;
	bool					_exit;
	bool					_notified;
	std::thread				*th;

	void Wait()
	{
		std::unique_lock<std::mutex> lk(g_lock);
		while(!_notified)
			g_cond.wait(lk);
		_notified = false;
	}

	void Run()
	{
		int  n = 1;
		printf("Run() -> Enter\n");
		ThreadId();

		while (!_exit)
		{
			Wait();

			printf("Notified - n = %d\n", n++);
			 
			printf("WrokerThread performing ops ");
			for (int i = 0; i < 10; i++)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(500));
				printf(".");
			}
			printf("Done!\n");
		}

		printf("Run() -> Bye\n");
	}

public:
	EWorkerThread() 
	{
		_exit = false;
		_notified = false;
		th = new std::thread(&EWorkerThread::Run, this);
	}

	~EWorkerThread()
	{
		delete th;
	}

	void Notify()
	{
		std::unique_lock<std::mutex> lk(g_lock);
		_notified = true;
		g_cond.notify_one();
	}

	void Stop()
	{
		{
			std::unique_lock<std::mutex> lk(g_lock);
			_notified = true;
			_exit = true;
			g_cond.notify_one();
		}
		th->join();
	}
};

void SimpleThread()
{
	EWorkerThread wk;
	wk.Notify();
	std::this_thread::sleep_for(std::chrono::seconds(5));
	wk.Notify();
	std::this_thread::sleep_for(std::chrono::seconds(5));
	wk.Notify();
	std::this_thread::sleep_for(std::chrono::seconds(5));
	wk.Stop();
	
	wait_on_enter();
	printf("ok\n");
}
///////////////////////////////////////////////////////////////////////////////////////////////////













///////////////////////////////////////////////////////////////////////////////////////////////////

class DoSomethigEvent : public QueueEvent
{
public:
	int value;
};

class  ThreadEventExample : public ThreadEventQueue
{
public:
	ThreadEventExample(std::string name);
	~ThreadEventExample();

	void DoSomething(int value);

private:
	int _value;

	void Run() override;
};

ThreadEventExample::ThreadEventExample(std::string name) : ThreadEventQueue(name)
{
	_value = 0;
}

 ThreadEventExample::~ThreadEventExample()
{
}

 void ThreadEventExample::DoSomething(int value)
 {
	 std::shared_ptr<DoSomethigEvent> ev = std::shared_ptr<DoSomethigEvent>(new DoSomethigEvent());
	 ev->value = value;

	 this->PushEvent(ev);
 }

 void ThreadEventExample::Run()
 {
	bool go = true;

	printf("ThreadEventExample::Run()\n");

	while (go == true)
	{
		std::shared_ptr<QueueEvent> ev = WaitEvent();

		if (std::shared_ptr<DoSomethigEvent> dsev = std::dynamic_pointer_cast<DoSomethigEvent>(ev))
		{
			
			printf("Evento DoSomethigEvent, value passed %d\n", dsev->value);
			printf("Doing ");
			for (int i = 0; i < 10; i++)
			{
				printf(".");
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
			printf("\n");
			_value += dsev->value;
			printf("Done, value total %d\n", _value);
		}
		else if (std::shared_ptr<StopEvent> stev = std::dynamic_pointer_cast<StopEvent>(ev))
		{
			printf("Stop event ricevuto\n");
			go = false;
		}
		else
		{ 
			printf("Event sconosciuto\n");
		}
	}

	printf("ThreadEventExample::Exit()\n");
 }






///////////////////////////////////////////////////////////////////////////////////////////////////
void Work(BackgroundWorker *bg, void *arg)
{
	printf("--- Enter BackgroundWorker\n");

	for (int i = 0; i < 15; i++)
	{
		printf("--- Start Work\n");

		if (bg->CancellationPending() == true)
		{
			printf("--- CancellationPending true -> exit\n");
			break;
		}
	
		printf("--- Doing Work ...\n");
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	printf("--- End BackgroundWorker\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int Choice()
{
	int input;

	printf("---------------------\n");
	printf("Scegli un esempio:\n");
	printf("0 - Quit\n");
	printf("1 - Simple Thread\n");
	printf("2 - BackgroundWorker\n");
	printf("3 - ThreadEventQueue\n");
	printf("4 - NWorkerThread\n");

	scanf("%d", &input);
	printf("---------------------\n");

	return input;
}

int main(int argc, char* argv[])
{
	bool quit = false;

	std::thread::id main_thread_id;
	main_thread_id = std::this_thread::get_id();
	std::cout << "Main thread up, id = " <<  main_thread_id << std::endl;

	while(!quit)
	{
		int choice = Choice();

		switch (choice)
		{
		case 0:
			quit = true;
			break;
		case 1:
			SimpleThread();
			break;
		case 2:
			{
				BackgroundWorker bg(Work);
				bg.Start(nullptr);

				std::this_thread::sleep_for(std::chrono::seconds(1));

				bool working = bg.IsWorking();
				printf("BackgroundWorker working %u\n", working);

				std::this_thread::sleep_for(std::chrono::seconds(1));

				bg.Stop();

				wait_on_enter();
			}
			break;
		case 3:
			{
				ThreadEventExample tev("Event thread");
				tev.Start();
				std::this_thread::sleep_for(std::chrono::seconds(2));
				tev.DoSomething(11);
				std::this_thread::sleep_for(std::chrono::seconds(2));
				tev.Stop();
			}
			break;
		case 4:
			NWorkerThreadTest();
			break;
		default:
			printf("Input errato!\n");
			break;
		}
	}

	return 0;
}

