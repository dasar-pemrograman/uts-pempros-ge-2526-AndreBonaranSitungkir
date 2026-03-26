#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 1000
#define MAX_LOGS 10000

typedef struct {
    char id[50];
    char name[100];
    int quantity;
    double price;
} Item;

typedef struct {
    int log_id;
    char item_id[50];
    char type[20];
    int quantity;
} Log;

int main() {
    Item inventory[MAX_ITEMS];
    Log logs[MAX_LOGS];
    
    int item_count=0;
    int log_count=0;
    int global_log_id=1;
    
    char line[256];

    while (fgets(line, sizeof(line), stdin)){
        line[strcspn(line, "\r\n")]=0;
        if (strcmp(line, "---")==0){
            break;
        }

        char *cmd=strtok(line, "#");
        if (cmd==NULL)continue;

        if (strcmp(cmd, "receive")==0){
            char *id=strtok(NULL, "#");
            char *name=strtok(NULL, "#");
            char *qty_str=strtok(NULL, "#");
            char *price_str=strtok(NULL, "#");

            if (id && name && qty_str && price_str){
                int is_duplicate=0;
                for (int i = 0; i < item_count; i++){
                    if (strcmp(inventory[i].id, id)==0){
                        is_duplicate=1;
                        break;
                    }
                }

                if (!is_duplicate && item_count < MAX_ITEMS){
                    strcpy(inventory[item_count].id, id);
                    strcpy(inventory[item_count].name, name);
                    inventory[item_count].quantity=atoi(qty_str);
                    inventory[item_count].price=atof(price_str);
                    item_count++;

                    if (log_count < MAX_LOGS){
                        logs[log_count].log_id=global_log_id++;
                        strcpy(logs[log_count].item_id, id);
                        strcpy(logs[log_count].type, "receive");
                        logs[log_count].quantity=atoi(qty_str);
                        log_count++;
                    }
                }
            }
        } 
        else if (strcmp(cmd, "ship")==0){
            char *id=strtok(NULL, "#");
            char *qty_str=strtok(NULL, "#");

            if (id && qty_str){
                int qty=atoi(qty_str);
                if (qty>0){
                    for (int i = 0; i < item_count; i++){
                        if (strcmp(inventory[i].id, id)==0){
                            if (inventory[i].quantity>=qty){
                                inventory[i].quantity-=qty;

                                if (log_count<MAX_LOGS) {
                                    logs[log_count].log_id=global_log_id++;
                                    strcpy(logs[log_count].item_id, id);
                                    strcpy(logs[log_count].type, "ship");
                                    logs[log_count].quantity=qty;
                                    log_count++;
                                }
                            }
                            break;
                        }
                    }
                }
            }
        } 
        else if (strcmp(cmd, "restock")==0){
            char *id=strtok(NULL, "#");
            char *qty_str=strtok(NULL, "#");

            if (id && qty_str){
                int qty=atoi(qty_str);
                if (qty > 0){
                    for (int i = 0; i < item_count; i++){
                        if (strcmp(inventory[i].id, id)==0){
                            inventory[i].quantity += qty;
                            if (log_count<MAX_LOGS){
                                logs[log_count].log_id=global_log_id++;
                                strcpy(logs[log_count].item_id, id);
                                strcpy(logs[log_count].type, "restock");
                                logs[log_count].quantity=qty;
                                log_count++;
                            }
                            break;
                        }
                    }
                }
            }
        } 
        else if (strcmp(cmd, "report")==0){
            for (int i = 0; i < item_count; i++){
                printf("%s|%s|%d|%.1f\n", inventory[i].id, inventory[i].name, inventory[i].quantity, inventory[i].price);
            }
        }
        else if (strcmp(cmd, "audit")==0){
            char *id=strtok(NULL, "#");
            if (id){
                int found=0;
                for (int i = 0; i < item_count; i++){
                    if (strcmp(inventory[i].id, id)==0){
                        printf("%s|%s|%d|%.1f\n", inventory[i].id, inventory[i].name, inventory[i].quantity, inventory[i].price);
                        found=1;
                        break;
                    }
                }

                if (found){
                    for (int i = 0; i < log_count; i++){
                        if (strcmp(logs[i].item_id, id)==0){
                            printf("%d|%s|%d\n", logs[i].log_id, logs[i].type, logs[i].quantity);
                        }
                    }
                }
            }
        }
    }

    return 0;
}