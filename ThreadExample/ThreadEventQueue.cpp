
#include "ThreadEventQueue.h"

ThreadEventQueue::ThreadEventQueue()
{
	//_thId = 0;
	_name = "";
}

ThreadEventQueue::ThreadEventQueue(std::string name)
{
	//_thId = 0;
	_name = name;
}

ThreadEventQueue::~ThreadEventQueue()
{
	delete(_th);
}

std::string ThreadEventQueue::GetName()
{
	return _name;
}

std::thread::id ThreadEventQueue::GetThreadId()
{
	return _thId;
}

void ThreadEventQueue::PushEvent(std::shared_ptr<QueueEvent> ev)
{
	std::unique_lock<std::mutex> lk(this->_lkCond);
	this->_queue.push(ev);
	this->_cond.notify_one();
}

void ThreadEventQueue::PushEventAgain(std::shared_ptr<QueueEvent> ev)
{
	std::unique_lock<std::mutex> lk(this->_lkCond);
	this->_queue.push(ev);
	this->_cond.notify_one();
}

std::shared_ptr<QueueEvent> ThreadEventQueue::WaitEvent()
{
	//devo dormire sull'evento coda vuota!!
	std::shared_ptr<QueueEvent> ev = nullptr;

	std::unique_lock<std::mutex> lk(this->_lkCond);

	while (_queue.empty() == true)
		this->_cond.wait(lk);
	ev = _queue.front();
	_queue.pop();

	return ev;
}

std::shared_ptr<QueueEvent> ThreadEventQueue::WaitEvent(int timeout)
{
	//devo dormire sull'evento coda vuota!!
	std::shared_ptr<QueueEvent> ev = nullptr;

	std::unique_lock<std::mutex> lk(this->_lkCond);

	while (_queue.empty() == true)
		this->_cond.wait_for(lk, std::chrono::microseconds(timeout));
	ev = _queue.front();
	_queue.pop();

	return ev;
}

void ThreadEventQueue::Start()
{
	_th = new std::thread(&ThreadEventQueue::Run, this);
	_thId = _th->get_id();//.hash();
	//_th->detach();
}

void ThreadEventQueue::Stop()
{
	std::shared_ptr<StopEvent> stop = std::shared_ptr<StopEvent>(new StopEvent());
	PushEvent(stop);
	_th->join();
}
