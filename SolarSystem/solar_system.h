#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include "celestial_body.h"
#include "camera.h"
#include <stdint.h>

#define MAX_BODIES 15

typedef struct {
    CelestialBody bodies[MAX_BODIES];
    uint8_t body_count;

    // Configuración de tiempo
    float time_scale;
    float total_time;

} SolarSystem;

// Funciones de inicialización
void SolarSystem_Init(SolarSystem* sys);
void SolarSystem_CreateDefaultSystem(SolarSystem* sys);
void SolarSystem_AddBody(SolarSystem* sys, CelestialBody body);

// Actualización
void SolarSystem_Update(SolarSystem* sys, float deltaTime);

// Renderizado
void SolarSystem_Render(SolarSystem* sys, Camera* cam);
void SolarSystem_RenderOrbits(SolarSystem* sys, Camera* cam);
void SolarSystem_SortByDistance(SolarSystem* sys, Camera* cam);

// Utilidades
CelestialBody* SolarSystem_GetBodyByName(SolarSystem* sys, const char* name);
CelestialBody* SolarSystem_GetBodyByIndex(SolarSystem* sys, uint8_t index);

#endif // SOLAR_SYSTEM_H
