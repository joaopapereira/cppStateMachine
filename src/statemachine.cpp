#include "libJPStateMachine.hpp"

using namespace jpCppLibs;
/**
 * Class Constructor that initialized the fist and last state
 * of the machine
 * @param logger Pointer to the logger to be used in the state machine
 * @param init Pointer to the initial state
 * @param end Pointer to the last state
 */
JPStateMachine::JPStateMachine(Logger * logger, SMState * init, SMState * end ){
	if( NULL == logger)
		throw SMExpNullLogger();

	this->logger = logger;
	moduleName = "STM";
	if( NULL == init ){
		logger->log(moduleName,M_LOG_MAX,M_LOG_ERR,"Initial state pointer is NULL!");
		throw SMExpNullState();
	}
	if( NULL == end ){
		logger->log(moduleName,M_LOG_MAX,M_LOG_ERR,"End state pointer is NULL!");
		throw SMExpNullState();
	}

	status = MSM_UNINIT;
	initial = init;
	last = end;
	actState = NULL;
	prevEvent = NULL;
	idle = NULL;
	error = NULL;
	stateChanged = false;
	sem = new JPBinSemaphore();
	sem->down();
}
/**
 * Contructor that only initializes the log name
 * Function used by class the inherit from this class
 * @param logger Pointer to the logger to be used in the state machine
 * @param logName String with the log module
 */
JPStateMachine::JPStateMachine( Logger * logger, std::string logName ){
	if( NULL == logger)
			throw SMExpNullLogger();
	this->logger = logger;
	moduleName = logName;
	status = MSM_UNINIT;
	initial = NULL;
	last = NULL;
	actState = NULL;
	prevEvent = NULL;
	idle = NULL;
	error = NULL;
	stateChanged = false;
	sem = new JPBinSemaphore();
	sem->down();
}

/**
 * Function used to initialize the initial and end state of the machine
 * @param init Pointer to the initial state
 * @param end Pointer to the last state
 */
void
JPStateMachine::addInitEnd( SMState * init, SMState * end ){
	logger->log(moduleName,M_LOG_LOW,M_LOG_TRC,
								  "addInitEnd( %p, %p )", init , end );
	if( NULL == init ){
		logger->log(moduleName,M_LOG_MAX,M_LOG_ERR,"Initial state pointer is NULL!");
		throw SMExpNullState();
	}
	if( NULL == end ){
		logger->log(moduleName,M_LOG_MAX,M_LOG_ERR,"End state pointer is NULL!");
		throw SMExpNullState();
	}
	initial = init;
	last = end;
}

/**
 * Function responsible for initializing the machine
 */
void
JPStateMachine::initialize(){
	logger->log(moduleName,M_LOG_LOW,M_LOG_TRC,
							  "initialize( )");
	status = MSM_RUN;
	actState = initial;
	sem->up();
}

/**
 * Retrive the current state of the machine
 * @return Pointer to the state
 */
const SMState*
JPStateMachine::getCurrState(){
	logger->log(moduleName,M_LOG_LOW,M_LOG_TRC,
			                      "getCurrState()");
	return (const SMState *) actState;
}

/**
 * Execution of an external event.
 * This function checks if the event changes the machine
 * @param ev Pointer to the event to be evaluated
 * @param changeState Boolean that allow the function only to check the
 *						event if changes the state(false) or changes the state
 *						if possible(true)
 * @return Integer 0 in case of success
 */
int
JPStateMachine::externalEvent( SMEvent * ev , bool changeState ){
	logger->log(moduleName,M_LOG_LOW,M_LOG_TRC,
			                      "externalEvent( %p )", ev );
	if( NULL == ev ){
		logger->log(moduleName,M_LOG_MAX,M_LOG_ERR,"Event being evaluated is NULL!");
		throw SMExpNullEvent();
	}
	sem->down();
	if( MSM_RUN == status )
		actState = evCheck( ev, changeState );
	if( actState->cmp( last ) ){
		sem->up();
		return MSM_END;
	}
	sem->up();
	return status;
}
/**
 * Function to check if machine ended
 * @return Boolean True if have ended
 */
bool
JPStateMachine::machineEnded( ){
	logger->log(moduleName,M_LOG_LOW,M_LOG_TRC,
			                      "machineEnded()" );
	bool result;
	sem->down();
	result = int_machineEnded();
	sem->up();
	return result;
}
/**
 * Function to check if machine ended
 * @return Bolean True if have ended
 */
bool
JPStateMachine::int_machineEnded( ){
	logger->log(moduleName,M_LOG_LOW,M_LOG_TRC,
			                      "machineEnded()" );
	if( MSM_RUN == status )
		return false;
	if( actState->cmp( last ) )
		return true;
	return false;
}

/**
 * Add a connection between to states that already exist in the machine
 * @param actual Pointer to the start state of the connection
 * @param ev Pointer to the event that will preform the state change
 * @param nextState Pointer to the state that will be reached with the event ev
 * @return Integer 0 in case of success
 */
int
JPStateMachine::addConnection( SMState* actual ,SMEvent *ev , SMState* nextState ){
	if( NULL == actual ){
		logger->log(moduleName,M_LOG_MAX,M_LOG_ERR,"Connection starting state pointer is NULL!");
		throw SMExpNullState();
	}
	if( NULL == ev ){
		logger->log(moduleName,M_LOG_MAX,M_LOG_ERR,"Connection event changer pointer is NULL!");
		throw SMExpNullEvent();
	}
	if( NULL == nextState ){
		logger->log(moduleName,M_LOG_MAX,M_LOG_ERR,"Connection end state pointer is NULL!");
		throw SMExpNullState();
	}
	logger->log(moduleName,M_LOG_LOW,M_LOG_TRC,
		                      "addConnection( %s , %s , %s )",
				      actual->getId().c_str(),
				      ev->getId().c_str(),
				      nextState->getId().c_str());
	connections[ actual ][ ev ] = nextState;

	return 0;

}

/**
 * Function that preform the check and updates the state of the machine
 * @param ev Pointer to the event to be evaluated
 * @param changeState Boolean that allow the function only to check the
 *						event if changes the state(false) or changes the state
 *						if possible(true)
 */
SMState *
JPStateMachine::evCheck( SMEvent *ev , bool changeState ){
	logger->log(moduleName,M_LOG_LOW,M_LOG_TRC,"evCheck( %s )",ev->getId().c_str());
	if( NULL == ev ){
		logger->log(moduleName,M_LOG_MAX,M_LOG_ERR,"Event pointer is NULL!");
		throw SMExpNullEvent();
	}
	SMState * aux;
	MStateNextMap aux1;
	if( connections.end() == connections.find(actState) ){
		logger->log(moduleName,M_LOG_NRM,M_LOG_DBG,"State does not have connect, Final?");
		return actState;
	}
	aux1 = connections.find(actState)->second;
	if( aux1.end() == aux1.find( ev ) ){
		logger->log(moduleName,M_LOG_NRM,M_LOG_DBG,"Event(%s) do not change state(%s)",ev->getId().c_str(),actState->getId().c_str());
		return actState;
	}

	aux = aux1.find( ev )->second;

	if( actState->cmp( aux ) ){
		logger->log(moduleName,M_LOG_NRM,M_LOG_DBG,"Jumping to itself");
		return actState;
	}
	logger->log(moduleName,M_LOG_NRM,M_LOG_DBG,"State Change %s->%s",actState->getId().c_str(),aux->getId().c_str());
	if( changeState )
		actState = aux;
	return aux;
}

/**
 * Function that tries to move the machine into the next state
 * The decision of the state to move to is retrieved from the result of the
 * function of the state
 * @param execArgs Arguments to be passed to the state function
 */
int
JPStateMachine::nextState( SMFunctionArgs* execArgs ){
	logger->log(moduleName,M_LOG_LOW,M_LOG_TRC,"nextState( %p )", execArgs );
	logger->log(moduleName,M_LOG_NRM,M_LOG_DBG,"Current State is: %s", actState->getId().c_str() );
	sem->down();
	logger->log(moduleName,M_LOG_LOW,M_LOG_DBG,"Going to execute function" );
	SMEvent * evt;
	try{
		evt = actState->exec( execArgs );
	}catch( SMExpMachineEnd &e ){
		sem->up();
		throw e;
	}
	if( NULL == evt ){
		logger->log(moduleName,M_LOG_NRM,M_LOG_WRN,"Error executing state function on [%s]", actState->getId().c_str() );
		sem->up();
		return -1;
	}

	logger->log(moduleName,M_LOG_NRM,M_LOG_DBG,"Event[%s] on state[%s]",evt->getId().c_str(), actState->getId().c_str() );
	SMState * next = evCheck( evt , false );
	if( actState->cmp( next ) ){
		logger->log(moduleName,M_LOG_NRM,M_LOG_DBG,"Event[%s] on state[%s] did not change state",evt->getId().c_str(), actState->getId().c_str() );
		stateChanged = false;
	}else{
		logger->log(moduleName,M_LOG_NRM,M_LOG_DBG,"Event[%s] on state[%s] change state to state[%s]",evt->getId().c_str(), actState->getId().c_str(), next->getId().c_str() );
		stateChanged = true;
		actState = next;
	}
	if( int_onErrorState() )
		prevEvent = evt;
	if( actState->cmp( last ) ){
		sem->up();
		return MSM_END;
	}
	sem->up();
	return 0;
}

/**
 * Set the machine state of Idle
 * @param idle Pointer to the machine state idle
 */
void
JPStateMachine::setIdleState( SMState * idle ){
	if( NULL == idle ){
		logger->log(moduleName,M_LOG_MAX,M_LOG_ERR,"Idle state pointer is NULL!");
		throw SMExpNullState();
	}
	this->idle = idle;
}
/**
 * Set the machine state of error
 * @param idle Pointer to the machine state error
 */
void
JPStateMachine::setErrorState( SMState * error ){
	if( NULL == idle ){
		logger->log(moduleName,M_LOG_MAX,M_LOG_ERR,"Error state pointer is NULL!");
		throw SMExpNullState();
	}
	this->error = error;
}
/**
 * Check if machine is in idle state
 * @return Boolean True if on idle state
 * 		false if idle state not defined or not in
 */
bool
JPStateMachine::onIdleState(){
	bool result = false;
	sem->down();
	if( NULL != idle )
		result = false;

	if( result && actState->cmp(idle) )
		result = true;
	else
		result = false;
	sem->up();
	return result;
}
/**
 * Check if machine is in error state
 * @return Boolean True if on error state
 * 		false if error state not defined or not in
 */
bool
JPStateMachine::onErrorState(){
	bool result;
	sem->down();
	result = int_onErrorState();
	sem->up();
	return result;
}
/**
 * Check if machine is in error state
 * @return Boolean True if on error state
 * 		false if error state not defined or not in
 */
bool
JPStateMachine::int_onErrorState(){
	if( NULL == error )
		return false;
	if( actState->cmp(error) )
		return true;
	else
		return false;
}

/**
 * Fcuntion used to understand if
 * state changed in last machine iteration
 * @return True if changed false in any other case
 */
bool
JPStateMachine::machineChanged(){
	return stateChanged;
}

/**
 * Function rewinds the machine to previous
 * state. This only works from error state
 */
void
JPStateMachine::rewindMachine(){
	if( !onErrorState() )
		return; // do nothing
	sem->down();
	SMState * next = evCheck( prevEvent , false );
	if( actState->cmp( next ) ){
		logger->log(moduleName,M_LOG_NRM,M_LOG_DBG,"Event[%s] on state[%s] did not change state",prevEvent->getId().c_str(), actState->getId().c_str() );
		stateChanged = false;
	}else{
		logger->log(moduleName,M_LOG_NRM,M_LOG_DBG,"Event[%s] on state[%s] change state to state[%s]",prevEvent->getId().c_str(), actState->getId().c_str(), next->getId().c_str() );
		stateChanged = true;
		actState = next;
	}

	sem->up();
}
