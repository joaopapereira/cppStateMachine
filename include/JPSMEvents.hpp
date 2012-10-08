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
#ifndef libJPSMEvents_H
#define libJPSMEvents_H
#include <string>

namespace jpCppLibs{
	/**
	 * Class that store one Event that will produce
	 * a state change in the State Machine
	 */
	class SMEvent{
	public:
		/**
		 * Event constructor
		 * Initialized the Event identifier
		 * @param id Identifier of the event
		 */
		SMEvent( std::string id );
		/**
		 * Retrieve the identifier of the Event
		 * @return String with the Identifier
		 */
		const std::string& getId();
	private:
		/**
		 * Variable that stores the Identifier of the event
		 */
		std::string id;
	};
};
#endif
