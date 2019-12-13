/*
 * Projeto: BreakOut: uma odisseia no espaço
 * Autores: Lucas Costa e Júlia Antines
 * build: final
 */

//#define TAM 10
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <time.h> 

using namespace std;

//Objetos da tela

class Textura {
public:
    //Image
    SDL_Texture* textura;
    //Dimensions and position
    SDL_Rect posicao;

};

class Player : public Textura {
private:
    int velX;
public:
    //Load Texture from file

    bool loadTExturePlayer(SDL_Renderer * gRenderer, char * path) {
        //The final texture
        SDL_Texture* newTexture = NULL;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load(path);
        if (loadedSurface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        } else {
            //Color key image
            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
            if (newTexture == NULL) {
                printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
            } else {
                //Get image dimensions

                posicao.w = loadedSurface->w;
                posicao.h = loadedSurface->h;

            }

            //Get rid of old loaded surface
            SDL_FreeSurface(loadedSurface);
        }

        //Return success
        textura = newTexture;
        return textura != NULL;
    }

    //get e set VelX (int & void)

    int getvelX() {
        return velX;
    }

    void setvelX(int x) {
        velX = x;
    }
};

class Bola : public Textura {
private:
    int velX;
    int velY;
public:
    //Load Texture from file

    bool loadTExtureBola(SDL_Renderer * gRenderer, char * path) {
        //The final texture
        SDL_Texture* newTexture = NULL;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load(path);
        if (loadedSurface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        } else {
            //Color key image
            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
            if (newTexture == NULL) {
                printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
            } else {
                //Get image dimensions

                posicao.w = loadedSurface->w;
                posicao.h = loadedSurface->h;

            }

            //Get rid of old loaded surface
            SDL_FreeSurface(loadedSurface);
        }

        //Return success
        textura = newTexture;
        return textura != NULL;
    }

    //get e set VelX (int & void)

    int getvelX() {
        return velX;
    }

    void setvelX(int x) {
        velX = x;
    }
    //get e set VelY (int & void)

    int getvelY() {
        return velY;
    }

    void setvelY(int y) {
        velY = y;
    }
};

class Tabuleiro {
private:
    //dimensoes da tela
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    //janela
    SDL_Window* gWindow = NULL;

    //renderer
    SDL_Renderer* gRenderer = NULL;

    //objetos do jogo
    Player player; //jogador
    Bola meteoro; //bola
    Textura obstaculos[6][8]; //obstaculos
    Textura vida[3];

    //background
    Textura fundo;
    Textura terra;

    //menus do jogo
    Textura MenuPrincipal;
    Textura telagameover;

    //sons do jogo
    Mix_Music *principal; //musica que toca durante o jogo
    Mix_Music *efeito; //efeito especial das colisões
    Mix_Music *musicaMenu; //musica que toca durante o Menu
    
    //retorna a música correspondente do parametro
    bool Sons(int x) {
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            return false;
            cout << "Erro ao Iniciar funcao de audio\n";
        } else {
            Mix_OpenAudio(48100, AUDIO_S16SYS, 1, 512);
            principal = Mix_LoadMUS("sons/David Bowie  Life On Mars Official Video.mp3");
            efeito = Mix_LoadMUS("sons/Explosao.mp3");
            musicaMenu = Mix_LoadMUS("sons/Crazy For You.mp3");
        }

        if (x == 0) {
            Mix_PlayMusic(principal, -1);
        }
        if (x == 1) {
            Mix_PlayMusic(efeito, -1);
        }
        if (x == 2) {
            Mix_PlayMusic(musicaMenu, -1);
        }
    }


    //Load Texture from file
    bool loadTExtureFromFile(Textura* textura, char * path) {
        //The final texture
        SDL_Texture* newTexture = NULL;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load(path);
        if (loadedSurface == NULL) {
            cout << "Unable to load image" << path << "! SDL_image Error: " << IMG_GetError();
        } else {
            //Color key image
            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
            if (newTexture == NULL) {
                cout << "Unable to create texture from" << path << "! SDL_image Error: " << IMG_GetError();
            } else {
                //Get image dimensions

                textura->posicao.w = loadedSurface->w;
                textura->posicao.h = loadedSurface->h;

            }

            //Get rid of old loaded surface
            SDL_FreeSurface(loadedSurface);
        }

        //Return success
        textura->textura = newTexture;
        return textura->textura != NULL;
    }

    //lida com as interações do usuário
    void trataEventos(Player * player, bool * quit, bool * start) {
        //Event handler
        SDL_Event e;
        //Handle events on queue   


        while (SDL_PollEvent(&e) != 0) {
            //User requests quit

            if (e.type == SDL_QUIT) {
                *quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                        player->setvelX(-3);
                        break;

                    case SDLK_RIGHT:
                        player->setvelX(3);
                        break;
                    case SDLK_ESCAPE:
                        *quit = true;
                        break;

                    case SDLK_SPACE:
                        *start = true;
                        Sons(2);
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                        player->setvelX(0);
                        break;

                    case SDLK_RIGHT:
                        player->setvelX(0);
                        break;
                }
            }
        }

    }

    //checa a colisao entre 2 objetos do tipo textura
    bool checaColisao(SDL_Rect a, SDL_Rect b) {
        bool sucesso;

        if ((a.y <= b.y + a.h && a.y >= b.y - a.h)&&(a.x <= b.x + a.w && a.x >= b.x - a.w)) {
            return true;
            cout << "colide" << endl;
        } else {
            return false;
        }
    }

    //set as posições iniciais de todos os objetos
    void MontaTabuleiro() {
        int i, j;

        //posicao do background
        fundo.posicao.x = 0;
        fundo.posicao.y = 0;

        //posicao da terra
        terra.posicao.x = -800;
        terra.posicao.y = 300;

        //posicao do player
        player.posicao.x = 255;
        player.posicao.y = 410;
        player.setvelX(0);

        //posicao do meteoro
        meteoro.posicao.x = 290;
        meteoro.posicao.y = 350;

        meteoro.setvelX(2);
        meteoro.setvelY(2);

        //posicao dos obstaculos
        obstaculos[0][0].posicao.x = 100;
        obstaculos[0][0].posicao.y = 100;

        //posicao dos menus
        MenuPrincipal.posicao.x = 0;
        MenuPrincipal.posicao.y = 0;

        //posicao tela gameover
        telagameover.posicao.x = 0;
        telagameover.posicao.y = 0;

        //posicao obstaculos
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 8; j++) {
                obstaculos[i][j].posicao.x = 50 + 75 * j;
                obstaculos[i][j].posicao.y = 10 + 50 * i;
            }
        }
        //posicoes vida
        for (j = 0; j < 3; j++) {
            vida[j].posicao.x = 520 + j * 30;
            vida[j].posicao.y = 420;
        }
    }
public:
    
    //inicia SDL e cria janela
    bool init() {
        //Initialization flag
        bool success = true;

        //Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Set texture filtering to linear
            if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
                printf("Warning: Linear texture filtering not enabled!");
            }

            //Create window
            gWindow = SDL_CreateWindow("BreakOut: Uma odisseia no espaço", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            if (gWindow == NULL) {
                printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                //Create vsynced renderer for window
                gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                if (gRenderer == NULL) {
                    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                    success = false;
                } else {
                    //Initialize renderer color
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                    //Initialize PNG loading
                    int imgFlags = IMG_INIT_PNG;
                    if (!(IMG_Init(imgFlags) & imgFlags)) {
                        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                        success = false;
                    }
                }
            }
        }

        return success;
    }

    //Loads media
    bool loadMedia() {
        //Loading success flag
        bool success = true;
        int i, j, aux;

        //carregar o fundo do jogo
        if (!loadTExtureFromFile(&fundo, "sprites/background.png")) {
            printf("Failed to load arrow texture!\n");
            success = false;
        }

        for (i = 0; i < 3; i++) {
            //carregar as barras de vida do jogo
            if (!loadTExtureFromFile(&vida[i], "sprites/vida.png")) {
                printf("Failed to load arrow texture!\n");
                success = false;
            }
        }
        //carregar a terra do jogo
        if (!loadTExtureFromFile(&terra, "sprites/terra.png")) {
            printf("Failed to load arrow texture!\n");
            success = false;
        }

        //carregar o player
        player.loadTExturePlayer(gRenderer, "sprites/barra.png");

        //carregar o player
        meteoro.loadTExtureBola(gRenderer, "sprites/meteoro.png");

        //carrega Menu principal
        if (!loadTExtureFromFile(&MenuPrincipal, "sprites/Main.png")) {
            printf("Failed to load arrow texture!\n");
            success = false;
        }

        //carrega tela de game over
        if (!loadTExtureFromFile(&telagameover, "sprites/gameover.png")) {
            printf("Failed to load arrow texture!\n");
            success = false;
        }

        //carrega obstaculos
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 8; j++) {
                aux = rand() % 10;
                if (aux == 1) {
                    if (!loadTExtureFromFile(&obstaculos[i][j], "sprites/planeta1.png")) {
                        printf("Failed to load arrow texture!\n");
                        success = false;
                    }
                }
                if (aux == 2) {
                    if (!loadTExtureFromFile(&obstaculos[i][j], "sprites/planeta2.png")) {
                        printf("Failed to load arrow texture!\n");
                        success = false;
                    }
                }
                if (aux == 3) {
                    if (!loadTExtureFromFile(&obstaculos[i][j], "sprites/planeta3.png")) {
                        printf("Failed to load arrow texture!\n");
                        success = false;
                    }
                }
                if (aux == 4) {
                    if (!loadTExtureFromFile(&obstaculos[i][j], "sprites/planeta4.png")) {
                        printf("Failed to load arrow texture!\n");
                        success = false;
                    }
                }
                if (aux == 5) {
                    if (!loadTExtureFromFile(&obstaculos[i][j], "sprites/planeta5.png")) {
                        printf("Failed to load arrow texture!\n");
                        success = false;
                    }
                }
                if (aux == 6) {
                    if (!loadTExtureFromFile(&obstaculos[i][j], "sprites/planeta6.png")) {
                        printf("Failed to load arrow texture!\n");
                        success = false;
                    }
                }
                if (aux == 7) {
                    if (!loadTExtureFromFile(&obstaculos[i][j], "sprites/planeta7.png")) {
                        printf("Failed to load arrow texture!\n");
                        success = false;
                    }
                }
                if (aux == 8) {
                    if (!loadTExtureFromFile(&obstaculos[i][j], "sprites/planeta8.png")) {
                        printf("Failed to load arrow texture!\n");
                        success = false;
                    }
                }
                if (aux == 9) {
                    if (!loadTExtureFromFile(&obstaculos[i][j], "sprites/planeta9.png")) {
                        printf("Failed to load arrow texture!\n");
                        success = false;
                    }
                }
                if (aux == 10) {
                    if (!loadTExtureFromFile(&obstaculos[i][j], "sprites/planeta10.png")) {
                        printf("Failed to load arrow texture!\n");
                        success = false;
                    }
                }
                if (aux == 0) {
                    if (!loadTExtureFromFile(&obstaculos[i][j], "sprites/planeta10.png")) {
                        printf("Failed to load arrow texture!\n");
                        success = false;
                    }
                }
            }
        }

        //dimensões da bola
        meteoro.posicao.w = 30;
        meteoro.posicao.h = 30;
        //dimensões do player
        player.posicao.w = 130;
        player.posicao.h = 40;
        //dimensões dos obstaculos
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 8; j++) {
                obstaculos[i][j].posicao.w = 30;
                obstaculos[i][j].posicao.h = 30;
            }
        }
        //dimensões do fundo
        fundo.posicao.w = 640;
        fundo.posicao.h = 480;

        //dimensões da tela de gameover
        telagameover.posicao.w = 640;
        telagameover.posicao.h = 480;

        //dimensões da terra
        terra.posicao.w = 1900;
        terra.posicao.h = 1500;

        //dimensões da vida
        for (j = 0; j < 3; j++) {
            vida[j].posicao.w = 30;
            vida[j].posicao.h = 30;
        }

        //dimensões dos menus
        MenuPrincipal.posicao.w = 640;
        MenuPrincipal.posicao.h = 480;

        return success;
    }
    
    //loop do jogo
    bool gameLoop() {
        //Teste de saida do jogo
        bool quit = false;
        //controla a saida e entrada no menu
        bool quitMenu = false;
        bool quitRegras = true;
        //controla se o game acabou
        bool quitGameover = false;
        //variaveis auxiliares
        int i, j, aux = 0, opcao = 1;
        float aux2 = 0;
        //guarda o numero de obstaculos
        int teste[6][8];
        //conta o numero de vidas
        int vidaCont = 3;
        int contaObstaculos = 0;


        for (i = 0; i < 6; i++) {
            for (j = 0; j < 8; j++) {
                teste[i][j] = 0;
                contaObstaculos++;
            }
        }

        MontaTabuleiro();

        //inicia a musica do menu
        Sons(0);

        //While application is running
        while (!quit) {

            trataEventos(&player, &quit, &quitMenu);

            cout << quitMenu;

            if (quitMenu == false) {
                MontaTabuleiro();
            }

            if (quitRegras == false) {
                vidaCont = 3;
                MontaTabuleiro();
                quitRegras = quitMenu;
            }

            aux += 1;
            aux2 += 0.2;

            //checa se ocorreu colisao entre o player e a bola
            if (checaColisao(player.posicao, meteoro.posicao) == true) {
                meteoro.setvelY(-2);
                if (player.getvelX() > 0) {
                    meteoro.setvelX(3);
                }
                if (player.getvelX() < 0) {
                    meteoro.setvelX(-3);
                }
            }

            //checa se a bola passa do limite inferior
            if (meteoro.posicao.y > 400) {
                vidaCont--;
                Sons(1);
                SDL_DestroyTexture(vida[vidaCont].textura);
                SDL_Delay(1700);
                MontaTabuleiro();
                Sons(2);
            }

            //checa vidas
            if (vidaCont <= 0 || contaObstaculos <= 0) {
                quitMenu = false;
                quitRegras = false;
            }

            //checa se ocorreu colisao entre a bola e o obstaculo
            for (i = 0; i < 6; i++) {
                for (j = 0; j < 8; j++) {
                    if (checaColisao(meteoro.posicao, obstaculos[i][j].posicao) == true && teste[i][j] == 0) {
                        //Sons(1);
                        teste[i][j] = 1;
                        contaObstaculos--;
                        SDL_DestroyTexture(obstaculos[i][j].textura);
                        meteoro.setvelY(meteoro.getvelY()*-1);
                        meteoro.setvelX(meteoro.getvelX()*-1);
                        if (player.getvelX() > 0) {
                            meteoro.setvelX(2);
                        } else if (player.getvelX() < 0) {
                            meteoro.setvelX(-2);
                        }
                    }
                }
            }

            if (meteoro.posicao.y > 405) {
                //quit = true;
            }

            //Atualiza o mundo
            player.posicao.x += player.getvelX();

            meteoro.posicao.x += meteoro.getvelX();
            meteoro.posicao.y += meteoro.getvelY();

            //repeticao player 

            if (player.posicao.x >= 510) {
                player.posicao.x = 510;
            }
            if (player.posicao.x <= 0) {
                player.posicao.x = 0;
            }

            //repeticao bola 

            if (meteoro.posicao.x > 620) {
                meteoro.posicao.x = 620;
                meteoro.setvelX(-2);
            }
            if (meteoro.posicao.x < 0) {
                meteoro.posicao.x = 0;
                meteoro.setvelX(2);
            }
            if (meteoro.posicao.y > 460) {
                meteoro.posicao.y = 460;
                meteoro.setvelX(-2);
            }
            if (meteoro.posicao.y < 0) {
                meteoro.posicao.y = 0;
                meteoro.setvelX(2);
            }

            //Redesenha
            //Clear screen
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);


            // Clear the entire screen to our selected color.
            SDL_RenderClear(gRenderer);


            //Render fundo                
            SDL_RenderCopyEx(gRenderer, fundo.textura, NULL, &(fundo.posicao), 0, NULL, SDL_FLIP_NONE);

            //Render terra                
            SDL_RenderCopyEx(gRenderer, terra.textura, NULL, &(terra.posicao), aux2, NULL, SDL_FLIP_NONE);

            for (j = 0; j < 3; j++) {
                //Render vida               
                SDL_RenderCopyEx(gRenderer, vida[j].textura, NULL, &(vida[j].posicao), aux, NULL, SDL_FLIP_NONE);
            }

            //Render player                
            SDL_RenderCopyEx(gRenderer, player.textura, NULL, &(player.posicao), 0, NULL, SDL_FLIP_NONE);

            //Render bola
            if (meteoro.getvelX() > 0 && meteoro.getvelY() > 0) {
                SDL_RenderCopyEx(gRenderer, meteoro.textura, NULL, &(meteoro.posicao), 0, NULL, SDL_FLIP_HORIZONTAL);
            } else if (meteoro.getvelX() < 0 && meteoro.getvelY() > 0) {
                SDL_RenderCopyEx(gRenderer, meteoro.textura, NULL, &(meteoro.posicao), 0, NULL, SDL_FLIP_NONE);
            } else if (meteoro.getvelX() > 0 && meteoro.getvelY() < 0) {
                SDL_RenderCopyEx(gRenderer, meteoro.textura, NULL, &(meteoro.posicao), 250, NULL, SDL_FLIP_HORIZONTAL);
            } else if (meteoro.getvelX() < 0 && meteoro.getvelY() < 0) {
                SDL_RenderCopyEx(gRenderer, meteoro.textura, NULL, &(meteoro.posicao), 90, NULL, SDL_FLIP_NONE);
            }


            for (i = 0; i < 6; i++) {
                for (j = 0; j < 8; j++) {
                    //Render osbtaculos                
                    SDL_RenderCopyEx(gRenderer, obstaculos[i][j].textura, NULL, &(obstaculos[i][j].posicao), aux, NULL, SDL_FLIP_NONE);
                }
            }

            if (quitMenu == false) {
                //Render gameover               
                SDL_RenderCopyEx(gRenderer, telagameover.textura, NULL, &(telagameover.posicao), 0, NULL, SDL_FLIP_NONE);
            }

            if (quitMenu == false && quitRegras == true) {
                //Render Menu                
                SDL_RenderCopyEx(gRenderer, MenuPrincipal.textura, NULL, &(MenuPrincipal.posicao), 0, NULL, SDL_FLIP_NONE);
            }

            //Update screen
            SDL_RenderPresent(gRenderer);
        }
    }

    //Finaliza SDL
    void close() {
        int i, j;

        //Free loaded player image
        SDL_DestroyTexture(player.textura);

        //Free loaded Menus images
        SDL_DestroyTexture(MenuPrincipal.textura);

        //Free loaded gameover images
        SDL_DestroyTexture(telagameover.textura);

        //Free loaded terra image
        SDL_DestroyTexture(terra.textura);

        //Free loaded bola image
        SDL_DestroyTexture(meteoro.textura);

        //Free loaded fundo image
        SDL_DestroyTexture(fundo.textura);

        for (i = 0; i < 6; i++) {
            for (j = 0; j < 8; j++) {
                //Free loaded obstaculo image
                SDL_DestroyTexture(obstaculos[i][j].textura);
            }
        }

        for (j = 0; j < 8; j++) {
            //Free loaded vida image
            SDL_DestroyTexture(vida[j].textura);
        }

        //Destroy window	
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
        gRenderer = NULL;

        //Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }
};

//Principal
int main(int argc, char* args[]) {
    Tabuleiro BreakOut;

    //Start up SDL and create window
    if (!BreakOut.init()) {
        cout << "Failed to initialize!";
    } else {
        //Load media
        if (!BreakOut.loadMedia()) {
            cout << "Failed to load media!";
        } else {
            BreakOut.gameLoop();
        }
    }

    //Free resources and close SDL
    BreakOut.close();

    return 0;
}