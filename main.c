// Copyright 2018 Simionescu Ana-Maria
#include <stdio.h>

// Verific daca indecsii apartin tablei de joc
int invalid_index(char n, int x, int y) {
    if (x < 0 || x >= n*n || y < 0 || y >= n*n) {
        return 1;
    }
    return 0;
}

// Daca celula contine caracterul '-' este libera
int not_an_empty_cell(char a[101][101], int x, int y) {
    if (a[x][y] != '-') {
        return 1;
    }
    return 0;
}

// Verific pe rand erorile posibile
int mutare_valida(int n, char nume, int x, int y, char a[101][101],
                                             int *p_turn) {
    if ((nume == 'X' && *p_turn == 0)  || (nume == '0' && *p_turn == 1)) {
        return 3;
    }
    if (invalid_index(n, x, y) == 1) {
        return 1;
    }
    if (not_an_empty_cell(a, x, y) == 1) {
        return 2;
    }
    return 0;
}

//   Dupa fiecare atribuire verific daca in microbordul corespunzator
// s-a obtinut un castigator. Voi face verificarea doar pentru jucatorul
// al carui rand tocmai s-a finalizat
int verificare_castig(char nume, int x, int y, int n, char a[101][101]) {
    int l, c, i, j;
    // Coordonatele coltului stanga sus al microbordului
    l = (x/n) * n;
    c = (y/n) * n;
    int ok;
    //   Parcurg fiecare linie pentru a verifica daca contine doar caracterul
    // jucatorului al carui rand a fost
    for (i = l; i < l+n; i++) {
        ok = 1;
        for (j = c; j < c+n; j++) {
            if (a[i][j] != nume) {
                ok = 0;
            }
        }
        if (ok == 1) {
            return 1;
        }
    }
    //   Parcurg fiecare coloana pentru a verifica daca contine doar caracterul
    // jucatorului al carui rand a fost
    for (j = c; j < c+n; j++) {
        ok = 1;
        for (i = l; i < l+n; i++) {
            if (a[i][j] != nume) {
                ok = 0;
            }
        }
        if (ok == 1) {
            return 1;
        }
    }
    //   Parcurg diagonala principala pentru a verifica daca contine
    // doar caracterul jucatorului al carui rand a fost
    ok = 1;
    j = c;
    for (i = l; i < l+n; i++, j++) {
        if (a[i][j] != nume) {
            ok = 0;
        }
    }
    if (ok == 1) {
        return 1;
    }
    //   Parcurg diagonala secundara pentru a verifica daca contine doar
    // caracterul jucatorului al carui rand a fost
    i = l;
    ok = 1;
    for (j = c+n-1; j >= c; j--, i++) {
        if (a[i][j] != nume) {
            ok = 0;
        }
    }
    if (ok == 1) {
        return 1;
    }
    return 0;
    // Returnez 1 daca s-a obtinut un castigator sau 0 in caz contrar
}

//   In cazul unei erori, aceasta functie va distribui automat o celula
// jucatorului la rand. Tot in cadrul ei decid daca tabla de joc este plina.
void atribuire_automata(char a[101][101], int *p_turn, int n,
                            char macro[101][101]) {
    int full_board = 0;
    int i, j;

    for (i = 0; i < n*n; i++) {
        // Prin urmatorul for parcurg diagonalele de deasupra celei principale
        for (j = 0; j < n*n-i; j++) {
            if (a[j][i+j] == '-') {
                full_board = 1; // Daca se gaseste o celula libera, atunci
                // tabla de joc nu este plina
                if (*p_turn == 1) {
                    a[j][i+j] = 'X';
                    //   Verific daca s-a obtinut un castigator prin aceasta
                    // atribuire
                    if (verificare_castig('X', j, i+j, n, a) == 1 &&
                        macro[j/n][(i+j)/n] == '-') {
                    //   Daca s-a obtinut castigatorul pun in macroboard
                    // caracterul jucatorului la rand
                         macro[j/n][(i+j)/n] = 'X';
                     }
                } else {
                    a[j][i+j] = '0';
                    if (verificare_castig('0', j, i+j, n, a) == 1 &&
                        macro[j/n][(i+j)/n] == '-') {
                         macro[j/n][(i+j)/n] = '0';
                     }
                }
                return;
            }
        }
        // Prin urmatorul for parcurg diagonalele de sub cea principala
        for (j = 0; j < n*n-i; j++) {
            if (a[i+j][j] == '-') {
                full_board = 1;
                if (*p_turn == 1) {
                    a[i+j][j] = 'X';
                    if (verificare_castig('X', i+j, j, n, a) == 1 &&
                        macro[(i+j)/n][j/n] == '-') {
                         macro[(i+j)/n][j/n] = 'X';
                     }
                } else {
                    a[i+j][j] = '0';
                    if (verificare_castig('0', i+j, j, n, a) == 1 &&
                        macro[(i+j)/n][j/n] == '-') {
                         macro[(i+j)/n][j/n] = '0';
                     }
                }
                return;
            }
        }
    }
    //   Daca la finalul parcurgerilor nu s-a gasit o celula libera atunci tabla
    // este plina si afisez mesajul corespunzator.
    if (full_board == 0) {
        printf("FULL BOARD\n");
        *p_turn = -1;
    }
}

//   Daca o mutare nu are erori atunci se va realiza atribuirea caracterului
// corespunzator in microboard si se verifica obtinerea unui castigator.
// Daca s-a gasit castigatorul, numarul mutarilor sale castigatoare creste.
void atribuire(char nume, int x, int y, char a[101][101], int n,
                            char macro[101][101], double coef[2][2]) {
     a[x][y] = nume;
     int juc;

     if (nume == 'X') {
        juc = 0;
    } else {
        juc = 1;
    }

     if (verificare_castig(nume, x, y, n, a) == 1 &&
                     macro[x/n][y/n] == '-') {
         macro[x/n][y/n] = nume;
         coef[juc][0]++;
     }
}

//   In aceasta functie se citesc mutarile si se urmeaza instructiunile pentru
// fiecare tip de eroare. Se apeleaza functia de atribuire automata in cazul
// erorilor de index invalid sau celula ocupata, si functia de atribuire pentru
// o mutare valida, si se incrementeaza numarul total al mutarilor pentru
// jucatorul la rand
void mutare(int n, char a[101][101], int *p_turn, char macro[101][101],
                        double coef[2][2]) {
    int x, y, juc;
    char nume;

    scanf(" %c%d%d", &nume, &x, &y);

    if (nume == 'X') {
        juc = 0;
    } else {
        juc = 1;
    }

    if (*p_turn == 0 || *p_turn == 1) {
        switch (mutare_valida(n, nume, x, y, a, p_turn)) {
            case 1:
                printf("INVALID INDEX\n");
                atribuire_automata(a, p_turn, n, macro);
                coef[juc][1]++;
                break;
            case 2:
                printf("NOT AN EMPTY CELL\n");
                atribuire_automata(a, p_turn, n, macro);
                coef[juc][1]++;
                break;
            case 3:
                printf("NOT YOUR TURN\n");
                *p_turn = 1 - *p_turn; // Modificam acum randul jucatorului
                // pentru ca dupa modificarea din int main randul sa
                // ramana la fel
                break;
            case 0:
                atribuire(nume, x, y, a, n, macro, coef);
                coef[juc][1]++;
                break;
        }
    }
}

void initializare_matrice(char a[101][101], int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            a[i][j] = '-';
        }
    }
}

int main() {
    int n, m, i, j, turn, *p_turn;
    double coef[2][2]={ {0, 0}, {0, 0} };
    char a[101][101], macro[101][101];

    // Incepe jucatorul X
    turn = 1;
    p_turn = &turn;

    scanf("%d%d", &n, &m);
    // Se initializeaza microboard-urile si macroboardul cu caracterul '-'
    initializare_matrice(a, n*n);
    initializare_matrice(macro, n);

    // Se executa toate mutarile
    for (i = 1; i <= m; i++) {
        mutare(n, a, p_turn, macro, coef);
        turn = 1 - turn; // Actualizez randul jucatorilor
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%c", macro[i][j]);
        }
        printf("\n");
    }

    // Folosesc functia de verificare castigator si pe macroboard
    if (verificare_castig('X', 0, 0, n, macro) == 1 &&
            verificare_castig('0', 0, 0, n, macro) == 1) {
        printf("Draw again! Let's play darts!\n");
    } else {
        if (verificare_castig('0', 0, 0, n, macro) == 1) {
            printf("0 won\n");
        } else {
            if (verificare_castig('X', 0, 0, n, macro) == 1) {
                printf("X won\n");
            } else {
            printf("Draw again! Let's play darts!\n");
            }
        }
    }

    // Calculez si afisez coeficientul de atentie
    if (coef[0][1] != 0) {
        double coefx = coef[0][0] / coef[0][1];
        printf("X %.10lf\n", coefx);
    } else {
        printf("X N/A\n");
    }

    if (coef[1][1] != 0) {
        double coef0 = coef[1][0] / coef[1][1];
        printf("0 %.10lf\n", coef0);
    } else {
        printf("0 N/A\n");
    }

    return 0;
}
