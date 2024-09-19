//==============================================
//"linked_list.h"
// Maker : 정소은(Lacriox)
// Date : 2024/8/14
//==============================================
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int num_pokemon;

/* the states*/
typedef enum { GLASS, FIRE, WATER } Type;

/*Struct declaration*/
typedef struct POKEMON {
    char name[15];  //
    Type type;
    int attack;
    int HP;
    struct POKEMON* next;
}Pokemon;

typedef struct MYPOKEMON {
    char name[15];
    char Nickname[20];
    Type type;
    int attack;
    int cur_HP;
    int HP;
    struct MYPOKEMON* next;
}Mypokemon;

typedef struct PLAYER {
    Mypokemon* pokemons;
    int money;
    int item[2]; //item[0] : pokeball, item[1] : HP potion
}Player;

void print_list(Pokemon*);
void print_mylist(Mypokemon*);
void print_recover_list(Mypokemon*);
void print_fight_list(int*, Mypokemon*);
void free_list(Pokemon**);
int enqueue(Player*, Pokemon*);
int enqueue_(Player*, Mypokemon*);
void dequeue(Mypokemon**, FILE*);
void dequeue_(Mypokemon**);

void print_list(Pokemon* head) {
    Pokemon* p = head;
    const char* buf = NULL;
    int idx = 1;
    printf(" no |     name     |  type  | HP | attack |\n");
    while (p->next != NULL){
        p = p->next;
        switch (p->type) {
        case 0:
            buf = "Glass";
            break;
        case 1:
            buf = "Fire";
            break;
        case 2:
            buf = "Water";
            break;
        }
        printf("%2d.   %-10s    %-6s   %-4d  %-4d\n", idx++, p->name, buf, p->attack, p->HP);
    }
}

void print_mylist(Mypokemon* head){
    Mypokemon* p = head;
    const char* buf = NULL;
    while (p != NULL) {
        switch (p->type) {
        case 0:
            buf = "Glass";
            break;
        case 1:
            buf = "Fire";
            break;
        case 2:
            buf = "Water";
            break;
        }
        printf("%s %s %d/%d\n", p->name, buf, p->cur_HP, p->HP);
        p = p->next;
    }
}

void print_recover_list(Mypokemon* head) {
    Mypokemon* p = head;
    const char* buf = NULL;
    while(p != NULL){
        switch (p->type) {
        case 0:
            buf = "Glass";
            break;
        case 1:
            buf = "Fire";
            break;
        case 2:
            buf = "Water";
            break;
        }
        p->cur_HP = p->HP;
        printf("%s %s %d/%d\n", p->name, buf, p->cur_HP, p->HP);
        p = p->next;
    }
}

void print_fight_list(int* idx, Mypokemon* pokemon) {
    int i = 0, num = 1;
    const char* buf = NULL;
    while (pokemon != NULL) {
        if (pokemon->cur_HP != 0) {
            idx[i++] = num++;
            switch (pokemon->type) {
            case 0:
                buf = "Glass";
                break;
            case 1:
                buf = "Fire";
                break;
            case 2:
                buf = "Water";
                break;
            }
            printf("%d. %s %s %d/%d\n", i, pokemon->name, buf, pokemon->cur_HP, pokemon->HP);
        }
        else
            num++;
        pokemon = pokemon->next;
    }
    if(idx[0]!=0)
        printf("어느 포켓몬을 내보낼까?(0 : 도망가기)\n▶ ");
}

void free_list(Pokemon** head) {
    Pokemon* temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}
int enqueue(Player* player, Pokemon* pokemon) {
    //srand(time(NULL));
    Mypokemon* temp = player->pokemons;
    Mypokemon* p = (Mypokemon*)calloc(1, sizeof(Mypokemon));
    int num = 0;
    if (player->pokemons == NULL) {
        if (p == NULL) {
            printf("Allocation failed\n");
            return 0;
        }
        player->pokemons = p;
        num = 1;
    }
    else {
        while (temp->next != NULL) {
            temp = temp->next;
            num++;
        }
        temp->next = p;
        num++;
    }
    strcpy(p->Nickname, pokemon->name);             //Nickname
    p->attack = pokemon->attack + rand() % 101;     //attack
    p->HP = pokemon->HP + rand() % 151;             //HP  
    strcpy(p->name, pokemon->name);                 //name
    p->type = pokemon->type;                        //type
    p->cur_HP = p->HP;                              //cur_Hp
    return num;
}

int enqueue_(Player* player, Mypokemon* pokemon) {
    Mypokemon* temp = player->pokemons;
    Mypokemon* p = (Mypokemon*)calloc(1, sizeof(Mypokemon));
    int num = 1;
    char yn;
    //p->next = NULL;
    if (player->pokemons == NULL) {
        if (p == NULL) {
            printf("Allocation failed\n");
            return 0;
        }
        player->pokemons = p;
        num = 1;
    }
    else {
        while (temp->next != NULL) {
            temp = temp->next;
            num++;
        }
        temp->next = p;
        num++;
    }
    printf("별명을 입력하시겠습니까?(y/n)\n▶ ");
    while (1) {
        scanf("%c", &yn);
        while (getc(stdin) != '\n');
        if (yn == 'Y' || yn == 'y') {
            printf("%s의 이름을 정해주세요!\n▶ ", pokemon->name);
            scanf("%s", p->Nickname);
            printf("%s는 새로운 친구가 되었다!", pokemon->name);
            getc(stdin);
            break;
        }
        else if (yn == 'N' || yn == 'n') {
            strcpy(p->Nickname, pokemon->name);
            printf("%s는 새로운 친구가 되었다!", pokemon->name);
            break;
        }
        else {
            printf("다시 입력해주세요\n▶ ");
        }
    }
    p->attack = pokemon->attack;        //attack
    p->HP = pokemon->HP;                //HP  
    strcpy(p->name, pokemon->name);     //name
    p->type = pokemon->type;            //type
    p->cur_HP = pokemon->cur_HP;        //cur_Hp
    return num;
}

void dequeue(Mypokemon** head, FILE* fd) {
    Mypokemon* target = *head;
    Mypokemon* temp = NULL;
    fprintf(fd, "%s %s %d %d %d %d\n", target->name, target->Nickname, target->type, target->attack, target->cur_HP, target->HP);
    temp = (* head)->next;
    free(target);
    *head = temp;
}

void dequeue_(Mypokemon** head) {
    Mypokemon* target = *head;
    Mypokemon* temp = NULL;
    temp = (*head)->next;
    free(target);
    *head = temp;
}
