#include "JPSMEvents.hpp"

using namespace jpCppLibs;

/**
 * CLASS: SMEvent
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
 * Event constructor
 * Initialized the Event identifier
 * @param id Identifier of the event
 */
SMEvent::SMEvent( std::string id ){
	this->id = id;

}
/**
 * Retrieve the identifier of the Event
 * @return String with the Identifier
 */
const std::string&
SMEvent::getId(){
	return (const std::string ) id;
}
/**
 * } End
 */
