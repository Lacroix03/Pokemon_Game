//==============================================
//"myprint.h"
// Maker : 정소은(Lacriox)
// Date : 2024/8/14
//==============================================
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "linked_list.h"

void banner();
void start_print();
void main_print();
void adv_print();
void battle_print(Mypokemon*, Mypokemon* );
void master_print(Player*);
void pokeball_print();
void save_print();
void shop_print();
void recovery_print();

void banner() {
    printf("\n■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n\n");
}

void start_print() {
    banner();
    printf("                                                           __      \n");
    printf("      _____       _                            _____      |  |     \n");
    printf("     |  _  | ___ | |_  ___  _____  ___  ___   |   __| ___ |  |     \n");
    printf("     |   __|| . || '_|| -_||     || . ||   |  |  |  || . ||__|     \n");
    printf("     |__|   |___||_,_||___||_|_|_||___||_|_|  |_____||___||__|     \n");
    printf("                                                                   \n");
    banner();
    printf("                    ▶ Press Enter to Start!\n");
    banner();
}

void main_print() {
    banner();
    printf("모험에 떠나시겠습니까?\n");
    printf("  1.Yes!  2.Save  3.shop  4.Pokemon center  5.Pokemon book\n▶ ");
}

void adv_print() {
    //srand(time(NULL));
    int wait = rand() % 5 + 1;
    banner();
    printf("포켓몬을  탐색하는 중 ...");
    for (int i = 0; i < wait; i++) {
        printf(" ...");
        sleep(1);
    }
}

void battle_print(Mypokemon* me, Mypokemon* you) {
    banner();
    printf("\t\t\t%s\n", you->name);
    printf("\t\t\t%d/%d\n\n", you->cur_HP, you->HP);
    if (me->cur_HP == 0) {
        printf("%s(기절)\n", me->name);
    }
    else {
        printf("%s\n", me->name);
    }
    printf("%d/%d\n", me->cur_HP, me->HP);
    

    banner();
}
void pokeball_print(Mypokemon* you) {
    printf("몬스터 볼을 %s에게 던졌다!\n", you->name);
    //srand(time(NULL));
    int wait = rand() % 5 + 1;
    for (int i = 0; i < wait; i++) {
        printf(" ...");
        sleep(1);
    }
}
void save_print() {
    banner();
    printf("  1.Restart    2.Quit game\n▶ ");
}
void shop_print() {
    banner();
    printf("$Shop$\n\n1. pokeball 1000$\n2. HP potion 500$\n");
    banner();
}

void master_print(Player* player) {
    banner();
    printf("포켓몬 마스터가 되었다!\n\n");
    print_mylist(player->pokemons);
    for(int i = 0; i<6; i++)
        dequeue_(&(player->pokemons));
    printf("\n포켓몬볼 x %d\n상처약 x %d", player->item[0], player->item[1]);
    printf("\n지갑 : % d\n", player->money);
    banner();
    printf("게임을 재시작하시겠습니까?\n▶ ");
}

void recovery_print() {
    banner();
    printf("포켓몬을 치료하는 중입니다");
    for (int i = 1; i < 3; i++) {
        printf(" ...");
        sleep(1);
    }
    printf("\n치료가 완료되었습니다!\n");
    banner();
}

