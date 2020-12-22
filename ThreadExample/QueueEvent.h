#ifndef QUEUEEVENT
#define QUEUEEVENT

//! Base event class.
/*!
	Base event class; other events must be derived from this class.
*/
class QueueEvent
{
public:
	/*!
		\brief Default Constructor.
	*/
	QueueEvent() {}

	/*!
		\brief Default desctructor.
	*/
	virtual ~QueueEvent() {}
};

#endif