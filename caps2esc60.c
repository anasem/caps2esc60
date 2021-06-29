#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <linux/input.h>

// clang-format off
const struct input_event
syn       = {.type = EV_SYN , .code = SYN_REPORT   , .value = 0},
esc_up    = {.type = EV_KEY , .code = KEY_ESC      , .value = 0},
ctrl_up   = {.type = EV_KEY , .code = KEY_LEFTCTRL , .value = 0},
esc_down  = {.type = EV_KEY , .code = KEY_ESC      , .value = 1},
ctrl_down = {.type = EV_KEY , .code = KEY_LEFTCTRL , .value = 1};
// clang-format on

void print_usage(FILE *stream, const char *program) {
    // clang-format off
    fprintf(stream,
            "caps2esc60 - transforming the most useless key ever in the most useful one\n"
            " A lean version for 60%% keyboard layouts."
            " It will swap keys such as\n"
            "      - caps as esc\n"
            "      - esc as grave accent\n"
            "      - grave accent as caps\n"
            "Note: capslock doesn't act as ctrl in this version"
            "\n"
            "usage: %s [-h]\n"
            "\n"
            "options:\n"
            "    -h         show this message and exit\n", 
            program
            );
    // clang-format on
}

int read_event(struct input_event *event) {
    return fread(event,  sizeof(struct input_event), 1, stdin) == 1;
}

void write_event(const struct input_event *event) {
    if (fwrite(event, sizeof(struct input_event), 1, stdout) != 1)
        exit(EXIT_FAILURE);
}

void write_event_60(struct input_event *event) {
    if (event->type == EV_KEY)
        switch (event->code) {
            case KEY_ESC:
                event->code = KEY_GRAVE;
                break;
            case KEY_GRAVE:
                event->code = KEY_CAPSLOCK;
                break;
            case KEY_CAPSLOCK:
                event->code = KEY_ESC;
        }
    write_event(event);
}

int main(int argc, char *argv[]) {

    for (int opt; (opt = getopt(argc, argv, "h")) != -1;) {
        switch (opt) {
            case 'h':
                return print_usage(stdout, argv[0]), EXIT_SUCCESS;
        }

        return print_usage(stderr, argv[0]), EXIT_FAILURE;
    }

    struct input_event input;
    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (read_event(&input)) {
        if (input.type == EV_MSC && input.code == MSC_SCAN)
            continue;

        if (input.type != EV_KEY && input.type != EV_REL &&
            input.type != EV_ABS) {
            write_event(&input);
            continue;
        }

        if (input.type == EV_KEY)
            write_event_60(&input);
        }
}

