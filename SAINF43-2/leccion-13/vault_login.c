/*
 * vault_login.c  -  Binario de ejemplo para analisis forense
 * CFT San Antonio - Analisis Forense - Leccion 13
 *
 * OBJETIVO DIDACTICO
 * ------------------
 * Este programa simula un "login" con usuario y contrasena embebidos
 * en el codigo. Es DELIBERADAMENTE inseguro: las credenciales quedan
 * en texto plano dentro del binario para que los alumnos las descubran
 * con `strings`, sin necesidad de ejecutarlo ni de ingenieria inversa.
 *
 * No escala privilegios ni hace nada danino: solo compara texto y
 * muestra mensajes. Los bits SUID/SGID se activan sobre este archivo
 * de ejemplo unicamente para practicar su deteccion (find -perm).
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* --- "Secretos" plantados a proposito para el ejercicio de strings --- */
static const char *BANNER      = "=== Forense Vault 1.3 - Acceso restringido ===";
static const char *VALID_USER  = "admin";
static const char *VALID_PASS  = "S3cr3t0-R0j0-2026";
static const char *API_TOKEN   = "FLAG{strings_revela_los_secretos}";
static const char *DEV_NOTE    = "TODO: mover credenciales a variable de entorno antes de produccion";

int main(void)
{
    char user[64];
    char pass[64];

    puts(BANNER);

    printf("Usuario: ");
    if (!fgets(user, sizeof(user), stdin)) return 1;
    user[strcspn(user, "\n")] = '\0';

    printf("Contrasena: ");
    if (!fgets(pass, sizeof(pass), stdin)) return 1;
    pass[strcspn(pass, "\n")] = '\0';

    if (strcmp(user, VALID_USER) == 0 && strcmp(pass, VALID_PASS) == 0) {
        puts("[+] Acceso concedido. Bienvenido, administrador.");
        printf("[+] Token de sesion: %s\n", API_TOKEN);
        return 0;
    }

    puts("[-] Credenciales invalidas. Acceso denegado.");
    (void)DEV_NOTE; /* referenciado para que no lo elimine el optimizador */
    return 1;
}
