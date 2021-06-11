/*
 * medicion.c
 *
 *  Created on: 3 jun. 2021
 *      Author: nicolas
 */

#include "main.h"
#include "sapi.h"     // <= sAPI header
#include "ff.h"       // <= Biblioteca FAT FS
#include "fssdc.h"      // API de bajo nivel para unidad "SDC:" en FAT FS
#include "medicion.h"      // API de bajo nivel para unidad "SDC:" en FAT FS

void actualizaSistema(void) {
	while (uartReadByte(UART_232, &datoGUI)) {
		switch (datoGUI) {
		case 'a':
			// selecciona medicion BIPOLAR
			setModoBipolar();
			break;
		case 'b':
			// selecciona medicion TETRAPOLAR
			setModoTetrapolar();
			break;
		case 'c':
			// INICIA CALIBRACION
			// calibracion = FALSE -> no calibrado
			// calibracion = 1 -> calibrado
			calMedicion();
			break;
		case 'd':
			// INICIA MEDICION
			// medicion = FALSE -> medicion no realizada
			// medicion = 1 -> medicion realizada
			initVectores(valFrecuencia, valModulo, valFase, valRealZ, valImgZ);
			ejecutaMedicion();
			break;
		case 'e':
			// ENVIO RESULTADOS A PANTALLA
			enviarDatosGUI(valModulo, valFase);
			break;
		case 'f':
			// guarda los datos en memoria SD
			guardaDatosSD();

			break;
		case 'g':
			// FINALIZA SESION. SE PONE EN BAJO CONSUMO
			finalizaSesion();
			break;
		default:

			break;
		}
	}
}

void finalizaSesion(void) {
	gpioWrite(LED2, OFF);
	gpioWrite(LEDG, OFF);
	sleepUntilNextInterrupt();
}

void setModoBipolar(void) {
	gpioWrite(LEDG, OFF);
	gpioWrite(LEDB, OFF);
	gpioWrite(LEDR, OFF);
	gpioWrite(LED1, OFF);
	gpioWrite(LED2, OFF);
	gpioWrite(LED3, OFF);
	modoElegido = FALSE;
	calibracion = FALSE;
	medicion = FALSE;
	gpioWrite(LEDR, ON);
	delay(1000);
	gpioWrite(LEDR, OFF);
}

void setModoTetrapolar(void) {
	gpioWrite(LEDG, OFF);
	gpioWrite(LEDB, OFF);
	gpioWrite(LEDR, OFF);
	gpioWrite(LED1, OFF);
	gpioWrite(LED2, OFF);
	gpioWrite(LED3, OFF);
	modoElegido = TRUE;
	calibracion = FALSE;
	medicion = FALSE;
	gpioWrite(LEDG, ON);
	delay(1000);
	gpioWrite(LEDG, OFF);
}

void calMedicion(void) {
	if (modoElegido == FALSE)	//modo BIPOLAR
	{
		calibracion = TRUE;
	}

	if (modoElegido == TRUE)	//modo TETRAPOLAR
	{
		calibracion = TRUE;
	}
	gpioWrite(LEDG, ON);
	delay(500);
	gpioWrite(LEDG, OFF);
}

void ejecutaMedicion(void) {
	gpioWrite(LEDG, OFF);
	gpioWrite(LEDB, OFF);
	gpioWrite(LEDR, OFF);
	gpioWrite(LED1, OFF);
	gpioWrite(LED2, OFF);
	gpioWrite(LED3, OFF);

	if (calibracion == TRUE) {
		medicion = TRUE;
		gpioWrite(LEDG, ON);
		delay(1000);
		gpioWrite(LEDG, OFF);
	}
}

void enviarDatosGUI(uint32_t * vector2, uint32_t * vector3) {
	gpioWrite(LEDB, ON);
	delay(1000);
	gpioWrite(LEDB, OFF);
	//agregar envio de parametro a pantalla nextion
}

void guardaDatosSD(void) {
	static uint8_t i = 0;		// indice que recorre todos los arrays de datos
	static uint8_t n = 0;		// cantidad de bytes de toda la fila de datos

//	gpioWrite(LEDB,ON);
	gpioWrite(LED2, ON);
	delay(1000);
//	gpioWrite(LEDB,OFF);
	gpioWrite(LED2, ON);

	rtcRead(&rtc); // en la variable de estructura rtc te queda la fecha/hora actual

	if (modoElegido == FALSE) //modo BIPOLAR
	{
		sprintf(filename, "SDC:/BIPOLAR %02d_%02d_%04d %02d-%02d-%02d.csv",
				rtc.mday, rtc.month, rtc.year, rtc.hour, rtc.min, rtc.sec);
	}

	if (modoElegido == TRUE) //modo TETRAPOLAR
	{
		sprintf(filename, "SDC:/TETRAPOLAR %02d_%02d_%04d %02d-%02d-%02d.csv",
				rtc.mday, rtc.month, rtc.year, rtc.hour, rtc.min, rtc.sec);
	}

	if (f_mount(&fs, "SDC:", 0) != FR_OK) {
	}

	if (f_open(&fp, &filename, FA_WRITE | FA_OPEN_APPEND) == FR_OK) {
		f_write(&fp, "frecuencia,moduloZ,faseZ,val_real,val_img\r\n", 43,
				&nbytes);
		f_close(&fp);
	}

	for (i = 0; i < 100; i++) {

		if (f_open(&fp, &filename, FA_WRITE | FA_OPEN_APPEND) == FR_OK) {
//         f_write( &fp, "Hola mundo\r\n", 12, &nbytes );
			n = sprintf(buffer, "%04d,%04d,%04d,%04d,%04d\r\n",
					valFrecuencia[i], valModulo[i], valFase[i], valRealZ[i],
					valImgZ[i]);
			f_write(&fp, buffer, n, &nbytes);

			f_close(&fp);

			if (nbytes == n) {
				// Turn ON LEDG if the write operation was successful
				gpioWrite(LEDG, ON);
			}
		} else {
			// Turn ON LEDR if the write operation was fail
			gpioWrite(LEDR, ON);
		}
	}
}

void initVectores(uint32_t * vector, uint32_t * vector2, uint32_t * vector3,
		uint32_t * vector4, uint32_t * vector5) {
//	static uint32_t	largo = sizeof(vector)/sizeof(uint32_t);
	static uint32_t largo = 100;

	for (uint32_t i = 0; i < largo; i++) {
		vector[i] = 100 * i;
		vector2[i] = i + 2;
		vector3[i] = i * 2;
		vector4[i] = i * 2;
		vector5[i] = i + 4;
	}
}
