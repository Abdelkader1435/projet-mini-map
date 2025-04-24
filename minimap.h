// Empêche l'inclusion multiple de ce fichier d'en-tête pour éviter les erreurs de redéfinition
#ifndef MINIMAP_H_
#define MINIMAP_H_

// Inclusion des bibliothèques SDL nécessaires pour les graphismes, le chargement d'images et l'audio
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
// Inclusion des bibliothèques C standard pour l'entrée/sortie, la gestion de mémoire et les types booléens
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Définition des constantes pour les dimensions de la minicarte
#define MINIMAP_WIDTH 400  // Largeur de la minicarte en pixels
#define MINIMAP_HEIGHT 300 // Hauteur de la minicarte en pixels
// Définition des dimensions de la zone de jeu
#define GAME_WIDTH 1200    // Largeur de la zone principale du jeu
#define GAME_HEIGHT 800    // Hauteur de la zone principale du jeu
// Calcul des dimensions de l'écran pour inclure le jeu et la minicarte avec un espacement
#define SCREEN_WIDTH (GAME_WIDTH + MINIMAP_WIDTH + 20) // Largeur totale de l'écran (jeu + minicarte + 20px d'espacement)
#define SCREEN_HEIGHT (GAME_HEIGHT > MINIMAP_HEIGHT ? GAME_HEIGHT : MINIMAP_HEIGHT) + 20 // Hauteur totale de l'écran (maximum entre jeu/minicarte + 20px)
#define PLAYER_SIZE 32     // Taille du sprite du joueur (carré)
#define PLAYER_SPEED 5     // Vitesse de déplacement du joueur en pixels par image
// Définition du taux de rafraîchissement et du temps par image
#define FPS 60             // Images par seconde ciblées
#define FRAME_TIME (1000 / FPS) // Temps par image en millisecondes (1000ms / 60 = ~16.67ms)

// Macros utilitaires pour limiter les valeurs
#define MAX(a,b) ((a) > (b) ? (a) : (b)) // Retourne la valeur maximale entre a et b
#define MIN(a,b) ((a) < (b) ? (a) : (b)) // Retourne la valeur minimale entre a et b
#define CLAMP(x, low, high) (MIN(high, MAX(x, low))) // Limite x entre low et high

// Structure pour représenter une image avec sa position à l'écran et son rectangle source
typedef struct struct_img {
  SDL_Surface* image;          // Pointeur vers la surface SDL contenant l'image
  SDL_Rect positioninscrenn;    // Rectangle définissant la position à l'écran
  SDL_Rect positionfromimage;   // Rectangle définissant la zone source de l'image
} struct_img;

// Structure pour représenter le personnage joueur (Personne)
typedef struct Personne {
  struct_img perso;            // Données d'image et de position du joueur
  int direction;               // Direction actuelle du mouvement (0 = droite, 1 = gauche)
  int deplacement;            // État du mouvement (1 = en mouvement, 0 = immobile)
  double vitesse;              // Vitesse de déplacement (pixels par milliseconde)
  Uint32 dt;                   // Temps écoulé pour les calculs de mouvement
  int up;                     // État du saut (1 = en saut, 0 = pas en saut)
  int velocityY;               // Vélocité verticale (pour les sauts/chutes)
  int velocityX;               // Vélocité horizontale
  int groundY;                // Coordonnée Y du sol
  int tabcollison[4];         // Drapeaux de collision (0 = droite, 1 = gauche, 2 = haut, 3 = bas)
} Personne;

// Structure pour représenter l'arrière-plan du jeu
typedef struct background {
  struct_img background;       // Données d'image et de position de l'arrière-plan
  struct_img masque;           // Données d'image et de position du masque de collision
  int stage;                   // Numéro du niveau ou de l'étape
} background;

// Structure pour représenter la minicarte
typedef struct {
  SDL_Surface *backg_img;      // Image d'arrière-plan de la minicarte
  SDL_Rect pos_minimap;        // Position de la minicarte à l'écran
  SDL_Surface *player_img;     // Image du marqueur du joueur sur la minicarte
  SDL_Rect pos_player;         // Position du marqueur du joueur sur la minicarte
  float scaleX;                // Facteur de mise à l'échelle horizontale
  float scaleY;                // Facteur de mise à l'échelle verticale
  int level;                   // Numéro du niveau actuel
} MiniMap;

// Structure pour représenter le joueur dans le jeu principal
typedef struct {
  SDL_Surface *sprite;         // Image du sprite du joueur
  SDL_Rect position;           // Position du joueur à l'écran
  SDL_Rect velocity;           // Vélocité du joueur (composantes x, y)
  bool isMoving;               // Drapeau indiquant si le joueur bouge
} Player;

// Structure pour représenter une plateforme dans le jeu
typedef struct {
  SDL_Surface *image;          // Image de la plateforme
  SDL_Rect rect;               // Position et taille de la plateforme
  bool isDestructible;         // Drapeau indiquant si la plateforme est destructible
  bool isMobile;               // Drapeau indiquant si la plateforme est mobile
  bool isDestroyed;            // Drapeau indiquant si la plateforme est détruite
} Platform;

// Structure pour représenter un niveau du jeu
typedef struct {
  SDL_Surface *background;     // Image d'arrière-plan du niveau
  SDL_Surface *minimap_bg;     // Image d'arrière-plan pour la minicarte
  Platform *platforms;         // Tableau des plateformes du niveau
  int platformCount;           // Nombre de plateformes
  SDL_Rect bounds;             // Limites du niveau
} Level;

// Prototypes des fonctions pour l'initialisation et la gestion du jeu
void initSDL();                // Initialise les sous-systèmes SDL
Player initPlayer();           // Initialise le joueur
Level initLevel(int levelNumber); // Initialise un niveau avec le numéro donné
MiniMap initMinimap();         // Initialise la minicarte
void changeLevel(int newLevel, Player *player, Level *level, MiniMap *minimap); // Change de niveau
void handleContinuousInput(Player *player); // Gère les entrées clavier continues
void updatePlayer(Player *player, Level *level); // Met à jour la position et l'état du joueur
void updateMinimap(MiniMap *minimap, Player *player, Level *level); // Met à jour l'affichage de la minicarte
void drawGame(SDL_Surface *screen, Player *player, Level *level, MiniMap *minimap); // Dessine le jeu
void drawMinimap(SDL_Surface *screen, MiniMap *minimap, Player *player, Level *level); // Dessine la minicarte
bool checkBBCollision(SDL_Rect *rect1, SDL_Rect *rect2); // Vérifie les collisions par boîtes englobantes
void cleanupResources(Player *player, Level *level, MiniMap *minimap); // Libère les ressources
void cleanup(Player *player, Level *level, MiniMap *minimap); // Nettoie les ressources du jeu

// Prototypes des fonctions pour les mécaniques supplémentaires du jeu
void init_background(struct_img *background); // Initialise l'arrière-plan
void init_minimap(MiniMap *m, int level);    // Initialise la minicarte pour un niveau
void MAJMinimap(SDL_Rect posJoueur, MiniMap *m, SDL_Rect camera, int redimensionnement); // Met à jour la position du joueur sur la minicarte
void afficher_minimap(MiniMap m, SDL_Surface *screen, SDL_Rect camera, int *i); // Affiche la minicarte
void afficher_animation(MiniMap m, SDL_Surface *screen, SDL_Rect camera, int *i); // Affiche l'animation de la minicarte
void scrolling(SDL_Rect *camera, SDL_Event event); // Gère le défilement de la caméra
int collisionPP(Personne *p, SDL_Surface *Masque, background bp); // Vérifie les collisions pixel-par-pixel
void animerMinimap(MiniMap *m);              // Anime la minicarte
void init_maske(struct_img *maske, int level); // Initialise le masque de collision
SDL_Color GetPixel(SDL_Surface *pSurface, int x, int y); // Obtient la couleur d'un pixel à (x, y)
void savedata(SDL_Rect posjoueur, SDL_Rect camera, char fich[]); // Sauvegarde les données du jeu
void loaddata(SDL_Rect *posjoueur, SDL_Rect *camera, char fich[]); // Charge les données du jeu
void initPerso(Personne *p);                 // Initialise le personnage joueur
void init(Personne *p, int numperso);        // Initialise un personnage avec un numéro
void afficherPerso(Personne p, SDL_Surface *screen); // Affiche le personnage joueur
void movePerso(Personne *p, background *bp); // Déplace le personnage joueur
void animerPerso(Personne *p);               // Anime le personnage joueur
void saut(Personne *p, float dt, int possol, background *bp); // Gère le saut du joueur
void handleDestructibleCollisions(Personne *p, Level *level); // Gère les collisions avec les plateformes destructibles

// Fin de la garde d'inclusion
#endif /* MINIMAP_H_ */
