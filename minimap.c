// Inclusion du fichier d'en-tête avec les déclarations
#include "minimap.h"
// Inclusion de string.h pour memcpy dans GetPixel
#include <string.h>

// Initialisation du personnage joueur (Personne)
void initPerso(Personne *p)
{
  p->up = 0;                        // Définit l'état de saut à non sauté
  p->velocityX = 40;                // Définit la vélocité horizontale initiale
  p->velocityY = -60;               // Définit la vélocité verticale initiale (négative pour monter)
  p->groundY = 40;                  // Définit la coordonnée Y du sol
  p->perso.image = IMG_Load("p.png"); // Charge l'image du sprite du joueur
  p->perso.positionfromimage.x = 0; // Coordonnée x du rectangle source
  p->perso.positionfromimage.y = 0; // Coordonnée y du rectangle source
  p->perso.positionfromimage.h = 47; // Hauteur du rectangle source
  p->perso.positionfromimage.w = 30; // Largeur du rectangle source
  p->perso.positioninscrenn.x = 200; // Position x à l'écran
  p->perso.positioninscrenn.y = 250; // Position y à l'écran
  p->direction = 0;                 // Définit la direction initiale à droite
  p->deplacement = 0;               // Définit l'état de mouvement à immobile
  p->tabcollison[0] = 1;            // Initialise le drapeau de collision pour la droite (bloqué)
  p->tabcollison[1] = 1;            // Initialise le drapeau de collision pour la gauche (bloqué)
  p->tabcollison[2] = 1;            // Initialise le drapeau de collision pour le haut (bloqué)
  p->tabcollison[3] = 1;            // Initialise le drapeau de collision pour le bas (bloqué)
}

// Affichage du personnage joueur à l'écran
void afficherPerso(Personne p, SDL_Surface *screen)
{
  SDL_BlitSurface(p.perso.image, &p.perso.positionfromimage, screen, &p.perso.positioninscrenn); // Copie le sprite du joueur à l'écran
}

// Initialisation de la minicarte pour un niveau spécifique
void init_minimap(MiniMap *m, int level)
{
  if (level == 1) {                 // Vérifie si le niveau est 1
    m->backg_img = IMG_Load("level1.2.png"); // Charge l'arrière-plan de la minicarte pour le niveau 1
    m->player_img = IMG_Load("pt rg.png");   // Charge l'image du marqueur du joueur
  } else {                          // Sinon (niveau 2)
    m->backg_img = IMG_Load("2.png");        // Charge l'arrière-plan de la minicarte pour le niveau 2
    m->player_img = IMG_Load("pt rg.png");   // Charge l'image du marqueur du joueur
  }
  m->pos_minimap.x = 0;             // Définit la position x de la minicarte à 0
  m->pos_minimap.y = 0;             // Définit la position y de la minicarte à 0
  m->pos_player.x = 0;              // Définit la position x du marqueur du joueur à 0
  m->pos_player.y = 0;              // Définit la position y du marqueur du joueur à 0
  m->level = level;                 // Stocke le numéro du niveau actuel
}

// Initialisation du masque de collision pour un niveau spécifique
void init_maske(struct_img *maske, int level)
{
  if (level == 1) {                 // Vérifie si le niveau est 1
    maske->image = IMG_Load("bg.png"); // Charge le masque de collision pour le niveau 1
  } else {                          // Sinon (niveau 2)
    maske->image = IMG_Load("bg2.png"); // Charge le masque de collision pour le niveau 2
  }
  maske->positionfromimage.x = 0;   // Définit la coordonnée x du rectangle source à 0
  maske->positionfromimage.y = 0;   // Définit la coordonnée y du rectangle source à 0
  maske->positionfromimage.h = 700; // Définit la hauteur du rectangle source
  maske->positionfromimage.w = 1300; // Définit la largeur du rectangle source
}

// Mise à jour de la position du joueur sur la minicarte
void MAJMinimap(SDL_Rect posJoueur, MiniMap *m, SDL_Rect camera, int redimensionnement)
{
  m->pos_player.x = ((posJoueur.x + camera.x) * 0.27); // Met à l'échelle la position x du joueur (27% de la position dans le jeu)
  m->pos_player.y = ((posJoueur.y + camera.y) * 0.2) - 30; // Met à l'échelle la position y du joueur (20% de la position dans le jeu, décalée de 30)
}

// Affichage de la minicarte à l'écran
void afficher_minimap(MiniMap m, SDL_Surface *screen, SDL_Rect camera, int *i)
{
  SDL_BlitSurface(m.backg_img, NULL, screen, &m.pos_minimap); // Copie l'arrière-plan de la minicarte à l'écran
  if ((*i) <= 5) {                  // Si le compteur d'animation est inférieur ou égal à 5
    SDL_BlitSurface(m.player_img, NULL, screen, &m.pos_player); // Copie le marqueur du joueur sur la minicarte
  }
  if ((*i) == 10) {                 // Si le compteur d'animation atteint 10
    *i = 0;                         // Réinitialise le compteur d'animation
  }
  (*i)++;                           // Incrémente le compteur d'animation
}

// Affichage de l'animation sur la minicarte (similaire à afficher_minimap)
void afficher_animation(MiniMap m, SDL_Surface *screen, SDL_Rect camera, int *i)
{
  if ((*i) <= 5) {                  // Si le compteur d'animation est inférieur ou égal à 5
    SDL_BlitSurface(m.player_img, NULL, screen, &m.pos_player); // Copie le marqueur du joueur sur la minicarte
  }
  if ((*i) == 10) {                 // Si le compteur d'animation atteint 10
    *i = 0;                         // Réinitialise le compteur d'animation
  }
  (*i)++;                           // Incrémente le compteur d'animation
}

// Gestion du défilement de la caméra en fonction des entrées clavier
void scrolling(SDL_Rect *camera, SDL_Event event)
{
  switch (event.key.keysym.sym) {   // Vérifie quelle touche a été pressée
    case SDLK_RIGHT:                // Touche flèche droite
      if (camera->x < 154) {        // Si la position x de la caméra est inférieure à 154 (limite)
        camera->x++;                // Déplace la caméra vers la droite
      }
      break;
    case SDLK_LEFT:                 // Touche flèche gauche
      if (camera->x > 0) {          // Si la position x de la caméra est supérieure à 0
        camera->x--;                // Déplace la caméra vers la gauche
      }
      break;
    case SDLK_UP:                   // Touche flèche haut
      camera->y = 0;                // Réinitialise la position y de la caméra à 0 (pas de défilement vertical)
      break;
  }
}

// Déplacement du personnage joueur et mise à jour du défilement de l'arrière-plan
void movePerso(Personne *p, background *bp)
{
  switch (p->direction) {           // Vérifie la direction du joueur
    case 0:                         // Déplacement vers la droite
      if (p->tabcollison[0] == 0) { // S'il n'y a pas de collision à droite
        if (p->perso.positioninscrenn.x > 300) // Si le joueur est au-delà de x=300
          bp->background.positionfromimage.x += p->vitesse * p->dt; // Défile l'arrière-plan vers la droite
        if (bp->background.positionfromimage.x == 0 || bp->background.positionfromimage.x > 2400) // Si l'arrière-plan est aux limites
          p->perso.positioninscrenn.x += p->vitesse * p->dt; // Déplace le joueur vers la droite
      }
      break;
    case 1:                         // Déplacement vers la gauche
      if (p->tabcollison[1] == 0) { // S'il n'y a pas de collision à gauche
        if (bp->background.positionfromimage.x == 0 || bp->background.positionfromimage.x >= 2400) // Si l'arrière-plan est aux limites
          p->perso.positioninscrenn.x -= p->vitesse * p->dt; // Déplace le joueur vers la gauche
        if (p->perso.positioninscrenn.x > 300 && p->perso.positioninscrenn.x < 310) // Si le joueur est dans la zone de défilement
          bp->background.positionfromimage.x -= p->vitesse * p->dt; // Défile l'arrière-plan vers la gauche
      }
      break;
  }
  if (bp->background.positionfromimage.x < 0) // Empêche le défilement de l'arrière-plan trop à gauche
    bp->background.positionfromimage.x = 0;
  if (bp->background.positionfromimage.x > 2400) // Empêche le défilement de l'arrière-plan trop à droite
    bp->background.positionfromimage.x = 2400;
}

// Obtention de la couleur d'un pixel à (x, y) d'une surface SDL
SDL_Color GetPixel(SDL_Surface *pSurface, int x, int y)
{
  SDL_Color color;                  // Variable pour stocker la couleur du pixel
  Uint32 col = 0;                   // Variable pour stocker les données brutes du pixel
  char* pPosition = (char*) pSurface->pixels; // Pointeur vers les données des pixels de la surface
  pPosition += (pSurface->pitch * y); // Déplace vers la bonne ligne (y * pitch)
  pPosition += (pSurface->format->BytesPerPixel * x); // Déplace vers la bonne colonne (x * octets par pixel)
  memcpy(&col, pPosition, pSurface->format->BytesPerPixel); // Copie les données du pixel dans col
  SDL_GetRGB(col, pSurface->format, &color.r, &color.g, &color.b); // Convertit les données du pixel en RGB
  return color;                     // Retourne la couleur
}

// Vérification des collisions pixel-par-pixel entre le joueur et le masque de collision
int collisionPP(Personne *p, SDL_Surface *Masque, background bp)
{
  int persowidth = 24;              // Largeur du joueur pour la collision
  int persoheght = 40;              // Hauteur du joueur pour la collision
  SDL_Color color;                  // Variable pour stocker la couleur du pixel
  int x, y, x1, y1, x2, y2, x3, y3; // Coordonnées pour les vérifications de collision

  // Vérification de la collision au-dessus du joueur
  x = p->perso.positioninscrenn.x + bp.background.positionfromimage.x; // Position x du joueur dans l'espace mondial
  y = p->perso.positioninscrenn.y + bp.background.positionfromimage.y; // Position y du joueur dans l'espace mondial
  color = GetPixel(Masque, x, y);   // Obtient la couleur du pixel en haut à gauche
  p->tabcollison[2] = (color.r == 0 && color.g == 0 && color.b == 0) ? 1 : 0; // Définit la collision en haut (1 si pixel noir)

  // Vérification de la collision à droite
  x = p->perso.positioninscrenn.x + persowidth + bp.background.positionfromimage.x; // Bord droit x
  p->tabcollison[0] = 0;            // Initialise la collision à droite comme libre
  y = p->perso.positioninscrenn.y + persoheght / 2 + bp.background.positionfromimage.y; // Milieu y
  color = GetPixel(Masque, x, y);   // Obtient la couleur du pixel
  if (color.r == 0 && color.g == 0 && color.b == 0) p->tabcollison[0] = 1; // Définit la collision si pixel noir
  y = p->perso.positioninscrenn.y + bp.background.positionfromimage.y; // Haut y
  color = GetPixel(Masque, x, y);   // Obtient la couleur du pixel
  if (color.r == 0 && color.g == 0 && color.b == 0) p->tabcollison[0] = 1; // Définit la collision si pixel noir
  y = p->perso.positioninscrenn.y + persoheght - 5 + bp.background.positionfromimage.y; // Bas y
  color = GetPixel(Masque, x, y);   // Obtient la couleur du pixel
  if (color.r == 0 && color.g == 0 && color.b == 0) p->tabcollison[0] = 1; // Définit la collision si pixel noir

  // Vérification de la collision à gauche
  x1 = p->perso.positioninscrenn.x + bp.background.positionfromimage.x; // Bord gauche x
  p->tabcollison[1] = 0;            // Initialise la collision à gauche comme libre
  y1 = p->perso.positioninscrenn.y + persoheght / 2 + bp.background.positionfromimage.y; // Milieu y
  color = GetPixel(Masque, x1, y1); // Obtient la couleur du pixel
  if (color.r == 0 && color.g == 0 && color.b == 0) p->tabcollison[1] = 1; // Définit la collision si pixel noir
  y1 = p->perso.positioninscrenn.y + bp.background.positionfromimage.y; // Haut y
  color = GetPixel(Masque, x1, y1); // Obtient la couleur du pixel
  if (color.r == 0 && color.g == 0 && color.b == 0) p->tabcollison[1] = 1; // Définit la collision si pixel noir
  y1 = p->perso.positioninscrenn.y + persoheght - 5 + bp.background.positionfromimage.y; // Bas y
  color = GetPixel(Masque, x1, y1); // Obtient la couleur du pixel
  if (color.r == 0 && color.g == 0 && color.b == 0) p->tabcollison[1] = 1; // Définit la collision si pixel noir

  // Vérification de la collision au-dessus (centre)
  x2 = p->perso.positioninscrenn.x + bp.background.positionfromimage.x + persowidth / 2; // Centre x
  y2 = p->perso.positioninscrenn.y + bp.background.positionfromimage.y; // Haut y
  color = GetPixel(Masque, x2, y2); // Obtient la couleur du pixel
  if (color.r == 0 && color.g == 0 && color.b == 0) p->tabcollison[2] = 1; // Définit la collision si pixel noir

  // Vérification de la collision en dessous
  p->tabcollison[3] = 0;            // Initialise la collision en bas comme libre
  x3 = p->perso.positioninscrenn.x + persowidth + bp.background.positionfromimage.x + persowidth / 2; // Centre-droit x
  y3 = p->perso.positioninscrenn.y + 5 + bp.background.positionfromimage.y + persoheght; // Bas y
  color = GetPixel(Masque, x3, y3); // Obtient la couleur du pixel
  if (color.r == 0 && color.g == 0 && color.b == 0) p->tabcollison[3] = 1; // Définit la collision si pixel noir
  x3 = p->perso.positioninscrenn.x + bp.background.positionfromimage.x + persowidth / 2; // Centre x
  y3 = p->perso.positioninscrenn.y + 5 + bp.background.positionfromimage.y + persoheght; // Bas y
  color = GetPixel(Masque, x3, y3); // Obtient la couleur du pixel
  if (color.r == 0 && color.g == 0 && color.b == 0) p->tabcollison[3] = 1; // Définit la collision si pixel noir
}

// Sauvegarde de la position du joueur et de la caméra dans un fichier
void savedata(SDL_Rect posjoueur, SDL_Rect camera, char fich[])
{
  FILE *f;                          // Pointeur vers le fichier
  f = fopen(fich, "wb");            // Ouvre le fichier en mode écriture binaire
  if (f != NULL) {                  // Si le fichier est ouvert avec succès
    fwrite(&posjoueur, sizeof(SDL_Rect), 1, f); // Écrit la position du joueur
    fwrite(&camera, sizeof(SDL_Rect), 1, f);    // Écrit la position de la caméra
    fclose(f);                      // Ferme le fichier
  }
}

// Chargement de la position du joueur et de la caméra à partir d'un fichier
void loaddata(SDL_Rect *posjoueur, SDL_Rect *camera, char fich[])
{
  FILE *f;                          // Pointeur vers le fichier
  f = fopen(fich, "rb");            // Ouvre le fichier en mode lecture binaire
  if (f != NULL) {                  // Si le fichier est ouvert avec succès
    fread(&(*posjoueur), sizeof(SDL_Rect), 1, f); // Lit la position du joueur
    fread(&(*camera), sizeof(SDL_Rect), 1, f);    // Lit la position de la caméra
    fclose(f);                      // Ferme le fichier
  }
}

// Initialisation d'un niveau du jeu
Level initLevel(int levelNumber)
{
  Level level;                      // Crée une structure de niveau
  level.background = IMG_Load(levelNumber == 1 ? "level1.2.png" : "2.png"); // Charge l'image d'arrière-plan selon le niveau
  level.minimap_bg = IMG_Load(levelNumber == 1 ? "level1.2.png" : "2.png"); // Charge l'arrière-plan de la minicarte
  level.platformCount = 2;          // Définit le nombre de plateformes (exemple : 2)
  level.platforms = (Platform*)malloc(sizeof(Platform) * level.platformCount); // Alloue la mémoire pour les plateformes
  
  // Initialisation de la plateforme 1 (non destructible)
  level.platforms[0].image = IMG_Load("RBB.png"); // Charge l'image de la plateforme
  level.platforms[0].rect.x = 450;  // Définit la position x de la plateforme
  level.platforms[0].rect.y = 485;  // Définit la position y de la plateforme
  level.platforms[0].rect.w = 100;  // Définit la largeur de la plateforme
  level.platforms[0].rect.h = 20;   // Définit la hauteur de la plateforme
  level.platforms[0].isDestructible = false; // La plateforme n'est pas destructible
  level.platforms[0].isMobile = false; // La plateforme n'est pas mobile
  level.platforms[0].isDestroyed = false; // La plateforme n'est pas détruite
  
  // Initialisation de la plateforme 2 (destructible)
  level.platforms[1].image = IMG_Load("pl2.png"); // Charge l'image de la plateforme destructible
  level.platforms[1].rect.x = 500;  // Définit la position x de la plateforme
  level.platforms[1].rect.y = 400;  // Définit la position y de la plateforme
  level.platforms[1].rect.w = 100;  // Définit la largeur de la plateforme
  level.platforms[1].rect.h = 20;   // Définit la hauteur de la plateforme
  level.platforms[1].isDestructible = true; // La plateforme est destructible
  level.platforms[1].isMobile = false; // La plateforme n'est pas mobile
  level.platforms[1].isDestroyed = false; // La plateforme n'est pas détruite
  
  level.bounds.x = 0;               // Définit la limite x du niveau
  level.bounds.y = 0;               // Définit la limite y du niveau
  level.bounds.w = 1300;            // Définit la largeur des limites du niveau
  level.bounds.h = 700;             // Définit la hauteur des limites du niveau
  
  return level;                     // Retourne le niveau initialisé
}

// Gestion des collisions avec les plateformes destructibles
void handleDestructibleCollisions(Personne *p, Level *level)
{
  SDL_Rect playerRect = p->perso.positioninscrenn; // Obtient la position du joueur à l'écran
  playerRect.w = 24;                // Définit la largeur du joueur pour la collision
  playerRect.h = 40;                // Définit la hauteur du joueur pour la collision
  
  for (int i = 0; i < level->platformCount; i++) { // Parcourt toutes les plateformes
    if (level->platforms[i].isDestroyed) continue; // Ignore les plateformes détruites
    
    if (checkBBCollision(&playerRect, &level->platforms[i].rect)) { // Si le joueur entre en collision avec la plateforme
      if (level->platforms[i].isDestructible) { // Si la plateforme est destructible
        level->platforms[i].isDestroyed = true; // Marque la plateforme comme détruite
        // Possibilité d'ajouter un son ou une animation ici
      } else {                        // Si la plateforme n'est pas destructible
        if (p->direction == 0) p->tabcollison[0] = 1; // Bloque le mouvement à droite
        else if (p->direction == 1) p->tabcollison[1] = 1; // Bloque le mouvement à gauche
      }
    }
  }
}

// Vérification des collisions par boîtes englobantes entre deux rectangles
bool checkBBCollision(SDL_Rect *rect1, SDL_Rect *rect2)
{
  return (rect1->x < rect2->x + rect2->w && // Vérifie si le bord gauche de rect1 est à gauche du bord droit de rect2
          rect1->x + rect1->w > rect2->x && // Vérifie si le bord droit de rect1 est à droite du bord gauche de rect2
          rect1->y < rect2->y + rect2->h && // Vérifie si le bord supérieur de rect1 est au-dessus du bord inférieur de rect2
          rect1->y + rect1->h > rect2->y); // Vérifie si le bord inférieur de rect1 est en dessous du bord supérieur de rect2
}

// Nettoyage des ressources du jeu
void cleanup(Player *player, Level *level, MiniMap *minimap)
{
  SDL_FreeSurface(player->sprite);   // Libère la surface du sprite du joueur
  SDL_FreeSurface(level->background); // Libère la surface de l'arrière-plan du niveau
  SDL_FreeSurface(level->minimap_bg); // Libère la surface de l'arrière-plan de la minicarte
  for (int i = 0; i < level->platformCount; i++) { // Parcourt les plateformes
    SDL_FreeSurface(level->platforms[i].image); // Libère l'image de la plateforme
  }
  free(level->platforms);           // Libère le tableau des plateformes
  SDL_FreeSurface(minimap->backg_img); // Libère l'image d'arrière-plan de la minicarte
  SDL_FreeSurface(minimap->player_img); // Libère l'image du marqueur du joueur
}
