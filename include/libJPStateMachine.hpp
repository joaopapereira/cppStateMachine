/**
 *  Copyright 2012 Joao Pereira<joaopapereira@gmail.com>
 *
 *
 *  This file is part of libJPStateMachine.
 *
 *  libJPStateMachine is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libJPStateMachine is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libJPStateMachine.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef libJPStateMachine_H
#define libJPStateMachine_H

#include <map>
#include <string>
#include <libJPSemaphore.hpp>
#include <JPSMStates.hpp>
#include <JPSMEvents.hpp>
namespace jpCppLibs{

	/**
	 * Enumeration used to set the state
     * of the state machine
	 */
	enum SMStateMachineStatus{
		/**
		 * Machine not initialized
		 */
		MSM_UNINIT,
		/**
		 * Machine is running
		 */
		MSM_RUN,
		/**
		 * Last event changed the machine state
		 */
		MSM_CHGSTATE,
		/**
		 * Machine is stopped
		 */
		MSM_END
	};
	
	/**
	 * Structure used to store the list of events
	 * and the state that is reached when the event
	 * is preformed
	 */
	typedef std::map<SMEvent*,SMState*> MStateNextMap;
	/**
	 * Structure used to store the list of events that 
	 * will change a state in the machine
	 */
	typedef std::map<SMState*,MStateNextMap> MStateConnections;

	/**
	 * This class implements the state machine
	 * On it is possible to generate the state machine
	 */
	class JPStateMachine{
	public:
		/**
		 * Class Constructor that initialized the fist and last state
		 * of the machine
		 * @param init Pointer to the initial state
		 * @param end Pointer to the last state
		 */
		JPStateMachine( SMState * init, SMState * end );
		/**
		 * Function responsible for initializing the machine
		 */
		void initialize();
		/**
		 * Function that adds a new state to the machine
		 * @param state Pointer to the state object to be added
		 */
		void addState( SMState * state );
		/**
		 * Add a connection between to states that already exist in the machine
		 * @param actual Pointer to the start state of the connection
		 * @param ev Pointer to the event that will preform the state change
		 * @param nextState Pointer to the state that will be reached with the event ev
		 * @return Integer 0 in case of success
		 */
		int addConnection( SMState* actual, SMEvent *ev , SMState* nextState );
		/**
		 * Retrive the current state of the machine
		 * @return Pointer to the state
		 */
		const SMState* getCurrState();
		/**
		 * Execution of an external event.
		 * This function checks if the event changes the machine
		 * @param ev Pointer to the event to be evaluated
		 * @param changeState Boolean that allow the function only to check the 
		 *						event if changes the state(false) or changes the state
		 *						if possible(true)
		 * @return Integer 0 in case of success
		 */
		int externalEvent( SMEvent * ev , bool changeState = true  );
		/**
		 * Function that tries to move the machine into the next state
		 * The decision of the state to move to is retrieved from the result of the
		 * function of the state
		 * @param execArgs Arguments to be passed to the state function
		 */
		int nextState( MFunctionArgs* execArgs );

		/**
		 * Function to check if machine ended
		 * @return BOolean True if have ended
		 */
		bool machineEnded( );
		/**
		 * Check if machine is in idle state
		 * @return Boolean True if on idle state
		 * 		false if idle state not defined or not in
		 */
		bool onIdleState();
		/**
		 * Check if machine is in error state
		 * @return Boolean True if on error state
		 * 		false if error state not defined or not in
		 */
		bool onErrorState();
		/**
		 * Fcuntion used to understand if
		 * state changed in last machine iteration
		 * @return True if changed false in any other case
		 */
		bool machineChanged();
		/**
		 * Function rewinds the machine to previous
		 * state. This only works from error state
		 */
		void rewindMachine();
	protected:
		/**
		 * Function used to initialize the initial and end state of the machine
		 * @param init Pointer to the initial state
		 * @param end Pointer to the last state
		 */
		void addInitEnd( SMState * init, SMState * end );
		/**
		 * Contructor that only initializes the log name
		 * Function used by class the inherit from this class
		 * @param logName String with the log module
		 */
		JPStateMachine( std::string logName = "STM");

		/**
		 * Set the machine state of Idle
		 * @param idle Pointer to the machine state idle
		 */
		void setIdleState( SMState * idle );
		/**
		 * Set the machine state of error
		 * @param idle Pointer to the machine state error
		 */
		void setErrorState( SMState * error );
	private:
		/**
		 * Check if machine is in error state
		 * @return Boolean True if on error state
		 * 		false if error state not defined or not in
		 */
		bool int_onErrorState();
		/**
		 * Function to check if machine ended
		 * @return BOolean True if have ended
		 */
		bool int_machineEnded( );
		/**
		 * List of connections of the machine
		 */
		MStateConnections connections;
		/**
		 * Actual status of the machine
		 */
		SMStateMachineStatus status;
		/**
		 * Pointer to the actual state of the machine
		 */
		SMState * actState;
		/**
		 * Pointer to the initial state of the machine
		 */
		SMState * initial;
		/**
		 * Pointer to the last state of the machine
		 */
		SMState * last;
		/**
		 * Previous state
		 */
		SMEvent * prevEvent;
		/**
		 * Idle state
		 */
		SMState * idle;
		/**
		 * Error state
		 */
		SMState * error;
		/**
		 * Variable used to understand if
		 * state changed in last machine iteration
		 */
		bool stateChanged;
		/**
		 * Function that preform the check and updates the state of the machine
		 * @param ev Pointer to the event to be evaluated
		 * @param changeState Boolean that allow the function only to check the 
		 *						event if changes the state(false) or changes the state
		 *						if possible(true)
		 */
		SMState * evCheck( SMEvent * ev , bool changeState);
		/**
		 * Name of the module to be used by the logger
		 */
		std::string moduleName;
		/**
		 * Pointer to a semaphore used to synchronize access to the machine
		 */
		JPBinSemaphore * sem;
	};

#endif
