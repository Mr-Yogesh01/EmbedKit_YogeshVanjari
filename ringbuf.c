#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE         (8U)
#define BUFFER_MASK         (BUFFER_SIZE - 1U)

typedef enum {
    RINGBUF_OK = 0,
    RINGBUF_ERR_FULL,
    RINGBUF_ERR_EMPTY,
    RINGBUF_ERR_NULL
} ringbuf_status_t;

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    uint32_t head;
    uint32_t tail;
    uint32_t count;
} ringbuf_t;

void ringbuf_init(ringbuf_t *rb) {
    if (rb == NULL) {
        return;
    }
    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;
}

bool ringbuf_is_full(const ringbuf_t *rb) {
    if (rb == NULL) {
        return false;
    }
    return (rb->count >= BUFFER_SIZE);
}

bool ringbuf_is_empty(const ringbuf_t *rb) {
    if (rb == NULL) {
        return true;
    }
    return (rb->count == 0U);
}

uint32_t ringbuf_get_count(const ringbuf_t *rb) {
    if (rb == NULL) {
        return 0U;
    }
    return rb->count;
}

ringbuf_status_t ringbuf_write(ringbuf_t *rb, uint8_t data) {
    if (rb == NULL) {
        return RINGBUF_ERR_NULL;
    }
    if (ringbuf_is_full(rb)) {
        return RINGBUF_ERR_FULL;
    }

    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1U) & BUFFER_MASK;
    rb->count++;

    return RINGBUF_OK;
}

ringbuf_status_t ringbuf_read(ringbuf_t *rb, uint8_t *data) {
    if ((rb == NULL) || (data == NULL)) {
        return RINGBUF_ERR_NULL;
    }
    if (ringbuf_is_empty(rb)) {
        return RINGBUF_ERR_EMPTY;
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1U) & BUFFER_MASK;
    rb->count--;

    return RINGBUF_OK;
}

static void demo_write(ringbuf_t *rb, uint8_t byte) {
    ringbuf_status_t status = ringbuf_write(rb, byte);
    if (status == RINGBUF_OK) {
        if (ringbuf_is_full(rb)) {
            printf("[WRITE] 0x%02X -> OK (count=%u) FULL\n", byte, (unsigned int)ringbuf_get_count(rb));
        } else {
            printf("[WRITE] 0x%02X -> OK (count=%u)\n", byte, (unsigned int)ringbuf_get_count(rb));
        }
    } else {
        printf("[WRITE] 0x%02X -> FAIL (buffer full)\n", byte);
    }
}

static void demo_read(ringbuf_t *rb) {
    uint8_t byte = 0U;
    ringbuf_status_t status = ringbuf_read(rb, &byte);
    if (status == RINGBUF_OK) {
        printf("[READ] -> 0x%02X (count=%u)\n", byte, (unsigned int)ringbuf_get_count(rb));
    } else {
        printf("[READ] (empty) -> FAIL (buffer empty)\n");
    }
}

int main(void) {
    ringbuf_t rb;
    ringbuf_init(&rb);

    /* Step 1: Write 8 bytes (0x41 to 0x48) */
    demo_write(&rb, 0x41);
    demo_write(&rb, 0x42);
    demo_write(&rb, 0x43);
    demo_write(&rb, 0x44);
    demo_write(&rb, 0x45);
    demo_write(&rb, 0x46);
    demo_write(&rb, 0x47);
    demo_write(&rb, 0x48);

    /* Step 2: Attempt to write one more byte (0x99) */
    demo_write(&rb, 0x99);

    /* Step 3: Read 3 bytes (0x41, 0x42, 0x43) */
    demo_read(&rb);
    demo_read(&rb);
    demo_read(&rb);

    /* Step 4: Write 3 new bytes (0x49, 0x4A, 0x4B) */
    demo_write(&rb, 0x49);
    demo_write(&rb, 0x4A);
    demo_write(&rb, 0x4B);

    /* Step 5: Read all remaining 8 bytes */
    for (uint32_t i = 0; i < 8U; i++) {
        demo_read(&rb);
    }

    /* Step 6: Attempt to read from empty buffer */
    demo_read(&rb);

    return 0;
}
