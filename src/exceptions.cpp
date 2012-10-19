
#include "JPSMExceptions.hpp"
#include <string.h>
#include <errno.h>

using namespace jpCppLibs;
/**
 * CLASS: StateMachineExceptions
 * BEGIN {
 */
/**
 * Private methods
 */
/**
 * Protected methods
 */
/**
 * Public methods
 */
/**
 * Constructor used when no message needs to be passed
 * @param showErrno Indicates if the ERRNO should be or not written
 */
StateMachineExceptions::StateMachineExceptions( const char* error, bool showErrno ) throw(){
	this->showErrno = showErrno;
	setMsg(error);

}
/**
 * Constructor used when no message needs to be passed
 * @param error Error message
 * @param showErrno Indicates if the ERRNO should be or not written
 */
StateMachineExceptions::StateMachineExceptions(std::string error, bool showErrno ) throw(){
	setMsg(error.c_str());
	this->showErrno = showErrno;
}
StateMachineExceptions::StateMachineExceptions( bool showErrno ) throw(){
	setMsg("Error happened on logger file");
	this->showErrno = showErrno;
}
void
StateMachineExceptions::setMsg(const char * error) throw(){
	myerror = error;
	if( showErrno && ( errno != 0 ) ){
		myerror += "[" ;
		myerror += strerror(errno);
		myerror += "]";
	}
}
StateMachineExceptions::~StateMachineExceptions() throw(){
}

const char *
StateMachineExceptions::what() const throw(){
	return myerror.c_str();
}
/**
 * END }
 */



