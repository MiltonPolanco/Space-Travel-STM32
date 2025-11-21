#include "celestial_body.h"
#include "../Drivers/LCD/lcd_driver.h"
#include <string.h>
#include <math.h>

void CelestialBody_Init(CelestialBody* body, const char* name, BodyType type)
{
    strncpy(body->name, name, MAX_NAME_LENGTH - 1);
    body->name[MAX_NAME_LENGTH - 1] = '\0';
    body->type = type;

    body->position = vec3_create(0, 0, 0);
    body->radius = 1.0f;
    body->mass = 1.0f;

    body->orbit_radius = 0.0f;
    body->orbit_speed = 0.0f;
    body->orbit_angle = 0.0f;
    body->orbit_tilt = 0.0f;

    body->rotation_angle = 0.0f;
    body->rotation_speed = 0.0f;

    body->color = 0xFFFF;

    body->parent = NULL;

    body->screen_x = 0;
    body->screen_y = 0;
    body->screen_radius = 0;
    body->is_visible = 0;
    body->distance_to_camera = 0.0f;


    body->prev_screen_x = -1000;
    body->prev_screen_y = -1000;
    body->prev_screen_radius = 0;
}

void CelestialBody_SetVisuals(CelestialBody* body, float radius, uint16_t color)
{
    body->radius = radius;
    body->color = color;
}

void CelestialBody_SetOrbitalParams(CelestialBody* body, float orbit_radius, float orbit_speed, float orbit_tilt)
{
    body->orbit_radius = orbit_radius;
    body->orbit_speed = orbit_speed;
    body->orbit_tilt = orbit_tilt;
}

void CelestialBody_SetRotation(CelestialBody* body, float rotation_speed)
{
    body->rotation_speed = rotation_speed;
}

void CelestialBody_SetParent(CelestialBody* body, CelestialBody* parent)
{
    body->parent = parent;
}

void CelestialBody_Update(CelestialBody* body, float deltaTime)
{
    // Actualizar órbita
    body->orbit_angle += body->orbit_speed * deltaTime;
    if (body->orbit_angle > TWO_PI) {
        body->orbit_angle -= TWO_PI;
    }


    if (body->orbit_radius > 0.0f) {
        Vector3 orbit_pos;
        orbit_pos.x = body->orbit_radius * cosf(body->orbit_angle);
        orbit_pos.z = body->orbit_radius * sinf(body->orbit_angle);
        orbit_pos.y = body->orbit_radius * sinf(body->orbit_tilt) * sinf(body->orbit_angle);

        // Posición relativa al padre
        if (body->parent != NULL) {
            body->position = vec3_add(body->parent->position, orbit_pos);
        } else {
            body->position = orbit_pos;
        }
    }

    // Actualizar rotación
    body->rotation_angle += body->rotation_speed * deltaTime;
    if (body->rotation_angle > TWO_PI) {
        body->rotation_angle -= TWO_PI;
    }
}

void CelestialBody_Render(CelestialBody* body)
{
    if (!body->is_visible) return;


    if (body->type != BODY_TYPE_MOON &&
        body->prev_screen_x >= 0 && body->prev_screen_x < LCD_WIDTH &&
        body->prev_screen_y >= 0 && body->prev_screen_y < LCD_HEIGHT) {

        int16_t clear_radius = body->prev_screen_radius + 2;
        LCD_FillCircle(body->prev_screen_x, body->prev_screen_y,
                      clear_radius, COLOR_SPACE);


        if (body->type == BODY_TYPE_PLANET && body->orbit_radius > 0) {
            uint16_t orbit_color = 0x632C;


            for (int i = -10; i <= 10; i++) {
                float angle = body->orbit_angle + (float)i * 0.05f;

                float scale = 0.28f;
                int16_t ox = (int16_t)(body->orbit_radius * cosf(angle) * scale + LCD_WIDTH / 2.0f);
                int16_t oy = (int16_t)(body->orbit_radius * sinf(angle) * scale + LCD_HEIGHT / 2.0f);

                LCD_DrawPixel(ox, oy, orbit_color);
                LCD_DrawPixel(ox + 1, oy, orbit_color);
            }
        }
    }


    LCD_FillCircle(body->screen_x, body->screen_y,
                   body->screen_radius, body->color);

    // Efecto especial para el Sol
    if (body->type == BODY_TYPE_SUN) {
        LCD_DrawCircle(body->screen_x, body->screen_y,
                      body->screen_radius + 1, COLOR_YELLOW);
        LCD_DrawCircle(body->screen_x, body->screen_y,
                      body->screen_radius + 2, 0xFD20);
    }


    body->prev_screen_x = body->screen_x;
    body->prev_screen_y = body->screen_y;
    body->prev_screen_radius = body->screen_radius;
}
