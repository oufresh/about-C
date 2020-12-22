#ifndef THREADEVENTQUEUE
#define THREADEVENTQUEUE

#include <thread>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "QueueEvent.h"

/*!
	\mainpage	Thread portable class.
*/

/*!
	\brief	Default Stop Thread Event: use this event to catch the Stop and exit from Run() method.
*/
class StopEvent : public QueueEvent
{
public:
	//! Stop Event constructor.
	StopEvent() {}
	//! Stop Event destructor.
	virtual ~StopEvent() {}
};
/*!
	\brief	Base class to implement a portable thread with a blocking queue of events; must be derived to 
			define a specif logic.

			This class is the base to implement a portable thread that process async operation operations;
			the thread is raised by an event pushed into the queue.
		
			Every Event is a derived class from the QueueEvent base class and may contains custom values and
			attributes depending on the business logic. Every event is managed by a std::shared_ptr, you must not
			call the delete method but only use shared_ptr to keep the object alive as long as it's needed.

			The Run() method must be defined and implements the main thread loop: it should call the WaitEvent 
			method to wait from an event and should use the std::dynamic_pointer_cast<...> to identify the type of
			the event and execute the appropriate logic.

			Example of thread loop:

			while(go)
			{
				std::shared_ptr<QueueEvent> ev = WaitEvent();

				if (std::shared_ptr<DoSomethigEvent> dsev = std::dynamic_pointer_cast<DoSomethigEvent>(ev))
				{
					// ....
				}
				else if (std::shared_ptr<StopEvent> stev = std::dynamic_pointer_cast<StopEvent>(ev))
				{
					go = false;
				}
				else
				{
					//unknown
				}
			}

			StopEvent must be caught, it is pushed by the Stop method.
*/
class ThreadEventQueue
{
private:
	std::thread::id							_thId; /*! Thread id from std library */
	std::string								_name; /*! Thread name passed to class constructor*/

	std::condition_variable					_cond; /*! Condition variable to make the thread sleep in case of empty queue */
	std::mutex								_lkCond; /*! Mutex to syncronized the acces to the queue */
	std::queue<std::shared_ptr<QueueEvent>>	_queue; /*! Library std::queue used to stored the events */

	std::thread								*_th; /*! Library std::thread class to create the thraed */

protected:

	/*! 
		\brief Thread implementation method, must be overridden to define the thread logic and catch events pushed 
		       from other threads.
	*/
	virtual void Run() = 0;

	/*!
		\brief Wait for an event in the queue; blocks the thread if the queue is empty.
	*/
	std::shared_ptr<QueueEvent> WaitEvent();

	/*!
		\brief	Wait for an event in the queue; blocks the thread if the queue is empty for the 
				timeout and then returns even if the queue is empty.
		\param	timeout milliseconds of sleeping on the queue empy.
	*/
	std::shared_ptr<QueueEvent> WaitEvent(int timeout);

    /*!
		\brief	Push an event in the queue and raise the thread if it's sleeping.
		\param	ev std Shared pointer of an event to process.
    */
	void PushEvent(std::shared_ptr<QueueEvent> ev);

	/*!
	  \brief	Push again an event in the queue, used if the event is not the one we're waiting for but 
				we don't want to loose this one.
      \param	ev std Shared pointer of an event to process again.
    */
	void PushEventAgain(std::shared_ptr<QueueEvent> ev);

public:
	/*!
		\brief Default constructor
	*/
	ThreadEventQueue();

	/*!
		\brief Constructor with a thread name associated.
		\param name Thread name.
	*/
	ThreadEventQueue(std::string name);

	/*!
		\brief Destructor.
	*/
	virtual ~ThreadEventQueue();

	/*!
		\brief Returns the id of the thread.
	*/
	std::thread::id GetThreadId();

	/*!
		\brief Return the thread name if passed in the constructor.
	*/
	std::string GetName();

	/*!
		\brief Start the thread define by the Run method.
	*/
	void Start();

	/*!
		\brief Push a StopEvent in the queue and raise the thread if it's sleeping, must be caugth in
		       the thread Run loop to quit the thread.
	*/
	virtual void Stop();
};

#endif