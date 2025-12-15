// Fuzz target for deserialize_transaction
#include <stdint.h>
#include <string.h>

#include "../src/transaction/deserialize.h"
#include "../src/globals.h"

// Provide the global context expected by the app
global_ctx_t G_context;

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    buffer_t buf = {0};
    buf.ptr = (uint8_t *) data;
    buf.offset = 0;
    buf.size = size;

    // Reset context to a known state
    memset(&G_context, 0, sizeof(G_context));

    (void) deserialize_transaction(&buf);

    return 0;
}
