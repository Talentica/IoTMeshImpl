/** @brief Application.h file.
 *
 */

#include <stdint.h>
#include <stdio.h>


extern void application_event_handler(uint8_t opcode, uint16_t msg_source_id, const uint8_t * data, uint8_t length);
extern void mesh_application_run(void);

/* End of file */
