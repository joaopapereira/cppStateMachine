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


	enum SMStateMachineStatus{
		MSM_UNINIT,
		MSM_RUN,
		MSM_CHGSTATE,
		MSM_END
	};
	
	typedef std::map<SMEvent*,SMState*> MStateNextMap;
	typedef std::map<SMState*,MStateNextMap> MStateConnections;

	class MStateMachine{
	public:
		MStateMachine( SMState * init, SMState * end );
		void initialize();
		void addState( SMState * state );
		int addConnection( SMState* actual, SMEvent *ev , SMState* nextState );
		const SMState* getCurrState();
		int externalEvent( SMEvent * ev , bool changeState = true  );
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
		void addInitEnd( SMState * init, SMState * end );
		MStateMachine( std::string logName = "STM");

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
		MStateConnections connections;
		SMStateMachineStatus status;
		SMState * actState;
		SMState * initial;
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
		SMState * evCheck( SMEvent * ev , bool changeState);
		std::string moduleName;
		JPBinSemaphore * sem;
	};

#endif
