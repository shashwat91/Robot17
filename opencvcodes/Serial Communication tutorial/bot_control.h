#ifndef BOT_CONTROL_H
#define BOT_CONTROL_H

#include "tserial.h"
class serial {

  private:
        // private attributes
		Tserial *com;



  public:
	serial() {
	}

	bool startDevice(char *port, int speed) {
		com = new Tserial();
		if (com!=0) {
			if(!com->connect(port, speed, spNONE)) {
				printf("Connected...\n");
				return TRUE;
			}
			else {
				printf("Not connected..\n");
				return FALSE;
			}
		}
		else
			return FALSE;
	}

	void stopDevice() {
		com->disconnect();
        delete com;
        com = 0;
	}

	void sendData(unsigned char data) {
		com->sendChar(char(data));
	}

	char getData() {
		return com->getChar();
	}
};

#endif // BOT_CONTROL_H
