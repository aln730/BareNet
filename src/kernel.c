#include <stdint.h>
#include <stddef.h>
#include <limine.h>

__attribute__((used, section(".limine_requests")))
static struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static struct limine_framebuffer_request fb_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Forward declaration
void kmain(void);

void _start(void) {
    // Ensure memory map was provided
    if (memmap_request.response == NULL || memmap_request.response->entry_count == 0) {
        for (;;) __asm__("hlt");
    }

    // Optional: fill framebuffer with a color
    if (fb_request.response != NULL && fb_request.response->framebuffer_count > 0) {
        struct limine_framebuffer* fb = fb_request.response->framebuffers[0];

        uint32_t* framebuffer = (uint32_t*) fb->address;
        size_t pixels = fb->width * fb->height;

        // Fill screen with blue color (BGR)
        for (size_t i = 0; i < pixels; i++) {
            framebuffer[i] = 0xFF0000; // Blue
        }
    }

    // Call main kernel logic
    kmain();

    // Halt if kmain ever returns
    for (;;) __asm__("hlt");
}

void kmain(void) {
    // Kernel loop placeholder
    for (;;) __asm__("hlt");
}
