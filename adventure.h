//==============================================
//"adventure.h"
// Maker : 정소은(Lacriox)
// Date : 2024/8/14
//==============================================
#pragma once
#include <stdio.h>
#include "myprint.h"

typedef enum { FIGHT, WIN, DEFEAT, RUNAWAY, MY_TURN, CATCH, MASTER, S_QUIT} State;
typedef enum { EQUAL, ME, YOU } Compat;

State state = FIGHT;
Compat compat = EQUAL;

State attack(Mypokemon*, Mypokemon* );
State attacked(Mypokemon*, Mypokemon**, Player*);
State bag(Player*, Mypokemon* , Mypokemon*);
State run(int, int);
Compat compat_(Mypokemon*, Mypokemon*);

State attack(Mypokemon* You, Mypokemon* Me) {
	//srand(time(NULL));
	int damage = Me->attack;
	banner();
	printf("%s의 공격!\n", Me->name);
	if (compat == ME) {
		damage = Me->attack * 1.5;
		printf("효과가 굉장했다!\n");
	}
	else if (compat == YOU) {
		damage = Me->attack * 0.5;
		printf("효과가 별로인 듯 하다.\n");
	}
	if (rand() % 100 < 20) {
		printf("급소에 맞았다!\n");
		damage = damage * 1.5;
	}
	printf("%s는 %d의 데미지를 입었다.\n", You->name, damage);
	if (You->cur_HP < damage) {
		You->cur_HP = 0;
	}
	else
		You->cur_HP -= damage;
	if (You->cur_HP == 0) {
		printf("%s는 쓰러졌다.\n", You->name);
		return WIN;
	}
	else
		return FIGHT;
}
State attacked(Mypokemon* You, Mypokemon** Me, Player* player) {
	//srand(time(NULL));
	int damage = You->attack;
	int idx[4] = {0, };
	banner();
	printf("%s의 공격!\n",You->name);
	if (compat == ME) {
		damage = You->attack * 0.5;
		printf("효과가 별로인 듯 하다.\n");
	}
	else if (compat == YOU) {
		damage = You->attack * 1.5;
		printf("효과가 굉장했다!\n");
	}
	if (rand() % 100 < 20) {
		printf("급소에 맞았다!\n");
		damage *= 1.5;
	}
	printf("%s는 %d의 데미지를 입었다.\n", (*Me)->name, damage);
	if ((* Me)->cur_HP < damage) {
		(*Me)->cur_HP = 0;
		printf("%s는 쓰러졌다.\n", (*Me)->name);
	}
	else
		(*Me)->cur_HP -= damage;
	if ((*Me)->cur_HP == 0) {
		battle_print(*Me, You);
		//노드 순회로 체력이 0이 아닌 idx 찾기
		Mypokemon* p = player->pokemons;
		print_fight_list(idx, player->pokemons);
		if (idx[0] == 0) {
			return DEFEAT;
		}//체력이 0이 아닌 idx가 없다면
		else {
			int ch;
			scanf("%d", &ch);
			while (getc(stdin) != '\n');
			while (1) {
				if (ch == 0)
					return RUNAWAY;
				else {
					if (idx[ch - 1] == 0)
						printf("다시 입력\n");
					else {
						for (int i = 0; i < idx[ch - 1]-1; i++) {
							p = p->next;
						}
						*Me = p;
						return FIGHT;
					}
				}
			}
		}
	}
	else
		return FIGHT;
}
State bag(Player* player, Mypokemon* Me, Mypokemon* You){
	int ch, num, i = 0;
	double a = You->cur_HP, b=You->HP;
	char yn;
	battle_print(Me, You);
	printf("1. 몬스터 볼 x %d\n2. 회복약 x %d\n▶ ",player->item[0], player->item[1]);
	scanf("%d", &ch);
	while (getc(stdin) != '\n');
	if (ch == 1) {
		if (player->item[0] == 0) {
			printf("몬스터볼이 없습니다!\n");
			return MY_TURN;
		}
		player->item[0]--;
		battle_print(Me, You);
		pokeball_print(You);
		//srand(time(NULL));
		if (rand() % 100 < 100 - (a / b) * 100) {
			printf("\n%s을(를) 잡았다!\n", You->name);
			num = enqueue_(player, You);
			if (num == 6) {
				master_print(player);
				yn = getc(stdin);
				while (getc(stdin) != '\n');
				while (1)
					if (yn == 'Y' ||  yn == 'y') {
						return MASTER;
					}
					else if (yn == 'N' || yn == 'n') {
						return S_QUIT;
					}
					else {
						printf("다시 입력\n");
					}		
			}
			return CATCH;
		}
		else {
			printf("포켓몬이 몬스터볼에서 빠져나왔다!\n");
			return FIGHT;
		}
	}
	else if (ch == 2) {
		if (player->item[1] == 0) {
			printf("회복약이 없습니다!\n");
			return MY_TURN;
		}
		player->item[1]--;
		int recover = Me->HP * 0.3;
		Me->cur_HP += recover;
		if (Me->cur_HP > Me->HP)
			Me->cur_HP = Me->HP;
		banner();
		printf("회복약을 사용하였다!\n%s(은)는 %d의 체력을 회복하였다.", Me->name, recover);
		banner();
		return MY_TURN;
	}
}
State run(int cur_HP, int HP) {
	//srand(5);
	double a = HP, b = cur_HP;
	double ratio =  b/a;
	//srand(time(NULL));
	if (ratio -1 < 0.001) {
		if (rand()%100 < 10) {
			state = RUNAWAY;
			return state;
		}
	}
	else if (ratio - 0.6 < 0.401) {
		if (rand() % 100 < 40) {
			state = RUNAWAY;
			return state;
		}
	}
	else if (ratio - 0.4 < 0.201) {
		if (rand() % 100 < 60) {
			state = RUNAWAY;
			return state;
		}
	}
	else if (ratio < 0.401) {
		if (rand() % 100 < 90) {
			state = RUNAWAY;
			return state;
		}
	}
	printf("도망에 실패했다!\n");
	return FIGHT;
}
Compat compat_(Mypokemon* Me, Mypokemon* You) {
	if (Me->type > You->type) {
		if (Me->type == WATER && You->type == GLASS)
			compat = YOU;
		else
			compat = ME;
	}
	else if (Me->type < You->type) {
		if (Me->type == GLASS && You->type == WATER)
			compat = ME;
		else
			compat = YOU;
	}
	else
		compat = EQUAL;
}
