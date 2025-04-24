// Inclusion des en-têtes SDL nécessaires pour la vidéo, les images et l'audio
#include <SDL/SDL.h>
#include <SDL/SDL_video.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
// Inclusion du fichier d'en-tête du jeu
#include "minimap.h"

// Fonction principale
int main()
{
  // Initialise tous les sous-systèmes SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    // Affiche un message d'erreur si l'initialisation SDL échoue
    printf("Erreur ! %s \n", SDL_GetError());
    return 0; // Quitte le programme
  }
  int Recmov;                       // Variable pour contrôler la direction du mouvement du rectangle
  int possol = 600;                 // Position du sol (non utilisée dans le code)
  int i = 0;                        // Compteur d'animation pour la minicarte
  SDL_Surface *screen = NULL;       // Pointeur vers la surface principale de l'écran
  SDL_Event event;                  // Variable pour stocker les événements SDL
  int numkeys;                      // Nombre de touches (pour SDL_GetKeyState)
  background backgroundst;          // Structure d'arrière-plan
  init_maske(&backgroundst.masque, 1); // Initialise le masque de collision pour le niveau 1
  Uint8 *keys;                      // Pointeur vers l'état du clavier
  Personne p;                       // Structure du personnage joueur
  initPerso(&p);                    // Initialise le joueur
  Uint32 timer, elapsed;            // Variables pour la gestion du temps
  MiniMap m;                        // Structure de la minicarte
  Level level = initLevel(1);       // Initialise le niveau 1 avec des plateformes
  SDL_Rect camera, image;           // Rectangles pour la caméra et une image (non utilisée)
  SDL_Rect positionRectangle;       // Rectangle pour un objet en mouvement
  positionRectangle.x = 50;         // Définit la position x du rectangle
  positionRectangle.y = 200;        // Définit la position y du rectangle
  SDL_Surface* Rectangle = IMG_Load("R.png"); // Charge l'image du rectangle
  SDL_Surface* RectangleBB = IMG_Load("RBB.png"); // Charge l'image alternative du rectangle
  int persowidth = 24;              // Largeur du joueur pour la collision
  int persoheight = 40;             // Hauteur du joueur pour la collision
  camera.x = 0;                     // Initialise la position x de la caméra
  camera.y = 0;                     // Initialise la position y de la caméra
  camera.w = 30 / 2;                // Définit la largeur de la caméra (15)
  camera.h = 10;                    // Définit la hauteur de la caméra
  int currentLevel = 1;             // Suit le niveau actuel (1 ou 2)
  init_minimap(&m, currentLevel);   // Initialise la minicarte pour le niveau 1
  int continuer = 1;                // Drapeau de contrôle de la boucle de jeu
  // Configure l'affichage (1040x780, 32 bits, surface matérielle, double tampon, redimensionnable)
  screen = SDL_SetVideoMode(1040, 780, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
  if (screen == NULL)
  {
    // Affiche une erreur si la création de l'écran échoue
    printf("Impossible de créer l'écran : %s \n ", SDL_GetError());
    SDL_Quit(); // Quitte SDL
  }
  // Charge les positions sauvegardées du joueur et de la caméra
  loaddata(&p.perso.positioninscrenn, &camera, "savemini");
  // Boucle principale du jeu
  while (continuer)
  {
    // Définit le rectangle source de l'arrière-plan
    backgroundst.background.positionfromimage.x = 0;
    backgroundst.background.positionfromimage.y = 0;
    backgroundst.background.positionfromimage.w = 1000;
    backgroundst.background.positionfromimage.h = 1000;
    // Copie le masque de collision comme arrière-plan (probablement une erreur ; devrait être l'image d'arrière-plan)
    SDL_BlitSurface(backgroundst.masque.image, &backgroundst.background.positionfromimage, screen, NULL);
    
    // Affiche toutes les plateformes non détruites
    for (int j = 0; j < level.platformCount; j++) {
      if (!level.platforms[j].isDestroyed) {
        SDL_BlitSurface(level.platforms[j].image, NULL, screen, &level.platforms[j].rect); // Copie la plateforme à l'écran
      }
    }
    
    // Traite les événements SDL
    SDL_PollEvent(&event);
    switch (event.type) {
      case SDL_QUIT:                // Événement de fermeture de la fenêtre
        continuer = 0;              // Quitte la boucle de jeu
        break;
      case SDL_KEYDOWN:             // Événement de touche pressée
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:         // Touche Échap
            savedata(p.perso.positioninscrenn, camera, "savemini"); // Sauvegarde les données du jeu
            exit(-1);               // Quitte le programme
            break;
          case SDLK_DOWN:           // Touche flèche bas
            if (p.tabcollison[3] == 0) // S'il n'y a pas de collision en dessous
              p.perso.positioninscrenn.y += 10; // Déplace le joueur vers le bas
            p.velocityX = 0;        // Réinitialise la vélocité horizontale
            p.velocityY = -80;      // Définit la vélocité verticale
            break;
          case SDLK_UP:             // Touche flèche haut
            if ((p.tabcollison[2] == 0) && (p.perso.positioninscrenn.y > 30)) // S'il n'y a pas de collision au-dessus et au-dessus de y=30
            {
              p.perso.positioninscrenn.y -= 10; // Déplace le joueur vers le haut
              p.velocityX = 0;      // Réinitialise la vélocité horizontale
              p.velocityY = -80;    // Définit la vélocité verticale
            }
            break;
          case SDLK_RIGHT:          // Touche flèche droite
            if (p.tabcollison[0] == 0) // S'il n'y a pas de collision à droite
              p.perso.positioninscrenn.x += 10; // Déplace le joueur vers la droite
            p.deplacement = 1;      // Définit l'état de mouvement à en mouvement
            p.direction = 0;        // Définit la direction à droite
            break;
          case SDLK_LEFT:           // Touche flèche gauche
            if (p.tabcollison[1] == 0) // S'il n'y a pas de collision à gauche
              p.perso.positioninscrenn.x -= 10; // Déplace le joueur vers la gauche
            p.deplacement = 1;      // Définit l'état de mouvement à en mouvement
            p.direction = 1;        // Définit la direction à gauche
            break;
          case SDLK_SPACE:          // Touche espace
            if (p.up == 0) {        // Si le joueur ne saute pas déjà
              p.up = 1;             // Définit l'état de saut
              p.velocityX = 60;     // Définit la vélocité horizontale
              p.velocityY = -100;   // Définit la vélocité verticale (montée)
            }
            break;
          case SDLK_n:              // Touche 'n' pour changer de niveau
            currentLevel = (currentLevel == 1) ? 2 : 1; // Alterne entre les niveaux 1 et 2
            // Libère les ressources existantes
            SDL_FreeSurface(m.backg_img); // Libère l'arrière-plan de la minicarte
            SDL_FreeSurface(m.player_img); // Libère le marqueur du joueur
            SDL_FreeSurface(backgroundst.masque.image); // Libère le masque de collision
            for (int j = 0; j < level.platformCount; j++) {
              SDL_FreeSurface(level.platforms[j].image); // Libère les images des plateformes
            }
            free(level.platforms); // Libère le tableau des plateformes
            // Réinitialise le niveau, la minicarte et le masque
            level = initLevel(currentLevel); // Initialise le nouveau niveau
            init_minimap(&m, currentLevel); // Initialise la minicarte
            init_maske(&backgroundst.masque, currentLevel); // Initialise le masque de collision
            initPerso(&p);          // Réinitialise le joueur
            camera.x = 0;           // Réinitialise la position x de la caméra
            camera.y = 0;           // Réinitialise la position y de la caméra
            break;
        }
        break;
      case SDL_KEYUP:               // Événement de touche relâchée
        switch (event.key.keysym.sym) {
          case SDLK_RIGHT:          // Touche flèche droite relâchée
            p.deplacement = 0;      // Arrête le mouvement
            break;
          case SDLK_LEFT:           // Touche flèche gauche relâchée
            p.deplacement = 0;      // Arrête le mouvement
            break;
        }
        break;
    }
    // Définit la vitesse du joueur en fonction de l'état de mouvement
    if (p.deplacement == 1) {
      p.vitesse = 0.5;              // Définit la vitesse lors du mouvement
    } else if (p.deplacement == 0) {
      p.vitesse = 0;                // Arrête le mouvement
    }
    camera.x = 0;                   // Réinitialise la position x de la caméra (pas de défilement)
    camera.y = 0;                   // Réinitialise la position y de la caméra (pas de défilement)
    afficher_minimap(m, screen, camera, &i); // Affiche la minicarte
    // Si une collision est détectée, affiche l'animation
    if ((p.tabcollison[0] == 1) || (p.tabcollison[1] == 1) || (p.tabcollison[2] == 1) || (p.tabcollison[3] == 1))
    {
      afficher_animation(m, screen, camera, &i); // Affiche l'animation de la minicarte
    }
    MAJMinimap(p.perso.positioninscrenn, &m, camera, 25); // Met à jour la position du joueur sur la minicarte
    afficherPerso(p, screen);       // Affiche le joueur à l'écran
    keys = SDL_GetKeyState(&numkeys); // Obtient l'état du clavier
    collisionPP(&p, screen, backgroundst); // Vérifie les collisions pixel-par-pixel
    handleDestructibleCollisions(&p, &level); // Gère les collisions avec les plateformes destructibles
    // Déplace le rectangle d'avant en arrière
    if (positionRectangle.x < 50)
      Recmov = 1;                   // Déplace à droite si à la limite gauche
    else if (positionRectangle.x > 800)
      Recmov = 0;                   // Déplace à gauche si à la limite droite
    if (Recmov)
      positionRectangle.x += 10;    // Déplace le rectangle à droite
    else
      positionRectangle.x -= 10;    // Déplace le rectangle à gauche
    // Vérifie la collision avec le rectangle et affiche l'image appropriée
    if (positionRectangle.x + positionRectangle.w > p.perso.positioninscrenn.x &&
        positionRectangle.x < p.perso.positioninscrenn.x + persowidth &&
        positionRectangle.y + positionRectangle.h > p.perso.positioninscrenn.y &&
        positionRectangle.y < p.perso.positioninscrenn.y + persoheight) {
      SDL_BlitSurface(RectangleBB, NULL, screen, &positionRectangle); // Affiche le rectangle alternatif en cas de collision
    } else {
      SDL_BlitSurface(Rectangle, NULL, screen, &positionRectangle); // Affiche le rectangle normal
    }
    SDL_Flip(screen);               // Met à jour l'écran (double tampon)
    timer = SDL_GetTicks();         // Obtient le temps actuel
    elapsed = SDL_GetTicks() - timer; // Calcule le temps écoulé
    if (elapsed < 20)               // Si moins de 20ms se sont écoulées
    {
      SDL_Delay(20 - elapsed);      // Attend pour maintenir ~50 FPS
    }
  }
  // Sauvegarde les données du jeu à la sortie
  savedata(p.perso.positioninscrenn, camera, "savemini");
  // Libère toutes les ressources
  SDL_FreeSurface(Rectangle);       // Libère l'image du rectangle
  SDL_FreeSurface(RectangleBB);     // Libère l'image alternative du rectangle
  SDL_FreeSurface(m.backg_img);     // Libère l'arrière-plan de la minicarte
  SDL_FreeSurface(m.player_img);    // Libère le marqueur du joueur
  SDL_FreeSurface(backgroundst.masque.image); // Libère le masque de collision
  for (int j = 0; j < level.platformCount; j++) {
    SDL_FreeSurface(level.platforms[j].image); // Libère les images des plateformes
  }
  free(level.platforms);           // Libère le tableau des plateformes
  SDL_FreeSurface(screen);         // Libère la surface de l'écran
  SDL_Quit();                      // Quitte SDL
  return 0;                        // Quitte le programme
}
