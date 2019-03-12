#ifndef API_H_
#define API_H_

#include "Declaration.h"
#include "CatalogManager.h"
#include "interpreter.h"
#include "RecordManager.h"
#include "BufferManager.h"

class API {
private:
	bool finish;
public:
	CatalogManager Cm;
	string inputline;
	API(void) :
			finish(false) {
	}
	;
	~API(void) {
	}
	;

	bool Exec_File(string File_Name);

	void dealString();



	bool Check_Finish();

}
;

#endif /* API_H_ */
