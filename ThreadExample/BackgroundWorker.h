#ifndef BACKGROUNDWORKER
#define BACKGROUNDWORKER

#include <thread>
#include <string>
#include <atomic>

/*!
	Forward declaration.
*/
class BackgroundWorker;

/*!
	\brief	Definition of the BackgroundWorker Thread method function.
	\param	bg Instance of BackgroundWorker passed as context.
	\param	arg User parameter passed to the thread method.
*/
typedef void (*BackgroundWorker_DoWork)(BackgroundWorker *bg, void *arg);

/*!
	\brief	Implements a thread that runs in background; it is a detached thread.

			Portable implemetation of a thread that runs in background; a function 
			with prototype define by BackgroundWorker_DoWork must be defined to perform
			the operations.
			In this function you may test (typically inside a loop) with the method CancellationPending 
			if the Stop has been called.

			Example:

			void Work(BackgroundWorker *bg, void *arg)
			{
				for (int i = 0; i < NUM; i++)
				{
					if (bg->CancellationPending() == true)
					{
						break;
					}

					// Do some work
					// ...
				}
			}

			...

			BackgroundWorker bg(Work);
			bg.Start(nullptr);

			...


			bg.Stop()
*/
class BackgroundWorker
{
private:

	std::atomic<bool>	_cancelAtomic; /*! Cancel atomic flag */
	std::atomic<bool>	_workingAtomic; /*! Working atomic flag */

	std::thread *		_worker; /*! Library std::thread */

	/*! 
		\brief	Internal worker thread procedure. 
	*/
	void WorkerProc();

protected:
	
	/*! 
		\brief	Pointer to the thread worker function 
	*/
	BackgroundWorker_DoWork _doWork; 

	/*! 
		\brief	Pointer to argument passed to the thread 
	*/
	void *					_arg; 

public:

	/*! 
		\brief	Contructor
		\param	doWork Function with the worker business logic.
	*/
	BackgroundWorker(BackgroundWorker_DoWork doWork); 

	/*!
		\brief Destructor
	*/
	~BackgroundWorker();

	/*!
		\brief	Check if a cancellation of the BackgroundWorker has been raised.
	*/
	bool CancellationPending();

	/*!
		\brief		Start the BackgroundWorker thread passing to the function the param.
		\param arg	Parameter passed to the thread function.
	*/
	void Start(void *arg);

	/*!
		\brief	Flag the backgroundworker to be stopped, test in the thread function with CancellationPending.
	*/
	void Stop();

	/*!
		\brief	Check if the thread is working.
	*/
	bool IsWorking();
};

#endif