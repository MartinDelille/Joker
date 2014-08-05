/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDIOUTPUT_H
#define PHMIDIOUTPUT_H

#include "PhMidiObject.h"

/**
 * @brief The PhMidiOutput class
 */
class PhMidiOutput : public PhMidiObject
{
	Q_OBJECT
public:
	PhMidiOutput();
	~PhMidiOutput();

	/**
	 * @brief Open an existing midi port.
	 * @param portName The midi port name
	 * @return True if success, false otherwise.
	 */
	bool open(QString portName);

	/**
	 * @brief close Close the midi port if opened
	 */
	void close();

	/**
	 * @brief Send a MTC quarter frame message
	 * @param data The data1 byte containing the MTC data.
	 */
	void sendMTC(unsigned char data);

private:
	RtMidiOut *_midiOut;
};

#endif // PHMIDIOUTPUT_H
