#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <papi.h>

using namespace std;

void initPapi() {
  int retval = PAPI_library_init(PAPI_VER_CURRENT); 
  if (retval != PAPI_VER_CURRENT) {
    fprintf(stderr, "PAPI library init error!\n");
    exit(1);
  }    
}


void addAndStartCounterForInstruction(int eventSet, int instruction) {
  /* Create the eventset */
  if (PAPI_create_eventset(&eventSet) != PAPI_OK) {
    fprintf(stderr, "PAPI_create_eventset error!\n");
    exit(1);
  }
  /* Add instructions to our eventset */
  if (PAPI_add_event(eventSet, instruction) != PAPI_OK){
    fprintf(stderr, "PAPI_add_event error!\n");
    exit(1);
  }
  /* Start counting events in the eventset */
  if (PAPI_start(eventSet) != PAPI_OK){
    fprintf(stderr, "PAPI_start error!\n");
    exit(1);
  }
}

void readCounter(int eventSet, long_long* value) {
  /* Read the counting events in the eventset */
  if (PAPI_read(eventSet, value) != PAPI_OK) {
    fprintf(stderr, "PAPI_read error!\n");
    exit(1);
  }
}


int IJK() {
  return 42;
}

int main () {

  int EventSet=PAPI_NULL;
  long_long values[1]; 

  initPapi();
  addAndStartCounterForInstruction(EventSet, PAPI_LST_INS);

  /* Defined in tests/do_loops.c in the PAPI source distribution */
  //Start process
  cout << "IJK" << endl;
  IJK();

  readCounter(EventSet, values);

  cout << values[0] << endl;

  // ------------------------------------------------------------------
  
  addAndStartCounterForInstruction(EventSet, PAPI_LST_INS);

  /* Defined in tests/do_loops.c in the PAPI source distribution */
  //Start process
  cout << "IJK" << endl;
  IJK();

  readCounter(EventSet, values);

  cout << values[0] << endl;

  // ------------------------------------------------------------------

  addAndStartCounterForInstruction(EventSet, PAPI_LST_INS);

  /* Defined in tests/do_loops.c in the PAPI source distribution */
  //Start process
  cout << "IJK" << endl;
  IJK();

  readCounter(EventSet, values);

  cout << values[0] << endl;

  // ------------------------------------------------------------------

  addAndStartCounterForInstruction(EventSet, PAPI_LST_INS);

  /* Defined in tests/do_loops.c in the PAPI source distribution */
  //Start process
  cout << "IJK" << endl;
  IJK();

  readCounter(EventSet, values);

  cout << values[0] << endl;

  // ------------------------------------------------------------------

  addAndStartCounterForInstruction(EventSet, PAPI_LST_INS);

  /* Defined in tests/do_loops.c in the PAPI source distribution */
  //Start process
  cout << "IJK" << endl;
  IJK();

  readCounter(EventSet, values);

  cout << values[0] << endl;

  // ------------------------------------------------------------------

  addAndStartCounterForInstruction(EventSet, PAPI_LST_INS);

  /* Defined in tests/do_loops.c in the PAPI source distribution */
  //Start process
  cout << "IJK" << endl;
  IJK();

  readCounter(EventSet, values);

  cout << values[0] << endl;
}

