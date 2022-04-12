#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#define MAX_TITLE 40
#define GAME_TYPES 4

typedef enum GameType
{
	FPS, MMO, RPG, RTS,
}GameType;

typedef struct Game
{
	char title[MAX_TITLE];
	double price;
	GameType type;
} Game;

typedef struct GameStoreElem
{
	Game game;
	struct GameStoreElem* next, * prev;
} GameStoreElem;

typedef struct GameStore
{
	GameStoreElem* head, * tail;
} GameStore;

//part 1
char* gametype_string(GameType type);
Game init_game(char* title, double price, GameType type);
void print_game(Game g);
//part 2
GameStoreElem* create_store_elem(Game g);
void add_game_to_store(GameStore* gs, GameStoreElem* el);
void list_store_games(GameStore* gs);
void free_store(GameStore* gs);
//part 3
GameStoreElem* get_game_of_type(GameStore* gs, GameType type);
//part 4
void move_games(GameStore* dst, GameStore* src);

int main(void)
{
	puts("**********************Part 1**********************");

	Game games[24] =
	{
		init_game("Overwatch", 19.99, FPS),
		init_game("Counter Strike: Global Offensive", 0, FPS),
		init_game("Call of Duty: Modern Warfare", 59.99, FPS),
		init_game("Serious Sam 4", 39.99, FPS),
		init_game("That super hot game you've dreamed of", 59.99, FPS),
		init_game("Quake Champions", 0, FPS),
		init_game("World of Warcraft", 39.99, MMO),
		init_game("Black Desert Online", 9.99, MMO),
		init_game("Final Fantasy XIV", 19.99, MMO),
		init_game("Guild Wars 2", 0, MMO),
		init_game("The Elder Scrolls Online", 19.99, MMO),
		init_game("Star Wars: The Old Republic", 14.99, MMO),
		init_game("Bloodborne", 19.99, RPG),
		init_game("Witcher III: The Wild Hunt", 24.99, RPG),
		init_game("Divinity: Original Sin 2", 39.99, RPG),
		init_game("Dragon Age: Inquisition", 39.99, RPG),
		init_game("Dark Souls III", 19.99, RPG),
		init_game("The Elder Scrolls V: Skyrim", 19.99, RPG),
		init_game("StarCraft II", 39.99, RTS),
		init_game("Age of Empires II", 19.99, RTS),
		init_game("WarCraft III", 19.99, RTS),
		init_game("Iron Harvest", 59.99, RTS),
		init_game("Total War: Warhammmer", 59.99, RTS),
		init_game("Stronghold HD", 19.99, RTS)
	};
	print_game(games[4]);

	puts("\n**********************Part 2**********************");

	GameStore gs1 = { NULL, NULL };
	GameStore gs2 = { NULL, NULL };

	int i;
	for (i = 0; i < 24; i++)
	{
		if (i % 2)
			add_game_to_store(&gs1, create_store_elem(games[i]));
		else
			add_game_to_store(&gs2, create_store_elem(games[i]));
	}

	printf("Store 1 games: \n");
	list_store_games(&gs1);

	puts("\n**********************Part 3**********************");

	printf("Getting an FPS game from store 1:\n");
	GameStoreElem* rem1 = get_game_of_type(&gs1, FPS);
	print_game(rem1->game);
	printf("\nGetting an RTS game from store 1:\n");
	GameStoreElem* rem2 = get_game_of_type(&gs1, RTS);
	print_game(rem2->game);

	printf("\nanother one...:\n");
	GameStoreElem* rem3 = get_game_of_type(&gs1, RTS);
	print_game(rem3->game);

	printf("\nand another one...:\n");
	GameStoreElem* rem4 = get_game_of_type(&gs1, RTS);
	print_game(rem4->game);

	free(rem1);
	free(rem2);
	free(rem3);
	free(rem4);

	printf("\nStore 1 games: \n");
	list_store_games(&gs1);
	return 0;
}

void move_games(GameStore* dst, GameStore* src) 
{
	if (src == NULL)
		return;
	if (dst->head == NULL) 
	{
		dst -> head = src->head;
		dst->tail = src->tail;
		return;
	}
	dst->tail->next = src->head;
	dst->tail = src->tail;
	return;

}

GameStoreElem* get_game_of_type(GameStore* gs, GameType type)
{
	GameStoreElem* tmp = gs->head;
	while (tmp->game.type != type && tmp != NULL)
		tmp = tmp->next;
	if (tmp == NULL)
		return NULL;
	if (tmp == gs->head && tmp == gs->tail) 
	{
		gs->head = gs->tail = NULL;
		return tmp;
	}
	if (tmp == gs->head) 
	{
		gs->head->next->prev = NULL;
		gs->head = gs->head->next;
		tmp->next = NULL;
		return tmp;
	}
	if (tmp == gs->tail) 
	{
		gs->tail->prev->next = NULL;
		gs->tail = gs->tail->prev;
		tmp->prev = NULL;
		return tmp;
	}
	GameStoreElem* prev = tmp->prev;
	GameStoreElem* next = tmp->next;
	prev->next = next;
	next->prev = prev;
	tmp->prev = tmp->next = NULL;
	return tmp;
}



void free_store(GameStore* gs) 
{
	GameStoreElem* tmp = gs->tail;
	while (tmp != NULL) {
		GameStoreElem* tmp2 = tmp->prev;
		free(tmp);
		tmp = tmp2;
	}
	gs->tail = NULL;
	gs->head = NULL;
}

void list_store_games(GameStore* gs) 
{
	GameStoreElem* tmp = gs->head;
	while (tmp != NULL){
		print_game(tmp->game);
		tmp = tmp->next;
	}

}

void add_game_to_store(GameStore* gs, GameStoreElem* el) 
{
	if (el == NULL)
		return;
	if (gs->head == NULL) {
		gs->head = gs->tail = el;
		gs->head->prev = gs->head->next = gs->tail->prev = gs->tail->next = NULL;
		return;
	}
	else 
	{
		gs->tail->next = el;
		el->prev = gs->tail;
		gs->tail = el;
		return;
	}
}

GameStoreElem* create_store_elem(Game g) 
{
	GameStoreElem* tmp = malloc(sizeof(GameStoreElem));
	if (tmp == NULL) {
		return NULL;
	}
	tmp->game.price = g.price;
	strcpy(tmp->game.title, g.title);
	tmp->game.type = g.type;
	tmp->next = tmp->prev= NULL;

	return tmp;
}


void print_game(Game g) 
{
	printf("%s [%s]: $%.2lf \n", g.title, (gametype_string(g.type)), g.price);
}

Game init_game(char* title, double price, GameType type)
{
	Game tmp;
	strcpy(tmp.title, title);
	tmp.price = price;
	tmp.type = type;
	return tmp;
}

char* gametype_string(GameType type) 
{
	switch (type) {
	case 0:
		return ("FPS");
	case 1:
		return ("MMO");
	case 2:
		return ("RPG");
	case 3:
		return ("RTS");
	}
	return NULL;
}