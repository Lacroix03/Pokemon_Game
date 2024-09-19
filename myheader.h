//==============================================
//"myheader.h"
// Maker : 정소은(Lacriox)
// Date : 2024/8/14
//==============================================
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "linked_list.h"
#include "myprint.h"
#include "myfile.h"
#include "adventure.h"
#include <time.h>

typedef enum { QUIT, RUN, RESTART } Game;

/* total number of pokemons */
int num_pokemon;
Game game = RUN;
/*game start*/
Game start();
void new_game(Pokemon*, Player*);
void cont_game(Pokemon*, Player*);
/* game main */
/* [1] adventure */
Game game_main(Pokemon*, Player*);
Game adventure(Pokemon*, Player*);
/* [2] save */
Game save(Player*);
/* [3] shop */
void shop(Player*);
/* [4] pokemon center*/
void center(Player*);
void recovery(Mypokemon*);
/* [5] pokemon book*/
void book(Pokemon*);


Game start() {
    Pokemon* Pokemon_list = (Pokemon*)calloc(1,sizeof(Pokemon));
    Player* player = (Player*)calloc(1,sizeof(Player));
    if (Pokemon_list == NULL) {
        printf("Pokemon_list failed\n");
    }
    if (player == NULL) {
        printf("player failed\n");
    }
    FILE * fd = fopen("pokemon.txt", "r");
    if (fd == NULL) {
        printf("failed to read pokemon.txt\n");
        free(Pokemon_list);
        free(player);
        return QUIT;
    }
    else
        num_pokemon = load_list(Pokemon_list, fd);
    
    start_print();
    /* Load savefile */
    FILE* fp = fopen("savefile.txt", "r");
    while (getc(stdin) != '\n');
    if (fp == NULL) { // savefile not exist
        new_game(Pokemon_list, player);
    }
 
    else { // savefile exist
        char ch = 0;
        while (1) {
            printf("1.new game\t2.continue\n\n▶ ");
            ch = getc(stdin);
            while (getc(stdin) != '\n');//tcflush(0, TCIFLUSH); buffer clear
            if (ch == '1') {
                new_game(Pokemon_list, player);
                while (getc(stdin) != '\n');
                break;
            }
            else if (ch == '2') {
                load_save(player, fp);
                break;
            }
            else {
                printf("Invalid input\n\n");
            }
        }
    }
    /* Start main game*/
    game = game_main(Pokemon_list, player);
    if (game == QUIT || game == RESTART) {
        free_list(&Pokemon_list);
        free(player);
    }
    return game;
}

void new_game(Pokemon* Pokemon_list, Player* player) {
    srand(time(NULL));
    int ch;
    Pokemon* list = Pokemon_list;
    while (1) {
        banner();
        printf("Which pokemon would you choose?\n");
        printf("  1.Charmander     2.Bulbasaur     3.Squirtle\n▶ ");
        scanf("%d", &ch);
        if (ch >= 1 && ch <= 3) {
            for (int i = 0; i < ch; i++) {
                list = list->next;
            }
            enqueue(player, list);
            player->item[0] = 0; 
            player->item[1] = 0;
            player->money = 10000;
            break;
        }
        else {
            printf("Invalid input\n\n");
        }
    }
}

Game game_main(Pokemon* Pokemon_list, Player* player) {
    game = RUN;
    char ch = 0;
    while (game == RUN) {
        main_print();
        ch = getc(stdin);
        while (getc(stdin) != '\n');
        if (ch == '1') {
            game = adventure(Pokemon_list, player);
        }
        else if (ch == '2') {
            game = save(player);
        }
        else if (ch == '3') {
            shop(player);
        }
        else if (ch == '4') {
            center(player);
        }
        else if (ch == '5') {
            book(Pokemon_list);
        }
        else {
            printf("Invalid input\n\n");
        }
    }
    return game;
}

Game adventure(Pokemon* Pokemon_list, Player* player) {
    State state = FIGHT;
    adv_print();
    int ch = 0;
    int idx[5] = { 0, };
    Pokemon* list = Pokemon_list;
    Mypokemon* You = (Mypokemon*)calloc(1, sizeof(Mypokemon));
    Mypokemon* Me = player->pokemons;
    //상대 포켓몬 초기화
    srand(time(NULL));
    for (int i = 0, j = rand() % num_pokemon + 1; i < j; i++) {
        list = list->next;
    }
    strcpy(You->name, list->name);
    You->type = list->type;
    You->attack = list->attack + rand()%101;
    You->cur_HP = list->HP + rand()% 151;
    You->HP = You->cur_HP;
    //상성 설정
    compat = compat_(Me, You);
    //배틀 시작
    banner();
    printf("앗! 야생의 %s가 나타났다!", You->name);
    while (getc(stdin) != '\n');
    //내 포켓몬 설정
    print_fight_list(idx, player->pokemons);
    if (idx[0] == 0) {
        printf("싸울 수 있는 포켓몬이 없습니다!\n");
        printf("도망치시겠습니까?(y/n)\n▶ ");
        free(You);
        char yn;
        yn = getc(stdin);
        while (getc(stdin) != '\n');
        if (yn == 'Y' || yn == 'y') {
            state = RUNAWAY;
        }
        else if (yn == 'N' || yn == 'n') {
            state = DEFEAT;
        }
    }
    else {
        int ch2;
        Mypokemon* p = player->pokemons;
        scanf("%d", &ch2);
        while (getc(stdin) != '\n');
        while (1) {
            if (ch2 == 0) {
                state = RUNAWAY;
                break;
            }
            else {
                if (idx[ch2 - 1] == 0)
                    printf("다시 입력\n");
                else {
                    for (int i = 0; i < idx[ch2 - 1] - 1; i++) {
                        p = p->next;
                    }
                    Me = p;
                    state = FIGHT;
                    break;
                }
            }
        }
    }
    //턴 설정
    int turn = rand() % 2; // 0: my turn, 1: opponent turn;
    while (state == FIGHT) {
        //srand(4);
        battle_print(Me, You);
        if (!turn) {
            while (1) {
                printf("무엇을 해야할까?\n1.공격 2.가방열기 3.도망치기\n▶ ");
                scanf("%d", &ch);
                if (ch == 1) {
                    state = attack(You, Me);
                    break;
                }
                else if (ch == 2) {
                    state = bag(player, Me, You);
                    if (state == MY_TURN) {
                        turn--;
                        state = FIGHT;
                    }
                    break;
                }
                else if (ch == 3) {
                    state = run(You->cur_HP, You->HP);
                    break;
                }
                else
                    printf("Invalid input");
            }
            turn++;
        }
        else {
            state = attacked(You, &Me, player);
            turn--;
        }
        compat = compat_(Me, You);
    }
    //typedef enum { FIGHT, WIN, DEFEAT, RUNAWAY, CATCH, MASTER, S_QUIT}
    if (state == WIN) {
        printf("win!\n"); // 돈 지급
        player->money += rand() % 201 + 300;
        free(You);
        game = RUN;
    }
    else if (state == DEFEAT) {
        printf("눈앞이 깜깜해졌다...\n");
        printf("defeat!\n"); // 돈 잃음
        if (player->money < 1000) {
            player->money = 0;
        }
        else {
            player->money -= 1000;
        }
        free(You);
        game = RUN;
    }
    else if (state == RUNAWAY) {
        printf("성공적으로 도망쳤다!\n");
        free(You);
        game = RUN;
    }
    else if (state == CATCH) {
        game = RUN;
    }
    else if (state == MASTER) {
        game = RESTART;
    }
    else if (state == S_QUIT) {
        game = QUIT;
    }
    while (getc(stdin) != '\n');
    return game;
}

void shop(Player* player) {
    char ch = 0;
    int num = 0;
    while (1) {
        shop_print();
        printf("My money : %d\n\n", player->money);
        printf("무엇을 구매할까? (나가기 0)\n▶ ");
        ch = getc(stdin);
        while (getc(stdin) != '\n');
        if (ch == '0')
            break;
        else if (ch == '1') {
            printf("포켓몬볼을 몇 개 구매할까? (취소 -1)\n▶ ");
            scanf("%d", &num);
            while (getc(stdin) != '\n');
            if (num < 0);
            else if (player->money < num * 1000)
                printf("\n돈이 부족합니다!\n");
            else {
                player->money -= num * 1000;
                player->item[0] += num;
                printf("\n성공적으로 구매하였다! (잔액 %d)\n", player->money);
            }
        }
        else if (ch == '2') {
            printf("상처약을 몇 개 구매할까? (취소 -1)\n▶ ");
            scanf("%d", &num);
            while (getc(stdin) != '\n');
            if (num < 0);
            else if (player->money < num * 500)
                printf("\n돈이 부족합니다!\n");
            else {
                player->money -= num * 500;
                player->item[1] += num;
                printf("\n성공적으로 구매하였다! (잔액 %d)\n", player->money);
            }
        }
        else {
            printf("Invalid input");
        }
    }
}

Game save(Player* player) {
    Mypokemon* head = player->pokemons;
    char ch;
    FILE* fd = fopen("savefile.txt", "w");
    if (fd == NULL) {
        printf("failed to open savefile.txt");
        return RUN;
    }
    if(head)
        save_info(player, fd);
    save_print();
    while (1) {
        ch = getc(stdin);
        while (getc(stdin) != '\n');
        if (ch == '1'){
            return RESTART;
        }
        else if (ch == '2') {
            return QUIT;
        }
        else {
            printf("Invalid input");
        }
    }
}

void center(Player * player) {
    Mypokemon* p = player->pokemons;
    if (p) {
        recovery_print();
        print_recover_list(p);
    }
    else {
        printf("No pokemon to treat!\n");
    }
    while (getc(stdin) != '\n');
}

void book(Pokemon * Pokemon_list) {
    banner();
    print_list(Pokemon_list);
    while (getc(stdin) != '\n');
}
