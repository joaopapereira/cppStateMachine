#include "JPSMStates.hpp"

using namespace jpCppLibs;
/**
 * CLASS: SMFunctionArgs
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
 * Constructor of the function arguments
 */
SMFunctionArgs::SMFunctionArgs(){
	errorno = 0;

}
/**
 * } End
 */
/**
 * CLASS: SMState
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
 * Constructor of the state
 * Assigns the value of the identifier and function pointer
 * @param id Identifier of the State
 * @param fun Pointer to the function to be executed
 */
SMState::SMState( std::string id, SMEvent *(*fun)(SMFunctionArgs* )  ){
	setState( id , fun );
}
/**
 * Constructor of the state
 * Assigns the value of the identifier
 * @param id Identifier of the State
 */
SMState::SMState( std::string id ){
		this->id = id;
		execfun = NULL;

}
/**
 * Execute the function that is stored in the State
 * @param args Arguments to pass to the function
 * @return Pointer to the event that to feed into the State Machine
 */
SMEvent*
SMState::exec( SMFunctionArgs* args ){
	if( NULL == execfun )
		return NULL;

	return execfun( args );
}
/**
 * Function used to change the Identifier and pointer to the function
 * of the state
 * @param id Identifier of the State
 * @param fun Pointer to the function to be executed
 * @return 0 in case of success
 */
int
SMState::setState( std::string id, SMEvent *(*fun)(SMFunctionArgs* ) ){
	this->id = id;
	this->execfun = fun;

	return 0;
}
/**
 * Function used to compare the passed state with this one
 * @param state Pointer to the function to compare with
 * @return Boolean result, True if is the same state
 *                         False if the state is not the same
 */
bool
SMState::cmp( SMState * state ){
	if( 0 == id.compare( state->getId() ) )
		return true;
	else
		return false;
}
/**
 * Retrieve the identifier of the State
 * @return String with the identifier of the State
 */
const std::string&
SMState::getId(){
	return (const std::string)id;
}
/**
 * } End
 */
