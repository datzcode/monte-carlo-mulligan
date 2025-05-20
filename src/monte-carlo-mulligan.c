#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "monte-carlo-mulligan.h"

int main(void)
{
	const int rounds = 10000000;
	int wins = 0;
	round_result res;

	time_t now;
	time(&now);

	if (now == ((time_t) - 1)) {
		return 1;
	}

	srand48((long)now);

	for (int i = 0; i < rounds; i++) {
		res = run_round();
		switch (res) {
		case ROUND_ERROR:
			return 1;
		case ROUND_GOODHAND:
			wins++;
		default:
			break;
		}
	}

	fprintf(stdout, "Played %d; ", rounds);
	fprintf(stdout, "Won %d; ", wins);
	fprintf(stdout, "Win rate %f\n", ((float)wins) / ((float)rounds));
	return 0;
}

void debug(char *message)
{
#ifdef DEBUG
	fprintf(stderr, message);
#endif				// DEBUG
}

void print_state(struct cards hand, struct cards deck)
{
#ifdef DEBUG
	fprintf(stderr, "Deck = {A: %2d, B: %2d}; ", deck.a, deck.rest);
	fprintf(stderr, "Hand = {A: %2d, B: %2d}\n", hand.a, hand.rest);
#endif				// DEBUG
}

struct cards initialize_deck(void)
{
	return (struct cards) { ACARDS, BCCARDS };
}

struct cards initialize_empty(void)
{
	return (struct cards) { 0, 0 };
}

bool is_draw_successful(const struct cards *hand)
{
	return hand->a != 0;
}

draw_error draw(struct cards *hand, struct cards *deck)
{
	uint32_t cards_on_deck = deck->a + deck->rest;
	if (cards_on_deck == 0) {
		debug("DRAW_ERROR: DECK WAS EMPTY! UNABLE TO DRAW\n");
		return DRAW_ERROR;
	}

	double r = drand48();
	uint32_t selected_card = (uint32_t) (r * ((double)cards_on_deck));

#ifdef DEBUG
	fprintf(stderr, "Selected number %d -> ", selected_card);
#endif				// DEBUG

	if (selected_card < deck->a) {
		hand->a++;
		deck->a--;
#ifdef DEBUG
		fprintf(stderr, "A\n");
#endif				// DEBUG

	} else {
		hand->rest++;
		deck->rest--;
#ifdef DEBUG
		fprintf(stderr, "rest\n");
#endif				// DEBUG
	}
	return DRAW_SUCCESS;
}

void move_cards(struct cards *from, struct cards *to)
{
	to->a = to->a + from->a;
	to->rest = to->rest + from->rest;
	*from = initialize_empty();
}

round_result run_round(void)
{
	struct cards hand = initialize_empty();
	struct cards board = initialize_empty();
	struct cards deck = initialize_deck();

	print_state(hand, deck);
	for (int i = 0; i < FIRSTDRAW; i++) {
		if (draw(&hand, &deck) == DRAW_ERROR) {
			debug("ROUND_ERROR: UNABLE TO DRAW!\n");
			return ROUND_ERROR;
		}
	}
	print_state(hand, deck);

	if (is_draw_successful(&hand)) {
		debug("Hand is GOOD\n");
		return ROUND_GOODHAND;
	}

	for (int i = 0; i < SECONDDRAW; i++) {
		if (draw(&board, &deck) == DRAW_ERROR) {
			debug("ROUND_ERROR: UNABLE TO DRAW!\n");
			return ROUND_ERROR;
		}
	}
	move_cards(&hand, &deck);
	move_cards(&board, &hand);
	debug("Draw new hand and return old hand to deck\n");
	print_state(hand, deck);

	if (is_draw_successful(&hand)) {
		debug("Hand is GOOD\n");
		return ROUND_GOODHAND;
	}
	print_state(hand, deck);

	for (int i = 0; i < THIRDDRAW; i++) {
		if (draw(&hand, &deck) == DRAW_ERROR) {
			debug("ROUND_ERROR: UNABLE TO DRAW!\n");
			return ROUND_ERROR;
		}
	}
	print_state(hand, deck);

	if (is_draw_successful(&hand)) {
		debug("Hand is GOOD\n");
		return ROUND_GOODHAND;
	}

	debug("Hand is BAD\n");
	return ROUND_BADHAND;
}
