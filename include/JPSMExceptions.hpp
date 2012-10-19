#ifndef __JPSMEXCEPTIONS_HPP__
#define __JPSMEXCEPTIONS_HPP__

namespace jpCppLibs{
	/**
	 * Class the implements the exceptions of the logger
	 */
	class StateMachineExceptions: public std::exception{
	public:
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		StateMachineExceptions(bool showErrno = false) throw();
		/**
		 * Constructor used when no message needs to be passed
		 * @param error Error message
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		StateMachineExceptions( const char* error,bool showErrno = false ) throw();
		/**
		 * Constructor used when no message needs to be passed
		 * @param error Error message
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		StateMachineExceptions(std::string error, bool showErrno = false) throw();
		/**
		 * Function to change error message
		 * @param error Error message
		 */
		void setMsg(const char * error) throw();
		/**
		 * Retrive error reason
		 * @return String with the error
		 */
		const char * what() const throw();
		/**
		 * Class destructor
		 */
		~StateMachineExceptions() throw();
	private:
		/**
		 * Error Message
		 */
		std::string myerror;
		/**
		 * Indicates if errno should be shown or not
		 */
		bool showErrno;
	};
	/**
	 * Class that implements the exception for end of state machine
	 */
	class SMExpMachineEnd: public StateMachineExceptions{
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		SMExpMachineEnd(bool showErrno = false) throw():StateMachineExceptions("State machine reached end state",showErrno){};
	};
	/**
	 * Class that implements the exception for end of state machine
	 */
	class SMExpNullLogger: public StateMachineExceptions{
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		SMExpNullLogger(bool showErrno = false) throw():StateMachineExceptions("The logger was not initialized",showErrno){};
	};
	/**
	 * Class that implements the exception for end of state machine
	 */
	class SMExpNullState: public StateMachineExceptions{
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		SMExpNullState(bool showErrno = false) throw():StateMachineExceptions("State is NULL",showErrno){};
	};
	/**
	 * Class that implements the exception for end of state machine
	 */
	class SMExpNullEvent: public StateMachineExceptions{
		/**
		 * Constructor used when no message needs to be passed
		 * @param showErrno Indicates if the ERRNO should be or not written
		 */
		SMExpNullEvent(bool showErrno = false) throw():StateMachineExceptions("Event is NULL",showErrno){};
	};
};
