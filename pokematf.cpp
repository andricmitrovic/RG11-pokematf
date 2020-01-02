#include <GL/glut.h>
#include <iostream>
#include "pokematf.h"
#include <unordered_set>
#include <string>

//STRUCTURES

typedef struct Position_info{
    float x, y, width, height;
} Position_info;

typedef struct Pokemon_info{
    int health, attack;
} Pokemon_info;

//GLOBAL VARIABLES

float matrix[16];
int window_width = 700;   
int window_height = 700;

#define WINDOW_FIELD              0  
#define WINDOW_POKEDEX            1
#define WINDOW_BATTLE             2
//PRIVATE VARIABLES

static int state = 1;
static int window_select = WINDOW_FIELD;
static int animation_parametar = 0;
static bool move_pokemon = true;
static int mouse_x;
static int mouse_y;
static Position_info wild_pokemon_info = { 0, 0, 0.3, 0.3};
static Position_info player_info = { 0, 0, 1, 2};
static Position_info pokecenter_info = { -7, 8, 3.5, 3.5};
static Pokemon_info poke_info[50];
static bool write_message = false;
static const char* message;
static int message_time = 100;
static int show_pokemon = 0;
static std::unordered_set<int> owned_pokemons = {0, 1, 2};
static char buffer[50];
static GLuint player_sprites[3];
static GLuint pokecenter_sprite;
static GLuint pokemon_sprites[50];
static GLuint background_textures[10];
static GLuint arrow_sprite;
static int battle_drawing = 0;
static int show_wild_pokemon = 0;

//PRIVATE FUNCTION DECLARATION

static void draw_axes(float len);

static void display_field();

static void display_pokemons();

static void reshape1(int w, int h);

static void draw_stats();

static void draw_player();

static void draw_pikachu();

static void draw_bulbasaur();

static void draw_squirtle();

static void draw_charmander();

static void draw_pokemons();

static bool check_collision(float x1, float y1, float w1, float h1, 
                        float x2, float y2, float w2, float h2);

static void draw_grass();

static void draw_floor();

static void open_pokedex();

static void text_log( float x, float y, const char *s);

static void draw_pokecenter();

static bool check_proximity(float x1, float y1, float w1, float h1, 
                        float x2, float y2, float w2, float h2);

static void draw_pokedex_background();

static void draw_arrows();

static void draw_wild_pokemon();

static void display_battle();

static void draw_forest_background();

//PRIVATE FUNCTION DEFINITION


static void draw_axes(float len) {
    // glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(len,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,len,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,len);
    glEnd();

    glPointSize(10);
    glBegin(GL_POINTS);
        glColor3f(0,0,0);
        for(int i = -10; i <=10; i++)
            glVertex3i(i, 0, 10);
        for(int i = -10; i <=10; i++)
            glVertex3i(0, i, 10);
    glEnd();
}

static void display_field()
{
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);

    
    glPushMatrix();

    glMultMatrixf(matrix);

    glDisable(GL_TEXTURE_2D);
    draw_axes(10);  
    

    if(write_message)
    {
        text_log(-8, 8.3, message);
    }
    else
    {
        text_log(-8, 8.3, "Press H next to the\nPokecenter to heal pokemons");
    }
    
    text_log(-9.5, -9.5, "Pokedex( P )");

    draw_wild_pokemon();

    glEnable(GL_TEXTURE_2D);

    draw_floor();

    draw_grass();

    draw_player();

    draw_pokecenter();

    
    glPopMatrix();
    glutSwapBuffers();                  
}

static void draw_wild_pokemon()
{
    glPushMatrix();
    
    if(move_pokemon)
    {
        srand(time(NULL));
        move_pokemon = false;
        wild_pokemon_info.x = rand()%10;
        wild_pokemon_info.y = rand()%10;
    }

    glTranslatef( wild_pokemon_info.x, wild_pokemon_info.y, 0);
    glScalef( wild_pokemon_info.width, wild_pokemon_info.height, 1);
    
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 10);
        glVertex3f(0, -1, 10);
        glVertex3f(1, -1, 10);
        glVertex3f(1, 0, 10);
    glEnd();
    glPopMatrix();
}

static void display_pokemons()
{
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);

    
    glPushMatrix();

    glMultMatrixf(matrix);



    glDisable(GL_TEXTURE_2D);
    draw_axes(10);  

    // if(write_message)
    //     text_log(-8, 8.3, message);


    if( owned_pokemons.count( show_pokemon ))
    {
        text_log( -8, 9, "OWNED");

        draw_stats();

    }
    else
    {
        text_log( -8, 9, "NOT OWNED");
    }
    
    glEnable(GL_TEXTURE_2D);

    draw_pokedex_background();

    draw_arrows();

    draw_pokemons();

    glPopMatrix();
    glutSwapBuffers(); 
}

static void display_battle()
{
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);

    
    glPushMatrix();

    glMultMatrixf(matrix);



    glDisable(GL_TEXTURE_2D);
    draw_axes(10);  
    glEnable(GL_TEXTURE_2D);

    draw_forest_background();

    battle_drawing = 1;
    show_pokemon = 0;
    draw_pokemons();
    draw_stats();

    battle_drawing = 2;
    show_pokemon = show_wild_pokemon;
    draw_pokemons();
    draw_stats();

    battle_drawing = 0;

    glPopMatrix();
    glutSwapBuffers(); 
}

static void draw_forest_background()
{
    glBindTexture(GL_TEXTURE_2D, background_textures[3]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-10, -10, 0);

        glTexCoord2f(1, 0);
        glVertex3f(10, -10, 0);

        glTexCoord2f(1, 1);
        glVertex3f(10, 10, 0);

        glTexCoord2f(0, 1);
        glVertex3f(-10, 10, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

static void draw_stats()
{
    glPushMatrix();

    if(battle_drawing == 0)
    {
        glTranslatef( -3, -4, 0);
        glScalef( 6, 0.7, 1);
    }
    if(battle_drawing == 1)
    {
        glTranslatef( -8, -1, 4);
        glScalef( 4, 0.7, 1);
    }
    if(battle_drawing == 2)
    {
        glTranslatef( 3, 1.5, 4);
        glScalef( 4, 0.7, 1);
    }

    sprintf (buffer, "Attack : %d", poke_info[ show_pokemon ].attack);
    text_log(0, -1.8, buffer);

    sprintf (buffer, "Health : %d", poke_info[ show_pokemon ].health);
    text_log(0, 0.2, buffer);
    
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 4);
        glVertex3f(0, -1, 4);
        glVertex3f(1, -1, 4);
        glVertex3f(1, 0, 4);
    glEnd();
    glPopMatrix();
}

static void draw_pokecenter()
{
    glPushMatrix();

    glTranslatef( pokecenter_info.x, pokecenter_info.y, 0 );
    glScalef( pokecenter_info.width, pokecenter_info.height, 1); 

    glBindTexture(GL_TEXTURE_2D, pokecenter_sprite);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 1);
        glVertex3f(0, 0, 4);

        glTexCoord2f(0  , 0);
        glVertex3f(0, -1, 4);

        glTexCoord2f(1, 0);
        glVertex3f(1, -1, 4);

        glTexCoord2f(1, 1);
        glVertex3f(1, 0, 4);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

static void draw_arrows()
{
    if(show_pokemon != 15 )
    {

    text_log(7.8, -0.15, "D");

    glPushMatrix();

    glTranslatef( 6, 0.75, 0 );
    glScalef( 3, 1.5, 1);

    glBindTexture(GL_TEXTURE_2D, arrow_sprite);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 1);
        glVertex3f(0, 0, 4);

        glTexCoord2f(0  , 0);
        glVertex3f(0, -1, 4);

        glTexCoord2f(1, 0);
        glVertex3f(1, -1, 4);

        glTexCoord2f(1, 1);
        glVertex3f(1, 0, 4);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    }



    if(show_pokemon != 0 )
    {

    text_log(-8., -0.15, "A");

    glPushMatrix();

    glRotatef(180, 0, 0, 1);
    glTranslatef( 6, 0.75, 0 );
    glScalef( 3, 1.5, 1); 
    
    glBindTexture(GL_TEXTURE_2D, arrow_sprite);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 1);
        glVertex3f(0, 0, 4);

        glTexCoord2f(0  , 0);
        glVertex3f(0, -1, 4);

        glTexCoord2f(1, 0);
        glVertex3f(1, -1, 4);

        glTexCoord2f(1, 1);
        glVertex3f(1, 0, 4);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    }
}


static void text_log( float x, float y, const char *s)
{
    glDisable(GL_TEXTURE_2D);

    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glRasterPos3f(x, y, 10);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, reinterpret_cast<const unsigned char *>( s ) ); 

    glEnable(GL_TEXTURE_2D);
}

static void draw_player()
{
    glPushMatrix();

    glTranslatef(player_info.x,player_info.y,0);
    glScalef( player_info.width, player_info.height, 1);

    glBindTexture(GL_TEXTURE_2D, player_sprites[0]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 1);
        glVertex3f(0, 0, 4);

        glTexCoord2f(0 , 0);
        glVertex3f(0, -1, 4);

        glTexCoord2f(1, 0);
        glVertex3f(1, -1, 4);

        glTexCoord2f(1, 1);
        glVertex3f(1, 0, 4);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

static void draw_pokemons()
{
    glPushMatrix();
    
    if(battle_drawing == 0)
    {
        glTranslatef( -5, 7, 4);
        glScalef( 10, 10, 1); 
    }
    if(battle_drawing == 1)
    {
        glTranslatef( -8, -2.5, 4);
        glScalef( 7, 7, 1);
    }
    if(battle_drawing == 2)
    {
        glTranslatef( 4 + 2.5, 0, 4);
        glRotatef( 180, 0, 1, 0);
        glScalef( 2.5, 2.5, 1);
    }



    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[ show_pokemon ]);


    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 1);
        glVertex3f(0, 0, 0);

        glTexCoord2f(0  , 0);
        glVertex3f(0, -1, 0);

        glTexCoord2f(1, 0);
        glVertex3f(1, -1, 0);

        glTexCoord2f(1, 1);
        glVertex3f(1, 0, 0);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

static bool check_collision(float x1, float y1, float w1, float h1, 
                        float x2, float y2, float w2, float h2)
{
    if (x1 + w1 >= x2 &&
       x1 <= x2 + w2 &&
       y1 - h1 <= y2 &&
       y1 >= y2 - h2 )
    {
    return true;
    }
    else
    {
        return false;
    }

}

static bool check_proximity(float x1, float y1, float w1, float h1, 
                        float x2, float y2, float w2, float h2)
{
    if ( check_collision( x1 + .1, y1, w1, h1, 
                        x2, y2, w2, h2 ) )
        return true;
    
    if ( check_collision( x1 - .1, y1, w1, h1, 
                        x2, y2, w2, h2 ) )
        return true;

    if ( check_collision( x1 , y1 + .1, w1, h1, 
                        x2, y2, w2, h2 ) )
        return true;

    if ( check_collision( x1 , y1 - .1, w1, h1, 
                        x2, y2, w2, h2 ) )
        return true;

    
    return false;
}


static void reshape1(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -10, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

static void draw_pokedex_background()
{
    /* Crta se pod*/
    glBindTexture(GL_TEXTURE_2D, background_textures[2]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-10, -10, 0);

        glTexCoord2f(1, 0);
        glVertex3f(10, -10, 0);

        glTexCoord2f(1, 1);
        glVertex3f(10, 10, 0);

        glTexCoord2f(0, 1);
        glVertex3f(-10, 10, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}  

static void draw_grass()
{
    /* Crtaju se trava */
    glBindTexture(GL_TEXTURE_2D, background_textures[1]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 3);

        glTexCoord2f(10, 0);
        glVertex3f(10, 0, 3);

        glTexCoord2f(10, 10);
        glVertex3f(10, 10, 3);

        glTexCoord2f(0, 10);
        glVertex3f(0, 10, 3);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}
static void draw_floor()
{
    /* Crta se pod*/
    glBindTexture(GL_TEXTURE_2D, background_textures[0]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-10, -10, 0);

        glTexCoord2f(6, 0);
        glVertex3f(10, -10, 0);

        glTexCoord2f(6, 6);
        glVertex3f(10, 10, 0);

        glTexCoord2f(0, 6);
        glVertex3f(-10, 10, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}  

static void open_pokedex()
{
    show_pokemon = 0;
    window_select = WINDOW_POKEDEX;
    reshape1(window_width, window_height);
    glutPostRedisplay();
}

//PUBLIC

void display()
{
    if(window_select == WINDOW_FIELD)
    {
        display_field();
    }
    else if(window_select == WINDOW_POKEDEX)
    {
        display_pokemons();
    }
    else if(window_select == WINDOW_BATTLE)
    {
        display_battle();
    }

}

void reshape(int w, int h)
{
    window_width = w;
    window_height = h;

    reshape1(w, h);
}

void timer(int timer_id)
{
    if (timer_id == TIMER_ID) 
    {
        if(animation_parametar >= 100)
            animation_parametar = 0;
        animation_parametar += 1;

        if(write_message)
        {
            message_time -= 1;
            if( message_time <=0 )
            {
                write_message = false;
                message_time = 100;
            }
        }
    }

    glutPostRedisplay();

    glutTimerFunc(TIMER_INTERVAL, timer, TIMER_ID);
}


void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27:
    {
        glDeleteTextures(2, background_textures);
        glDeleteTextures(1, &pokecenter_sprite);
        glDeleteTextures(1, player_sprites);
        exit(0);
    break;
    }
    case 'k':
    case 'K':
    {
        window_select = WINDOW_FIELD;
        reshape1(window_width, window_height);
        glutPostRedisplay();
    break;
    }
    case 'r':
    case 'R':
    {
        glLoadIdentity();
        glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
        glutPostRedisplay();
    break;
    }
    }
    if(window_select == WINDOW_POKEDEX)
    {
        switch(key)
        {
        case 'a':
        case 'A':
            if(show_pokemon == 0 )
                break;
            show_pokemon --;
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            if(show_pokemon == 15 )
                break;
            show_pokemon ++;
            glutPostRedisplay();
            break;
        }
    }   

    if(window_select == WINDOW_FIELD)
    {
    switch (key) {
    case 'p':
    case 'P':
    {
        open_pokedex();
    break;
    }
    case 'w':
    case 'W':
        if(player_info.y <= 10 )
        {
            if(!check_collision( player_info.x, player_info.y + .1, player_info.width, player_info.height,
                         pokecenter_info.x, pokecenter_info.y, pokecenter_info.width, pokecenter_info.height))
            {
                if(check_collision( player_info.x, player_info.y + .1, player_info.width, player_info.height,
                         wild_pokemon_info.x, wild_pokemon_info.y, wild_pokemon_info.width, wild_pokemon_info.height))
                {             
                    printf("WILD POKEMON\n");
                    window_select = WINDOW_BATTLE;
                    move_pokemon = true;
                    srand(time(NULL));
                    show_wild_pokemon = rand()%16;
                }

                player_info.y+=.1;
                glutPostRedisplay();
                
            }
        }
        break;
    case 's':
    case 'S':
        if(player_info.y - 0.1 >= -10 + player_info.height)
        {
            if(!check_collision( player_info.x, player_info.y - .1, player_info.width, player_info.height,
                         pokecenter_info.x, pokecenter_info.y, pokecenter_info.width, pokecenter_info.height))
            {
                if(check_collision( player_info.x, player_info.y - .1, player_info.width, player_info.height,
                         wild_pokemon_info.x, wild_pokemon_info.y, wild_pokemon_info.width, wild_pokemon_info.height))
                {             
                    printf("WILD POKEMON\n");
                    window_select = WINDOW_BATTLE;
                    move_pokemon = true;
                    srand(time(NULL));
                    show_wild_pokemon = rand()%16;                    
                }

                player_info.y-=.1;
                glutPostRedisplay();
            }
        }
        break;
    case 'a':
    case 'A':
        if(player_info.x >= -10)
        {
            if(!check_collision( player_info.x - .1, player_info.y, player_info.width, player_info.height,
                         pokecenter_info.x, pokecenter_info.y, pokecenter_info.width, pokecenter_info.height))
            {
                if(check_collision( player_info.x - .1, player_info.y, player_info.width, player_info.height,
                         wild_pokemon_info.x, wild_pokemon_info.y, wild_pokemon_info.width, wild_pokemon_info.height))
                {             
                    printf("WILD POKEMON\n");
                    window_select = WINDOW_BATTLE;
                    move_pokemon = true;
                    srand(time(NULL));
                    show_wild_pokemon = rand()%16;
                }

                player_info.x-=.1;
                glutPostRedisplay();
            }
        }
        break;
    case 'd':
    case 'D':
        if(player_info.x+.1 <= 10 - player_info.width)
        {
            if(!check_collision( player_info.x + .1, player_info.y, player_info.width, player_info.height,
                         pokecenter_info.x, pokecenter_info.y, pokecenter_info.width, pokecenter_info.height))
            {
                if(check_collision( player_info.x + .1, player_info.y, player_info.width, player_info.height,
                         wild_pokemon_info.x, wild_pokemon_info.y, wild_pokemon_info.width, wild_pokemon_info.height))
                {             
                    printf("WILD POKEMON\n");
                    window_select = WINDOW_BATTLE;
                    move_pokemon = true;
                    srand(time(NULL));
                    show_wild_pokemon = rand()%16;
                }

                player_info.x+=.1;
                glutPostRedisplay();
            }
        }
        break;
    case 'h':
    case 'H':
    {
        if( check_proximity( player_info.x, player_info.y, player_info.width, player_info.height,
                         pokecenter_info.x, pokecenter_info.y, pokecenter_info.width, pokecenter_info.height))
            {
                write_message = true;
                message = "All pokemons healed";
                for( auto elem : owned_pokemons)
                    poke_info[elem].health = 100;
                glutPostRedisplay();
            }
        break;
    }
       
    }
    }
}


void mouse(int button, int state, int x, int y)
{
    /* Pamti se pozicija pokazivaca misa. */
    mouse_x = x;
    mouse_y = y;
}

void motion(int x, int y)
{
    /* Promene pozicije pokazivaca misa. */
    int delta_x, delta_y;

    /* Izracunavaju se promene pozicije pokazivaca misa. */
    delta_x = x - mouse_x;
    delta_y = y - mouse_y;

    /* Pamti se nova pozicija pokazivaca misa. */
    mouse_x = x;
    mouse_y = y;

    /* Izracunava se nova matrica rotacije. */
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();
        glRotatef(180 * (float) delta_x / window_width, 0, 1, 0);
        glRotatef(180 * (float) delta_y / window_height, 1, 0, 0);
        glMultMatrixf(matrix);

        glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();

    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();
}

void texture_init()
{
        Image * image;

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    glGenTextures(4, background_textures);

    /* Kreira se prva tekstura. */

    image_read(image, "grass.bmp");

    glBindTexture(GL_TEXTURE_2D, background_textures[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    /* Kreira se druga tekstura. */
    image_read(image, "floor.bmp");

    glBindTexture(GL_TEXTURE_2D, background_textures[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);


    /* Kreira se treca tekstura. */
    image_read(image, "background.bmp");

    glBindTexture(GL_TEXTURE_2D, background_textures[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    
    image_read(image, "./resources/forest.bmp");

    glBindTexture(GL_TEXTURE_2D, background_textures[3]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);



    glGenTextures(1, &pokecenter_sprite);

    /* Kreira se pokecentar. */
    image_read(image, "pokecenter.bmp");

    glBindTexture(GL_TEXTURE_2D, pokecenter_sprite);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    
    glGenTextures(1, player_sprites);
    image_read(image, "character.bmp");

    glBindTexture(GL_TEXTURE_2D, player_sprites[0]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);


    glGenTextures(1, &arrow_sprite);
    image_read(image, "./resources/arrow.bmp");

    glBindTexture(GL_TEXTURE_2D, arrow_sprite);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    


////////////////////////////////////////// POKEMON LOADING

    glGenTextures(16, pokemon_sprites);

    image_read(image, "./resources/pokemon_sprites/bulbasaur.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[0]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    poke_info[0] = { 100, 5 };

    
    image_read(image, "./resources/pokemon_sprites/squirtle.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[1]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    poke_info[1] = { 95, 5 };
    
    image_read(image, "./resources/pokemon_sprites/charmander.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[2]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    poke_info[2] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/pikachu.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[3]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    poke_info[3] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/abra.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[4]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    poke_info[4] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/articuno.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[5]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    poke_info[5] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/eevee.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[6]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    poke_info[6] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/growlithe.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[7]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    poke_info[7] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/lapras.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[8]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);


    poke_info[8] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/snorlax.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[9]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    poke_info[9] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/zapdos.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[10]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    poke_info[10] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/moltres.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[11]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    poke_info[11] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/cubone.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[12]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    poke_info[12] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/pidgey.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[13]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    poke_info[13] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/vulpix.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[14]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    poke_info[14] = { 100, 5 };

    image_read(image, "./resources/pokemon_sprites/onix.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[15]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    poke_info[15] = { 100, 5 };

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);
}

