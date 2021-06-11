/*==================[inlcusiones]============================================*/

#include "main.h"   // <= own header (optional)
#include "sapi.h"     // <= sAPI header
#include "ff.h"       // <= Biblioteca FAT FS
#include "fssdc.h"      // API de bajo nivel para unidad "SDC:" en FAT FS
#include "medicion.h"

/*==================[definiciones y macros]==================================*/

//#define FILENAME "SDC:/hola.txt"
/*==================[definiciones de datos internos]=========================*/

void diskTickHook(void *ptr); // FUNCION que se ejecuta cada vezque ocurre un Tick
void disk_timerproc(void);

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void) {

	// ---------- CONFIGURACIONES ------------------------------
	// Inicializar y configurar la plataforma
	boardConfig();

	// Inicializar y configurar comunicacion UART
//   uartConfig(UART_USB,115200);
	uartConfig(UART_232, 9600);

	// SPI configuration
	spiConfig(SPI0);

	// Inicializar RTC
	rtcInit();
//   rtcWrite( &rtc );
//   rtcRead( &rtc ); // en la variable de estructura rtc te queda la fecha/hora actual

	// Inicializar el conteo de Ticks con resolucion de 10ms,
	// con tickHook diskTickHook
	tickConfig(10);
	tickCallbackSet(diskTickHook, NULL);

	// Initialize SD card driver
	FSSDC_InitSPI();

	// ---------- REPETIR POR SIEMPRE --------------------------
	while ( TRUE) {
		actualizaSistema();
//      sleepUntilNextInterrupt();
	}

	// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
	// directamenteno sobre un microcontroladore y no es llamado/ por ningun
	// Sistema Operativo, como en el caso de un programa para PC.
	return 0;
}

// FUNCION que se ejecuta cada vezque ocurre un Tick
void diskTickHook(void *ptr) {
	disk_timerproc();   // Disk timer process
}

/*==================[fin del archivo]========================================*/
