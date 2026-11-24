
#ifndef TASK_INTEGRATION
#define	TASK_INTEGRATION

#include <xc.h>

#include "state.h" // include processor files - each processor file is guarded.  

char allTasksComplete(){
    return (state.deposited_sample && state.finished_canyon && state.collected_sample);
}

#endif	/* XC_HEADER_TEMPLATE_H */

