/*
______________________________________
* File:
	sdk.h
* Date:
	10/08/2021
* Author:
	rafameou
* Notes:
* Purpose:  
______________________________________
*/

#include <gtk/gtk.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <string.h>
#include <pthread.h>

#ifdef __unix__         
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#elif defined(_WIN32) || defined(WIN32) 
#define NotUnix
#endif
