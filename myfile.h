//==============================================
//"myfile.h" 파일 입출력 관련 함수들
// Maker : 정소은(Lacriox)
// Date : 2024/8/14
//==============================================
#pragma once
#include "linked_list.h"

int load_list(Pokemon*, FILE*);
void load_save(Player*, FILE*);
void save_info(Player*, FILE*);

int load_list(Pokemon* head, FILE* fd) {
    int num = 0;
    fscanf(fd,"%d\n", &num);
    for (int i = 0; i < num; i++) {
        Pokemon* new_ = (Pokemon*)calloc(1,sizeof(Pokemon));
        //new_->next = NULL;
        head->next = new_;
        fscanf(fd,"%s %d %d %d\n", new_->name, &new_->type, &new_->attack, &new_->HP);
        head = new_;
    }
    return num;
}

void load_save(Player* player, FILE* fd) {
    Mypokemon* temp = player->pokemons;
    int num = 0;
    fscanf(fd,"%d\n", &num);
    for (int i = 0; i < num; i++) {
        Mypokemon* p = (Mypokemon*)calloc(1,sizeof(Mypokemon));
        //p->next = NULL;
        if (player->pokemons == NULL) {
            // If this is the first pokemon, set it as the head of the list
            player->pokemons = p;
        }
        else {
            // Otherwise, link it to the last node
            temp->next = p;
        }
        temp = p; // Move the last pointer to the newly added node
        fscanf(fd, "%s %s %d %d %d %d\n", p->name, p->Nickname, &p->type, &p->attack, &p->cur_HP, &p->HP);
    }
    fscanf(fd, "\n");
    fscanf(fd, "%d %d\n", &player->item[0], &player->item[1]);
    fscanf(fd, "\n");
    fscanf(fd, "%d", &player->money); 
    fclose(fd);
}

void save_info(Player* player, FILE* fd) {
    Mypokemon* p = player->pokemons;
    int num = 0;
    while (p) {
        p = p->next;
        num++;
    }
    fprintf(fd, "%d\n", num);
    for (int i = 0; i < num; i++) {
        dequeue(&(player->pokemons), fd);
    }
    fprintf(fd, "\n");
    fprintf(fd, "%d %d\n\n", player->item[0], player->item[1]);
    fprintf(fd, "%d", player->money);
    fclose(fd);
}
