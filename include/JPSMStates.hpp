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
#ifndef libJPSMStates_H
#define libJPSMStates_H
#include <JPSMEvents.hpp>

namespace jpCppLibs{
	/**
	 * Class that need to be overwritten in order to the machine
	 * to be able to execute a function during a state.
	 * This function is responsible for storing the arguments that
	 * need to be passed to the inner State function.
	 */
	class MFunctionArgs{
	public:
		/**
		 * Error Number storage
		 */
		int errorno;
	protected:
		/**
		 * Constructor of the function arguments
		 */
		MFunctionArgs();
	};

	/**
	 * This class represent a state inside the State Machine
	 */
	class SMState{
	private:
		/**
		 * Identifier of the State
		 */
		std::string id;
		/**
		 * Pointer to the function that can be executed inside the state
		 */
		SMEvent *(*execfun)( MFunctionArgs* );
	public:
		/**
		 * Constructor of the state
		 * Assigns the value of the identifier and function pointer
		 * @param id Identifier of the State
		 * @param fun Pointer to the function to be executed
		 */
		SMState( std::string id, SMEvent *(*fun)(MFunctionArgs* )  );
		/**
		 * Constructor of the state
		 * Assigns the value of the identifier
		 * @param id Identifier of the State
		 */
		SMState( std::string id );
		/**
		 * Execute the function that is stored in the State
		 * @param args Arguments to pass to the function
		 * @return Pointer to the event that to feed into the State Machine
		 */
		SMEvent* exec( MFunctionArgs* args );
		/**
		 * Function used to change the Identifier and pointer to the function
		 * of the state
		 * @param id Identifier of the State
		 * @param fun Pointer to the function to be executed
		 * @return 0 in case of success
		 */
		int setState( std::string id, SMEvent *(*fun)(MFunctionArgs* ) );
		/**
		 * Function used to compare the passed state with this one
		 * @param state Pointer to the function to compare with
		 * @return Boolean result, True if is the same state
		 *                         False if the state is not the same
		 */
		bool cmp( SMState * state );
		/**
		 * Retrieve the identifier of the State
		 * @return String with the identifier of the State
		 */
		const std::string& getId();
	};
};
#endif
