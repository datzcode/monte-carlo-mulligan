#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define ACARDS 2
#define BCCARDS 18

#define FIRSTDRAW 5
#define SECONDDRAW 3
#define THIRDDRAW 1

struct cards {
	uint32_t a;
	uint32_t rest;
};

typedef enum {
	DRAW_SUCCESS,
	DRAW_ERROR
} draw_error;

typedef enum {
	ROUND_GOODHAND,
	ROUND_BADHAND,
	ROUND_ERROR
} round_result;

struct cards initialize_deck(void);

struct cards initialize_empty(void);

bool is_draw_successful(const struct cards *hand);

draw_error draw(struct cards *hand, struct cards *deck);

void return_to_deck(struct cards *hand, struct cards *deck);

round_result run_round(void);
