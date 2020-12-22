
#include "BackgroundWorker.h"

BackgroundWorker::BackgroundWorker(BackgroundWorker_DoWork doWork)
{
	_cancelAtomic = false;
	_doWork = doWork;
	_worker = nullptr;
}

BackgroundWorker::~BackgroundWorker()
{
	delete(this->_worker);
}

void BackgroundWorker::Start(void *arg)
{
	if (_worker != nullptr)
		delete _worker;

	_arg= arg;

	_worker = new std::thread(&BackgroundWorker::WorkerProc, this);
	_worker->detach();
}

bool BackgroundWorker::CancellationPending()
{
	bool ret = _cancelAtomic;
	return ret;
}

void BackgroundWorker::Stop()
{
	_cancelAtomic = true;
}

bool BackgroundWorker::IsWorking()
{
	bool ret = _workingAtomic;
	return ret;
}

void BackgroundWorker::WorkerProc()
{
	_workingAtomic = true;
	_doWork(this, _arg);
	_workingAtomic = false;
}